#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
simple_escape_planner_pp.py
───────────────────────────
Nodo ROS 2 dedicado a ESCAPAR de un atasco:

• Detecta colisión en /virtual_collision → busca punto libre (BFS).
• Publica:
     – /escape_goal   (Marker esfera verde)
     – /escape_path   (Path verde)
• Ejecuta un Pure-Pursuit muy básico (10 Hz) para llegar al punto.
  – throttle fijo 6 % (⇢ ≈ 0.6 m/s).
  – steering proporcional a la curvatura.
"""

import math, collections, numpy as np, rclpy
from rclpy.node import Node
from std_msgs.msg      import Bool, Header
from nav_msgs.msg      import Odometry, OccupancyGrid, Path
from geometry_msgs.msg import PoseStamped, Point, Vector3
from visualization_msgs.msg import Marker
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg
import math   
from visualization_msgs.msg import Marker, MarkerArray
import colorsys

# ─────────── parámetros rápidos ────────────────────────────────────────
CLEAR_MIN  = 0.6        # holgura (m)
SEARCH_MAX = 25.0       # BFS máx. (m)

#  Control PP “light”
WHEEL_BASE   = 1.35
DELTA_MAX    = math.radians(32)
MAX_STEERING = 70.0
THR_CONST    = 6.0       # ≈ 0.6 m/s en tu plataforma
LOOK_AHEAD   = 1.0       # fijo; basta para un segmento

# ─────────── utilidades de mapa ────────────────────────────────────────
def idx_from_world(info, pt):
    res = info.resolution
    return int((pt[0]-info.origin.position.x)/res), \
           int((pt[1]-info.origin.position.y)/res)

def world_from_idx(info, ij):
    res = info.resolution
    return (info.origin.position.x + (ij[0]+.5)*res,
            info.origin.position.y + (ij[1]+.5)*res)

def bres_line_free(grid, info, a, b):
    i0,j0 = idx_from_world(info,a);  i1,j1 = idx_from_world(info,b)
    di,dj = abs(i1-i0),abs(j1-j0);  si = 1 if i0<i1 else -1;  sj = 1 if j0<j1 else -1
    err = di-dj
    while True:
        if not(0<=i0<info.width and 0<=j0<info.height): return False
        v = grid[j0,i0]
        if v==-1 or v>=50: return False
        if (i0,j0)==(i1,j1): return True
        e2 = 2*err
        if e2>-dj: err-=dj; i0+=si
        if e2< di: err+=di; j0+=sj

def clearance_ok(grid, info, pt, r):
    i0,j0 = idx_from_world(info,pt);  rad = int(r/info.resolution)
    H,W = grid.shape
    for dj in range(-rad,rad+1):
        for di in range(-rad,rad+1):
            ii,jj = i0+di, j0+dj
            if 0<=ii<W and 0<=jj<H:
                v=grid[jj,ii]
                if v==-1 or v>=50: return False
    return True


def l2(a: tuple[float, float], b: tuple[float, float]) -> float:
    """Distancia Euclídea 2-D  √((x₂-x₁)²+(y₂-y₁)²)."""
    return math.hypot(b[0] - a[0], b[1] - a[1])


def quadrant_scores(grid, info, cp, yaw, r_probe=1.0):
    """
    Devuelve dict con score de ocupación para los 4 cuadrantes
    del frame LOCAL del robot.  1.0 = libre, 0.0 = totalmente bloqueado.
    """
    scores = {'F':0.0,'R':0.0,'L':0.0,'B':0.0}
    n_samples = 16                 # 22.5° cada muestra
    free_cnt  = dict.fromkeys(scores,0)
    for k in range(n_samples):
        ang_local = -math.pi + 2*math.pi*k/n_samples   # [-π,π]
        # mapea a global
        ang_global = yaw + ang_local
        test = (cp[0] + r_probe*math.cos(ang_global),
                cp[1] + r_probe*math.sin(ang_global))
        # ­— celda libre? —
        if clearance_ok(grid, info, test, info.resolution*0.8):
            # qué cuadrante?
            if   -math.pi/4 <= ang_local <  math.pi/4:  free_cnt['F']+=1
            elif  math.pi/4 <= ang_local < 3*math.pi/4: free_cnt['L']+=1
            elif -3*math.pi/4 <= ang_local < -math.pi/4:free_cnt['R']+=1
            else:                                       free_cnt['B']+=1
    # normaliza
    for q in scores:
        scores[q] = free_cnt[q] / (n_samples/4)
    return scores      # ej.: {'F':0.25,'L':1.0,'R':0.5,'B':0.0}
# ─────────── nodo ───────────────────────────────────────────────────────
class EscapePP(Node):
    def __init__(self):
        super().__init__("escape_planner_pp")

        qos = 10
        self.create_subscription(Odometry,      "/ARGJ801/odom_demo", self.cb_odom, qos)
        self.create_subscription(OccupancyGrid, "/occupancy_grid",    self.cb_grid, qos)
        self.create_subscription(Bool,          "/virtual_collision", self.cb_col,  qos)

        self.path_pub = self.create_publisher(Path,   "/escape_path",  qos)
        self.goal_pub = self.create_publisher(Marker, "/escape_goal",  qos)
        self.cmd_pub  = self.create_publisher(CmdThrottleMsg, "/ARGJ801/cmd_throttle_msg", qos)

        # estado
        self.pose=None; self.yaw=0.0
        self.grid=None; self.info=None
        self.curr_path=[]; self.wp_idx=0

        self.create_timer(0.1, self.step)    # 10 Hz
        self.get_logger().info("Escape-planner + PP listo ✓")

    # ---------- callbacks básicos -------------------------------------
    def cb_odom(self, msg):
        self.pose = msg.pose.pose
        q=msg.pose.pose.orientation
        self.yaw = math.atan2(2*(q.w*q.z+q.x*q.y), 1-2*(q.y*q.y+q.z*q.z))

    def cb_grid(self, msg):
        self.info = msg.info
        self.grid = np.asarray(msg.data,dtype=np.int8)\
                       .reshape((msg.info.height,msg.info.width))

    def cb_col(self, msg):
        if not msg.data or self.pose is None or self.grid is None: return
        cp=(self.pose.position.x, self.pose.position.y)
        wp=self.find_escape_point(cp,self.yaw)
        scores = quadrant_scores(self.grid, self.info, cp, self.yaw)
        self.publish_quadrant_debug(cp, self.yaw, scores=scores)

        if wp is None:
            self.get_logger().warning("[ESCAPE-BFS] sin salida (25 m)")
            return
        self.curr_path=[cp, wp]; self.wp_idx=1
        self.publish_path(self.curr_path)
        self.publish_goal(wp)
        self.get_logger().info(f"[ESCAPE] elegido → {wp}")


    # ---------- búsqueda BFS ------------------------------------------
    def find_escape_point(self,
                      cp: tuple[float,float],
                      yaw: float,
                      clear_r: float=CLEAR_MIN,
                      r_max: float=SEARCH_MAX,
                      step_r: float=0.4) -> tuple[float,float]|None:
        """
        Busca un destino usando heurística de cuadrantes libres.
        1. Calcula qué lados (F,L,R,B) están menos bloqueados.
        2. Prioriza primero esos lados y recorre radios crecientes.
        """
        if self.grid is None or self.info is None:
            return None

        # 1) mide “libertad” de cada lado
        scores = quadrant_scores(self.grid, self.info, cp, yaw)
        # ordena de mejor (más libre) a peor
        ordered_quads = sorted(scores, key=scores.get, reverse=True)

        # lista ángulos preferidos por cuadrante en frame local
        quad_angles = {
            'F': ( -math.pi/4,  math.pi/4  ),
            'L': (  math.pi/4,  3*math.pi/4),
            'R': ( -3*math.pi/4,-math.pi/4 ),
            'B': (  3*math.pi/4, 5*math.pi/4)   # π→−π continuidad
        }

        # 2) BFS radial empezando por el lado más libre
        for r in np.arange(clear_r*1.5, r_max+1e-3, step_r):
            for q in ordered_quads:
                lo, hi = quad_angles[q]
                # corrige continuidad de 'B'
                if hi < lo: hi += 2*math.pi
                n_ang = max(1, int((hi-lo)/math.radians(10)))
                for k in range(n_ang+1):
                    ang_local = lo + k*(hi-lo)/n_ang
                    ang_global= yaw + ang_local
                    cand=(cp[0]+r*math.cos(ang_global),
                        cp[1]+r*math.sin(ang_global))

                    if not clearance_ok(self.grid, self.info, cand, clear_r):
                        continue
                    if not bres_line_free(self.grid, self.info, cp, cand):
                        continue
                    self.get_logger().info(f"[ESCAPE] cuadrante {q} score={scores[q]:.2f}")
                    return cand

        self.get_logger().warning("[ESCAPE] sin salida tras buscar %.0f m", r_max)
        return None



    # ---------- Pure-Pursuit mínimo -----------------------------------
    def follow_path(self):
        if not self.curr_path:
            return
        cp = (self.pose.position.x, self.pose.position.y)
        tgt = self.curr_path[self.wp_idx]

        # — coordenadas locales —
        dx, dy = tgt[0] - cp[0], tgt[1] - cp[1]
        lx =  math.cos(-self.yaw) * dx - math.sin(-self.yaw) * dy
        ly =  math.sin(-self.yaw) * dx + math.cos(-self.yaw) * dy
        dist = math.hypot(lx, ly)
        alpha = math.atan2(ly, lx)               # [-π, π]

        # — marcha atrás si el punto queda “detrás” (> 100°) —
        reverse = False
        if abs(alpha) > math.radians(100):
            reverse = True
            alpha += -math.pi if alpha > 0 else math.pi
            dist = max(dist, 1e-3)

        # — curvatura P-P —
        kappa = 2 * math.sin(alpha) / max(dist, 1e-3)
        delta = math.atan(WHEEL_BASE * kappa)
        if reverse:
            delta *= -1.0            # invertir sentido en reversa

        steering = float(np.clip(-(delta / DELTA_MAX) * MAX_STEERING,
                                -MAX_STEERING, MAX_STEERING))
        throttle = -THR_CONST if reverse else THR_CONST

        cmd = CmdThrottleMsg()
        cmd.throttle = throttle
        cmd.steering = steering
        self.cmd_pub.publish(cmd)

        # — avanzar índice —
        if dist < LOOK_AHEAD and self.wp_idx + 1 < len(self.curr_path):
            self.wp_idx += 1

    # ---------- Timer --------------------------------------------------
    def step(self):
        if not self.curr_path or self.pose is None: return
        self.follow_path()

    def publish_quadrant_debug(self, cp, yaw,
                            probe_r=1.0,
                            scores=None):
        """
        • circulo blanco · id=0
        • cruz amarilla  · id=1
        • 4 sectores TRIANGLE_FAN ids 10-13 coloreados por score
        """
        hdr=Header(frame_id="map",stamp=self.get_clock().now().to_msg())

        arr=MarkerArray()

        # ---- círculo ------------------------------------------
        circ=Marker(header=hdr, ns="qdbg", id=0, type=Marker.LINE_STRIP,
                    action=Marker.ADD, scale=Vector3(x=0.03,y=0,z=0))
        circ.color.r=circ.color.g=circ.color.b=1.0;  circ.color.a=1.0
        for th in np.linspace(0,2*math.pi,73):
            circ.points.append(Point(x=cp[0]+probe_r*math.cos(th),
                                    y=cp[1]+probe_r*math.sin(th),
                                    z=0.05))
        arr.markers.append(circ)

        # ---- cruz ---------------------------------------------
        cross=Marker(header=hdr, ns="qdbg", id=1, type=Marker.LINE_LIST,
                    action=Marker.ADD, scale=Vector3(x=0.04,y=0,z=0))
        cross.color.r=cross.color.g=1.0; cross.color.b=0; cross.color.a=1.0
        for ang in (0, math.pi/2):
            p1=Point(x=cp[0]-probe_r*math.cos(yaw+ang),
                    y=cp[1]-probe_r*math.sin(yaw+ang), z=0.05)
            p2=Point(x=cp[0]+probe_r*math.cos(yaw+ang),
                    y=cp[1]+probe_r*math.sin(yaw+ang), z=0.05)
            cross.points.extend([p1,p2])
        arr.markers.append(cross)

        # ---- sectores coloreados ------------------------------
        quad_info = {'F':(-45, 45, 10),
                    'L':( 45,135,11),
                    'R':(-135,-45,12),
                    'B':(135,225,13)}
        if scores is None: scores={'F':0,'L':0,'R':0,'B':0}

        for q,(lo,hi,id_) in quad_info.items():
            fan=Marker(header=hdr, ns="qdbg", id=id_, type=Marker.TRIANGLE_LIST,
                    action=Marker.ADD, scale=Vector3(x=1,y=1,z=1))
            # HSV → RGB (0=rojo, 0.33=verde)
            hue   = 0.33*scores[q]          # rojo→verde con el score
            r,g,b = colorsys.hsv_to_rgb(hue,1.0,1.0)
            fan.color.r,fan.color.g,fan.color.b,fan.color.a = r,g,b,0.4
            # centro
            fan.points.append(Point(x=cp[0],y=cp[1],z=0.04))
            # borde
            for ang_deg in range(lo,hi+1,5):
                ang = yaw + math.radians(ang_deg)
                fan.points.append(Point(x=cp[0]+probe_r*math.cos(ang),
                                        y=cp[1]+probe_r*math.sin(ang),
                                        z=0.04))
            arr.markers.append(fan)

        self.goal_pub.publish(arr)          # usa mismo publisher (MarkerArray OK)

    # ---------- RViz helpers ------------------------------------------
    def publish_goal(self, wp):
        hdr=Header(frame_id="map", stamp=self.get_clock().now().to_msg())
        m=Marker(header=hdr, ns="escape_goal", id=0, type=Marker.SPHERE, action=Marker.ADD)
        m.pose.position.x, m.pose.position.y, m.pose.position.z = *wp,0.2
        m.scale=Vector3(x=0.4,y=0.4,z=0.4); m.color.r,m.color.g,m.color.b,m.color.a=0.0,1.0,0.0,1.0
        self.goal_pub.publish(m)

    def publish_path(self, pts):
        hdr=Header(frame_id="map", stamp=self.get_clock().now().to_msg())
        msg=Path(header=hdr)
        for x,y in pts:
            ps=PoseStamped(header=hdr); ps.pose.position.x=x; ps.pose.position.y=y; ps.pose.orientation.w=1.0
            msg.poses.append(ps)
        self.path_pub.publish(msg)

# -----------------------------------------------------------------------
def main():
    rclpy.init()
    node=EscapePP()
    rclpy.spin(node)

if __name__=="__main__":
    main()
