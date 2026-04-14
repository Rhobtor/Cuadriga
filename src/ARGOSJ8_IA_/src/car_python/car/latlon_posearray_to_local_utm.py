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


def _quat_mul(
    a: Tuple[float, float, float, float], b: Tuple[float, float, float, float]
) -> Tuple[float, float, float, float]:
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


def _wgs84_to_utm(
    lat_deg: float, lon_deg: float, *, zone_number: Optional[int] = None
) -> Tuple[float, float, int, str]:
    """Convert WGS84 lat/lon to UTM easting/northing.

    Returns: (easting_m, northing_m, zone_number, hemisphere)

    Notes:
      - Uses standard UTM projection (WGS84 ellipsoid).
      - If zone_number is provided, that zone's central meridian is used.
      - For local navigation use (small areas), forcing the reference zone for all points is OK.
    """

    if zone_number is None:
        zone_number = int((lon_deg + 180.0) / 6.0) + 1
        zone_number = max(1, min(60, zone_number))

    hemisphere = "N" if lat_deg >= 0.0 else "S"

    a = 6378137.0
    f = 1.0 / 298.257223563
    ecc_sq = 2.0 * f - f * f
    ecc_prime_sq = ecc_sq / (1.0 - ecc_sq)
    k0 = 0.9996

    lat = _deg2rad(lat_deg)
    lon = _deg2rad(lon_deg)

    lon0_deg = (zone_number - 1) * 6 - 180 + 3
    lon0 = _deg2rad(lon0_deg)

    sin_lat = math.sin(lat)
    cos_lat = math.cos(lat)

    n = a / math.sqrt(1.0 - ecc_sq * sin_lat * sin_lat)
    t = math.tan(lat)
    t2 = t * t
    c = ecc_prime_sq * cos_lat * cos_lat
    A = cos_lat * (lon - lon0)

    ecc4 = ecc_sq * ecc_sq
    ecc6 = ecc4 * ecc_sq

    M = a * (
        (1 - ecc_sq / 4 - 3 * ecc4 / 64 - 5 * ecc6 / 256) * lat
        - (3 * ecc_sq / 8 + 3 * ecc4 / 32 + 45 * ecc6 / 1024) * math.sin(2 * lat)
        + (15 * ecc4 / 256 + 45 * ecc6 / 1024) * math.sin(4 * lat)
        - (35 * ecc6 / 3072) * math.sin(6 * lat)
    )

    easting = k0 * n * (
        A
        + (1 - t2 + c) * (A ** 3) / 6
        + (5 - 18 * t2 + t2 * t2 + 72 * c - 58 * ecc_prime_sq) * (A ** 5) / 120
    ) + 500000.0

    northing = k0 * (
        M
        + n
        * t
        * (
            (A ** 2) / 2
            + (5 - t2 + 9 * c + 4 * c * c) * (A ** 4) / 24
            + (61 - 58 * t2 + t2 * t2 + 600 * c - 330 * ecc_prime_sq) * (A ** 6) / 720
        )
    )

    if lat_deg < 0.0:
        northing += 10000000.0

    return (float(easting), float(northing), int(zone_number), hemisphere)


