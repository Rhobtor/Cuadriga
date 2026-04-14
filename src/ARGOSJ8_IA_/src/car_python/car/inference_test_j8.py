# #!/usr/bin/env python3
# # -*- coding: utf-8 -*-
# """
# inference_test_j8.py  –  Nodo ROS 2 **solo-inferencia** para tu arquitectura
# jerárquica (meta-policy + low-policy).

# • Carga el *último* checkpoint *.weights.h5 del directorio dado.
# • Publica los mismos tópicos de depuración que el trainer:
#     /global_path_predicted  (verde)
#     /meta_wp                (flecha azul)
#     /goal_vec               (flecha roja)
# • Reutiliza el mismo controlador de seguimiento   follow_path()
#   importándolo desde TerrainPPOTrainer.
# """

# # ─────────────────────── imports genéricos ──────────────────────────────
# import argparse, pathlib, re, math, time, copy
# import numpy as np
# import rclpy
# from rclpy.node import Node

# # ROS-msgs
# from std_msgs.msg        import Header, ColorRGBA
# from geometry_msgs.msg   import Point, Vector3, PoseStamped, PoseArray
# from nav_msgs.msg        import Path, Odometry, OccupancyGrid
# from visualization_msgs.msg import Marker
# from grid_map_msgs.msg   import GridMap
# from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg

# # TensorFlow / Keras
# import tensorflow as tf
# from tensorflow.keras import layers as L

# META_PERIOD   = 10         # ciclos (10 Hz) ≈ 1 s
# # ─────────────────────── reutilizamos utilidades ────────────────────────
# from car.j8_test_ppo import (
#     TerrainPPOTrainer,                                 # para follow_path()
#     PATCH, BIG_PATCH, MIN_WP_DIST, R_META, CLEAR_MIN,  # const
#     WAYPOINT_RADIUS, l2, clearance_ok, densify,        # helpers
#     rrt_plan_cost, smooth_bspline, _filter_curv, gridmap_to_numpy,
#     slope_ok                                           # follow_path lo usa
# )
# def idx_from_world(info, pt):
#     if not np.isfinite(pt[0]) or not np.isfinite(pt[1]):
#         raise ValueError("idx_from_world recibió NaN")
#     res = info.resolution
#     return (int((pt[0] - info.origin.position.x) / res),
#             int((pt[1] - info.origin.position.y) / res))


# # ─────────────────────────  SAFE HELPERS  ──────────────────────────
# def bresenham_points(a, b, info):
#     """
#     Generador que itera sobre índices de celdas (i,j) entre dos puntos a,b usando Bresenham.
#     `info` es OccupancyGrid.info para cálculo de índices.
#     """
#     i0, j0 = idx_from_world(info, a)
#     i1, j1 = idx_from_world(info, b)
#     di, dj = abs(i1 - i0), abs(j1 - j0)
#     si, sj = (1 if i0 < i1 else -1), (1 if j0 < j1 else -1)
#     err = di - dj
#     while True:
#         yield i0, j0
#         if (i0, j0) == (i1, j1):
#             break
#         e2 = 2 * err
#         if e2 > -dj:
#             err -= dj; i0 += si
#         if e2 < di:
#             err += di; j0 += sj
# def bres_line_free(grid, info, a, b):
#     """
#     Comprueba que la línea entre puntos a y b (en coordenadas reales) traverse solo celdas libres
#     (valor < 50). Retorna False si choca o sale del mapa.
#     """
#     for i, j in bresenham_points(a, b, info):
#         if not (0 <= i < grid.shape[1] and 0 <= j < grid.shape[0]):
#             return False
#         if grid[j, i] == -1 or grid[j, i] >= 50:
#             return False
#     return True


# def safe_norm(v: np.ndarray, eps: float = 1e-6) -> float:
#     """||v|| con protección contra división-por-cero."""
#     n = float(np.linalg.norm(v))
#     return n if n > eps else eps           # nunca devuelve 0

# def safe_wp_rel(vec_t: np.ndarray,
#                 r_max: float,
#                 r_min: float = 0.8) -> np.ndarray:
#     """
#     Convierte el vector robo->target en un waypoint relativo (≤ r_max)
#     garantizando que:
#         • no contiene NaNs
#         • su módulo ≥ r_min
#     """
#     n = safe_norm(vec_t)
#     wp = vec_t / n * min(r_max, n)          # acota al radio
#     if np.linalg.norm(wp) < r_min:          # asegúrate de avanzar
#         wp = wp / safe_norm(wp) * r_min
#     return wp


