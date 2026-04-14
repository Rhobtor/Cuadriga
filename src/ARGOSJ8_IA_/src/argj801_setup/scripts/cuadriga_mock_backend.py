#!/usr/bin/env python3

import math
from typing import Optional

import rclpy
from geometry_msgs.msg import Vector3Stamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from sensor_msgs.msg import Imu, NavSatFix, NavSatStatus
from std_msgs.msg import UInt8MultiArray


def yaw_to_quaternion(yaw: float):
    half = yaw * 0.5
    return 0.0, 0.0, math.sin(half), math.cos(half)


class CuadrigaMockBackend(Node):
    def __init__(self) -> None:
        super().__init__('cuadriga_mock_backend')

        self.declare_parameter('serial_write_topic', '/Cuadriga/serial_write')
        self.declare_parameter('imu_topic', '/imu_head/data')
        self.declare_parameter('odom_topic', '/fixposition/odometry_enu')
        self.declare_parameter('fix_topic', '/fixposition/navsatfix')
        self.declare_parameter('gps_vel_topic', '/fixposition/navsatfix/vel')
        self.declare_parameter('update_rate_hz', 20.0)
        self.declare_parameter('origin_lat', 36.721302)
        self.declare_parameter('origin_lon', -4.421637)
        self.declare_parameter('origin_alt', 12.0)
        self.declare_parameter('max_linear_speed', 1.2)
        self.declare_parameter('max_yaw_rate', 1.4)
        self.declare_parameter('pwm_limit', 127)

        serial_write_topic = self.get_parameter('serial_write_topic').value
        imu_topic = self.get_parameter('imu_topic').value
        odom_topic = self.get_parameter('odom_topic').value
        fix_topic = self.get_parameter('fix_topic').value
        gps_vel_topic = self.get_parameter('gps_vel_topic').value
        update_rate_hz = float(self.get_parameter('update_rate_hz').value)
        self.origin_lat = float(self.get_parameter('origin_lat').value)
        self.origin_lon = float(self.get_parameter('origin_lon').value)
        self.origin_alt = float(self.get_parameter('origin_alt').value)
        self.max_linear_speed = float(self.get_parameter('max_linear_speed').value)
        self.max_yaw_rate = float(self.get_parameter('max_yaw_rate').value)
        self.pwm_limit = int(self.get_parameter('pwm_limit').value)

        self.linear_command = 0.0
        self.angular_command = 0.0
        self.x_m = 0.0
        self.y_m = 0.0
        self.yaw_rad = 0.0
        self.last_update = self.get_clock().now()

        self.create_subscription(UInt8MultiArray, serial_write_topic, self._on_serial_write, 10)
        self.imu_pub = self.create_publisher(Imu, imu_topic, 10)
        self.odom_pub = self.create_publisher(Odometry, odom_topic, 10)
        self.fix_pub = self.create_publisher(NavSatFix, fix_topic, 10)
        self.gps_vel_pub = self.create_publisher(Vector3Stamped, gps_vel_topic, 10)
        self.create_timer(max(0.01, 1.0 / max(1.0, update_rate_hz)), self._tick)

        self.get_logger().info(
            f"Cuadriga mock backend listening on '{serial_write_topic}' and publishing fake telemetry"
        )

    def _on_serial_write(self, msg: UInt8MultiArray) -> None:
        command = bytes(msg.data)
        decoded = self._decode_command(command)
        if decoded is None:
            self.get_logger().debug(f'Ignoring unknown serial payload: {command!r}')
            return

        channel, signed_value = decoded
        if channel == 'linear':
            self.linear_command = self._scale_to_speed(signed_value, self.max_linear_speed)
        elif channel == 'angular':
            self.angular_command = self._scale_to_speed(signed_value, self.max_yaw_rate)

        self.get_logger().info(
            f"Mock Cuadriga cmd -> linear={self.linear_command:.3f} m/s angular={self.angular_command:.3f} rad/s"
        )

    def _decode_command(self, payload: bytes) -> Optional[tuple[str, int]]:
        if len(payload) < 5 or payload[0:1] != b'!':
            return None

        prefix = chr(payload[1])
        try:
            magnitude = int(payload[2:4].decode('ascii'), 16)
        except Exception:
            return None

        sign = -1 if prefix in {'a', 'b'} else 1
        signed_value = sign * magnitude

        if prefix in {'A', 'a'}:
            return 'angular', signed_value
        if prefix in {'B', 'b'}:
            return 'linear', signed_value
        return None

    def _scale_to_speed(self, pwm_value: int, max_speed: float) -> float:
        if self.pwm_limit <= 0:
            return 0.0
        clamped = max(-self.pwm_limit, min(self.pwm_limit, pwm_value))
        return (float(clamped) / float(self.pwm_limit)) * max_speed

    def _tick(self) -> None:
        now = self.get_clock().now()
        dt = (now - self.last_update).nanoseconds / 1e9
        self.last_update = now
        if dt <= 0.0:
            return

        self.yaw_rad = math.atan2(
            math.sin(self.yaw_rad + self.angular_command * dt),
            math.cos(self.yaw_rad + self.angular_command * dt),
        )
        self.x_m += self.linear_command * math.cos(self.yaw_rad) * dt
        self.y_m += self.linear_command * math.sin(self.yaw_rad) * dt

        lat, lon = self._meters_to_latlon(self.x_m, self.y_m)
        self._publish_imu(now)
        self._publish_odom(now)
        self._publish_fix(now, lat, lon)
        self._publish_gps_vel(now)

    def _meters_to_latlon(self, east_m: float, north_m: float) -> tuple[float, float]:
        lat = self.origin_lat + (north_m / 111320.0)
        lon_scale = max(1e-6, math.cos(math.radians(self.origin_lat)))
        lon = self.origin_lon + (east_m / (111320.0 * lon_scale))
        return lat, lon

    def _publish_imu(self, stamp) -> None:
        msg = Imu()
        msg.header.stamp = stamp.to_msg()
        msg.header.frame_id = 'imu_link'
        qx, qy, qz, qw = yaw_to_quaternion(self.yaw_rad)
        msg.orientation.x = qx
        msg.orientation.y = qy
        msg.orientation.z = qz
        msg.orientation.w = qw
        msg.angular_velocity.z = self.angular_command
        self.imu_pub.publish(msg)

    def _publish_odom(self, stamp) -> None:
        msg = Odometry()
        msg.header.stamp = stamp.to_msg()
        msg.header.frame_id = 'odom'
        msg.child_frame_id = 'base_link'
        msg.pose.pose.position.x = self.x_m
        msg.pose.pose.position.y = self.y_m
        qx, qy, qz, qw = yaw_to_quaternion(self.yaw_rad)
        msg.pose.pose.orientation.x = qx
        msg.pose.pose.orientation.y = qy
        msg.pose.pose.orientation.z = qz
        msg.pose.pose.orientation.w = qw
        msg.twist.twist.linear.x = self.linear_command
        msg.twist.twist.angular.z = self.angular_command
        self.odom_pub.publish(msg)

    def _publish_fix(self, stamp, lat: float, lon: float) -> None:
        msg = NavSatFix()
        msg.header.stamp = stamp.to_msg()
        msg.header.frame_id = 'gps'
        msg.status.status = NavSatStatus.STATUS_FIX
        msg.status.service = NavSatStatus.SERVICE_GPS
        msg.latitude = lat
        msg.longitude = lon
        msg.altitude = self.origin_alt
        self.fix_pub.publish(msg)

    def _publish_gps_vel(self, stamp) -> None:
        msg = Vector3Stamped()
        msg.header.stamp = stamp.to_msg()
        msg.header.frame_id = 'gps'
        msg.vector.x = self.linear_command * math.cos(self.yaw_rad)
        msg.vector.y = self.linear_command * math.sin(self.yaw_rad)
        msg.vector.z = 0.0
        self.gps_vel_pub.publish(msg)


def main(args=None) -> None:
    rclpy.init(args=args)
    node = CuadrigaMockBackend()
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