#!/usr/bin/env python3
# Copyright 2021 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from typing import Optional

import rclpy



# Node, State and Publisher are aliases for LifecycleNode, LifecycleState and LifecyclePublisher
# respectively.
# In case of ambiguity, the more explicit names can be imported.

from rclpy.lifecycle import Node
from rclpy.lifecycle import Publisher
from rclpy.lifecycle import State
from rclpy.lifecycle import TransitionCallbackReturn
from rclpy.timer import Timer
import threading
from rclpy.duration import Duration
from nav_msgs.msg import Path, Odometry
from geometry_msgs.msg import PoseStamped, Pose, Point, Quaternion
from std_msgs.msg import Float32
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import odeint
from ctl_mission.utils import compute_path_from_wp, get_ref_trajectory
from ctl_mission.cvxpy_mpc import MPC, VehicleModel
import sys
import tf2_ros
import tf_transformations
import tf2_geometry_msgs
import time

class LifecycleTalker(Node):
    """Our lifecycle talker node."""

    def __init__(self, node_name, **kwargs):
        """Construct the node."""
        super().__init__(node_name, **kwargs)
        self._count: int = 0
        self._pub: Optional[Publisher] = None
        self._timer: Optional[Timer] = None
        self.state = [0.0, 0.0, 0.0, 0.0]
        self.T_horizont = 30
        self.delta_t = 0.2
        self.max_v = 1.0
        self.control = np.zeros(2)
        self.local_path_x = [0]
        self.local_path_y = [0]
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)
        self.input_path = None
        self.input_path_timestamp = None  
        self.K = int(self.T_horizont / self.delta_t)
        self.declare_parameter('local_path_topic', 'local_path')
        self.declare_parameter('local_trajectory_topic', 'local_trajectory')
        self.declare_parameter('local_trajectory_topic_ll', 'local_trajectory_ll')
        self.declare_parameter('odometry_topic', '/fixposition/odometry_enu')
        self.declare_parameter('robot_frame', 'FP_POI')
        self.declare_parameter('local_fixed_frame', 'FP_ENU')
        self.declare_parameter('global_fixed_frame', 'ECEF')
        self.declare_parameter('node_frequency', 10.0)
        self.node_frequency = 10.0
        self.Q = [20, 20, 10, 20]  # state error cost
        self.Qf = [30, 30, 30, 30]  # state final error cost
        self.R = [10, 10]  # input cost
        self.P = [10, 10]  # input rate of change cost
        self.received_Path = Path()
        self.path_msg = None
        self.lock = threading.Lock()
        # Start a thread to periodically call pub_ll_path
        self.timer_thread = threading.Thread(target=self.start_timer)
        self.timer_thread.daemon = True
        self.timer_thread.start()

    def start_timer(self):
        while rclpy.ok():
            self.pub_ll_path()
            time.sleep(0.01)  # Adjust the time period as needed

    def odom_callback(self, msg):
        self.state[0] = msg.pose.pose.position.x
        self.state[1] = msg.pose.pose.position.y
        self.state[2] = msg.twist.twist.angular.z
        # Extract the orientation quaternion
        orientation_q = msg.pose.pose.orientation
        quaternion = (
            orientation_q.x,
            orientation_q.y,
            orientation_q.z,
            orientation_q.w
        )

        # Convert quaternion to Euler angles
        euler = tf_transformations.euler_from_quaternion(quaternion)

        # euler is a tuple (roll, pitch, yaw)
        self.state[3] = euler[2]  # yaw

    def transform_path_to_ecef(self, path):
        path_in_ecef = Path()
        path_in_ecef.header.frame_id = 'ECEF'
        path_in_ecef.header.stamp = path.header.stamp

        for pose_stamped in path.poses:
            try:
                # Transform pose from local fixed frame to ECEF frame
                pose_in_ecef = self.tf_buffer.transform(pose_stamped, 'ECEF', timeout=rclpy.duration.Duration(seconds=1))
                path_in_ecef.poses.append(pose_in_ecef)
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException) as e:
                continue  # Handle exceptions or log errors as necessary

        return path_in_ecef


    def ecef_to_geo(self, x, y, z):
        # Constants for WGS84
        a = 6378137.0  # Semi-major axis
        e_sq = 6.69437999014 * 0.001  # Square of eccentricity

        b = np.sqrt(a**2 * (1 - e_sq))  # Semi-minor axis
        ep = np.sqrt((a**2 - b**2) / b**2)
        p = np.sqrt(x**2 + y**2)
        th = np.arctan2(a * z, b * p)

        lon = np.arctan2(y, x)
        lat = np.arctan2((z + ep**2 * b * np.sin(th)**3), (p - e_sq * a * np.cos(th)**3))
        N = a / np.sqrt(1 - e_sq * np.sin(lat)**2)
        alt = p / np.cos(lat) - N

        # Convert to degrees and meters
        lon = np.degrees(lon)
        lat = np.degrees(lat)

        return lat, lon, alt

    def path_callback(self, msg):
        # Get current time from the node's clock
        now = self.get_clock().now()
        self.received_Path = msg
        # Get the time stamp of the message
        msg_time = rclpy.time.Time.from_msg(msg.header.stamp)
        self.input_path_timestamp = msg_time  # Store the time when the path was valid
        # Calculate the difference in seconds
        time_diff = now - msg_time
        
        # Define your threshold (e.g., 2 seconds)
        time_threshold = rclpy.duration.Duration(seconds=2)
        
        # Check if the message is too old
        if time_diff > time_threshold:
            self.get_logger().error('The path message is too old.')

        # Add odometry enu pose to first point
        self.local_path_x = [self.state[0]] 
        self.local_path_y = [self.state[1]]
        # No odometry enu pose to first point
        self.local_path_x = [] 
        self.local_path_y = []
        for pose_stamped in msg.poses:
            self.local_path_x.append(pose_stamped.pose.position.x)
            self.local_path_y.append(pose_stamped.pose.position.y)

        if len(self.local_path_x) > 2:
            self.input_path = compute_path_from_wp(self.local_path_x, self.local_path_y, 0.05)


    def convert_to_ros_path(self, input_path, frame_id='ENU0'):
        """
        Converts the computed path to a ROS 2 nav_msgs/Path message.

        Args:
            input_path (ndarray): Array of shape (3, N) representing the path as x, y, heading.
            frame_id (str): Frame ID for the Path message.

        Returns:
            Path: ROS 2 Path message.
        """
        path_msg = Path()
        path_msg.header.stamp = rclpy.time.Time().to_msg()
        path_msg.header.frame_id = frame_id

        for x, y, theta in zip(input_path[0], input_path[1], input_path[2]):
            pose = PoseStamped()
            pose.header.stamp = rclpy.time.Time().to_msg()
            pose.header.frame_id = frame_id
            pose.pose.position = Point(x=x, y=y, z=0.0)
            q = self.quaternion_from_euler(0, 0, theta)
            pose.pose.orientation = Quaternion(x=q[0], y=q[1], z=q[2], w=q[3])
            path_msg.poses.append(pose)
        
        return path_msg

    def quaternion_from_euler(self, roll, pitch, yaw):
        """
        Converts Euler angles to quaternion.

        Args:
            roll (float): Roll angle.
            pitch (float): Pitch angle.
            yaw (float): Yaw angle.

        Returns:
            tuple: Quaternion (x, y, z, w).
        """
        qx = np.sin(roll / 2) * np.cos(pitch / 2) * np.cos(yaw / 2) - np.cos(roll / 2) * np.sin(pitch / 2) * np.sin(yaw / 2)
        qy = np.cos(roll / 2) * np.sin(pitch / 2) * np.cos(yaw / 2) + np.sin(roll / 2) * np.cos(pitch / 2) * np.sin(yaw / 2)
        qz = np.cos(roll / 2) * np.cos(pitch / 2) * np.sin(yaw / 2) - np.sin(roll / 2) * np.sin(pitch / 2) * np.cos(yaw / 2)
        qw = np.cos(roll / 2) * np.cos(pitch / 2) * np.cos(yaw / 2) + np.sin(roll / 2) * np.sin(pitch / 2) * np.sin(yaw / 2)
        return qx, qy, qz, qw


    def pub_ll_path(self):
        with self.lock:
            if self.path_msg is None:
                return
            path_msg = self.path_msg

        ll_path = Path()
        for pose_stamped in path_msg.poses:
            try:
                pose_in_ecef = self.tf_buffer.transform(pose_stamped, 'ECEF', timeout=rclpy.duration.Duration(seconds=1))
                lat, lon, alt = self.ecef_to_geo(pose_in_ecef.pose.position.x, pose_in_ecef.pose.position.y, pose_in_ecef.pose.position.z)
                
                new_pose = PoseStamped()
                new_pose.header = ll_path.header
                new_pose.pose.position.x = lat
                new_pose.pose.position.y = lon
                new_pose.pose.position.z = alt
                ll_path.poses.append(new_pose)
            except Exception as e:
                self.get_logger().error(f'Failed to transform pose: {str(e)}')

        self.publisher_ll_.publish(ll_path)


    def publish_path(self, x_history, y_history):

        path = Path()
        path.header.frame_id = self.local_fixed_frame
        path.header.stamp = self.get_clock().now().to_msg()
        for x, y in zip(x_history, y_history):
            pose = PoseStamped()
            pose.header.frame_id = self.local_fixed_frame
            pose.header = path.header
            pose.pose.position.x = x
            pose.pose.position.y = y
            path.poses.append(pose)
        self.publisher_.publish(path)
        self.pub_ll_path(path)


    def timer_callback(self):
            start = time.time()
            if self.input_path is None:
                return
            # Check the age of the input_path
            now = self.get_clock().now()
            time_diff = now - self.input_path_timestamp
            time_threshold = Duration(seconds=2)
            if time_diff > time_threshold:
                self.get_logger().error("The stored path is too old.")
                #return
            self.path_msg = self.convert_to_ros_path(self.input_path, frame_id='FP_ENU0')
            #self.local_path_pub.publish(path_msg)
            self.publisher_.publish(self.path_msg)
            #target = get_ref_trajectory(self.state,  self.input_path , self.max_v, self.T_horizont, self.delta_t)
            self.received.publish(self.received_Path )
            #threading.Thread(target=self.pub_ll_path, args=(self.path_msg,)).start()            
            # dynamycs w.r.t robot frame
            curr_state = np.array([0, 0, 0.0, 0])
            self.control[:] = [0.5,0.5]
            #curr_state = np.array(self.state)
            #curr_state = np.array([self.state[0], self.state[1], self.state[2], self.state[3]])
            #x_mpc, u_mpc = self.mpc.step(
            #    curr_state,
            #    target,
            #    self.control,
            #    verbose=True,
            #)

            # print("CVXPY Optimization Time: {:.4f}s".format(time.time()-start))
            # only the first one is used to advance the simulation

            #self.publish_path(x_mpc.value[0,:], x_mpc.value[1,:])
            # use the optimizer output to preview the predicted state trajectory



    def on_configure(self, state: State) -> TransitionCallbackReturn:
 
        self.local_path_topic_name = self.get_parameter('local_path_topic').value
        self.local_trajectory_topic_name = self.get_parameter('local_trajectory_topic').value
        self.local_trajectory_ll_topic_name = self.get_parameter('local_trajectory_topic_ll').value
        self.path_subscriber = self.create_subscription(Path, self.local_path_topic_name , self.path_callback, 1)
        self.odometry_subscriber = self.create_subscription(Odometry, '/fixposition/odometry_enu', self.odom_callback, 1)
        self.robot_frame = self.get_parameter('robot_frame').value
        self.local_fixed_frame = self.get_parameter('local_fixed_frame').value
        self.global_fixed_frame = self.get_parameter('global_fixed_frame').value
        self.node_frequency = self.get_parameter('node_frequency').value
        self.publisher_ = self.create_publisher(Path, self.local_trajectory_topic_name , 1)
        self.publisher_ll_ = self.create_publisher(Path, self.local_trajectory_ll_topic_name , 1)
        self.received = self.create_publisher(Path, "received" , 1)
        self.local_path_pub = self.create_publisher(Path, 'computed_path', 1)
        self.get_logger().info('on_configure() is called.')
        return TransitionCallbackReturn.SUCCESS

    def on_activate(self, state: State) -> TransitionCallbackReturn:
        timer_period = 1/self.node_frequency
        self.timer = self.create_timer(timer_period, self.timer_callback)  # Adjust the time period as needed
        self.get_logger().info('on_activate() is called.')
        self.mpc = MPC(VehicleModel(), self.T_horizont, self.delta_t, self.Q, self.Qf, self.R, self.P)
        #Reinitialice the local path
        self.local_path_x = [0]
        self.local_path_y = [0]
        # The default LifecycleNode callback is the one transitioning
        # LifecyclePublisher entities from inactive to enabled.
        # If you override on_activate(), don't forget to call the parent class method as well!!
        return super().on_activate(state)

    def on_deactivate(self, state: State) -> TransitionCallbackReturn:
        self.get_logger().info('Try.')
        if self.timer:
            self.timer.cancel()
        self.get_logger().info('Timer deactivated.')
        self.get_logger().info('on_deactivate() is called.')
        # Same reasong here that for on_activate().
        # These are the two only cases where you need to call the parent method.
        return super().on_deactivate(state)

    def on_cleanup(self, state: State) -> TransitionCallbackReturn:

        self.destroy_timer(self._timer)
        self.destroy_publisher(self._pub)

        self.get_logger().info('on_cleanup() is called.')
        return TransitionCallbackReturn.SUCCESS

    def on_shutdown(self, state: State) -> TransitionCallbackReturn:

        self.destroy_timer(self._timer)
        self.destroy_publisher(self._pub)

        self.get_logger().info('on_shutdown() is called.')
        return TransitionCallbackReturn.SUCCESS


# A lifecycle node has the same node API
# as a regular node. This means we can spawn a
# node, give it a name and add it to the executor.

def main():
    rclpy.init()

    executor = rclpy.executors.SingleThreadedExecutor()
    lc_node = LifecycleTalker('mpc_planner_node')
    executor.add_node(lc_node)
    try:
        executor.spin()
    except (KeyboardInterrupt, rclpy.executors.ExternalShutdownException):
        lc_node.destroy_node()


if __name__ == '__main__':
    main()