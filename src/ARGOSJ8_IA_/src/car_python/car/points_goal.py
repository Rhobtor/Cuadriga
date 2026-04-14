
# ###################################################################
# #!/usr/bin/env python3
# import rclpy, math, random
# from collections import deque
# from rclpy.node import Node
# from geometry_msgs.msg import PoseArray, Pose
# from std_msgs.msg import Bool
# from visualization_msgs.msg import Marker
# from nav_msgs.msg import OccupancyGrid

# # ---------- Parámetros ajustables ----------
# OBSTACLE_COST_TH = 50          # ≥50 ⇒ ocupado
# UNKNOWN_COST_TH  = -1          # -1 ⇒ desconocido (trátalo como obst.)
# CLEARANCE_M      = 0.30        # [m] distancia mínima a obstáculo
# MAX_SEARCH_M     = 2.0         # [m] radio máx. para “rescatar” el goal

# class FrontierSelector(Node):
#     def __init__(self):
#         super().__init__('frontier_selector')     # ← ¡IMPORTANTE!

#         # --- Suscripciones ---
#         self.frontier_sub = self.create_subscription(
#             PoseArray, '/safe_frontier_points', self.frontier_cb, 10)
#         self.map_sub = self.create_subscription(
#             OccupancyGrid, '/occupancy_grid', self.map_cb, 10)
#         self.goal_reached_sub = self.create_subscription(
#             Bool, 'goal_reached', self.goal_reached_cb, 10)

#         # --- Publicaciones ---
#         self.goal_pub   = self.create_publisher(PoseArray, 'goal', 10)
#         self.marker_pub = self.create_publisher(Marker, 'goal_marker', 10)

#         # --- Timer para re-publicar el goal activo ---
#         self.timer = self.create_timer(0.5, self.timer_cb)

#         # --- Estado interno ---
#         self.frontiers: list[Pose] = []
#         self.current_goal: Pose | None = None
#         self.goal_active  = False
#         self.grid_data    = None        # OccupancyGrid.data (lista de int8)
#         self.grid_info    = None        # OccupancyGrid.info

#     # ============ Callbacks ============

#     def frontier_cb(self, msg: PoseArray):
#         self.frontiers = msg.poses
#         if self.frontiers and not self.goal_active:
#             self.select_and_publish_goal()

#     def map_cb(self, msg: OccupancyGrid):
#         self.grid_data = msg.data
#         self.grid_info = msg.info
#         # Valida el goal actual cada vez que llega un mapa
#         if self.goal_active and self.current_goal:
#             if self.is_goal_in_collision(self.current_goal):
#                 self.get_logger().info(
#                     'El goal actual ocupa o roza un obstáculo; buscándolo libre…')
#                 new_pose = self.closest_free_pose(self.current_goal)
#                 if new_pose:
#                     self.current_goal = new_pose
#                     self.publish_goal(new_pose)

#     def goal_reached_cb(self, msg: Bool):
#         if msg.data:
#             self.goal_active = False
#             self.select_and_publish_goal()

#     def timer_cb(self):
#         if self.goal_active and self.current_goal:
#             self.publish_goal(self.current_goal)

#     # ============ Conversión mundo <-> grid ============

#     def world_to_grid(self, pose: Pose):
#         if not self.grid_info:
#             return None
#         res = self.grid_info.resolution
#         gx = int((pose.position.x - self.grid_info.origin.position.x) / res)
#         gy = int((pose.position.y - self.grid_info.origin.position.y) / res)
#         if 0 <= gx < self.grid_info.width and 0 <= gy < self.grid_info.height:
#             return gx, gy
#         return None

#     def grid_to_world_pose(self, gx: int, gy: int, ref_orientation):
#         res = self.grid_info.resolution
#         p = Pose()
#         p.position.x = self.grid_info.origin.position.x + (gx + 0.5) * res
#         p.position.y = self.grid_info.origin.position.y + (gy + 0.5) * res
#         p.orientation = ref_orientation
#         return p

