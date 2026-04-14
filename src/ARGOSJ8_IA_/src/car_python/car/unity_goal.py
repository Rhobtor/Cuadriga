#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rclpy.qos import (
    QoSProfile, QoSDurabilityPolicy, QoSHistoryPolicy, QoSReliabilityPolicy
)
from geometry_msgs.msg import PoseArray, Pose

class GoalPointPublisher(Node):
    def __init__(self):
        super().__init__('goal_point_publisher')

        # Parámetros (por defecto pongo tus coordenadas)
        self.declare_parameter('topic', 'goal')
        self.declare_parameter('frame_id', 'map')
        self.declare_parameter('x', -53.345558166503906)
        self.declare_parameter('y',  0.9955338835716248)
        self.declare_parameter('z', -61.25819778442383)
        self.declare_parameter('rate_hz', 0.0)  # 0.0 => publica una sola vez

        topic     = self.get_parameter('topic').get_parameter_value().string_value
        frame_id  = self.get_parameter('frame_id').get_parameter_value().string_value
        self.rate = float(self.get_parameter('rate_hz').value)

        # QoS tipo "latched" en ROS2
        qos = QoSProfile(
            depth=1,
            durability=QoSDurabilityPolicy.TRANSIENT_LOCAL,
            reliability=QoSReliabilityPolicy.RELIABLE,
            history=QoSHistoryPolicy.KEEP_LAST
        )
        self.pub = self.create_publisher(PoseArray, topic, qos)

        if self.rate <= 0.0:
            # Publica una sola vez
            self.publish_once()
        else:
            # Publica periódicamente
            period = max(0.01, 1.0 / self.rate)
            self.timer = self.create_timer(period, self.publish_once)

        self.get_logger().info(
            f"Publicando PoseArray en '{topic}' (frame='{frame_id}') "
            f"rate_hz={self.rate if self.rate>0 else 'once'}"
        )

    def build_msg(self) -> PoseArray:
        x = float(self.get_parameter('x').value)
        y = float(self.get_parameter('y').value)
        z = float(self.get_parameter('z').value)
        frame_id = self.get_parameter('frame_id').get_parameter_value().string_value

        pa = PoseArray()
        pa.header.frame_id = frame_id
        pa.header.stamp    = self.get_clock().now().to_msg()

        p = Pose()
        p.position.x = x
        p.position.y = y
        p.position.z = z
        p.orientation.w = 1.0  # orientación identidad

        pa.poses.append(p)
        return pa

    def publish_once(self):
        msg = self.build_msg()
        self.pub.publish(msg)
        if self.rate <= 0.0:
            self.get_logger().info("PoseArray enviado (una vez).")

def main():
    rclpy.init()
    node = GoalPointPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