# # ───────────────────────── modelos idénticos ────────────────────────────
# def build_low_policy():
#     """
#     Política local (bajo nivel) que recibe parche 128×128×3 + estado [dx_wp, dy_wp, d_front, d_left].
#     Produce 2 valores tanh(-1,1) = (Δ throttle, Δ steering).
#     """
#     g = tf.keras.Input(shape=(PATCH, PATCH, 3), name="grid")
#     st = tf.keras.Input(shape=(4,), name="state")
#     x = tf.keras.layers.Conv2D(16, 3, padding="same", activation="relu")(g)
#     x = tf.keras.layers.MaxPooling2D()(x)
#     x = tf.keras.layers.Conv2D(32, 3, padding="same", activation="relu")(x)
#     x = tf.keras.layers.GlobalAveragePooling2D()(x)
#     z = tf.keras.layers.Concatenate()([x, st])
#     h0 = tf.keras.layers.Dense(128, activation="tanh")(z)
#     c0 = tf.keras.layers.Dense(128, activation="tanh")(z)
#     lstm = tf.keras.layers.LSTMCell(128)
#     w0 = tf.keras.layers.Input(shape=(2,), name="w0")  # estado oculto inicial
#     h1, _ = lstm(w0, [h0, c0])
#     delta = tf.keras.layers.Dense(2, activation="tanh")(h1)
#     return tf.keras.Model([g, st, w0], delta, name="policy")

# def build_meta_policy():
#     """
#     Política de alto nivel que recibe parche 64×64×3 (global) + vector [dx_goal, dy_goal].
#     Produce (Δx, Δy) en [-1,1], se escala por R_META.
#     """
#     # Ahora input shape=(BIG_PATCH,BIG_PATCH,5) (3 occ+1 goal+1 entropy)
#     G = tf.keras.Input(shape=(BIG_PATCH, BIG_PATCH, 6), name="big_patch")
#     v = tf.keras.Input(shape=(2,), name="vec_goal")
#     x = tf.keras.layers.Conv2D(32, 3, activation="relu")(G)
#     x = tf.keras.layers.MaxPooling2D()(x)
#     x = tf.keras.layers.Conv2D(64, 3, activation="relu")(x)
#     x = tf.keras.layers.GlobalAveragePooling2D()(x)
#     z = tf.keras.layers.Concatenate()([x, v])
#     z = tf.keras.layers.Dense(128, activation="relu")(z)
#     out = tf.keras.layers.Dense(2, activation="tanh")(z)  # rango [-1,1]
#     return tf.keras.Model([G, v], out, name="meta_policy")

# # ───────────────────────── nodo de inferencia ───────────────────────────
# class TerrainPPOInference(Node):
#     def __init__(self, weights_dir: pathlib.Path):
#         super().__init__("terrain_ppo_inference")

#         # ─── suscripciones ──────────────────────────────────────────────
#         self.create_subscription(Odometry,      "/ARGJ801/odom_demo", self.cb_odom, 10)
#         self.create_subscription(PoseArray,     "/goal",              self.cb_goal, 10)
#         self.create_subscription(OccupancyGrid, "/occupancy_grid",    self.cb_grid, 10)
#         self.create_subscription(GridMap,       "/terrain_grid",      self.cb_hmap, 10)

#         # ─── publishers ────────────────────────────────────────────────
#         self.path_pub   = self.create_publisher(Path,   "/global_path_predicted", 10)
#         self.arrow_pub  = self.create_publisher(Marker, "/meta_wp",               10)
#         self.goal_vec_pub = self.create_publisher(Marker, "/goal_vec",            10)
#         self.cmd_pub    = self.create_publisher(CmdThrottleMsg, "/ARGJ801/cmd_throttle_msg", 10)

#         # ─── redes y pesos ─────────────────────────────────────────────
#         self.low  = build_low_policy()
#         self.meta = build_meta_policy()
#         self._load_latest(weights_dir)

#         # ─── estado mínimo para follow_path() ──────────────────────────
#         self.pose=None; self.twist=None; self.goal=None
#         self.grid=None; self.grid_msg=None; self.height_map_msg=None
#         self.grid_dyn=None
#         self.hmap=None                                 # alias cómodo
#         self.need_new_wp = True
#         self.current_path=[]; self.wp_index=0
#         self.last_cmd = CmdThrottleMsg()               # para el filtro
#         self.last_cmd.throttle = 0.0
#         self.last_cmd.steering = 0.0
#         self.meta_tick = 0                             # 1.5 s

#         # ─── enganchar follow_path() y helpers del trainer ─────────────
#         self.follow_path           = TerrainPPOTrainer.follow_path.__get__(self)
#         self._yaw_from_quaternion  = TerrainPPOTrainer._yaw_from_quaternion.__get__(self)
#         self._global_to_local      = TerrainPPOTrainer._global_to_local.__get__(self)
#         # (slope_ok y demás ya son funciones libres)

#         # ─── timer 10 Hz ───────────────────────────────────────────────
#         self.create_timer(0.1, self.step)
#         self.get_logger().info("Nodo de inferencia listo ✓")

#     # ──────────────────── carga de checkpoints ─────────────────────────
#     def _load_latest(self, wdir: pathlib.Path):
#         def _latest(pattern):
#             files = sorted(wdir.glob(pattern))
#             if not files:
#                 return None
#             # intenta _ep<n>; si no, coge el último por timestamp
#             with_ep = [f for f in files if re.search(r'_ep(\d+)', f.stem)]
#             return max(with_ep, key=lambda p:int(re.search(r'_ep(\d+)',p.stem).group(1))) \
#                    if with_ep else max(files, key=lambda p:p.stat().st_mtime)

