from __future__ import annotations

import math
from typing import Optional

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data

from geometry_msgs.msg import PointStamped
from sensor_msgs.msg import NavSatFix


class NavSatFixToEcefNode(Node):
    def __init__(self) -> None:
        super().__init__('navsatfix_to_ecef')

        self.declare_parameter('input_topic', '/fix')
        self.declare_parameter('output_topic', '/ecef')
        self.declare_parameter('ecef_frame_id', 'ecef')

        self._ecef_frame_id = (
            self.get_parameter('ecef_frame_id').get_parameter_value().string_value
        )

        input_topic = self.get_parameter('input_topic').get_parameter_value().string_value
        output_topic = self.get_parameter('output_topic').get_parameter_value().string_value

        self._pub = self.create_publisher(PointStamped, output_topic, 10)
        self._sub = self.create_subscription(
            NavSatFix,
            input_topic,
            self._on_fix,
            qos_profile_sensor_data,
        )

        self._transformer = self._create_transformer()
        self.get_logger().info(
            f"NavSatFix->ECEF listo. Sub: {input_topic}  Pub: {output_topic}  frame_id: {self._ecef_frame_id}"
        )

    @staticmethod
    def _create_transformer():
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
    def _is_finite(value: float) -> bool:
        return not (math.isnan(value) or math.isinf(value))

    def _on_fix(self, msg: NavSatFix) -> None:
        lat = float(msg.latitude)
        lon = float(msg.longitude)
        alt = float(msg.altitude)

        if not (self._is_finite(lat) and self._is_finite(lon)):
            self.get_logger().warn('Lat/Lon inválidos (NaN/Inf); ignorando mensaje')
            return

        if not self._is_finite(alt):
            alt = 0.0

        # pyproj con always_xy=True espera (lon, lat, alt)
        x, y, z = self._transformer.transform(lon, lat, alt)

        out = PointStamped()
        out.header = msg.header
        out.header.frame_id = self._ecef_frame_id
        out.point.x = float(x)
        out.point.y = float(y)
        out.point.z = float(z)

        self._pub.publish(out)


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = NavSatFixToEcefNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
