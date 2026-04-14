#!/usr/bin/env python3

import math
from typing import Optional, Tuple

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import PoseArray, Pose
from nav_msgs.msg import Odometry
from sensor_msgs.msg import NavSatFix

from tf2_ros import Buffer, TransformListener


def _deg2rad(deg: float) -> float:
    return deg * math.pi / 180.0


def _quat_conj(q: Tuple[float, float, float, float]) -> Tuple[float, float, float, float]:
    x, y, z, w = q
    return (-x, -y, -z, w)


def _quat_mul(a: Tuple[float, float, float, float], b: Tuple[float, float, float, float]) -> Tuple[float, float, float, float]:
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    return (
        aw * bx + ax * bw + ay * bz - az * by,
        aw * by - ax * bz + ay * bw + az * bx,
        aw * bz + ax * by - ay * bx + az * bw,
        aw * bw - ax * bx - ay * by - az * bz,
    )


def _quat_rotate_vec(q: Tuple[float, float, float, float], v: Tuple[float, float, float]) -> Tuple[float, float, float]:
    # Rotate vector v by quaternion q: v' = q * (v,0) * conj(q)
    vx, vy, vz = v
    vq = (vx, vy, vz, 0.0)
    rq = _quat_mul(_quat_mul(q, vq), _quat_conj(q))
    return (rq[0], rq[1], rq[2])


