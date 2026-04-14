#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""cmd_vel_to_throttle

Nodo ROS 2 (rclpy) que adapta comandos high-level `geometry_msgs/Twist` (cmd_vel)
al mando low-level `argj801_ctl_platform_interfaces/msg/CmdThrottleMsg`.

Motivación
---------
En el stack preparado originalmente para Unity suele usarse `/cmd_vel`.
En Gazebo, esta plataforma se mueve con `/ARGJ801/cmd_throttle_msg`.

Contrato (por defecto)
----------------------
- Sub:  `~/cmd_vel_in` (geometry_msgs/Twist)
- Pub:  `~/cmd_throttle_out` (argj801_ctl_platform_interfaces/CmdThrottleMsg)

Mapeo:
- throttle = linear.x * throttle_scale   (y luego clamp)
- steering = -angular.z * steering_scale (y luego clamp)

Seguridad:
- Si no llegan mensajes en `timeout_s`, publica 0 (throttle=0, steering=0).
"""

from __future__ import annotations

from dataclasses import dataclass

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile
from rclpy.time import Time

from geometry_msgs.msg import Twist
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg


def _clamp(x: float, lo: float, hi: float) -> float:
    return float(max(lo, min(hi, x)))


@dataclass(frozen=True)
class _Limits:
    throttle_min: float
    throttle_max: float
    steering_min: float
    steering_max: float


class CmdVelToThrottleNode(Node):
    def __init__(self) -> None:
        super().__init__('cmd_vel_to_throttle')

        # Topics (relativos al namespace del nodo para permitir remaps fáciles)
        self.declare_parameter('cmd_vel_in', 'cmd_vel')
        self.declare_parameter('cmd_throttle_out', '/ARGJ801/cmd_throttle_msg')

        # Escalas (Twist -> CmdThrottleMsg)
        # Nota: en este repo hay ejemplos usando rangos ~[-500, 500].
        self.declare_parameter('throttle_scale', 200.0)
        self.declare_parameter('steering_scale', 200.0)

        # Límites
        self.declare_parameter('throttle_min', -200.0)
        self.declare_parameter('throttle_max', 200.0)
        self.declare_parameter('steering_min', -200.0)
        self.declare_parameter('steering_max', 200.0)

        # Timeout de seguridad
        self.declare_parameter('timeout_s', 0.5)
        self.declare_parameter('publish_rate_hz', 50.0)

        cmd_vel_in = self.get_parameter('cmd_vel_in').get_parameter_value().string_value
        cmd_throttle_out = self.get_parameter('cmd_throttle_out').get_parameter_value().string_value

        qos = QoSProfile(depth=10)
        self._pub = self.create_publisher(CmdThrottleMsg, cmd_throttle_out, qos)
        self._sub = self.create_subscription(Twist, cmd_vel_in, self._on_cmd_vel, qos)

        self._last_cmd_time: Time | None = None
        self._last_cmd: Twist | None = None

        publish_rate_hz = float(self.get_parameter('publish_rate_hz').value)
        publish_period = 1.0 / publish_rate_hz if publish_rate_hz > 0 else 0.02
        self._timer = self.create_timer(publish_period, self._on_timer)

        self.get_logger().info(
            "Convertidor activo: Twist('%s') -> CmdThrottleMsg('%s')"
            % (cmd_vel_in, cmd_throttle_out)
        )

    def _limits(self) -> _Limits:
        return _Limits(
            throttle_min=float(self.get_parameter('throttle_min').value),
            throttle_max=float(self.get_parameter('throttle_max').value),
            steering_min=float(self.get_parameter('steering_min').value),
            steering_max=float(self.get_parameter('steering_max').value),
        )

    def _on_cmd_vel(self, msg: Twist) -> None:
        self._last_cmd = msg
        self._last_cmd_time = self.get_clock().now()

    def _on_timer(self) -> None:
        timeout_s = float(self.get_parameter('timeout_s').value)
        now = self.get_clock().now()

        # Si no hay cmd todavía, no publiques nada (evita spam de ceros al arrancar)
        if self._last_cmd is None or self._last_cmd_time is None:
            return

        age = (now - self._last_cmd_time).nanoseconds * 1e-9
        if timeout_s > 0.0 and age > timeout_s:
            out = CmdThrottleMsg()
            out.throttle = 0.0
            out.steering = 0.0
            self._pub.publish(out)
            return

        throttle_scale = float(self.get_parameter('throttle_scale').value)
        steering_scale = float(self.get_parameter('steering_scale').value)
        lim = self._limits()

        vx = float(self._last_cmd.linear.x)
        wz = float(self._last_cmd.angular.z)

        out = CmdThrottleMsg()
        out.throttle = _clamp(vx * throttle_scale, lim.throttle_min, lim.throttle_max)
        # Nota: invertimos el signo porque en esta plataforma el sentido del giro
        # (steering) está invertido respecto a la convención típica de cmd_vel.
        out.steering = _clamp((-wz) * steering_scale, lim.steering_min, lim.steering_max)
        self._pub.publish(out)


def main(args: list[str] | None = None) -> None:
    rclpy.init(args=args)
    node = CmdVelToThrottleNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