class LatLonPoseArrayToLocalUTM(Node):
    """Convert PoseArray goals expressed in lat/lon into local coordinates using UTM.

    This is the same idea as `latlon_posearray_to_local`, but it computes the metric delta
    using UTM projection (more accurate than a simple equirectangular approximation).

    It still needs a yaw ENU->local if your local frame (ZED odom/map) isn't aligned to ENU.
    """

    def __init__(self) -> None:
        super().__init__("latlon_posearray_to_local_utm")

        self.declare_parameter("gnss_topic", "/fixposition/gnss1")
        self.declare_parameter("odom_topic", "/zed/zed_node/odom")
        self.declare_parameter("input_topic", "/gui/goal_latlon")
        self.declare_parameter("output_topic", "/goal_local")

        # - 'startup': lock origin once at startup (default)
        # - 'current_fix': use latest GNSS+odom as reference for each goal
        self.declare_parameter("reference_mode", "startup")

        self.declare_parameter("local_frame", "odom")
        self.declare_parameter("fp_frame", "")  # legacy alias
        self.declare_parameter("output_frame", "map")

        self.declare_parameter("use_tf", True)
        self.declare_parameter("fallback_to_local_frame_on_tf_failure", True)

        self.declare_parameter("publish_in_output_frame_without_tf", True)
        self.declare_parameter("fallback_to_output_frame_without_tf_on_tf_failure", True)
        self.declare_parameter("align_yaw", False)

        self.declare_parameter("input_order", "lat_lon")
        self.declare_parameter("use_altitude", False)

        # ENU->local horizontal rotation (radians)
        self.declare_parameter("enu_to_local_yaw_rad", 0.0)
        self.declare_parameter("estimate_enu_to_local_yaw", True)
        self.declare_parameter("yaw_estimation_min_distance_m", 1.0)

        # UTM options
        # If 0 => auto-select from reference lon. If 1..60 => force that zone.
        self.declare_parameter("utm_zone", 0)
        # If True, always use the reference zone for goal projection (avoids jumps near zone boundaries).
        self.declare_parameter("utm_use_reference_zone_for_goals", True)

        self._tf_buffer = Buffer()
        self._tf_listener = TransformListener(self._tf_buffer, self)

        self._latest_gnss: Optional[NavSatFix] = None
        self._latest_odom: Optional[Odometry] = None

        self._origin_latlon_alt: Optional[Tuple[float, float, float]] = None
        self._origin_local_xyz: Optional[Tuple[float, float, float]] = None
        self._origin_local_yaw: Optional[float] = None

        self._origin_utm: Optional[Tuple[float, float, int, str]] = None
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
            "latlon_posearray_to_local_utm running. Waiting for GNSS + odometry to establish reference."
        )

    def _on_gnss(self, msg: NavSatFix) -> None:
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
            return

        if self._origin_latlon_alt is not None and self._origin_local_xyz is not None and self._origin_utm is not None:
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
        self._origin_local_yaw = math.atan2(
            2.0 * (float(q.w) * float(q.z) + float(q.x) * float(q.y)),
            1.0 - 2.0 * (float(q.y) * float(q.y) + float(q.z) * float(q.z)),
        )

        utm_zone_param = int(self.get_parameter("utm_zone").get_parameter_value().integer_value)
        zone_for_ref = None if utm_zone_param == 0 else utm_zone_param
        self._origin_utm = _wgs84_to_utm(lat0, lon0, zone_number=zone_for_ref)

        e0, n0, z0, h0 = self._origin_utm
        self.get_logger().info(
            f"Reference locked: lat0={lat0:.8f} lon0={lon0:.8f} alt0={alt0:.3f} | "
            f"local_origin=({p.x:.3f},{p.y:.3f},{p.z:.3f}) | UTM(z={z0}{h0} e={e0:.3f} n={n0:.3f})"
        )

    def _get_reference(self) -> Optional[Tuple[Tuple[float, float, float], Tuple[float, float, float], float, Tuple[float, float, int, str]]]:
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

            utm_zone_param = int(self.get_parameter("utm_zone").get_parameter_value().integer_value)
            zone_for_ref = None if utm_zone_param == 0 else utm_zone_param
            utm_ref = _wgs84_to_utm(lat0, lon0, zone_number=zone_for_ref)

            return ((lat0, lon0, alt0), (float(p.x), float(p.y), float(p.z)), float(yaw0), utm_ref)

        if self._origin_latlon_alt is None or self._origin_local_xyz is None or self._origin_utm is None:
            return None

        yaw0 = float(self._origin_local_yaw) if self._origin_local_yaw is not None else 0.0
        return (self._origin_latlon_alt, self._origin_local_xyz, yaw0, self._origin_utm)

    def _update_enu_to_local_yaw_estimate(self) -> None:
        reference_mode = self.get_parameter("reference_mode").get_parameter_value().string_value
        if reference_mode.strip().lower() == "current_fix":
            return
        if not bool(self.get_parameter("estimate_enu_to_local_yaw").get_parameter_value().bool_value):
            return
        if self._origin_utm is None or self._origin_local_xyz is None:
            return
        if self._latest_gnss is None or self._latest_odom is None:
            return

        # ENU displacement derived from UTM between origin and current fix
        lat = float(self._latest_gnss.latitude)
        lon = float(self._latest_gnss.longitude)
        utm_zone_param = int(self.get_parameter("utm_zone").get_parameter_value().integer_value)
        zone_for_ref = None if utm_zone_param == 0 else utm_zone_param

        e_cur, n_cur, z_cur, h_cur = _wgs84_to_utm(lat, lon, zone_number=zone_for_ref)
        e0, n0, z0, h0 = self._origin_utm

        if (z_cur != z0) or (h_cur != h0):
            # Still usable locally, but warn.
            self.get_logger().warn(
                f"UTM zone/hemisphere changed (ref {z0}{h0} vs cur {z_cur}{h_cur}); yaw estimation may be noisy.",
                throttle_duration_sec=5.0,
            )

        d_e = float(e_cur - e0)
        d_n = float(n_cur - n0)

        # Local displacement from origin to current odom
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
        x = c * d_e - s * d_n
        y = s * d_e + c * d_n
        return (x, y)

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
        ref = self._get_reference()
        if ref is None:
            self.get_logger().warn("Reference not ready yet (need GNSS + odometry). Ignoring goal.")
            return

        (lat0, lon0, alt0), (origin_x, origin_y, origin_z), origin_yaw, utm_ref = ref
        e0, n0, zone0, hemi0 = utm_ref

        fp_frame = self.get_parameter("fp_frame").get_parameter_value().string_value
        local_frame = self.get_parameter("local_frame").get_parameter_value().string_value
        if fp_frame.strip():
            local_frame = fp_frame.strip()

        out_frame = self.get_parameter("output_frame").get_parameter_value().string_value
        use_tf = bool(self.get_parameter("use_tf").get_parameter_value().bool_value)
        fallback_to_local = bool(self.get_parameter("fallback_to_local_frame_on_tf_failure").get_parameter_value().bool_value)

        publish_in_output_without_tf = bool(self.get_parameter("publish_in_output_frame_without_tf").get_parameter_value().bool_value)
        fallback_to_output_without_tf = bool(
            self.get_parameter("fallback_to_output_frame_without_tf_on_tf_failure").get_parameter_value().bool_value
        )

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

        use_alt = bool(self.get_parameter("use_altitude").get_parameter_value().bool_value)

        use_ref_zone_for_goals = bool(
            self.get_parameter("utm_use_reference_zone_for_goals").get_parameter_value().bool_value
        )

        for pose_in in msg.poses:
            lat_deg, lon_deg, alt_m = self._parse_latlon(pose_in)

            if use_ref_zone_for_goals:
                e1, n1, zone1, hemi1 = _wgs84_to_utm(lat_deg, lon_deg, zone_number=zone0)
            else:
                e1, n1, zone1, hemi1 = _wgs84_to_utm(lat_deg, lon_deg, zone_number=None)

            if (zone1 != zone0) or (hemi1 != hemi0):
                self.get_logger().warn(
                    f"Goal UTM zone/hemisphere differs from reference (ref {zone0}{hemi0} vs goal {zone1}{hemi1}); "
                    "consider enabling utm_use_reference_zone_for_goals.",
                    throttle_duration_sec=2.0,
                )

            d_e = float(e1 - e0)
            d_n = float(n1 - n0)
            d_x, d_y = self._rotate_enu_delta_to_local(d_e, d_n)

            gx_local = float(origin_x + d_x)
            gy_local = float(origin_y + d_y)
            gz_local = float(origin_z + (float(alt_m - alt0) if use_alt else 0.0))

            if publish_frame == local_frame:
                gx, gy, gz = (gx_local, gy_local, gz_local)
            elif use_tf and tf_local_to_out is not None:
                gx, gy, gz = self._apply_transform(tf_local_to_out, gx_local, gy_local, gz_local)
            else:
                # No-TF localization: treat output_frame as local map (origin at reference odom pose).
                gx, gy, gz = (gx_local - float(origin_x), gy_local - float(origin_y), gz_local - float(origin_z))

            pose_out = Pose()
            pose_out.position.x = float(gx)
            pose_out.position.y = float(gy)
            pose_out.position.z = float(gz)
            pose_out.orientation.w = 1.0
            out.poses.append(pose_out)

        self._pub.publish(out)


def main() -> None:
    rclpy.init()
    node = LatLonPoseArrayToLocalUTM()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