#     # ============ Occupancy & Clearance ============

#     def is_cell_occupied(self, gx: int, gy: int) -> bool:
#         idx = gy * self.grid_info.width + gx
#         if idx >= len(self.grid_data):
#             return True             # fuera de rango ⇒ trátalo ocupado
#         cost = self.grid_data[idx]
#         return cost >= OBSTACLE_COST_TH or cost == UNKNOWN_COST_TH

#     def has_clearance(self, cx: int, cy: int, rad_cells: int) -> bool:
#         res = self.grid_info.resolution
#         for dx in range(-rad_cells, rad_cells + 1):
#             for dy in range(-rad_cells, rad_cells + 1):
#                 if math.hypot(dx, dy) * res >= CLEARANCE_M:
#                     continue
#                 nx, ny = cx + dx, cy + dy
#                 if (0 <= nx < self.grid_info.width and
#                     0 <= ny < self.grid_info.height and
#                     self.is_cell_occupied(nx, ny)):
#                     return False
#         return True

#     def is_goal_in_collision(self, pose: Pose) -> bool:
#         cell = self.world_to_grid(pose)
#         return cell is not None and self.is_cell_occupied(*cell)

#     # ============ Búsqueda BFS de la celda libre más cercana ============

#     def closest_free_pose(self, origin_pose: Pose):
#         start = self.world_to_grid(origin_pose)
#         if start is None:
#             return None
#         res = self.grid_info.resolution
#         max_cells = int(MAX_SEARCH_M / res)
#         clearance_cells = int(CLEARANCE_M / res)

#         q = deque([(start[0], start[1], 0)])
#         visited = {start}
#         neigh = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]

#         while q:
#             x, y, d = q.popleft()
#             if d > max_cells:
#                 break
#             if (not self.is_cell_occupied(x, y) and
#                 self.has_clearance(x, y, clearance_cells)):
#                 return self.grid_to_world_pose(x, y, origin_pose.orientation)
#             for dx, dy in neigh:
#                 nx, ny = x + dx, y + dy
#                 if (0 <= nx < self.grid_info.width and
#                     0 <= ny < self.grid_info.height and
#                     (nx, ny) not in visited):
#                     visited.add((nx, ny))
#                     q.append((nx, ny, d + 1))
#         return None

#     # ============ Gestión del objetivo ============

#     def select_and_publish_goal(self):
#         # Filtra fronteras que, con el mapa actual, estén libres
#         valid = [p for p in self.frontiers if not self.is_goal_in_collision(p)]
#         if not valid:
#             self.get_logger().warn('No hay fronteras libres ahora mismo.')
#             return
#         self.current_goal = random.choice(valid)
#         # Por si la celda acaba de marcarse ocupada entre ciclos…
#         if self.is_goal_in_collision(self.current_goal):
#             corr = self.closest_free_pose(self.current_goal)
#             if corr:
#                 self.current_goal = corr
#         self.goal_active = True
#         self.publish_goal(self.current_goal)

#     def publish_goal(self, pose: Pose):
#         # PoseArray para planners
#         arr = PoseArray()
#         arr.header.stamp = self.get_clock().now().to_msg()
#         arr.header.frame_id = 'map'
#         arr.poses.append(pose)
#         self.goal_pub.publish(arr)

#         # Marker RViz
#         m          = Marker()
#         m.header   = arr.header
#         m.ns, m.id = 'goal_marker', 0
#         m.type     = Marker.SPHERE
#         m.action   = Marker.ADD
#         m.pose     = pose
#         m.scale.x = m.scale.y = m.scale.z = 2.3
#         m.color.r, m.color.a = 1.0, 1.0
#         self.marker_pub.publish(m)

# # ----------------------- main -----------------------
# def main(args=None):
#     rclpy.init(args=args)
#     try:
#         rclpy.spin(FrontierSelector())
#     except KeyboardInterrupt:
#         pass
#     rclpy.shutdown()

