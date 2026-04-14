#!/usr/bin/env python3

from __future__ import annotations

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


class FpPoiMapStaticFromStart(Node):
    """Publish a single static TF that anchors `map` at the startup FP_POI pose.

    We listen once to `/fixposition/odometry_enu` (default), which provides the pose of `FP_POI`
    in `FP_ENU0` (header.frame_id = FP_ENU0, child_frame_id = FP_POI).

    Then we publish a STATIC transform:
      local_fixed_frame (FP_ENU0)  ->  map
    using that initial pose.

    Result at startup:
      map coincides with FP_POI (i.e. map->FP_POI is identity at t0).

    This does NOT create a conflict with Fixposition TFs, because Fixposition already publishes
    ECEF->FP_ENU0 and we only add FP_ENU0->map.
    """

    def __init__(self) -> None:
        super().__init__("fp_poi_map_static_from_start")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("local_fixed_frame", "FP_ENU0")
        self.declare_parameter("fixposition_robot_frame", "FP_POI")
        self.declare_parameter("map_frame", "map")

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._local_fixed_frame = str(self.get_parameter("local_fixed_frame").value).strip()
        self._fp_robot_frame = str(self.get_parameter("fixposition_robot_frame").value).strip()
        self._map_frame = str(self.get_parameter("map_frame").value).strip()

        self._static_br = StaticTransformBroadcaster(self)

        qos = QoSProfile(depth=1)
        qos.reliability = ReliabilityPolicy.BEST_EFFORT
        qos.durability = DurabilityPolicy.VOLATILE

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._on_odom, qos)
        self.get_logger().info(
            f"Waiting for 1 msg on {self._odom_topic} to publish static TF {self._local_fixed_frame}->{self._map_frame}"
        )

    def _on_odom(self, msg: Odometry) -> None:
        if msg.header.frame_id and self._local_fixed_frame and msg.header.frame_id != self._local_fixed_frame:
            self.get_logger().warn(
                f"odom header.frame_id='{msg.header.frame_id}' but local_fixed_frame='{self._local_fixed_frame}'. Using message anyway."
            )

        if msg.child_frame_id and self._fp_robot_frame and msg.child_frame_id != self._fp_robot_frame:
            self.get_logger().warn(
                f"odom child_frame_id='{msg.child_frame_id}' but fixposition_robot_frame='{self._fp_robot_frame}'. Using message anyway."
            )

        p = msg.pose.pose.position
        q = msg.pose.pose.orientation

        tf = TransformStamped()
        tf.header.stamp = self.get_clock().now().to_msg()
        tf.header.frame_id = self._local_fixed_frame or msg.header.frame_id
        tf.child_frame_id = self._map_frame
        tf.transform.translation.x = float(p.x)
        tf.transform.translation.y = float(p.y)
        tf.transform.translation.z = float(p.z)
        tf.transform.rotation.x = float(q.x)
        tf.transform.rotation.y = float(q.y)
        tf.transform.rotation.z = float(q.z)
        tf.transform.rotation.w = float(q.w)

        self._static_br.sendTransform(tf)
        self.get_logger().info(
            f"Published static TF {tf.header.frame_id}->{tf.child_frame_id} from startup odom pose (map coincides with {self._fp_robot_frame} at t0)."
        )

        # One-shot
        self.destroy_subscription(self._sub)


def main() -> None:
    rclpy.init()
    node = FpPoiMapStaticFromStart()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
