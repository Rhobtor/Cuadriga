#!/usr/bin/env python3

import math
from typing import Tuple

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


def _quat_normalize(x: float, y: float, z: float, w: float) -> Tuple[float, float, float, float]:
    n = x * x + y * y + z * z + w * w
    if (not math.isfinite(n)) or n < 1e-12:
        raise ValueError("invalid quaternion")
    s = 1.0 / math.sqrt(n)
    return (x * s, y * s, z * s, w * s)


def _quat_multiply(a: Tuple[float, float, float, float], b: Tuple[float, float, float, float]) -> Tuple[float, float, float, float]:
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    x = aw * bx + ax * bw + ay * bz - az * by
    y = aw * by - ax * bz + ay * bw + az * bx
    z = aw * bz + ax * by - ay * bx + az * bw
    w = aw * bw - ax * bx - ay * by - az * bz
    return _quat_normalize(x, y, z, w)


def _quat_conjugate(q: Tuple[float, float, float, float]) -> Tuple[float, float, float, float]:
    x, y, z, w = q
    return (-x, -y, -z, w)


def _quat_rotate_vector(q: Tuple[float, float, float, float], v: Tuple[float, float, float]) -> Tuple[float, float, float]:
    x, y, z, w = q
    vx, vy, vz = v
    tx = 2.0 * (y * vz - z * vy)
    ty = 2.0 * (z * vx - x * vz)
    tz = 2.0 * (x * vy - y * vx)
    cx = y * tz - z * ty
    cy = z * tx - x * tz
    cz = x * ty - y * tx
    return (vx + w * tx + cx, vy + w * ty + cy, vz + w * tz + cz)


def _quat_from_rpy(roll: float, pitch: float, yaw: float) -> Tuple[float, float, float, float]:
    cr = math.cos(roll * 0.5)
    sr = math.sin(roll * 0.5)
    cp = math.cos(pitch * 0.5)
    sp = math.sin(pitch * 0.5)
    cy = math.cos(yaw * 0.5)
    sy = math.sin(yaw * 0.5)

    w = cy * cp * cr + sy * sp * sr
    x = cy * cp * sr - sy * sp * cr
    y = sy * cp * sr + cy * sp * cr
    z = sy * cp * cr - cy * sp * sr
    return _quat_normalize(x, y, z, w)


def _quat_from_yaw(yaw: float) -> Tuple[float, float, float, float]:
    return _quat_from_rpy(0.0, 0.0, yaw)


def _compose(
    t1_xyz: Tuple[float, float, float],
    t1_q: Tuple[float, float, float, float],
    t2_xyz: Tuple[float, float, float],
    t2_q: Tuple[float, float, float, float],
) -> Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]:
    rx, ry, rz = _quat_rotate_vector(t1_q, t2_xyz)
    out_xyz = (t1_xyz[0] + rx, t1_xyz[1] + ry, t1_xyz[2] + rz)
    out_q = _quat_multiply(t1_q, t2_q)
    return out_xyz, out_q


def _inverse(
    xyz: Tuple[float, float, float],
    q: Tuple[float, float, float, float],
) -> Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]:
    qi = _quat_conjugate(q)
    rx, ry, rz = _quat_rotate_vector(qi, (-xyz[0], -xyz[1], -xyz[2]))
    return (rx, ry, rz), qi


def _yaw_from_quaternion(x: float, y: float, z: float, w: float) -> float:
    # yaw = atan2(2(wz + xy), 1 - 2(y^2 + z^2))
    return math.atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))