# if __name__ == '__main__':
#     main()
###################################################################################
# #!/usr/bin/env python3
# """
# RandomGoalSelector
# ==================
# Nodo ROS 2 que publica metas de navegación aleatorias elegidas de una
# lista fija de puntos (x, y) definida como parámetro ROS.  Se asegura de
# que el punto elegido no caiga sobre un obstáculo en el OccupancyGrid y,
# si fuera necesario, busca la celda libre más cercana con distancia de
# seguridad.

# ► Parámetros (ejemplo YAML)
# --------------------------------
# random_goal_selector:
#   ros__parameters:
#     goal_points:           # lista de pares [x, y] en el frame `map`
#       - [  5.50,  -2.00 ]
#       - [ -8.30,  10.00 ]
#       - [  0.00,   0.00 ]
#     republish_hz: 2.0      # frecuencia a la que re‑publica la meta activa

# Publica:
#   * topic `goal`        (PoseArray) – un único Pose con la meta
#   * topic `goal_marker` (Marker)    – esfera roja para RViz

# Autor: ChatGPT – 2025‑05‑21
# """

# import math
# import random
# from collections import deque

# import rclpy
# from rclpy.node import Node
# from geometry_msgs.msg import PoseArray, Pose
# from std_msgs.msg import Bool
# from visualization_msgs.msg import Marker
# from nav_msgs.msg import OccupancyGrid
# from tf_transformations import quaternion_from_euler

# # ---------- Parámetros de colisión ----------
# OBSTACLE_COST_TH = 50        # ≥50 ⇒ ocupado
# UNKNOWN_COST_TH  = -1        # -1 ⇒ desconocido → trátalo como obstáculo
# CLEARANCE_M      = 0.30      # [m] radio libre alrededor del goal
# MAX_SEARCH_M     = 2.0       # [m] radio máx. para "rescatar" el goal

# class RandomGoalSelector(Node):
#     def __init__(self):
#         super().__init__('random_goal_selector')

#         # --- Parámetros de usuario ---
#         self.declare_parameter('goal_points', [0.0, 0.0])
#         self.declare_parameter('republish_hz', 2.0)  # Hz

#         raw_points = self.get_parameter('goal_points').get_parameter_value().double_array_value


#         # rclpy convierte listas de listas en un vector plano: [x1, y1, x2, y2, ...]
#         if len(raw_points) % 3 != 0:
#             raise ValueError('goal_points debe tener un número par de elementos (x1,y1,x2,y2,...)')
#         self.goal_list: list[Pose] = []
#         for i in range(0, len(raw_points), 3):
#             x, y, z = raw_points[i : i+3]
#             self.goal_list.append(self._make_pose(x, y, z, 0.0))

#         hz = self.get_parameter('republish_hz').value

#         # --- Suscripciones ---
#         self.map_sub = self.create_subscription(
#             OccupancyGrid, '/occupancy_grid', self.map_cb, 10)
#         self.goal_reached_sub = self.create_subscription(
#             Bool, 'goal_reached', self.goal_reached_cb, 10)

#         # --- Publicaciones ---
#         self.goal_pub   = self.create_publisher(PoseArray, 'goal', 10)
#         self.marker_pub = self.create_publisher(Marker, 'goal_marker', 10)

#         # --- Timer ---
#         self.timer = self.create_timer(1.0 / hz, self.timer_cb)

#         # --- Estado ---
#         self.current_goal: Pose | None = None
#         self.goal_active  = False
#         self.grid_data    = None  # OccupancyGrid.data
#         self.grid_info    = None  # OccupancyGrid.info

#         # Selecciona primera meta
#         self.select_and_publish_goal(initial=True)

#     # ============ Callbacks ============

