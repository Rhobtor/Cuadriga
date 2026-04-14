# # #!/usr/bin/env python3
# # import rclpy
# # from rclpy.node import Node
# # from nav_msgs.msg import OccupancyGrid, MapMetaData
# # from geometry_msgs.msg import PoseArray, Pose
# # from std_msgs.msg import Header
# # import numpy as np
# # import cv2
# # import math
# # import time

# # class DynamicOccupancyGridFuser(Node):
# #     def __init__(self):
# #         super().__init__('dynamic_occupancy_grid_fuser')
# #         # Parámetros base para el mapa (valores iniciales)
# #         self.resolution = 1.0         # metros por celda
# #         self.margin = 10              # margen extra en celdas para expansión
# #         # Valores iniciales para el grid
# #         self.origin_x = -25.0         # origen en x
# #         self.origin_y = -25.0         # origen en y
# #         self.width = 100              # celdas en x
# #         self.height = 100             # celdas en y

# #         # Nuevo parámetro: rango máximo para considerar obstáculos "actuales"
# #         self.sensor_range = 40.0      # en metros

# #         # Variables para almacenar la información de los topics
# #         # (en este caso, no se usa timestamp ya que filtraremos por distancia)
# #         self.free_points = None       # PoseArray
# #         self.obstacle_points = None   # PoseArray

# #         # Suscriptores
# #         self.create_subscription(PoseArray, '/filtered_navigation_nodes', self.free_points_callback, 10)
# #         self.create_subscription(PoseArray, '/terrain_slope_pose', self.free_points_callback, 10)
# #         self.create_subscription(PoseArray, '/obstacle_navigation_nodes', self.obstacle_points_callback, 10)

# #         # Publicador del OccupancyGrid
# #         self.occ_grid_pub = self.create_publisher(OccupancyGrid, '/occupancy_grid', 10)

# #         # Timer para actualizar y publicar el mapa de ocupación (por ejemplo, 1 Hz)
# #         self.create_timer(0.1, self.publish_occupancy_grid)
# #         self.get_logger().info("Nodo DynamicOccupancyGridFuser iniciado.")

# #         # Para conocer la posición actual del robot (necesario para filtrar obstáculos)
# #         self.robot_pose = None
# #         self.create_subscription(PoseArray, '/robot_pose_topic', self.robot_pose_callback, 10)  # Cambia el topic si es necesario

# #     # def free_points_callback(self, msg: PoseArray):
# #     #     self.free_points = msg

# #     def free_points_callback(self, msg: PoseArray):
# #         # si ya había una lista, concaténala
# #         if self.free_points is None:
# #             self.free_points = PoseArray(header=msg.header, poses=list(msg.poses))
# #         else:
# #             self.free_points.poses.extend(msg.poses)

# #     def obstacle_points_callback(self, msg: PoseArray):
# #         self.obstacle_points = msg

# #     def robot_pose_callback(self, msg: PoseArray):
# #         # Asumimos que viene un PoseArray con al menos una pose, la primera es la posición actual
# #         if msg.poses:
# #             self.robot_pose = msg.poses[0]

# #     def world_to_map(self, x, y):
# #         # Convierte coordenadas del mundo a índices en la matriz del mapa
# #         i = int((x - self.origin_x) / self.resolution)
# #         j = int((y - self.origin_y) / self.resolution)
# #         return i, j

# #     def update_map_size(self):
# #         """
# #         Recalcula los límites del mapa basados en los puntos libres y de obstáculos,
# #         y ajusta el origen, el ancho y la altura para incluir todos los puntos.
# #         """
# #         all_points = []
# #         for msg in [self.free_points, self.obstacle_points]:
# #             if msg is None:
# #                 continue
# #             for pose in msg.poses:
# #                 all_points.append((pose.position.x, pose.position.y))
# #         if not all_points:
# #             return

# #         xs, ys = zip(*all_points)
# #         min_x, max_x = min(xs), max(xs)
# #         min_y, max_y = min(ys), max(ys)
# #         # Añadir margen (en metros)
# #         margin_m = self.margin * self.resolution

# #         new_origin_x = min(self.origin_x, min_x - margin_m)
# #         new_origin_y = min(self.origin_y, min_y - margin_m)
# #         new_width = int(math.ceil((max_x - new_origin_x) / self.resolution)) + self.margin
# #         new_height = int(math.ceil((max_y - new_origin_y) / self.resolution)) + self.margin

