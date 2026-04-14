#!/usr/bin/env python3

import math

import rclpy
from rclpy.node import Node
from rclpy.duration import Duration

from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from tf2_ros import Buffer, TransformBroadcaster, TransformListener


class OdomToTf(Node):
    def __init__(self):
        super().__init__("odom_to_tf")

        self.topic = self.declare_parameter("odom_topic", "/fixposition/odometry").value
        # Optional overrides if Odometry messages do not fill frame_id / child_frame_id
        self.parent_frame = self.declare_parameter("parent_frame", "").value
        self.child_frame = self.declare_parameter("child_frame", "").value
        self.disable_if_tf_exists = self.declare_parameter("disable_if_tf_exists", True).value

        # Optional TF rate limiting. Set to >0 to reduce /tf bandwidth.
        # Example: tf_publish_rate_hz:=30.0
        self.tf_publish_rate_hz = float(self.declare_parameter("tf_publish_rate_hz", 0.0).value)
        self._tf_publish_period_ns = int(1e9 / self.tf_publish_rate_hz) if self.tf_publish_rate_hz > 0.0 else 0
        self._last_pub_ns = 0

        self._tf_buffer = Buffer(cache_time=Duration(seconds=10.0))
        self._tf_listener = TransformListener(self._tf_buffer, self)

        self._published_once = False
        self._disabled_due_to_existing = False

        self.sub = self.create_subscription(Odometry, self.topic, self.cb, 50)
        self.br = TransformBroadcaster(self)

    def _tf_exists(self, parent: str, child: str) -> bool:
        try:
            return self._tf_buffer.can_transform(
                parent,
                child,
                rclpy.time.Time(),
                timeout=Duration(seconds=0.0),
            )
        except Exception:
            return False

    def cb(self, msg: Odometry):
        if self._disabled_due_to_existing:
            return

        if self._tf_publish_period_ns > 0:
            now_ns = int(self.get_clock().now().nanoseconds)
            if (now_ns - self._last_pub_ns) < self._tf_publish_period_ns:
                return
            self._last_pub_ns = now_ns

        parent = str(msg.header.frame_id).strip() if msg.header.frame_id else ""
        child = str(msg.child_frame_id).strip() if msg.child_frame_id else ""

        parent_override = str(self.parent_frame).strip() if self.parent_frame else ""
        child_override = str(self.child_frame).strip() if self.child_frame else ""

        if not parent and parent_override:
            parent = parent_override
        if not child and child_override:
            child = child_override

        if not parent or not child:
            self.get_logger().warn(
                f"Odometry sin frames válidos (frame_id='{parent}', child_frame_id='{child}'); salto TF"
            )
            return

        if parent == child:
            self.get_logger().warn(
                f"Odometry con self-transform (frame_id==child_frame_id=='{parent}'); salto TF"
            )
            return

        # If another node already publishes this TF, do not publish duplicates.
        # Only check before our first publication to avoid disabling ourselves because of our own TF.
        if (not self._published_once) and bool(self.disable_if_tf_exists) and self._tf_exists(parent, child):
            self.get_logger().warn(
                f"TF ya existe para {parent}->{child}; deshabilito odom_to_tf para evitar conflictos"
            )
            self._disabled_due_to_existing = True
            return

        q = msg.pose.pose.orientation
        n = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w
        if (not math.isfinite(n)) or n < 1e-12:
            self.get_logger().warn("Quaternion inválido, salto TF")
            return

        s = 1.0 / math.sqrt(n)  # normalizar

        t = TransformStamped()
        t.header.stamp = msg.header.stamp
        t.header.frame_id = parent  # ECEF o FP_ENU0
        t.child_frame_id = child  # FP_POI
        t.transform.translation.x = msg.pose.pose.position.x
        t.transform.translation.y = msg.pose.pose.position.y
        t.transform.translation.z = msg.pose.pose.position.z
        t.transform.rotation.x = q.x * s
        t.transform.rotation.y = q.y * s
        t.transform.rotation.z = q.z * s
        t.transform.rotation.w = q.w * s

        self.br.sendTransform(t)
        self._published_once = True


def main():
    rclpy.init()
    node = OdomToTf()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