#     def map_cb(self, msg: OccupancyGrid):
#         self.grid_data = msg.data
#         self.grid_info = msg.info
#         # Valida la meta actual cada vez que llega un mapa
#         if self.goal_active and self.current_goal:
#             if self.is_goal_in_collision(self.current_goal):
#                 self.get_logger().info('Goal actual en colisión; corrigiendo…')
#                 new_pose = self.closest_free_pose(self.current_goal)
#                 if new_pose:
#                     self.current_goal = new_pose
#                     self.publish_goal(new_pose)

#     def goal_reached_cb(self, msg: Bool):
#         if msg.data:
#             self.goal_active = False
#             self.select_and_publish_goal()

#     def timer_cb(self):
#         if self.goal_active and self.current_goal:
#             self.publish_goal(self.current_goal)

#     # ============ Conversión mundo <-> grid ============

#     def world_to_grid(self, pose: Pose):
#         if not self.grid_info:
#             return None
#         res = self.grid_info.resolution
#         gx = int((pose.position.x - self.grid_info.origin.position.x) / res)
#         gy = int((pose.position.y - self.grid_info.origin.position.y) / res)
#         if 0 <= gx < self.grid_info.width and 0 <= gy < self.grid_info.height:
#             return gx, gy
#         return None

#     def grid_to_world_pose(self, gx: int, gy: int, ref_orientation):
#         res = self.grid_info.resolution
#         p = Pose()
#         p.position.x = self.grid_info.origin.position.x + (gx + 0.5) * res
#         p.position.y = self.grid_info.origin.position.y + (gy + 0.5) * res
#         p.orientation = ref_orientation
#         return p

#     # ============ Occupancy & Clearance ============

#     def is_cell_occupied(self, gx: int, gy: int) -> bool:
#         idx = gy * self.grid_info.width + gx
#         if idx >= len(self.grid_data):
#             return True  # fuera de rango ⇒ trátalo ocupado
#         cost = self.grid_data[idx]
#         return cost >= OBSTACLE_COST_TH or cost == UNKNOWN_COST_TH

#     def has_clearance(self, cx: int, cy: int, rad_cells: int) -> bool:
#         res = self.grid_info.resolution
#         for dx in range(-rad_cells, rad_cells + 1):
#             for dy in range(-rad_cells, rad_cells + 1):
#                 if math.hypot(dx, dy) * res >= CLEARANCE_M:
#                     continue
#                 nx, ny = cx + dx, cy + dy
#                 if (0 <= nx < self.grid_info.width and
#                     0 <= ny < self.grid_info.height and
#                     self.is_cell_occupied(nx, ny)):
#                     return False
#         return True

#     def is_goal_in_collision(self, pose: Pose) -> bool:
#         cell = self.world_to_grid(pose)
#         return cell is not None and self.is_cell_occupied(*cell)

#     # ============ Búsqueda BFS de la celda libre más cercana ============

#     def closest_free_pose(self, origin_pose: Pose):
#         start = self.world_to_grid(origin_pose)
#         if start is None:
#             return None
#         res = self.grid_info.resolution
#         max_cells = int(MAX_SEARCH_M / res)
#         clearance_cells = int(CLEARANCE_M / res)

#         q = deque([(start[0], start[1], 0)])
#         visited = {start}
#         neigh = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]

#         while q:
#             x, y, d = q.popleft()
#             if d > max_cells:
#                 break
#             if (not self.is_cell_occupied(x, y) and
#                 self.has_clearance(x, y, clearance_cells)):
#                 return self.grid_to_world_pose(x, y, origin_pose.orientation)
#             for dx, dy in neigh:
#                 nx, ny = x + dx, y + dy
#                 if (0 <= nx < self.grid_info.width and
#                     0 <= ny < self.grid_info.height and
#                     (nx, ny) not in visited):
#                     visited.add((nx, ny))
#                     q.append((nx, ny, d + 1))
#         return None

#     # ============ Gestión de la meta ============

