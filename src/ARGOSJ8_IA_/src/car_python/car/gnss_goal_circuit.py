#!/usr/bin/env python3

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import List, Sequence

import rclpy
from rclpy.node import Node
from rclpy.parameter import Parameter
from rclpy.qos import qos_profile_sensor_data

from sensor_msgs.msg import NavSatFix, NavSatStatus
from std_msgs.msg import Bool


@dataclass(frozen=True)
class _Waypoint:
    lat_deg: float
    lon_deg: float
    alt_m: float


def _parse_waypoints(flat: Sequence[float], dim: int) -> List[_Waypoint]:
    if dim not in (2, 3):
        raise ValueError("parameter 'waypoint_dim' must be 2 or 3")

    if len(flat) == 0:
        return []

    if len(flat) % dim != 0:
        raise ValueError(
            f"parameter 'waypoints' length must be multiple of waypoint_dim={dim}. "
            f"Got {len(flat)} values."
        )

    out: List[_Waypoint] = []
    for i in range(0, len(flat), dim):
        lat = float(flat[i + 0])
        lon = float(flat[i + 1])
        alt = float(flat[i + 2]) if dim == 3 else 0.0
        out.append(_Waypoint(lat_deg=lat, lon_deg=lon, alt_m=alt))
    return out


class GnssGoalCircuit(Node):
    """Publish GNSS (NavSatFix) waypoints sequentially on /goal_gnss.

    This matches the manual command:
      ros2 topic pub /goal_gnss sensor_msgs/msg/NavSatFix "{latitude: .., longitude: .., altitude: ..}"

    It republishes the current waypoint at a configurable rate, and advances
    to the next waypoint when it receives Bool(True) on `goal_reached_topic`.

    Parameters:
      - waypoints: flat list [lat, lon, alt, lat, lon, alt, ...] (or without alt)
      - waypoint_dim: 3 (default) or 2
      - output_topic: NavSatFix output topic (default: /goal_gnss)
      - goal_reached_topic: Bool topic to advance (default: goal_reached)
      - republish_hz: republish current waypoint at this rate (0 disables timer)
      - loop: if True, wrap around at the end
      - hold_when_done: if True and loop=False, stop publishing after last is reached
      - start_index: starting waypoint index
      - frame_id: header.frame_id for outgoing NavSatFix (default: gps)
    """

    def __init__(self) -> None:
        super().__init__("gnss_goal_circuit")

        # IMPORTANT (ROS2): declaring an empty list `[]` makes rclpy infer BYTE_ARRAY.
        # Declare the parameter type explicitly so YAML DOUBLE_ARRAY overrides work.
        self.declare_parameter("waypoints", Parameter.Type.DOUBLE_ARRAY)
        self.declare_parameter("waypoint_dim", 3)

        self.declare_parameter("output_topic", "/goal_gnss")
        self.declare_parameter("goal_reached_topic", "goal_reached")

        self.declare_parameter("republish_hz", 2.0)
        self.declare_parameter("loop", False)
        self.declare_parameter("hold_when_done", True)
        self.declare_parameter("start_index", 0)
        self.declare_parameter("frame_id", "gps")

        flat = list(self.get_parameter("waypoints").value)
        dim = int(self.get_parameter("waypoint_dim").value)
        self._wps = _parse_waypoints(flat, dim)
        if not self._wps:
            raise ValueError("No waypoints provided. Set parameter 'waypoints'.")

        self._loop = bool(self.get_parameter("loop").value)
        self._hold_when_done = bool(self.get_parameter("hold_when_done").value)

        start_index = int(self.get_parameter("start_index").value)
        if start_index < 0 or start_index >= len(self._wps):
            raise ValueError(f"start_index out of range: {start_index} for {len(self._wps)} waypoints")
        self._idx = start_index

        output_topic = str(self.get_parameter("output_topic").value)
        goal_reached_topic = str(self.get_parameter("goal_reached_topic").value)

        self._pub = self.create_publisher(NavSatFix, output_topic, qos_profile_sensor_data)
        self.create_subscription(Bool, goal_reached_topic, self._on_goal_reached, 10)

        self._active = True

        hz = float(self.get_parameter("republish_hz").value)
        if hz > 0.0 and math.isfinite(hz):
            self.create_timer(1.0 / hz, self._republish_timer)

        self.get_logger().info(
            f"gnss_goal_circuit started with {len(self._wps)} waypoints. "
            f"Publishing NavSatFix to {output_topic}; advancing on {goal_reached_topic}."
        )
        self._publish_current(initial=True)

    def _make_fix_for_wp(self, wp: _Waypoint) -> NavSatFix:
        msg = NavSatFix()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = str(self.get_parameter("frame_id").value)

        msg.status.status = NavSatStatus.STATUS_FIX
        msg.status.service = 0

        msg.latitude = float(wp.lat_deg)
        msg.longitude = float(wp.lon_deg)
        msg.altitude = float(wp.alt_m)

        # Leave covariance unknown unless you want to set it.
        msg.position_covariance_type = NavSatFix.COVARIANCE_TYPE_UNKNOWN
        return msg

    def _publish_current(self, initial: bool = False) -> None:
        if not self._active:
            return
        wp = self._wps[self._idx]
        self._pub.publish(self._make_fix_for_wp(wp))
        if initial:
            self.get_logger().info(
                f"Publishing initial waypoint #{self._idx}: lat={wp.lat_deg:.8f} lon={wp.lon_deg:.8f} alt={wp.alt_m:.2f}"
            )

    def _republish_timer(self) -> None:
        self._publish_current()

    def _on_goal_reached(self, msg: Bool) -> None:
        if not msg.data:
            return
        if not self._active:
            return

        at_last = self._idx == (len(self._wps) - 1)
        if at_last and not self._loop:
            self.get_logger().info("Last GNSS waypoint reached.")
            if self._hold_when_done:
                self._active = False
            return

        self._idx += 1
        if self._idx >= len(self._wps):
            self._idx = 0

        wp = self._wps[self._idx]
        self.get_logger().info(
            f"Advancing to waypoint #{self._idx}: lat={wp.lat_deg:.8f} lon={wp.lon_deg:.8f} alt={wp.alt_m:.2f}"
        )
        self._publish_current()


def main(args: List[str] | None = None) -> None:
    rclpy.init(args=args)
    node = GnssGoalCircuit()
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
