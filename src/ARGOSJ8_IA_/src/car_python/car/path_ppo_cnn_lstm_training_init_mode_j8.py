
##################################################################
##################################################################
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
flexible_planner_train.py
─────────────────────────
Nodo ROS 2 que entrena online con PPO:
  • Selección robusta del target (goal visible / frontier válido)
  • Génesis de minipaths flexibles (policy CNN + LSTM + búsqueda abanico)
  • Recompensas por progreso, llegada, colisión, seguridad, longitud de ruta
  • Publicación de Path + Markers y topics auxiliares
  • Guardado de pesos por episodio y métricas en TensorBoard
"""

# ==============  IMPORTS  =================================================
import math, random, time, os, pathlib, copy, numpy as np, tensorflow as tf
import rclpy
from rclpy.node import Node
from rclpy.qos  import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from nav_msgs.msg      import Odometry, OccupancyGrid, Path
from geometry_msgs.msg import (PoseArray, PoseStamped,
                               Point, Vector3, Twist)
from visualization_msgs.msg import Marker
from std_msgs.msg       import Header, Bool
from scipy.ndimage      import (binary_dilation,
                                generate_binary_structure,
                                distance_transform_edt)

import datetime
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg
from grid_map_msgs.msg import GridMap
from rclpy.time import Time
from heapq import heappush, heappop
import numpy as np, math
from scipy.interpolate import splprep, splev
from visualization_msgs.msg import Marker, MarkerArray
from std_msgs.msg import ColorRGBA
from scipy.ndimage import zoom 

# ==============  PARÁMETROS GLOBALES  =====================================
PATCH           = 128                       # lado del parche (celdas)
CLEAR_MIN       = 3.4                       # m (holgura waypoint)
GOAL_VIS_OK     = 4                         # ciclos “goal visible” → OK
GOAL_RADIUS     = 3.0                       # m para “goal reached”
RADII           = [2.3, 3.6,5.9]           # radios candidatos
ANGLES          = np.linspace(-math.pi/3,
                               math.pi/3, 11)   # ±60° (10 pasos)
MAX_WPS_EP      = 60
# Velocidades límite
MIN_VEL = 1.0          # m/s  (velocidad mínima deseada)
MAX_VEL = 6.0          # m/s  (velocidad máxima permitida)
LOOK_A  = 2.0    # m (aceptación)
# PPO
ROLLOUT_STEPS   = 2048 
BATCH_SZ        = 256
EPOCHS          = 10
MAX_UPDATES     = 10
GAMMA           = 0.99
GAE_LAMBDA      = 0.95
CLIP_EPS        = 0.2
LR_ACTOR        = 3e-4
LR_CRITIC       = 1e-3
STD_START       = 0.4
STD_MIN         = 0.05
STD_DECAY       = 0.998
MAX_EPISODES = 300 
MAX_TILT = 1.0
MAX_STEERING = 40.0
MAX_THROTTLE = 40.0
WHEEL_BASE = 1.35        # m – distancia ejes
DELTA_MAX  = math.radians(32)   # giro máx. ruedas
K_STEER_LP = 0.7         # filtro exp. en steering
K_THROTTLE_LP = 0.4      # filtro exp. en throttle
KP_SPEED   = 10.0       # PID (solo P)  → throttle
MAX_DTHR   = 2.0        # rampa throttle / ciclo
V_MAX      = 3.0         # m/s (nuevo techo)
V_MIN      = 0.6         # m/s (no stall)
THR_IDLE = 2.0
DECEL_STOP = 0.20
HEIGHT_LAYER = "elevation"               # nombre de la capa de altura
MAX_HEIGHT   =  5.0       
# Terreno / pendiente ───────────────────────────────────────────────
MAX_SLOPE_TAN   = math.tan(math.radians(15.0))   # ≃ 15°  → pendiente límite
SLOPE_COST_GAIN = 20.0                            # peso en la métrica de coste

# ─────────────────────────── stuck / reverse ────────────────────────────
STUCK_DIST_THR   = 0.01     # m – avance mínimo para no considerarlo atascado
STUCK_CYCLES_MAX = 60       # nº de ciclos consecutivos sin progreso
REV_STEPS        = 60       # ciclos de marcha atrás
REV_THROTTLE     = -0.4 * MAX_THROTTLE
REV_STEER_SIGN   = +1      # cambia a −1 si quieres salir girando al otro lado
# ─────────────────────────── scan extra ─────────────────────────────────
UNKNOWN_FRAC_THR = 0.30     # % de celdas -1 en el frontal que dispara giro

REACHED_F_THR = 2.5      # m – tolerancia para considerar “visitada”



WEIGHT_GOAL   = 1.0      # peso de la distancia al goal
WEIGHT_ROBOT  = 0.3      # peso de la distancia al robot
CONE_DEG      = 45       # opcional: sólo frontiers dentro de ±45°


# exploración de way-points
EPS_WP_START  = 0.6      # probabilidad inicial de usar un wp exploratorio
EPS_WP_END    = 0.05     # al final del entrenamiento
EPS_DECAY_STEPS = 3000000
RND_RADIUS_MAX = 3.0     # m  radio máx. para muestreo aleatorio



BETA_SLOPE     = 10.0 

DTYPE = np.float32


RUN_DIR = pathlib.Path.home() /  "PHD" / "ARGOJ8_IA" / "weights"
RUN_DIR.mkdir(exist_ok=True)

# ==============  UTILS GEOMÉTRICOS  =======================================
def l2(a, b):  # distancia euclídea 2-D
    return math.hypot(b[0]-a[0], b[1]-a[1])

def idx_from_world(info, pt):
    res = info.resolution
    return (int((pt[0]-info.origin.position.x)/res),
            int((pt[1]-info.origin.position.y)/res))

def distance(a,b): return math.hypot(b[0]-a[0], b[1]-a[1])


def _angle(a, b, c):
    """ángulo (rad) entre ab y ac"""
    v1 = (b[0]-a[0], b[1]-a[1])
    v2 = (c[0]-a[0], c[1]-a[1])
    dot = v1[0]*v2[0] + v1[1]*v2[1]
    n1  = math.hypot(*v1); n2 = math.hypot(*v2)
    return math.acos(max(-1.0, min(1.0, dot/(n1*n2+1e-9))))

def height_at(arr_h, info_h, pt):
    """
    Altura en ‘pt’ para un GridMap:
      * info_h.pose.position  = centro del mapa
      * longitudes = info_h.length_x/y
    """
    res = info_h.resolution
    ox = info_h.pose.position.x - info_h.length_x / 2.0
    oy = info_h.pose.position.y - info_h.length_y / 2.0
    i = int((pt[0] - ox) / res)
    j = int((pt[1] - oy) / res)
    if 0 <= j < arr_h.shape[0] and 0 <= i < arr_h.shape[1]:
        return float(arr_h[j, i])
    return 0.0

def slope_ok(arr_h, info, a, b):
    """
    Comprueba la pendiente media entre ‘a’ y ‘b’.
    Devuelve (ok:bool, slope:float).
    """
    dz = abs(height_at(arr_h, info, b) - height_at(arr_h, info, a))
    dx = distance(a, b)
    if dx < 1e-6:
        return True, 0.0
    s  = dz / dx
    return s <= MAX_SLOPE_TAN, s

def fuse_dynamic_layer(grid_static, obstacles, info, decay_steps=5):
    grid = grid_static.copy()
    res  = info.resolution
    H, W = grid.shape
    for obs in obstacles:                       # PoseArray con vel.z = radius
        x, y, r = obs.position.x, obs.position.y, obs.orientation.w
        i0 = int((x - info.origin.position.x) / res)
        j0 = int((y - info.origin.position.y) / res)
        rad = int(r / res)
        for dj in range(-rad, rad+1):
            for di in range(-rad, rad+1):
                if di*di+dj*dj <= rad*rad:
                    j, i = j0+dj, i0+di
                    if 0 <= j < H and 0 <= i < W:
                        grid[j, i] = min(100, grid[j, i]+100)  # marca ocupado
    return grid





def gridmap_to_numpy(msg: GridMap, layer=HEIGHT_LAYER):
    """
    Devuelve un np.ndarray (H×W) con la capa pedida.
    Celdas NaN → 0.0   (puedes cambiar la política si prefieres).
    """
    if layer not in msg.layers:
        return None
    idx_layer = msg.layers.index(layer)
    h  = msg.data[idx_layer].layout.dim[1].size
    w  = msg.data[idx_layer].layout.dim[0].size
    buf = np.asarray(msg.data[idx_layer].data, dtype=np.float32)
    arr = buf.reshape((h, w))
    arr = np.nan_to_num(arr, nan=0.0)

    return arr , msg.info


def bres_free(grid, info, a, b):
    """Bresenham + bloqueo: (-1) desconocido ó >=100 obstáculo."""
    i0,j0 = idx_from_world(info,a)
    i1,j1 = idx_from_world(info,b)
    di,dj = abs(i1-i0), abs(j1-j0)
    si = 1 if i0<i1 else -1
    sj = 1 if j0<j1 else -1
    err=di-dj
    H,W = grid.shape
    while True:
        if not (0<=i0<W and 0<=j0<H): return False
        v = grid[j0,i0]
        if v==-1 or v>=100:          return False
        if (i0,j0)==(i1,j1):         return True
        e2=2*err
        if e2>-dj: err-=dj; i0+=si
        if e2< di: err+=di; j0+=sj

def clearance_ok(grid, info, pt, r_m):
    i,j = idx_from_world(info,pt)
    r = int(r_m/info.resolution)
    H,W = grid.shape
    for dj in range(-r, r+1):
        for di in range(-r, r+1):
            x,y = i+di, j+dj
            if 0<=x<W and 0<=y<H and (grid[y,x]==-1 or grid[y,x]>=100):
                return False
    return True


def slope_map(h_arr, info):
    """Devuelve mapa |∇z| normalizado a 0..1."""
    dy, dx = np.gradient(h_arr, info.resolution)
    mag = np.hypot(dx, dy)
    return np.clip(mag / 0.5, 0.0, 1.0)      # 0.5 ≃ 26°

def smooth_bspline(pts, ds=0.25):
    """Suaviza lista de puntos (x,y) con B-Spline y re-muestra cada ds metros."""
    if len(pts) < 3:
        return pts
    x, y = zip(*pts)
    tck, u = splprep([x, y], s=0.3)
    # longitud total aproximada
    dist = sum(math.hypot(x2-x1, y2-y1) for (x1,y1),(x2,y2) in zip(pts, pts[1:]))
    n = max(2, int(dist/ds))
    unew = np.linspace(0, 1, n)
    x_s, y_s = splev(unew, tck)
    return list(zip(x_s, y_s))

def resize_to(arr, shape):
    """Redimensiona 2-D arr a ‘shape’ con interpolación bilineal."""
    sy = shape[0] / arr.shape[0]
    sx = shape[1] / arr.shape[1]
    return zoom(arr, (sy, sx), order=1)



def _motion_primitives(r_min, step, n_head=16):
    """Pre-genera (Δx,Δy,Δθ, cost) para n_head discretos."""
    dθ = 2*math.pi/n_head
    mp = [[] for _ in range(n_head)]
    for h in range(n_head):
        θ = h*dθ
        for δ in (-1, 0, +1):          # izquierda, recto, derecha
            if δ == 0:
                dx = step*math.cos(θ);      dy = step*math.sin(θ)
                mp[h].append( (dx,dy, 0, step) )
            else:                       # arco de radio r_min
                rad = r_min
                ang = step/rad
                dx = rad*(math.sin(δ*ang+θ) - math.sin(θ))
                dy = rad*(-math.cos(δ*ang+θ) + math.cos(θ))
                mp[h].append( (dx,dy, δ*ang, rad*abs(ang)) )
    return mp, dθ

# def hybastar(start_xy, goal_xy, *, grid_cost, res, origin=(0.0, 0.0),
#              r_min=1.0, step=0.4, n_head=16, reverse_ok=False,
#              goal_tol=0.35):
#     """
#     Devuelve lista [(x,y), …] o [] si falla.
#     · origin  = (ox, oy) offset del mapa en mundo.
#     · reverse_ok se ignora en esta versión (solo avance).
#     """
#     H, W = grid_cost.shape
#     mp, dθ = _motion_primitives(r_min, step, n_head)