# #         if new_width != self.width or new_height != self.height or new_origin_x != self.origin_x or new_origin_y != self.origin_y:
# #             self.get_logger().info(
# #                 f"Expandiendo mapa: nuevo origen=({new_origin_x:.2f}, {new_origin_y:.2f}), tamaño=({new_width}x{new_height})"
# #             )
# #         self.origin_x = new_origin_x
# #         self.origin_y = new_origin_y
# #         self.width = new_width
# #         self.height = new_height

# #     def publish_occupancy_grid(self):
# #         # Actualiza el tamaño del mapa basado en los puntos actuales
# #         self.update_map_size()

# #         # Inicializa el grid con -1 (desconocido)
# #         grid = -1 * np.ones((self.height, self.width), dtype=np.int8)
# #         free_mask = np.zeros((self.height, self.width), dtype=np.uint8)
# #         obs_mask = np.zeros((self.height, self.width), dtype=np.uint8)

# #         # Dibujar puntos libres
# #         if self.free_points is not None:
# #             for pose in self.free_points.poses:
# #                 x = pose.position.x
# #                 y = pose.position.y
# #                 i, j = self.world_to_map(x, y)
# #                 if 0 <= i < self.width and 0 <= j < self.height:
# #                     free_mask[j, i] = 255

# #         # Rellenar áreas libres
# #         if np.count_nonzero(free_mask) > 0:
# #             contours, _ = cv2.findContours(free_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
# #             cv2.fillPoly(free_mask, contours, 255)

# #         # Dibujar puntos de obstáculos, filtrando por distancia al robot
# #         if self.obstacle_points is not None:
# #             for pose in self.obstacle_points.poses:
# #                 x = pose.position.x
# #                 y = pose.position.y
# #                 # Si se conoce la posición del robot, se filtran los obstáculos lejanos
# #                 if self.robot_pose is not None:
# #                     rx = self.robot_pose.position.x
# #                     ry = self.robot_pose.position.y
# #                     if distance((x, y), (rx, ry)) > self.sensor_range:
# #                         continue  # Se omite el obstáculo si está fuera del rango actual
# #                 i, j = self.world_to_map(x, y)
# #                 if 0 <= i < self.width and 0 <= j < self.height:
# #                     obs_mask[j, i] = 255

# #         # Dilatación para obstáculo (ajustable según necesidad)
# #         kernel = np.ones((1, 1), np.uint8)
# #         obs_mask = cv2.dilate(obs_mask, kernel, iterations=1)

# #         # Fusionar las máscaras:
# #         grid[free_mask == 255] = 0
# #         grid[obs_mask == 255] = 100

# #         # Preparar y publicar el mensaje OccupancyGrid
# #         occ_grid_msg = OccupancyGrid()
# #         occ_grid_msg.header.stamp = self.get_clock().now().to_msg()
# #         occ_grid_msg.header.frame_id = "map"
# #         occ_grid_msg.info = MapMetaData()
# #         occ_grid_msg.info.resolution = self.resolution
# #         occ_grid_msg.info.width = self.width
# #         occ_grid_msg.info.height = self.height
# #         occ_grid_msg.info.origin.position.x = self.origin_x
# #         occ_grid_msg.info.origin.position.y = self.origin_y
# #         occ_grid_msg.info.origin.position.z = 0.0
# #         occ_grid_msg.info.origin.orientation.w = 1.0
# #         occ_grid_msg.data = grid.flatten().tolist()

# #         self.occ_grid_pub.publish(occ_grid_msg)
# #         self.get_logger().info(f"Mapa de ocupación publicado a las {time.strftime('%H:%M:%S')}")

# # def distance(p1, p2):
# #     return math.hypot(p2[0] - p1[0], p2[1] - p1[1])

# # def main(args=None):
# #     rclpy.init(args=args)
# #     node = DynamicOccupancyGridFuser()
# #     try:
# #         rclpy.spin(node)
# #     except KeyboardInterrupt:
# #         pass
# #     node.destroy_node()
# #     rclpy.shutdown()

# # if __name__ == '__main__':
# #     main()


# #!/usr/bin/env python3
# # -*- coding: utf-8 -*-
# """
# DynamicOccupancyGridFuser
# Combina dos PoseArray de “zonas libres” y uno de “obstáculos” para generar
# un OccupancyGrid dinámico.

