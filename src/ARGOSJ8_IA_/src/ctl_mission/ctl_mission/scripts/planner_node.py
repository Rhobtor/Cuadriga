#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from argj801_ctl_mission.planner import *

from nav_msgs.msg import Path
from geometry_msgs.msg import Pose, PoseStamped
from nav_msgs.msg import Odometry
from trajectory_msgs.msg import JointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint

import numpy as np

class PlannerNode(Node):
    def __init__(self):
        super().__init__('planner_node')
        self.path_to_follow = np.empty((3, 3))
        self.Trajectory = JointTrajectory()
        self.Trajectory.header.frame_id = 'odom'
        self.Trajectory.joint_names = ['x', 'y', 'yaw']
        self.Trajectory_visual = Path()
        self.Trajectory_visual.header.frame_id = 'odom'
        self.planner = Planner(a_max=3, N=3)
        self.path_revieved = False
        self.odom_revieved = False
        self.path_subscriber = self.create_subscription(Path,'/ARGJ801/path_planner',self.path_planner_callback,10)
        self.odom_subscriber = self.create_subscription(Odometry,'/ARGJ801/odometry/filtered',self.odom_callback,10)
        self.trajectory_visual_publisher = self.create_publisher(Path,'/ARGJ801/trajectory_visual',10)
        self.trajectory_publisher = self.create_publisher(JointTrajectory,'/ARGJ801/trajectory',10)
        
        self.timer = self.create_timer(0.01, self.timer_callback)

    
    def path_planner_callback(self,msg):
        self.get_logger().info('Path received')
        self.path_revieved = True
        for i in range(3):
            self.path_to_follow[0,i] = msg.poses[i].pose.position.x
            self.path_to_follow[1,i] = msg.poses[i].pose.position.y
            self.path_to_follow[2,i] = msg.poses[i].pose.position.z        
                
    def odom_callback(self,msg):
        self.get_logger().info('Odometry received')
        self.odom_revieved = True

        if self.path_revieved:
            
            actualState = np.array([msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z,msg.twist.twist.linear.x,msg.twist.twist.linear.y,msg.twist.twist.linear.z])
            
            self.planner.reset(actualState,self.path_to_follow)
            self.planner.initializations(0)
            self.pmm_trajectory = self.planner.optimizeTraj()
            
            # l, p, v = arc_len_3d_points(pmm_trajectory)
            # trajectory_visual = Path()
            # trajectory_visual.header.frame_id = 'odom'
            # trajectory_visual.header.stamp = self.get_clock().now().to_msg()
            # for i in range(len(l)):
            #     trajectory = JointTrajectoryPoint()
            #     trajectory.positions.append(p[:,i])
            #     trajectory.velocities.append(v[:,i])
            #     trajectory.time_from_start = rclpy.time.Duration.from_msg(l[i])
            #     self.Trajectory.points.append(trajectory)
            #     pos = Pose()
            #     pos.position.x = p[0,i]
            #     pos.position.y = p[1,i]
            #     pos.position.z = p[2,i]
            #     trajectory_visual.poses.append(pos)
            
            # self.trajectory_visual_publisher.publish(trajectory_visual)
            # self.trajectory_publisher.publish(self.Trajectory)
            # self.Trajectory.points.clear()
        else:
            self.get_logger().info('No path received')
            
    def timer_callback(self):
        if  hasattr(self, 'pmm_trajectory'):
            self.get_logger().info('Timer cb')
            l, p, v = utils.arc_len_3d_points(self.pmm_trajectory)
            for i in range(len(l)):
                trajectory = JointTrajectoryPoint()
                print(p[0,i])
                print(p[1,i])
                print(v[0,i])
                print(v[1,i])

                trajectory.positions.append(p[0,i])  # X position
                trajectory.positions.append(p[1,i])  # Y position
                trajectory.positions.append(np.arctan2(v[1,i],v[0,i]))  # Orientation (yaw)
                #trajectory.time_from_start = 0.1
                self.Trajectory.points.append(trajectory)
                pos = PoseStamped()
                pos.pose.position.x = p[0,i]
                pos.pose.position.y = p[1,i]
                pos.pose.position.z = 0.0
                self.Trajectory_visual.poses.append(pos)
            
            self.Trajectory_visual.header.stamp = self.get_clock().now().to_msg()
            self.Trajectory.header.stamp = self.get_clock().now().to_msg()
            self.trajectory_visual_publisher.publish(self.Trajectory_visual)
            self.trajectory_publisher.publish(self.Trajectory)
            self.Trajectory.points.clear()
            self.Trajectory_visual.poses.clear()
        

def main(args=None):
    rclpy.init(args=args)
    planner_node = PlannerNode()
    rclpy.spin(planner_node)
    planner_node.destroy_node()
    rclpy.shutdown()
    
    
    
if __name__ == '__main__':
    main()