#     ox, oy = origin
#     to_idx = lambda p: (int((p[0] - ox) / res),
#                         int((p[1] - oy) / res))

#     sx, sy = start_xy
#     gx, gy = goal_xy
#     ix, iy = to_idx((sx, sy))
#     gx_i, gy_i = to_idx((gx, gy))

#     if not (0 <= ix < W and 0 <= iy < H):     # start fuera
#         self.get_logger().warn(f"Start {start_xy} fuera del mapa")
#         return []
#     if not (0 <= gx_i < W and 0 <= gy_i < H): # goal fuera
#         self.get_logger().warn(f"Goal {goal_xy} fuera del mapa")
#         return []

#     Node = lambda x, y, h, g, parent: (x, y, h, g, parent)
#     occ_thr = 200
#     openL, closed = [], {}

#     h0 = int(((math.atan2(gy - sy, gx - sx) + 2*math.pi) % (2*math.pi)) // dθ)
#     n0 = Node(sx, sy, h0, 0.0, None)
#     heappush(openL, (0.0, n0))

#     while openL:
#         f, n = heappop(openL)
#         key = (round((n[0]-ox)/res), round((n[1]-oy)/res), n[2])
#         if key in closed:
#             continue
#         closed[key] = n

#         if math.hypot(n[0] - gx, n[1] - gy) < goal_tol:
#             # reconstruir camino
#             path = [(n[0], n[1])]
#             while n[4]:
#                 n = n[4]
#                 path.append((n[0], n[1]))
#             return path[::-1]

#         # expandir
#         for dx, dy, dth, cost in mp[n[2]]:
#             x2, y2 = n[0] + dx, n[1] + dy
#             ix2, iy2 = to_idx((x2, y2))
#             if not (0 <= ix2 < W and 0 <= iy2 < H):
#                 continue
#             if grid_cost[iy2, ix2] >= occ_thr:
#                 continue
#             h2 = (n[2] + int(round(dth / dθ))) % n_head
#             g2 = n[3] + cost + 0.01 * grid_cost[iy2, ix2]   # coste terreno
#             h_est = math.hypot(x2 - gx, y2 - gy)
#             heappush(openL, (g2 + h_est, Node(x2, y2, h2, g2, n)))

#     # ---------- fallback recto si la línea es libre ----------
#     if (0 <= gx_i < W and 0 <= gy_i < H
#             and 0 <= ix < W and 0 <= iy < H
#             and np.all(grid_cost[min(iy, gy_i):max(iy, gy_i)+1,
#                                 min(ix, gx_i):max(ix, gx_i)+1] < occ_thr)):
#         return [start_xy, goal_xy]

#     return []
# # ──────────────
# ─────────────────────────  RRT LIGHT  ──────────────────────────────
class _RRTNode:
    __slots__ = ("x", "y", "parent")
    def __init__(self, x, y, parent):
        self.x, self.y, self.parent = x, y, parent


def _sample_free(info, grid, goal, goal_bias=0.15):
    """Con prob.=goal_bias devuelve el propio goal; si no, un punto aleatorio
    dentro del bbox del mapa cuya celda sea libre (<50)."""
    if random.random() < goal_bias:
        return goal
    H, W = grid.shape
    for _ in range(100):                     # 100 intentos máx.
        i = random.randint(0, W-1)
        j = random.randint(0, H-1)
        if grid[j, i] < 50:                  # celda libre
            x = info.origin.position.x + (i+0.5)*info.resolution
            y = info.origin.position.y + (j+0.5)*info.resolution
            return (x, y)
    return goal                              # fallback


def _nearest(tree, pt):
    """Devuelve el nodo del árbol más cercano a pt (en L2)."""
    return min(tree, key=lambda n: distance((n.x, n.y), pt))


def _steer(from_pt, to_pt, max_step):
    """Recorta el vector hasta longitud ≤ max_step."""
    dx, dy = to_pt[0]-from_pt[0], to_pt[1]-from_pt[1]
    d = math.hypot(dx, dy)
    if d <= max_step:
        return to_pt
    k = max_step / d
    return (from_pt[0] + k*dx, from_pt[1] + k*dy)

def bres_line_free(grid, info, a, b):
    """True si el rayo a→b no atraviesa celdas ≥50  (incluye “inflado”)."""
    for i, j in bresenham_points(a, b, info):
        if grid[j, i] >= 50:         # antes sólo mirabas 100
            return False
    return True

def rrt_plan(start, goal, grid, info,
             h_arr=None, hm_info=None,
             max_iter=1200,  step=1.0,  goal_tol=0.8):
    """
    RRT goal-biased muy ligero.  Devuelve lista [(x,y), …] o [] si falla.
    * step se da en metros.
    * Filtra obstáculos (grid ≥50) y pendiente>30° si se pasa h_arr.
    """
    if distance(start, goal) < goal_tol:
        return [start, goal]

    tree = [_RRTNode(start[0], start[1], None)]
    for _ in range(max_iter):
        rnd = _sample_free(info, grid, goal)
        nearest = _nearest(tree, rnd)
        new_pt = _steer((nearest.x, nearest.y), rnd, step)

        # ---- colisión y pendiente ----------------------------------
        if not bres_line_free(grid, info, (nearest.x, nearest.y), new_pt):
            continue
        if not clearance_ok(grid, info, new_pt, CLEAR_MIN):
            continue
        if h_arr is not None:
            _, slope = slope_ok(h_arr, hm_info, (nearest.x, nearest.y), new_pt)
            if slope > math.tan(math.radians(30)):
                continue

        new_node = _RRTNode(new_pt[0], new_pt[1], nearest)
        tree.append(new_node)

        # ¿hemos llegado?
        if distance(new_pt, goal) < goal_tol:
            # reconstruye desde goal→start
            path = [(goal[0], goal[1])]
            n = new_node
            while n:
                path.append((n.x, n.y))
                n = n.parent
            return path[::-1]

    return []            # agotado el número de iteraciones

# ─── utils extra ─────────────────────────────────────────────────────────
def bresenham_points(a, b, info):
    """
    Genera los índices (i,j) de todas las celdas que une el segmento a→b
    usando Bresenham.  Se emplea para calcular costes de “rozar” obstáculos.
    """
    i0, j0 = idx_from_world(info, a)
    i1, j1 = idx_from_world(info, b)
    di, dj = abs(i1 - i0), abs(j1 - j0)
    si = 1 if i0 < i1 else -1
    sj = 1 if j0 < j1 else -1
    err = di - dj
    while True:
        yield i0, j0
        if (i0, j0) == (i1, j1):
            break
        e2 = 2 * err
        if e2 > -dj:
            err -= dj
            i0 += si
        if e2 < di:
            err += di
            j0 += sj


def clearance(grid, info, pt, max_r=5.0):
    """
    Devuelve la distancia (m) libre desde `pt` hasta la celda ≥50 (o -1
    si en un radio `max_r` no hay nada).  Se usa para bonificar holgura.
    """
    i, j = idx_from_world(info, pt)
    r_lim = int(max_r / info.resolution)
    for r in range(1, r_lim + 1):
        ring = grid[max(j - r, 0): j + r + 1,
                    max(i - r, 0): i + r + 1]
        if np.any(ring >= 50):
            return r * info.resolution
    return -1.0



# ==============  RED CNN + LSTM  ==========================================
def build_policy():
    g   = tf.keras.Input(shape=(PATCH,PATCH,2), name="grid")
    st  = tf.keras.Input(shape=(7,),            name="state")
    w0  = tf.keras.Input(shape=(2,),            name="wp0")
    # CNN
    x = tf.keras.layers.Conv2D(16,3,padding="same",activation="relu")(g)
    x = tf.keras.layers.MaxPooling2D()(x)
    x = tf.keras.layers.Conv2D(32,3,padding="same",activation="relu")(x)
    x = tf.keras.layers.GlobalAveragePooling2D()(x)
    z = tf.keras.layers.Concatenate()([x,st])
    # LSTM un paso
    h0 = tf.keras.layers.Dense(128,activation="tanh")(z)
    c0 = tf.keras.layers.Dense(128,activation="tanh")(z)
    lstm = tf.keras.layers.LSTMCell(128)
    h1,_ = lstm(w0,[h0,c0])
    delta = tf.keras.layers.Dense(3,activation="tanh")(h1)
    return tf.keras.Model([g,st,w0], delta, name="policy")

