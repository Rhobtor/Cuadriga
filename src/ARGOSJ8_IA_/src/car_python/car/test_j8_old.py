"""
terrain_ppo_trainer_hierarchical.py
────────────────────────────────────
Nodo ROS 2 que aprende, en tiempo real, a planificar rutas en 3D (considerando pendiente)
y a ejecutar controles localmente. Consta de dos niveles:

  1. **Meta‐policy (alto nivel)**: elige waypoints globales (radio R_META) basándose en un parche
     amplio (64×64) de ocupación + gradiente de alturas, y la distancia al objetivo final.
     Entrenada con PPO para maximizar progreso global y minimizar coste energético por pendiente.
  2. **Low‐level (bajo nivel)**: dado un waypoint, planifica localmente usando RRT* con coste continuo
     (distancia + λ·Δh) y sigue la ruta con un Pure‐Pursuit mejorado (puede ir hacia adelante o atrás,
     detecta deriva lateral y fuerza replanificación si se sale más de 1 m). Incluye
     post‐procesado para descartar arcos de curvatura imposible y garantizar que el camino
     respete radio mínimo y pendiente máxima. También entrenada con PPO, usando observaciones locales 128×128 + gradiente local.

El nodo se adapta sobre la marcha: no requiere un mapa global previo, sino que usa ocupación y alturas
construidas online para planificar y navegar. Publica continuamente el Path y un Marker del waypoint
para que RViz lo muestre en todo momento.

Uso:
  ros2 run car terrain_ppo_trainer_hierarchical

Dependencias:
  • ROS 2 Foxy/Humble (python3, rclpy, nav_msgs, geometry_msgs, etc.)
  • TensorFlow 2.x
  • numpy, scipy (interpolación, zoom)
"""

import os
import math
import random
import time
import pathlib

import numpy as np
import tensorflow as tf
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from std_srvs.srv import Empty
from nav_msgs.msg import Odometry, OccupancyGrid, Path
from geometry_msgs.msg import PoseArray, PoseStamped, Point, Vector3
from visualization_msgs.msg import Marker
from std_msgs.msg import Header, Bool, ColorRGBA
from grid_map_msgs.msg import GridMap
from scipy.interpolate import splprep, splev
from scipy.ndimage import zoom
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg

# ==============  PARÁMETROS GLOBALES  =====================================
PATCH             = 128                         # lado del parche local (celdas)
BIG_PATCH         = 64                          # lado del parche global reducido (celdas)
CLEAR_MIN         = 0.6                         # holgura mínima (m)
GOAL_RADIUS       = 2.5                         # m para “goal reached”
ROLLOUT_STEPS     = 2048
BATCH_SZ          = 256
EPOCHS            = 100
GAMMA             = 0.99
GAE_LAMBDA        = 0.95
CLIP_EPS          = 0.2
LR_ACTOR          = 3e-4
LR_CRITIC         = 1e-3
STD_START         = 0.4
STD_MIN           = 0.05
STD_DECAY         = 0.995
MAX_EPISODES      = 300
MAX_STEPS_EP      = 10000                        # Máximo de pasos por episodio (bajo nivel)
MAX_TILT          = 1.0
WHEEL_BASE        = 1.35
DELTA_MAX         = math.radians(32)
KP_SPEED          = 10.0
V_MAX             = 3.0
V_REV_MAX         = 1.5                         # velocidad máxima en marcha atrás
BACK_ALPHA_MIN    = math.radians(120)           # ángulo para activar reversa
V_MIN             = 0.6
THR_IDLE          = 2.0
MAX_DTHR          = 2.0
MAX_STEERING = 60.0
MAX_SLOPE_TAN     = math.tan(math.radians(90.0))
MAX_HEIGHT        = 5.0
HEIGHT_LAYER      = "elevation"
RUN_DIR           = pathlib.Path.home() / "PHD" / "ARGOJ8_IA" / "weights"
RUN_DIR.mkdir(parents=True, exist_ok=True)

# Parámetros de coste en RRT*
LAMBDA_SLOPE      = 8.0       # peso de Δh en coste: coste = dist + λ·Δh
SLOPE_MAX         = 0.6       # tan(θ) ≈ 30°, descartar aristas con slope > SLOPE_MAX

# Parámetros meta-policy
R_META            = 8.0       # m: radio máximo de waypoint relativo
MIN_WP_DIST       = 3.0       # m: distancia mínima desde el robot al nuevo waypoint

# Parámetros de energía para la recompensa
M_ROBOT           = 150.0     # kg del robot
G                 = 9.81      # gravedad (m/s²)
ENERGY_SCALE      = 0.02      # escala para convertir julios a puntos de recompensa

# Parámetros de “atascado”
NO_PROGRESS_LIMIT = 1000        # 3 segundos (30 ciclos a 10 Hz)
MIN_MOVEMENT      = 0.05      # m: umbral de movimiento para considerar progreso

# Parámetros de “alcanzar waypoint”
WAYPOINT_RADIUS   = 2.5       # m: radio para dar waypoint por alcanzado

# Parámetros de curvatura y re-plan
R_MIN             = 3.5       # radio mínimo real del robot (m)
KAPPA_MAX         = 1.0 / R_MIN
REPLAN_DIST       = 1.5       # m, error lateral para replanificar

# Parámetros look-ahead
K1_LD             = 0.8
K2_LD             = 1.2


RESET_GRACE_SEC = 15.0          # segundos que damos para estabilizar

# ==============  UTILIDADES  ===============================================
def l2(a, b):
    """Distancia Euclídea 2D entre puntos a=(x1,y1), b=(x2,y2)."""
    return math.hypot(b[0] - a[0], b[1] - a[1])

def idx_from_world(info, pt):
    """Convierte punto (x,y) real a índices (i,j) en OccupancyGrid."""
    res = info.resolution
    return int((pt[0] - info.origin.position.x) / res), int((pt[1] - info.origin.position.y) / res)

def gridmap_to_numpy(msg: GridMap, layer=HEIGHT_LAYER):
    """
    Extrae capa 'layer' de GridMap y la convierte a array 2D numpy (height map).
    Devuelve (arr, info) donde info es GridMap.info.
    """
    if msg is None or layer not in msg.layers:
        return None, None
    idx_layer = msg.layers.index(layer)
    h = msg.data[idx_layer].layout.dim[1].size
    w = msg.data[idx_layer].layout.dim[0].size
    buf = np.asarray(msg.data[idx_layer].data, dtype=np.float32)
    arr = buf.reshape((h, w))
    arr = np.nan_to_num(arr, nan=0.0)
    return arr, msg.info

def bresenham_points(a, b, info):
    """
    Generador que itera sobre índices de celdas (i,j) entre dos puntos a,b usando Bresenham.
    `info` es OccupancyGrid.info para cálculo de índices.
    """
    i0, j0 = idx_from_world(info, a)
    i1, j1 = idx_from_world(info, b)
    di, dj = abs(i1 - i0), abs(j1 - j0)
    si, sj = (1 if i0 < i1 else -1), (1 if j0 < j1 else -1)
    err = di - dj
    while True:
        yield i0, j0
        if (i0, j0) == (i1, j1):
            break
        e2 = 2 * err
        if e2 > -dj:
            err -= dj; i0 += si
        if e2 < di:
            err += di; j0 += sj

def bres_line_free(grid, info, a, b):
    """
    Comprueba que la línea entre puntos a y b (en coordenadas reales) traverse solo celdas libres
    (valor < 50). Retorna False si choca o sale del mapa.
    """
    for i, j in bresenham_points(a, b, info):
        if not (0 <= i < grid.shape[1] and 0 <= j < grid.shape[0]):
            return False
        if grid[j, i] == -1 or grid[j, i] >= 50:
            return False
    return True

def clearance_ok(grid, info, pt, r_m):
    """
    Comprueba holgura circular de radio r_m alrededor de pt en OccupancyGrid.
    Retorna False si alguna celda dentro del radio está ocupada (≥50 o -1).
    """
    i, j = idx_from_world(info, pt)
    r = int(r_m / info.resolution)
    H, W = grid.shape
    for dj in range(-r, r + 1):
        for di in range(-r, r + 1):
            ii, jj = i + di, j + dj
            if 0 <= ii < W and 0 <= jj < H and (grid[jj, ii] == -1 or grid[jj, ii] >= 50):
                return False
    return True