#         low_ckpt  = _latest("policy_low*.weights.h5")
#         meta_ckpt = _latest("policy_meta*.weights.h5")
#         if not low_ckpt or not meta_ckpt:
#             raise RuntimeError(f"No se encontraron pesos en {wdir}")

#         # Keras 3 → sólo `skip_mismatch`
#         self.low .load_weights(low_ckpt,  skip_mismatch=True)
#         self.meta.load_weights(meta_ckpt, skip_mismatch=True)
#         self.get_logger().info(f"Cargados pesos:\n   LOW : {low_ckpt.name}\n   META: {meta_ckpt.name}")

#     # ──────────────────── callbacks ROS2 ───────────────────────────────
#     def cb_odom(self,msg):
#         self.pose  = msg.pose.pose
#         self.twist = msg.twist.twist
#     def cb_goal(self,msg):
#         self.goal = (msg.poses[0].position.x, msg.poses[0].position.y) if msg.poses else None
#     def cb_grid(self,msg):
#         self.grid_msg = msg
#         self.grid_dyn = np.asarray(msg.data,dtype=np.int8)
#         self.grid     = self.grid_dyn.reshape((msg.info.height,msg.info.width))
#     def cb_hmap(self,msg):
#         self.height_map_msg = msg
#         self.hmap = msg

#     # ──────────────────── patches (idénticos al trainer) ───────────────
#     def extract_patch(self):
#         if self.pose is None or self.goal is None or self.grid_msg is None or self.grid is None:return
#         info = self.grid_msg.info
#         cp   = (self.pose.position.x, self.pose.position.y)
#         ci   = int((cp[0]-info.origin.position.x)/info.resolution)
#         cj   = int((cp[1]-info.origin.position.y)/info.resolution)
#         i0,i1 = ci-PATCH//2, ci+PATCH//2
#         j0,j1 = cj-PATCH//2, cj+PATCH//2
#         occ = self.grid[max(0,j0):min(j1,info.height),
#                         max(0,i0):min(i1,info.width)]
#         pad = ((max(0,-j0),max(0,j1-info.height)),
#                (max(0,-i0),max(0,i1-info.width)))
#         occ = np.pad(occ,pad,'constant',constant_values=-1)
#         norm_occ = ((occ+1)/101.0).astype(np.float32)

#         # altura normalizada
#         h_norm = np.zeros_like(norm_occ,dtype=np.float32)
#         if self.hmap:
#             h_arr,hi = gridmap_to_numpy(self.hmap)
#             if h_arr is not None:
#                 res = hi.resolution
#                 ox  = hi.pose.position.x - hi.length_x/2
#                 oy  = hi.pose.position.y - hi.length_y/2
#                 for jj in range(PATCH):
#                     wy = cp[1] + (jj-PATCH//2)*info.resolution
#                     jh = int((wy-oy)/res)
#                     if 0<=jh<h_arr.shape[0]:
#                         for ii in range(PATCH):
#                             wx = cp[0] + (ii-PATCH//2)*info.resolution
#                             ih = int((wx-ox)/res)
#                             if 0<=ih<h_arr.shape[1]:
#                                 h_norm[jj,ii] = h_arr[jh,ih]
#                 mu = np.nanmean(h_norm)
#                 h_norm = (h_norm-mu)/5.0      # ≈ -1..+1

#         gx,gy = np.gradient(h_norm)
#         return np.stack([norm_occ,gx,gy],axis=-1).astype(np.float32)

#     def extract_big_patch(self):
#         local = self.extract_patch()
#         if local is None: return None
#         big = tf.image.resize(local,(BIG_PATCH,BIG_PATCH),method='bilinear').numpy()
#         goal_layer = np.zeros((BIG_PATCH,BIG_PATCH),np.float32)
#         if self.goal:
#             scale = BIG_PATCH/PATCH
#             dx = (self.goal[0]-self.pose.position.x)/self.grid_msg.info.resolution
#             dy = (self.goal[1]-self.pose.position.y)/self.grid_msg.info.resolution
#             gi = int(BIG_PATCH/2 + dx*scale)
#             gj = int(BIG_PATCH/2 + dy*scale)
#             goal_layer[max(0,gj-2):gj+3, max(0,gi-2):gi+3] = 5.0
#         ent = np.zeros_like(goal_layer)
#         frs = np.zeros_like(goal_layer)
#         return np.dstack([big,goal_layer,ent,frs]).astype(np.float32)

#     # ──────────────────── path local RRT* coste ─────────────────────────
#     def short_path(self,start,wp):
#         arr,info = self.grid, self.grid_msg.info
#         h,hinfo  = gridmap_to_numpy(self.hmap)
#         path = rrt_plan_cost(start,wp,arr,info,h,hinfo,max_iter=300,step=0.5,goal_tol=0.3)
#         if len(path)<2: path=[start,wp]
#         return densify(smooth_bspline(_filter_curv(path)))

