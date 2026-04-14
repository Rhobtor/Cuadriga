from __future__ import annotations

import math
from typing import Optional, Tuple

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data

from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import NavSatFix
from tf2_ros import Buffer, TransformListener


def _quat_rotate_vector(q_xyzw: Tuple[float, float, float, float], v_xyz: Tuple[float, float, float]) -> Tuple[float, float, float]:
    x, y, z, w = q_xyzw
    vx, vy, vz = v_xyz
    tx = 2.0 * (y * vz - z * vy)
    ty = 2.0 * (z * vx - x * vz)
    tz = 2.0 * (x * vy - y * vx)
    cx = y * tz - z * ty
    cy = z * tx - x * tz
    cz = x * ty - y * tx
    return (vx + w * tx + cx, vy + w * ty + cy, vz + w * tz + cz)


class NavSatFixGoalToMapPose(Node):
    """Convert a GNSS goal (lat/lon/alt) into a local map PoseStamped using TF.

    Pipeline:
      NavSatFix (WGS84 lat/lon/alt) -> ECEF XYZ (EPSG:4978) via pyproj
      then TF: output_frame (default: map) <- ecef_frame_id (default: ECEF)

    Output is a PoseStamped suitable for Nav2 on /goal_pose.
    """

    def __init__(self) -> None:
        super().__init__('navsatfix_goal_to_map_pose')

        self.declare_parameter('input_topic', '/goal_gnss')
        self.declare_parameter('output_topic', '/goal_pose')
        self.declare_parameter('output_frame', 'map')
        self.declare_parameter('ecef_frame_id', 'ECEF')
        self.declare_parameter('use_altitude', False)
        self.declare_parameter('fixed_altitude_m', 0.0)

        self._output_frame = self.get_parameter('output_frame').get_parameter_value().string_value
        self._ecef_frame_id = self.get_parameter('ecef_frame_id').get_parameter_value().string_value
        self._use_altitude = bool(self.get_parameter('use_altitude').get_parameter_value().bool_value)
        self._fixed_altitude_m = float(self.get_parameter('fixed_altitude_m').get_parameter_value().double_value)

        input_topic = self.get_parameter('input_topic').get_parameter_value().string_value
        output_topic = self.get_parameter('output_topic').get_parameter_value().string_value

        self._tf_buffer = Buffer()
        self._tf_listener = TransformListener(self._tf_buffer, self)

        self._transformer = self._create_ecef_transformer()

        self._pub = self.create_publisher(PoseStamped, output_topic, 10)
        self._sub = self.create_subscription(NavSatFix, input_topic, self._on_goal, qos_profile_sensor_data)

        self.get_logger().info(
            f"GNSS goal -> {self._output_frame} listo. Sub: {input_topic}  Pub: {output_topic}  TF: {self._ecef_frame_id}->{self._output_frame}"
        )

    @staticmethod
    def _create_ecef_transformer():
        try:
            from pyproj import Transformer
        except Exception as exc:  # pragma: no cover
            raise RuntimeError(
                'No se pudo importar pyproj. Instala el paquete (ej: python3-pyproj o pip install pyproj).'
            ) from exc

        # EPSG:4979 = WGS84 3D (lon, lat, h)
        # EPSG:4978 = ECEF (X, Y, Z)
        return Transformer.from_crs('EPSG:4326', 'EPSG:4978', always_xy=True)

    @staticmethod
    def _finite(v: float) -> bool:
        return math.isfinite(v)

    def _on_goal(self, msg: NavSatFix) -> None:
        lat = float(msg.latitude)
        lon = float(msg.longitude)

        if not (self._finite(lat) and self._finite(lon)):
            self.get_logger().warn('Lat/Lon inválidos (NaN/Inf); ignorando goal')
            return

        if self._use_altitude and self._finite(float(msg.altitude)):
            alt = float(msg.altitude)
        else:
            alt = float(self._fixed_altitude_m)

        x_ecef, y_ecef, z_ecef = self._transformer.transform(lon, lat, alt)

        # TF lookup: transform from ECEF into output_frame
        try:
            tf = self._tf_buffer.lookup_transform(
                self._output_frame,
                self._ecef_frame_id,
                rclpy.time.Time(),
                timeout=rclpy.duration.Duration(seconds=0.3),
            )
        except Exception as exc:  # noqa: BLE001
            self.get_logger().warn(
                f"No TF {self._output_frame}<-{self._ecef_frame_id} todavía; no publico goal. ({exc})",
                throttle_duration_sec=1.0,
            )
            return

        t = tf.transform.translation
        r = tf.transform.rotation
        q = (float(r.x), float(r.y), float(r.z), float(r.w))

        rx, ry, rz = _quat_rotate_vector(q, (float(x_ecef), float(y_ecef), float(z_ecef)))
        x_map = float(t.x) + float(rx)
        y_map = float(t.y) + float(ry)
        z_map = 0.0  # Nav2 goals are 2D; ignore vertical component in map frame

        out = PoseStamped()
        out.header.stamp = msg.header.stamp
        out.header.frame_id = self._output_frame
        out.pose.position.x = x_map
        out.pose.position.y = y_map
        out.pose.position.z = z_map
        out.pose.orientation.w = 1.0

        self._pub.publish(out)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = NavSatFixGoalToMapPose()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
