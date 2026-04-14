#!/usr/bin/env python3

from __future__ import annotations

from typing import Optional

import rclpy
from nav_msgs.msg import Odometry
from rclpy.node import Node
from rclpy.qos import HistoryPolicy, QoSProfile, ReliabilityPolicy
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import NavSatFix


class FixpositionReframe(Node):
    def __init__(self) -> None:
        super().__init__("fixposition_reframe")

        # Inputs
        self.declare_parameter("odom_in", "/fixposition/odometry_enu")
        self.declare_parameter("gnss1_in", "/fixposition/gnss1")
        self.declare_parameter("gnss2_in", "/fixposition/gnss2")

        # Outputs
        self.declare_parameter("odom_out", "/j8/fixposition/odometry")
        self.declare_parameter("gnss1_out", "/j8/fixposition/gnss1")
        self.declare_parameter("gnss2_out", "/j8/fixposition/gnss2")

        # Our frames (the only ones we want to expose)
        self.declare_parameter("odom_frame", "odom")
        self.declare_parameter("base_link_frame", "base_link")
        self.declare_parameter("map_frame", "map")

        # QoS (outputs)
        # If any downstream subscriber requests RELIABLE, publisher must be RELIABLE.
        self.declare_parameter("out_reliable", True)
        self.declare_parameter("out_depth", 50)

        # Stamp behavior
        self.declare_parameter("preserve_input_stamp", True)

        self._odom_in = str(self.get_parameter("odom_in").value)
        self._gnss1_in = str(self.get_parameter("gnss1_in").value)
        self._gnss2_in = str(self.get_parameter("gnss2_in").value)

        self._odom_out = str(self.get_parameter("odom_out").value)
        self._gnss1_out = str(self.get_parameter("gnss1_out").value)
        self._gnss2_out = str(self.get_parameter("gnss2_out").value)

        self._odom_frame = str(self.get_parameter("odom_frame").value)
        self._base_link_frame = str(self.get_parameter("base_link_frame").value)
        self._map_frame = str(self.get_parameter("map_frame").value)

        out_reliable = bool(self.get_parameter("out_reliable").value)
        out_depth = int(self.get_parameter("out_depth").value)
        self._out_qos = QoSProfile(
            history=HistoryPolicy.KEEP_LAST,
            depth=max(out_depth, 1),
            reliability=ReliabilityPolicy.RELIABLE if out_reliable else ReliabilityPolicy.BEST_EFFORT,
        )

        self._preserve_stamp = bool(self.get_parameter("preserve_input_stamp").value)

        self._pub_odom = self.create_publisher(Odometry, self._odom_out, self._out_qos)
        self._pub_gnss1 = self.create_publisher(NavSatFix, self._gnss1_out, self._out_qos)
        self._pub_gnss2 = self.create_publisher(NavSatFix, self._gnss2_out, self._out_qos)

        self._sub_odom = self.create_subscription(Odometry, self._odom_in, self._on_odom, qos_profile_sensor_data)
        self._sub_gnss1 = self.create_subscription(NavSatFix, self._gnss1_in, self._on_gnss1, qos_profile_sensor_data)
        self._sub_gnss2 = self.create_subscription(NavSatFix, self._gnss2_in, self._on_gnss2, qos_profile_sensor_data)

        self.get_logger().info(
            "Reframing Fixposition topics (no TF): "
            f"odom: {self._odom_in} -> {self._odom_out} (frame_id='{self._odom_frame}', child='{self._base_link_frame}'); "
            f"gnss1: {self._gnss1_in} -> {self._gnss1_out} (frame_id='{self._map_frame}'); "
            f"gnss2: {self._gnss2_in} -> {self._gnss2_out} (frame_id='{self._map_frame}'); "
            f"out_qos=reliability={'RELIABLE' if out_reliable else 'BEST_EFFORT'} depth={self._out_qos.depth}"
        )

    def _stamp(self, msg_stamp) -> object:
        if self._preserve_stamp:
            return msg_stamp
        return self.get_clock().now().to_msg()

    def _rewrite_header(self, header, frame_id: str) -> None:
        header.frame_id = frame_id
        header.stamp = self._stamp(header.stamp)

    def _on_odom(self, msg: Odometry) -> None:
        out = Odometry()
        out.header = msg.header
        self._rewrite_header(out.header, self._odom_frame)

        out.child_frame_id = self._base_link_frame

        out.pose = msg.pose
        out.twist = msg.twist

        self._pub_odom.publish(out)

    def _on_gnss1(self, msg: NavSatFix) -> None:
        self._pub_gnss1.publish(self._reframe_gnss(msg))

    def _on_gnss2(self, msg: NavSatFix) -> None:
        self._pub_gnss2.publish(self._reframe_gnss(msg))

    def _reframe_gnss(self, msg: NavSatFix) -> NavSatFix:
        out = NavSatFix()
        out.header = msg.header
        self._rewrite_header(out.header, self._map_frame)

        out.status = msg.status
        out.latitude = msg.latitude
        out.longitude = msg.longitude
        out.altitude = msg.altitude
        out.position_covariance = msg.position_covariance
        out.position_covariance_type = msg.position_covariance_type
        return out


def main(args: Optional[list[str]] = None) -> None:
    rclpy.init(args=args)
    node = FixpositionReframe()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
