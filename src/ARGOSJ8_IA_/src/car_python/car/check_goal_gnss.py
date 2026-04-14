#!/usr/bin/env python3

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import List, Optional

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data

from sensor_msgs.msg import NavSatFix
from std_msgs.msg import Bool


@dataclass(frozen=True)
class _GoalKey:
    lat_deg: float
    lon_deg: float
    alt_m: float


def _finite(v: float) -> bool:
    return math.isfinite(v)


def _haversine_m(lat1_deg: float, lon1_deg: float, lat2_deg: float, lon2_deg: float) -> float:
    # Mean earth radius in meters.
    r = 6371000.0
    lat1 = math.radians(lat1_deg)
    lon1 = math.radians(lon1_deg)
    lat2 = math.radians(lat2_deg)
    lon2 = math.radians(lon2_deg)

    dlat = lat2 - lat1
    dlon = lon2 - lon1

    a = math.sin(dlat / 2.0) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2.0) ** 2
    c = 2.0 * math.atan2(math.sqrt(a), math.sqrt(1.0 - a))
    return r * c


class CheckGoalGnss(Node):
    """Check if current GNSS fix is close to the GNSS goal.

    Subscribes:
      - goal_topic (NavSatFix) default: /goal_gnss
      - fix_topic  (NavSatFix) default: /fixposition/navsatfix

    Publishes:
      - goal_reached_topic (Bool) default: goal_reached

    Parameters:
      - goal_threshold_m: horizontal distance threshold (meters)
      - use_altitude: if True, also require |alt - goal_alt| < alt_threshold_m
      - alt_threshold_m: altitude tolerance (meters)
    """

    def __init__(self) -> None:
        super().__init__("check_goal_gnss")

        self.declare_parameter("goal_topic", "/goal_gnss")
        self.declare_parameter("fix_topic", "/fixposition/navsatfix")
        self.declare_parameter("goal_reached_topic", "goal_reached")

        self.declare_parameter("goal_threshold_m", 3.0)
        self.declare_parameter("use_altitude", False)
        self.declare_parameter("alt_threshold_m", 6.0)

        goal_topic = str(self.get_parameter("goal_topic").value)
        fix_topic = str(self.get_parameter("fix_topic").value)
        goal_reached_topic = str(self.get_parameter("goal_reached_topic").value)

        self._tol_m = float(self.get_parameter("goal_threshold_m").value)
        self._use_alt = bool(self.get_parameter("use_altitude").value)
        self._alt_tol_m = float(self.get_parameter("alt_threshold_m").value)

        self._pub = self.create_publisher(Bool, goal_reached_topic, 10)

        self._goal: Optional[NavSatFix] = None
        self._fix: Optional[NavSatFix] = None
        self._goal_key: Optional[_GoalKey] = None
        self._reached_for_goal: bool = False

        self.create_subscription(NavSatFix, goal_topic, self._on_goal, qos_profile_sensor_data)
        self.create_subscription(NavSatFix, fix_topic, self._on_fix, qos_profile_sensor_data)

        self.create_timer(0.1, self._control_loop)

        self.get_logger().info(
            f"check_goal_gnss listo. Goal: {goal_topic}  Fix: {fix_topic}  Pub: {goal_reached_topic}  tol={self._tol_m:.2f}m"
        )

    def _on_goal(self, msg: NavSatFix) -> None:
        lat = float(msg.latitude)
        lon = float(msg.longitude)
        alt = float(msg.altitude)

        if not (_finite(lat) and _finite(lon)):
            self.get_logger().warn("Goal GNSS inválido (NaN/Inf); ignorando")
            return

        new_key = _GoalKey(lat_deg=lat, lon_deg=lon, alt_m=alt if _finite(alt) else 0.0)
        if self._goal_key != new_key:
            self._goal_key = new_key
            self._reached_for_goal = False

        self._goal = msg

    def _on_fix(self, msg: NavSatFix) -> None:
        lat = float(msg.latitude)
        lon = float(msg.longitude)
        if not (_finite(lat) and _finite(lon)):
            return
        self._fix = msg

    def _control_loop(self) -> None:
        if self._reached_for_goal:
            return
        if self._goal is None or self._fix is None:
            return

        g_lat = float(self._goal.latitude)
        g_lon = float(self._goal.longitude)
        g_alt = float(self._goal.altitude)

        f_lat = float(self._fix.latitude)
        f_lon = float(self._fix.longitude)
        f_alt = float(self._fix.altitude)

        if not (_finite(g_lat) and _finite(g_lon) and _finite(f_lat) and _finite(f_lon)):
            return

        dist_xy_m = _haversine_m(f_lat, f_lon, g_lat, g_lon)
        if dist_xy_m > self._tol_m:
            return

        if self._use_alt:
            if not (_finite(g_alt) and _finite(f_alt)):
                return
            if abs(f_alt - g_alt) > self._alt_tol_m:
                return

        self._reached_for_goal = True
        self._pub.publish(Bool(data=True))

        self.get_logger().info(
            f"Goal GNSS alcanzado: dist_xy={dist_xy_m:.2f} m (tol={self._tol_m:.2f} m)"
        )


def main(args: List[str] | None = None) -> None:
    rclpy.init(args=args)
    node = CheckGoalGnss()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        try:
            if rclpy.ok():
                rclpy.shutdown()
        except Exception:
            pass


if __name__ == "__main__":
    main()
