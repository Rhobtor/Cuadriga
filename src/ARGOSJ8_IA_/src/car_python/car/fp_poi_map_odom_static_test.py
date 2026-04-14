#!/usr/bin/env python3

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


class FpPoiMapOdomStaticTest(Node):
    """Minimal TF test node with a static map anchored at startup.

    It listens once to `/fixposition/odometry_enu`, which gives the startup pose of
    `FP_POI` in `FP_ENU0`, and then publishes:

      FP_ENU0 -> map   (startup pose of FP_POI)
      map     -> odom  (identity)

    This keeps `map` static after startup while still making it coincide with the
    initial vehicle pose.
    """

    def __init__(self) -> None:
        super().__init__("fp_poi_map_odom_static_test")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("local_fixed_frame", "FP_ENU0")
        self.declare_parameter("fp_poi_frame", "FP_POI")
        self.declare_parameter("map_frame", "map")
        self.declare_parameter("odom_frame", "odom")

        self._odom_topic = str(self.get_parameter("odom_topic").value).strip()
        self._local_fixed_frame = str(self.get_parameter("local_fixed_frame").value).strip()
        self._fp_poi_frame = str(self.get_parameter("fp_poi_frame").value).strip()
        self._map_frame = str(self.get_parameter("map_frame").value).strip()
        self._odom_frame = str(self.get_parameter("odom_frame").value).strip()

        self._broadcaster = StaticTransformBroadcaster(self)

        qos = QoSProfile(depth=1)
        qos.reliability = ReliabilityPolicy.BEST_EFFORT
        qos.durability = DurabilityPolicy.VOLATILE

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._on_odom, qos)
        self.get_logger().info(
            f"Waiting for 1 msg on {self._odom_topic} to publish static TF chain: "
            f"{self._local_fixed_frame}->{self._map_frame}->{self._odom_frame}"
        )

    def _on_odom(self, msg: Odometry) -> None:
        if msg.header.frame_id and msg.header.frame_id != self._local_fixed_frame:
            self.get_logger().warn(
                f"odom header.frame_id='{msg.header.frame_id}', expected '{self._local_fixed_frame}'. Using message anyway."
            )

        if msg.child_frame_id and msg.child_frame_id != self._fp_poi_frame:
            self.get_logger().warn(
                f"odom child_frame_id='{msg.child_frame_id}', expected '{self._fp_poi_frame}'. Using message anyway."
            )

        now = self.get_clock().now().to_msg()

        tf_local_fixed_to_map = TransformStamped()
        tf_local_fixed_to_map.header.stamp = now
        tf_local_fixed_to_map.header.frame_id = self._local_fixed_frame
        tf_local_fixed_to_map.child_frame_id = self._map_frame
        tf_local_fixed_to_map.transform.translation.x = float(msg.pose.pose.position.x)
        tf_local_fixed_to_map.transform.translation.y = float(msg.pose.pose.position.y)
        tf_local_fixed_to_map.transform.translation.z = float(msg.pose.pose.position.z)
        tf_local_fixed_to_map.transform.rotation.x = float(msg.pose.pose.orientation.x)
        tf_local_fixed_to_map.transform.rotation.y = float(msg.pose.pose.orientation.y)
        tf_local_fixed_to_map.transform.rotation.z = float(msg.pose.pose.orientation.z)
        tf_local_fixed_to_map.transform.rotation.w = float(msg.pose.pose.orientation.w)

        tf_map_to_odom = TransformStamped()
        tf_map_to_odom.header.stamp = now
        tf_map_to_odom.header.frame_id = self._map_frame
        tf_map_to_odom.child_frame_id = self._odom_frame
        tf_map_to_odom.transform.rotation.w = 1.0

        self._broadcaster.sendTransform([tf_local_fixed_to_map, tf_map_to_odom])

        self.get_logger().info(
            f"Published static TF chain: {self._local_fixed_frame}->{self._map_frame}->{self._odom_frame} "
            f"using startup pose of {self._fp_poi_frame}."
        )

        self.destroy_subscription(self._sub)


def main() -> None:
    rclpy.init()
    node = FpPoiMapOdomStaticTest()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
