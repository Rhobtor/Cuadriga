#!/usr/bin/env python3
import math
import numpy as np
from dataclasses import dataclass

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy, QoSDurabilityPolicy
from sensor_msgs.msg import LaserScan
from nav_msgs.msg import OccupancyGrid, MapMetaData
from geometry_msgs.msg import Pose
import tf2_ros
from tf2_ros import TransformException
from rclpy.duration import Duration


@dataclass
class LogOddsParams:
    l_occ: float = 2.0
    l_free: float = -0.7
    l_min: float = -4.0
    l_max: float = 4.0
    l_occ_thresh: float = 1.5
    l_free_thresh: float = -1.5


def bresenham_line(x0, y0, x1, y1):
    """Genera celdas de una línea en rejilla (enteros)."""
    x0 = int(x0); y0 = int(y0); x1 = int(x1); y1 = int(y1)
    dx = abs(x1 - x0)
    dy = -abs(y1 - y0)
    sx = 1 if x0 < x1 else -1
    sy = 1 if y0 < y1 else -1
    err = dx + dy
    x, y = x0, y0
    while True:
        yield (x, y)
        if x == x1 and y == y1:
            break
        e2 = 2 * err
        if e2 >= dy:
            err += dy
            x += sx
        if e2 <= dx:
            err += dx
            y += sy


