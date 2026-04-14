# #!/usr/bin/env python3
# import math
# from typing import List, Optional

# import rclpy
# from rclpy.node import Node
# from rclpy.time import Time
# from rclpy.duration import Duration

# from sensor_msgs.msg import PointCloud2
# from builtin_interfaces.msg import Time as TimeMsg
# from sensor_msgs_py import point_cloud2 as pc2

# import numpy as np
# from geometry_msgs.msg import TransformStamped
# from tf2_ros import Buffer, TransformListener


# def time_to_rclpy_with_node_clock(node, t_msg):
#     # fuerza el mismo clock_type que el del nodo
#     return Time(seconds=int(t_msg.sec),
#                 nanoseconds=int(t_msg.nanosec),
#                 clock_type=node.get_clock().clock_type)


# def transform_to_matrix(T: TransformStamped) -> np.ndarray:
#     tx = T.transform.translation.x
#     ty = T.transform.translation.y
#     tz = T.transform.translation.z
#     qx = T.transform.rotation.x
#     qy = T.transform.rotation.y
#     qz = T.transform.rotation.z
#     qw = T.transform.rotation.w

#     # quat -> rot matrix
#     # (sin librerías externas)
#     xx, yy, zz = qx*qx, qy*qy, qz*qz
#     xy, xz, yz = qx*qy, qx*qz, qy*qz
#     wx, wy, wz = qw*qx, qw*qy, qw*qz

#     R = np.array([
#         [1.0 - 2.0*(yy+zz), 2.0*(xy - wz),       2.0*(xz + wy)],
#         [2.0*(xy + wz),     1.0 - 2.0*(xx+zz),   2.0*(yz - wx)],
#         [2.0*(xz - wy),     2.0*(yz + wx),       1.0 - 2.0*(xx+yy)]
#     ], dtype=np.float32)
#     M = np.eye(4, dtype=np.float32)
#     M[:3,:3] = R
#     M[:3, 3] = np.array([tx, ty, tz], dtype=np.float32)
#     return M


# class CloudMerger(Node):
#     """
#     Fusiona N nubes en un único PointCloud2:
#       - Transforma cada nube al frame objetivo (tf2).
#       - Opcionalmente aplica voxel grid.
#       - Publica /merged_points (XYZ32).
#     """

#     def __init__(self):
#         super().__init__('cloud_merger')

#         # Parámetros
#         self.declare_parameter('input_topics', ['/lidar_scan', '/camera/points'])
#         self.declare_parameter('target_frame', 'map')
#         self.declare_parameter('output_topic', '/merged_points')
#         self.declare_parameter('output_rate', 5.0)            # Hz
#         self.declare_parameter('tf_timeout', 0.2)             # s
#         self.declare_parameter('voxel_leaf_size', 0.0)        # m; 0 = sin downsample
#         self.declare_parameter('max_age', 0.5)                # s; descartamos nubes muy viejas
#         self.declare_parameter('strip_fields', True)          # deja solo XYZ

#         self.input_topics: List[str] = self.get_parameter('input_topics').get_parameter_value().string_array_value
#         self.target_frame: str = self.get_parameter('target_frame').get_parameter_value().string_value
#         self.output_topic: str = self.get_parameter('output_topic').get_parameter_value().string_value
#         self.output_rate: float = self.get_parameter('output_rate').get_parameter_value().double_value
#         self.tf_timeout: float = self.get_parameter('tf_timeout').get_parameter_value().double_value
#         self.voxel_leaf: float = self.get_parameter('voxel_leaf_size').get_parameter_value().double_value
#         self.max_age: float = self.get_parameter('max_age').get_parameter_value().double_value
#         self.strip_fields: bool = self.get_parameter('strip_fields').get_parameter_value().bool_value

#         if not self.input_topics:
#             self.get_logger().error("input_topics vacío.")
#             raise RuntimeError("No input topics")

#         # TF
#         self.tf_buffer = Buffer(cache_time=Duration(seconds=10))
#         self.tf_listener = TransformListener(self.tf_buffer, self, spin_thread=True)