# Autor: ChatGPT · 27-may-2025
# """
# import math
# import time
# from typing import List, Tuple, Optional

# import cv2                       # sudo apt install ros-humble-vision-opencv
# import numpy as np
# import rclpy
# from rclpy.node   import Node
# from nav_msgs.msg import OccupancyGrid, MapMetaData
# from geometry_msgs.msg import PoseArray, Pose
# from std_msgs.msg import Header


# def euclidean(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
#     """Distancia Euclídea 2-D."""
#     return math.hypot(p2[0] - p1[0], p2[1] - p1[1])


# class DynamicOccupancyGridFuser(Node):
#     def __init__(self) -> None:
#         super().__init__("dynamic_occupancy_grid_fuser")

#         # -------- parámetros principales (puedes remapear con ros2 param) -----
#         self.declare_parameter("resolution",                1.0)   # m/px
#         self.declare_parameter("margin_cells",             10)     # celdas extra al expandir
#         self.declare_parameter("sensor_range",             40.0)   # m (para filtrar obst)
#         self.declare_parameter("robot_pose_topic", "/robot_pose_topic")

#         self.resolution: float = self.get_parameter("resolution").get_parameter_value().double_value
#         self.margin:     int   = self.get_parameter("margin_cells").get_parameter_value().integer_value
#         self.sensor_range: float = self.get_parameter("sensor_range").get_parameter_value().double_value
#         robot_pose_topic = self.get_parameter("robot_pose_topic").get_parameter_value().string_value

#         # -------- mapa inicial (pequeño, se expandirá al llegar datos) --------
#         self.origin_x = -25.0   # metros
#         self.origin_y = -25.0
#         self.width  = 100       # celdas
#         self.height = 100

#         # -------- buffers de datos -------------------------------------------
#         self.free_messages: List[PoseArray] = []      # union de ambos topics libres
#         self.obstacle_msg: Optional[PoseArray] = None
#         self.robot_pose:   Optional[Pose] = None
#         self.last_filtered_msg: PoseArray | None = None
#         self.last_slope_msg:    PoseArray | None = None
#         # -------- suscripciones ----------------------------------------------
#         qos = 10
#         self.create_subscription(PoseArray, "/filtered_navigation_nodes",
#                                  self.free_cb,  qos)
#         self.create_subscription(PoseArray, "/terrain_slope_pose",
#                                  self.free_cb,  qos)
#         self.create_subscription(PoseArray, "/obstacle_navigation_nodes",
#                                  self.obstacle_cb, qos)
#         self.create_subscription(PoseArray, robot_pose_topic,
#                                  self.robot_pose_cb, qos)

#         # -------- publicador del grid -----------------------------------------
#         self.grid_pub = self.create_publisher(OccupancyGrid, "/occupancy_grid", 10)

#         # -------- temporizador de publicación (10 Hz) -------------------------
#         self.create_timer(0.1, self.publish_grid)

#         self.get_logger().info("DynamicOccupancyGridFuser listo.")

#     # =====================================================================
#     #  Callbacks
#     # =====================================================================
#     def free_cb(self, msg: PoseArray) -> None:
#         """Acumula los mensajes de zonas libres (dos topics distintos)."""
#         self.free_messages.append(msg)

#     # def free_cb(self, msg):
#     #     if msg.header.frame_id == "filtered_frame":
#     #         self.last_filtered_msg = msg
#     #     else:  # terrain_slope_pose
#     #         self.last_slope_msg = msg
#     #     self.free_messages = [m for m in (self.last_filtered_msg,
#     #                                     self.last_slope_msg) if m]


#     def obstacle_cb(self, msg: PoseArray) -> None:
#         self.obstacle_msg = msg

#     def robot_pose_cb(self, msg: PoseArray) -> None:
#         if msg.poses:
#             self.robot_pose = msg.poses[0]

#     # =====================================================================
#     #  Utilidades
#     # =====================================================================
#     def world_to_map(self, x: float, y: float) -> Tuple[int, int]:
#         """Convierte coordenadas del mundo (m) a índices (col, row)."""
#         col = int((x - self.origin_x) / self.resolution)
#         row = int((y - self.origin_y) / self.resolution)
#         return col, row

