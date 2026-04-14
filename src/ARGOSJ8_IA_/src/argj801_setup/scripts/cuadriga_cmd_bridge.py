#!/usr/bin/env python3

from typing import Any, Optional

from geometry_msgs.msg import Twist
import rclpy
from rclpy.duration import Duration
from rclpy.node import Node
from std_msgs.msg import UInt8MultiArray

try:
    import serial
except ImportError:  # pragma: no cover - runtime dependency
    serial = None


class CuadrigaCmdBridge(Node):
    def __init__(self) -> None:
        super().__init__('cuadriga_cmd_bridge_node')

        self.declare_parameter('input_cmd_vel_topic', 'secured_cmd_vel')
        self.declare_parameter('output_mode', 'topic')
        self.declare_parameter('serial_write_topic', '/Cuadriga/serial_write')
        self.declare_parameter('serial_port', '/dev/ttyUSB0')
        self.declare_parameter('baud_rate', 9600)
        self.declare_parameter('serial_bytesize', 7)
        self.declare_parameter('serial_parity', 'E')
        self.declare_parameter('serial_stop_bits', 1)
        self.declare_parameter('command_timeout', 1.0)
        self.declare_parameter('linear_velocity_max', 1.2)
        self.declare_parameter('angular_velocity_max', 6.0)
        self.declare_parameter('pwm_limit', 127)
        self.declare_parameter('allow_reverse_linear', False)
        self.declare_parameter('send_angular_first', True)

        self.input_cmd_vel_topic = self.get_parameter('input_cmd_vel_topic').get_parameter_value().string_value
        self.output_mode = self.get_parameter('output_mode').get_parameter_value().string_value.lower()
        self.serial_write_topic = self.get_parameter('serial_write_topic').get_parameter_value().string_value
        self.serial_port = self.get_parameter('serial_port').get_parameter_value().string_value
        self.baud_rate = self.get_parameter('baud_rate').get_parameter_value().integer_value
        self.serial_bytesize = self.get_parameter('serial_bytesize').get_parameter_value().integer_value
        self.serial_parity = self.get_parameter('serial_parity').get_parameter_value().string_value.upper()
        self.serial_stop_bits = self.get_parameter('serial_stop_bits').get_parameter_value().integer_value
        self.command_timeout = self.get_parameter('command_timeout').get_parameter_value().double_value
        self.linear_velocity_max = self.get_parameter('linear_velocity_max').get_parameter_value().double_value
        self.angular_velocity_max = self.get_parameter('angular_velocity_max').get_parameter_value().double_value
        self.pwm_limit = self.get_parameter('pwm_limit').get_parameter_value().integer_value
        self.allow_reverse_linear = self.get_parameter('allow_reverse_linear').get_parameter_value().bool_value
        self.send_angular_first = self.get_parameter('send_angular_first').get_parameter_value().bool_value

        self.serial_handle: Optional[Any] = None
        self.serial_write_publisher: Optional[rclpy.publisher.Publisher] = None
        self.last_cmd_time = self.get_clock().now()
        self.last_sent_linear = 0.0
        self.last_sent_angular = 0.0
        self.stop_sent_for_timeout = False

        if self.output_mode not in {'topic', 'serial'}:
            self.get_logger().warn(
                f"Unsupported output_mode '{self.output_mode}', falling back to 'topic'"
            )
            self.output_mode = 'topic'

        if self.output_mode == 'topic':
            self.serial_write_publisher = self.create_publisher(UInt8MultiArray, self.serial_write_topic, 10)

        self.subscription = self.create_subscription(
            Twist,
            self.input_cmd_vel_topic,
            self.cmd_vel_callback,
            10,
        )
        self.timeout_timer = self.create_timer(0.1, self.check_timeout)

        if self.output_mode == 'serial':
            self.open_serial_if_needed()

        self.get_logger().info(
            f"Cuadriga command bridge listening on '{self.input_cmd_vel_topic}' using output_mode='{self.output_mode}'"
        )

    def open_serial_if_needed(self) -> None:
        if self.serial_handle is not None and self.serial_handle.is_open:
            return

        if serial is None:
            self.get_logger().error('pyserial is not available. Install python3-serial to use cuadriga_cmd_bridge.')
            return

        bytesize_map = {
            5: serial.FIVEBITS,
            6: serial.SIXBITS,
            7: serial.SEVENBITS,
            8: serial.EIGHTBITS,
        }
        parity_map = {
            'N': serial.PARITY_NONE,
            'E': serial.PARITY_EVEN,
            'O': serial.PARITY_ODD,
        }
        stop_bits_map = {
            1: serial.STOPBITS_ONE,
            2: serial.STOPBITS_TWO,
        }

        try:
            self.serial_handle = serial.Serial(
                port=self.serial_port,
                baudrate=self.baud_rate,
                bytesize=bytesize_map.get(self.serial_bytesize, serial.SEVENBITS),
                parity=parity_map.get(self.serial_parity, serial.PARITY_EVEN),
                stopbits=stop_bits_map.get(self.serial_stop_bits, serial.STOPBITS_ONE),
                timeout=0.0,
                write_timeout=0.2,
            )
            self.get_logger().info(f"Opened serial port {self.serial_port} at {self.baud_rate} baud")
        except Exception as exc:  # pragma: no cover - hardware dependent
            self.serial_handle = None
            self.get_logger().error(f'Failed to open serial port {self.serial_port}: {exc}')

    def cmd_vel_callback(self, msg: Twist) -> None:
        self.last_cmd_time = self.get_clock().now()
        self.stop_sent_for_timeout = False
        self.send_motion(msg.linear.x, msg.angular.z)

    def check_timeout(self) -> None:
        now = self.get_clock().now()
        if self.command_timeout <= 0.0:
            return

        if now - self.last_cmd_time <= Duration(seconds=self.command_timeout):
            return

        self.open_serial_if_needed()
        if self.stop_sent_for_timeout:
            return

        self.get_logger().warn('No recent cmd_vel received, sending stop command to Cuadriga backend')
        self.send_motion(0.0, 0.0)
        self.stop_sent_for_timeout = True

    def send_motion(self, linear_velocity: float, angular_velocity: float) -> None:
        if not self.allow_reverse_linear:
            linear_velocity = max(0.0, linear_velocity)

        linear_pwm = self.velocity_to_pwm(linear_velocity, self.linear_velocity_max)
        angular_pwm = self.angular_to_pwm(angular_velocity, self.angular_velocity_max)

        command_angular = self.encode_pwm_command(angular_pwm, 'A', 'a')
        command_linear = self.encode_pwm_command(linear_pwm, 'B', 'b')

        commands = [command_angular, command_linear] if self.send_angular_first else [command_linear, command_angular]

        if self.output_mode == 'topic':
            for command in commands:
                message = UInt8MultiArray()
                message.data = list(command)
                self.serial_write_publisher.publish(message)
            self.last_sent_linear = linear_velocity
            self.last_sent_angular = angular_velocity
            return

        self.open_serial_if_needed()
        if self.serial_handle is None or not self.serial_handle.is_open:
            return

        try:
            for command in commands:
                self.serial_handle.write(command)
            self.last_sent_linear = linear_velocity
            self.last_sent_angular = angular_velocity
        except Exception as exc:  # pragma: no cover - hardware dependent
            self.get_logger().error(f'Failed to write command to serial backend: {exc}')
            try:
                self.serial_handle.close()
            except Exception:
                pass
            self.serial_handle = None

    def velocity_to_pwm(self, value: float, max_value: float) -> int:
        return self.scale_to_pwm(value, max_value)

    def angular_to_pwm(self, value: float, max_value: float) -> int:
        scaled_sign = -1 if value >= 0.0 else 1
        return self.scale_to_pwm(abs(value) * scaled_sign, max_value)

    def scale_to_pwm(self, value: float, max_value: float) -> int:
        if max_value <= 0.0:
            return 0

        clamped = max(-max_value, min(max_value, value))
        normalized = abs(clamped) / max_value
        pwm_value = int(round(normalized * self.pwm_limit))
        pwm_value = min(self.pwm_limit, max(0, pwm_value))
        return -pwm_value if clamped < 0.0 else pwm_value

    def encode_pwm_command(self, pwm_value: int, positive_prefix: str, negative_prefix: str) -> bytes:
        prefix = negative_prefix if pwm_value < 0 else positive_prefix
        magnitude = min(self.pwm_limit, max(0, abs(pwm_value)))
        return f'!{prefix}{magnitude:02X}\r'.encode('ascii')

    def destroy_node(self) -> bool:
        try:
            if self.serial_handle is not None and self.serial_handle.is_open:
                self.send_motion(0.0, 0.0)
                self.serial_handle.close()
        except Exception:
            pass
        return super().destroy_node()


def main(args=None) -> None:
    rclpy.init(args=args)
    node = CuadrigaCmdBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()