class LatLonPoseArrayToLocal(Node):
    """Convert PoseArray goals expressed in lat/lon into local map coordinates.

    Input convention:
      - Each Pose in the input PoseArray encodes a GPS coordinate in degrees.
      - You can choose whether (x,y) is (lat,lon) or (lon,lat) via `input_order`.
      - Altitude can be optionally provided in `position.z` (meters).

    The node locks an origin when it has BOTH:
      - a GNSS fix (lat0, lon0, alt0)
            - a local pose of the robot from `odom_topic` (typically ZED odometry)

    Then for every incoming GPS goal it computes an ENU delta from (lat0, lon0)
    and adds it to the captured local origin position, producing a goal in `local_frame`.
    Finally it transforms it into `output_frame` (default: map) using TF (if enabled).

    Notes:
      - Uses an equirectangular approximation; accurate for local areas (typically < few km).
      - This is intended as a glue layer so your GUI can stay in lat/lon while MPPI stays local.
    """

    def __init__(self) -> None:
        super().__init__("latlon_posearray_to_local")

        self.declare_parameter("gnss_topic", "/fixposition/gnss1")
        self.declare_parameter("odom_topic", "/zed/zed_node/odom")
        self.declare_parameter("input_topic", "/gui/goal_latlon")
        self.declare_parameter("output_topic", "/goal_local")

        # How to choose the reference used to convert lat/lon to local coordinates.
        # - 'startup': lock origin once at startup (default)
        # - 'current_fix': use the latest GNSS+odom as reference for each incoming goal
        self.declare_parameter("reference_mode", "startup")

        # Frame in which the incoming odometry pose is expressed (the node treats it as the local metric frame).
        # With ZED, this is commonly 'odom'.
        self.declare_parameter("local_frame", "odom")
        # Backwards-compatible alias (older configs used Fixposition ENU frame names).
        self.declare_parameter("fp_frame", "")
        self.declare_parameter("output_frame", "map")

        # If False, publish goals directly in local_frame (no TF lookup needed).
        self.declare_parameter("use_tf", True)
        # If True and TF lookup fails, fall back to publishing in local_frame.
        self.declare_parameter("fallback_to_local_frame_on_tf_failure", True)

        # When TF is disabled (or unavailable), publish in output_frame by localizing using the
        # captured startup odometry as origin: output = (local - local_origin) optionally rotated by -yaw0.
        self.declare_parameter("publish_in_output_frame_without_tf", True)
        self.declare_parameter("fallback_to_output_frame_without_tf_on_tf_failure", True)
        self.declare_parameter("align_yaw", False)

        # 'lat_lon' => pose.position.x=lat, y=lon
        # 'lon_lat' => pose.position.x=lon, y=lat
        self.declare_parameter("input_order", "lat_lon")

        # Optional correction for ENU axis conventions.
        # Some setups effectively behave like the EN axes are rotated (e.g. +90deg),
        # making goals appear to the right when they should be in front.
        # Applied to the computed (east,north) delta before adding to the captured FP origin.
        self.declare_parameter("enu_yaw_offset_rad", 0.0)

        # ZED odom/map frames are often not aligned to ENU. Rotate ENU deltas into local frame.
        # You can provide a fixed yaw, and optionally estimate it from motion (startup mode).
        self.declare_parameter("enu_to_local_yaw_rad", 0.0)
        self.declare_parameter("estimate_enu_to_local_yaw", True)
        self.declare_parameter("yaw_estimation_min_distance_m", 1.0)

        self.declare_parameter("earth_radius_m", 6378137.0)
        self.declare_parameter("use_altitude", False)

        # Debug logs to verify frames and computed deltas.
        self.declare_parameter("debug", False)

        self._tf_buffer = Buffer()
        self._tf_listener = TransformListener(self._tf_buffer, self)

        self._latest_gnss: Optional[NavSatFix] = None
        self._latest_odom: Optional[Odometry] = None

        self._origin_latlon_alt: Optional[Tuple[float, float, float]] = None
        self._origin_local_xyz: Optional[Tuple[float, float, float]] = None
        self._origin_local_yaw: Optional[float] = None

        self._enu_to_local_yaw_est: Optional[float] = None

        gnss_topic = self.get_parameter("gnss_topic").get_parameter_value().string_value
        odom_topic = self.get_parameter("odom_topic").get_parameter_value().string_value
        input_topic = self.get_parameter("input_topic").get_parameter_value().string_value
        output_topic = self.get_parameter("output_topic").get_parameter_value().string_value

        self.create_subscription(NavSatFix, gnss_topic, self._on_gnss, 10)
        self.create_subscription(Odometry, odom_topic, self._on_odom, 10)
        self.create_subscription(PoseArray, input_topic, self._on_goal, 10)

        self._pub = self.create_publisher(PoseArray, output_topic, 10)

        self.get_logger().info(
            "latlon_posearray_to_local running. Waiting for GNSS + odometry to lock origin."
        )

    def _on_gnss(self, msg: NavSatFix) -> None:
        # Keep last fix even if status is not great; user can gate upstream.
        self._latest_gnss = msg
        self._maybe_lock_origin()
        self._update_enu_to_local_yaw_estimate()

    def _on_odom(self, msg: Odometry) -> None:
        self._latest_odom = msg
        self._maybe_lock_origin()
        self._update_enu_to_local_yaw_estimate()

    def _maybe_lock_origin(self) -> None:
        reference_mode = self.get_parameter("reference_mode").get_parameter_value().string_value
        if reference_mode.strip().lower() == "current_fix":
            # In current_fix mode we don't lock; origin is taken from latest messages per-goal.
            return

        if self._origin_latlon_alt is not None and self._origin_local_xyz is not None:
            return
        if self._latest_gnss is None or self._latest_odom is None:
            return

        lat0 = float(self._latest_gnss.latitude)
        lon0 = float(self._latest_gnss.longitude)
        alt0 = float(self._latest_gnss.altitude)
        p = self._latest_odom.pose.pose.position
        q = self._latest_odom.pose.pose.orientation
        self._origin_latlon_alt = (lat0, lon0, alt0)
        self._origin_local_xyz = (float(p.x), float(p.y), float(p.z))

        # Cache startup yaw for the no-TF localization mode.
        # yaw = atan2(2(wz + xy), 1 - 2(y^2 + z^2))
        self._origin_local_yaw = math.atan2(
            2.0 * (float(q.w) * float(q.z) + float(q.x) * float(q.y)),
            1.0 - 2.0 * (float(q.y) * float(q.y) + float(q.z) * float(q.z)),
        )

        self.get_logger().info(
            f"Origin locked: lat0={lat0:.8f} lon0={lon0:.8f} alt0={alt0:.3f} | "
            f"local_origin=({p.x:.3f},{p.y:.3f},{p.z:.3f})"
        )

    def _get_reference_origin(self) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float], float]]:
        """Return (latlonalt, local_xyz, local_yaw) used as reference origin."""

        reference_mode = self.get_parameter("reference_mode").get_parameter_value().string_value
        reference_mode = reference_mode.strip().lower()

        if reference_mode == "current_fix":
            if self._latest_gnss is None or self._latest_odom is None:
                return None

            lat0 = float(self._latest_gnss.latitude)
            lon0 = float(self._latest_gnss.longitude)
            alt0 = float(self._latest_gnss.altitude)
            p = self._latest_odom.pose.pose.position
            q = self._latest_odom.pose.pose.orientation
            yaw0 = math.atan2(
                2.0 * (float(q.w) * float(q.z) + float(q.x) * float(q.y)),
                1.0 - 2.0 * (float(q.y) * float(q.y) + float(q.z) * float(q.z)),
            )
            return ((lat0, lon0, alt0), (float(p.x), float(p.y), float(p.z)), float(yaw0))

        # startup (locked) mode
        if self._origin_latlon_alt is None or self._origin_local_xyz is None:
            return None
        yaw0 = float(self._origin_local_yaw) if self._origin_local_yaw is not None else 0.0
        return (self._origin_latlon_alt, self._origin_local_xyz, yaw0)

    def _update_enu_to_local_yaw_estimate(self) -> None:
        # Only meaningful when we have a fixed, locked origin.
        reference_mode = self.get_parameter("reference_mode").get_parameter_value().string_value
        if reference_mode.strip().lower() == "current_fix":
            return
        if not bool(self.get_parameter("estimate_enu_to_local_yaw").get_parameter_value().bool_value):
            return
        if self._origin_latlon_alt is None or self._origin_local_xyz is None:
            return
        if self._latest_gnss is None or self._latest_odom is None:
            return

        # ENU displacement from locked origin to current GNSS
        lat = float(self._latest_gnss.latitude)
        lon = float(self._latest_gnss.longitude)
        alt = float(self._latest_gnss.altitude)
        d_e, d_n, _ = self._latlon_to_enu_delta(lat, lon, alt)

        # Local displacement from locked origin to current odom
        p = self._latest_odom.pose.pose.position
        dx = float(p.x) - float(self._origin_local_xyz[0])
        dy = float(p.y) - float(self._origin_local_xyz[1])

        dist_enu = math.hypot(d_e, d_n)
        dist_local = math.hypot(dx, dy)
        min_dist = float(self.get_parameter("yaw_estimation_min_distance_m").get_parameter_value().double_value)
        if dist_enu < min_dist or dist_local < min_dist:
            return

        theta_enu = math.atan2(d_n, d_e)
        theta_local = math.atan2(dy, dx)
        self._enu_to_local_yaw_est = theta_local - theta_enu

    def _rotate_enu_delta_to_local(self, d_e: float, d_n: float) -> Tuple[float, float]:
        yaw = float(self.get_parameter("enu_to_local_yaw_rad").get_parameter_value().double_value)
        if self._enu_to_local_yaw_est is not None:
            yaw += float(self._enu_to_local_yaw_est)

        if abs(yaw) < 1e-12:
            return (d_e, d_n)

        c = math.cos(yaw)
        s = math.sin(yaw)
        # (east,north) rotated into local (x,y)
        x = c * d_e - s * d_n
        y = s * d_e + c * d_n
        return (x, y)

    def _local_point_to_output_without_tf(self, x_local: float, y_local: float, z_local: float) -> Tuple[float, float, float]:
        assert self._origin_local_xyz is not None

        ox, oy, oz = self._origin_local_xyz
        dx = float(x_local - ox)
        dy = float(y_local - oy)
        dz = float(z_local - oz)

        align_yaw = bool(self.get_parameter("align_yaw").get_parameter_value().bool_value)
        if align_yaw and self._origin_local_yaw is not None:
            yaw0 = float(self._origin_local_yaw)
            c = math.cos(yaw0)
            s = math.sin(yaw0)
            # rotate by -yaw0
            x_out = c * dx + s * dy
            y_out = -s * dx + c * dy
            return (x_out, y_out, dz)

        return (dx, dy, dz)

    def _parse_latlon(self, pose: Pose) -> Tuple[float, float, float]:
        order = self.get_parameter("input_order").get_parameter_value().string_value
        if order not in {"lat_lon", "lon_lat"}:
            order = "lat_lon"

        x = float(pose.position.x)
        y = float(pose.position.y)
        z = float(pose.position.z)

        if order == "lat_lon":
            lat_deg, lon_deg = x, y
        else:
            lon_deg, lat_deg = x, y

        return (lat_deg, lon_deg, z)

    def _latlon_to_enu_delta(self, lat_deg: float, lon_deg: float, alt_m: float) -> Tuple[float, float, float]:
        assert self._origin_latlon_alt is not None
        lat0_deg, lon0_deg, alt0_m = self._origin_latlon_alt

        r = float(self.get_parameter("earth_radius_m").get_parameter_value().double_value)

        lat = _deg2rad(lat_deg)
        lon = _deg2rad(lon_deg)
        lat0 = _deg2rad(lat0_deg)
        lon0 = _deg2rad(lon0_deg)

        d_lat = lat - lat0
        d_lon = lon - lon0

        d_north = d_lat * r
        d_east = d_lon * r * math.cos(lat0)

        # Optional fixed rotation in the horizontal EN plane.
        # Vector is expressed as (east, north).
        yaw_off = float(self.get_parameter("enu_yaw_offset_rad").get_parameter_value().double_value)
        if abs(yaw_off) > 1e-12:
            c = math.cos(yaw_off)
            s = math.sin(yaw_off)
            e2 = c * d_east - s * d_north
            n2 = s * d_east + c * d_north
            d_east, d_north = e2, n2

        use_alt = bool(self.get_parameter("use_altitude").get_parameter_value().bool_value)
        d_up = (alt_m - alt0_m) if use_alt else 0.0

        return (d_east, d_north, d_up)

    def _lookup_transform_latest(
        self, src_frame: str, dst_frame: str
    ) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]]:
        if src_frame == dst_frame:
            return ((0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0))

        try:
            tf = self._tf_buffer.lookup_transform(dst_frame, src_frame, rclpy.time.Time())
        except Exception as ex:  # noqa: BLE001
            self.get_logger().warn(
                f"TF lookup failed {src_frame}->{dst_frame}: {ex}",
                throttle_duration_sec=1.0,
            )
            return None

        t = tf.transform.translation
        q = tf.transform.rotation
        return ((float(t.x), float(t.y), float(t.z)), (float(q.x), float(q.y), float(q.z), float(q.w)))

    @staticmethod
    def _apply_transform(
        tf: Tuple[Tuple[float, float, float], Tuple[float, float, float, float]],
        x: float,
        y: float,
        z: float,
    ) -> Tuple[float, float, float]:
        (tx, ty, tz), qxyzw = tf
        rx, ry, rz = _quat_rotate_vec(qxyzw, (x, y, z))
        return (rx + tx, ry + ty, rz + tz)

    def _on_goal(self, msg: PoseArray) -> None:
        ref = self._get_reference_origin()
        if ref is None:
            self.get_logger().warn("Reference not ready yet (need GNSS + odometry). Ignoring goal.")
            return

        (lat0, lon0, alt0), (origin_x, origin_y, origin_z), origin_yaw = ref

        # Ensure helpers use the selected reference.
        self._origin_latlon_alt = (float(lat0), float(lon0), float(alt0))
        self._origin_local_xyz = (float(origin_x), float(origin_y), float(origin_z))
        self._origin_local_yaw = float(origin_yaw)

        fp_frame = self.get_parameter("fp_frame").get_parameter_value().string_value
        local_frame = self.get_parameter("local_frame").get_parameter_value().string_value
        # If user still sets fp_frame, treat it as an override.
        if fp_frame.strip():
            local_frame = fp_frame.strip()

        out_frame = self.get_parameter("output_frame").get_parameter_value().string_value
        use_tf = bool(self.get_parameter("use_tf").get_parameter_value().bool_value)
        fallback_to_local = bool(
            self.get_parameter("fallback_to_local_frame_on_tf_failure").get_parameter_value().bool_value
        )

        publish_in_output_without_tf = bool(
            self.get_parameter("publish_in_output_frame_without_tf").get_parameter_value().bool_value
        )
        fallback_to_output_without_tf = bool(
            self.get_parameter("fallback_to_output_frame_without_tf_on_tf_failure").get_parameter_value().bool_value
        )

        origin_x, origin_y, origin_z = self._origin_local_xyz

        # Decide publishing frame and (optional) TF.
        # - If TF is disabled: default to publishing in output_frame using odom-localization.
        # - If TF is enabled but lookup fails: optionally fall back to odom-localization in output_frame.
        tf_local_to_out = None
        if not use_tf:
            publish_frame = out_frame if publish_in_output_without_tf else local_frame
        elif local_frame == out_frame:
            publish_frame = local_frame
        else:
            publish_frame = out_frame
            tf_local_to_out = self._lookup_transform_latest(local_frame, out_frame)
            if tf_local_to_out is None:
                if fallback_to_output_without_tf:
                    publish_frame = out_frame
                elif fallback_to_local:
                    publish_frame = local_frame

        out = PoseArray()
        out.header.stamp = self.get_clock().now().to_msg()
        out.header.frame_id = publish_frame

        debug = bool(self.get_parameter("debug").get_parameter_value().bool_value)
        if debug:
            self.get_logger().info(
                (
                    f"Publishing {len(msg.poses)} goal(s) in frame_id='{publish_frame}' "
                    f"(local_frame='{local_frame}', output_frame='{out_frame}', use_tf={use_tf}, "
                    f"publish_in_output_frame_without_tf={publish_in_output_without_tf})"
                ),
                throttle_duration_sec=1.0,
            )

        for p in msg.poses:
            lat_deg, lon_deg, alt_m = self._parse_latlon(p)
            d_e, d_n, d_u = self._latlon_to_enu_delta(lat_deg, lon_deg, alt_m)

            # ENU delta -> local delta (ZED frames are often rotated vs ENU)
            d_x, d_y = self._rotate_enu_delta_to_local(d_e, d_n)

            if debug:
                self.get_logger().info(
                    (
                        f"Goal lat/lon=({lat_deg:.8f},{lon_deg:.8f}) ENU(dE={d_e:.3f}, dN={d_n:.3f}) "
                        f"-> local(dX={d_x:.3f}, dY={d_y:.3f})"
                    ),
                    throttle_duration_sec=1.0,
                )

            # Goal in local metric frame (typically ZED odom frame)
            gx_local = origin_x + d_x
            gy_local = origin_y + d_y
            gz_local = origin_z + d_u

            if publish_frame == local_frame:
                gx, gy, gz = (gx_local, gy_local, gz_local)
            elif use_tf and tf_local_to_out is not None:
                gx, gy, gz = self._apply_transform(tf_local_to_out, gx_local, gy_local, gz_local)
            else:
                # No-TF localization: treat output_frame as a local map whose origin is the startup odom pose.
                gx, gy, gz = self._local_point_to_output_without_tf(gx_local, gy_local, gz_local)

            pose = Pose()
            pose.position.x = float(gx)
            pose.position.y = float(gy)
            pose.position.z = float(gz)
            pose.orientation.w = 1.0
            out.poses.append(pose)

        self._pub.publish(out)


def main() -> None:
    rclpy.init()
    node = LatLonPoseArrayToLocal()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