#     def select_and_publish_goal(self, initial=False):
#         if not self.goal_list:
#             self.get_logger().error('goal_points está vacío.')
#             return
#         # Filtra puntos que estén libres en el mapa actual (si lo hay)
#         candidate_list = self.goal_list
#         if self.grid_info:
#             candidate_list = [p for p in self.goal_list if not self.is_goal_in_collision(p)]
#             if not candidate_list:
#                 self.get_logger().warn('Ninguno de los goal_points está libre ahora mismo.')
#                 return
#         self.current_goal = random.choice(candidate_list)
#         if self.grid_info and self.is_goal_in_collision(self.current_goal):
#             corr = self.closest_free_pose(self.current_goal)
#             if corr:
#                 self.current_goal = corr
#         self.goal_active = True
#         self.publish_goal(self.current_goal)
#         if not initial:
#             self.get_logger().info('➡️  Nueva meta publicada.')

#     def publish_goal(self, pose: Pose):
#         # PoseArray (Nav2 BT recibiría sólo uno, pero mantenemos formato genérico)
#         arr = PoseArray()
#         arr.header.stamp = self.get_clock().now().to_msg()
#         arr.header.frame_id = 'map'
#         arr.poses.append(pose)
#         self.goal_pub.publish(arr)

#         # Marker RViz
#         m          = Marker()
#         m.header   = arr.header
#         m.ns, m.id = 'goal_marker', 0
#         m.type     = Marker.SPHERE
#         m.action   = Marker.ADD
#         m.pose     = pose
#         m.scale.x = m.scale.y = m.scale.z = 2.0
#         m.color.r, m.color.a = 1.0, 1.0
#         self.marker_pub.publish(m)

#     # ============ Utilidades ============

#     @staticmethod
#     def _make_pose(x: float, y: float, z:float, yaw_deg: float) -> Pose:
#         p = Pose()
#         p.position.x = x
#         p.position.y = y
#         p.position.z = z
#         qx, qy, qz, qw = quaternion_from_euler(0.0, 0.0, math.radians(yaw_deg))
#         p.orientation.x = qx
#         p.orientation.y = qy
#         p.orientation.z = qz
#         p.orientation.w = qw
#         return p

# # ------------------------- main -------------------------

# def main(args=None):
#     rclpy.init(args=args)
#     try:
#         rclpy.spin(RandomGoalSelector())
#     except KeyboardInterrupt:
#         pass
#     rclpy.shutdown()

# if __name__ == '__main__':
#     main()


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
RandomGoalSelector
==================
Nodo ROS 2 que publica metas de navegación aleatorias elegidas de una
lista fija de puntos (x, y, z) definida como parámetro ROS.  Se asegura de
que el punto elegido no caiga sobre un obstáculo en el OccupancyGrid y,
si fuera necesario, busca la celda libre más cercana manteniendo el z
original.

Parámetros YAML (ejemplo):
--------------------------------
random_goal_selector:
  ros__parameters:
    goal_points:           # lista de triples [x, y, z] en el frame `map`
      -4.0  -22.0  4.0
      -7.0   15.0  2.7
      11.0  -22.0  1.62
      13.0   16.0  3.13
      32.0  -23.0  6.2
      32.0   17.0  0.0
      4.0   -26.0  8.35

    republish_hz: 2.0      # frecuencia a la que re-publica la meta activa

Publica:
  * topic `goal`        (PoseArray) – un único Pose con la meta
  * topic `goal_marker` (Marker)    – esfera roja para RViz