#     # ──────────────────── helpers de visualización ─────────────────────
#     def arrow(self,p_from,p_to,color,ns,id=0):
#         hdr = Header(frame_id="map",stamp=self.get_clock().now().to_msg())
#         m   = Marker(header=hdr,ns=ns,id=id,type=Marker.ARROW,action=Marker.ADD)
#         m.scale = Vector3(x=0.05,y=0.1,z=0.1)
#         m.color = ColorRGBA(r=float(color[0]),g=float(color[1]),b=float(color[2]),a=1.0)
#         m.points=[Point(x=p_from[0],y=p_from[1],z=0.2),
#                   Point(x=p_to  [0],y=p_to  [1],z=0.2)]
#         (self.goal_vec_pub if ns=="goal_vec" else self.arrow_pub).publish(m)

#     def publish_path(self,pts):
#         hdr=Header(frame_id="map",stamp=self.get_clock().now().to_msg())
#         msg=Path(header=hdr)
#         for x,y in pts:
#             ps=PoseStamped(header=hdr); ps.pose.position.x=x; ps.pose.position.y=y; ps.pose.orientation.w=1.0
#             msg.poses.append(ps)
#         self.path_pub.publish(msg)
 
    
#     def choose_meta_target(self, cp):
#         """Devuelve (point, tag) donde tag ∈ {'GOAL', 'FRONTIER'}."""
#         # 1) ¿goal visible?
#         if bres_line_free(self.grid_dyn, self.grid_msg.info, cp, self.goal):
#             return self.goal, "GOAL"

#         # 2) mejor frontier
#         front, _ = self.select_frontier(cp,
#                                         self.grid_dyn,
#                                         self.grid_msg.info)
#         if front is not None:
#             return front, "FRONTIER"

#         # 3) fallback: goal
#         return self.goal, "GOAL"
#     # ──────────────────── bucle de control (10 Hz) ──────────────────────
#     def step(self):
#         # espera a tener todos los datos
#         if self.pose is None or self.goal is None or self.grid_msg is None or self.grid is None:return
#         cp=(self.pose.position.x,self.pose.position.y)

#         # ------------- 2. META-PHASE ----------------------------------------
#         if self.need_new_wp or self.meta_tick <= 0:
#             # 2.a  Objetivo global (goal visible → goal, si no best frontier)
#             target, tag = self.choose_meta_target(cp)        # usa tu propia función
#             vec_t = np.array([target[0]-cp[0], target[1]-cp[1]], np.float32)

#             # 2.b  ¡SEGURIDAD!  (evita NaN si ya estamos encima del target)
#             wp_rel = safe_wp_rel(vec_t, self.curr_r_meta, MIN_WP_DIST)
#             waypoint = (cp[0] + wp_rel[0], cp[1] + wp_rel[1])

#             # 2.c  Filtro de ocupación; si falla busca escape
#             if (not np.isfinite(waypoint[0]) or not np.isfinite(waypoint[1]) or
#                 not clearance_ok(self.grid_dyn, self.grid_msg.info,
#                                 waypoint, CLEAR_MIN)):
#                 esc = self.find_escape_point(cp, yaw)
#                 if esc is None:                                # fallback: no te muevas
#                     self.get_logger().warning("Waypoint inválido ➜ se descarta")
#                     self.meta_tick = 5        # re-intenta en 0.5 s
#                     return
#                 waypoint = esc

#             # 2.d  Genera path corto y publica
#             self.current_waypoint = waypoint
#             self.current_path     = self.generate_short_path(cp, waypoint)
#             self.wp_index         = 1
#             self.publish_path(self.current_path)
#             self.publish_arrow(cp, waypoint, ns="meta_wp", color=(0,0,1))
#             self.need_new_wp = False
#             self.meta_tick   = META_PERIOD
#         else:
#             self.meta_tick -= 1


#         # ─── llamada al mismo Pure-Pursuit del trainer ─────────────────
#         if self.current_path:
#             # alias para compatibilidad con follow_path()
#             self.height_map_msg = self.hmap
#             self.grid_dyn       = self.grid
#             self.follow_path(cp)           # ← del trainer

# # ─────────────────────────────  main  ───────────────────────────────────
# def main():
#     parser = argparse.ArgumentParser()
#     parser.add_argument("--weights_dir", required=True,
#                         help="Directorio con *.weights.h5")
#     args = parser.parse_args()
#     rclpy.init()
#     node = TerrainPPOInference(pathlib.Path(args.weights_dir).expanduser())
#     rclpy.spin(node)