def slope_ok(arr_h, info_h, a, b):
    """
    Calcula pendiente y Δh entre a y b sobre altura arr_h.  
    - Retorna (True, slope, dz).  
    - slope = tan(θ) = |dz|/dist.  
    - dz = |h_b - h_a|.
    No filtra por pendiente máxima; eso se aplica externamente.
    """
    if arr_h is None or info_h is None:
        return True, 0.0, 0.0
    res = info_h.resolution
    ox = info_h.pose.position.x - info_h.length_x / 2.0
    oy = info_h.pose.position.y - info_h.length_y / 2.0
    i_a = int((a[0] - ox) / res); j_a = int((a[1] - oy) / res)
    i_b = int((b[0] - ox) / res); j_b = int((b[1] - oy) / res)
    if 0 <= j_a < arr_h.shape[0] and 0 <= i_a < arr_h.shape[1]:
        ha = float(arr_h[j_a, i_a])
    else:
        ha = 0.0
    if 0 <= j_b < arr_h.shape[0] and 0 <= i_b < arr_h.shape[1]:
        hb = float(arr_h[j_b, i_b])
    else:
        hb = 0.0
    dz = abs(hb - ha)
    dx = l2(a, b)
    if dx < 1e-6:
        return True, 0.0, dz
    s = dz / dx
    return True, s, dz

def _sample_free(info, grid, goal, goal_bias=0.15):
    """
    Sampling para RRT*: con probabilidad goal_bias retorna goal;  
    si no, elige un punto aleatorio libre en grid (valor < 50).
    """
    if random.random() < goal_bias:
        return goal
    H, W = grid.shape
    for _ in range(100):
        i = random.randint(0, W - 1)
        j = random.randint(0, H - 1)
        if grid[j, i] < 50:
            x = info.origin.position.x + (i + 0.5) * info.resolution
            y = info.origin.position.y + (j + 0.5) * info.resolution
            return (x, y)
    return goal

# def smooth_bspline(pts, ds=0.25):
#     """
#     Suaviza lista de puntos `pts` con B-spline (tck) y densifica cada segmento a max ds.
#     """
#     if len(pts) < 3:
#         return pts
#     x, y = zip(*pts)
#     tck, u = splprep([x, y], s=0.3)
#     dist = sum(math.hypot(x2 - x1, y2 - y1) for (x1, y1), (x2, y2) in zip(pts, pts[1:]))
#     n = max(2, int(dist / ds))
#     unew = np.linspace(0, 1, n)
#     x_s, y_s = splev(unew, tck)
#     return list(zip(x_s, y_s))

def smooth_bspline(pts, ds=0.25):
        """
        Suaviza lista de puntos `pts` con B-spline (tck) y densifica cada segmento a máximo ds.
        Ajusta automáticamente el grado k para evitar errores cuando hay pocos puntos.
        """
        m = len(pts)
        if m < 3:
            return pts
        x, y = zip(*pts)
        # Para que m > k siempre se cumpla, elegimos k = min(3, m-1)
        k = min(3, m - 1)
        tck, u = splprep([x, y], s=0.3, k=k)
        dist = sum(math.hypot(x2 - x1, y2 - y1)
                   for (x1, y1), (x2, y2) in zip(pts, pts[1:]))
        n = max(2, int(dist / ds))
        unew = np.linspace(0, 1, n)
        x_s, y_s = splev(unew, tck)
        return list(zip(x_s, y_s))



def densify(path, max_seg=0.6):
    """
    Inserta puntos intermedios en `path` si la distancia entre dos puntos > max_seg.
    """
    out = [path[0]]
    for a, b in zip(path, path[1:]):
        d = l2(a, b)
        if d > max_seg:
            steps = int(math.ceil(d / max_seg))
            for i in range(1, steps):
                t = i / steps
                out.append(((a[0] * (1 - t) + b[0] * t),
                            (a[1] * (1 - t) + b[1] * t)))
        out.append(b)
    return out

# ==============  RRT* CON COSTE (DIST + λ·Δh)  ============================
def rrt_plan_cost(start, goal, grid, info, h_arr, hm_info,
                  max_iter=1500, step=1.0, goal_tol=0.8):
    """
    RRT* simplificado que minimiza coste = distancia + λ·Δh entre nodos.
    Descarta aristas con pendiente > SLOPE_MAX.
    """
    class Node:
        __slots__ = ("x", "y", "parent", "cost")
        def __init__(self, x, y, parent, cost):
            self.x = x; self.y = y; self.parent = parent; self.cost = cost

    def edge_ok(p, q):
        if not bres_line_free(grid, info, p, q):
            return False, None
        ok, slope, dz = slope_ok(h_arr, hm_info, p, q)
        if slope > SLOPE_MAX:
            return False, None
        dist = l2(p, q)
        return True, dist + LAMBDA_SLOPE * max(0.0, dz)

    if l2(start, goal) < goal_tol:
        return [start, goal]

    tree = [Node(start[0], start[1], None, 0.0)]
    for _ in range(max_iter):
        rnd = _sample_free(info, grid, goal)
        nearest = min(tree, key=lambda n: l2((n.x, n.y), rnd))
        new_pt = None
        dx, dy = rnd[0] - nearest.x, rnd[1] - nearest.y
        dist = math.hypot(dx, dy)
        if dist <= step:
            new_pt = rnd
        else:
            k = step / dist
            new_pt = (nearest.x + k * dx, nearest.y + k * dy)

        ok, edge_cost = edge_ok((nearest.x, nearest.y), new_pt)
        if not ok:
            continue

        new_cost = nearest.cost + edge_cost
        new_node = Node(new_pt[0], new_pt[1], nearest, new_cost)
        tree.append(new_node)

        if l2(new_pt, goal) < goal_tol:
            # reconstruir camino
            path = []
            n = new_node
            while n:
                path.append((n.x, n.y))
                n = n.parent
            return path[::-1]

    return []  # fallo

def _filter_curv(path):
    """
    Filtra puntos con curvatura excesiva (> KAPPA_MAX).
    Retorna lista de puntos filtrados, suavizados si hace falta.
    """
    if len(path) < 3:
        return path
    filtered = [path[0]]
    for a, b, c in zip(path, path[1:], path[2:]):
        # curvatura discreta ≈ |α|/d
        vx1, vy1 = b[0] - a[0], b[1] - a[1]
        vx2, vy2 = c[0] - b[0], c[1] - b[1]
        cross = vx2 * vy1 - vy2 * vx1
        dot   = vx2 * vx1 + vy2 * vy1
        ang   = abs(math.atan2(cross, dot))
        d     = l2(a, b)
        kappa = ang / max(d, 1e-3)
        if kappa < KAPPA_MAX:
            filtered.append(b)
        else:
            # Descarta b porque genera curva imposible
            continue
    filtered.append(path[-1])
    return smooth_bspline(filtered, ds=0.25) if len(filtered) >= 4 else filtered

