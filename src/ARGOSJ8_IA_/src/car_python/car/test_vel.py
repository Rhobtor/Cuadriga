#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
ROS2 node para probar valores de throttle y steering
Publica CmdThrottleMsg y mide respuesta desde Odometry, guardando resultados en CSV.
"""
import os
import csv
import math
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from argj801_ctl_platform_interfaces.msg import CmdThrottleMsg

# Parámetros de prueba\N
MAX = 2  # número de pasos en cada dirección
MAX_THROTTLE = 500.0
MAX_STEERING = 500.0
MEASURE_DELAY = 0.5  # ciclos de timer antes de medir (intervalo de timer=1s)


def quaternion_to_yaw(q):
    """Convierte quaternion a yaw en radianes."""
    siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
    return math.atan2(siny_cosp, cosy_cosp)


def normalize_angle(a):
    """Normaliza ángulo al rango (-pi, pi]."""
    return (a + math.pi) % (2.0 * math.pi) - math.pi


class CmdTesterNode(Node):
    def __init__(self):
        super().__init__('cmd_tester')
        # Suscripción a odometría\N        
        self.odom_sub = self.create_subscription(
            Odometry,
            '/ARGJ801/odom_demo',
            self.odom_callback,
            10
        )
        # Publicador de comandos\N 
        self.cmd_pub = self.create_publisher(
            CmdThrottleMsg,
            '/ARGJ801/cmd_throttle_msg',
            10
        )
        # Timer para avance de prueba cada 1s\N        
        self.timer = self.create_timer(1.0, self.timer_callback)

        # Generar valores de prueba
        self.throttle_values = [i * MAX_THROTTLE / MAX for i in range(-MAX, MAX + 1)]
        self.steering_values = [i * MAX_STEERING / MAX for i in range(-MAX, MAX + 1)]
        self.phase = 'throttle'  # 'throttle' o 'steering'
        self.index = 0
        self.testing = False
        self.wait_count = 0
        self.start_odom = None
        self.last_odom = None

        # Preparar archivo CSV
        home = os.getenv('HOME', '.')
        self.log_path = os.path.join(home, 'cmd_test_results.csv')
        self.log_file = open(self.log_path, 'w', newline='')
        self.csv_writer = csv.writer(self.log_file)
        self.csv_writer.writerow(['phase', 'value', 'delta_x', 'delta_y', 'delta_yaw'])

        self.get_logger().info(f'Resultados se guardarán en: {self.log_path}')

    def odom_callback(self, msg: Odometry):
        """Callback de odometría: guarda el último mensaje."""
        self.last_odom = msg

    def timer_callback(self):
        """Envía comandos y mide respuesta cada ciclo."""
        # Si hemos terminado todas las pruebas
        if self.phase == 'done':
            self.get_logger().info('Pruebas completadas, cerrando nodo.')
            self.log_file.close()
            rclpy.shutdown()
            return

        # Seleccionar lista de valores según fase
        values = self.throttle_values if self.phase == 'throttle' else self.steering_values
        if self.index >= len(values):
            # Cambiar de fase o terminar
            if self.phase == 'throttle':
                self.phase = 'steering'
                self.index = 0
                self.get_logger().info('Cambiando a prueba de steering')
            else:
                self.phase = 'done'
            return

        val = values[self.index]
        # Si no estamos midiendo, publicamos el comando
        if not self.testing:
            if self.last_odom is None:
                self.get_logger().warning('Aún no hay datos de odometría')
                return
            self.start_odom = self.last_odom
            cmd = CmdThrottleMsg()
            if self.phase == 'throttle':
                cmd.throttle = float(val)
                cmd.steering = 0.0
            else:
                cmd.throttle = 0.0
                cmd.steering = float(val)
            self.cmd_pub.publish(cmd)
            self.testing = True
            self.wait_count = 0
            self.get_logger().info(f'Iniciando prueba {self.phase}: {val:.1f}')
        else:
            # Esperar MEASURE_DELAY ciclos antes de medir
            self.wait_count += 1
            if self.wait_count >= MEASURE_DELAY:
                end = self.last_odom
                dx = end.pose.pose.position.x - self.start_odom.pose.pose.position.x
                dy = end.pose.pose.position.y - self.start_odom.pose.pose.position.y
                yaw0 = quaternion_to_yaw(self.start_odom.pose.pose.orientation)
                yaw1 = quaternion_to_yaw(end.pose.pose.orientation)
                dyaw = normalize_angle(yaw1 - yaw0)
                # Registrar en CSV
                self.csv_writer.writerow([self.phase, val, dx, dy, dyaw])
                self.get_logger().info(
                    f'Registrado {self.phase}={val:.1f}: ' 
                    f'Δx={dx:.3f}, Δy={dy:.3f}, Δyaw={dyaw:.3f}'
                )
                self.testing = False
                self.index += 1


def main(args=None):
    rclpy.init(args=args)
    node = CmdTesterNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.log_file.close()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