class PriorMapBuilder(Node):
    def __init__(self):
        super().__init__('prior_map_builder')

        # --- Parámetros ---
        self.declare_parameter('scan_topic', '/virt_lidar/scan')
        self.declare_parameter('map_frame', 'world')   # mejor 'world' si el TF de Gazebo es world->link
        self.declare_parameter('resolution', 0.20)     # m/celda
        self.declare_parameter('width', 400)           # nº celdas (eje x)
        self.declare_parameter('height', 400)          # nº celdas (eje y)
        self.declare_parameter('origin_x', -40.0)      # m (origen del mapa en frame 'map_frame')
        self.declare_parameter('origin_y', -40.0)      # m
        self.declare_parameter('build_duration_sec', 5.0)  # tiempo de integración
        self.declare_parameter('max_scans', 0)             # 0 => ignora, usa tiempo
        self.declare_parameter('publish_topic', '/prior_map')

        self.scan_topic = self.get_parameter('scan_topic').get_parameter_value().string_value
        self.map_frame = self.get_parameter('map_frame').get_parameter_value().string_value
        self.res = float(self.get_parameter('resolution').value)
        self.w = int(self.get_parameter('width').value)
        self.h = int(self.get_parameter('height').value)
        self.ox = float(self.get_parameter('origin_x').value)
        self.oy = float(self.get_parameter('origin_y').value)
        self.build_duration = float(self.get_parameter('build_duration_sec').value)
        self.max_scans = int(self.get_parameter('max_scans').value)
        self.pub_topic = self.get_parameter('publish_topic').get_parameter_value().string_value

        # --- Estado ---
        self.lparams = LogOddsParams()
        self.grid = np.zeros((self.h, self.w), dtype=np.float32)  # log-odds
        self.built = False
        self.start_time = None
        self.scan_count = 0

        # --- TF ---
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self, spin_thread=True)

        # --- QoS ---
        sensor_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10
        )
        latched_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.RELIABLE,
            durability=QoSDurabilityPolicy.TRANSIENT_LOCAL,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1
        )

        # --- Pub/Sub ---
        self.scan_sub = self.create_subscription(LaserScan, self.scan_topic, self.scan_cb, sensor_qos)
        self.map_pub = self.create_publisher(OccupancyGrid, self.pub_topic, latched_qos)

        self.get_logger().info(
            f"Construyendo mapa a priori UNA SOLA VEZ de {self.w}x{self.h} @ {self.res} m/celda "
            f"en frame '{self.map_frame}'. Zona [({self.ox},{self.oy}) -> ({self.ox+self.w*self.res},{self.oy+self.h*self.res})]"
        )

    def world_to_grid(self, x, y):
        gx = int((x - self.ox) / self.res)
        gy = int((y - self.oy) / self.res)
        return gx, gy

    def in_bounds(self, gx, gy):
        return 0 <= gx < self.w and 0 <= gy < self.h

    def apply_logodds(self, gx, gy, delta):
        if not self.in_bounds(gx, gy):
            return
        newv = float(self.grid[gy, gx]) + float(delta)
        # saturación
        if newv > self.lparams.l_max: newv = self.lparams.l_max
        if newv < self.lparams.l_min: newv = self.lparams.l_min
        self.grid[gy, gx] = newv

    def scan_cb(self, scan: LaserScan):
        if self.built:
            return

        # Marca inicio
        if self.start_time is None:
            self.start_time = self.get_clock().now()
            self.get_logger().info("Comenzando integración del mapa a priori...")

        # TF: láser -> mapa
        laser_frame = scan.header.frame_id or 'virt_lidar_link'
        stamp = scan.header.stamp

        try:
            tf = self.tf_buffer.lookup_transform(self.map_frame, laser_frame, stamp, timeout=Duration(seconds=0.2))
        except TransformException as ex:
            self.get_logger().warn(f"No TF {self.map_frame}<-{laser_frame}: {ex}")
            return

        # Pose del láser en mapa
        tx = tf.transform.translation.x
        ty = tf.transform.translation.y
        tz = tf.transform.translation.z
        # orientación (usamos yaw para 2D; si tu sensor mira hacia abajo, rotación debe reflejarse en SDF)
        q = tf.transform.rotation
        # roll-pitch-yaw
        # (pequeña utilidad para yaw)
        yaw = self.quaternion_to_yaw(q.x, q.y, q.z, q.w)
        cos_y = math.cos(yaw)
        sin_y = math.sin(yaw)

        # celda del láser
        lx, ly = self.world_to_grid(tx, ty)
        if not self.in_bounds(lx, ly):
            # si el láser está fuera del mapa, no integramos este scan
            self.get_logger().warn_throttle(5.0, "Láser fuera del área del mapa a priori; ignoro scan.")
            return

        # Integra rayos
        angle = scan.angle_min
        for i, r in enumerate(scan.ranges):
            if math.isinf(r) or math.isnan(r):
                angle += scan.angle_increment
                continue

            r_clamped = max(min(r, scan.range_max), scan.range_min)
            # endpoint en marco del láser (2D)
            ex_l = r_clamped * math.cos(angle)
            ey_l = r_clamped * math.sin(angle)
            angle += scan.angle_increment

            # rota a yaw del láser y traslada a mundo
            ex_w = tx + (ex_l * cos_y - ey_l * sin_y)
            ey_w = ty + (ex_l * sin_y + ey_l * cos_y)

            gx1, gy1 = self.world_to_grid(ex_w, ey_w)

            # Libre a lo largo del rayo (excepto la celda final)
            for (gx, gy) in bresenham_line(lx, ly, gx1, gy1):
                if gx == gx1 and gy == gy1:
                    break
                self.apply_logodds(gx, gy, self.lparams.l_free)

            # Ocupada al final si era un hit “real” (no clamped por max)
            if r <= (scan.range_max - 1e-3):
                self.apply_logodds(gx1, gy1, self.lparams.l_occ)
            else:
                # si fue max range: solo libres, no marcamos ocupación
                pass

        self.scan_count += 1

        # ¿Parar por tiempo o por número de scans?
        elapsed = (self.get_clock().now() - self.start_time).nanoseconds * 1e-9
        if (self.max_scans > 0 and self.scan_count >= self.max_scans) or (elapsed >= self.build_duration):
            self.freeze_and_publish()

    def freeze_and_publish(self):
        # Convierte log-odds -> OccupancyGrid
        grid_int8 = np.full((self.h, self.w), -1, dtype=np.int8)  # -1 unknown

        occ_mask = self.grid >= self.lparams.l_occ_thresh
        free_mask = self.grid <= self.lparams.l_free_thresh

        grid_int8[occ_mask] = 100
        grid_int8[free_mask] = 0

        # Mensaje
        msg = OccupancyGrid()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = self.map_frame

        info = MapMetaData()
        info.resolution = float(self.res)
        info.width = self.w
        info.height = self.h

        # Origen del mapa (pose de la celda [0,0])
        origin = Pose()
        origin.position.x = float(self.ox)
        origin.position.y = float(self.oy)
        origin.position.z = 0.0
        origin.orientation.w = 1.0
        info.origin = origin

        msg.info = info
        msg.data = grid_int8.flatten(order='C').tolist()

        # Publica “latched”
        self.map_pub.publish(msg)

        # Detiene suscripción
        if self.scan_sub is not None:
            self.destroy_subscription(self.scan_sub)
            self.scan_sub = None

        self.built = True
        self.get_logger().info(
            f"Mapa a priori listo (scans={self.scan_count}). Publicado en '{self.pub_topic}' con QoS TRANSIENT_LOCAL. "
            "No se actualizará más."
        )

    @staticmethod
    def quaternion_to_yaw(x, y, z, w):
        # yaw (z)
        siny_cosp = 2.0 * (w * z + x * y)
        cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
        return math.atan2(siny_cosp, cosy_cosp)


def main(args=None):
    rclpy.init(args=args)
    node = PriorMapBuilder()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
