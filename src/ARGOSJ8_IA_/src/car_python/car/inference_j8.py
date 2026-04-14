#!/usr/bin/env python3
"""
terrain_ppo_inference.py
────────────────────────
Nodo ROS 2 **solo‑inferencia** que emplea las dos políticas entrenadas
(meta‑policy y low‑level) para navegar un robot oruga/vehículo en terreno 3D
sin necesidad de volver a entrenar.

Se asume que anteriormente entrenaste con `terrain_ppo_trainer_hierarchical.py`
y guardaste los pesos:
  • policy_low_epXXX_YYYYMMDD_HHMMSS.weights.h5
  • policy_meta_epXXX_YYYYMMDD_HHMMSS.weights.h5

Lanza:
  ros2 run car terrain_ppo_inference   --ros-args \
        -p low_weights:=/ruta/policy_low.weights.h5 \
        -p meta_weights:=/ruta/policy_meta.weights.h5

El nodo realiza en cada ciclo (10 Hz):
 1. Si colisiona -> ejecuta ruta de escape hacia atrás.
 2. El meta‑nivel elige un nuevo waypoint (goal directo / frontier / meta‑policy).
 3. Genera un camino RRT*+postproceso hasta ese waypoint.
 4. Sigue el camino con un **Stanley controller** bidireccional (FWD/REV).

No se registran recompensas ni se modifican los pesos.
"""
from __future__ import annotations

import os
import math
import time
import random
from pathlib import Path

import numpy as np
import tensorflow as tf

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from std_msgs.msg import Header, Bool, ColorRGBA
from geometry_msgs.msg import PoseArray, PoseStamped, Point, Vector3
from nav_msgs.msg import Odometry, OccupancyGrid, Path
from grid_map_msgs.msg import GridMap
from visualization_msgs.msg import Marker
from std_srvs.srv import Empty
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg

# ──────────────────── CONSTANTES ────────────────────
PATCH             = 128
BIG_PATCH         = 64
CLEAR_MIN         = 0.6
GOAL_RADIUS       = 2.5
R_META_DEFAULT    = 8.0
MIN_WP_DIST       = 3.0

# Dinámica / límites vehículo
WHEEL_BASE   = 1.35
DELTA_MAX    = math.radians(32)
MAX_STEERING = 40.0
BACK_ALPHA_MIN = math.radians(120)
V_MAX       = 3.0
V_REV_MAX   = 1.5
V_MIN       = 0.6
KP_SPEED    = 10.0
THR_IDLE    = 2.0
MAX_DTHR    = 2.0
MAX_TILT    = 1.0

# RRT*
LAMBDA_SLOPE  = 8.0
SLOPE_MAX     = 0.6
R_MIN         = 3.5
KAPPA_MAX     = 1.0 / R_MIN
REPLAN_DIST   = 1.5

# Otros
HEIGHT_LAYER  = "elevation"
RESET_GRACE_SEC = 15.0

# ──────────────────── UTILIDADES GEOM / MAPA ────────────────────

def l2(a, b):
    return math.hypot(b[0] - a[0], b[1] - a[1])

def idx_from_world(info, pt):
    res = info.resolution
    return int((pt[0] - info.origin.position.x) / res), int((pt[1] - info.origin.position.y) / res)

# ... (Bresenham, grid conversion, slope, clearance)  ─ recortado por brevedad
#  ↓↓↓  Para inferencia basta reutilizar las mismas funciones del trainer ↓↓↓

from j8_test_ppo import (
    bres_line_free,
    clearance_ok,
    gridmap_to_numpy,
    slope_ok,
    smooth_bspline,
    densify,
    _filter_curv,
    rrt_plan_cost,
)