#     def maybe_expand_map(self) -> None:
#         """Ajusta origen y tamaño para incluir todos los puntos recibidos."""
#         xs: List[float] = []
#         ys: List[float] = []

#         for msg in self.free_messages:
#             xs.extend([p.position.x for p in msg.poses])
#             ys.extend([p.position.y for p in msg.poses])
#         if self.obstacle_msg:
#             xs.extend([p.position.x for p in self.obstacle_msg.poses])
#             ys.extend([p.position.y for p in self.obstacle_msg.poses])

#         if not xs:        # no data yet
#             return

#         # márgenes en metros
#         margin_m = self.margin * self.resolution
#         new_origin_x = min(self.origin_x, min(xs) - margin_m)
#         new_origin_y = min(self.origin_y, min(ys) - margin_m)
#         new_width  = int(math.ceil((max(xs) - new_origin_x) / self.resolution)) + self.margin
#         new_height = int(math.ceil((max(ys) - new_origin_y) / self.resolution)) + self.margin

#         if (new_width  != self.width  or
#             new_height != self.height or
#             new_origin_x != self.origin_x or
#             new_origin_y != self.origin_y):
#             self.get_logger().info(
#                 f"Mapa expandido → origen=({new_origin_x:.1f},{new_origin_y:.1f}) "
#                 f"tamaño=({new_width}×{new_height})")
#             self.origin_x, self.origin_y = new_origin_x, new_origin_y
#             self.width, self.height     = new_width,  new_height

#     # =====================================================================
#     #  Publicación del OccupancyGrid
#     # =====================================================================
#     def publish_grid(self) -> None:
#         """Construye y publica el OccupancyGrid."""
#         # sin datos, nada que publicar
#         if not self.free_messages and self.obstacle_msg is None:
#             return

#         # 1) asegurarse de que todos los puntos quepan
#         self.maybe_expand_map()

#         # 2) máscaras de 0/255 para libres y obstáculos
#         free_mask = np.zeros((self.height, self.width), dtype=np.uint8)
#         obst_mask = np.zeros_like(free_mask)

#         # --- puntos libres (se toman todos, sin filtrar) -----------------
#         for msg in self.free_messages:
#             for pose in msg.poses:
#                 col, row = self.world_to_map(pose.position.x, pose.position.y)
#                 if 0 <= col < self.width and 0 <= row < self.height:
#                     free_mask[row, col] = 255
#         # rellenamos contornos para “pintar” áreas libres continuas
#         if np.any(free_mask):
#             contours, _ = cv2.findContours(free_mask, cv2.RETR_EXTERNAL,
#                                            cv2.CHAIN_APPROX_SIMPLE)
#             cv2.fillPoly(free_mask, contours, 255)

#         # --- puntos de obstáculos (filtrado por rango opcional) ----------
#         if self.obstacle_msg:
#             for pose in self.obstacle_msg.poses:
#                 if self.robot_pose:
#                     if euclidean((pose.position.x, pose.position.y),
#                                  (self.robot_pose.position.x, self.robot_pose.position.y)) > self.sensor_range:
#                         continue            # demasiado lejos
#                 col, row = self.world_to_map(pose.position.x, pose.position.y)
#                 if 0 <= col < self.width and 0 <= row < self.height:
#                     obst_mask[row, col] = 255
#         # dilatación para engrosar obstáculos (ajusta si lo necesitas)
#         if np.any(obst_mask):
#             kernel = np.ones((1, 1), np.uint8)
#             obst_mask = cv2.dilate(obst_mask, kernel, iterations=1)

#         # 3) fusionar en la grilla final: -1 desconocido, 0 libre, 100 ocupado
#         grid = -1 * np.ones_like(free_mask, dtype=np.int8)
#         grid[free_mask == 255] = 0
#         grid[obst_mask == 255] = 100

#         # 4) preparar mensaje OccupancyGrid
#         msg = OccupancyGrid()
#         msg.header = Header()
#         msg.header.stamp = self.get_clock().now().to_msg()
#         msg.header.frame_id = "map"

#         info = MapMetaData()
#         info.resolution = self.resolution
#         info.width  = self.width
#         info.height = self.height
#         info.origin.position.x = self.origin_x
#         info.origin.position.y = self.origin_y
#         info.origin.orientation.w = 1.0
#         msg.info = info
#         msg.data = grid.flatten().tolist()

