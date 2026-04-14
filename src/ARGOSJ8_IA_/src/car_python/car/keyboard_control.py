#!/usr/bin/env python3
import sys
import time
import select
import termios
import tty

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist


HELP = """
Controles (terminal activo):
  W / ↑ : avanzar  (+linear.x)
  S / ↓ : retroceder (-linear.x)
  A / ← : girar izq (+angular.z)
  D / → : girar der (-angular.z)
  ESPACIO: parar (linear=0, angular=0)
  H: ayuda
  Q: salir

Publicando en /cmd_vel
"""

# Secuencias ANSI de flechas
ARROW_UP = "\x1b[A"
ARROW_DOWN = "\x1b[B"
ARROW_RIGHT = "\x1b[C"
ARROW_LEFT = "\x1b[D"


class RawTerminal:
    """Pone stdin en modo raw y lo restaura al salir."""
    def __init__(self):
        self.fd = sys.stdin.fileno()
        self.old_settings = termios.tcgetattr(self.fd)

    def __enter__(self):
        tty.setraw(self.fd)
        return self

    def __exit__(self, exc_type, exc, tb):
        termios.tcsetattr(self.fd, termios.TCSADRAIN, self.old_settings)


def read_key_nonblocking():
    """Lee una tecla sin bloquear. Devuelve None si no hay nada.
    Soporta flechas (devuelve '\x1b[A', etc.)"""
    if not select.select([sys.stdin], [], [], 0)[0]:
        return None

    ch1 = sys.stdin.read(1)
    if ch1 == '\x1b':
        # Posible secuencia ESC [
        if select.select([sys.stdin], [], [], 0)[0]:
            ch2 = sys.stdin.read(1)
            if ch2 == '[' and select.select([sys.stdin], [], [], 0)[0]:
                ch3 = sys.stdin.read(1)
                return '\x1b[' + ch3
        return '\x1b'
    return ch1


def clamp(v, vmin, vmax):
    return max(vmin, min(vmax, v))


class KeyboardCmdVelNode(Node):
    def __init__(self):
        super().__init__('keyboard_cmd_vel')

        # Parámetros
        self.topic = self.declare_parameter('cmd_topic', '/cmd_vel').value
        self.rate_hz = float(self.declare_parameter('rate_hz', 20.0).value)

        # Incrementos por pulsación
        self.step_linear = float(self.declare_parameter('step_linear', 0.05).value)     # m/s
        self.step_angular = float(self.declare_parameter('step_angular', 0.15).value)   # rad/s

        # Saturaciones
        self.max_linear = float(self.declare_parameter('max_linear', 1.0).value)        # m/s
        self.max_angular = float(self.declare_parameter('max_angular', 2.0).value)      # rad/s

        # Decaimiento opcional hacia 0
        self.auto_decay = bool(self.declare_parameter('auto_decay', False).value)
        self.decay_linear_per_s = float(self.declare_parameter('decay_linear_per_s', 0.0).value)
        self.decay_angular_per_s = float(self.declare_parameter('decay_angular_per_s', 0.0).value)

        self.print_every_s = float(self.declare_parameter('print_every_s', 0.5).value)

        self.pub = self.create_publisher(Twist, self.topic, 10)

        self.linear_x = 0.0   # m/s
        self.angular_z = 0.0  # rad/s

        self._last_print = 0.0
        self._last_time = time.monotonic()
        self._exit_requested = False

        self.get_logger().info(f"Iniciado control por teclado. Publicando en {self.topic}")
        self.get_logger().info(HELP.strip())

        period = 1.0 / max(1e-3, self.rate_hz)
        self.timer = self.create_timer(period, self._tick)

    def _tick(self):
        # 1) Leer teclas pendientes
        key_pressed = False
        while True:
            k = read_key_nonblocking()
            if k is None:
                break
            key_pressed = True
            self._handle_key(k)

        # Si se pidió salir, paramos y cerramos
        if self._exit_requested:
            self._publish_stop()
            rclpy.shutdown()
            return

        # 2) Decaimiento opcional
        now = time.monotonic()
        dt = now - self._last_time
        self._last_time = now
        if self.auto_decay and dt > 0.0:
            self._apply_decay(dt)

        # 3) Publicar cmd_vel
        self._publish_cmd()

        # 4) Log de estado
        if key_pressed or (now - self._last_print) >= self.print_every_s:
            self._last_print = now
            self.get_logger().info(
                f"linear.x: {self.linear_x:+.3f} m/s | angular.z: {self.angular_z:+.3f} rad/s"
            )

    def _apply_decay(self, dt):
        def decay(value, rate_per_s):
            if rate_per_s <= 0.0:
                return value
            delta = rate_per_s * dt
            if value > 0.0:
                return max(0.0, value - delta)
            if value < 0.0:
                return min(0.0, value + delta)
            return value

        self.linear_x = decay(self.linear_x, self.decay_linear_per_s)
        self.angular_z = decay(self.angular_z, self.decay_angular_per_s)

    def _handle_key(self, k: str):
        k_low = k.lower()

        if k_low == 'q':
            self.get_logger().info("Saliendo por 'q'…")
            self._exit_requested = True
            return

        if k_low == 'h':
            self.get_logger().info(HELP.strip())
            return

        if k_low == ' ':
            self.linear_x = 0.0
            self.angular_z = 0.0
            return

        # Avance/retroceso
        if k_low == 'w' or k == ARROW_UP:
            self.linear_x = clamp(self.linear_x + self.step_linear, -self.max_linear, self.max_linear)
        elif k_low == 's' or k == ARROW_DOWN:
            self.linear_x = clamp(self.linear_x - self.step_linear, -self.max_linear, self.max_linear)

        # Giro (convención ROS: +z = izquierda, -z = derecha)
        if k_low == 'a' or k == ARROW_LEFT:
            self.angular_z = clamp(self.angular_z + self.step_angular, -self.max_angular, self.max_angular)
        elif k_low == 'd' or k == ARROW_RIGHT:
            self.angular_z = clamp(self.angular_z - self.step_angular, -self.max_angular, self.max_angular)

    def _publish_cmd(self):
        msg = Twist()
        msg.linear.x = float(self.linear_x)
        msg.angular.z = float(self.angular_z)
        self.pub.publish(msg)

    def _publish_stop(self):
        msg = Twist()
        self.pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardCmdVelNode()

    try:
        with RawTerminal():
            rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        # Enviar stop al salir
        try:
            node._publish_stop()
        except Exception:
            pass

        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
