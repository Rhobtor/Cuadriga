import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg

class JoystickControlNode(Node):
    def __init__(self):
        super().__init__('joystick_control')
        self.joy_subscriber = self.create_subscription(
            Joy, '/joy', self.joy_callback, 10)
        # self.command_publisher = self.create_publisher(
        #     CmdThrottleMsg, '/ARGJ801/cmd_throttle_msg', 10)
        self.command_publisher = self.create_publisher(
            Twist, '/ARGJ801/joy', 10)
        self.get_logger().info("Joystick control node started.")

    def joy_callback(self, msg: Joy):
        # Crear el mensaje Twist basado en el input del joystick
        twist_msg = Twist()

        # Asignar ejes del joystick a linear.x y angular.z
        twist_msg.linear.x = msg.axes[1] * 1.0# Eje izquierdo vertical (escala al torque)
        twist_msg.angular.z = msg.axes[3] * 1.0   # Eje izquierdo horizontal (dirección)
        
        # Publicar el mensaje Twist
        self.command_publisher.publish(twist_msg)
        self.get_logger().info(f"Published: linear.x={twist_msg.linear.x}, angular.z={twist_msg.angular.z}")
    #     throttle_msg = CmdThrottleMsg()
    #     throttle_msg.throttle = msg.axes[1] * 100.0  # 100% → +rad/s
    #     throttle_msg.steering = msg.axes[3] * 100.0  # 100% → +rad/s
    #     self.command_publisher.publish(throttle_msg)

    #     self.get_logger().info(
    #         f"Throttle: {throttle_msg.throttle:.1f}%, "
    #         f"Steering: {throttle_msg.steering:.1f}%"
    # )

def main(args=None):
    rclpy.init(args=args)
    joystick_control_node = JoystickControlNode()
    rclpy.spin(joystick_control_node)
    joystick_control_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

