#!/usr/bin/env python3
"""
DynamicOccupancyGridFuser2p5D
=============================

Fusiona diferentes fuentes de información de terreno para generar un *GridMap* 2.5 D con capas:
  • **occupancy**  – 0 = libre, 100 = obstáculo/intransitable
  • **elevation**  – altura absoluta en metros (NaN donde no hay dato)
  • **slope**      – pendiente en grados
  • **fissure**    – máscara binaria 1=fisura ancha

La pendiente y las fisuras se vuelcan de forma inmediata en `occupancy`, de modo que
el planificador downstream evita zonas peligrosas.  Además, publica estadísticas de
pendiente para ayudar a elegir el umbral óptimo (`slope_max_deg`).

Entradas (QoS 10):
  • /filtered_navigation_nodes   (PoseArray)  → puntos libres
  • /obstacle_navigation_nodes   (PoseArray)  → puntos obstáculo
  • /robot_pose_topic            (PoseArray)  → posición robot (primer pose)
  • /ground_nodes_height         (PointCloud2)→ nube densa con altura del suelo

Salidas:
  • /occupancy_grid           (nav_msgs/OccupancyGrid)
  • /elevation_grid           (std_msgs/Float32MultiArray)
  • /elevation_image          (sensor_msgs/Image, mono8)
  • /elevation_gridmap        (grid_map_msgs/GridMap)
  • /terrain_slope            (std_msgs/Float32MultiArray)

Autor: ChatGPT – mayo 2025
"""

import math
from typing import Optional

import cv2
import numpy as np
import rclpy
from cv_bridge import CvBridge
from geometry_msgs.msg import PoseArray
from nav_msgs.msg import OccupancyGrid, MapMetaData
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray, MultiArrayDimension
from sensor_msgs.msg import Image, PointCloud2
from sensor_msgs_py import point_cloud2
from grid_map_msgs.msg import GridMap, GridMapInfo