Autor: ChatGPT — 2025-06
"""

import math
import random
from collections import deque

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseArray, Pose
from std_msgs.msg import Bool
from visualization_msgs.msg import Marker
from nav_msgs.msg import OccupancyGrid
from tf_transformations import quaternion_from_euler

# ---------- Parámetros de colisión y búsqueda ----------
OBSTACLE_COST_TH = 50        # ≥50 ⇒ ocupado
UNKNOWN_COST_TH  = -1        # -1 ⇒ desconocido → trátalo como obstáculo
CLEARANCE_M      = 0.30      # [m] radio libre alrededor del goal
MAX_SEARCH_M     = 2.0       # [m] radio máx. para "rescatar" el goal

class RandomGoalSelector(Node):
    def __init__(self):
        super().__init__('random_goal_selector')

        # --- 1) Declaración de parámetros ---
        # Esperamos *exactamente* triples (x,y,z) planos en un double_array.
        self.declare_parameter('goal_points', [0.0, 0.0, 0.0])
        self.declare_parameter('republish_hz', 2.0)

        raw_points = (
            self.get_parameter('goal_points').get_parameter_value()
                                    .double_array_value
        )

        # Validación: longitud debe ser múltiplo de 3
        if len(raw_points) % 3 != 0:
            raise ValueError(
                f"El parámetro 'goal_points' debe tener número de elementos múltiplo de 3, "
                f"pero recibí {len(raw_points)}"
            )

        # Construimos la lista de Pose, agrupando de 3 en 3:
        self.goal_list = []
        for i in range(0, len(raw_points), 3):
            x, y, z = raw_points[i : i + 3]
            pose = Pose()
            pose.position.x = float(x)
            pose.position.y = float(y)
            pose.position.z = float(z)
            # Por defecto orientamos yaw = 0 (puedes cambiarlo si quieres otro ángulo)
            qx, qy, qz, qw = quaternion_from_euler(0.0, 0.0, 0.0)
            pose.orientation.x = qx
            pose.orientation.y = qy
            pose.orientation.z = qz
            pose.orientation.w = qw
            self.goal_list.append(pose)

        self.get_logger().info(f"✔️  Cargados {len(self.goal_list)} goal_points válidos.")

        self.republish_hz = self.get_parameter('republish_hz').value

        # --- 2) Suscripciones ---
        # Suscribirse al mapa para detectar colisiones / correcciones de meta
        self.map_sub = self.create_subscription(
            OccupancyGrid, '/occupancy_grid', self.map_cb, 10
        )
        # Si el robot alcanza la meta (otro nodo publica Bool en 'goal_reached')
        self.goal_reached_sub = self.create_subscription(
            Bool, 'goal_reached', self.goal_reached_cb, 10
        )

        # --- 3) Publicaciones ---
        self.goal_pub   = self.create_publisher(PoseArray, 'goal', 10)
        self.marker_pub = self.create_publisher(Marker, 'goal_marker', 10)

        # --- 4) Timer para re-publicar la meta vigente ---
        self.timer = self.create_timer(1.0 / self.republish_hz, self.timer_cb)

        # --- 5) Estado interno ---
        self.current_goal = None    # Pose actual
        self.goal_active  = False   # Si hay meta activa
        self.grid_data    = None    # Datos de OccupancyGrid.data (flat array)
        self.grid_info    = None    # OccupancyGrid.info (width, height, origin, resolution)

        # Publicar primera meta al arrancar
        self.select_and_publish_goal(initial=True)

    # ==================== Callback del OccupancyGrid ====================
    def map_cb(self, msg: OccupancyGrid):
        """
        Cada vez que recibimos un mapa, guardamos la información y validamos
        si la meta actual sigue libre. Si está en colisión, buscamos la celda
        libre más cercana (manteniendo la misma orientación y z) y la corregimos.
        """
        self.grid_data = msg.data
        self.grid_info = msg.info

        if self.goal_active and self.current_goal:
            # Si nuestra meta actual cae en celda ocupada, la corregimos
            if self.is_goal_in_collision(self.current_goal):
                self.get_logger().info('⚠️  Meta actual en colisión; corrigiendo…')
                nueva = self.closest_free_pose(self.current_goal)
                if nueva:
                    self.current_goal = nueva
                    self.publish_goal(nueva)

    # ==================== Callback de “goal_reached” ====================
    def goal_reached_cb(self, msg: Bool):
        """
        Cuando otro nodo publica True en /goal_reached, marcamos la meta
        como inactiva y seleccionamos una nueva.
        """
        if msg.data:
            self.goal_active = False
            self.get_logger().info('🏁  Meta alcanzada; eligiendo otra...')
            self.select_and_publish_goal()

    # ==================== Timer para re-publicar ====================
    def timer_cb(self):
        """
        Si ya hay una meta activa, cada 1/republish_hz segundos volvemos
        a publicarla (PoseArray + Marker) para que no caduque en los consumidores.
        """
        if self.goal_active and self.current_goal:
            self.publish_goal(self.current_goal)

    # ============ Conversión mundo <-> grid (pixels) ============

    def world_to_grid(self, pose: Pose):
        """
        Convierte una Pose en coordenadas (x,y) a índices (gx,gy) de celda del OccupancyGrid.
        Si cae fuera del mapa, devuelve None.
        """
        if not self.grid_info:
            return None
        res = self.grid_info.resolution
        gx = int((pose.position.x - self.grid_info.origin.position.x) / res)
        gy = int((pose.position.y - self.grid_info.origin.position.y) / res)
        if 0 <= gx < self.grid_info.width and 0 <= gy < self.grid_info.height:
            return (gx, gy)
        return None

    def grid_to_world_pose(self, gx: int, gy: int, ref_pose: Pose):
        """
        Dado un par (gx,gy) de celda libre, reconstruye una Pose en 'map' con:
          - posición centrada en esa celda
          - mantiene el mismo orientation (cuaternion) que ref_pose
          - mantiene el mismo z que ref_pose
        """
        res = self.grid_info.resolution
        nueva = Pose()
        nueva.position.x = self.grid_info.origin.position.x + (gx + 0.5) * res
        nueva.position.y = self.grid_info.origin.position.y + (gy + 0.5) * res
        nueva.position.z = ref_pose.position.z
        nueva.orientation = ref_pose.orientation
        return nueva

    # ==================== Métodos de colisión y filtrado ====================

    def is_cell_occupied(self, gx: int, gy: int) -> bool:
        """
        Comprueba si la celda (gx,gy) del OccupancyGrid está ocupada o desconocida.
        Cualquier valor ≥ OBSTACLE_COST_TH o == UNKNOWN_COST_TH se considera “no libre”.
        """
        idx = gy * self.grid_info.width + gx
        if idx < 0 or idx >= len(self.grid_data):
            return True  # fuera de rango → tratamos como ocupado
        cost = self.grid_data[idx]
        return (cost >= OBSTACLE_COST_TH) or (cost == UNKNOWN_COST_TH)

    def has_clearance(self, cx: int, cy: int, rad_cells: int) -> bool:
        """
        Comprueba que alrededor del píxel (cx,cy) exista un radio de CLEARANCE_M,
        es decir, que ninguna celda dentro de ese radio esté ocupada.
        """
        res = self.grid_info.resolution
        for dx in range(-rad_cells, rad_cells + 1):
            for dy in range(-rad_cells, rad_cells + 1):
                # Saltamos si la distancia en metros excede CLEARANCE_M
                if math.hypot(dx, dy) * res >= CLEARANCE_M:
                    continue
                nx, ny = cx + dx, cy + dy
                if (0 <= nx < self.grid_info.width and
                    0 <= ny < self.grid_info.height and
                    self.is_cell_occupied(nx, ny)):
                    return False
        return True

    def is_goal_in_collision(self, pose: Pose) -> bool:
        """
        Indica True si la Pose dada cae en una celda ocupada.
        """
        celda = self.world_to_grid(pose)
        return (celda is not None) and self.is_cell_occupied(celda[0], celda[1])

    # ==================== Búsqueda de la celda libre más cercana ====================

    def closest_free_pose(self, origin_pose: Pose):
        """
        Realiza una búsqueda en anchura (BFS) a partir de la celda de origin_pose,
        expandiendo hasta dist = MAX_SEARCH_M, para encontrar la primera celda
        que esté libre y tenga clearance de CLEARANCE_M. Devuelve una Pose con
        el mismo z y la misma orientación que origin_pose.
        """
        inicio = self.world_to_grid(origin_pose)
        if inicio is None:
            return None

        res = self.grid_info.resolution
        max_cells = int(MAX_SEARCH_M / res)
        clearance_cells = int(CLEARANCE_M / res)

        q = deque([(inicio[0], inicio[1], 0)])  # (x, y, distancia_en_celdas)
        visited = { (inicio[0], inicio[1]) }
        # Movimientos 8-conectividad
        vecinos = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]

        while q:
            x, y, d = q.popleft()
            if d > max_cells:
                break

            if (not self.is_cell_occupied(x, y) and
                self.has_clearance(x, y, clearance_cells)):
                # Reconstruimos la Pose (mantenemos z y orientación de origin_pose)
                return self.grid_to_world_pose(x, y, origin_pose)

            for dx, dy in vecinos:
                nx, ny = x + dx, y + dy
                if (0 <= nx < self.grid_info.width and
                    0 <= ny < self.grid_info.height and
                    (nx, ny) not in visited):
                    visited.add((nx, ny))
                    q.append((nx, ny, d + 1))

        return None

    # ==================== Selección y publicación de la meta ====================

    def select_and_publish_goal(self, initial=False):
        """
        Elige aleatoriamente uno de los Pose de self.goal_list que aún no esté
        en colisión. Si hace falta, corrige la posición con closest_free_pose.
        Posteriormente lo publica en /goal y /goal_marker.
        """
        if not self.goal_list:
            self.get_logger().error('🚨  Lista de goal_points vacía.')
            return

        # Si aún no tenemos grid_info (mapa), elegimos sin filtrar — solo al inicio.
        if self.grid_info is None:
            candidato_list = self.goal_list.copy()
        else:
            # Filtramos metas que *no* estén ocupadas en el mapa actual
            candidato_list = [
                p for p in self.goal_list
                if not self.is_goal_in_collision(p)
            ]
            if not candidato_list:
                self.get_logger().warn(
                    '⚠️  Ninguno de los goal_points definidos está libre en este mapa.'
                )
                return

        # Elegimos uno al azar de la lista filtrada
        elegido = random.choice(candidato_list)

        # Si el mapa ya existe y ese elegido está en colisión (cabe caso en que la colisión llegara justo después
        # de filtrar), lo “rescataremos” con BFS:
        if self.grid_info and self.is_goal_in_collision(elegido):
            corregido = self.closest_free_pose(elegido)
            if corregido:
                elegido = corregido

        self.current_goal = elegido
        self.goal_active = True
        self.publish_goal(elegido)

        if not initial:
            self.get_logger().info(f'➡️  Nueva meta publicada: '
                                   f'x={elegido.position.x:.2f}, '
                                   f'y={elegido.position.y:.2f}, '
                                   f'z={elegido.position.z:.2f}')

    def publish_goal(self, pose: Pose):
        """
        Publica la Pose en /goal (PoseArray) y en /goal_marker (Marker).
        """
        # 1) PoseArray
        arr = PoseArray()
        arr.header.stamp = self.get_clock().now().to_msg()
        arr.header.frame_id = 'map'
        arr.poses.append(pose)
        self.goal_pub.publish(arr)

        # 2) Marker para RViz
        m = Marker()
        m.header   = arr.header
        m.ns       = 'goal_marker'
        m.id       = 0
        m.type     = Marker.SPHERE
        m.action   = Marker.ADD
        m.pose     = pose
        m.scale.x = m.scale.y = m.scale.z = 2.0
        m.color.r = 1.0
        m.color.g = 0.0
        m.color.b = 0.0
        m.color.a = 1.0
        self.marker_pub.publish(m)

    # ==================== Función principal ====================

def main(args=None):
    rclpy.init(args=args)
    node = RandomGoalSelector()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