# ==============  NODO PRINCIPAL  ==========================================
class TerrainPPOTrainer(Node):
    def __init__(self):
        super().__init__("terrain_ppo_trainer_hierarchical")

        qos = 10
        # Suscriptores
        self.create_subscription(Odometry,      "/ARGJ801/odom_demo",      self.cb_odom,        qos)
        self.create_subscription(PoseArray,     "/goal",                   self.cb_goal,        qos)
        self.create_subscription(OccupancyGrid, "/occupancy_grid",         self.cb_grid,        10)
        self.create_subscription(GridMap,       "/terrain_grid",           self.cb_heightmap,   10)
        self.create_subscription(PoseArray,     "/safe_frontier_points",   self.cb_frontiers,   qos)
        self.create_subscription(Bool,          "/virtual_collision",      self.cb_collision,   qos)
        self.create_subscription(Bool,          "/goal_reached",           self.cb_goal_reached, qos)
        self.create_subscription(Bool,          "/reset_confirmation",     self.cb_reset_conf,   qos)

        # Publicadores
        self.cmd_pub          = self.create_publisher(CmdThrottleMsg, "/ARGJ801/cmd_throttle_msg", qos)
        self.path_pub         = self.create_publisher(Path,           "/global_path_predicted",   qos)
        self.wps_pub          = self.create_publisher(Marker,         "/path_waypoints_marker",   qos)
        self.waypoint_pub     = self.create_publisher(Marker,         "/current_waypoint_marker", qos)
        self.goal_pub         = self.create_publisher(Bool,           "/goal_reached",            qos)

        # Cliente para resetear octomap
        self.reset_client = self.create_client(Empty, 'octomap_server/reset')
        if not self.reset_client.wait_for_service(timeout_sec=2.0):
            self.get_logger().warning("Servicio octomap_server/reset no disponible al iniciar.")

        latched = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                             durability=DurabilityPolicy.TRANSIENT_LOCAL)
        self.reset_pub  = self.create_publisher(Bool, "/reset_request", latched)

        # Estado interno
        self.pose              = None
        self.twist             = None
        self.goal              = None
        self.grid_msg          = None
        self.grid_dyn          = None
        self.reset_t0          = None
        self.height_map_msg    = None
        self.frontiers         = []
        self.collided          = False
        self.goal_reached_flag = False
        self.waiting_reset = False
        self.goal_reset_octomap= 0
        self.last_wp_rel = np.zeros(2, dtype=np.float32)  
        self.current_modo = "NONE"      # modo del waypoint actual

        # Escape guiado
        self.escaping       = False
        self.escape_path    = []
        self.escape_timer   = 0

        # Flags de jerarquía
        self.need_new_wp     = True         # cuándo el meta-nivel debe elegir waypoint
        self.current_waypoint = None        # waypoint global actual

        # Path actual (para bajo nivel)
        self.current_path      = []
        self.visited_frontiers = []
        self.bad_frontiers     = []
        self.wp_index          = 0

        # ========== Low-Level PPO ==========
        self.policy     = self.build_policy()
        self.log_std    = tf.Variable(np.log(STD_START * np.ones(2, np.float32)), trainable=True)
        self.opt_actor  = tf.keras.optimizers.Adam(LR_ACTOR)
        self.opt_critic = tf.keras.optimizers.Adam(LR_CRITIC)
        self.value_net  = tf.keras.Sequential([
            tf.keras.layers.Input(shape=(PATCH * PATCH * 3 + 4,)),
            tf.keras.layers.Dense(128, activation="tanh"),
            tf.keras.layers.Dense(1)
        ])

        # Buffers low-level
        self.reset_buffers()
        self.episode        = 0
        self.total_updates  = 0
        self.step_count     = 0

        # ========== Meta-Level PPO ==========
        self.meta_policy     = self.build_meta_policy()
        self.meta_log_std    = tf.Variable(np.log(STD_START * np.ones(2, np.float32)), trainable=True)
        self.opt_meta_actor  = tf.keras.optimizers.Adam(LR_ACTOR)
        self.opt_meta_critic = tf.keras.optimizers.Adam(LR_CRITIC)
        self.meta_value_net  = self.build_meta_value()

        # Buffers meta-level
        self.reset_meta_buffers()
        self.meta_episode   = 0
        self.meta_total_upd = 0

        # Meta-level estado inicial: distancia al goal
        self.meta_start_dist = None

        # Escritor para TensorBoard (ambos niveles)
        self.writer = tf.summary.create_file_writer(str(RUN_DIR / time.strftime("run_%Y%m%d_%H%M%S")))

        # Para detectar “atascado” (bajo nivel)
        self.last_cp = None
        self.no_progress_counter = 0

        # Parámetro de control low-level
        self.last_cmd = CmdThrottleMsg()
        self.ts       = time.strftime("%Y%m%d_%H%M%S")

        # Timer 10 Hz
        self.create_timer(0.1, self.step)
        self.get_logger().info("Terrain PPO Trainer Hie Val ready (3D adaptive).")

    # ========== Construcción de redes ==========
    def build_policy(self):
        """
        Política local (bajo nivel) que recibe parche 128×128×3 + estado [dx_wp, dy_wp, d_front, d_left].
        Produce 2 valores tanh(-1,1) = (Δ throttle, Δ steering).
        """
        g = tf.keras.Input(shape=(PATCH, PATCH, 3), name="grid")
        st = tf.keras.Input(shape=(4,), name="state")
        x = tf.keras.layers.Conv2D(16, 3, padding="same", activation="relu")(g)
        x = tf.keras.layers.MaxPooling2D()(x)
        x = tf.keras.layers.Conv2D(32, 3, padding="same", activation="relu")(x)
        x = tf.keras.layers.GlobalAveragePooling2D()(x)
        z = tf.keras.layers.Concatenate()([x, st])
        h0 = tf.keras.layers.Dense(128, activation="tanh")(z)
        c0 = tf.keras.layers.Dense(128, activation="tanh")(z)
        lstm = tf.keras.layers.LSTMCell(128)
        w0 = tf.keras.layers.Input(shape=(2,), name="w0")  # estado oculto inicial
        h1, _ = lstm(w0, [h0, c0])
        delta = tf.keras.layers.Dense(2, activation="tanh")(h1)
        return tf.keras.Model([g, st, w0], delta, name="policy")

    def build_meta_policy(self):
        """
        Política de alto nivel que recibe parche 64×64×3 (global) + vector [dx_goal, dy_goal].
        Produce (Δx, Δy) en [-1,1], se escala por R_META.
        """
        G = tf.keras.Input(shape=(BIG_PATCH, BIG_PATCH, 4), name="big_patch")
        v = tf.keras.Input(shape=(2,), name="vec_goal")
        x = tf.keras.layers.Conv2D(32, 3, activation="relu")(G)
        x = tf.keras.layers.MaxPooling2D()(x)
        x = tf.keras.layers.Conv2D(64, 3, activation="relu")(x)
        x = tf.keras.layers.GlobalAveragePooling2D()(x)
        z = tf.keras.layers.Concatenate()([x, v])
        z = tf.keras.layers.Dense(128, activation="relu")(z)
        out = tf.keras.layers.Dense(2, activation="tanh")(z)  # rango [-1,1]
        return tf.keras.Model([G, v], out, name="meta_policy")

    def build_meta_value(self):
        """
        Crítico de la meta-policy que recibe parche 64×64×3 + [dx_goal, dy_goal],
        produce valor escalar.
        """
        G = tf.keras.Input(shape=(BIG_PATCH, BIG_PATCH, 4), name="big_patch_val")
        v = tf.keras.Input(shape=(2,), name="vec_goal_val")
        x = tf.keras.layers.Conv2D(32, 3, activation="relu")(G)
        x = tf.keras.layers.MaxPooling2D()(x)
        x = tf.keras.layers.Conv2D(64, 3, activation="relu")(x)
        x = tf.keras.layers.GlobalAveragePooling2D()(x)
        z = tf.keras.layers.Concatenate()([x, v])
        z = tf.keras.layers.Dense(128, activation="tanh")(z)
        out = tf.keras.layers.Dense(1)(z)
        return tf.keras.Model([G, v], out, name="meta_value")

    # ---------- Callbacks ROS ----------
    def cb_odom(self, msg: Odometry):
        self.pose  = msg.pose.pose
        self.twist = msg.twist.twist

    def cb_goal(self, msg: PoseArray):
        self.goal = (msg.poses[0].position.x, msg.poses[0].position.y) if msg.poses else None

    def cb_grid(self, msg: OccupancyGrid):
        arr = np.array(msg.data, dtype=np.int8).reshape((msg.info.height, msg.info.width))
        self.grid_dyn = arr
        self.grid_msg = msg

    def cb_heightmap(self, msg: GridMap):
        self.height_map_msg = msg

    def cb_frontiers(self, msg: PoseArray):
        self.frontiers = [(p.position.x, p.position.y) for p in msg.poses]

    def cb_collision(self, msg: Bool):
        """
        Al detectar colisión (flanco False→True) a nivel bajo, forzar escape guiado.
        """
        if msg.data and not self.collided and not self.escaping and self.pose is not None:
            cp = (self.pose.position.x, self.pose.position.y)
            yaw = self._yaw_from_quaternion(self.pose.orientation)
            h_arr, hm_info = gridmap_to_numpy(self.height_map_msg)
            esc_pt = self.find_escape_point(cp, yaw,
                                            self.grid_dyn, self.grid_msg.info,
                                            h_arr, hm_info)
            if esc_pt:
                self.escape_path  = [cp, esc_pt]
                self.current_path = self.escape_path
                self.escaping     = True
                self.escape_timer = 60
                self.wp_index     = 1
                self.publish_path(self.escape_path)
                self.get_logger().info(f"[ESCAPE] point={esc_pt}")
            else:
                self.get_logger().warning("[ESCAPE] no free point found")
        self.collided = bool(msg.data)

    def cb_goal_reached(self, msg: Bool):
        if not msg.data:
            return
        # Evitar conflicto tras reset
        if (self.reset_t0 and
            (self.get_clock().now() - self.reset_t0).nanoseconds < 2e9):
            return
        if self.collided:
            return
        self.goal_reached_flag = True

    def cb_reset_conf(self, msg: Bool):
        if not msg.data:
            return
        self.waiting_reset = False
        self.ready         = False
        self.reset_t0      = self.get_clock().now()
        self.collided      = False
        self.reset_buffers()
        self.get_logger().info("[Reset] confirmado por el supervisor")


    # ----------- Comprobacion del robot en el mapa grid --------------
    
    # ---------- Avance lento hasta entrar en el grid -----------------
    def slow_forward(self, v_des=0.25):
        """
        Publica un pequeño throttle para avanzar recto.
        Si todavía no existe self.twist, usa un valor fijo.
        """
        cmd = CmdThrottleMsg()
        # Si aún no tenemos velocidad medida, envía 5 % del rango
        if self.twist is None:
            cmd.throttle = 5.0            # ajusta a tu vehículo
        else:
            v_curr = self.twist.linear.x
            cmd.throttle = np.clip(KP_SPEED * (v_des - v_curr) + THR_IDLE,
                                0.0, 20.0)
        cmd.steering = 0.0
        self.cmd_pub.publish(cmd)

    # ---------- Comprobar si el robot está dentro del OccupancyGrid ---
    def robot_inside_grid(self):
        if self.grid_msg is None or self.grid_dyn is None or self.pose is None:
            return False
        i, j = idx_from_world(self.grid_msg.info,
                            (self.pose.position.x, self.pose.position.y))
        if not (0 <= i < self.grid_msg.info.width and 0 <= j < self.grid_msg.info.height):
            return False
        return self.grid_dyn[j, i] != -1

    




    # ---------- Extracción de parche local (128×128×3)  -----------
    def extract_big_patch(self):
        """
        Devuelve parche 64×64×4:
        - canales 0-2  →  [norm_occ, grad_x, grad_y]             (↓2 del parche local)
        - canal 3      →  círculo de 1s en la posición proyectada del goal
        """
        patch_local, arr, info = self.extract_patch()         # (128,128,3)
        patch_down = zoom(patch_local, (BIG_PATCH / PATCH,
                                        BIG_PATCH / PATCH, 1), order=1)

        # ---------- canal 3 con el goal ----------------------------------
        goal_layer = np.zeros((BIG_PATCH, BIG_PATCH), np.float32)
        if self.goal is not None:
            # Vector (goal − robot) en metros → en celdas de BIG_PATCH
            dx = (self.goal[0] - self.pose.position.x) / info.resolution
            dy = (self.goal[1] - self.pose.position.y) / info.resolution
            # Escala porque hemos reducido 128→64
            gi = int(BIG_PATCH / 2 + dx * BIG_PATCH / PATCH)
            gj = int(BIG_PATCH / 2 + dy * BIG_PATCH / PATCH)
            rr = 2                   # radio de 3×3 celdas
            goal_layer[max(0, gj - rr): gj + rr + 1,
                    max(0, gi - rr): gi + rr + 1] = 1.0
        # -----------------------------------------------------------------

        big_patch = np.dstack([patch_down, goal_layer])       # (64,64,4)
        return big_patch.astype(np.float32), arr, info





    def extract_patch(self):
        """
        Retorna:
          - patch_local: array (128×128×3) con [norm_occ, grad_x, grad_y]
          - arr_occ, info_occ.
        """
        info = self.grid_msg.info
        H, W = info.height, info.width
        arr = self.grid_dyn.copy()
        cp = (self.pose.position.x, self.pose.position.y)
        ci = int((cp[0] - info.origin.position.x) / info.resolution)
        cj = int((cp[1] - info.origin.position.y) / info.resolution)
        i_lo, i_hi = ci - PATCH//2, ci + PATCH//2
        j_lo, j_hi = cj - PATCH//2, cj + PATCH//2
        i0, i1 = max(i_lo, 0), min(i_hi, W)
        j0, j1 = max(j_lo, 0), min(j_hi, H)
        occ_patch = arr[j0:j1, i0:i1]
        pad = ((j0 - j_lo, j_hi - j1), (i0 - i_lo, i_hi - i1))
        occ_patch = np.pad(occ_patch, pad, 'constant', constant_values=-1)
        norm_occ = ((occ_patch + 1) / 101.0).astype(np.float32)

        norm_h = np.zeros((PATCH, PATCH), dtype=np.float32)
        if self.height_map_msg:
            h_full, hm_info = gridmap_to_numpy(self.height_map_msg)
            if h_full is not None:
                res_h = hm_info.resolution
                ox_h = hm_info.pose.position.x - hm_info.length_x / 2.0
                oy_h = hm_info.pose.position.y - hm_info.length_y / 2.0
                Hh, Wh = h_full.shape
                h_patch = np.zeros_like(norm_h)
                for jj in range(PATCH):
                    y_world = cp[1] + (jj - PATCH//2) * info.resolution
                    jh = int((y_world - oy_h) / res_h)
                    if not (0 <= jh < Hh):
                        continue
                    for ii in range(PATCH):
                        x_world = cp[0] + (ii - PATCH//2) * info.resolution
                        ih = int((x_world - ox_h) / res_h)
                        if 0 <= ih < Wh:
                            h_patch[jj, ii] = h_full[jh, ih]
                h_mean = float(np.nanmean(h_patch))
                norm_h = np.clip((h_patch - h_mean) / MAX_HEIGHT, -1.0, 1.0)

        gx, gy = np.gradient(norm_h)
        patch_local = np.stack([norm_occ, gx, gy], axis=-1)
        return patch_local.astype(np.float32), arr, info

    # # ---------- Extracción de parche Global (64×64×3)  -----------
    # def extract_big_patch(self):
    #     """
    #     Similar a extract_patch, pero crea parche 64×64 para meta-policy.
    #     Usa zoom ↓2 sobre el parche local para obtener 64×64.
    #     """
    #     patch_local, arr, info = self.extract_patch()  # normalizado occ + gx,gy
    #     patch_down = zoom(patch_local, (BIG_PATCH / PATCH, BIG_PATCH / PATCH, 1), order=1)
    #     return patch_down.astype(np.float32), arr, info

    def curr_R_META(self):
        # simplísimo: 0-30 eps 2 m, 30-100 eps 4 m, resto 8 m
        if self.meta_episode < 30:
            return 2.0
        elif self.meta_episode < 100:
            return 4.0
        else:
            return 8.0





    # ---------- Selección de target (base frontier/goal)  -----------
    def select_frontier(self, cp, grid, info):
        """
        Si goal visible → retorna goal; si no, retorna frontier más cercano a goal.
        Si no hay frontiers → None.
        """
        if self.goal is not None:
            gp = self.goal
            if bres_line_free(grid, info, cp, gp):
                return gp, "GOAL"
        else:
            gp = None

        if not self.frontiers:
            return None, "NONE"

        remaining = [
            f for f in self.frontiers
            if f not in self.visited_frontiers and f not in self.bad_frontiers
        ]
        if not remaining:
            return None, "NONE"

        if gp is not None:
            best = min(remaining, key=lambda f: math.hypot(f[0] - gp[0], f[1] - gp[1]))
            return best, "FRONTIER"

        return None, "NONE"

    # ---------- Publicar path y markers en RViz  -----------
    def publish_path(self, pts):
        hdr = Header(frame_id="map", stamp=self.get_clock().now().to_msg())
        path_msg = Path(header=hdr)
        for x, y in pts:
            ps = PoseStamped(header=hdr)
            ps.pose.position.x = x; ps.pose.position.y = y
            ps.pose.orientation.w = 1.0
            path_msg.poses.append(ps)
        self.path_pub.publish(path_msg)

        mk = Marker(header=hdr, ns="wps", id=0, type=Marker.POINTS, action=Marker.ADD)
        mk.scale = Vector3(x=0.15, y=0.15, z=0.0)
        mk.color.r = mk.color.g = 1.0; mk.color.a = 1.0
        mk.points = [Point(x=x, y=y, z=0.1) for x, y in pts[1:]]
        self.wps_pub.publish(mk)

    # ---------- RRT* cost + filtrar curvatura + suavizar/densificar  -----------
    def generate_flexible_path_cost(self, start, target, grid, info):
        """
        Usa rrt_plan_cost para planear → filtra curvatura, suaviza con B-spline y densifica.
        Si falla, retorna [start, target].
        """
        h_arr, hm_info = gridmap_to_numpy(self.height_map_msg) if self.height_map_msg else (None, None)
        path_rrt = rrt_plan_cost(start, target, grid, info, h_arr, hm_info,
                                 max_iter=2000, step=2.0, goal_tol=GOAL_RADIUS)
        if len(path_rrt) < 4:
            return [start, target]

        # 1) Filtrar curvatura imposible
        path_rrt = _filter_curv(path_rrt)
        # 2) Suavizar B-spline
        path_bs = smooth_bspline(path_rrt, ds=0.25)
        # 3) Densificar
        dens = densify(path_bs)
        return dens

    # ---------- Pure-Pursuit con marcha atrás y look-ahead dinámico (Low-Level) -----------
    def _next_target_index(self, cp, look_ahead):
        idx = self.wp_index
        while idx + 1 < len(self.current_path) and l2(cp, self.current_path[idx]) < look_ahead:
            idx += 1
        return idx

    def follow_path(self, cp):
        """
        Pure-Pursuit ext.: 
          - Si α (ángulo local) > BACK_ALPHA_MIN, activa reversa (throttle negativo, giro invertido).
          - Look-ahead dinámico: Ld = K1_LD·v + K2_LD/|κ|.
          - Re-planificación si cross-track error > REPLAN_DIST.
        """
        if self.wp_index >= len(self.current_path):
            self.cmd_pub.publish(self._brake())
            return

        # --- coordenadas en frame local -----------------------------------
        tgt = self.current_path[self.wp_index]
        dx_g, dy_g = tgt[0] - cp[0], tgt[1] - cp[1]
        yaw = self._yaw_from_quaternion(self.pose.orientation)
        dx, dy = self._global_to_local(dx_g, dy_g, yaw)
        dist   = math.hypot(dx, dy)
        alpha  = math.atan2(dy, dx)           # [-π, π]

        # ------ ¿Reversa? ---------------------------------------------------
        reverse = False
        if abs(alpha) > BACK_ALPHA_MIN:
            reverse = True
            if alpha > 0:
                alpha -= math.pi
            else:
                alpha += math.pi
            dist = max(dist, 1e-3)

        # --- curvatura local -----------------------------------------------
        kappa = 2.0 * math.sin(alpha) / max(dist, 1e-3)

        # --- look-ahead dinámico --------------------------------------------
        v_curr = max(abs(self.twist.linear.x), 0.05)
        Ld = np.clip(K1_LD * v_curr + K2_LD / (abs(kappa) + 1e-3), 1.0, 6.0)

        self.wp_index = self._next_target_index(cp, Ld)
        tgt = self.current_path[self.wp_index]
        dx_g, dy_g = tgt[0] - cp[0], tgt[1] - cp[1]
        dx, dy = self._global_to_local(dx_g, dy_g, yaw)
        dist   = math.hypot(dx, dy)
        alpha  = math.atan2(dy, dx)

        # Recalcular curvatura
        kappa = 2.0 * math.sin(alpha) / max(dist, 1e-3)
        if reverse:
            kappa *= -1.0

        delta_des = math.atan(WHEEL_BASE * kappa)
        if reverse:
            delta_des *= -1.0

        str_pre = -(delta_des / DELTA_MAX) * MAX_STEERING
        str_pre = np.clip(str_pre, -MAX_STEERING, MAX_STEERING)

        # --- Velocidad deseada -----------------------------------------------
        if reverse:
            v_des = -np.clip(V_REV_MAX * (1 - 0.6 * abs(kappa)), V_MIN, V_REV_MAX)
        else:
            v_des = np.clip(V_MAX * (1 - 0.6 * abs(kappa)), V_MIN, V_MAX)

        thr_pre = KP_SPEED * (v_des - v_curr) + THR_IDLE * np.sign(v_des)

        # --- Filtro crítico-amortiguado --------------------------------------
        TAU, dt = 0.25, 0.05
        a1 = 2 * TAU / (2 * TAU + dt)
        b1 = dt / (2 * TAU + dt)
        str_cmd = a1 * str_pre + b1 * (str_pre - self.last_cmd.steering)
        thr_cmd = a1 * thr_pre + b1 * (thr_pre - self.last_cmd.throttle)

        # Limita rampa longitudinal
        d_thr = np.clip(thr_cmd - self.last_cmd.throttle, -MAX_DTHR, MAX_DTHR)
        thr_cmd = self.last_cmd.throttle + d_thr

        # Saturaciones finales
        str_cmd = np.clip(str_cmd, -MAX_STEERING, MAX_STEERING)

        # --- Re-plan si cross-track error > REPLAN_DIST ----------------------
        cross_track = abs(dist * math.sin(alpha))
        if cross_track > REPLAN_DIST and not self.escaping:
            self.get_logger().info(f"[REPLAN] error lateral {cross_track:.2f} m → nuevo RRT")
            self.need_new_wp = True
            return

        # --- Publicar comando -----------------------------------------------
        cmd = CmdThrottleMsg()
        cmd.throttle = thr_cmd
        cmd.steering = str_cmd
        self.cmd_pub.publish(cmd)
        self.last_cmd = cmd

        self.get_logger().info(
            f"[CTRL {'REV' if reverse else 'FWD'}] wp={self.wp_index}/{len(self.current_path)-1} "
            f"α={alpha*57.3:+.1f}° k={kappa:.3f} v={v_curr:.2f}->{v_des:.2f} "
            f"thr={thr_cmd:.1f} str={str_cmd:.1f}"
        )

    # ---------- Conversión global→local y cuaterniones -----------
    def _global_to_local(self, dx, dy, yaw):
        cos_y, sin_y = math.cos(-yaw), math.sin(-yaw)
        return dx * cos_y - dy * sin_y, dx * sin_y + dy * cos_y

    def _yaw_from_quaternion(self, q):
        siny = 2.0 * (q.w * q.z + q.x * q.y)
        cosy = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
        return math.atan2(siny, cosy)

    # ---------- Cálculo de recompensa (Low-Level) -----------

    # def compute_reward_meta(self, dist_old, dist_new, total_dz, collided,
    #                         wp_rel, vec_goal):
    #     """
    #     Nueva reward meta:
    #     dist_old - dist_new   (progreso)
    #     - 0.1·total_dz        (coste subida)
    #     direccional = 5·cosθ  (alineación)
    #     """
    #     r = dist_old - dist_new
    #     r -= 0.1 * total_dz
    #     if collided:
    #         r -= 50.0
    #     # dirección global
    #     dot = np.dot(wp_rel, vec_goal*20)
    #     cos = dot / (np.linalg.norm(wp_rel) * (np.linalg.norm(vec_goal*20)+1e-6))
    #     r += 5.0 * cos
    #     return r

    def compute_reward_meta(self, dist_old, dist_new, total_dz,
                        collided, wp_rel, vec_goal):
        """
        • Recompensa por acercarse al goal (dist_old - dist_new)
        • Penaliza desnivel total (total_dz)
        • Penaliza colisión
        • Bonifica alineación coseno(wp_rel, vec_goal)
        """
        r = (dist_old - dist_new) * 2.0           # progreso
        r -= 0.1 * total_dz                       # coste energético
        if collided:  r -= 50.0
        # bonificación direccional
        cos_dir = np.dot(wp_rel, vec_goal) / (np.linalg.norm(wp_rel) *
                                            np.linalg.norm(vec_goal) + 1e-6)
        r += 5.0 * cos_dir
        return r





    def compute_reward_low(self, old_d, new_d, collided, reached, step_len,
                           overturned, min_obs_dist, slope, dz):
        """
        Recompensa local (bajo nivel):
         - +2·(old_d - new_d) → progreso al waypoint.
         - -0.05 coste por paso.
         - -0.1·step_len (castigo por distancia recorrida).
         - +200 al alcanzar waypoint (reached).
         - -200 colisión / vuelco.
         - -1/(min_obs_dist + ε) penaliza obstáculos cercanos.
         - -10·slope penaliza pendientes.
         - si dz>0: -m·g·dz·scale penaliza energía para subir.
        """
        r = 2.0 * (old_d - new_d)
        r -= 0.05
        r -= 0.1 * step_len
        if reached:    r += 200.0
        if collided:   r -= 200.0
        if overturned: r -= 200.0
        r -= 1.0 / (min_obs_dist + 1e-3)
        r -= 10.0 * slope
        if dz > 0:
            dE = M_ROBOT * G * dz
            r -= dE * ENERGY_SCALE
        return r

    # # ---------- Cálculo de recompensa (Meta-Level) -----------
    # def compute_reward_meta(self, dist_old, dist_new, total_dz, collided):
    #     """
    #     Recompensa meta:
    #      - + (dist_old - dist_new): progreso hacia objetivo global.
    #      - -0.1·total_dz: coste acumulado de subida en el sub-episodio.
    #      - -50 si collided.
    #     """
    #     r = dist_old - dist_new
    #     r -= 0.1 * total_dz
    #     if collided:
    #         r -= 50.0
    #     return r

    # ---------- Distancias a obstáculos en parche local -----------
    def compute_obs_distances(self, patch_occ, info):
        R = PATCH // 2
        front = patch_occ[R - 4:R + 4, R:]
        left  = patch_occ[R:, 0:8]
        right = patch_occ[R:, PATCH - 8:]
        def min_dist(slice_):
            occ = np.where(slice_ >= 0, slice_, np.inf)
            if np.all(np.isinf(occ)):
                return np.inf
            idx = np.unravel_index(np.argmin(occ), occ.shape)
            return (idx[1] * info.resolution) if slice_ is front else (idx[0] * info.resolution)
        return min_dist(front), min_dist(left), min_dist(right)

    def _roll_from_quat(self, q):
        sinr = 2.0 * (q.w * q.x + q.y * q.z)
        cosr = 1.0 - 2.0 * (q.x * q.x + q.y * q.y)
        return math.atan2(sinr, cosr)

    def _pitch_from_quat(self, q):
        sinp = 2.0 * (q.w * q.y - q.z * q.x)
        return math.asin(max(-1.0, min(1.0, sinp)))

    # ---------- Buffers Low-Level -------------
    def reset_buffers(self):
        self.patch_buf = []
        self.state_buf = []
        self.act_buf   = []
        self.logp_buf  = []
        self.rew_buf   = []
        self.val_buf   = []
        self.done_buf  = []

    # ---------- Buffers Meta-Level -------------
    def reset_meta_buffers(self):
        self.meta_obs_buf   = []
        self.meta_act_buf   = []
        self.meta_logp_buf  = []
        self.meta_rew_buf   = []
        self.meta_val_buf   = []
        self.meta_done_buf  = []

    # ---------- Método de escape (baja velocidad hacia atrás) -----------
    def find_escape_point(self, cp, yaw, grid, info, h_arr, hm_info):
        """
        Genera punto de escape detrás del robot (±30°) hasta R_META*1.2,
        que cumpla: línea libre, holgura mínima y pendiente aceptable.
        """
        back_vec = (-math.cos(yaw), -math.sin(yaw))
        for r in np.arange(0.5, R_META * 1.2, 0.3):
            for dθ in np.linspace(-math.radians(30), math.radians(30), 7):
                vx = math.cos(dθ) * back_vec[0] - math.sin(dθ) * back_vec[1]
                vy = math.sin(dθ) * back_vec[0] + math.cos(dθ) * back_vec[1]
                cand = (cp[0] + r * vx, cp[1] + r * vy)
                if not bres_line_free(grid, info, cp, cand):
                    continue
                if not clearance_ok(grid, info, cand, CLEAR_MIN):
                    continue
                if h_arr is not None:
                    _, slope, _ = slope_ok(h_arr, hm_info, cp, cand)
                    if slope > SLOPE_MAX:
                        continue
                return cand
        return None




    def in_reset_grace(self):
        if self.reset_t0 is None:
            return False
        dt = (self.get_clock().now() - self.reset_t0).nanoseconds * 1e-9
        return dt < RESET_GRACE_SEC




    # ---------- Pure‐Pursuit + lógica jerárquica (10 Hz) -----------
    def step(self):
        """
        Callback periódico (10 Hz). Gestiona dos fases:
          1) ESCAPE guiado si se detectó colisión o atascamiento.
          2) Meta-phase: elige waypoint según goal/frontier/meta-policy.
          3) Low-phase: sigue el waypoint con Pure-Pursuit y acumula recompensas.
        """





        #-- Pre todo , comprobacion de todo antes de emepazr
        
        # Si estamos esperando la confirmación, solo hacemos brake y salimos
        if self.in_reset_grace():
            self._brake()              # mantén al robot quieto
            return    

        

        # --- Si el robot aún no está dentro del grid, avanza despacio -------------
        if not self.robot_inside_grid():
            self.get_logger().info("[STARTUP] robot aún fuera del OccupancyGrid: avanzando despacio hasta entrar…")
            self.slow_forward(0.25)       # 0.25 m/s
            return                        # saltamos el resto del ciclo




        # --- 1) ESCAPE guiado (bajo nivel) ----------------------------------
        if self.escaping:
            if self.escape_timer <= 0:
                self.escaping = False
            else:
                cp = (self.pose.position.x, self.pose.position.y)
                self.follow_path(cp)
                if l2(cp, self.escape_path[-1]) < 0.4:
                    self.escaping = False
                self.escape_timer -= 1
            if self.current_path:
                self.publish_path(self.current_path)
            return

        # --- 2) Validar pose/mapas/goal ------------------------------------
        if None in (self.pose, self.grid_msg, self.height_map_msg, self.goal):
            if self.current_path:
                self.publish_path(self.current_path)
            return
        cp = (self.pose.position.x, self.pose.position.y)

        # --- 2.a) Curriculum: ajustar R_META según episodio meta -------------
        if self.meta_episode < 30:
            R_use = 2.0
        elif self.meta_episode < 100:
            R_use = 4.0
        else:
            R_use = R_META  # valor original (por ej. 8.0)

        # --- 3) Meta-phase: elegir nuevo waypoint ---------------------------
        if self.need_new_wp:
            # 3.a) ¿Goal visible?
            if self.goal is not None and bres_line_free(self.grid_dyn, self.grid_msg.info, cp, self.goal):
                waypoint    = self.goal
                modo_origen = "GOAL_DIRECTO"
                self.current_modo = "GOAL_DIRECTO"
                wp_rel      = (waypoint[0] - cp[0], waypoint[1] - cp[1])
                self.last_wp_rel = np.array(wp_rel, dtype=np.float32)
                wp_rel   = np.array([waypoint[0]-cp[0], waypoint[1]-cp[1]], np.float32)
                big_patch, _ , _ = self.extract_big_patch()    # 64×64×4 listo
                vec_in   = wp_rel / (np.linalg.norm(wp_rel)+1e-6) * 0  # sirve sólo como placeholder
                # guarda en los buffers meta como si fuera una acción “tomada”
                self.meta_obs_buf.append((big_patch, vec_in))
                self.meta_act_buf.append(wp_rel / R_use)   # => acción en rango [-1,1]
                self.meta_logp_buf.append(0.0)             # logp ficticio
                self.meta_val_buf.append(0.0)              # critic ficticio
                # recompensa ficticia (0) y done = True para cortar el sub-episodio
                self.meta_rew_buf.append(0.0)
                self.meta_done_buf.append(True)

            else:
                # 3.b) ¿Frontier válido?
                frontier, _ = self.select_frontier(cp, self.grid_dyn, self.grid_msg.info)
                if frontier is not None:
                    waypoint    = frontier
                    modo_origen = "FRONTIER"
                    self.current_modo = "FRONTIER"
                    wp_rel      = (waypoint[0] - cp[0], waypoint[1] - cp[1])
                    wp_rel   = np.array([waypoint[0]-cp[0], waypoint[1]-cp[1]], np.float32)
                    big_patch, _ , _ = self.extract_big_patch()    # 64×64×4 listo
                    vec_in   = wp_rel / (np.linalg.norm(wp_rel)+1e-6) * 0  # sirve sólo como placeholder
                    # guarda en los buffers meta como si fuera una acción “tomada”
                    self.meta_obs_buf.append((big_patch, vec_in))
                    self.meta_act_buf.append(wp_rel / R_use)   # => acción en rango [-1,1]
                    self.meta_logp_buf.append(0.0)             # logp ficticio
                    self.meta_val_buf.append(0.0)              # critic ficticio
                    # recompensa ficticia (0) y done = True para cortar el sub-episodio
                    self.meta_rew_buf.append(0.0)
                    self.meta_done_buf.append(True)

                else:
                    # 3.c) Meta-policy (PPO)
                    # Extraer parche grande 64×64 con cuarto canal para el goal
                    patch_local, _, _ = self.extract_patch()  # 128×128×3
                    patch_down = zoom(patch_local, (BIG_PATCH / PATCH, BIG_PATCH / PATCH, 1), order=1)
                    vec_goal = np.array([self.goal[0] - cp[0], self.goal[1] - cp[1]], np.float32)
                    vec_goal_norm = vec_goal / (np.linalg.norm(vec_goal) + 1e-6)

                    # Construir capa one-hot del goal en la cuadricula 64×64
                    goal_layer = np.zeros((BIG_PATCH, BIG_PATCH), np.float32)
                    # Aproximar la posición del goal en el parche: centrado en robot
                    center_idx = BIG_PATCH // 2
                    gi = int(center_idx + (R_use and (vec_goal[0] / (R_use * 1.0))) * center_idx)
                    gj = int(center_idx + (R_use and (vec_goal[1] / (R_use * 1.0))) * center_idx)
                    rr = 2
                    if 0 <= gi < BIG_PATCH and 0 <= gj < BIG_PATCH:
                        goal_layer[max(0, gj - rr): gj + rr + 1,
                                   max(0, gi - rr): gi + rr + 1] = 1.0

                    # Concatenar parche reducido 64×64×3 con goal_layer → 64×64×4
                    big_patch = np.dstack([patch_down, goal_layer]).astype(np.float32)

                    # Vector al goal normalizado para la red
                    vec_in = (vec_goal / (R_use * 1.0)).astype(np.float32)[None, ...]

                    sigma_m = tf.exp(self.meta_log_std)
                    mu_m    = tf.squeeze(self.meta_policy([big_patch[None, ...], vec_in]), 0)
                    eps     = tf.random.normal(shape=mu_m.shape)
                    act_m   = tf.clip_by_value(mu_m + sigma_m * eps, -1.0, 1.0)
                    logp_m  = -0.5 * tf.reduce_sum(((act_m - mu_m) / sigma_m) ** 2
                                                   + 2 * tf.math.log(sigma_m)
                                                   + tf.math.log(2 * math.pi))
                    logp_meta = float(logp_m.numpy())
                    val_meta  = float(self.meta_value_net([big_patch[None, ...], vec_in])[0, 0])

                    # Escalar desplazamiento a R_use y garantizar distancia mínima
                    wp_rel = (R_use * act_m.numpy()).astype(float)
                    norm  = np.linalg.norm(wp_rel)
                    if norm < MIN_WP_DIST:
                        if norm < 1e-6:
                            wp_rel = vec_goal_norm * MIN_WP_DIST
                        else:
                            wp_rel = wp_rel / norm * MIN_WP_DIST

                    waypoint    = (cp[0] + wp_rel[0], cp[1] + wp_rel[1])
                    modo_origen = "META_RNN"
                    self.current_modo = "META_RNN"

                    # Guardar sólo si proviene de meta-policy
                    self.meta_obs_buf.append((big_patch, vec_in[0]))
                    self.meta_act_buf.append(act_m.numpy())
                    self.meta_logp_buf.append(logp_meta)
                    self.meta_val_buf.append(val_meta)

            # Publicar waypoint + planificar localmente
            self.current_waypoint = waypoint
            self.publish_waypoint_marker(waypoint, modo_origen)

            patch_local2, grid, info = self.extract_patch()
            path_rrt = self.generate_flexible_path_cost(cp, waypoint, grid, info)
            self.current_path = path_rrt
            self.wp_index     = 1
            self.need_new_wp  = False

            # Inicializar buffers meta para sub-episodio
            self.meta_start_dist = l2(cp, self.goal)
            self.meta_total_dz   = 0.0
            self.meta_collided   = False

            self.publish_path(path_rrt)
            self.get_logger().info(f"[META] waypoint={waypoint} modo={modo_origen}")

        # --- 4) Detectar “no progreso” (atascado) ---------------------------
        if self.last_cp is not None:
            dist_moved = l2(cp, self.last_cp)
            if dist_moved < MIN_MOVEMENT:
                self.no_progress_counter += 1
            else:
                self.no_progress_counter = 0
        self.last_cp = cp

        if self.no_progress_counter >= NO_PROGRESS_LIMIT:
            self.get_logger().warning(f"[STUCK] sin avanzar {NO_PROGRESS_LIMIT} pasos")
            yaw = self._yaw_from_quaternion(self.pose.orientation)
            h_arr, hm_info = gridmap_to_numpy(self.height_map_msg)
            esc_pt = self.find_escape_point(cp, yaw,
                                           self.grid_dyn, self.grid_msg.info,
                                           h_arr, hm_info)
            if esc_pt:
                self.escape_path  = [cp, esc_pt]
                self.current_path = self.escape_path
                self.escaping     = True
                self.escape_timer = 60
                self.wp_index     = 1
                self.publish_path(self.escape_path)
                self.get_logger().info(f"[ESCAPE-STUCK] point={esc_pt}")
            else:
                self.get_logger().warning("[ESCAPE-STUCK] no free point")
            self.no_progress_counter = 0
            return

        # --- 5) Low-phase: seguir path con Pure-Pursuit --------------------
        if not self.current_path:
            return

        patch_local3, grid, info = self.extract_patch()
        tgt     = self.current_waypoint
        old_d   = l2(cp, tgt)
        # ➊ ¿hemos llegado?
        if self.goal_reached_flag:
            self._brake()
            self.goal_reached_flag = True          # por si viene de distancia
            reached_low= True
            done_low = True                        # fuerza fin de episodio
        else:
            self.follow_path(cp)                   # ← sólo si NO hemos llegado
            reached_low=False
            done_low = (self.step_count >= MAX_STEPS_EP)
        wp      = self.current_path[self.wp_index]
        new_d   = l2(wp, tgt)

        # Detectar vuelco
        roll  = self._roll_from_quat(self.pose.orientation)
        pitch = self._pitch_from_quat(self.pose.orientation)
        overturned = abs(roll) > MAX_TILT or abs(pitch) > MAX_TILT
        if self.in_reset_grace():
            overturned = False
        if overturned:
            self.get_logger().warning("🚨 Robot volcado")
            self.reset_pub.publish(Bool(data=True))
            self.waiting_reset = True
            self.meta_collided = True
            self._brake()
            return

        d_front, d_left, d_right = self.compute_obs_distances(patch_local3[:, :, 0], info)
        min_obs_dist = min(d_front, d_left, d_right)

        h_arr, hm_info = gridmap_to_numpy(self.height_map_msg)
        if h_arr is not None:
            _, slope, dz = slope_ok(h_arr, hm_info, cp, wp)
        else:
            slope, dz = 0.0, 0.0

        self.step_count += 1
        self.meta_total_dz += dz

        collided_low = self.collided
        reached_low  = self.goal_reached_flag

        reward_low = self.compute_reward_low(
            old_d, new_d,
            collided_low, reached_low,
            step_len=l2(cp, wp),
            overturned=overturned,
            min_obs_dist=min_obs_dist,
            slope=slope,
            dz=dz
        )

        # Preparar tensores para red low-level
        patch_tensor = tf.convert_to_tensor(patch_local3[None, ...], tf.float32)
        state_vec    = np.array([tgt[0] - cp[0], tgt[1] - cp[1], d_front, d_left], np.float32)
        state_tensor = tf.convert_to_tensor(state_vec[None, ...], tf.float32)

        val_low = float(self.value_net(
            tf.concat([tf.reshape(patch_tensor, (1, -1)), state_tensor], axis=-1)
        )[0, 0])

        mu_low = self.policy([patch_tensor, state_tensor, tf.zeros((1, 2), tf.float32)])
        act_low = tf.squeeze(mu_low, axis=0).numpy()
        sigma_low = tf.exp(self.log_std)
        logp_low = -0.5 * tf.reduce_sum(((act_low - mu_low[0]) / sigma_low) ** 2
                                        + 2 * tf.math.log(sigma_low)
                                        + tf.math.log(2 * math.pi))
        logp_low = float(logp_low.numpy())

        self.patch_buf.append(patch_local3.astype(np.float32))
        self.state_buf.append(state_vec)
        self.act_buf.append(act_low.astype(np.float32))
        self.logp_buf.append(np.float32(logp_low))
        self.rew_buf.append(np.float32(reward_low))
        self.val_buf.append(np.float32(val_low))

        done_low = reached_low or overturned or (self.step_count >= MAX_STEPS_EP)
        self.done_buf.append(bool(done_low))

        # --- 6) Fin sub-episodio Low → entrenar ambos niveles --------------
        if done_low:
            self.get_logger().info(f"Entrenando")
            

            motivo = ("reached" if reached_low else
                      "collided" if collided_low else
                      "overturned" if overturned else
                      "timeout")
            self.get_logger().info(
                f"--- EPISODIO {self.episode+1} terminado → {motivo} "
                f"(steps={self.step_count}, reward_total={sum(self.rew_buf):.1f})"
            )


            self._brake()

            dist_new = l2(cp, self.goal)
            reward_meta = self.compute_reward_meta(
                dist_old=self.meta_start_dist,
                dist_new=dist_new,
                total_dz=self.meta_total_dz,
                collided=self.meta_collided or collided_low,
                wp_rel=self.last_wp_rel,                 # desplazamiento real usado
                vec_goal=np.array([self.goal[0] - cp[0], self.goal[1] - cp[1]], np.float32)
            )
            val_meta_end = 0.0

            # Solo añadir rewards/meta_val al buffer si la modalidad meta fue META_RNN
            if self.current_modo == "META_RNN":
                self.meta_rew_buf.append(np.float32(reward_meta))
                self.meta_val_buf.append(np.float32(val_meta_end))
                self.meta_done_buf.append(True)


            

            

            # Actualizar PPO de ambos niveles
            self.update_ppo()
            self.update_meta_ppo()
            self.reset_meta_buffers()

            if self.goal_reached_flag:
                self.meta_episode += 1
                if self.meta_episode >= MAX_EPISODES:
                    self.get_logger().info(f"Meta-policy {MAX_EPISODES} episodios completos. Saliendo.")
                    rclpy.shutdown()
            
            # Reiniciar buffers low-level
            self.step_count = 0
            self.reset_buffers()
            self.collided      = False
            self.goal_reached_flag = False

            if self.current_path:
                self.publish_path(self.current_path)

            if not self.goal_reached_flag:
                self.need_new_wp = True
            
            return
        # # --- 7) Actualizar low-level PPO cada 50 pasos ---------------------
        # if self.step_count % 50 == 0 and len(self.patch_buf) >= BATCH_SZ:
        #     self.get_logger().info("Update the low model")
        #     self._brake()
           
        #     self.update_ppo()

        # --- 8) Log de debug Low-Level cada 10 pasos -----------------------
        if self.step_count % 10 == 0:
            self.get_logger().debug(
                f"[LOW] Paso {self.step_count}/{MAX_STEPS_EP} old_d={old_d:.2f} new_d={new_d:.2f} r={reward_low:.2f}"
            )

        # ───────────── Republicar path en cada ciclo ─────────────
        if self.current_path:
            self.publish_path(self.current_path)



    #---------- publicar los waypoints --------------
    def publish_waypoint_marker(self, waypoint, modo):
        """
        Publica en RViz un Marker esférico para el waypoint dado,
        coloreado según el modo de selección:
        • "GOAL_DIRECTO": verde
        • "FRONTIER":    amarillo
        • "META_RNN":    azul
        """
        mk = Marker()
        mk.header.frame_id = "map"
        mk.header.stamp = self.get_clock().now().to_msg()
        mk.ns     = "current_wp"
        mk.id     = 1
        mk.type   = Marker.SPHERE
        mk.action = Marker.ADD

        # Posición del waypoint
        mk.pose.position.x = waypoint[0]
        mk.pose.position.y = waypoint[1]
        mk.pose.position.z = 0.2
        mk.pose.orientation.w = 1.0

        # Tamaño de la esfera
        mk.scale.x = 0.4
        mk.scale.y = 0.4
        mk.scale.z = 0.4

        # Color según modo
        if modo == "GOAL_DIRECTO":
            mk.color = ColorRGBA(r=0.0, g=1.0, b=0.0, a=1.0)
        elif modo == "FRONTIER":
            mk.color = ColorRGBA(r=1.0, g=1.0, b=0.0, a=1.0)
        else:  # META_RNN
            mk.color = ColorRGBA(r=0.0, g=0.0, b=1.0, a=1.0)

        # Publicar el Marker
        self.waypoint_pub.publish(mk)




    # ---------- Entrenamiento PPO Low-Level -----------
    def update_ppo(self):


        # n_obs = len(self.meta_obs_buf)
        # if n_obs == 0 \
        #    or not (n_obs == len(self.meta_act_buf) == len(self.meta_logp_buf)
        #            == len(self.meta_rew_buf) == len(self.meta_val_buf) == len(self.meta_done_buf)):
        #     return


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

        patches = np.stack(self.patch_buf).astype(np.float32)    # (N,128,128,3)
        states  = np.stack(self.state_buf).astype(np.float32)    # (N,4)
        acts    = np.stack(self.act_buf).astype(np.float32)      # (N,2)
        logps   = np.array(self.logp_buf, np.float32)
        advs    = advs.astype(np.float32)
        rets    = returns.astype(np.float32)

        dataset = tf.data.Dataset.from_tensor_slices((patches, states, acts, logps, advs, rets))
        dataset = dataset.shuffle(4096).batch(BATCH_SZ)

        for _ in range(EPOCHS):
            for g, st, act, lp_old, adv, ret in dataset:
                with tf.GradientTape() as tpi, tf.GradientTape() as tpv:
                    w0_dummy = tf.zeros((tf.shape(act)[0], 2), tf.float32)
                    mu  = self.policy([g, st, w0_dummy], training=True)
                    std = tf.exp(self.log_std)

                    lp = -0.5 * tf.reduce_sum(((act - mu)/std)**2 +
                                              2 * tf.math.log(std) +
                                              tf.math.log(2 * math.pi), axis=-1)
                    ratio = tf.exp(lp - lp_old)
                    pg_loss = -tf.reduce_mean(tf.minimum(ratio * adv,
                                        tf.clip_by_value(ratio, 1 - CLIP_EPS, 1 + CLIP_EPS) * adv))

                    patch_flat = tf.reshape(g, (tf.shape(g)[0], -1))
                    state_vec = tf.concat([patch_flat[:, :PATCH * PATCH * 3], st], axis=-1)
                    v_pred = tf.squeeze(self.value_net(state_vec, training=True), axis=-1)
                    v_loss = tf.reduce_mean((ret - v_pred)**2)

                grads_pi = tpi.gradient(pg_loss, self.policy.trainable_variables + [self.log_std])
                self.opt_actor.apply_gradients(zip(grads_pi, self.policy.trainable_variables + [self.log_std]))
                grads_v = tpv.gradient(v_loss, self.value_net.trainable_variables)
                self.opt_critic.apply_gradients(zip(grads_v, self.value_net.trainable_variables))

        new_std = tf.maximum(tf.exp(self.log_std) * STD_DECAY, STD_MIN)
        self.log_std.assign(tf.math.log(new_std))

        with self.writer.as_default():
            tf.summary.scalar("low/episode_reward", sum(self.rew_buf), step=self.episode)
            tf.summary.scalar("low/loss_actor", float(pg_loss),    step=self.episode)
            tf.summary.scalar("low/loss_critic", float(v_loss),    step=self.episode)
            tf.summary.scalar("low/policy_std", float(new_std[0]), step=self.episode)
        self.policy.save_weights(RUN_DIR / f"policy_low_ep{self.episode}_{self.ts}.weights.h5")
        self.get_logger().info(f"[LOW PPO] ep={self.episode} σ={float(new_std[0]):.3f}")
        self.episode += 1
        self.total_updates += 1

    # ---------- Entrenamiento PPO Meta-Level -----------
    def update_meta_ppo(self):



        self.get_logger().info(f"[META PPO] Actualizando meta-policy (episodio {self.meta_episode})")
        self.get_logger().info(
            f"[META PPO] {len(self.meta_obs_buf)} muestras, "
            f"episodio={self.meta_episode}, total_updates={self.meta_total_upd}"
        )
        n_obs = len(self.meta_obs_buf)
        if n_obs == 0 \
           or not (n_obs == len(self.meta_act_buf) == len(self.meta_logp_buf)
                   == len(self.meta_rew_buf) == len(self.meta_val_buf) == len(self.meta_done_buf)):
            return


        returns, advs = [], []
        gae = 0.0
        next_val = 0.0
        for r, v, d in zip(reversed(self.meta_rew_buf),
                           reversed(self.meta_val_buf),
                           reversed(self.meta_done_buf)):
            delta = r + GAMMA * next_val * (1 - d) - v
            gae   = delta + GAMMA * GAE_LAMBDA * (1 - d) * gae
            advs.insert(0, gae)
            next_val = v
        returns = np.array(advs) + np.array(self.meta_val_buf)
        advs    = (np.array(advs) - np.mean(advs)) / (np.std(advs) + 1e-8)

        big_patches = np.stack([obs[0] for obs in self.meta_obs_buf]).astype(np.float32)  # (M,64,64,3)
        vec_goals   = np.stack([obs[1] for obs in self.meta_obs_buf]).astype(np.float32)  # (M,2)
        acts_meta   = np.stack(self.meta_act_buf).astype(np.float32)                       # (M,2)
        logps_meta  = np.array(self.meta_logp_buf, np.float32)
        advs_meta   = advs.astype(np.float32)
        rets_meta   = returns.astype(np.float32)

        dataset = tf.data.Dataset.from_tensor_slices((big_patches, vec_goals, acts_meta, logps_meta, advs_meta, rets_meta))
        dataset = dataset.shuffle(4096).batch(BATCH_SZ)

        for _ in range(EPOCHS):
            for Gp, vg, act_m, lp_old_m, adv_m, ret_m in dataset:
                with tf.GradientTape() as tpi, tf.GradientTape() as tpv:
                    mu_m = self.meta_policy([Gp, vg], training=True)
                    std_m = tf.exp(self.meta_log_std)

                    lp_m = -0.5 * tf.reduce_sum(((act_m - mu_m)/std_m)**2 +
                                                2 * tf.math.log(std_m) +
                                                tf.math.log(2 * math.pi), axis=-1)
                    ratio_m = tf.exp(lp_m - lp_old_m)
                    pg_loss_m = -tf.reduce_mean(tf.minimum(ratio_m * adv_m,
                                        tf.clip_by_value(ratio_m, 1 - CLIP_EPS, 1 + CLIP_EPS) * adv_m))

                    v_pred_m = tf.squeeze(self.meta_value_net([Gp, vg], training=True), axis=-1)
                    v_loss_m = tf.reduce_mean((ret_m - v_pred_m)**2)

                grads_pi_m = tpi.gradient(pg_loss_m, self.meta_policy.trainable_variables + [self.meta_log_std])
                self.opt_meta_actor.apply_gradients(zip(grads_pi_m, self.meta_policy.trainable_variables + [self.meta_log_std]))
                grads_v_m = tpv.gradient(v_loss_m, self.meta_value_net.trainable_variables)
                self.opt_meta_critic.apply_gradients(zip(grads_v_m, self.meta_value_net.trainable_variables))

        new_std_m = tf.maximum(tf.exp(self.meta_log_std) * STD_DECAY, STD_MIN)
        self.meta_log_std.assign(tf.math.log(new_std_m))

        with self.writer.as_default():
            tf.summary.scalar("meta/episode_reward", sum(self.meta_rew_buf), step=self.meta_episode)
            tf.summary.scalar("meta/loss_actor", float(pg_loss_m),    step=self.meta_episode)
            tf.summary.scalar("meta/loss_critic", float(v_loss_m),    step=self.meta_episode)
            tf.summary.scalar("meta/policy_std", float(new_std_m[0]), step=self.meta_episode)
        self.meta_policy.save_weights(RUN_DIR / f"policy_meta_ep{self.meta_episode}_{self.ts}.weights.h5")
        self.get_logger().info(f"[META PPO] ep={self.meta_episode} σ={float(new_std_m[0]):.3f}")
        self.meta_episode += 1
        self.meta_total_upd += 1

    def _brake(self):
        stop = CmdThrottleMsg()
        stop.throttle = 0.0
        stop.steering = 0.0
        self.cmd_pub.publish(stop)           # ← ¡publicar!
        self.last_cmd = stop                 # ← para que el filtro use 0
        

# ==============  MAIN  ====================================================
def main(args=None):
    rclpy.init(args=args)
    node = TerrainPPOTrainer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()