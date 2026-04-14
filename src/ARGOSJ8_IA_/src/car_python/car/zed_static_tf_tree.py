#!/usr/bin/env python3

import math

import rclpy
from geometry_msgs.msg import TransformStamped
from rclpy.node import Node
from tf2_ros import StaticTransformBroadcaster


def _quat_from_rpy(roll: float, pitch: float, yaw: float) -> tuple[float, float, float, float]:
    cr = math.cos(roll * 0.5)
    sr = math.sin(roll * 0.5)
    cp = math.cos(pitch * 0.5)
    sp = math.sin(pitch * 0.5)
    cy = math.cos(yaw * 0.5)
    sy = math.sin(yaw * 0.5)

    # intrinsic XYZ (roll-pitch-yaw) => quaternion
    qw = cr * cp * cy + sr * sp * sy
    qx = sr * cp * cy - cr * sp * sy
    qy = cr * sp * cy + sr * cp * sy
    qz = cr * cp * sy - sr * sp * cy
    return (qx, qy, qz, qw)


class ZedStaticTfTree(Node):
    """Publish a minimal ZED TF tree rooted at zed_camera_link.

    Use this when the ZED wrapper is not publishing TF (or you want full control).
    It publishes:
      zed_camera_link -> zed_camera_center
      zed_camera_link -> zed_left_camera_frame
      zed_camera_link -> zed_right_camera_frame
      *_camera_frame -> *_camera_optical_frame (standard ROS optical rotation)
      zed_camera_link -> zed_imu_link

    All translations are configurable; defaults are conservative identities except baseline.
    """

    def __init__(self) -> None:
        super().__init__("zed_static_tf_tree")

        self.declare_parameter("root_frame", "zed_camera_link")
        self.declare_parameter("center_frame", "zed_camera_center")
        self.declare_parameter("left_frame", "zed_left_camera_frame")
        self.declare_parameter("right_frame", "zed_right_camera_frame")
        self.declare_parameter("left_optical_frame", "zed_left_camera_optical_frame")
        self.declare_parameter("right_optical_frame", "zed_right_camera_optical_frame")
        self.declare_parameter("imu_frame", "zed_imu_link")

        self.declare_parameter("baseline_m", 0.12)

        self.declare_parameter("center_x", 0.0)
        self.declare_parameter("center_y", 0.0)
        self.declare_parameter("center_z", 0.0)

        self.declare_parameter("imu_x", 0.0)
        self.declare_parameter("imu_y", 0.0)
        self.declare_parameter("imu_z", 0.0)

        # Optical frame rotation from a typical camera_link (x forward, y left, z up)
        # to optical frame (z forward, x right, y down).
        self.declare_parameter("optical_roll", -math.pi / 2)
        self.declare_parameter("optical_pitch", 0.0)
        self.declare_parameter("optical_yaw", -math.pi / 2)

        root = str(self.get_parameter("root_frame").value)
        center = str(self.get_parameter("center_frame").value)
        left = str(self.get_parameter("left_frame").value)
        right = str(self.get_parameter("right_frame").value)
        left_opt = str(self.get_parameter("left_optical_frame").value)
        right_opt = str(self.get_parameter("right_optical_frame").value)
        imu = str(self.get_parameter("imu_frame").value)

        baseline = float(self.get_parameter("baseline_m").value)

        center_x = float(self.get_parameter("center_x").value)
        center_y = float(self.get_parameter("center_y").value)
        center_z = float(self.get_parameter("center_z").value)

        imu_x = float(self.get_parameter("imu_x").value)
        imu_y = float(self.get_parameter("imu_y").value)
        imu_z = float(self.get_parameter("imu_z").value)

        optical_roll = float(self.get_parameter("optical_roll").value)
        optical_pitch = float(self.get_parameter("optical_pitch").value)
        optical_yaw = float(self.get_parameter("optical_yaw").value)
        q_opt = _quat_from_rpy(optical_roll, optical_pitch, optical_yaw)

        br = StaticTransformBroadcaster(self)
        now = self.get_clock().now().to_msg()

        def make(parent: str, child: str, x: float, y: float, z: float, q: tuple[float, float, float, float]) -> TransformStamped:
            t = TransformStamped()
            t.header.stamp = now
            t.header.frame_id = parent
            t.child_frame_id = child
            t.transform.translation.x = float(x)
            t.transform.translation.y = float(y)
            t.transform.translation.z = float(z)
            t.transform.rotation.x = float(q[0])
            t.transform.rotation.y = float(q[1])
            t.transform.rotation.z = float(q[2])
            t.transform.rotation.w = float(q[3])
            return t

        q_id = (0.0, 0.0, 0.0, 1.0)

        transforms: list[TransformStamped] = []

        # Root -> center
        transforms.append(make(root, center, center_x, center_y, center_z, q_id))

        # Root -> left/right frames (baseline along +Y/-Y in ROS REP-103 for a forward-facing camera)
        transforms.append(make(root, left, 0.0, +baseline / 2.0, 0.0, q_id))
        transforms.append(make(root, right, 0.0, -baseline / 2.0, 0.0, q_id))

        # Left/right -> optical
        transforms.append(make(left, left_opt, 0.0, 0.0, 0.0, q_opt))
        transforms.append(make(right, right_opt, 0.0, 0.0, 0.0, q_opt))

        # Root -> IMU
        transforms.append(make(root, imu, imu_x, imu_y, imu_z, q_id))

        br.sendTransform(transforms)

        self.get_logger().info(
            "Published static ZED TF tree rooted at '%s' (baseline=%.3f m)." % (root, baseline)
        )


def main() -> None:
    rclpy.init()
    node = ZedStaticTfTree()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