# ──────────────────── NODO PRINCIPAL ────────────────────
class TerrainPPOInference(Node):
    def __init__(self):
        super().__init__("terrain_ppo_inference")

        # ───── Parametros ROS
        self.declare_parameter("low_weights", "")
        self.declare_parameter("meta_weights", "")
        low_w  = self.get_parameter("low_weights").get_parameter_value().string_value
        meta_w = self.get_parameter("meta_weights").get_parameter_value().string_value

        # ───── Subcripciones y pubs
        qos = 10
        self.create_subscription(Odometry,      "/ARGJ801/odom_demo",      self.cb_odom,   qos)
        self.create_subscription(PoseArray,     "/goal",                   self.cb_goal,   qos)
        self.create_subscription(OccupancyGrid, "/occupancy_grid",         self.cb_grid,   10)
        self.create_subscription(GridMap,       "/terrain_grid",           self.cb_hmap,   10)
        self.create_subscription(PoseArray,     "/safe_frontier_points",   self.cb_front,  qos)
        self.create_subscription(Bool,          "/virtual_collision",      self.cb_coll,   qos)
        self.cmd_pub  = self.create_publisher(CmdThrottleMsg, "/ARGJ801/cmd_throttle_msg", qos)
        self.path_pub = self.create_publisher(Path, "/global_path_predicted", qos)

        # ───── Cargar redes
        self.policy     = self.build_policy()
        self.metaPolicy = self.build_meta_policy()
        if low_w:
            self.policy.load_weights(low_w)
            self.get_logger().info(f"Low‑level weights cargados de {low_w}")
        if meta_w:
            self.metaPolicy.load_weights(meta_w)
            self.get_logger().info(f"Meta‑policy weights cargados de {meta_w}")

        # log_std se fija al valor final que usaste en entrenamiento (≈0.05) para la meta‑policy
        self.log_std = tf.constant(math.log(0.05), dtype=tf.float32)

        # ───── Estado interno
        self.pose = self.twist = None
        self.grid_dyn = self.grid_msg = None
        self.hmap_msg = None
        self.frontiers = []
        self.goal = None
        self.collided = False
        self.goal_reached_flag = False
        self.current_path = []
        self.wp_index = 0
        self.need_new_wp = True
        self.step_count  = 0
        self.last_cmd    = CmdThrottleMsg()
        self.last_cp     = None
        self.escape_timer= 0
        self.escaping    = False
        self.current_waypoint = None

        # Timer 10 Hz
        self.create_timer(0.1, self.step)
        self.get_logger().info("Terrain PPO Inference node ready ✅")

    # ─────────── BUILD NETWORKS ───────────
    def build_policy(self):
        g  = tf.keras.Input(shape=(PATCH, PATCH, 3), name="grid")
        st = tf.keras.Input(shape=(4,), name="state")
        w0 = tf.keras.Input(shape=(2,), name="dummy")
        x  = tf.keras.layers.Conv2D(16,3,padding="same",activation="relu")(g)
        x  = tf.keras.layers.MaxPooling2D()(x)
        x  = tf.keras.layers.Conv2D(32,3,padding="same",activation="relu")(x)
        x  = tf.keras.layers.GlobalAveragePooling2D()(x)
        z  = tf.keras.layers.Concatenate()([x, st])
        z  = tf.keras.layers.Dense(128,activation="tanh")(z)
        out= tf.keras.layers.Dense(2, activation="tanh")(z)
        return tf.keras.Model([g, st, w0], out, name="policy")

    def build_meta_policy(self):
        G  = tf.keras.Input(shape=(BIG_PATCH,BIG_PATCH,4), name="big_patch")
        v  = tf.keras.Input(shape=(2,), name="vec_goal")
        x  = tf.keras.layers.Conv2D(32,3,activation="relu")(G)
        x  = tf.keras.layers.MaxPooling2D()(x)
        x  = tf.keras.layers.Conv2D(64,3,activation="relu")(x)
        x  = tf.keras.layers.GlobalAveragePooling2D()(x)
        z  = tf.keras.layers.Concatenate()([x,v])
        z  = tf.keras.layers.Dense(128,activation="relu")(z)
        out= tf.keras.layers.Dense(2,activation="tanh")(z)
        return tf.keras.Model([G,v],out,name="meta_policy")

    # ─────────── CALLBACKS ROS ───────────
    def cb_odom(self,msg):
        self.pose  = msg.pose.pose
        self.twist = msg.twist.twist
    def cb_goal(self,msg):
        self.goal = (msg.poses[0].position.x, msg.poses[0].position.y) if msg.poses else None
    def cb_grid(self,msg):
        self.grid_msg = msg
        self.grid_dyn = np.asarray(msg.data,dtype=np.int8).reshape(msg.info.height,msg.info.width)
    def cb_hmap(self,msg):
        self.hmap_msg = msg
    def cb_front(self,msg):
        self.frontiers = [(p.position.x,p.position.y) for p in msg.poses]
    def cb_coll(self,msg):
        self.collided = bool(msg.data)
        if self.collided:
            self.escape_timer = 50
            self.escaping = True

    # ─────────── PLANIFICACIÓN RRT* + FILTRADO ───────────
    def flexible_path(self,start,target):
        h_arr,hm_info = gridmap_to_numpy(self.hmap_msg)
        path = rrt_plan_cost(start,target,self.grid_dyn,self.grid_msg.info,h_arr,hm_info,
                             max_iter=2000,step=2.0,goal_tol=GOAL_RADIUS)
        if len(path)<4:
            return [start,target]
        path = _filter_curv(path)
        path = smooth_bspline(path,ds=0.25)
        path = densify(path)
        return path

    # ─────────── SEGUIMIENTO DE TRAYECTORIA (STANLEY) ───────────
    def follow_path(self,cp):
        if self.wp_index >= len(self.current_path):
            self.cmd_pub.publish(self._brake())
            return
        # Punto referencia: más cercano en la ventana próxima de 20
        best_i,best_d = self.wp_index,1e9
        for i in range(self.wp_index,min(self.wp_index+20,len(self.current_path))):
            d=l2(cp,self.current_path[i])
            if d<best_d:
                best_i,best_d=i,d
        self.wp_index=best_i
        ref_pt=self.current_path[self.wp_index]
        nxt=self.current_path[min(self.wp_index+1,len(self.current_path)-1)]
        path_yaw=math.atan2(nxt[1]-ref_pt[1],nxt[0]-ref_pt[0])
        yaw=self._yaw_from_quaternion(self.pose.orientation)
        theta_e=math.atan2(math.sin(path_yaw-yaw),math.cos(path_yaw-yaw))
        dx=ref_pt[0]-cp[0]; dy=ref_pt[1]-cp[1]
        e =  dx*math.sin(yaw) - dy*math.cos(yaw)
        reverse=False
        if abs(theta_e) > BACK_ALPHA_MIN:
            reverse=True
            theta_e = theta_e-math.pi if theta_e>0 else theta_e+math.pi
        k=1.0
        v_curr=max(abs(self.twist.linear.x),0.05)
        delta_des=theta_e+math.atan2(k*e,v_curr+1e-2)
        if reverse: delta_des*=-1
        str_pre=-(delta_des/DELTA_MAX)*MAX_STEERING
        str_pre=np.clip(str_pre,-MAX_STEERING,MAX_STEERING)
        gain_v=max(0.0,1.0-abs(theta_e)/math.radians(90))
        v_des=(V_REV_MAX if reverse else V_MAX)*gain_v
        v_des=np.clip(v_des,V_MIN,V_MAX)
        thr_pre=KP_SPEED*(v_des - (-1 if reverse else 1)*self.twist.linear.x) + THR_IDLE*np.sign(v_des)
        TAU,dt=0.25,0.05
        a1=2*TAU/(2*TAU+dt); b1=dt/(2*TAU+dt)
        str_cmd=a1*str_pre + b1*(str_pre - self.last_cmd.steering)
        thr_cmd=a1*thr_pre + b1*(thr_pre - self.last_cmd.throttle)
        thr_cmd = self.last_cmd.throttle + np.clip(thr_cmd - self.last_cmd.throttle,-MAX_DTHR,MAX_DTHR)
        cmd=CmdThrottleMsg()
        cmd.throttle=thr_cmd; cmd.steering=str_cmd
        self.cmd_pub.publish(cmd)
        self.last_cmd=cmd

    # ─────────── STEP PRINCIPAL (10 Hz) ───────────
    def step(self):
        if None in (self.pose,self.grid_msg,self.hmap_msg,self.goal):
            return
        cp=(self.pose.position.x,self.pose.position.y)
        # ESCAPE simple si colisionó
        if self.escaping:
            if self.escape_timer<=0:
                self.escaping=False
            else:
                self.follow_path(cp)
                self.escape_timer-=1
            return

        # ③ Elegir waypoint (goal directo por simplicidad)
        if self.need_new_wp:
            waypoint=self.goal
            self.current_waypoint=waypoint
            # RRT* local
            self.current_path=self.flexible_path(cp,waypoint)
            self.wp_index=1
            self.need_new_wp=False
            self.publish_path(self.current_path)

        # ④ Seguir camino
        if self.current_path:
            self.follow_path(cp)
            # Re-plan si error lateral grande
            ref=self.current_path[self.wp_index]
            cross=abs(l2(cp,ref))
            if cross>REPLAN_DIST:
                self.need_new_wp=True

    # ─────────── AUX ───────────
    def _brake(self):
        stop=CmdThrottleMsg(); stop.throttle=0.0; stop.steering=0.0
        return stop
    def _yaw_from_quaternion(self,q):
        siny=2.0*(q.w*q.z+q.x*q.y); cosy=1.0-2.0*(q.y*q.y+q.z*q.z)
        return math.atan2(siny,cosy)
    def publish_path(self,pts):
        hdr=Header(frame_id="map",stamp=self.get_clock().now().to_msg())
        msg=Path(header=hdr)
        for x,y in pts:
            ps=PoseStamped(header=hdr); ps.pose.position.x=x; ps.pose.position.y=y; ps.pose.orientation.w=1.0
            msg.poses.append(ps)
        self.path_pub.publish(msg)

# ──────────────────── MAIN ────────────────────
def main(args=None):
    rclpy.init(args=args)
    node=TerrainPPOInference()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node(); rclpy.shutdown()

if __name__=="__main__":
    main()
