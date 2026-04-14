# # car/voxel_grid_node.py
# import rclpy
# from rclpy.node import Node
# from sensor_msgs.msg import PointCloud2, PointField
# from std_msgs.msg import Header
# import numpy as np
# import time

# class VoxelGridNode(Node):
#     def __init__(self):
#         super().__init__("voxel_grid")
#         self.declare_parameter("input", "/camera/points")
#         self.declare_parameter("output", "/merged_points_filtered")
#         self.declare_parameter("leaf_size", 0.15)       # 0.12–0.20 va bien
#         self.declare_parameter("frame_id", "base_link")       # salida en el mismo frame que entrada

#         in_topic  = self.get_parameter("input").get_parameter_value().string_value
#         out_topic = self.get_parameter("output").get_parameter_value().string_value
#         self.leaf = float(self.get_parameter("leaf_size").value)
#         self.frame_id = self.get_parameter("frame_id").get_parameter_value().string_value

#         self.pub = self.create_publisher(PointCloud2, out_topic, 5)
#         self.sub = self.create_subscription(PointCloud2, in_topic, self.cb, 5)
#         self.get_logger().info(f"VoxelGrid: {in_topic} -> {out_topic} (leaf={self.leaf} m)")

#     def cb(self, msg: PointCloud2):
#         # Asumimos x,y,z al inicio con offsets 0,4,8 y point_step=12 (nuestros publishers)
#         if msg.point_step != 12:
#             self.get_logger().warn("point_step != 12. Este voxel sólo soporta nubes x,y,z compactas.")
#             return
#         npts = int(len(msg.data) // 12)
#         if npts == 0:
#             return

#         # Vista numpy directa (sin copiar) sobre el payload
#         a = np.frombuffer(msg.data, dtype=np.float32, count=npts*3).reshape(-1,3)

#         # Filtra NaNs si los hubiera
#         good = np.isfinite(a).all(axis=1)
#         a = a[good]
#         if a.size == 0:
#             return

#         # Voxelización rápida por cuantización
#         # indices de voxel
#         q = np.floor(a / self.leaf).astype(np.int32)
#         # elegimos primer punto de cada voxel (podrías hacer media si quieres)
#         _, keep_idx = np.unique(q, axis=0, return_index=True)
#         out = a[keep_idx]

#         # Empaquetar PointCloud2 (x,y,z float32)
#         header = Header()
#         header.stamp = msg.header.stamp
#         header.frame_id = self.frame_id or msg.header.frame_id

#         out_bytes = out.astype(np.float32).tobytes()
#         pc2 = PointCloud2(
#             header=header,
#             height=1,
#             width=out.shape[0],
#             fields=[
#                 PointField(name='x', offset=0,  datatype=PointField.FLOAT32, count=1),
#                 PointField(name='y', offset=4,  datatype=PointField.FLOAT32, count=1),
#                 PointField(name='z', offset=8,  datatype=PointField.FLOAT32, count=1),
#             ],
#             is_bigendian=False,
#             point_step=12,
#             row_step=12*out.shape[0],
#             data=out_bytes,
#             is_dense=True
#         )
#         self.pub.publish(pc2)

# def main():
#     rclpy.init()
#     node = VoxelGridNode()
#     rclpy.spin(node)
#     node.destroy_node()
#     rclpy.shutdown()
# car/voxel_grid_node.py
import rclpy
from rclpy.node import Node
from rclpy.duration import Duration
from rclpy.time import Time

from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header

import numpy as np

import tf2_ros

def quat_to_mat(x, y, z, w):
    n = x*x + y*y + z*z + w*w
    if n < 1e-12:
        return np.eye(3, dtype=np.float64)
    s = 2.0 / n
    xx, yy, zz = x*x*s, y*y*s, z*z*s
    xy, xz, yz = x*y*s, x*z*s, y*z*s
    wx, wy, wz = w*x*s, w*y*s, w*z*s
    return np.array([
        [1.0 - (yy + zz),     xy - wz,           xz + wy],
        [    xy + wz,     1.0 - (xx + zz),       yz - wx],
        [    xz - wy,         yz + wx,       1.0 - (xx + yy)]
    ], dtype=np.float64)