# ==============  NODO PRINCIPAL  ==========================================
class FlexPlanner(Node):
    def __init__(self):
        super().__init__("flexible_trainer")

        # --- Subscripciones
        qos=10
        self.create_subscription(Odometry,      "/ARGJ801/odom_demo",            self.cb_odom,     qos)
        self.create_subscription(PoseArray,     "/goal",            self.cb_goal,     qos)
        self.create_subscription(OccupancyGrid, "/occupancy_grid",  self.cb_grid,     10)
        #self.create_subscription(PoseArray,     "/safe_frontier_points_centroid",self.cb_frontier, qos)
        self.create_subscription(Bool,"/virtual_collision", self.cb_collision, qos)
        self.create_subscription(Bool,"/reset_confirmation",self.cb_reset_conf,qos)
        self.create_subscription(Bool,"/goal_reached",self.cb_goal_reached,qos)
        #self.create_subscription(PoseArray, "/obstacle_navigation_nodes_lidar",self.cb_obstacles, 10)
        self.create_subscription(GridMap,"/terrain_grid",self.cb_heightmap, 10)
        self.create_subscription(PoseArray, "/safe_frontier_points", self.cb_frontier, qos)

        # --- Publicadores
        self.path_pub  = self.create_publisher(Path,  "/global_path_predicted", qos)
        self.wps_pub   = self.create_publisher(Marker,"/path_waypoints_marker", qos)
        self.bad_pub  = self.create_publisher(Marker, "/frontiers_bad", 10)
        self.vis_pub  = self.create_publisher(Marker, "/frontiers_visited", 10)
        self.mk_vis_pub = self.create_publisher(
        Marker, "/frontiers_visited_marker", 10)
        self.mk_bad_pub = self.create_publisher(
                Marker, "/frontiers_bad_marker",     10)
        self.goal_pub  = self.create_publisher(Bool,  "/goal_reached", qos)
        latched=QoSProfile(depth=1,
                           reliability=ReliabilityPolicy.RELIABLE,
                           durability=DurabilityPolicy.TRANSIENT_LOCAL)
        self.reset_pub = self.create_publisher(Bool,  "/reset_request", latched)
        self.cmd_pub   = self.create_publisher(CmdThrottleMsg, "/ARGJ801/cmd_throttle_msg", qos)
        self.debug_wp_pub = self.create_publisher(Marker, "/debug_waypoints", 10)

        # --- Estado ROS
        self.waiting_reset=False
        self.current_path = []     # lista de waypoints activos
        self.wp_index     = 1      # índice del wp que se está siguiendo
        self.pose=self.twist=None
        self.goal=None
        self.grid_msg=None
        self.frontiers=[]
        self.obstacles = []  
        self.collided=False
        self.ready = True 
        self.reset_t0=None
        self.episode_done=False
        self.ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        self.update_done=0
        self.max_seg  =0.6
        self.max_steps=60
        self.dev_thr  =0.8
        self.clear_min=0.4     # m
        self.max_seg_length      = 1.0
        self.reach_thr=0.4
        self.goal_reached_flag=False
        self.scanning     = False      # ← NUEVO
        self.scan_t0      = None
        self.SCAN_SECS    = 1.5        # duración de un barrido 360°
        self.height_map_msg = None
        #  —  estado marcha atrás
        self.stuck_counter     = 0
        self.reversing         = False
        self.rev_steps_left    = 0
        self.training = False

        ### list frontier delete
        self.bad_frontiers : list[tuple[float,float]] = []   
        self.visited_frontiers  : list[tuple[float, float]] = []

        self.declare_parameter("beta_slope", 10.0)
        self.beta_slope = self.get_parameter("beta_slope").value

        ###########
        self.map_max_age_sec = 0.5      # ← umbral; ajusta a tu ritmo de /occupancy_grid
        self._grid_stamp     = None

        self.active_target = None  # (x,y) del target activo
        self.total_steps = 0

        self.last_cmd = CmdThrottleMsg()

        # --- Red y PPO
        self.policy = build_policy()
        self.log_std = tf.Variable(np.log(STD_START*np.ones(3,np.float32)),
                                   trainable=True)
        self.opt_actor  = tf.keras.optimizers.Adam(LR_ACTOR)
        self.opt_critic = tf.keras.optimizers.Adam(LR_CRITIC)
        self.value_net  = tf.keras.Sequential([
            tf.keras.layers.Input(shape=(39,)),     # 32 (patch emb.) + 6 (state)
            tf.keras.layers.Dense(128,activation="tanh"),
            tf.keras.layers.Dense(1)
        ])

        # TensorBoard
        self.writer=tf.summary.create_file_writer(str(RUN_DIR/
                                                      time.strftime("run_%Y%m%d_%H%M%S")))
        # Buffers PPO
        self.reset_buffers()

        # Episodios
        self.episode=0
        self.goal_counter=0
        self.goals_in_world=random.randint(5,7)
        self.goal_vis=0

        self.create_timer(0.1, self.step)
        self.get_logger().info("Flexible planner + PPO listo")

    # ---------- Callbacks ROS ----------
    def cb_goal_reached(self, msg: Bool):
        if not msg.data:
            return
        # descarta si acabamos de reiniciar (<2 s) o hay colisión activa
        if (self.reset_t0 and
            (self.get_clock().now() - self.reset_t0).nanoseconds < 2e9):
            return
        if self.collided:
            return
        self.goal_reached_flag = True
    def cb_odom(self,m):
        self.pose=m.pose.pose; self.twist=m.twist.twist
    # def cb_obstacles(self, msg: PoseArray):
    #     self.obstacles = msg.poses

    def cb_goal(self,m):
        self.goal=m.poses[0] if m.poses else None
    # def cb_grid(self,m):
    #     self.grid_msg=m

    def cb_heightmap(self, msg: GridMap):
        """Guarda el GridMap de terreno más reciente."""
        self.height_map_msg = msg

    def _too_close(self, p, q, tol=1.2):
        return l2(p, q) < tol            # l2() ya la tienes definida

    def cb_grid(self, m: OccupancyGrid):
        # mezclamos capa estática + móviles
        static_arr = np.array(m.data, dtype=np.int8).reshape((m.info.height,
                                                            m.info.width))
        if self.obstacles:
            dyn_arr = fuse_dynamic_layer(static_arr, self.obstacles, m.info)
            self.grid_dyn = dyn_arr                    # guarda dinámico
        else:
            self.grid_dyn = static_arr
        self.grid_msg = m                              # conserva original
        self._grid_stamp = Time.from_msg(m.header.stamp)  # guarda el timestamp del grid

    def cb_frontier(self,m):
        self.frontiers=[(p.position.x,p.position.y) for p in m.poses]
    def cb_collision(self,m):
        self.collided = bool(m.data)
    # def cb_reset_conf(self, msg:Bool):
    #     if msg.data:
    #         self.waiting_reset = False
    #         self.get_logger().info("[Reset] confirmado por el supervisor")
    #         self.goal_counter = 0
    #         self.goals_in_world = random.randint(5,7)
    #         self.collided = False
    #         self.reset_buffers()

    def cb_reset_conf(self, msg: Bool):
        if msg.data:
            self.waiting_reset = False
            self.ready         = False          # ← PAUSA hasta que el mundo cargue
            self.reset_t0      = self.get_clock().now()
            self.goal_counter  = 0
            self.goals_in_world = random.randint(5, 7)
            self.collided      = False
            # self.bad_frontiers.clear()
            # self.visited_frontiers.clear()
            self.bad_pub.publish(PoseArray())    # lista vacía
            self.vis_pub.publish(PoseArray())
            self.reset_buffers()
            self.get_logger().info("[Reset] confirmado por el supervisor")

    def _near(self, p, q, tol=1.2):
        return l2(p, q) < tol

    # --------------------------------------------------------------
    #  Mantiene el vehículo quieto compensando la pendiente
    # --------------------------------------------------------------
    def _hold_position(self):
        """
        Publica repetidamente un CmdThrottleMsg para contrarrestar
        cualquier velocidad residual cuando el robot *debe* estar parado
        (p.ej. durante el entrenamiento PPO tras llegar al goal).
        · Lee la velocidad lineal real            -> v_real  (m/s)
        · Aplica un control P  (con limitación)   -> throttle
        · Steering a 0     (para no cambiar rumbo)
        """
        v_real = math.hypot(self.twist.linear.x, self.twist.linear.y)

        KP_HOLD = 80.0                 # afinado empírico
        thr_cmd = -KP_HOLD * v_real    # signo opuesto a la velocidad
        thr_cmd = np.clip(thr_cmd,
                          -0.40 * MAX_THROTTLE,   # freno motor hacia atrás
                           0.40 * MAX_THROTTLE)   # freno motor hacia delante

        msg = CmdThrottleMsg()
        msg.throttle = thr_cmd
        msg.steering = 0.0
        self.cmd_pub.publish(msg)
        self.last_cmd = msg



    def _brake(self):
        msg = CmdThrottleMsg(); msg.throttle = 0.0; msg.steering = 0.0
        return msg
    
    def _reverse_cmd(self):
        msg = CmdThrottleMsg()
        msg.throttle  = REV_THROTTLE
        msg.steering  = REV_STEER_SIGN * 0.35 * MAX_STEERING

        return msg

    def _front_unknown_fraction(self, patch_occ):
        """
        Devuelve fracción de celdas desconocidas (-1) en la franja frontal
        (filas [R-4:R+4], columnas [R:PATCH]).
        """
        R = PATCH//2
        front = patch_occ[R-4:R+4, R:]
        total = front.size
        unknown = np.count_nonzero(front == -1)
        return unknown / total if total else 0.0


    def _spin_in_place(self, sign=+1):
        msg = CmdThrottleMsg(); msg.throttle = 0.0
        msg.steering = sign * 0.85 * MAX_STEERING
        self.get_logger().info(f"[SPIN] {sign:+d}")
        return msg

    @staticmethod
    def _wrap_angle(a):
        return (a + math.pi) % (2*math.pi) - math.pi


    def _yaw_from_quaternion(self, q):
        """Devuelve yaw (rad) desde geometry_msgs.msg.Quaternion."""
        siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
        cosy_cosp = 1.0 - 2.0 * (q.y*q.y + q.z*q.z)
        return math.atan2(siny_cosp, cosy_cosp)

    def _roll_pitch_from_quaternion(self, q):
        """Devuelve (roll, pitch) en radianes."""
        sinr_cosp = 2*(q.w*q.x + q.y*q.z)
        cosr_cosp = 1 - 2*(q.x*q.x + q.y*q.y)
        roll  = math.atan2(sinr_cosp, cosr_cosp)

        sinp = 2*(q.w*q.y - q.z*q.x)
        pitch = math.asin(max(-1.0, min(1.0, sinp)))   # clamp numérico
        return roll, pitch



    def _global_to_local(self, dx, dy, yaw):
        """Convierte ΔX,ΔY de frame 'map' a 'base_link'."""
        cos_y, sin_y = math.cos(-yaw), math.sin(-yaw)
        return dx*cos_y - dy*sin_y, dx*sin_y + dy*cos_y


    def _list_to_posearray(self, lst, frame_id="map"):
        pa = PoseArray()
        pa.header.frame_id = frame_id
        pa.header.stamp = self.get_clock().now().to_msg()
        for x, y in lst:
            pose = PoseStamped().pose    # truco rápido para un Pose vacío
            pose.position.x, pose.position.y = x, y
            pose.orientation.w = 1.0
            pa.poses.append(pose)
        return pa


    def entry_point_in_grid(self,cp, fp, info):
        """
        Dado cp=(x0,y0) y fp=(x1,y1), devuelve el primer punto
        dentro del bbox del grid [x0,x1]×[y0,y1] por donde pasa
        el segmento cp→fp, o None si no intersecta.
        Usa el algoritmo de Liang–Barsky.
        """
        x0 = info.origin.position.x
        y0 = info.origin.position.y
        x1 = x0 + info.width  * info.resolution
        y1 = y0 + info.height * info.resolution

        dx = fp[0] - cp[0]
        dy = fp[1] - cp[1]

        p = [-dx,  dx, -dy,  dy]
        q = [ cp[0]-x0, x1-cp[0], cp[1]-y0, y1-cp[1]]

        u0, u1 = 0.0, 1.0
        for pi, qi in zip(p, q):
            if pi == 0:
                if qi < 0:
                    return None  # paralelo y completamente fuera
            else:
                t = qi / pi
                if pi < 0:
                    u0 = max(u0, t)
                else:
                    u1 = min(u1, t)
        if u0 > u1:
            return None

        # punto de entrada al grid:
        return (cp[0] + u0*dx, cp[1] + u0*dy)

    # ---------- Parche local -----------
    # def extract_patch(self):
    #     info=self.grid_msg.info
    #     H,W=info.height,info.width
    #     # arr=np.array(self.grid_msg.data,dtype=np.int8).reshape((H,W))
    #     arr = self.grid_dyn.copy() # capa dinámica #nueva
    #     cp=(self.pose.position.x,self.pose.position.y)
    #     ci=int((cp[0]-info.origin.position.x)/info.resolution)
    #     cj=int((cp[1]-info.origin.position.y)/info.resolution)
    #     i_lo,i_hi=ci-PATCH//2,ci+PATCH//2
    #     j_lo,j_hi=cj-PATCH//2,cj+PATCH//2
    #     i0,i1=max(i_lo,0),min(i_hi,W)
    #     j0,j1=max(j_lo,0),min(j_hi,H)
    #     patch=arr[j0:j1,i0:i1]
    #     pad=((j0-j_lo,j_hi-j1),(i0-i_lo,i_hi-i1))
    #     patch=np.pad(patch,pad,'constant',constant_values=-1)
    #     norm=((patch+1)/101.0).astype(np.float32)   # [-1,100] → [0,1]
    #     return np.expand_dims(norm,-1), arr, info
    def hybastar(self,start_xy, goal_xy, *, grid_cost, res, origin=(0.0, 0.0),
                r_min=1.0, step=0.4, n_head=16, reverse_ok=False,
                goal_tol=0.35):
        """
        Devuelve lista [(x,y), …] o [] si falla.
        · origin  = (ox, oy) offset del mapa en mundo.
        · reverse_ok se ignora en esta versión (solo avance).
        """
        H, W = grid_cost.shape
        mp, dθ = _motion_primitives(r_min, step, n_head)

        ox, oy = origin
        to_idx = lambda p: (int((p[0] - ox) / res),
                            int((p[1] - oy) / res))

        sx, sy = start_xy
        gx, gy = goal_xy
        ix, iy = to_idx((sx, sy))
        gx_i, gy_i = to_idx((gx, gy))

        if not (0 <= ix < W and 0 <= iy < H):     # start fuera
            self.get_logger().warn(f"Start {start_xy} fuera del mapa")
            return []
        if not (0 <= gx_i < W and 0 <= gy_i < H): # goal fuera
            self.get_logger().warn(f"Goal {goal_xy} fuera del mapa")
            return []

        Node = lambda x, y, h, g, parent: (x, y, h, g, parent)
        occ_thr = 200
        openL, closed = [], {}

        h0 = int(((math.atan2(gy - sy, gx - sx) + 2*math.pi) % (2*math.pi)) // dθ)
        n0 = Node(sx, sy, h0, 0.0, None)
        heappush(openL, (0.0, n0))

        while openL:
            f, n = heappop(openL)
            key = (round((n[0]-ox)/res), round((n[1]-oy)/res), n[2])
            self.get_logger().debug(f"Explorando nodo {key} f={f:.2f} g={n[3]:.2f}")
            if key in closed:
                continue
            closed[key] = n

            if math.hypot(n[0] - gx, n[1] - gy) < goal_tol:
                self.get_logger().info(f"Goal alcanzado: {n[0]:.2f}, {n[1]:.2f}")
                self.get_logger().debug(f"Coste total: {n[3]:.2f}")
                # reconstruir camino
                path = [(n[0], n[1])]
                while n[4]:
                    n = n[4]
                    path.append((n[0], n[1]))

                return path[::-1]

            # expandir
            for dx, dy, dth, cost in mp[n[2]]:
                x2, y2 = n[0] + dx, n[1] + dy
                ix2, iy2 = to_idx((x2, y2))
                if not (0 <= ix2 < W and 0 <= iy2 < H):
                    continue
                if grid_cost[iy2, ix2] >= occ_thr:
                    continue
                h2 = (n[2] + int(round(dth / dθ))) % n_head
                g2 = n[3] + cost + 0.01 * grid_cost[iy2, ix2]   # coste terreno
                h_est = math.hypot(x2 - gx, y2 - gy)
                heappush(openL, (g2 + h_est, Node(x2, y2, h2, g2, n)))

        # ---------- fallback recto si la línea es libre ----------
        if (0 <= gx_i < W and 0 <= gy_i < H
                and 0 <= ix < W and 0 <= iy < H
                and np.all(grid_cost[min(iy, gy_i):max(iy, gy_i)+1,
                                    min(ix, gx_i):max(ix, gx_i)+1] < occ_thr)):
            return [start_xy, goal_xy]

        return []
    # ──────────────


    def extract_patch(self):
        """Devuelve (patch_2ch, full_occ, info). patch shape = PATCH×PATCH×2."""
        # -------- occupancy (canal 0) ----------
        info = self.grid_msg.info
        H, W = info.height, info.width
        occ_full = self.grid_dyn.copy()                 # (H×W) int8
        cp = (self.pose.position.x, self.pose.position.y)
        ci = int((cp[0] - info.origin.position.x) / info.resolution)
        cj = int((cp[1] - info.origin.position.y) / info.resolution)
        i_lo, i_hi = ci - PATCH//2, ci + PATCH//2
        j_lo, j_hi = cj - PATCH//2, cj + PATCH//2
        i0, i1 = max(i_lo, 0), min(i_hi, W)
        j0, j1 = max(j_lo, 0), min(j_hi, H)
        occ_patch = occ_full[j0:j1, i0:i1]
        pad = ((j0 - j_lo, j_hi - j1), (i0 - i_lo, i_hi - i1))
        occ_patch = np.pad(occ_patch, pad, 'constant', constant_values=-1)
        norm_occ = ((occ_patch + 1) / 101.0).astype(np.float32)      #  [-1,100]→[0,1]

        # -------- height (canal 1) ------------
        # lienzo vacío del mismo tamaño que occ_patch
        norm_h = np.zeros_like(norm_occ, dtype=np.float32)

        if self.height_map_msg is not None:
            # ① convierto todo el GridMap a np.array
            h_full,hm_info = gridmap_to_numpy(self.height_map_msg)
            if h_full is not None:
                
                res_h   = hm_info.resolution
                ox_h    = hm_info.pose.position.x - hm_info.length_x / 2.0
                oy_h    = hm_info.pose.position.y - hm_info.length_y / 2.0
                Hh, Wh  = h_full.shape

                # ③ recorremos cada celda del lienzo y copiamos valor si existe
                for jj in range(PATCH):
                    y_world = cp[1] + (jj - PATCH//2) * info.resolution
                    jh = int((y_world - oy_h) / res_h)
                    if not (0 <= jh < Hh):       # fuera -> queda 0
                        continue
                    for ii in range(PATCH):
                        x_world = cp[0] + (ii - PATCH//2) * info.resolution
                        ih = int((x_world - ox_h) / res_h)
                        if 0 <= ih < Wh:
                            norm_h[jj, ii] = np.clip(
                                h_full[jh, ih] / MAX_HEIGHT, -1.0, 1.0
                            )

        patch_2ch = np.stack([norm_occ, norm_h], axis=-1)            # (PATCH,PATCH,2)
        return patch_2ch, occ_full, info


    # ---------- Target robusto ----------
    # def choose_target(self, cp, grid, info):
    #     # 1) goal directo si está definido y visible
    #     if self.goal:
    #         gp = (self.goal.position.x, self.goal.position.y)
    #         if bres_free(grid, info, cp, gp):
    #             return gp, "GOAL"

    #     # 2) cualquier frontier
    #     if self.frontiers:
    #         best = min(self.frontiers, key=lambda f: l2(f, gp if self.goal else cp))
    #         return best, "FRONTIER"

    #     # 3) nada de nada
    #     return None, "NONE"

    def choose_target(self, cp, grid, info):
        # 1) goal visible
        if self.goal:
            gp = (self.goal.position.x, self.goal.position.y)
            if bres_free(grid, info, cp, gp):
                return gp, "GOAL"
        else:
            gp = None

        # 2) frontiers recibidos (sin los malos / visitados)
        if not self.frontiers:
            return None, "NONE"

        remaining = [f for f in self.frontiers
                    if f not in self.bad_frontiers
                    and f not in self.visited_frontiers]

        if not remaining:
            return None, "NONE"

        #
        if gp:                                    # hay goal definido
            best = min(remaining, key=lambda f: l2(f, gp))
        else:                                     # sin goal → al más cercano al robot
            best = min(remaining, key=lambda f: l2(f, cp))
        return best, "FRONTIER"

    def recovery(self, cp, grid, info):
        # 1) giro 180° buscando hueco
        for _ in range(20):
            self.cmd_pub.publish(self._spin_in_place(+1))
            rclpy.spin_once(self, timeout_sec=0.05)
            if clearance_ok(grid, info, cp, CLEAR_MIN*1.2):
                return True
        # 2) Hybrid-A* en reversa 2 m
        yaw = self._yaw_from_quaternion(self.pose.orientation)
        rev_goal = (cp[0] - 2*math.cos(yaw), cp[1] - 2*math.sin(yaw))
        rev_path = self.hybastar(cp, rev_goal,
                            grid_cost=grid,
                            res=info.resolution,
                            origin=(info.origin.position.x,
                                    info.origin.position.y),
                            r_min=1.2,
                            step=0.3,
                            reverse_ok=True)
        if rev_path:
            for p in smooth_bspline(rev_path):
                self.cmd_pub.publish(self._reverse_cmd())
                rclpy.spin_once(self, timeout_sec=0.05)
            return True
        return False




    # def bres_line_free(self,grid, info, a, b):
    #     def idx(p):
    #         return (int((p[0]-info.origin.position.x)/info.resolution),
    #                 int((p[1]-info.origin.position.y)/info.resolution))
    #     i0,j0 = idx(a); i1,j1 = idx(b)
    #     di,dj = abs(i1-i0), abs(j1-j0); si = 1 if i0<i1 else -1; sj = 1 if j0<j1 else -1
    #     err = di-dj; H,W = grid.shape
    #     while True:
    #         if not (0<=i0<W and 0<=j0<H) or grid[j0,i0] == -1 or grid[j0,i0] >= 100:
    #             return False
    #         if (i0,j0)==(i1,j1): return True
    #         e2=2*err
    #         if e2>-dj: err-=dj; i0+=si
    #         if e2< di: err+=di; j0+=sj






    def densify(self, path):
        out = [path[0]]
        for a, b in zip(path, path[1:]):
            d = distance(a, b)
            if d > self.max_seg_length:
                steps = int(math.ceil(d / self.max_seg_length))
                for i in range(1, steps):
                    t = i / steps
                    out.append((a[0]*(1-t)+b[0]*t, a[1]*(1-t)+b[1]*t))
            out.append(b)
        return out
    
    # def generate_flexible_path(self, start, target, grid, info):
    #     path=[start]
    #     cp=start
    #     step_len=self.max_seg
    #     angles=np.linspace(-math.pi/3, math.pi/3, 10)   # ±60°
    #     radii=[step_len*0.5, step_len, step_len*1.5]

    #     for _ in range(self.max_steps):
    #         best=None; best_cost=float("inf")
    #         vec_t=(target[0]-cp[0], target[1]-cp[1])
    #         ang0=math.atan2(vec_t[1], vec_t[0])

    #         for r in radii:
    #             for a_off in angles:
    #                 ang=ang0 + a_off
    #                 cand=(cp[0]+r*math.cos(ang), cp[1]+r*math.sin(ang))

    #                 # ---------- filtro ③  “celda debe ser conocida y libre” ----------
    #                 i = int((cand[0] - info.origin.position.x) / info.resolution)
    #                 j = int((cand[1] - info.origin.position.y) / info.resolution)
    #                 if not (0 <= i < grid.shape[1] and 0 <= j < grid.shape[0]):
    #                     continue                             # fuera de mapa
    #                 if grid[j, i] == -1 or grid[j, i] >= 100:
    #                     continue                             # desconocida u obstáculo
    #                 # -----------------------------------------------------------------

    #                 if not self.bres_line_free(grid, info, cp, cand):
    #                     continue
    #                 if not clearance_ok(grid, info, cand, self.clear_min):
    #                     continue

    #                 cost = distance(cand, target) - 0.5*self.clear_min
    #                 if cost < best_cost:
    #                     best_cost, best = cost, cand

    #         if best is None:
    #             break
    #         path.append(best)
    #         cp = best
    #         if distance(cp, target) < self.reach_thr:
    #             break

    #     path.append(target)
    #     return self.densify(path)       



    # def generate_flexible_path(self, start, target, grid, info):
    #     """Como antes, pero descarta y penaliza pendientes > MAX_SLOPE_TAN."""
    #     h_arr, hm_info= gridmap_to_numpy(self.height_map_msg) if self.height_map_msg else (None,None)

    #     path = [start]
    #     cp   = start
    #     step_len = self.max_seg
    #     angles = np.linspace(-math.pi/8,  math.pi/8, 10)     # ±22.5°
    #     radii  = [step_len*0.5, step_len, step_len*1.5]

    #     for _ in range(self.max_steps):
    #         best = None
    #         best_metric = (float("inf"), float("inf"))   # (slope, dist)


    #         vec_t = (target[0]-cp[0], target[1]-cp[1])
    #         ang0  = math.atan2(vec_t[1], vec_t[0])

    #         for r in radii:
    #             for a_off in angles:
    #                 ang  = ang0 + a_off
    #                 cand = (cp[0] + r*math.cos(ang),
    #                         cp[1] + r*math.sin(ang))

    #                 i, j = idx_from_world(info, cand)
    #                 if not (0 <= i < grid.shape[1] and 0 <= j < grid.shape[0]):
    #                     continue
    #                 if grid[j, i] == -1 or grid[j, i] >= 100:
    #                     continue
    #                 if not self.bres_line_free(grid, info, cp, cand):
    #                     continue
    #                 if not clearance_ok(grid, info, cand, self.clear_min):
    #                     continue

    #                 # ---------- pendiente ----------
    #                 if h_arr is not None:
    #                     ok_slope, slope = slope_ok(h_arr, hm_info, cp, cand)
    #                     if slope > math.tan(math.radians(30)): # cambio pasa todo except 30 o alguno peligroso
    #                         continue                     # demasiado empinado
    #                 else:
    #                     slope = 0.0
    #                 # clear = distance(cand,cp)
    #                 # C_OBS = 4.0
    #                 # # cost = (distance(cand, target)
    #                 # #         + SLOPE_COST_GAIN * s        # penaliza subir/bajar
    #                 # #         - 0.5 * self.clear_min)
    #                 # cost = (distance(cand, target)
    #                 #          + SLOPE_COST_GAIN * s
    #                 #          + C_OBS / (clear + 1e-3)) 
    #                 # if cost < best_cost:
    #                 #     best_cost, best = cost, cand
    #                 dist    = distance(cand, target)
    #                 metric  = (slope, dist)          # primero slope, luego dist
    #                 if metric < best_metric:
    #                     best_metric, best = metric, cand

    #         if best is None:
    #             break
    #         path.append(best)
    #         cp = best
    #         if distance(cp, target) < self.reach_thr:
    #             break

    #     path.append(target)
    #     return self.densify(path)

    def _eps_wp(self):
        t = min(1.0, self.total_steps / EPS_DECAY_STEPS)
        return EPS_WP_START + t * (EPS_WP_END - EPS_WP_START)



    def generate_flexible_path(self, start, target, grid, info):
        """
        Intento ➊: RRT goal-biased rápido y seguro.
        Si falla → intento ➋: el generador “fan + policy” que ya tienes.
        """
        h_arr, hm_info = (gridmap_to_numpy(self.height_map_msg)
                        if self.height_map_msg else (None, None))

        # ---------- ➊  RRT -------------------------------------------------
        path_rrt = rrt_plan(start, target,
                            grid, info,
                            h_arr=h_arr, hm_info=hm_info,
                            max_iter=1000, step=0.8, goal_tol=self.reach_thr)

        #  Guarda cada segmento del árbol aceptado como transición “experta”
        for a, b in zip(path_rrt[:-1], path_rrt[1:]):
            # 1) parche centrado en `a`
            self.pose.position.x, self.pose.position.y = a   # truco rápido
            patch, _, _ = self.extract_patch()
            # 2) estado de 7 elementos
            st = np.zeros(7, np.float32)
            st[-2:] = b[0]-a[0], b[1]-a[1]          # vector deseado
            # 3) acción = Δ normalizado
            d = distance(a, b) + 1e-6
            act = np.array([(b[0]-a[0])/d,
                            (b[1]-a[1])/d,
                            0.0], dtype=np.float32)
            # 4) push a los buffers como si fuesen “roll-outs”
            self.patch_buf.append(patch.astype(DTYPE))
            self.state_buf.append(st)
            self.act_buf.append(act)
            self.logp_buf.append(0.0)               # log-π(Fake)=0
            self.rew_buf.append( 0.0 )              # sin recompensa
            self.val_buf.append( 0.0 )
            self.done_buf.append(False)


        if len(path_rrt) >= 3:                       # éxito razonable
            return self.densify(smooth_bspline(path_rrt, ds=0.25))

        # ---------- ➋  fallback (policy + exploración) --------------------
        return self._fan_path_with_policy(start, target, grid, info, h_arr, hm_info)


# ─────────────────────────────────────────────────────────────
#  Fallback planner: abanico  +  policy  +  ε-greedy
# ─────────────────────────────────────────────────────────────
    def _fan_path_with_policy(self,
                            start,   # (x,y) robot
                            target,  # (x,y) goal/frontier
                            grid, info,
                            h_arr, hm_info):
        """
        Devuelve lista [(x,y), …] construida paso a paso:
        1. Δ sugerido por la policy CNN-LSTM.
        2. Muestreo aleatorio (ε-greedy) p/ dar diversidad.
        3. Heurística directa al target.
        Cada candidato se filtra por:
            · celda libre (<50) y línea Bresenham libre
            · holgura ≥ CLEAR_MIN
            · pendiente < 30°  (si hay mapa de alturas)
        """
        cp   = start
        path = [start]

        # parche inicial centrado en el robot
        patch, _, _ = self.extract_patch()        # (PATCH,PATCH,2) flotante

        for _ in range(self.max_steps):

            # ─── 1)  Δ recomendado por la policy ────────────────────────
            state_pol          = np.zeros(7, dtype=np.float32)
            state_pol[-2]      = target[0] - cp[0]
            state_pol[-1]      = target[1] - cp[1]

            delta = self.policy(
                [patch[None, ...],          # (1,128,128,2)
                state_pol[None, :],        # (1,7)
                np.zeros((1, 2), np.float32)],
                training=False
            )[0].numpy()                   # (Δx,Δy,Δr) en –1…1

            wp_pol = (cp[0] + delta[0]*self.max_seg,
                    cp[1] + delta[1]*self.max_seg)

            # ─── 2)  ε-greedy: muestra un punto aleatorio ───────────────
            eps     = self._eps_wp()
            wp_rnd  = None
            if random.random() < eps:
                ang = random.uniform(-math.pi,  math.pi)
                rad = random.uniform(0.4,       RND_RADIUS_MAX)
                wp_rnd = (cp[0] + rad*math.cos(ang),
                        cp[1] + rad*math.sin(ang))

            # ─── 3)  Heurística: un paso recto al goal ──────────────────
            vec      = np.array(target) - np.array(cp)
            dist_t   = np.linalg.norm(vec) + 1e-6
            wp_heur  = (cp[0] + self.max_seg*vec[0]/dist_t,
                        cp[1] + self.max_seg*vec[1]/dist_t)

            cand_list = [wp_pol, wp_heur] + ([wp_rnd] if wp_rnd else [])

            # ─── 4)  Selección del mejor candidato ──────────────────────
            best, best_score = None, float("inf")
            for cand in cand_list:

                # 4.a dentro del mapa
                i, j = idx_from_world(info, cand)
                if not (0 <= i < grid.shape[1] and 0 <= j < grid.shape[0]):
                    continue

                # 4.b celda conocida y libre
                if grid[j, i] == -1 or grid[j, i] >= 50:
                    continue

                # 4.c línea libre + holgura
                if (not bres_line_free(grid, info, cp, cand) or
                    not clearance_ok(grid, info, cand, CLEAR_MIN)):
                    continue

                # 4.d pendiente
                slope = 0.0
                if h_arr is not None:
                    _, slope = slope_ok(h_arr, hm_info, cp, cand)
                    if slope > math.tan(math.radians(30)):
                        continue

                score = 1.5*slope + distance(cand, target)   # pesos simples
                if score < best_score:
                    best, best_score = cand, score

            # 5)  ¿avance posible?
            if best is None:
                break
            path.append(best)
            cp = best

            if distance(cp, target) < self.reach_thr:
                break

            # 6)  Parche nuevo centrado en el nuevo cp para la próxima iteración
            self.pose.position.x, self.pose.position.y = cp   # «truco» temporal
            patch, _, _ = self.extract_patch()

        path.append(target)
        return self.densify(path)




####################################################antiguo
    # ---------- Siguiente waypoint ----------
    def next_waypoint(self, cp, tgt, grid, info, patch):
        # 1) Δ “preferido” de la policy
        state=np.array([0,0,tgt[0]-cp[0],tgt[1]-cp[1]],np.float32)
        patch_b = patch[None,...]
        state_b = state[None,:]
        delta=self.policy([patch_b, state_b, np.zeros((1,2),np.float32)],
                          training=False)[0].numpy()
        ang0=math.atan2(delta[1],delta[0]) if np.linalg.norm(delta)>1e-3 \
             else math.atan2(tgt[1]-cp[1], tgt[0]-cp[0])

        # 2) abanico de candidatos
        best=None; best_cost=float("inf")
        h_arr = gridmap_to_numpy(self.height_map_msg) if self.height_map_msg else None

        for r in RADII:
            for off in ANGLES:
                ang=ang0+off
                cand=(cp[0]+r*math.cos(ang), cp[1]+r*math.sin(ang))
                i,j=idx_from_world(info,cand)
                if not (0<=i<grid.shape[1] and 0<=j<grid.shape[0]): continue
                if grid[j,i]==-1 or grid[j,i]>=100: continue
                if not bres_free(grid,info,cp,cand):          continue
                if not clearance_ok(grid,info,cand,CLEAR_MIN):continue

                # ----- pendiente -----
                if h_arr is not None:
                    ok_slope, s = slope_ok(h_arr, info, cp, cand)
                    if not ok_slope:
                        continue
                else:
                    s = 0.0

                cost = l2(cand, tgt) + SLOPE_COST_GAIN * s - 0.5 * CLEAR_MIN
                if cost < best_cost:
                    best_cost, best = cost, cand
        return best, delta
    ##########################################
    def compute_obstacle_distances(self,patch, info):
        """
        patch: array PATCH×PATCH con valores [-1..100]
        info.resolution en m/celda
        Devuelve d_front, d_left, d_right en metros (∞ si libre).
        """
        R = PATCH//2
        # frontal: filas [R-4:R+4], columnas [R:PATCH]
        front = patch[R-4:R+4, R:]
        # izquierda: filas [R:PATCH], cols [0:8]
        left  = patch[R:, 0:8]
        # derecha: rows [R:PATCH], cols [PATCH-8:PATCH]
        right = patch[R:, PATCH-8:]
        def min_dist(slice_):
            # índice de la primera celda ≥0 que sea obstáculo (valor ≥0)
            occ = np.where(slice_>=0, slice_, np.inf)
            idx = np.unravel_index(np.argmin(occ), occ.shape)
            # distancia en células = columna del idx o fila según slice
            # adaptamos: para front, medimos el eje x; para left/right, eje y
            return np.inf if occ[idx]==np.inf else (idx[1] if slice_ is front else idx[0])
        d_front = min_dist(front) * info.resolution
        d_left  = min_dist(left)  * info.resolution
        d_right = min_dist(right) * info.resolution
        return d_front, d_left, d_right

    # ─────────────────────────────────────────────────────────
    #  RRT «solo» – sin policy, sin fan-planner
    # ─────────────────────────────────────────────────────────
    def rrt_plan_dbg(self,start, goal, grid, info,
                 h_arr=None, hm_info=None,
                 max_iter=1500, step=1.0, goal_tol=0.8,
                 occ_thr=100,     # ← umbral flexible
                 clear_r=0.6,     # ← holgura “light”
                    max_slope_deg=30):
        ok_slope_tan = math.tan(math.radians(max_slope_deg))

        tree = [_RRTNode(start[0], start[1], None)]
        reasons = {"occ":0, "clear":0, "slope":0, "bres":0}

        for it in range(max_iter):
            rnd     = _sample_free(info, grid, goal)
            nearest = _nearest(tree, rnd)
            new_pt  = _steer((nearest.x, nearest.y), rnd, step)

            # 1) obstáculo directo
            i, j = idx_from_world(info, new_pt)
            if grid[j, i] >= occ_thr or grid[j, i] == -1:
                reasons["occ"] += 1
                continue

            # 2) línea libre
            if not bres_line_free(grid, info, (nearest.x, nearest.y), new_pt):
                reasons["bres"] += 1
                continue

            # 3) holgura
            if not clearance_ok(grid, info, new_pt, clear_r):
                reasons["clear"] += 1
                continue

            # 4) pendiente
            if h_arr is not None:
                _, slope = slope_ok(h_arr, hm_info, (nearest.x, nearest.y), new_pt)
                if slope > ok_slope_tan:
                    reasons["slope"] += 1
                    continue

            new_node = _RRTNode(new_pt[0], new_pt[1], nearest)
            tree.append(new_node)

            if distance(new_pt, goal) < goal_tol:
                # reconstruye ruta
                path = [(goal[0], goal[1])]
                n = new_node
                while n:
                    path.append((n.x, n.y))
                    n = n.parent
                return path[::-1], reasons   # ===== éxito =====

        return [], reasons                    # ===== fracaso =====

    def _rrt_only_path(self, start, target, grid, info):
        """Sólo RRT + logs; sin fallback, sin suavizado."""
        h_arr, hm_info = (gridmap_to_numpy(self.height_map_msg)
                        if self.height_map_msg else (None, None))

        path, stats = self.rrt_plan_dbg(
            start, target,
            grid, info,
            h_arr=h_arr, hm_info=hm_info,
            max_iter=2000,
            step=max(info.resolution*3, 0.6),   # paso ≈ 3 celdas
            goal_tol=self.reach_thr,
            occ_thr=100,        # tu mapa marca 100 los ocupados
            clear_r=0.6         # holgura relajada para explorar
        )

        if path:
            self.get_logger().info(
                f"✅  RRT ok  n={len(path)}  rechazados:"
                f" occ={stats['occ']} bres={stats['bres']} "
                f"clr={stats['clear']} slope={stats['slope']}"
            )
            return self.densify(path)

        else:
            self.get_logger().warn(
                f"❌  RRT falló  rechazos:"
                f" occ={stats['occ']} bres={stats['bres']} "
                f"clr={stats['clear']} slope={stats['slope']}"
            )
            return []



    ##########################################



    # def follow_path(self, cp):
    #     # si no hay ruta o ya terminamos → quieto
    #     if len(self.current_path) <= self.wp_index:
    #         self.cmd_pub.publish(Twist())
    #         return

    #     wp = self.current_path[self.wp_index]

    #     # ¿hemos llegado a este wp?
    #     if l2(cp, wp) < LOOK_A:
    #         self.wp_index += 1
    #         if self.wp_index >= len(self.current_path):
    #             self.cmd_pub.publish(Twist())
    #             return
    #         wp = self.current_path[self.wp_index]

    #     dx, dy = wp[0] - cp[0], wp[1] - cp[1]
    #     d      = math.hypot(dx, dy)
    #     vx     = min(MAX_VEL, 1.5 * d) * dx / d
    #     vy     = min(MAX_VEL, 1.5 * d) * dy / d

    #     cmd = Twist()
    #     cmd.linear.x = vx
    #     cmd.linear.y = vy
    #     self.cmd_pub.publish(cmd)

    #     self.get_logger().info(
    #         f"[FOLLOW] wp {self.wp_index}/{len(self.current_path)-1} "
    #         f"→ v=({vx:.2f},{vy:.2f})")
###nuevo
    def _next_target_index(self, cp, look_ahead):
        """Devuelve el índice del primer waypoint a ≥ look_ahead del robot."""
        idx = self.wp_index
        while (idx + 1 < len(self.current_path)
            and l2(cp, self.current_path[idx]) < look_ahead):
            idx += 1
        return idx
######### antiguo###########################################
    # def follow_path(self, cp):
    #     """
    #     Sigue self.current_path publicando CmdThrottleMsg en lugar de Twist.
    #     cp: (x,y) posición actual del robot en world frame.
    #     """
    #     # 1) Ruta terminada → frena
    #     if self.wp_index >= len(self.current_path):
    #         stop = CmdThrottleMsg()
    #         stop.throttle = 0.0
    #         stop.steering = 0.0
    #         self.cmd_pub.publish(stop)
    #         return

    #     # 2) Look‐ahead adaptativo
    #     v_nom = MAX_VEL
    #     Ld = max(0.25, 0.4 * v_nom)
    #     self.wp_index = self._next_target_index(cp, Ld)
    #     tgt = self.current_path[self.wp_index]

    #     # 3) Cálculo del vector en world
    #     dx_g = tgt[0] - cp[0]
    #     dy_g = tgt[1] - cp[1]
    #     dist = math.hypot(dx_g, dy_g)
    #     if dist < 1e-3:
    #         stop = CmdThrottleMsg()
    #         stop.throttle = 0.0
    #         stop.steering = 0.0
    #         self.cmd_pub.publish(stop)
    #         return

    #     # 4) Pasa a coordenadas del robot
    #     yaw = self._yaw_from_quaternion(self.pose.orientation)
    #     dx, dy = self._global_to_local(dx_g, dy_g, yaw)

    #     # 5) Pure-Pursuit
    #     alpha = math.atan2(dy, dx)
    #     kappa = 2.0 * math.sin(alpha) / Ld

    #     # 6) Velocidades deseadas
    #     v_lin = max(MIN_VEL, min(MAX_VEL, 1.8 * dist))
    #     omega = kappa * v_lin

    #     # 7) Mapea a unidades de throttle/steering
    #     MAX_THROTTLE = 70.0   # unidad máxima de tu Ctrl
    #     MAX_STEERING = 70.0   # idem
    
    #     thr_cmd = (v_lin / MAX_VEL) * MAX_THROTTLE
    #     str_cmd = -(omega / (2.0)) * MAX_STEERING  # si 2.0 rad/s fuera tu ω máx.

    #     cmd = CmdThrottleMsg()
    #     cmd.throttle = max(-MAX_THROTTLE, min(MAX_THROTTLE, thr_cmd))
    #     cmd.steering = max(-MAX_STEERING, min(MAX_STEERING, str_cmd))

    #     self.cmd_pub.publish(cmd)
    #     self.get_logger().info(
    #         f"[FOLLOW-TH] wp={self.wp_index}/{len(self.current_path)-1}  "
    #         f"thr={cmd.throttle:.1f}  steer={cmd.steering:.1f}  "
    #         f"α={alpha*180/math.pi:+.1f}°"
    #     )
   
   ###################################
    # def follow_path(self, cp):
    #     """
    #     Sigue self.current_path publicando CmdThrottleMsg.
    #     Gira en sitio si α > 60°, curva mientras avanza si α ≤ 60°.
    #     """
    #     if self.wp_index >= len(self.current_path):
    #         stop = CmdThrottleMsg()
    #         stop.throttle = 0.0
    #         stop.steering = 0.0
    #         self.cmd_pub.publish(stop)
    #         return

    #     v_nom = MAX_VEL
    #     Ld = max(0.25, 0.4 * v_nom)
    #     self.wp_index = self._next_target_index(cp, Ld)
    #     tgt = self.current_path[self.wp_index]

    #     dx_g = tgt[0] - cp[0]
    #     dy_g = tgt[1] - cp[1]
    #     dist = math.hypot(dx_g, dy_g)
    #     if dist < 1e-3:
    #         stop = CmdThrottleMsg()
    #         stop.throttle = 0.0
    #         stop.steering = 0.0
    #         self.cmd_pub.publish(stop)
    #         return

    #     yaw = self._yaw_from_quaternion(self.pose.orientation)
    #     dx, dy = self._global_to_local(dx_g, dy_g, yaw)

    #     alpha = math.atan2(dy, dx)
    #     kappa = 2.0 * math.sin(alpha) / Ld

    #     if abs(alpha) > math.radians(60):
    #         thr_cmd = 0.0
    #         str_cmd = math.copysign(0.8 * MAX_STEERING, alpha)
    #     else:
    #         v_lin  = max(MIN_VEL, min(MAX_VEL, 1.8 * dist))
    #         omega  = kappa * v_lin
    #         thr_cmd = (v_lin / MAX_VEL) * MAX_THROTTLE
    #         str_cmd = -(omega / 2.0) * MAX_STEERING

    #     cmd = CmdThrottleMsg()
    #     cmd.throttle = max(-MAX_THROTTLE, min(MAX_THROTTLE, thr_cmd))
    #     cmd.steering = max(-MAX_STEERING, min(MAX_STEERING, str_cmd))
    #     self.cmd_pub.publish(cmd)
    #     self.last_cmd = cmd
    #     self.get_logger().info(
    #         f"[FOLLOW-TH] wp={self.wp_index}/{len(self.current_path)-1}  "
    #         f"α={alpha*57.3:+.1f}°  thr={cmd.throttle:.1f}  steer={cmd.steering:.1f}"
    #     )

####################################### bueno anterior
    # def follow_path(self, cp):
    #     """
    #     Pure-Pursuit mejorado:
    #     · v_des depende de curvatura y pendiente
    #     · steering via Ackermann
    #     · throttle por control P en velocidad
    #     · filtros de 1er orden para suavizar inercia
    #     """
    #     if self.wp_index >= len(self.current_path):
    #         self.cmd_pub.publish(self._brake())
    #         return

    #     # 1) Look-ahead dependiente de velocidad
    #     v_curr = math.hypot(self.twist.linear.x, self.twist.linear.y)
    #     Ld     = np.clip(1.6 * v_curr, 0.3, 4.0)      # m
    #     self.wp_index = self._next_target_index(cp, Ld)
    #     tgt = self.current_path[self.wp_index]

    #     # 2) Transformación a frame del vehículo
    #     dx_g, dy_g = tgt[0] - cp[0], tgt[1] - cp[1]
    #     yaw = self._yaw_from_quaternion(self.pose.orientation)
    #     dx, dy = self._global_to_local(dx_g, dy_g, yaw)

    #     dist   = math.hypot(dx, dy)
    #     # if dist < 1e-2:
    #     #     self.cmd_pub.publish(self._brake()); return
    #     at_last_wp = (self.wp_index == len(self.current_path)-1)
    #     if at_last_wp and dist < 0.15:        # parada real
    #         self.cmd_pub.publish(self._brake())
    #         self.last_cmd = self._brake()
    #         return

    #     alpha  = math.atan2(dy, dx)
    #     kappa  = 2.0 * math.sin(alpha) / max(dist, 1e-3)   # curvatura





    #     # 3) Desired steering (Ackermann)
    #     delta_des = math.atan(WHEEL_BASE * kappa)
    #     delta_des = np.clip(delta_des, -DELTA_MAX, DELTA_MAX)
    #     str_cmd   = -(delta_des / DELTA_MAX) * MAX_STEERING   # map to units

    #     # 4) Desired speed (curvatura + pendiente)
    #     slope = 0.0
    #     if self.height_map_msg is not None:
    #         h_arr,hm_info = gridmap_to_numpy(self.height_map_msg)
    #         slope = abs(height_at(h_arr, hm_info, tgt) -
    #                     height_at(h_arr, hm_info, cp)) / dist
    #     # v_des = V_MAX * (1 - 0.7*abs(kappa)) * (1 - 0.5*slope)
    #     # v_des = np.clip(v_des, V_MIN, V_MAX)
    #     ##v_des = V_MAX * (1 - 0.7*abs(kappa)) * (1 - 0.5*slope)

    #     v_des = V_MAX * (1 - 0.7*abs(kappa))
    #     v_des *= 1.0 - 0.5*max(slope, 0)         # cuesta arriba
    #     v_des *= 1.0 - 0.2*max(-slope, 0)        # cuesta abajo (menos severo)
    #     v_des = np.clip(v_des, V_MIN, V_MAX)

    #     # desaceleración progresiva en los últimos 0.6 m
    #     if at_last_wp and dist < 0.6:
    #         v_des *= dist / 0.6

    #     # 5) Longitudinal control (P)
    #     # thr_raw = KP_SPEED * (v_des - v_curr)          # N-units
    #     err_v   = v_des - v_curr
    #     # integral mínima para vencer rozamiento
    #     self.int_err = getattr(self, 'int_err', 0.0) + err_v*0.01
    #     self.int_err = np.clip(self.int_err, -2.0, 2.0)
    #     ## thr_raw = KP_SPEED * err_v + self.int_err + THR_IDLE*np.sign(v_des)
    #     # añade “boost” +10 % si estamos subiendo más de 5 %
    #     boost = 0.10*MAX_THROTTLE if slope > 0.05 else 0.0
    #     thr_raw = KP_SPEED * err_v + self.int_err + THR_IDLE*np.sign(v_des) + boost
    #     thr_cmd = np.clip(thr_raw, -MAX_THROTTLE, MAX_THROTTLE)

    #     # 6) Filtros y rampa
    #     ##str_cmd = (1-K_STEER_LP) * str_cmd + K_STEER_LP * self.last_cmd.steering
    #     # thr_cmd = (1-K_THROTTLE_LP) * thr_cmd + K_THROTTLE_LP * self.last_cmd.throttle
    #     # # thr_cmd = (1-K_THROTTLE_LP)*thr_cmd + K_THROTTLE_LP*self.last_cmd.throttle
    #     # # d_thr   = np.clip(thr_cmd - self.last_cmd.throttle,
    #     # #                 -MAX_DTHR, MAX_DTHR)
    #     str_filt = (1-K_STEER_LP)*str_cmd + K_STEER_LP*self.last_cmd.steering
    #     thr_filt = (1-K_THROTTLE_LP)*thr_cmd + K_THROTTLE_LP*self.last_cmd.throttle

    #     d_thr   = np.clip(thr_filt - self.last_cmd.throttle,
    #                       -MAX_DTHR, MAX_DTHR)
    #     thr_cmd = self.last_cmd.throttle + d_thr
    #     str_cmd = str_filt

    #     # 7) Publica
    #     cmd = CmdThrottleMsg()
    #     cmd.throttle = thr_cmd
    #     cmd.steering = str_cmd
    #     self.cmd_pub.publish(cmd)
    #     self.last_cmd = cmd

    #     self.get_logger().info(
    #         f"[CTRL] wp={self.wp_index}/{len(self.current_path)-1} "
    #         f"α={alpha*57.3:+.1f}° k={kappa:.3f} v={v_curr:.2f}->{v_des:.2f} "
    #         f"thr={thr_cmd:.0f} str={str_cmd:.0f}"
    #     )

    def follow_path(self, cp):
        if self.wp_index >= len(self.current_path):
            self.cmd_pub.publish(self._brake()); return

        # --- Look-ahead dinámico --------------------------------------
        v_curr = math.hypot(self.twist.linear.x, self.twist.linear.y)
        Ld_min, Ld_max = 1.2, 4.5
        Ld = np.clip(1.2 * max(v_curr, 0.1), Ld_min, Ld_max)

        self.wp_index = self._next_target_index(cp, Ld)
        tgt = self.current_path[self.wp_index]

        # --- Geometría -------------------------------------------------
        dx_g, dy_g = tgt[0]-cp[0], tgt[1]-cp[1]
        yaw = self._yaw_from_quaternion(self.pose.orientation)
        dx,  dy  = self._global_to_local(dx_g, dy_g, yaw)
        dist = math.hypot(dx, dy)
        alpha = math.atan2(dy, dx)
        kappa = 0.0           # valor dummy; no se usa durante el spin-in-place
        v_des = v_curr        # o 0.0 si prefieres

        # # --- Modo giro en sitio vs avance ------------------------------
        # if abs(alpha) > math.radians(45):             # giro puro
        #     thr_pre = 0.0
        #     str_pre = 0.85 * MAX_STEERING * np.sign(alpha)
        # else:
        # ----- Pure-pursuit suavizado ------------------------------
        kappa = 2.0 * math.sin(alpha) / max(dist, 1e-3)
        K_CURV = 0.6
        delta_des = math.atan(WHEEL_BASE * K_CURV * kappa)
        str_pre = -(delta_des / DELTA_MAX) * MAX_STEERING
        # velocidad deseada
        v_des = np.clip(V_MAX * (1 - 0.6*abs(kappa)), V_MIN, V_MAX)
        # simple P
        thr_pre = KP_SPEED * (v_des - v_curr) + THR_IDLE*np.sign(v_des)

        # --- Filtro crítico-amortiguado -------------------------------
        TAU, dt = 0.25, 0.05
        a1 = 2*TAU/(2*TAU+dt)
        b1 = dt/(2*TAU+dt)
        str_cmd = a1*str_pre + b1*(str_pre - self.last_cmd.steering)
        thr_cmd = a1*thr_pre + b1*(thr_pre - self.last_cmd.throttle)

        # --- Saturación y rampa longitudinal --------------------------
        d_thr   = np.clip(thr_cmd - self.last_cmd.throttle, -MAX_DTHR, MAX_DTHR)
        thr_cmd = self.last_cmd.throttle + d_thr
        str_cmd = np.clip(str_cmd, -MAX_STEERING, MAX_STEERING)

        # --- Publicar --------------------------------------------------
        cmd = CmdThrottleMsg()
        cmd.throttle, cmd.steering = thr_cmd, str_cmd
        self.cmd_pub.publish(cmd)
        self.last_cmd = cmd
        self.get_logger().info(
            f"[CTRL] wp={self.wp_index}/{len(self.current_path)-1} "
            f"α={alpha*57.3:+.1f}° k={kappa:.3f} v={v_curr:.2f}->{v_des:.2f} "
            f"thr={thr_cmd:.0f} str={str_cmd:.0f}"
        )



############################################
 
###nuevo
    # ---------- Recompensa --------------
    def compute_reward(self, old_d, new_d, collided, reached, step_len,
                    overturned, min_obs_dist, is_spinning,slope_curr):
        """
        old_d, new_d: distancias a target
        min_obs_dist: mínimo entre d_front/d_left/d_right
        is_spinning: bool, si estamos girando in situ
        """
        r = 2.0*(old_d - new_d)      # progreso
        r -= 0.05                    # coste por paso
        r -= 0.1 * step_len          # coste ruta larga
        if reached:    r += 200
        if collided:   r -= 200
        if overturned: r -= 200
        # penaliza proximidad a obstáculo
        r -= 1.0 / (min_obs_dist + 1e-3)
        r -= 15.0 * slope_curr          # slope_curr = pendiente media del último segmento

        # bonifica giros in situ
        if is_spinning:
            r += 10.0
        return r

    # ---------- Buffers PPO -------------
    def reset_buffers(self):
        self.patch_buf=[]; self.state_buf=[]
        self.act_buf=[];   self.logp_buf=[]
        self.rew_buf=[];   self.val_buf=[]
        self.done_buf=[]
        self.vel_buf = []

    def step(self):

        if getattr(self, "training", True):
            self._hold_position()
            return                         # no hacemos nada más


        # 0 · Espera a que el mundo nuevo esté listo ------------------
        if not self.ready:
            ok_time = self.reset_t0 and \
                    (self.get_clock().now() - self.reset_t0).nanoseconds > 2e9
            ok_grid = self.grid_msg and \
                    np.any(np.array(self.grid_msg.data) != -1)
            if ok_time and self.pose and ok_grid:
                self.ready = True
                self.get_logger().info("[Reset] mundo cargado; reanudando")
            else:
                return                                   # sigue en pausa


        # #####
        # now   = self.get_clock().now()
        # age_s = (now - self._grid_stamp).nanoseconds * 1e-9 if self._grid_stamp else 0.0
        # if age_s > self.map_max_age_sec:
        #     # mapa “viejo” ⇒ mantente quieto
        #     self._hold_position()
        #     self.get_logger().debug(f"⏸  Esperando mapa (age={age_s:.2f}s)")
        #     return
###################

        # 1 · Validaciones mínimas ------------------------------------
        if None in (self.pose, self.goal, self.grid_msg):
            return
        cp = (self.pose.position.x, self.pose.position.y)

        # ——— detector de atasco ——————————————————————————
        dist_progress = l2(cp, getattr(self, 'prev_pos', cp))
        if dist_progress < STUCK_DIST_THR:
            self.stuck_counter += 1
        else:
            self.stuck_counter = 0
        self.prev_pos = cp

        # #  ▶ si ya estamos en modo marcha atrás
        # if self.reversing:
        #     if self.rev_steps_left > 0:
        #         self.cmd_pub.publish(self._reverse_cmd())
        #         self.rev_steps_left -= 1
        #         return
        #     else:
        #         self.reversing = False          # fin de la maniobra
        #  ▶ entrar en marcha atrás si acumulamos ciclos atascados
        # if self.stuck_counter >= STUCK_CYCLES_MAX:
        #     self.get_logger().warning("🚗  Atasco detectado → marcha atrás")
        #     self.reversing       = True
        #     self.rev_steps_left  = REV_STEPS

        #     if self.reversing:
        #         if self.rev_steps_left > 0:
        #             self.cmd_pub.publish(self._reverse_cmd())
        #             self.rev_steps_left -= 1
        #             return
        #         else:
        #             self.reversing = False          # fin de la maniobra
        #         self.stuck_counter   = 0
        #     return
        if self.stuck_counter >= STUCK_CYCLES_MAX:
            self.get_logger().warning("🚗  Atasco detectado → marcha atrás")
            self.reversing       = True
            self.rev_steps_left  = REV_STEPS

            if self.reversing:
                if self.rev_steps_left > 0:
                    self.cmd_pub.publish(self._reverse_cmd())
                    self.rev_steps_left -= 1
                    return
                else:
                    self.reversing = False          # fin de la maniobra
                self.stuck_counter   = 0
            return




    #---------- 2 · ¿estamos escaneando? ----------
        need_extra_scan = False
        if self.scanning:
            # ←--- PRIMERO comprobamos si **ya** hay un target
            patch, grid, info = self.extract_patch()
            tgt, mode = self.choose_target(cp, grid, info)
            self.get_logger().info(f"scanning: {self.scanning} {tgt}")
            if tgt is not None:               # ¡ya vemos algo!
                self.scanning = False         # cancela giro y sigue
            elif (self.get_clock().now() - self.scan_t0).nanoseconds < self.SCAN_SECS*1e9:
                self.cmd_pub.publish(self._spin_in_place(+1))
                return
            else:                             # terminó el giro completo
                self.scan_t0 = self.get_clock().now()   # arranca otro barrido
                return


        # ---------- 3 · parche + target ----------
        patch, grid, info = self.extract_patch()
        tgt, mode = self.choose_target(cp, grid, info)

        # 3.a  Si sigue sin target ⇒ iniciar un único barrido
        need_extra_scan = False
        if tgt is None:
            need_extra_scan = True
        else:
            #  nuevo: demasiado desconocido en frente aunque haya objetivo
            unknown_frac = self._front_unknown_fraction(patch[:, :, 0])
            if unknown_frac > UNKNOWN_FRAC_THR:
                need_extra_scan = True
                self.get_logger().info(f"🌑  {unknown_frac:.0%} frontal desconocido → escaneo")

        if need_extra_scan:
            self.scanning = True
            self.scan_t0  = self.get_clock().now()
            self.get_logger().info("🌪  Giro exploratorio")
            return

        # ---------- 4 · alinearse al objetivo antes de avanzar ----------
        # yaw_goal  = math.atan2(tgt[1]-cp[1], tgt[0]-cp[0])
        # yaw_robot = self._yaw_from_quaternion(self.pose.orientation)
        # yaw_err   = self._wrap_angle(yaw_goal - yaw_robot)

        # if abs(yaw_err) > math.radians(12):       # ±12°
        #     self.cmd_pub.publish(self._spin_in_place(math.copysign(1, yaw_err)))
        #     return                                # aún girando para encarar

        # 1.b Detector de vuelco --------------------------------------
        roll, pitch = self._roll_pitch_from_quaternion(self.pose.orientation)
        overturned  = abs(roll) > MAX_TILT or abs(pitch) > MAX_TILT
        if overturned:
            self.get_logger().warning("🚨  Robot volcado")
            self.reset_pub.publish(Bool(data=True))
            self.waiting_reset = True
            return                        # espera a que el mundo se reinicie


        # 2 · Parche local y target robusto ---------------------------
        patch, grid, info = self.extract_patch()
        d_front, d_left, d_right = self.compute_obstacle_distances(patch[:,:,0], info)
        min_obs_dist = min(d_front, d_left, d_right)
        tgt, mode = self.choose_target(cp, grid, info)
        if tgt is None:
            self.get_logger().warn("Sin target válido")
            return

        # comprueba si el target esta activo
        if tgt != self.active_target:
            need_replan = True
            self.active_target = tgt


        # 3 · ¿Replanificamos? ----------------------------------------
        need_replan = (
            not self.current_path or
            self.collided or
            l2(cp, self.current_path[min(2, len(self.current_path)-1)]) > 0.8
        )
        if need_replan:

            #path_tmp = self.generate_flexible_path(cp, tgt, grid, info)
            path_tmp = self._rrt_only_path(cp, tgt, grid, info)

            # → si la ruta resultó muy corta ( ≤2 puntos ) o vacía, marca esa frontier
            if len(path_tmp) <= 2:
                # sólo blacklist si era una frontier, no el goal
                if mode == "FRONTIER":
                    self.bad_frontiers.append(tgt)
                    # self.bad_pub.publish(self._list_to_posearray(self.bad_frontiers))
                    self._pub_frontier_set(self.bad_frontiers,     self.mk_bad_pub,
                       (1.0, 1.0, 0.0), "bad")
                    self.get_logger().info(
                        f"❌  Frontier descartada por callejón sin salida {tgt}")
                return                              # pide otro target en el próximo ciclo


            self.current_path = path_tmp
            self.wp_index = 1
            self.get_logger().info(f"[PATH] len={len(self.current_path)} wps")
            
            slopes = []
            if self.height_map_msg:
                h_arr, hm_info = gridmap_to_numpy(self.height_map_msg)
                for a, b in zip(path_tmp, path_tmp[1:]):
                    slopes.append(abs(height_at(h_arr, hm_info, b) -
                                      height_at(h_arr, hm_info, a)) /
                                  max(distance(a, b), 1e-3))
            self.publish_debug_path(path_tmp, slopes+[0.0])

        # 4 · Seguimiento del path ------------------------------------
        self.follow_path(cp)
        self.publish_path(self.current_path)
        # self._publish_frontier_markers()
        # marca los frontiers visitados
        if mode == "FRONTIER" and l2(cp, tgt) < REACHED_F_THR:
            self.visited_frontiers.append(tgt)
            #self.vis_pub.publish(self._list_to_posearray(self.visited_frontiers))
            self._pub_frontier_set(self.visited_frontiers, self.mk_vis_pub,
                       (0.0, 1.0, 1.0), "visited")
            self.get_logger().info(f"✅  Frontier visitada {tgt}")


        # 5 · Recompensa + buffers PPO --------------------------------
        wp = self.current_path[self.wp_index]
        old_d = l2(cp, tgt)
        new_d = l2(wp, tgt)

        # ——— pendiente del tramo actual ———————————————
        if self.height_map_msg is not None:
            h_arr, hm_info = gridmap_to_numpy(self.height_map_msg)
            if h_arr is not None:
                dz = abs(height_at(h_arr, hm_info, wp) -
                         height_at(h_arr, hm_info, cp))
                slope_curr = dz / max(distance(cp, wp), 1e-6)
            else:
                slope_curr = 0.0
        else:
            slope_curr = 0.0



        reached=self.goal_reached_flag
        collided=self.collided
        # detecta si está girando en sitio:
        # por ejemplo: si throttle casi 0 y steering alto
        is_spinning = (abs(self.last_cmd.throttle) < 1e-2 and 
                    abs(self.last_cmd.steering) > 0.5 * MAX_STEERING)

        reward = self.compute_reward(
            old_d, new_d,
            collided=self.collided,
            reached=self.goal_reached_flag,
            step_len=l2(cp, wp),
            overturned=abs(self._roll_pitch_from_quaternion(self.pose.orientation)[0]) > MAX_TILT or
                    abs(self._roll_pitch_from_quaternion(self.pose.orientation)[1]) > MAX_TILT,
            min_obs_dist=min_obs_dist,
            is_spinning=is_spinning,
            slope_curr=slope_curr
        )

        state_vec = np.concatenate([patch.flatten()[:32],
                                    np.array([0, 0,
                                            tgt[0]-cp[0],
                                            tgt[1]-cp[1]], DTYPE)])


                # ---------------- velocidad experta (la que acabamos de mandar) ----------
        dx, dy = wp[0]-cp[0], wp[1]-cp[1]
        dist   = math.hypot(dx, dy) + 1e-6
        v_lin  = min(MAX_VEL, 1.5*dist)          # misma fórmula que follow_path
        v_norm = np.clip(2*v_lin/MAX_VEL - 1, -1, 1)   # normaliza 0..MAX → -1..+1

        self.vel_buf.append(np.float32(v_norm))         # ← guarda


        # acción ficticia (Δx,Δy de la policy, opcional)
        act_delta = np.zeros(3, DTYPE)
        slope_tgt = slope_curr
        roll, pitch = self._roll_pitch_from_quaternion(self.pose.orientation)
        state6 = np.array([
                d_front,
                d_left,
                d_right,
                slope_tgt,
                pitch,
                tgt[0] - cp[0],
                tgt[1] - cp[1]
            ], dtype=DTYPE)
        self.state_buf.append(state6)
        self.patch_buf.append(patch.astype(DTYPE))
        # self.state_buf.append(state_vec)
        self.act_buf.append(act_delta)
        self.logp_buf.append(np.float32(0.0))
        self.rew_buf.append(reward)
        # self.val_buf.append(self.value_net(state_vec[None, ...])[0, 0])
        self.done_buf.append(reached)
        # state5 ya lo tienes definido como np.array([d_front,d_left,d_right, dx, dy])
        patch_emb32 = patch.flatten()[:32]
        critic_input = np.concatenate([patch_emb32, state6])   # tamaño 37
        self.val_buf.append(self.value_net(critic_input[None, ...])[0, 0])
        self.total_steps += 1

        # 6 · Fin de episodio -----------------------------------------
        if reached:
            self.cmd_pub.publish(CmdThrottleMsg())                  # v=0, ω=0

            if reached:    self.get_logger().info("⛳  Goal alcanzado")
            if collided:   self.get_logger().info("💥  Colisión detectada")
            if overturned: self.get_logger().warning("🚨  Robot volcado")
            self.training = True
            self.update_ppo()                              # entrenamiento
            self.training = False
            self.goal_reached_flag = False
            self.episode  += 1
            self.goal_counter += int(reached and mode == "GOAL")


            with self.writer.as_default():
                tf.summary.scalar("episode_reward",  sum(self.rew_buf),
                                step=self.episode)
                tf.summary.scalar("collided", int(collided), step=self.episode)




        # 7 · Reset de mapa si procede -------------------------------
        if (self.goal_counter >= self.goals_in_world):

            self.reset_pub.publish(Bool(data=True))
            self.ready         = False
            self.waiting_reset = True
            self.reset_t0      = self.get_clock().now()
            self.goal_counter  = 0
            self.goals_in_world = random.randint(5, 7)
            self.get_logger().info("[RESET] petición enviada; esperando confirmación")

        # 8 · Límite global de episodios ------------------------------
        if self.episode >= MAX_EPISODES:
            self.get_logger().info(f"{MAX_EPISODES} episodios completados. "
                                "Finalizando nodo.")
            self.cmd_pub.publish(CmdThrottleMsg())
            rclpy.shutdown()

    # def _publish_frontier_markers(self):
    #     header = Header(frame_id="map",
    #                     stamp=self.get_clock().now().to_msg())

    #     def mk(points, rgb, ns):
    #         m = Marker(header=header, ns=ns, id=0,
    #                 type=Marker.SPHERE_LIST, action=Marker.ADD)
    #         m.scale.x = m.scale.y = m.scale.z = 2.0
    #         m.color.a = 1.0
    #         m.color.r, m.color.g, m.color.b = rgb
    #         m.points = [Point(x=x, y=y, z=0.0) for x, y in points]
    #         return m

    #     self.wps_pub.publish(mk(self.frontiers,        (0.0, 1.0, 0.0), "safe"))
    #     self.wps_pub.publish(mk(self.visited_frontiers,(0.0, 1.0, 1.0), "visited"))
    #     self.wps_pub.publish(mk(self.bad_frontiers,    (1.0, 1.0, 0.0), "bad"))

    def _pub_frontier_set(self, pts, pub, rgb, ns):
        m = Marker()
        m.header.frame_id = "map"
        m.header.stamp    = self.get_clock().now().to_msg()
        m.ns = ns;  m.id = 0
        m.type   = Marker.SPHERE_LIST
        m.action = Marker.ADD
        m.scale.x = m.scale.y = m.scale.z = 2.5
        m.color.r, m.color.g, m.color.b, m.color.a = *rgb, 1.0
        m.points = [Point(x=x, y=y, z=0.25) for x, y in pts]
        pub.publish(m)

    # ---------- Publicación RViz ----------
    def publish_path(self,pts):
        hdr=Header(frame_id="map",
                   stamp=self.get_clock().now().to_msg())
        path=Path(header=hdr)
        for x,y in pts:
            ps=PoseStamped(header=hdr)
            ps.pose.position.x=x; ps.pose.position.y=y
            ps.pose.orientation.w=1.0
            path.poses.append(ps)
        self.path_pub.publish(path)

        mk=Marker(header=hdr,ns="wps",id=0,
                  type=Marker.POINTS,action=Marker.ADD)
        mk.scale=Vector3(x=0.15,y=0.15,z=0.0)
        mk.color.r=mk.color.g=1.0; mk.color.a=1.0
        mk.points=[Point(x=x,y=y) for x,y in pts[1:]]
        self.wps_pub.publish(mk)


    def publish_debug_path(self, pts, slopes):
            mk = Marker()
            mk.header.frame_id = "map"
            mk.header.stamp    = self.get_clock().now().to_msg()
            mk.ns = "debug_wp"; mk.id = 0
            mk.type  = Marker.SPHERE_LIST
            mk.action= Marker.ADD
            mk.scale.x = mk.scale.y = mk.scale.z = 0.25
            mk.points = [Point(x=p[0], y=p[1], z=0.15) for p in pts]
            for s in slopes:
                c = ColorRGBA()
                c.r, c.g, c.b, c.a = min(1.0, 2.0*s), 1.0-min(1.0,2.0*s), 0.2, 1.0
                mk.colors.append(c)
            self.debug_wp_pub.publish(mk)




    def update_ppo(self):
        # 1) Returns y ventajas ---------------------------------------
        returns, advs = [], []
        gae = 0.0
        next_val = 0.0
        for r, v, d in zip(reversed(self.rew_buf),
                        reversed(self.val_buf),
                        reversed(self.done_buf)):
            delta = r + GAMMA * next_val * (1 - d) - v
            gae   = delta + GAMMA * GAE_LAMBDA * (1 - d) * gae
            advs.insert(0, gae)
            next_val = v
        returns = np.array(advs) + np.array(self.val_buf)
        advs    = (np.array(advs) - np.mean(advs)) / (np.std(advs) + 1e-8)

        # 2) DataSet ---------------------------------------------------
        ds = tf.data.Dataset.from_tensor_slices(
            (np.stack(self.patch_buf).astype(np.float32),
            np.stack(self.state_buf).astype(np.float32),
            np.stack(self.act_buf).astype(np.float32),     # (N,2)
            np.array(self.logp_buf, np.float32),
            advs.astype(np.float32),
            returns.astype(np.float32))
        ).shuffle(4096).batch(BATCH_SZ)

        # 3) Optimización ---------------------------------------------
        for _ in range(EPOCHS):
            for g, st, act, lp_old, adv, ret in ds:
                with tf.GradientTape() as tpi, tf.GradientTape() as tpv:
                    w0_dummy = tf.zeros((tf.shape(act)[0], 2), tf.float32)
                    mu  = self.policy([g, st, w0_dummy], training=True)
                    std = tf.exp(self.log_std)              # (2,)

                    lp  = -0.5 * tf.reduce_sum(
                            ((act - mu) / std) ** 2 +
                            2 * tf.math.log(std) +
                            tf.math.log(2 * np.pi), axis=-1)

                    ratio    = tf.exp(lp - lp_old)
                    pg_loss  = -tf.reduce_mean(
                                tf.minimum(ratio * adv,
                                            tf.clip_by_value(ratio,
                                                            1-CLIP_EPS, 1+CLIP_EPS) * adv))

                    # state_vec = tf.concat(
                    #      [tf.reshape(g, (-1, PATCH*PATCH))[:, :32], st], axis=-1)
                    patch_flat = tf.reshape(g, (tf.shape(g)[0], -1))
                    state_vec  = tf.concat([patch_flat[:, :32], st], axis=-1)
                    v      = tf.squeeze(self.value_net(state_vec, training=True), axis=-1)
                    v_loss = tf.reduce_mean((ret - v) ** 2)

                self.opt_actor.apply_gradients(
                    zip(tpi.gradient(pg_loss,
                                    self.policy.trainable_variables + [self.log_std]),
                        self.policy.trainable_variables + [self.log_std]))
                self.opt_critic.apply_gradients(
                    zip(tpv.gradient(v_loss, self.value_net.trainable_variables),
                        self.value_net.trainable_variables))

        # 4) Annealing de σ -------------------------------------------
        new_std = tf.maximum(tf.exp(self.log_std) * STD_DECAY, STD_MIN)
        self.log_std.assign(tf.math.log(new_std))

        # 5) Logs ------------------------------------------------------
        with self.writer.as_default():
            tf.summary.scalar("loss_actor",  pg_loss,      step=self.episode)
            tf.summary.scalar("loss_critic", v_loss,       step=self.episode)
            tf.summary.scalar("policy_std",  float(new_std[0]), step=self.episode)

        # 6) Limpieza y guardado --------------------------------------------------
        
            self.policy.save_weights(RUN_DIR / f"policy_latest_{self.ts}.weights.h5")
            demo_fname = RUN_DIR / f"demo_ep{self.ts}.npz"
            np.savez_compressed(
                demo_fname,
                patches = np.stack(self.patch_buf, 0),
                states  = np.stack(self.state_buf, 0),
                actions = np.stack(self.act_buf,  0),   # Δx Δy que ya tenías
                v_norm  = np.stack(self.vel_buf,  0)    # ← NUEVO
            )
            self.get_logger().info(f"[DEMO] guardada en {demo_fname.name}")


        self.reset_buffers()
        self.get_logger().info(f"[PPO] update  π={pg_loss.numpy():.3f}  "
                            f"V={v_loss.numpy():.3f}  σ={float(new_std[0]):.2f}")

# ==============  MAIN  ====================================================
def main(args=None):
    rclpy.init(args=args)
    node=FlexPlanner()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node(); rclpy.shutdown()

if __name__=="__main__":
    main()