#         # Suscriptores
#         self.latest_msgs = {topic: None for topic in self.input_topics}
#         for topic in self.input_topics:
#             self.create_subscription(PointCloud2, topic, self._make_cb(topic), 10)
#             self.get_logger().info(f"Subscribing to: {topic}")

#         # Publicador
#         self.pub = self.create_publisher(PointCloud2, self.output_topic, 10)
#         self.get_logger().info(f"Publishing merged cloud -> {self.output_topic} (frame={self.target_frame})")

#         # Timer
#         self.timer = self.create_timer(1.0 / max(0.1, self.output_rate), self.on_timer)

#     def _make_cb(self, topic):
#         def _cb(msg: PointCloud2):
#             self.latest_msgs[topic] = msg
#         return _cb

#     def on_timer(self):
#         now = self.get_clock().now()

#         # cache para avisos "una sola vez" sin depender del logger
#         if not hasattr(self, "_warned_once"):
#             self._warned_once = set()

#         # 1) Recolecta nubes recientes (mismo clock)
#         clouds = []
#         for topic, msg in self.latest_msgs.items():
#             if msg is None:
#                 continue
#             try:
#                 msg_t = time_to_rclpy_with_node_clock(self, msg.header.stamp)
#                 age = (now - msg_t).nanoseconds * 1e-9
#             except TypeError:
#                 age = (now.nanoseconds - (int(msg.header.stamp.sec) * 10**9 + int(msg.header.stamp.nanosec))) * 1e-9

#             if age <= self.max_age:
#                 clouds.append(msg)
#             else:
#                 key = f"old_msg_{topic}"
#                 if key not in self._warned_once:
#                     self.get_logger().warning(f"Nube vieja descartada de {topic} (age={age:.2f}s)")
#                     self._warned_once.add(key)

#         if not clouds:
#             return

#         # 2) TF + fusión
#         merged_xyz = []
#         for msg in clouds:
#             xyz = self.extract_xyz_in_target(msg)  # usa la versión con fallback a 'latest'
#             if xyz is not None and xyz.shape[0] > 0:
#                 merged_xyz.append(xyz)

#         if not merged_xyz:
#             return

#         points = np.concatenate(merged_xyz, axis=0)

#         # 3) Voxel opcional
#         if getattr(self, "voxel_leaf", 0.0) > 1e-6 and points.shape[0] > 0:
#             leaf = float(self.voxel_leaf)
#             q = np.floor(points / leaf)
#             _, idx = np.unique(q.view([('', q.dtype)] * q.shape[1]), return_index=True)
#             points = points[idx]

#         # 4) Publica PointCloud2 (XYZ32)
#         hdr = PointCloud2().header
#         hdr.stamp = now.to_msg()
#         hdr.frame_id = self.target_frame

#         out_msg = pc2.create_cloud_xyz32(hdr, points.tolist()) if getattr(self, "strip_fields", True) else pc2.create_cloud(
#             hdr,
#             [('x', 0, pc2.PointField.FLOAT32, 1),
#             ('y', 4, pc2.PointField.FLOAT32, 1),
#             ('z', 8, pc2.PointField.FLOAT32, 1)],
#             points.tolist()
#         )
#         self.pub.publish(out_msg)

#     def extract_xyz_in_target(self, msg):
#         """Lee XYZ del PointCloud2 y lo transforma al frame objetivo con tf2.
#         Si la TF al tiempo del mensaje no está, reintenta con 'latest'."""
#         src_frame = msg.header.frame_id

#         # Usa el mismo clock_type que el nodo para evitar conflictos
#         stamp = Time(
#             seconds=int(msg.header.stamp.sec),
#             nanoseconds=int(msg.header.stamp.nanosec),
#             clock_type=self.get_clock().clock_type
#         )

