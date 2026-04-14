import math

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
from tf2_ros import Buffer, TransformListener


class FixpositionMapAnchor(Node):
    def __init__(self) -> None:
        super().__init__("fixposition_map_anchor")

        self.declare_parameter("odom_topic", "/fixposition/odometry_enu")
        self.declare_parameter("map_frame", "map")
        self.declare_parameter("local_fixed_frame", "FP_ENU0")
        self.declare_parameter("global_frame", "ECEF")
        self.declare_parameter("align_yaw", False)
        self.declare_parameter("publish_rate_hz", 10.0)
        self.declare_parameter("publish_identity_until_anchored", True)
        self.declare_parameter(
            "allow_tfless_anchor",
            True,
        )

        self._odom_topic = str(self.get_parameter("odom_topic").value)
        self._map_frame = str(self.get_parameter("map_frame").value)
        self._local_fixed_frame = str(self.get_parameter("local_fixed_frame").value)
        self._global_frame = str(self.get_parameter("global_frame").value)
        self._align_yaw = bool(self.get_parameter("align_yaw").value)
        self._publish_rate_hz = float(self.get_parameter("publish_rate_hz").value)
        self._publish_identity_until_anchored = bool(self.get_parameter("publish_identity_until_anchored").value)
        self._allow_tfless_anchor = bool(self.get_parameter("allow_tfless_anchor").value)

        self._anchored = False
        self._broadcaster = TransformBroadcaster(self)
        self._current_transform = self._identity_transform()

        self._tf_buffer = Buffer()
        self._tf_listener = TransformListener(self._tf_buffer, self)

        self._sub = self.create_subscription(Odometry, self._odom_topic, self._on_odom, 10)
        self._timer = self.create_timer(1.0 / max(self._publish_rate_hz, 0.1), self._publish_tf)
        self.get_logger().info(
            f"Waiting first odom on {self._odom_topic} to anchor {self._global_frame}->{self._map_frame}"
        )

    def _identity_transform(self) -> TransformStamped:
        ts = TransformStamped()
        ts.header.frame_id = self._global_frame
        ts.child_frame_id = self._map_frame
        ts.transform.translation.x = 0.0
        ts.transform.translation.y = 0.0
        ts.transform.translation.z = 0.0
        ts.transform.rotation.x = 0.0
        ts.transform.rotation.y = 0.0
        ts.transform.rotation.z = 0.0
        ts.transform.rotation.w = 1.0
        return ts

    def _publish_tf(self) -> None:
        if not self._anchored and not self._publish_identity_until_anchored:
            return
        self._current_transform.header.stamp = self.get_clock().now().to_msg()
        self._broadcaster.sendTransform(self._current_transform)

    @staticmethod
    def _yaw_from_quaternion(x: float, y: float, z: float, w: float) -> float:
        # Standard yaw (Z axis) from quaternion (x,y,z,w)
        # yaw = atan2(2(wz + xy), 1 - 2(y^2 + z^2))
        return math.atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))

    @staticmethod
    def _quat_from_yaw(yaw: float) -> tuple[float, float, float, float]:
        half = 0.5 * yaw
        return (0.0, 0.0, math.sin(half), math.cos(half))

    @staticmethod
    def _quat_multiply(a: tuple[float, float, float, float], b: tuple[float, float, float, float]) -> tuple[float, float, float, float]:
        ax, ay, az, aw = a
        bx, by, bz, bw = b
        # (x,y,z,w) Hamilton product
        x = aw * bx + ax * bw + ay * bz - az * by
        y = aw * by - ax * bz + ay * bw + az * bx
        z = aw * bz + ax * by - ay * bx + az * bw
        w = aw * bw - ax * bx - ay * by - az * bz
        return (x, y, z, w)

    @staticmethod
    def _quat_rotate_vector(q: tuple[float, float, float, float], v: tuple[float, float, float]) -> tuple[float, float, float]:
        # Rotate vector v by quaternion q (x,y,z,w)
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

    def _on_odom(self, msg: Odometry) -> None:
        if self._anchored:
            return

        if msg.header.frame_id and msg.header.frame_id != self._local_fixed_frame:
            self.get_logger().warn(
                f"Odometry frame_id is '{msg.header.frame_id}', expected '{self._local_fixed_frame}'. Anchoring anyway."
            )

        p = msg.pose.pose.position
        q = msg.pose.pose.orientation

        yaw0 = self._yaw_from_quaternion(q.x, q.y, q.z, q.w) if self._align_yaw else 0.0

        tf_g_l = None

        # Fast path: if global_frame == local_fixed_frame, global->local_fixed is identity.
        if self._global_frame == self._local_fixed_frame:
            tf_g_l = TransformStamped()
            tf_g_l.header.frame_id = self._global_frame
            tf_g_l.child_frame_id = self._local_fixed_frame
            tf_g_l.transform.rotation.w = 1.0

        # Otherwise, try to get global->local_fixed from the existing Fixposition TF tree.
        if tf_g_l is None:
            try:
                tf_g_l = self._tf_buffer.lookup_transform(
                    self._global_frame,
                    self._local_fixed_frame,
                    rclpy.time.Time(),
                    timeout=rclpy.duration.Duration(seconds=1.0),
                )
            except Exception as exc:  # noqa: BLE001
                if not self._allow_tfless_anchor:
                    self.get_logger().warn(
                        f"TF lookup failed for {self._global_frame}->{self._local_fixed_frame} (will retry): {exc}"
                    )
                    return

                # TF-less fallback: anchor using the odometry frame directly.
                # This keeps everything connected in a single tree even when Fixposition TF is not being published.
                odom_frame = str(msg.header.frame_id) if msg.header.frame_id else ""
                if not odom_frame:
                    self.get_logger().warn(
                        f"TF lookup failed for {self._global_frame}->{self._local_fixed_frame} and odom header.frame_id is empty; cannot anchor yet: {exc}"
                    )
                    return

                if odom_frame not in (self._global_frame, self._local_fixed_frame):
                    self.get_logger().warn(
                        f"TF lookup failed for {self._global_frame}->{self._local_fixed_frame}. "
                        f"Using odom header.frame_id '{odom_frame}' as anchor frame. (original error: {exc})"
                    )
                else:
                    self.get_logger().warn(
                        f"TF lookup failed for {self._global_frame}->{self._local_fixed_frame}. "
                        f"Falling back to TF-less anchoring in '{odom_frame}'. (original error: {exc})"
                    )

                self._global_frame = odom_frame
                self._local_fixed_frame = odom_frame
                tf_g_l = TransformStamped()
                tf_g_l.header.frame_id = odom_frame
                tf_g_l.child_frame_id = odom_frame
                tf_g_l.transform.rotation.w = 1.0

        # Define local_fixed->map as: translate by p0, rotate by yaw0 (so map origin = robot start)
        # Then global->map = (global->local_fixed) ∘ (local_fixed->map)
        t_gl = tf_g_l.transform.translation
        r_gl = tf_g_l.transform.rotation
        q_gl = (float(r_gl.x), float(r_gl.y), float(r_gl.z), float(r_gl.w))

        q_lm = self._quat_from_yaw(yaw0)  # rotation local_fixed -> map
        # translation local_fixed -> map is p0 in local_fixed coords
        tx_lm, ty_lm, tz_lm = float(p.x), float(p.y), float(p.z)
        rx, ry, rz = self._quat_rotate_vector(q_gl, (tx_lm, ty_lm, tz_lm))

        ts = TransformStamped()
        ts.header.frame_id = self._global_frame
        ts.child_frame_id = self._map_frame
        ts.transform.translation.x = float(t_gl.x) + float(rx)
        ts.transform.translation.y = float(t_gl.y) + float(ry)
        ts.transform.translation.z = float(t_gl.z) + float(rz)

        q_gm = self._quat_multiply(q_gl, q_lm)
        ts.transform.rotation.x = float(q_gm[0])
        ts.transform.rotation.y = float(q_gm[1])
        ts.transform.rotation.z = float(q_gm[2])
        ts.transform.rotation.w = float(q_gm[3])

        self._current_transform = ts
        self._anchored = True
        self.get_logger().info(
            f"Anchored {self._global_frame}->{self._map_frame} using startup pose in {self._local_fixed_frame}: "
            f"p=({p.x:.3f},{p.y:.3f},{p.z:.3f}) yaw={yaw0:.3f} rad"
        )

        self.destroy_subscription(self._sub)


def main() -> None:
    rclpy.init()
    node = FixpositionMapAnchor()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
