#!/usr/bin/env python3

from __future__ import annotations

from dataclasses import dataclass
from typing import List, Sequence, Tuple

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Pose, PoseArray
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


class LatLonCircuit(Node):
    """Publish a fixed list of GPS (lat/lon[/alt]) waypoints sequentially.

    This node is meant to work with the existing pipeline:
      - It publishes PoseArray on `/gui/goal_latlon` (or configurable topic)
      - `latlon_posearray_to_local` converts that to metric goals
      - `check_goal` publishes `goal_reached` when the goal is reached

    Only responsibility here: publish the current waypoint, and advance
    to the next one when `goal_reached` becomes True.

    Parameters:
      - waypoints: flat list [lat, lon, alt, lat, lon, alt, ...] (or without alt)
      - waypoint_dim: 3 (default) or 2
      - output_topic: where to publish PoseArray lat/lon goals
      - goal_reached_topic: Bool topic that triggers advancing
      - republish_hz: republish current waypoint at this rate (0 disables timer)
      - loop: if True, wrap around at the end
      - hold_when_done: if True and loop=False, stop publishing after last is reached
      - input_order: 'lat_lon' (default) or 'lon_lat' encoding into Pose.position
    """

    def __init__(self) -> None:
        super().__init__("latlon_circuit")

        self.declare_parameter("waypoints", [])
        self.declare_parameter("waypoint_dim", 3)

        self.declare_parameter("output_topic", "/gui/goal_latlon")
        self.declare_parameter("goal_reached_topic", "goal_reached")

        self.declare_parameter("republish_hz", 2.0)
        self.declare_parameter("loop", False)
        self.declare_parameter("hold_when_done", True)
        self.declare_parameter("start_index", 0)

        self.declare_parameter("input_order", "lat_lon")
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

        self._input_order = str(self.get_parameter("input_order").value).strip().lower()
        if self._input_order not in ("lat_lon", "lon_lat"):
            raise ValueError("input_order must be 'lat_lon' or 'lon_lat'")

        output_topic = str(self.get_parameter("output_topic").value)
        goal_reached_topic = str(self.get_parameter("goal_reached_topic").value)

        self._pub = self.create_publisher(PoseArray, output_topic, 10)
        self.create_subscription(Bool, goal_reached_topic, self._on_goal_reached, 10)

        self._active = True

        hz = float(self.get_parameter("republish_hz").value)
        if hz > 0.0:
            self.create_timer(1.0 / hz, self._republish_timer)

        self.get_logger().info(
            f"latlon_circuit started with {len(self._wps)} waypoints. "
            f"Publishing to {output_topic}; advancing on {goal_reached_topic}."
        )
        self._publish_current(initial=True)

    def _make_posearray_for_wp(self, wp: _Waypoint) -> PoseArray:
        msg = PoseArray()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = str(self.get_parameter("frame_id").value)

        p = Pose()
        if self._input_order == "lat_lon":
            p.position.x = float(wp.lat_deg)
            p.position.y = float(wp.lon_deg)
        else:
            p.position.x = float(wp.lon_deg)
            p.position.y = float(wp.lat_deg)
        p.position.z = float(wp.alt_m)
        p.orientation.w = 1.0

        msg.poses = [p]
        return msg

    def _publish_current(self, initial: bool = False) -> None:
        if not self._active:
            return
        wp = self._wps[self._idx]
        self._pub.publish(self._make_posearray_for_wp(wp))
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

        at_last = (self._idx == (len(self._wps) - 1))
        if at_last and not self._loop:
            self.get_logger().info("Last waypoint reached.")
            if self._hold_when_done:
                self._active = False
                return
            # If not holding, keep publishing last.
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
    node = LatLonCircuit()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