class VoxelGridNode(Node):
    def __init__(self):
        super().__init__("voxel_grid")

        # === Parámetros de IO (los mismos que tenías) ===
        self.declare_parameter("input", "/zed/zed_node/point_cloud/cloud_registered")
        self.declare_parameter("output", "/merged_points_filtered")
        self.declare_parameter("leaf_size", 0.15)
        # frame de salida (si vacío, usa el del mensaje de entrada)
        self.declare_parameter("frame_id", "base_link")

        # === Filtro de exclusión por caja (opcional) ===
        self.declare_parameter("exclude_box_enabled", True)
        # Frame en el que defines la caja (normalmente base_link)
        self.declare_parameter("exclude_box_frame", "base_link")
        # Centro de la caja (m) en exclude_box_frame
        self.declare_parameter("exclude_box_center_x", 0.0)
        self.declare_parameter("exclude_box_center_y", 0.0)
        self.declare_parameter("exclude_box_center_z", 0.0)
        # Tamaños TOTALES de la caja (m)
        self.declare_parameter("exclude_box_size_x", 4.0)
        self.declare_parameter("exclude_box_size_y", 4.0)
        self.declare_parameter("exclude_box_size_z", 3.5)
        # TF (sólo si exclude_box_frame != frame del mensaje)
        self.declare_parameter("use_latest_tf", True)   # si falla el stamp, coge el último
        self.declare_parameter("tf_timeout", 0.07)      # segundos

        # === Lee parámetros ===
        in_topic  = self.get_parameter("input").get_parameter_value().string_value
        out_topic = self.get_parameter("output").get_parameter_value().string_value
        self.leaf = float(self.get_parameter("leaf_size").value)
        self.frame_id = self.get_parameter("frame_id").get_parameter_value().string_value

        self.exclude_enabled = bool(self.get_parameter("exclude_box_enabled").value)
        self.exclude_frame = self.get_parameter("exclude_box_frame").get_parameter_value().string_value
        self.ex_center = np.array([
            float(self.get_parameter("exclude_box_center_x").value),
            float(self.get_parameter("exclude_box_center_y").value),
            float(self.get_parameter("exclude_box_center_z").value)
        ], dtype=np.float64)
        self.ex_size = np.array([
            float(self.get_parameter("exclude_box_size_x").value),
            float(self.get_parameter("exclude_box_size_y").value),
            float(self.get_parameter("exclude_box_size_z").value)
        ], dtype=np.float64)

        self.use_latest_tf = bool(self.get_parameter("use_latest_tf").value)
        self.tf_timeout = float(self.get_parameter("tf_timeout").value)

        # Pub/Sub
        self.pub = self.create_publisher(PointCloud2, out_topic, 5)
        self.sub = self.create_subscription(PointCloud2, in_topic, self.cb, 5)

        # TF (sólo se usa si hace falta)
        self.tf_buffer = tf2_ros.Buffer(cache_time=Duration(seconds=5.0))
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self, spin_thread=True)

        self.get_logger().info(
            f"VoxelGrid: {in_topic} -> {out_topic} (leaf={self.leaf} m). "
            f"Filtro caja: {'ON' if self.exclude_enabled else 'OFF'} en frame '{self.exclude_frame}'."
        )

        self.warned_tf = False  # para no spamear warnings

    def cb(self, msg: PointCloud2):
        # Asumimos x,y,z float32 contiguos (point_step=12), como en tus publishers
        if msg.point_step != 12:
            self.get_logger().warn("point_step != 12. Este voxel sólo soporta nubes x,y,z compactas.")
            return
        npts = int(len(msg.data) // 12)
        if npts == 0:
            return

        # Vista numpy directa
        pts = np.frombuffer(msg.data, dtype=np.float32, count=npts*3).reshape(-1,3)

        # Filtra NaNs
        good = np.isfinite(pts).all(axis=1)
        pts = pts[good]
        if pts.size == 0:
            return

        # --- Filtro AABB (si está activado) ---
        if self.exclude_enabled and self.ex_size.min() > 0.0:
            pts_for_test = pts
            src_frame = msg.header.frame_id

            if self.exclude_frame and self.exclude_frame != src_frame:
                # Transformamos puntos al frame de la caja SÓLO para testear
                R_t = self.lookup_T(self.exclude_frame, src_frame, msg.header.stamp)
                if R_t is None:
                    if not self.warned_tf:
                        self.get_logger().warn(
                            f"Sin TF {self.exclude_frame}<-{src_frame}. "
                            f"Filtro de caja omitido (set use_latest_tf:=true o revisa TFs)."
                        )
                        self.warned_tf = True
                    # Omitimos filtro para no borrar nubes por error
                    pts_for_test = pts
                else:
                    R, t = R_t
                    pts_for_test = (pts @ R.T) + t  # a exclude_frame

            half = 0.5 * self.ex_size
            rel = pts_for_test - self.ex_center
            inside = (np.abs(rel[:,0]) <= half[0]) & (np.abs(rel[:,1]) <= half[1]) & (np.abs(rel[:,2]) <= half[2])
            if inside.any():
                pts = pts[~inside]
                if pts.size == 0:
                    return

        # --- Voxelización rápida por cuantización ---
        q = np.floor(pts / self.leaf).astype(np.int32)
        _, keep_idx = np.unique(q, axis=0, return_index=True)
        out = pts[keep_idx]

        # --- Empaquetar PointCloud2 (x,y,z float32) ---
        header = Header()
        header.stamp = msg.header.stamp
        header.frame_id = self.frame_id or msg.header.frame_id

        out_bytes = out.astype(np.float32).tobytes()
        pc2 = PointCloud2(
            header=header,
            height=1,
            width=out.shape[0],
            fields=[
                PointField(name='x', offset=0,  datatype=PointField.FLOAT32, count=1),
                PointField(name='y', offset=4,  datatype=PointField.FLOAT32, count=1),
                PointField(name='z', offset=8,  datatype=PointField.FLOAT32, count=1),
            ],
            is_bigendian=False,
            point_step=12,
            row_step=12*out.shape[0],
            data=out_bytes,
            is_dense=True
        )
        self.pub.publish(pc2)

    def lookup_T(self, tgt: str, src: str, stamp):
        """Devuelve (R,t) tal que p_tgt = p_src * R^T + t  (R: 3x3, t: 3,)"""
        try:
            tf = self.tf_buffer.lookup_transform(tgt, src, stamp, timeout=Duration(seconds=self.tf_timeout))
        except Exception:
            if not self.use_latest_tf:
                return None
            try:
                tf = self.tf_buffer.lookup_transform(tgt, src, Time(), timeout=Duration(seconds=self.tf_timeout))
            except Exception:
                return None

        t = tf.transform.translation
        q = tf.transform.rotation
        R = quat_to_mat(q.x, q.y, q.z, q.w)
        tvec = np.array([t.x, t.y, t.z], dtype=np.float64)
        return R, tvec

def main():
    rclpy.init()
    node = VoxelGridNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