class DynamicOccupancyGridFuser2p5D(Node):
    """Fusiona nodos de navegación, obstáculos y alturas para producir un GridMap 2.5 D."""

    # ————————————————————————————————————————————
    # ❱❱  Constructor y parámetros
    # ————————————————————————————————————————————
    def __init__(self):
        super().__init__('dynamic_occupancy_grid_fuser_2p5d')

        # — Básicos del mapa —
        self.declare_parameter('resolution', 1.0)             # m/celda
        self.declare_parameter('margin', 10)                  # celdas extra
        self.declare_parameter('origin_x', -25.0)             # m
        self.declare_parameter('origin_y', -25.0)             # m
        self.declare_parameter('width', 100)                  # celdas
        self.declare_parameter('height', 100)                 # celdas
        self.declare_parameter('sensor_range', 10.0)          # m para ignorar obstáculos lejanos

        # — Terreno peligroso —
        self.declare_parameter('slope_max_deg', 15.0)         # º
        self.declare_parameter('fissure_min_width', 0.5)      # m

        # Recuperar parámetros
        p = lambda k: self.get_parameter(k).get_parameter_value()
        self.resolution: float = p('resolution').double_value
        self.margin: int = p('margin').integer_value
        self.origin_x: float = p('origin_x').double_value
        self.origin_y: float = p('origin_y').double_value
        self.width: int = p('width').integer_value
        self.height: int = p('height').integer_value
        self.sensor_range: float = p('sensor_range').double_value
        self.slope_max_deg: float = p('slope_max_deg').double_value
        self.fissure_min_width: float = p('fissure_min_width').double_value

        # — Buffers (se ajustan dinámicamente) —
        self._allocate_grids(self.height, self.width)

        # — Datos entrantes —
        self.free_points: Optional[PoseArray] = None
        self.obstacle_points: Optional[PoseArray] = None
        self.robot_pose = None

        # — Suscripciones —
        self.create_subscription(PoseArray, '/filtered_navigation_nodes', self.free_cb, 10)
        self.create_subscription(PoseArray, '/obstacle_navigation_nodes', self.obs_cb, 10)
        self.create_subscription(PoseArray, '/robot_pose_topic', self.robot_cb, 10)
        self.create_subscription(PointCloud2, '/ground_nodes_height', self.height_cb, 10)

        # — Publicadores —
        self.occ_pub = self.create_publisher(OccupancyGrid, '/occupancy_grid_test', 10)
        self.elev_pub = self.create_publisher(Float32MultiArray, '/elevation_grid_test', 10)
        self.image_pub = self.create_publisher(Image, '/elevation_image_test', 10)
        self.grid_map_pub = self.create_publisher(GridMap, '/elevation_gridmap_test', 10)
        self.slope_pub = self.create_publisher(Float32MultiArray, '/terrain_slope_test', 10)

        # CvBridge para imágenes RViz
        self.bridge = CvBridge()

        # Ejecutar a 1 Hz
        self.create_timer(1.0, self.publish_maps)
        self.get_logger().info('DynamicOccupancyGridFuser2.5D iniciado.')

    # ————————————————————————————————————————————
    # ❱❱  Callbacks de suscripción
    # ————————————————————————————————————————————
    def free_cb(self, msg: PoseArray):
        self.free_points = msg

    def obs_cb(self, msg: PoseArray):
        self.obstacle_points = msg

    def robot_cb(self, msg: PoseArray):
        if msg.poses:
            self.robot_pose = msg.poses[0]

    def height_cb(self, cloud: PointCloud2):
        """Rellena la malla de elevación con la nube densificada del suelo."""
        for x, y, z in point_cloud2.read_points(cloud, field_names=('x', 'y', 'z'), skip_nans=True):
            i, j = self.world_to_map(x, y)
            if 0 <= i < self.width and 0 <= j < self.height:
                self.elev_grid[j, i] = z

    # ————————————————————————————————————————————
    # ❱❱  Funciones auxiliares
    # ————————————————————————————————————————————
    def world_to_map(self, x: float, y: float):
        """Convierte coordenadas mundo → índices de celda (i, j)."""
        i = int((x - self.origin_x) / self.resolution)
        j = int((y - self.origin_y) / self.resolution)
        return i, j

    def _allocate_grids(self, h: int, w: int):
        """Crea/reinicia buffers internos con tamaño (h, w)."""
        self.elev_grid = np.full((h, w), np.nan, dtype=np.float32)
        self.slope_grid = np.full_like(self.elev_grid, np.nan, dtype=np.float32)
        self.fissure_mask = np.zeros_like(self.elev_grid, dtype=np.uint8)

    # ————————————————————————————————————————————
    # ❱❱  Paso principal (1 Hz)
    # ————————————————————————————————————————————
    def publish_maps(self):
        # 1) Ajustar tamaño del mapa
        self.update_map_size()

        # Buffers de salida
        occ = -1 * np.ones((self.height, self.width), dtype=np.int8)  # valor -1: desconocido
        elev = np.full_like(self.elev_grid, np.nan, dtype=np.float32)  # copia limpia para publicar

        # ————————————————————
        # 2) Volcar puntos libres
        # ————————————————————
        if self.free_points:
            for p in self.free_points.poses:
                i, j = self.world_to_map(p.position.x, p.position.y)
                if 0 <= i < self.width and 0 <= j < self.height:
                    occ[j, i] = 0
                    elev[j, i] = p.position.z
                    self.elev_grid[j, i] = p.position.z  # refuerza valor

        # 3) Volcar obstáculos (dentro de rango)
        if self.obstacle_points:
            for p in self.obstacle_points.poses:
                if self.robot_pose:
                    if math.hypot(p.position.x - self.robot_pose.position.x,
                                   p.position.y - self.robot_pose.position.y) > self.sensor_range:
                        continue
                i, j = self.world_to_map(p.position.x, p.position.y)
                if 0 <= i < self.width and 0 <= j < self.height:
                    occ[j, i] = 100
                    elev[j, i] = p.position.z
                    self.elev_grid[j, i] = p.position.z

        # ————————————————————
        # 4) Cálculo de pendiente
        # ————————————————————
        self.compute_slope()
        occ[self.slope_grid > self.slope_max_deg] = 100

        # ————————————————————
        # 5) Detección de fisuras anchas
        # ————————————————————
        self.detect_fissures()
        occ[self.fissure_mask == 1] = 100

        # ————————————————————
        # 6) Publicaciones
        # ————————————————————
        self.publish_occupancy_grid(occ)
        self.publish_elevation_array(elev)
        self.publish_slope_array()
        self.publish_elevation_image(elev)
        self.publish_grid_map(occ, elev)

    # ————————————————————————————————————————————
    # ❱❱  Funciones de procesado del terreno
    # ————————————————————————————————————————————
    def compute_slope(self):
        """Calcula pendiente en grados a partir de la malla de elevación."""
        gy, gx = np.gradient(self.elev_grid, self.resolution, edge_order=1)
        self.slope_grid[:] = np.degrees(np.arctan(np.sqrt(gx**2 + gy**2)))

        # Estadísticas para tunear el umbral
        valid = ~np.isnan(self.slope_grid)
        if valid.any():
            s_max = float(np.nanmax(self.slope_grid))
            s_mean = float(np.nanmean(self.slope_grid))
            s_p95 = float(np.nanpercentile(self.slope_grid, 95))
            self.get_logger().info(f'Slope stats → mean={s_mean:.1f}°, 95%={s_p95:.1f}°, max={s_max:.1f}°')

    def detect_fissures(self):
        """Genera máscara binaria de fisuras anchas (brechas sin dato)."""
        nan_mask = np.isnan(self.elev_grid).astype(np.uint8)
        closed = cv2.morphologyEx(nan_mask, cv2.MORPH_CLOSE, np.ones((3, 3), np.uint8))
        fissure_candidates = nan_mask & ~closed

        self.fissure_mask.fill(0)
        num_lbl, labels, stats, _ = cv2.connectedComponentsWithStats(fissure_candidates, connectivity=8)
        min_px = int(self.fissure_min_width / self.resolution)
        for k in range(1, num_lbl):
            w, h = stats[k, cv2.CC_STAT_WIDTH], stats[k, cv2.CC_STAT_HEIGHT]
            if max(w, h) >= min_px:
                self.fissure_mask[labels == k] = 1

    # ————————————————————————————————————————————
    # ❱❱  Publicadores auxiliares
    # ————————————————————————————————————————————
    def publish_occupancy_grid(self, occ: np.ndarray):
        msg = OccupancyGrid()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'map'

        info = MapMetaData()
        info.resolution = self.resolution
        info.width = self.width
        info.height = self.height
        info.origin.position.x = self.origin_x
        info.origin.position.y = self.origin_y
        info.origin.orientation.w = 1.0
        msg.info = info
        msg.data = occ.flatten().tolist()
        self.occ_pub.publish(msg)

    def publish_elevation_array(self, elev: np.ndarray):
        arr = Float32MultiArray()
        d0 = MultiArrayDimension(label='height', size=self.height, stride=self.width * self.height)
        d1 = MultiArrayDimension(label='width', size=self.width, stride=self.width)
        arr.layout.dim = [d0, d1]
        arr.data = np.nan_to_num(elev, nan=0.0).flatten().astype(np.float32).tolist()
        self.elev_pub.publish(arr)

    def publish_slope_array(self):
        arr = Float32MultiArray()
        d0 = MultiArrayDimension(label='height', size=self.height, stride=self.width * self.height)
        d1 = MultiArrayDimension(label='width', size=self.width, stride=self.width)
        arr.layout.dim = [d0, d1]
        arr.data = np.nan_to_num(self.slope_grid, nan=0.0).flatten().astype(np.float32).tolist()
        self.slope_pub.publish(arr)

    def publish_elevation_image(self, elev: np.ndarray):
        img = np.zeros_like(elev, dtype=np.uint8)
        valid = ~np.isnan(elev)
        if valid.any():
            mn, mx = np.nanmin(elev), np.nanmax(elev)
            if mx > mn:
                norm = ((elev - mn) / (mx - mn) * 255.0)
                img = np.where(valid, norm, 0).astype(np.uint8)
        img_msg = self.bridge.cv2_to_imgmsg(img, encoding='mono8')
        img_msg.header.stamp = self.get_clock().now().to_msg()
        img_msg.header.frame_id = 'map'
        self.image_pub.publish(img_msg)

    def publish_grid_map(self, occ: np.ndarray, elev: np.ndarray):
        gm = GridMap()
        gm.header.stamp = self.get_clock().now().to_msg()
        gm.header.frame_id = 'map'

        info = GridMapInfo()
        info.resolution = self.resolution
        info.length_x = self.width * self.resolution
        info.length_y = self.height * self.resolution
        info.pose.position.x = self.origin_x + info.length_x / 2.0
        info.pose.position.y = self.origin_y + info.length_y / 2.0
        info.pose.orientation.w = 1.0
        gm.info = info
        gm.layers = ['occupancy', 'elevation', 'slope', 'fissure']

                # —— Reutilizar MultiArrayLayout para las cuatro capas ——
        d0 = MultiArrayDimension(label='height', size=self.height, stride=self.width * self.height)
        d1 = MultiArrayDimension(label='width',  size=self.width,  stride=self.width)

        def build_layer(np_data: np.ndarray) -> Float32MultiArray:
            fa = Float32MultiArray()
            fa.layout.dim = [d0, d1]
            fa.data = np_data.flatten().tolist()
            return fa

        fm_occ  = build_layer(occ.astype(np.float32))
        fm_elev = build_layer(np.nan_to_num(elev, nan=0.0))
        fm_slope= build_layer(np.nan_to_num(self.slope_grid, nan=0.0))
        fm_fiss = build_layer(self.fissure_mask.astype(np.float32))
        gm.data = [fm_occ, fm_elev, fm_slope, fm_fiss]
        self.grid_map_pub.publish(gm)

    # ————————————————————————————————————————————
    # ❱❱  Funciones de gestión de tamaño dinámico
    # ————————————————————————————————————————————
    def update_map_size(self):
        """Ajusta dinámicamente el mapa para cubrir todos los puntos."""
        pts = []
        for arr in (self.free_points, self.obstacle_points):
            if arr:
                pts.extend([(p.position.x, p.position.y) for p in arr.poses])
        if not pts:
            return

        xs, ys = zip(*pts)
        mg = self.margin * self.resolution
        ox = min(self.origin_x, min(xs) - mg)
        oy = min(self.origin_y, min(ys) - mg)
        w = int(math.ceil((max(xs) - ox) / self.resolution)) + self.margin
        h = int(math.ceil((max(ys) - oy) / self.resolution)) + self.margin

        if w != self.width or h != self.height or ox != self.origin_x or oy != self.origin_y:
            self.get_logger().info(f'Resizing map: {self.width}×{self.height} → {w}×{h}')
            self.origin_x, self.origin_y, self.width, self.height = ox, oy, w, h
            self._allocate_grids(h, w)


def main(args=None):
    rclpy.init(args=args)
    node = DynamicOccupancyGridFuser2p5D()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
