#!/usr/bin/env python3

import math
from typing import Optional, Tuple

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.duration import Duration
from rclpy.node import Node
from tf2_ros import Buffer, TransformBroadcaster, TransformListener


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
    # t = 2 * cross(q_xyz, v)
    tx = 2.0 * (y * vz - z * vy)
    ty = 2.0 * (z * vx - x * vz)
    tz = 2.0 * (x * vy - y * vx)
    # v' = v + w * t + cross(q_xyz, t)
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

    # ROS convention: R = Rz(yaw) * Ry(pitch) * Rx(roll)
    w = cy * cp * cr + sy * sp * sr
    x = cy * cp * sr - sy * sp * cr
    y = sy * cp * sr + cy * sp * cr
    z = sy * cp * cr - cy * sp * sr
    return _quat_normalize(x, y, z, w)


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


def _inverse(xyz: Tuple[float, float, float], q: Tuple[float, float, float, float]) -> Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]:
    qi = _quat_conjugate(q)
    rx, ry, rz = _quat_rotate_vector(qi, (-xyz[0], -xyz[1], -xyz[2]))
    return (rx, ry, rz), qi


class FixpositionOdomToBaseLinkTf(Node):
    def __init__(self) -> None:
        super().__init__("fixposition_odom_to_base_link_tf")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("target_parent_frame", "odom")
        self.declare_parameter("output_child_frame", "base_link")

        # Frame used for TF lookups to convert the odometry pose into the output parent frame.
        # Typical setup: map->odom is identity, so we lookup in map and publish in odom.
        self.declare_parameter("lookup_target_frame", "map")

        # Optionally override odom msg frames (useful when a driver doesn't fill them)
        self.declare_parameter("source_parent_frame", "")
        self.declare_parameter("source_child_frame", "")

        # Static rigid offset from source_child_frame (e.g., FP_POI) to base_link
        self.declare_parameter("x", 0.0)
        self.declare_parameter("y", 0.0)
        self.declare_parameter("z", 0.0)
        self.declare_parameter("roll", 0.0)
        self.declare_parameter("pitch", 0.0)
        self.declare_parameter("yaw", 0.0)

        self.declare_parameter("tf_lookup_timeout_sec", 0.2)
        self.declare_parameter("disable_if_tf_exists", True)

        # Optional TF rate limiting. Set to >0 to reduce /tf bandwidth.
        # Example: tf_publish_rate_hz:=30.0
        self.declare_parameter("tf_publish_rate_hz", 0.0)

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._target_parent_frame = str(self.get_parameter("target_parent_frame").value).strip()
        self._output_child_frame = str(self.get_parameter("output_child_frame").value).strip()
        self._lookup_target_frame = str(self.get_parameter("lookup_target_frame").value).strip()
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

        self._timeout = float(self.get_parameter("tf_lookup_timeout_sec").value)
        self._disable_if_tf_exists = bool(self.get_parameter("disable_if_tf_exists").value)

        tf_publish_rate_hz = float(self.get_parameter("tf_publish_rate_hz").value)
        self._tf_publish_period_ns = int(1e9 / tf_publish_rate_hz) if tf_publish_rate_hz > 0.0 else 0
        self._last_pub_ns = 0

        self._tf_buffer = Buffer(cache_time=Duration(seconds=10.0))
        self._tf_listener = TransformListener(self._tf_buffer, self)
        self._br = TransformBroadcaster(self)

        self._published_once = False
        self._disabled_due_to_existing = False

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._cb, 50)
        self.get_logger().info(
            f"Publishing TF {self._target_parent_frame}->{self._output_child_frame} from {self._odom_topic} (with static offset, lookup in '{self._lookup_target_frame}')"
        )

    def _tf_exists(self, parent: str, child: str) -> bool:
        try:
            return self._tf_buffer.can_transform(parent, child, rclpy.time.Time(), timeout=Duration(seconds=0.0))
        except Exception:
            return False

    def _lookup_parent_to_parent(self, lookup_target: str, source_parent: str) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]]:
        if lookup_target == source_parent:
            return ((0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0))

        try:
            tf = self._tf_buffer.lookup_transform(
                lookup_target,
                source_parent,
                rclpy.time.Time(),
                timeout=Duration(seconds=self._timeout),
            )
        except Exception as exc:  # noqa: BLE001
            self.get_logger().warn(
                f"TF lookup failed for {lookup_target}->{source_parent}: {exc}",
                throttle_duration_sec=1.0,
            )
            return None

        t = tf.transform.translation
        r = tf.transform.rotation
        q = _quat_normalize(float(r.x), float(r.y), float(r.z), float(r.w))
        return ((float(t.x), float(t.y), float(t.z)), q)

    def _cb(self, msg: Odometry) -> None:
        if self._disabled_due_to_existing:
            return

        if self._tf_publish_period_ns > 0:
            now_ns = int(self.get_clock().now().nanoseconds)
            if (now_ns - self._last_pub_ns) < self._tf_publish_period_ns:
                return
            self._last_pub_ns = now_ns

        if not self._target_parent_frame or not self._output_child_frame:
            self.get_logger().error("target_parent_frame/output_child_frame are empty")
            return

        if not self._lookup_target_frame:
            self.get_logger().error("lookup_target_frame is empty")
            return

        if (not self._published_once) and self._disable_if_tf_exists and self._tf_exists(self._target_parent_frame, self._output_child_frame):
            self.get_logger().warn(
                f"TF already exists for {self._target_parent_frame}->{self._output_child_frame}; disabling to avoid conflicts"
            )
            self._disabled_due_to_existing = True
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

        if source_parent == source_child:
            self.get_logger().warn(
                f"Odometry with self-transform (frame_id==child_frame_id=='{source_parent}'); skip",
                throttle_duration_sec=1.0,
            )
            return

        # Lookup in a stable/global-ish frame (typically 'map') and publish under odom.
        # If map->odom is identity, the numeric pose is valid in both frames.
        t_tp_sp = self._lookup_parent_to_parent(self._lookup_target_frame, source_parent)
        if t_tp_sp is None:
            return

        # T_sourceParent_sourceChild (from odometry pose)
        p = msg.pose.pose.position
        q = msg.pose.pose.orientation
        try:
            q_sc = _quat_normalize(float(q.x), float(q.y), float(q.z), float(q.w))
        except ValueError:
            self.get_logger().warn("Invalid odom quaternion; skip", throttle_duration_sec=1.0)
            return
        t_sp_sc = ((float(p.x), float(p.y), float(p.z)), q_sc)

        # T_sourceChild_baseLink (static offset)
        t_sc_bl = (self._offset_xyz, self._offset_q)

        # Compose: T_targetParent_baseLink = T_tp_sp ∘ T_sp_sc ∘ T_sc_bl
        xyz1, q1 = t_tp_sp
        xyz2, q2 = t_sp_sc
        xyz3, q3 = t_sc_bl

        xyz12, q12 = _compose(xyz1, q1, xyz2, q2)
        xyz_out, q_out = _compose(xyz12, q12, xyz3, q3)

        out = TransformStamped()
        out.header.stamp = msg.header.stamp
        out.header.frame_id = self._target_parent_frame
        out.child_frame_id = self._output_child_frame
        out.transform.translation.x = float(xyz_out[0])
        out.transform.translation.y = float(xyz_out[1])
        out.transform.translation.z = float(xyz_out[2])
        out.transform.rotation.x = float(q_out[0])
        out.transform.rotation.y = float(q_out[1])
        out.transform.rotation.z = float(q_out[2])
        out.transform.rotation.w = float(q_out[3])

        self._br.sendTransform(out)
        self._published_once = True


def main() -> None:
    rclpy.init()
    node = FixpositionOdomToBaseLinkTf()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
