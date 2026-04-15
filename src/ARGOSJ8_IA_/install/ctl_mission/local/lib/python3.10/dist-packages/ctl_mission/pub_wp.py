#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped

class WaypointPublisher(Node):
    def __init__(self):
        super().__init__('waypoint_publisher')
        self.publisher_ = self.create_publisher(Path, 'input_path', 10)
        self.timer = self.create_timer(2, self.publish_waypoints)

    def publish_waypoints(self):
        path = Path()
        path.header.frame_id = "FP_ENU"
        path.header.stamp = self.get_clock().now().to_msg()

        # Define your waypoints here
        waypoints = [(0, 0), (1, 1), (2, 2)]  # Example waypoints

        for x, y in waypoints:
            pose = PoseStamped()
            pose.header = path.header
            pose.pose.position.x = float(x)
            pose.pose.position.y = float(y)
            path.poses.append(pose)

        self.publisher_.publish(path)

def main(args=None):
    rclpy.init(args=args)
    waypoint_publisher = WaypointPublisher()
    rclpy.spin(waypoint_publisher)
    waypoint_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