#         self.grid_pub.publish(msg)
#         self.get_logger().debug(f"OccupancyGrid publicado ({self.width}×{self.height}) "
#                                 f"a las {time.strftime('%H:%M:%S')}")

#         # 5) opcional: vaciar buffers antiguos para evitar crecimiento indefinido
#         #self.free_messages.clear()
#         # (no se vacía obstacle_msg: asumimos que se actualiza continuamente)

# # =========================================================================
# #  main
# # =========================================================================
# def main(args=None) -> None:
#     rclpy.init(args=args)
#     node = DynamicOccupancyGridFuser()
#     try:
#         rclpy.spin(node)
#     except KeyboardInterrupt:
#         pass
#     node.destroy_node()
#     rclpy.shutdown()


# if __name__ == "__main__":
#     main()


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
DynamicOccupancyGridFuser
Fusor de dos PoseArray de zonas libres y uno de obstáculos.
Publica un OccupancyGrid con -1 UNKNOWN, 0 FREE y 100 OCCUPIED.  
Mantiene un anillo UNKNOWN alrededor de la zona observada.

Autor: ChatGPT · 27-may-2025
"""

import math
import time
from typing import List, Tuple, Optional

import cv2                       # sudo apt install ros-humble-vision-opencv
import numpy as np
import rclpy
from rclpy.node   import Node
from nav_msgs.msg import OccupancyGrid, MapMetaData
from geometry_msgs.msg import PoseArray, Pose
from std_msgs.msg import Header


# ---------------------------------------------------------------------------
def euclidean(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
    """Distancia Euclídea 2-D."""
    return math.hypot(p2[0] - p1[0], p2[1] - p1[1])


# ---------------------------------------------------------------------------
class DynamicOccupancyGridFuser(Node):
    def __init__(self) -> None:
        super().__init__("dynamic_occupancy_grid_fuser")

        # ---------------- parámetros ROS ----------------
        self.declare_parameter("resolution",          1.0)    # m/px
        self.declare_parameter("margin_cells",       10)      # celdas extra
        self.declare_parameter("sensor_range",       40.0)    # m
        self.declare_parameter("unknown_ring_width",  1)      # celdas
        self.declare_parameter("robot_pose_topic",   "/robot_pose_topic")

        self.resolution      = self.get_parameter("resolution")         .value
        self.margin          = self.get_parameter("margin_cells")       .value
        self.sensor_range    = self.get_parameter("sensor_range")       .value
        self.unk_ring_width  = self.get_parameter("unknown_ring_width") .value
        robot_pose_topic     = self.get_parameter("robot_pose_topic")   .value

        # ---------------- mapa dinámico -----------------
        self.origin_x = -25.0
        self.origin_y = -25.0
        self.width  = 100
        self.height = 100

        # ---------------- buffers -----------------------
        self.free_msgs:  List[PoseArray]      = []
        self.obst_msg:   Optional[PoseArray]  = None
        self.robot_pose: Optional[Pose]       = None

        # ---------------- suscripciones -----------------
        qos = 10
        self.create_subscription(PoseArray, "/filtered_navigation_nodes",
                                 self.free_cb, qos)
        self.create_subscription(PoseArray, "/terrain_slope_pose",
                                 self.free_cb, qos)
        self.create_subscription(PoseArray, "/obstacle_navigation_nodes",
                                 self.obst_cb, qos)
        self.create_subscription(PoseArray, robot_pose_topic,
                                 self.robot_pose_cb, qos)

        # ---------------- publicador --------------------
        self.grid_pub = self.create_publisher(OccupancyGrid,
                                              "/occupancy_grid", 10)

        # ---------------- timer (10 Hz) -----------------
        self.create_timer(0.1, self.publish_grid)

        self.get_logger().info("DynamicOccupancyGridFuser listo.")

    # ======================================================================
    #  Callbacks
    # ======================================================================
    def free_cb(self, msg: PoseArray) -> None:
        self.free_msgs.append(msg)

    def obst_cb(self, msg: PoseArray) -> None:
        self.obst_msg = msg

    def robot_pose_cb(self, msg: PoseArray) -> None:
        if msg.poses:
            self.robot_pose = msg.poses[0]

    # ======================================================================
    #  Utilidades
    # ======================================================================
    def world_to_map(self, x: float, y: float) -> Tuple[int, int]:
        col = int((x - self.origin_x) / self.resolution)
        row = int((y - self.origin_y) / self.resolution)
        return col, row

    def maybe_expand_map(self) -> None:
        xs, ys = [], []
        for m in self.free_msgs:
            xs.extend([p.position.x for p in m.poses])
            ys.extend([p.position.y for p in m.poses])
        if self.obst_msg:
            xs.extend([p.position.x for p in self.obst_msg.poses])
            ys.extend([p.position.y for p in self.obst_msg.poses])
        if not xs:
            return

        margin_m   = self.margin * self.resolution
        new_ox     = min(self.origin_x, min(xs) - margin_m)
        new_oy     = min(self.origin_y, min(ys) - margin_m)
        new_width  = int(math.ceil((max(xs)-new_ox)/self.resolution)) + self.margin
        new_height = int(math.ceil((max(ys)-new_oy)/self.resolution)) + self.margin

        if (new_width  != self.width  or new_height != self.height or
            new_ox != self.origin_x  or new_oy   != self.origin_y):
            self.get_logger().info(
                f"Mapa expandido → origen=({new_ox:.1f},{new_oy:.1f}) "
                f"tamaño=({new_width}×{new_height})")
            self.origin_x, self.origin_y = new_ox, new_oy
            self.width,   self.height    = new_width, new_height

    # ======================================================================
    #  Publicación del OccupancyGrid
    # ======================================================================
    def publish_grid(self) -> None:
        if not self.free_msgs and self.obst_msg is None:
            return

        self.maybe_expand_map()

        free_mask = np.zeros((self.height, self.width), dtype=np.uint8)
        obst_mask = np.zeros_like(free_mask)

        # ----------- puntos libres --------------------------------------
        for m in self.free_msgs:
            for p in m.poses:
                c,r = self.world_to_map(p.position.x, p.position.y)
                if 0 <= c < self.width and 0 <= r < self.height:
                    free_mask[r, c] = 255

        if np.any(free_mask):
            contours, _ = cv2.findContours(free_mask, cv2.RETR_EXTERNAL,
                                           cv2.CHAIN_APPROX_SIMPLE)
            cv2.fillPoly(free_mask, contours, 255)

            # ---- banda UNKNOWN de n celdas -----------------------------
            k = 1 + 2*self.unk_ring_width
            eroded = cv2.erode(free_mask,
                               np.ones((k, k), np.uint8), iterations=1)
            border_mask = cv2.bitwise_xor(free_mask, eroded)
            free_mask   = eroded          # interior definitivo

        # ----------- puntos obstáculo -----------------------------------
        if self.obst_msg:
            for p in self.obst_msg.poses:
                if self.robot_pose and euclidean(
                        (p.position.x, p.position.y),
                        (self.robot_pose.position.x,
                         self.robot_pose.position.y)) > self.sensor_range:
                    continue
                c,r = self.world_to_map(p.position.x, p.position.y)
                if 0 <= c < self.width and 0 <= r < self.height:
                    obst_mask[r, c] = 255
        if np.any(obst_mask):
            obst_mask = cv2.dilate(obst_mask, np.ones((1,1),np.uint8),
                                   iterations=1)

        # ----------- fusionar -------------------------------------------
        grid = -1 * np.ones_like(free_mask, dtype=np.int8)
        grid[free_mask == 255] = 0
        grid[obst_mask == 255] = 100
        # border_mask ya es -1 (UNKNOWN) por inicialización

        # ----------- publicar ------------------------------------------
        og = OccupancyGrid()
        og.header             = Header()
        og.header.stamp       = self.get_clock().now().to_msg()
        og.header.frame_id    = "map"

        info = MapMetaData()
        info.resolution       = self.resolution
        info.width, info.height = self.width, self.height
        info.origin.position.x = self.origin_x
        info.origin.position.y = self.origin_y
        info.origin.orientation.w = 1.0
        og.info = info
        og.data = grid.flatten().tolist()

        self.grid_pub.publish(og)
        self.get_logger().debug(f"OccupancyGrid publicado "
                                f"({self.width}×{self.height}) "
                                f"a las {time.strftime('%H:%M:%S')}")

        # (opcional) vaciar mensajes viejos
        # self.free_msgs.clear()


# ===========================================================================
def main(args=None) -> None:
    rclpy.init(args=args)
    node = DynamicOccupancyGridFuser()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