class FixpositionMapOdomStaticFromOdom(Node):
    """Publishes a one-shot static TF map->odom based on the first odometry message.

    The goal is to create a conventional TF root (map) without requiring Fixposition's
    full TF tree. It uses /fixposition/odometry_enu (or any nav_msgs/Odometry topic)
    and an optional rigid offset from the odometry child frame to base_link.

    By default it publishes ONLY map->odom (static). Optionally it can also publish:
    - odom-><source_parent_frame> identity (frame alias)
    - odom->base_link as static (freezes base_link; usually not desired)
    """

    def __init__(self) -> None:
        super().__init__("fixposition_map_odom_static_from_odom")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("map_frame", "map")
        self.declare_parameter("odom_frame", "odom")
        self.declare_parameter("base_link_frame", "base_link")

        self.declare_parameter("source_parent_frame", "")
        self.declare_parameter("source_child_frame", "")

        # Offset from odometry child frame (e.g., FP_POI) to base_link
        self.declare_parameter("x", 0.0)
        self.declare_parameter("y", 0.0)
        self.declare_parameter("z", 0.0)
        self.declare_parameter("roll", 0.0)
        self.declare_parameter("pitch", 0.0)
        self.declare_parameter("yaw", 0.0)

        self.declare_parameter("align_yaw", False)
        self.declare_parameter("publish_source_parent_alias", True)
        self.declare_parameter("freeze_base_link_static", False)

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._map_frame = str(self.get_parameter("map_frame").value).strip()
        self._odom_frame = str(self.get_parameter("odom_frame").value).strip()
        self._base_link_frame = str(self.get_parameter("base_link_frame").value).strip()

        self._source_parent_override = str(self.get_parameter("source_parent_frame").value).strip()
        self._source_child_override = str(self.get_parameter("source_child_frame").value).strip()

        self._offset_xyz = (
            float(self.get_parameter("x").value),
            float(self.get_parameter("y").value),
            float(self.get_parameter("z").value),
        )
        self._offset_q = _quat_from_rpy(
            float(self.get_parameter("roll").value),
            float(self.get_parameter("pitch").value),
            float(self.get_parameter("yaw").value),
        )

        self._align_yaw = bool(self.get_parameter("align_yaw").value)
        self._publish_source_parent_alias = bool(self.get_parameter("publish_source_parent_alias").value)
        self._freeze_base_link_static = bool(self.get_parameter("freeze_base_link_static").value)

        self._br = StaticTransformBroadcaster(self)
        self._done = False

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._cb, 10)
        self.get_logger().info(
            f"Waiting first odom on {self._odom_topic} to publish static {self._map_frame}->{self._odom_frame}"
        )

    def _cb(self, msg: Odometry) -> None:
        if self._done:
            return

        if not self._map_frame or not self._odom_frame:
            self.get_logger().error("map_frame/odom_frame are empty")
            return

        source_parent = str(msg.header.frame_id).strip() if msg.header.frame_id else ""
        source_child = str(msg.child_frame_id).strip() if msg.child_frame_id else ""

        if self._source_parent_override:
            source_parent = self._source_parent_override
        if self._source_child_override:
            source_child = self._source_child_override

        if not source_parent or not source_child:
            self.get_logger().warn(
                f"Odometry without valid frames (frame_id='{source_parent}', child_frame_id='{source_child}'); skip",
                throttle_duration_sec=1.0,
            )
            return

        # Pose T_sourceParent_sourceChild from odometry
        p = msg.pose.pose.position
        q = msg.pose.pose.orientation
        try:
            q_sp_sc = _quat_normalize(float(q.x), float(q.y), float(q.z), float(q.w))
        except ValueError:
            self.get_logger().warn("Invalid odom quaternion; skip", throttle_duration_sec=1.0)
            return
        t_sp_sc = ((float(p.x), float(p.y), float(p.z)), q_sp_sc)

        # Static offset T_sourceChild_baseLink
        t_sc_bl = (self._offset_xyz, self._offset_q)

        # Compose T_sourceParent_baseLink
        xyz_sp_bl, q_sp_bl = _compose(t_sp_sc[0], t_sp_sc[1], t_sc_bl[0], t_sc_bl[1])

        # Compute map->sourceParent so that base_link is at origin in map at startup.
        if self._align_yaw:
            yaw0 = _yaw_from_quaternion(q_sp_bl[0], q_sp_bl[1], q_sp_bl[2], q_sp_bl[3])
            q_map_sp = _quat_from_yaw(-yaw0)
            rx, ry, rz = _quat_rotate_vector(q_map_sp, xyz_sp_bl)
            xyz_map_sp = (-rx, -ry, -rz)
        else:
            xyz_map_sp, q_map_sp = _inverse(xyz_sp_bl, q_sp_bl)

        tf_map_odom = TransformStamped()
        tf_map_odom.header.stamp = self.get_clock().now().to_msg()
        tf_map_odom.header.frame_id = self._map_frame
        tf_map_odom.child_frame_id = self._odom_frame
        tf_map_odom.transform.translation.x = float(xyz_map_sp[0])
        tf_map_odom.transform.translation.y = float(xyz_map_sp[1])
        tf_map_odom.transform.translation.z = float(xyz_map_sp[2])
        tf_map_odom.transform.rotation.x = float(q_map_sp[0])
        tf_map_odom.transform.rotation.y = float(q_map_sp[1])
        tf_map_odom.transform.rotation.z = float(q_map_sp[2])
        tf_map_odom.transform.rotation.w = float(q_map_sp[3])

        tfs = [tf_map_odom]

        # Optional alias: odom_frame -> source_parent (identity) so both names can be used.
        if self._publish_source_parent_alias and source_parent and source_parent != self._odom_frame:
            alias = TransformStamped()
            alias.header.stamp = tf_map_odom.header.stamp
            alias.header.frame_id = self._odom_frame
            alias.child_frame_id = source_parent
            alias.transform.rotation.w = 1.0
            tfs.append(alias)

        # Optional: freeze base_link as static under odom (generally NOT recommended)
        if self._freeze_base_link_static:
            if not self._base_link_frame:
                self.get_logger().error("base_link_frame is empty")
            else:
                tf_odom_bl = TransformStamped()
                tf_odom_bl.header.stamp = tf_map_odom.header.stamp
                tf_odom_bl.header.frame_id = self._odom_frame
                tf_odom_bl.child_frame_id = self._base_link_frame
                tf_odom_bl.transform.translation.x = float(xyz_sp_bl[0])
                tf_odom_bl.transform.translation.y = float(xyz_sp_bl[1])
                tf_odom_bl.transform.translation.z = float(xyz_sp_bl[2])
                tf_odom_bl.transform.rotation.x = float(q_sp_bl[0])
                tf_odom_bl.transform.rotation.y = float(q_sp_bl[1])
                tf_odom_bl.transform.rotation.z = float(q_sp_bl[2])
                tf_odom_bl.transform.rotation.w = float(q_sp_bl[3])
                tfs.append(tf_odom_bl)

        self._br.sendTransform(tfs)

        self.get_logger().info(
            f"Published static {self._map_frame}->{self._odom_frame} from first odom sample "
            f"(source_parent='{source_parent}', source_child='{source_child}', align_yaw={self._align_yaw})"
        )

        self._done = True
        self.destroy_subscription(self._sub)


def main() -> None:
    rclpy.init()
    node = FixpositionMapOdomStaticFromOdom()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