#         # 1) Lookup TF a tiempo del mensaje
#         try:
#             T = self.tf_buffer.lookup_transform(
#                 self.target_frame, src_frame, stamp,
#                 timeout=Duration(seconds=float(self.tf_timeout))
#             )
#         except Exception as e_first:
#             # 2) Fallback: usar el último transform disponible (Time() = latest)
#             try:
#                 T = self.tf_buffer.lookup_transform(
#                     self.target_frame, src_frame, Time()  # latest
#                 )
#                 self.get_logger().warning(
#                     f"TF {src_frame}->{self.target_frame} extrapolada (futuro). "
#                     f"Uso 'latest' en lugar del stamp del mensaje."
#                 )
#             except Exception as e_second:
#                 self.get_logger().warning(
#                     f"TF {src_frame}->{self.target_frame} no disponible: {e_second}"
#                 )
#                 return None

#         # Matriz 4x4
#         M = transform_to_matrix(T)  # ya la tienes en tu archivo

#         # 3) Leer XYZ de forma robusta
#         try:
#             arr = pc2.read_points_numpy(msg, field_names=['x', 'y', 'z'], skip_nans=True)
#             if arr.size == 0:
#                 return None
#             if arr.dtype.names is not None:                   # dtype estructurado
#                 arr = np.stack([arr['x'], arr['y'], arr['z']], axis=-1)
#             arr = arr.astype(np.float32, copy=False)
#         except Exception:
#             pts = list(pc2.read_points(msg, field_names=('x', 'y', 'z'), skip_nans=True))
#             if not pts:
#                 return None
#             arr = np.asarray(pts, dtype=np.float32)           # (N,3)

#         # 4) Transformar a target_frame
#         ones = np.ones((arr.shape[0], 1), dtype=np.float32)
#         homo = np.concatenate([arr, ones], axis=1)            # (N,4)
#         out  = (homo @ M.T)[:, :3]                            # (N,3)

#         return out


# def main():
#     rclpy.init()
#     node = CloudMerger()
#     try:
#         rclpy.spin(node)
#     finally:
#         node.destroy_node()
#         rclpy.shutdown()


# if __name__ == '__main__':
#     main()
#!/usr/bin/env python3
import numpy as np
import rclpy
from rclpy.node import Node
from rclpy.time import Time
from rclpy.duration import Duration

from sensor_msgs.msg import PointCloud2
from sensor_msgs_py import point_cloud2 as pc2
from geometry_msgs.msg import TransformStamped
from tf2_ros import Buffer, TransformListener
from message_filters import Subscriber, ApproximateTimeSynchronizer

# --- helpers ---
def transform_to_matrix(T: TransformStamped) -> np.ndarray:
    t = T.transform.translation
    q = T.transform.rotation
    # quaternion -> rot
    x, y, z, w = q.x, q.y, q.z, q.w
    R = np.array([
        [1-2*(y*y+z*z),   2*(x*y - z*w),   2*(x*z + y*w)],
        [  2*(x*y + z*w), 1-2*(x*x+z*z),   2*(y*z - x*w)],
        [  2*(x*z - y*w),   2*(y*z + x*w), 1-2*(x*x+y*y)]
    ], dtype=np.float32)
    M = np.eye(4, dtype=np.float32)
    M[:3,:3] = R
    M[0,3] = t.x; M[1,3] = t.y; M[2,3] = t.z
    return M

def read_xyz_numpy(msg: PointCloud2) -> np.ndarray:
    try:
        arr = pc2.read_points_numpy(msg, field_names=['x','y','z'], skip_nans=True)
        if arr.size == 0:
            return np.empty((0,3), np.float32)
        # Si viene dtype estructurado ('x','y','z'), desestructura:
        if arr.dtype.names is not None:
            arr = np.stack([arr['x'], arr['y'], arr['z']], axis=-1)
        return arr.astype(np.float32, copy=False)
    except Exception:
        pts = list(pc2.read_points(msg, field_names=('x','y','z'), skip_nans=True))
        if not pts:
            return np.empty((0,3), np.float32)
        return np.asarray(pts, dtype=np.float32)

