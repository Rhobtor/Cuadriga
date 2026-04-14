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

    # ROS convention: R = Rz(yaw) * Ry(pitch) * Rx(roll)
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


class FixpositionTfTreeForZed(Node):
    """Publish a minimal TF tree for Fixposition + ZED mounting.

    Inputs:
      - nav_msgs/Odometry from Fixposition (default: /fixposition/odometry_enu)

    Outputs:
      - static: map->fp_frame (anchors startup pose at map origin)
      - static: map->odom identity
      - dynamic: odom->base_link (from Fixposition odometry + optional rigid offset)
      - optional static mounting:
          base_link->camera_link
          camera_link->zed_camera_link

    This is meant to be used with ZED wrapper with publish_tf:=false, to avoid TF conflicts.
    """

    def __init__(self) -> None:
        super().__init__("fixposition_tf_tree_for_zed")

        # Core frames
        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("map_frame", "map")
        self.declare_parameter("odom_frame", "odom")
        self.declare_parameter("base_link_frame", "base_link")

        # Expected Fixposition frames in odometry
        self.declare_parameter("fp_frame", "FP_ENU0")
        self.declare_parameter("fixposition_robot_frame", "FP_POI")

        # Which frame to use for the initial anchoring (map origin).
        # - "fixposition_robot_frame": anchor so FP_POI starts at map origin
        # - "base_link_frame": anchor so base_link starts at map origin
        self.declare_parameter("anchor_on", "fixposition_robot_frame")

        # TF stamping policy:
        # - False (default): stamp dynamic TF with now() to avoid TF_OLD_DATA when upstream stamps are off
        # - True: stamp with incoming odometry header stamp
        self.declare_parameter("use_msg_stamp", False)

        # Offset from Fixposition robot frame to base_link
        self.declare_parameter("x", 0.0)
        self.declare_parameter("y", 0.0)
        self.declare_parameter("z", 0.0)
        self.declare_parameter("roll", 0.0)
        self.declare_parameter("pitch", 0.0)
        self.declare_parameter("yaw", 0.0)

        # Publish base_link -> fixposition_robot_frame (e.g. base_link -> FP_POI) as a static TF.
        # WARNING: If Fixposition is already publishing FP_ENU0->FP_POI, enabling this will create a TF conflict
        # (two parents for FP_POI). Keep False unless you know FP_POI is not published elsewhere.
        self.declare_parameter("publish_base_to_fixposition_robot_tf", False)

        self.declare_parameter("align_yaw", False)

        # Mounting TFs
        self.declare_parameter("publish_base_camera_tf", True)
        self.declare_parameter("camera_parent_frame", "base_link")
        self.declare_parameter("camera_frame", "camera_link")
        self.declare_parameter("camera_x", 0.9206628)
        self.declare_parameter("camera_y", 0.0075201)
        self.declare_parameter("camera_z", 0.75542)
        self.declare_parameter("camera_roll", 0.0)
        self.declare_parameter("camera_pitch", 0.0)
        self.declare_parameter("camera_yaw", 0.0)

        self.declare_parameter("publish_camera_to_zed_link_tf", True)
        self.declare_parameter("zed_link_parent_frame", "camera_link")
        self.declare_parameter("zed_link_frame", "zed_camera_link")
        self.declare_parameter("zed_link_x", 0.0)
        self.declare_parameter("zed_link_y", 0.0)
        self.declare_parameter("zed_link_z", 0.04)
        self.declare_parameter("zed_link_roll", 0.0)
        self.declare_parameter("zed_link_pitch", 0.0)
        self.declare_parameter("zed_link_yaw", 0.0)

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._map_frame = str(self.get_parameter("map_frame").value).strip()
        self._odom_frame = str(self.get_parameter("odom_frame").value).strip()
        self._base_link_frame = str(self.get_parameter("base_link_frame").value).strip()
        self._fp_frame = str(self.get_parameter("fp_frame").value).strip()
        self._fp_robot_frame = str(self.get_parameter("fixposition_robot_frame").value).strip()
        self._anchor_on = str(self.get_parameter("anchor_on").value).strip()
        self._use_msg_stamp = bool(self.get_parameter("use_msg_stamp").value)

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

        self._fp_to_map: Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]] = None
        self._static_published = False

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._cb, 50)

        self._publish_mounting_once()

        self.get_logger().info(
            f"Subscribing {self._odom_topic}. Will publish TF: {self._fp_frame}->{self._map_frame} (static), "
            f"{self._map_frame}->{self._odom_frame} (static identity), {self._odom_frame}->{self._base_link_frame} (dynamic)."
        )

        if self._anchor_on not in ("fixposition_robot_frame", "base_link_frame"):
            self.get_logger().warn(
                f"anchor_on='{self._anchor_on}' not recognized; using 'fixposition_robot_frame'"
            )
            self._anchor_on = "fixposition_robot_frame"

    def _publish_mounting_once(self) -> None:
        now = self.get_clock().now().to_msg()
        tfs = []

        if bool(self.get_parameter("publish_base_to_fixposition_robot_tf").value):
            if self._base_link_frame and self._fp_robot_frame:
                # We store POI->base_link as offset; publish base_link->POI.
                (xyz_bl_poi, q_bl_poi) = _inverse(self._offset_xyz, self._offset_q)

                tf = TransformStamped()
                tf.header.stamp = now
                tf.header.frame_id = self._base_link_frame
                tf.child_frame_id = self._fp_robot_frame
                tf.transform.translation.x = float(xyz_bl_poi[0])
                tf.transform.translation.y = float(xyz_bl_poi[1])
                tf.transform.translation.z = float(xyz_bl_poi[2])
                tf.transform.rotation.x = float(q_bl_poi[0])
                tf.transform.rotation.y = float(q_bl_poi[1])
                tf.transform.rotation.z = float(q_bl_poi[2])
                tf.transform.rotation.w = float(q_bl_poi[3])
                tfs.append(tf)

        if bool(self.get_parameter("publish_base_camera_tf").value):
            parent = str(self.get_parameter("camera_parent_frame").value).strip()
            child = str(self.get_parameter("camera_frame").value).strip()
            if parent and child:
                q = _quat_from_rpy(
                    float(self.get_parameter("camera_roll").value),
                    float(self.get_parameter("camera_pitch").value),
                    float(self.get_parameter("camera_yaw").value),
                )
                tf = TransformStamped()
                tf.header.stamp = now
                tf.header.frame_id = parent
                tf.child_frame_id = child
                tf.transform.translation.x = float(self.get_parameter("camera_x").value)
                tf.transform.translation.y = float(self.get_parameter("camera_y").value)
                tf.transform.translation.z = float(self.get_parameter("camera_z").value)
                tf.transform.rotation.x = float(q[0])
                tf.transform.rotation.y = float(q[1])
                tf.transform.rotation.z = float(q[2])
                tf.transform.rotation.w = float(q[3])
                tfs.append(tf)

        if bool(self.get_parameter("publish_camera_to_zed_link_tf").value):
            parent = str(self.get_parameter("zed_link_parent_frame").value).strip()
            child = str(self.get_parameter("zed_link_frame").value).strip()
            if parent and child:
                q = _quat_from_rpy(
                    float(self.get_parameter("zed_link_roll").value),
                    float(self.get_parameter("zed_link_pitch").value),
                    float(self.get_parameter("zed_link_yaw").value),
                )
                tf = TransformStamped()
                tf.header.stamp = now
                tf.header.frame_id = parent
                tf.child_frame_id = child
                tf.transform.translation.x = float(self.get_parameter("zed_link_x").value)
                tf.transform.translation.y = float(self.get_parameter("zed_link_y").value)
                tf.transform.translation.z = float(self.get_parameter("zed_link_z").value)
                tf.transform.rotation.x = float(q[0])
                tf.transform.rotation.y = float(q[1])
                tf.transform.rotation.z = float(q[2])
                tf.transform.rotation.w = float(q[3])
                tfs.append(tf)

        if tfs:
            self._static_br.sendTransform(tfs)

    def _compute_fp_to_poi(self, msg: Odometry) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]]:
        if not self._fp_frame or not self._fp_robot_frame:
            self.get_logger().error("fp_frame/fixposition_robot_frame are empty")
            return None
        p = msg.pose.pose.position
        q = msg.pose.pose.orientation
        try:
            q_fp_poi = _quat_normalize(float(q.x), float(q.y), float(q.z), float(q.w))
        except ValueError:
            self.get_logger().warn("Invalid odom quaternion; skip", throttle_duration_sec=1.0)
            return None

        return ((float(p.x), float(p.y), float(p.z)), q_fp_poi)

    def _compute_fp_to_base(self, fp_to_poi: Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]) -> Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]:
        t_poi_bl = (self._offset_xyz, self._offset_q)
        xyz_fp_bl, q_fp_bl = _compose(fp_to_poi[0], fp_to_poi[1], t_poi_bl[0], t_poi_bl[1])
        return (xyz_fp_bl, q_fp_bl)

    def _publish_static_once(
        self,
        fp_to_poi: Tuple[Tuple[float, float, float], Tuple[float, float, float, float]],
        fp_to_base: Tuple[Tuple[float, float, float], Tuple[float, float, float, float]],
    ) -> None:
        if self._static_published:
            return

        if self._anchor_on == "base_link_frame":
            anchor_xyz, anchor_q = fp_to_base
        else:
            anchor_xyz, anchor_q = fp_to_poi

        if self._align_yaw:
            yaw0 = _yaw_from_quaternion(anchor_q[0], anchor_q[1], anchor_q[2], anchor_q[3])
            # Define fp->map as: translate to anchor pose, but rotate so map yaw is zero at startup.
            q_fp_map = _quat_from_yaw(yaw0)
            xyz_fp_map = anchor_xyz
        else:
            q_fp_map = anchor_q
            xyz_fp_map = anchor_xyz

        self._fp_to_map = (xyz_fp_map, q_fp_map)

        now = self.get_clock().now().to_msg()

        tf_fp_map = TransformStamped()
        tf_fp_map.header.stamp = now
        tf_fp_map.header.frame_id = self._fp_frame
        tf_fp_map.child_frame_id = self._map_frame
        tf_fp_map.transform.translation.x = float(xyz_fp_map[0])
        tf_fp_map.transform.translation.y = float(xyz_fp_map[1])
        tf_fp_map.transform.translation.z = float(xyz_fp_map[2])
        tf_fp_map.transform.rotation.x = float(q_fp_map[0])
        tf_fp_map.transform.rotation.y = float(q_fp_map[1])
        tf_fp_map.transform.rotation.z = float(q_fp_map[2])
        tf_fp_map.transform.rotation.w = float(q_fp_map[3])

        tf_map_odom = TransformStamped()
        tf_map_odom.header.stamp = now
        tf_map_odom.header.frame_id = self._map_frame
        tf_map_odom.child_frame_id = self._odom_frame
        tf_map_odom.transform.rotation.w = 1.0

        self._static_br.sendTransform([tf_fp_map, tf_map_odom])
        self._static_published = True

        self.get_logger().info(
            f"Published static TF {self._fp_frame}->{self._map_frame} and identity {self._map_frame}->{self._odom_frame} "
            f"(align_yaw={self._align_yaw}, anchor_on={self._anchor_on})"
        )

    def _cb(self, msg: Odometry) -> None:
        fp_to_poi = self._compute_fp_to_poi(msg)
        if fp_to_poi is None:
            return

        fp_to_base = self._compute_fp_to_base(fp_to_poi)

        self._publish_static_once(fp_to_poi, fp_to_base)

        if self._fp_to_map is None:
            return

        xyz_fp_map, q_fp_map = self._fp_to_map
        xyz_fp_bl, q_fp_bl = fp_to_base

        # map->base_link = inv(fp->map) ∘ (fp->base_link)
        (xyz_map_fp, q_map_fp) = _inverse(xyz_fp_map, q_fp_map)
        xyz_map_bl, q_map_bl = _compose(xyz_map_fp, q_map_fp, xyz_fp_bl, q_fp_bl)

        out = TransformStamped()
        out.header.stamp = msg.header.stamp if self._use_msg_stamp else self.get_clock().now().to_msg()
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
    node = FixpositionTfTreeForZed()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
