#!/usr/bin/env python3

import math
from typing import Optional, Tuple

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
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


def _yaw_from_quaternion(x: float, y: float, z: float, w: float) -> float:
    return math.atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))


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


class FixpositionLocalTfFromOdom(Node):
    """Build a minimal TF tree from Fixposition odometry only.

    Subscribes to a nav_msgs/Odometry (default: /fixposition/odometry_enu).

    On the first odom message it computes a static transform map->fp_frame such that:
      - base_link is at the map origin at startup
      - optionally aligns yaw so initial base_link yaw is ~0

    It also publishes static map->odom identity.

    For every odom message it publishes dynamic odom->base_link.

    This is intended to avoid requiring Fixposition's own TF tree (ECEF/ENU/POI).
    It is compatible with `latlon_posearray_to_local` which needs TF map<-FP_ENU0.
    """

    def __init__(self) -> None:
        super().__init__("fixposition_local_tf_from_odom")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")

        self.declare_parameter("map_frame", "map")
        self.declare_parameter("odom_frame", "odom")
        self.declare_parameter("base_link_frame", "base_link")

        # Expected Fixposition ENU parent frame in the odom message
        self.declare_parameter("fp_frame", "FP_ENU0")
        # Expected Fixposition POI/body child frame in the odom message
        self.declare_parameter("fixposition_robot_frame", "FP_POI")

        # Offset from fixposition_robot_frame (FP_POI) to base_link
        self.declare_parameter("x", 0.0)
        self.declare_parameter("y", 0.0)
        self.declare_parameter("z", 0.0)
        self.declare_parameter("roll", 0.0)
        self.declare_parameter("pitch", 0.0)
        self.declare_parameter("yaw", 0.0)

        self.declare_parameter("align_yaw", False)

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._map_frame = str(self.get_parameter("map_frame").value).strip()
        self._odom_frame = str(self.get_parameter("odom_frame").value).strip()
        self._base_link_frame = str(self.get_parameter("base_link_frame").value).strip()
        self._fp_frame = str(self.get_parameter("fp_frame").value).strip()
        self._fp_robot_frame = str(self.get_parameter("fixposition_robot_frame").value).strip()

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

        self._static_br = StaticTransformBroadcaster(self)
        self._dyn_br = TransformBroadcaster(self)

        self._map_to_fp: Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]] = None
        self._static_published = False

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._cb, 50)
        self.get_logger().info(
            f"Subscribing {self._odom_topic}. Will publish TF: {self._map_frame}->{self._fp_frame} (static), "
            f"{self._map_frame}->{self._odom_frame} (static identity), {self._odom_frame}->{self._base_link_frame} (dynamic)"
        )

    def _compute_fp_to_base(self, msg: Odometry) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]]:
        parent = str(msg.header.frame_id).strip() if msg.header.frame_id else ""
        child = str(msg.child_frame_id).strip() if msg.child_frame_id else ""

        if self._fp_frame:
            parent = self._fp_frame
        if self._fp_robot_frame:
            child = self._fp_robot_frame

        if not parent or not child:
            self.get_logger().warn(
                f"Odometry without valid frames (frame_id='{parent}', child_frame_id='{child}'); skip",
                throttle_duration_sec=1.0,
            )
            return None

        p = msg.pose.pose.position
        q = msg.pose.pose.orientation
        try:
            q_fp_poi = _quat_normalize(float(q.x), float(q.y), float(q.z), float(q.w))
        except ValueError:
            self.get_logger().warn("Invalid odom quaternion; skip", throttle_duration_sec=1.0)
            return None

        t_fp_poi = ((float(p.x), float(p.y), float(p.z)), q_fp_poi)
        t_poi_bl = (self._offset_xyz, self._offset_q)
        xyz_fp_bl, q_fp_bl = _compose(t_fp_poi[0], t_fp_poi[1], t_poi_bl[0], t_poi_bl[1])
        return (xyz_fp_bl, q_fp_bl)

    def _publish_static_once(self, fp_to_base: Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]) -> None:
        if self._static_published:
            return

        xyz_fp_bl, q_fp_bl = fp_to_base

        if self._align_yaw:
            yaw0 = _yaw_from_quaternion(q_fp_bl[0], q_fp_bl[1], q_fp_bl[2], q_fp_bl[3])
            q_map_fp = _quat_from_yaw(-yaw0)
            rx, ry, rz = _quat_rotate_vector(q_map_fp, xyz_fp_bl)
            xyz_map_fp = (-rx, -ry, -rz)
        else:
            xyz_map_fp, q_map_fp = _inverse(xyz_fp_bl, q_fp_bl)

        self._map_to_fp = (xyz_map_fp, q_map_fp)

        now = self.get_clock().now().to_msg()

        tf_map_fp = TransformStamped()
        tf_map_fp.header.stamp = now
        tf_map_fp.header.frame_id = self._map_frame
        tf_map_fp.child_frame_id = self._fp_frame
        tf_map_fp.transform.translation.x = float(xyz_map_fp[0])
        tf_map_fp.transform.translation.y = float(xyz_map_fp[1])
        tf_map_fp.transform.translation.z = float(xyz_map_fp[2])
        tf_map_fp.transform.rotation.x = float(q_map_fp[0])
        tf_map_fp.transform.rotation.y = float(q_map_fp[1])
        tf_map_fp.transform.rotation.z = float(q_map_fp[2])
        tf_map_fp.transform.rotation.w = float(q_map_fp[3])

        tf_map_odom = TransformStamped()
        tf_map_odom.header.stamp = now
        tf_map_odom.header.frame_id = self._map_frame
        tf_map_odom.child_frame_id = self._odom_frame
        tf_map_odom.transform.rotation.w = 1.0

        self._static_br.sendTransform([tf_map_fp, tf_map_odom])
        self._static_published = True

        self.get_logger().info(
            f"Published static TF {self._map_frame}->{self._fp_frame} and identity {self._map_frame}->{self._odom_frame} "
            f"(align_yaw={self._align_yaw})"
        )

    def _cb(self, msg: Odometry) -> None:
        fp_to_base = self._compute_fp_to_base(msg)
        if fp_to_base is None:
            return

        self._publish_static_once(fp_to_base)

        # Compute odom->base_link using map->fp and fp->base
        # map->odom is identity, so odom and map coincide.
        if self._map_to_fp is None:
            return

        xyz_map_fp, q_map_fp = self._map_to_fp
        xyz_fp_bl, q_fp_bl = fp_to_base

        xyz_map_bl, q_map_bl = _compose(xyz_map_fp, q_map_fp, xyz_fp_bl, q_fp_bl)

        out = TransformStamped()
        out.header.stamp = msg.header.stamp
        out.header.frame_id = self._odom_frame
        out.child_frame_id = self._base_link_frame
        out.transform.translation.x = float(xyz_map_bl[0])
        out.transform.translation.y = float(xyz_map_bl[1])
        out.transform.translation.z = float(xyz_map_bl[2])
        out.transform.rotation.x = float(q_map_bl[0])
        out.transform.rotation.y = float(q_map_bl[1])
        out.transform.rotation.z = float(q_map_bl[2])
        out.transform.rotation.w = float(q_map_bl[3])

        self._dyn_br.sendTransform(out)


def main() -> None:
    rclpy.init()
    node = FixpositionLocalTfFromOdom()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