class CloudMergerSync(Node):
    def __init__(self):
        super().__init__('cloud_merger_sync')
        # Params
        self.declare_parameter('lidar_topic', '/lidar_scan')      # AJUSTA a tu tópico
        self.declare_parameter('camera_topic','/camera/points')     # AJUSTA a tu tópico
        self.declare_parameter('target_frame','map')
        self.declare_parameter('slop', 0.15)        # segundos para sincronía aproximada
        self.declare_parameter('voxel_leaf', 0.0)   # 0 = sin voxel
        self.declare_parameter('strip_fields', True)

        self.lidar_topic  = self.get_parameter('lidar_topic').get_parameter_value().string_value
        self.camera_topic = self.get_parameter('camera_topic').get_parameter_value().string_value
        self.target_frame = self.get_parameter('target_frame').get_parameter_value().string_value
        self.slop         = float(self.get_parameter('slop').value)
        self.voxel_leaf   = float(self.get_parameter('voxel_leaf').value)
        self.strip_fields = bool(self.get_parameter('strip_fields').value)

        # TF
        self.tf_buffer = Buffer(cache_time=Duration(seconds=10.0))
        self.tf_listener = TransformListener(self.tf_buffer, self, spin_thread=True)

        # Subs sincronizados
        self.sub_lidar  = Subscriber(self, PointCloud2, self.lidar_topic)
        self.sub_camera = Subscriber(self, PointCloud2, self.camera_topic)
        self.ats = ApproximateTimeSynchronizer(
            [self.sub_lidar, self.sub_camera],
            queue_size=10,
            slop=self.slop,
            allow_headerless=False
        )
        self.ats.registerCallback(self.sync_cb)

        # Pub
        self.pub = self.create_publisher(PointCloud2, '/merged_points', 10)

        self.get_logger().info(f"Sync fusion -> {self.lidar_topic} + {self.camera_topic} => /merged_points (frame={self.target_frame}, slop={self.slop}s)")

    def transform_xyz_to_target(self, xyz: np.ndarray, src_frame: str) -> np.ndarray:
        if xyz.shape[0] == 0:
            return xyz
        # Usamos ALWAYS 'latest' para evitar extrapolaciones al futuro
        try:
            T = self.tf_buffer.lookup_transform(self.target_frame, src_frame, Time())  # latest
        except Exception as e:
            self.get_logger().warning(f"TF {src_frame}->{self.target_frame} no disponible: {e}")
            return np.empty((0,3), np.float32)

        M = transform_to_matrix(T)   # 4x4
        homo = np.concatenate([xyz, np.ones((xyz.shape[0],1), np.float32)], axis=1)  # (N,4)
        out  = (homo @ M.T)[:, :3]
        return out

    def sync_cb(self, lidar_msg: PointCloud2, cam_msg: PointCloud2):
        # 1) Leer XYZ
        lidar_xyz = read_xyz_numpy(lidar_msg)
        cam_xyz   = read_xyz_numpy(cam_msg)

        # 2) TF -> target_frame (usando latest)
        lidar_xyz = self.transform_xyz_to_target(lidar_xyz, lidar_msg.header.frame_id)
        cam_xyz   = self.transform_xyz_to_target(cam_xyz,   cam_msg.header.frame_id)

        # 3) Fusionar
        pts = np.concatenate([lidar_xyz, cam_xyz], axis=0) if lidar_xyz.size or cam_xyz.size else np.empty((0,3), np.float32)
        if pts.shape[0] == 0:
            return

        # 4) Voxel opcional
        if self.voxel_leaf > 1e-6:
            leaf = float(self.voxel_leaf)
            q = np.floor(pts / leaf).astype(np.int32, copy=False)
            # unique por celda
            key = q.view([('', q.dtype)] * q.shape[1])
            _, idx = np.unique(key, return_index=True)
            pts = pts[idx]

        # 5) Publicar una sola nube (stamp=now)
        hdr = PointCloud2().header
        hdr.stamp = self.get_clock().now().to_msg()
        hdr.frame_id = self.target_frame

        if self.strip_fields:
            out_msg = pc2.create_cloud_xyz32(hdr, pts.tolist())
        else:
            out_msg = pc2.create_cloud(
                hdr,
                [('x', 0, pc2.PointField.FLOAT32, 1),
                 ('y', 4, pc2.PointField.FLOAT32, 1),
                 ('z', 8, pc2.PointField.FLOAT32, 1)],
                pts.tolist()
            )
        self.pub.publish(out_msg)

def main():
    rclpy.init()
    node = CloudMergerSync()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
