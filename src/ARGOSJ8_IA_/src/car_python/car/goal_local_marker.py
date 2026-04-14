#!/usr/bin/env python3

from __future__ import annotations

from typing import Optional

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import PoseArray
from visualization_msgs.msg import Marker, MarkerArray


class GoalLocalMarker(Node):
    def __init__(self) -> None:
        super().__init__("goal_local_marker")

        self.declare_parameter("input_topic", "/goal_local")
        self.declare_parameter("output_topic", "/goal_local_markers")
        self.declare_parameter("frame_id", "")  # if empty, use msg.header.frame_id
        self.declare_parameter("ns", "goal_local")

        self.declare_parameter("scale_xyz", 1.0)  # meters
        self.declare_parameter("alpha", 0.9)
        self.declare_parameter("rgb", [1.0, 0.2, 0.2])

        self.declare_parameter("publish_line", True)
        self.declare_parameter("line_width", 0.15)

        self._last_count: int = 0

        input_topic = self.get_parameter("input_topic").get_parameter_value().string_value
        output_topic = self.get_parameter("output_topic").get_parameter_value().string_value

        self._pub = self.create_publisher(MarkerArray, output_topic, 10)
        self.create_subscription(PoseArray, input_topic, self._on_goal, 10)

        self.get_logger().info(f"Publishing RViz markers from {input_topic} -> {output_topic}")

    def _make_delete_extra(self, frame_id: str, ns: str, start_id: int, end_id: int) -> list[Marker]:
        markers: list[Marker] = []
        for mid in range(start_id, end_id):
            m = Marker()
            m.header.frame_id = frame_id
            m.header.stamp = self.get_clock().now().to_msg()
            m.ns = ns
            m.id = mid
            m.action = Marker.DELETE
            markers.append(m)
        return markers

    def _on_goal(self, msg: PoseArray) -> None:
        frame_override = self.get_parameter("frame_id").get_parameter_value().string_value
        frame_id = frame_override if frame_override else msg.header.frame_id
        if not frame_id:
            frame_id = "map"

        ns = self.get_parameter("ns").get_parameter_value().string_value

        scale = float(self.get_parameter("scale_xyz").get_parameter_value().double_value)
        alpha = float(self.get_parameter("alpha").get_parameter_value().double_value)
        rgb = self.get_parameter("rgb").get_parameter_value().double_array_value
        r = float(rgb[0]) if len(rgb) > 0 else 1.0
        g = float(rgb[1]) if len(rgb) > 1 else 0.2
        b = float(rgb[2]) if len(rgb) > 2 else 0.2

        publish_line = bool(self.get_parameter("publish_line").get_parameter_value().bool_value)
        line_width = float(self.get_parameter("line_width").get_parameter_value().double_value)

        out = MarkerArray()
        now = self.get_clock().now().to_msg()

        # Spheres for each goal pose
        for i, pose in enumerate(msg.poses):
            m = Marker()
            m.header.frame_id = frame_id
            m.header.stamp = now
            m.ns = ns
            m.id = i
            m.type = Marker.SPHERE
            m.action = Marker.ADD
            m.pose = pose
            m.scale.x = scale
            m.scale.y = scale
            m.scale.z = scale
            m.color.a = alpha
            m.color.r = r
            m.color.g = g
            m.color.b = b
            out.markers.append(m)

        # Optional line strip connecting waypoints
        if publish_line and len(msg.poses) >= 2:
            line = Marker()
            line.header.frame_id = frame_id
            line.header.stamp = now
            line.ns = ns
            line.id = 10_000
            line.type = Marker.LINE_STRIP
            line.action = Marker.ADD
            line.scale.x = line_width
            line.color.a = min(1.0, alpha)
            line.color.r = r
            line.color.g = g
            line.color.b = b
            for pose in msg.poses:
                line.points.append(pose.position)
            out.markers.append(line)

        # Delete leftover markers if waypoint count shrank
        if self._last_count > len(msg.poses):
            out.markers.extend(self._make_delete_extra(frame_id, ns, len(msg.poses), self._last_count))
        self._last_count = len(msg.poses)

        self._pub.publish(out)


def main() -> None:
    rclpy.init()
    node = GoalLocalMarker()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