# if __name__ == "__main__":
#     main()
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
terrain_ppo_inference.py
────────────────────────
Nodo ROS 2 **sólo-inferencia** para la arquitectura jerárquica
(meta-policy + low-policy) entrenada en terrain_ppo_trainer_hierarchical.py
"""

# ------------------------------------------------------------------------
# IMPORTS
# ------------------------------------------------------------------------
import argparse, pathlib, re, math, time
import numpy as np
import rclpy
from rclpy.node          import Node
from std_msgs.msg        import Header, ColorRGBA
from geometry_msgs.msg   import (Point, Vector3, PoseStamped, PoseArray)
from nav_msgs.msg        import Path, Odometry, OccupancyGrid
from visualization_msgs.msg import Marker
from grid_map_msgs.msg   import GridMap
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg
import tensorflow as tf


KP_SPEED          = 10.0
THR_IDLE          = 2.0
# ----------  utilidades que reutilizamos del trainer --------------------
from car.j8_test_ppo import (
    TerrainPPOTrainer,                    # para follow_path()
    PATCH, BIG_PATCH, R_META_INIT,        # constantes
    MIN_WP_DIST, CLEAR_MIN, WAYPOINT_RADIUS,
    LAMBDA_SLOPE, SLOPE_MAX,
    densify, smooth_bspline, _filter_curv,
    gridmap_to_numpy, slope_ok, rrt_plan_cost, l2
)
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


def idx_from_world(info, pt):
    """Convierte punto (x,y) real a índices (i,j) en OccupancyGrid."""
    res = info.resolution
    return int((pt[0] - info.origin.position.x) / res), int((pt[1] - info.origin.position.y) / res)

def idx_from_world(info, pt):
    """Convierte punto (x,y) real a índices (i,j) en OccupancyGrid."""
    res = info.resolution
    return int((pt[0] - info.origin.position.x) / res), int((pt[1] - info.origin.position.y) / res)


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

def quadrant_scores(grid, info, cp, yaw, r_probe=1.0):
    """
    Devuelve dict {'F','L','R','B'} -> score∈[0,1] midiendo cuántas
    celdas están libres en un anillo de radio `r_probe` alrededor del
    robot (frame local).
    """
    hits = {'F':0,'L':0,'R':0,'B':0};  n = 16          # 22.5° por rayo
    for k in range(n):
        ang_loc  = -math.pi + 2*math.pi*k/n
        ang_glob = yaw + ang_loc
        test = (cp[0] + r_probe*math.cos(ang_glob),
                cp[1] + r_probe*math.sin(ang_glob))
        if clearance_ok(grid, info, test, info.resolution*0.8):
            if -math.pi/4 <= ang_loc <  math.pi/4:         hits['F'] += 1
            elif  math.pi/4 <= ang_loc < 3*math.pi/4:      hits['L'] += 1
            elif -3*math.pi/4 <= ang_loc < -math.pi/4:     hits['R'] += 1
            else:                                          hits['B'] += 1
    return {q: hits[q] / (n/4) for q in hits}
# ------------------------------------------------------------------------
# 1.  MODELOS  (idénticos a los del trainer)
# ------------------------------------------------------------------------
def build_low_policy():
    g   = tf.keras.Input(shape=(PATCH, PATCH, 3), name="grid")
    st  = tf.keras.Input(shape=(4,),              name="state")
    x = tf.keras.layers.Conv2D(16, 3, padding="same", activation="relu")(g)
    x = tf.keras.layers.MaxPooling2D()(x)
    x = tf.keras.layers.Conv2D(32, 3, padding="same", activation="relu")(x)
    x = tf.keras.layers.GlobalAveragePooling2D()(x)
    z = tf.keras.layers.Concatenate()([x, st])
    h0 = tf.keras.layers.Dense(128, activation="tanh")(z)
    c0 = tf.keras.layers.Dense(128, activation="tanh")(z)
    lstm = tf.keras.layers.LSTMCell(128)
    w0   = tf.keras.Input(shape=(2,), name="w0")
    h1,_ = lstm(w0, [h0, c0])
    delta= tf.keras.layers.Dense(2, activation="tanh")(h1)
    return tf.keras.Model([g, st, w0], delta, name="policy_low")

def build_meta_policy():
    G = tf.keras.Input(shape=(BIG_PATCH, BIG_PATCH, 6), name="big_patch")
    v = tf.keras.Input(shape=(2,),                      name="vec_goal")
    x = tf.keras.layers.Conv2D(32, 3, activation="relu")(G)
    x = tf.keras.layers.MaxPooling2D()(x)
    x = tf.keras.layers.Conv2D(64, 3, activation="relu")(x)
    x = tf.keras.layers.GlobalAveragePooling2D()(x)
    z = tf.keras.layers.Concatenate()([x, v])
    z = tf.keras.layers.Dense(128, activation="relu")(z)
    out = tf.keras.layers.Dense(2, activation="tanh")(z)
    return tf.keras.Model([G, v], out, name="policy_meta")

# ------------------------------------------------------------------------
# 2.  NODO  INFERENCIA
# ------------------------------------------------------------------------
META_PERIOD = 15                       # ticks de 10 Hz → 1.5 s
R_META_MAX  = 8.0                      # radio al que escalar la acción tanh
LAMBDA_IG = 1.0    # peso de entropía
LAMBDA_D  = 0.2    # distancia robot–frontier
LAMBDA_G  = 1.5    # distancia frontier–goal
class TerrainPPOInference(Node):
    def __init__(self, weights_dir: pathlib.Path):
        super().__init__("terrain_ppo_inference")

        # ----------- subscripciones ------------------------------------
        qos = 10
        self.create_subscription(Odometry,      "/ARGJ801/odom_demo", self.cb_odom,  qos)
        self.create_subscription(PoseArray,     "/goal",              self.cb_goal,  qos)
        self.create_subscription(OccupancyGrid, "/occupancy_grid",    self.cb_grid,  qos)
        self.create_subscription(GridMap,       "/terrain_grid",      self.cb_hmap,  qos)
        self.create_subscription(PoseArray, "/safe_frontier_points", self.cb_frontiers, 10)
        # ----------- publishers ----------------------------------------
        self.path_pub  = self.create_publisher(Path,   "/global_path_predicted", qos)
        self.arrow_pub = self.create_publisher(Marker, "/meta_wp",              qos)
        self.goal_pub  = self.create_publisher(Marker, "/goal_vec",             qos)
        self.cmd_pub   = self.create_publisher(CmdThrottleMsg,
                                               "/ARGJ801/cmd_throttle_msg",      qos)

        # ----------- modelos y pesos -----------------------------------
        self.low  = build_low_policy()
        self.meta = build_meta_policy()
        self._load_latest(weights_dir)

        # ----------- estado mínimo -------------------------------------
        self.pose=self.twist=None
        self.goal=None
        self.grid=self.grid_dyn=self.grid_msg=None
        self.hmap=None
        self.need_new_wp = True
        self.meta_tick   = 0
        self.curr_r_meta = R_META_INIT
        self.frontiers = []          # <── antes de need_new_wp = True
        self.current_waypoint = None
        self.current_path=[]; self.wp_index=0
        self.last_cmd = CmdThrottleMsg()

        # Enganchar el Pure-Pursuit del trainer
        self.follow_path          = TerrainPPOTrainer.follow_path.__get__(self)
        self._yaw_from_quaternion = TerrainPPOTrainer._yaw_from_quaternion.__get__(self)
        self._global_to_local     = TerrainPPOTrainer._global_to_local.__get__(self)

        self.create_timer(0.1, self.step)
        self.get_logger().info("Nodo de inferencia listo ✓")

    # ----------------- carga de pesos ----------------------------------
    def _load_latest(self, wdir: pathlib.Path):
        def latest(pattern:str):
            files=sorted(wdir.glob(pattern))
            if not files:
                return None
            # prioriza *_ep<n>.weights.h5
            with_ep=[f for f in files if re.search(r'_ep\d+', f.stem)]
            if with_ep:
                return max(with_ep,
                           key=lambda p:int(re.search(r'_ep(\d+)',p.stem).group(1)))
            return max(files, key=lambda p:p.stat().st_mtime)
        low_ckpt  = latest("policy_low*.weights.h5")
        meta_ckpt = latest("policy_meta_ep.weights_fixed.h5")
        if not low_ckpt or not meta_ckpt:
            raise RuntimeError(f"No se encontraron checkpoints en {wdir}")
        self.low .load_weights(low_ckpt,  skip_mismatch=True)
        self.meta.load_weights(meta_ckpt, skip_mismatch=True)
        bad_layers = []
        for w in self.meta.weights:
            if not np.all(np.isfinite(w.numpy())):
                bad_layers.append(w.name)

        if bad_layers:
            self.get_logger().error(
                f"⛔ Pesos con NaN/Inf en {len(bad_layers)} tensors "
                f"(ej: {bad_layers[:4]})")
        else:
            self.get_logger().info("Todos los pesos son finitos ✓")
        self.get_logger().info(f"Cargados pesos:\n   LOW : {low_ckpt.name}\n   META: {meta_ckpt.name}")

    # ----------------- callbacks ROS -----------------------------------
    def cb_frontiers(self, msg):
        self.frontiers = [(p.position.x, p.position.y) for p in msg.poses]

    def cb_odom(self,msg):  self.pose,msg.twist
    def cb_odom(self,msg):
        self.pose  = msg.pose.pose
        self.twist = msg.twist.twist
    def cb_goal(self,msg):
        self.goal = (msg.poses[0].position.x, msg.poses[0].position.y) if msg.poses else None
    def cb_grid(self,msg):
        self.grid_msg = msg
        self.grid_dyn = np.asarray(msg.data, dtype=np.int8).reshape(msg.info.height, msg.info.width)
        self.grid     = self.grid_dyn
    def cb_hmap(self,msg):  self.hmap = msg

    # ----------------- construcción de parches -------------------------
    def select_frontier(self, cp):
        """Devuelve (frontier, score) o (None, None) si no hay."""
        if not self.frontiers:
            return None, None

        best, best_s = None, -np.inf
        for f in self.frontiers:
            ig      = 0.0                       # si no tienes entropía, pon 0
            d_robot = math.hypot(f[0]-cp[0], f[1]-cp[1])
            d_goal  = math.hypot(f[0]-self.goal[0], f[1]-self.goal[1])
            s = +LAMBDA_IG*ig - LAMBDA_D*d_robot - LAMBDA_G*d_goal
            if s > best_s:
                best, best_s = f, s
        return best, best_s

    def choose_meta_target(self, cp):
        # 1) ¿goal visible?
        if self.goal and bres_line_free(self.grid_dyn,
                                        self.grid_msg.info,
                                        cp, self.goal):
            return self.goal, "GOAL"

        # 2) mejor frontier
        front, _ = self.select_frontier(cp)
        if front is not None:
            return front, "FRONTIER"

        # 3) fallback: goal (aunque no sea visible)
        return self.goal, "GOAL_FALLBACK"

    def extract_patch(self):
        if any(v is None for v in (self.pose, self.grid_msg, self.grid)):
            return None
        info = self.grid_msg.info
        cp   = (self.pose.position.x, self.pose.position.y)
        ci   = int((cp[0]-info.origin.position.x)/info.resolution)
        cj   = int((cp[1]-info.origin.position.y)/info.resolution)
        i0,i1 = ci-PATCH//2, ci+PATCH//2
        j0,j1 = cj-PATCH//2, cj+PATCH//2
        occ = self.grid[max(0,j0):min(j1,info.height),
                        max(0,i0):min(i1,info.width)]
        pad=((max(0,-j0),max(0,j1-info.height)),
             (max(0,-i0),max(0,i1-info.width)))
        occ=np.pad(occ,pad,'constant',constant_values=-1)
        norm_occ=((occ+1)/101.0).astype(np.float32)

        h_norm=np.zeros_like(norm_occ,dtype=np.float32)
        if self.hmap:
            h_arr,hinfo=gridmap_to_numpy(self.hmap)
            if h_arr is not None:
                resh,ox,oy=hinfo.resolution, hinfo.pose.position.x-hinfo.length_x/2,\
                            hinfo.pose.position.y-hinfo.length_y/2
                for jj in range(PATCH):
                    wy=cp[1]+(jj-PATCH//2)*info.resolution
                    jh=int((wy-oy)/resh)
                    if 0<=jh<h_arr.shape[0]:
                        for ii in range(PATCH):
                            wx=cp[0]+(ii-PATCH//2)*info.resolution
                            ih=int((wx-ox)/resh)
                            if 0<=ih<h_arr.shape[1]:
                                h_norm[jj,ii]=h_arr[jh,ih]
                h_norm=(h_norm-np.nanmean(h_norm))/5.0
        gx,gy=np.gradient(h_norm)
        return np.stack([norm_occ,gx,gy],axis=-1).astype(np.float32)

    def extract_big_patch(self):
        local=self.extract_patch()
        if local is None: return None
        big=tf.image.resize(local,(BIG_PATCH,BIG_PATCH),method='bilinear').numpy()
        goal_ly=np.zeros((BIG_PATCH,BIG_PATCH),np.float32)
        if self.goal:
            dx=(self.goal[0]-self.pose.position.x)/self.grid_msg.info.resolution
            dy=(self.goal[1]-self.pose.position.y)/self.grid_msg.info.resolution
            gi=int(BIG_PATCH/2+dx*BIG_PATCH/PATCH)
            gj=int(BIG_PATCH/2+dy*BIG_PATCH/PATCH)
            goal_ly[max(0,gj-2):gj+3,max(0,gi-2):gi+3]=5.0
        ent=np.zeros_like(goal_ly)   # podrías rellenarlo si envías IG
        frs=np.zeros_like(goal_ly)   # idem
        return np.dstack([big,goal_ly,ent,frs]).astype(np.float32)

    # ----------------- path local (idéntico trainer) -------------------
    def short_path(self,start,wp):
        g,info=self.grid,self.grid_msg.info
        h_arr,hinfo=gridmap_to_numpy(self.hmap)
        path=rrt_plan_cost(start,wp,g,info,h_arr,hinfo,
                           max_iter=300,step=0.5,goal_tol=0.3)
        if len(path)<2: path=[start,wp]
        return densify(smooth_bspline(_filter_curv(path)))

    # ----------------- helpers visualización ---------------------------
    def arrow(self,p_from,p_to,color,ns):
        hdr=Header(frame_id="map",stamp=self.get_clock().now().to_msg())
        m=Marker(header=hdr,ns=ns,id=0,type=Marker.ARROW,action=Marker.ADD)
        m.scale=Vector3(x=0.05,y=0.1,z=0.1)
        m.color=ColorRGBA(r=color[0],g=color[1],b=color[2],a=1.0)
        m.points=[Point(x=p_from[0],y=p_from[1],z=0.2),
                  Point(x=p_to[0]  ,y=p_to[1]  ,z=0.2)]
        (self.goal_pub if ns=="goal_vec" else self.arrow_pub).publish(m)

    def publish_path(self,pts):
        hdr=Header(frame_id="map",stamp=self.get_clock().now().to_msg())
        msg=Path(header=hdr)
        for x,y in pts:
            ps=PoseStamped(header=hdr); ps.pose.position.x=x; ps.pose.position.y=y
            ps.pose.orientation.w=1.0
            msg.poses.append(ps)
        self.path_pub.publish(msg)

    def robot_inside_grid(self):
        if self.grid_msg is None or self.grid_dyn is None or self.pose is None:
            return False
        i, j = idx_from_world(self.grid_msg.info,
                            (self.pose.position.x, self.pose.position.y))
        if not (0 <= i < self.grid_msg.info.width and 0 <= j < self.grid_msg.info.height):
            return False
        return self.grid_dyn[j, i] != -1
    
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

    def find_escape_point(self, cp, yaw,
                            clear_r=CLEAR_MIN,
                            r_max=25.0, step_r=0.4):
            """
            Nueva búsqueda BFS guiada por los cuadrantes más libres.
            """
            if self.grid_dyn is None or self.grid_msg is None:
                return None

            grid, info = self.grid_dyn, self.grid_msg.info
            scores = quadrant_scores(grid, info, cp, yaw)
            prefer   = sorted(scores, key=scores.get, reverse=True)

            quad_ang = {'F':(-math.pi/4, math.pi/4),
                        'L':( math.pi/4, 3*math.pi/4),
                        'R':(-3*math.pi/4,-math.pi/4),
                        'B':( 3*math.pi/4, 5*math.pi/4)}  # continuidad

            for r in np.arange(clear_r*1.5, r_max+1e-3, step_r):
                for q in prefer:
                    lo,hi = quad_ang[q]
                    if hi < lo: hi += 2*math.pi
                    n = max(1,int((hi-lo)/math.radians(10)))
                    for k in range(n+1):
                        ang_l = lo + k*(hi-lo)/n
                        ang_g = yaw + ang_l
                        cand  = (cp[0]+r*math.cos(ang_g),
                                cp[1]+r*math.sin(ang_g))
                        if not clearance_ok(grid, info, cand, clear_r):
                            continue
                        if not bres_line_free(grid, info, cp, cand):
                            continue
                        self.get_logger().info(
                            f"[ESCAPE] cuadrante {q} libre={scores[q]:.2f}")
                        return cand
            return None
    # ----------------- bucle principal (10 Hz) -------------------------
    def step(self):
        # ─── 0) esperamos a tener todos los datos mínimos ──────────────
        if any(v is None for v in (self.pose, self.goal, self.grid_msg, self.grid)):return

        cp = (self.pose.position.x, self.pose.position.y)
        yaw  = self._yaw_from_quaternion(self.pose.orientation)
        # ─── 1) META-PHASE ─────────────────────────────────────────────
        if self.need_new_wp or self.meta_tick <= 0:

            # 1.a  Elegir “target” (goal visible  ⇒ goal ; si no, frontier top)
            target, tag = self.choose_meta_target(cp)   # usa frontiers si existen
            vec_t = np.array([target[0]-cp[0], target[1]-cp[1]], np.float32)

            # 1.b  Parches + red meta  →  Δwp (normalizado)
            big_patch = self.extract_big_patch()




            if big_patch is None:
                return
            vec_goal = vec_t / R_META_MAX               # normaliza al radio


            if not np.all(np.isfinite(big_patch)):
                self.get_logger().error("big_patch contiene NaNs")
                bad = np.argwhere(~np.isfinite(big_patch))
                self.get_logger().error(f"   primeras 5 posiciones: {bad[:5]}")
                return                      # saltamos este tick

            #  ─── llamada a la red meta ───────────────────────────────────────
            mu = self.meta([big_patch[None, ...],
                            vec_goal[None, ...]])[0].numpy()

            if not np.all(np.isfinite(mu)):
                self.get_logger().error(f"meta-policy devolvió NaN: mu={mu}")
                return 


            mu = self.meta([big_patch[None, ...],
                            vec_goal[None, ...]])[0].numpy()
            wp_rel = np.clip(mu, -1, 1) * R_META_MAX    # dentro del radio

            # 1.c  Mezcla con el target escogido
            #      (50 % hacia la predicción, 50 % hacia el target bruto)
            blend = 0.5
            direct = vec_t / (np.linalg.norm(vec_t)+1e-6) * self.curr_r_meta
            wp_rel = (1-blend)*direct + blend*wp_rel

            # 1.d  Seguridad: mínimo avance y NaNs
            n = np.linalg.norm(wp_rel)
            if n < MIN_WP_DIST:
                wp_rel = wp_rel / (n+1e-6) * MIN_WP_DIST

            waypoint = (cp[0] + wp_rel[0], cp[1] + wp_rel[1])

            # 1.e  Waypoint en obstáculo  →  fallback
            if not clearance_ok(self.grid_dyn, self.grid_msg.info,
                                waypoint, CLEAR_MIN):
                waypoint = self.find_escape_point(cp,yaw)


            # 1.f  Path corto, publicación y reset de contadores
            self.current_waypoint = waypoint
            self.current_path = self.short_path(cp, waypoint)
            self.wp_index = 1
            self.publish_path(self.current_path)
            self.arrow(cp, waypoint, (0, 0, 1), "meta_wp")
            col = (0, 1, 0) if tag.startswith("GOAL") else (1, 1, 0)
            self.arrow(cp, target, col, "target_dbg")
            self.need_new_wp = False
            self.meta_tick = META_PERIOD

        else:
            self.meta_tick -= 1

        # ─── 2) LOW-LEVEL (Pure-Pursuit) ───────────────────────────────
        if self.current_path:
            self.height_map_msg = self.hmap  # para slope_ok()
            self.grid_dyn       = self.grid  # para follow_path()
            self.follow_path(cp)


# ------------------------------------------------------------------------
# 3.  MAIN
# ------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--weights_dir", required=True,
                        help="Directorio con *.weights.h5 del trainer")
    args = parser.parse_args()

    rclpy.init()

    # ①  guarda el nodo en una variable
    node = TerrainPPOInference(pathlib.Path(args.weights_dir).expanduser())

    try:
        # ②  pásalo a spin
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()