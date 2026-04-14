#!/usr/bin/env python3
import math, rclpy
from rclpy.node import Node
from geometry_msgs.msg import Point
from nav_msgs.msg import Odometry
from octomap_msgs.srv import BoundingBoxQuery as BBox        # srv que expone octomap_server

RADIUS  = 20.0   # [m] radio de mapa que conservamos
Z_MIN   = -2.0   # [m] suelo
Z_MAX   =  6.0   # [m] techo

class RollingCleaner(Node):
    def __init__(self):
        super().__init__('octomap_rolling_cleaner')

        # --- posición actual (x,y) via odometría --------------------
        self.pose_xy = None
        self.create_subscription(
            Odometry,
            '/ARGJ801/odom_demo',
            lambda msg: self._cb_odom(msg),
            10)

        # --- cliente del servicio clear_bbox del octomap ------------
        self.cli = self.create_client(
            BBox,
            '/octomap_local/clear_bbox')          # << mismo nombre que en launch

        # timer que se dispara cada 0.5 s
        self.create_timer(0.5, self._tick)

    def _cb_odom(self, msg: Odometry):
        self.pose_xy = (msg.pose.pose.position.x,
                        msg.pose.pose.position.y)

    def _tick(self):
        if self.pose_xy is None:
            self.get_logger().debug("Esperando odometría…")
            return
        if not self.cli.service_is_ready():
            self.get_logger().debug("Esperando servicio clear_bbox…")
            return

        x, y = self.pose_xy
        req = BBox.Request()
        req.min = Point(x=x-RADIUS, y=y-RADIUS, z=Z_MIN)
        req.max = Point(x=x+RADIUS, y=y+RADIUS, z=Z_MAX)

        self.cli.call_async(req)          # no bloqueamos el ciclo
        self.get_logger().debug("Enviado clear_bbox")

def main():
    rclpy.init()
    rclpy.spin(RollingCleaner())

if __name__ == '__main__':
    main()
