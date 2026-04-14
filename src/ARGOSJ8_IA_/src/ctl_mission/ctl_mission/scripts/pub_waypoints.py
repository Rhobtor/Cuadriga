#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import String
import os
from ament_index_python.packages import get_package_share_directory
class MinimalPublisher(Node):

	def __init__(self):
		super().__init__('minimal_publisher')
		self.publisher_ = self.create_publisher(Float32MultiArray, '/RoverJ8/waypoints', 10)
		timer_period = 0.5  # seconds
		self.timer = self.create_timer(timer_period, self.timer_callback)
		self.i = 0
		cwd = os.getcwd()
		file_path = os.path.join(get_package_share_directory('argj801_ctl_mission'),'waypoints_UTM.txt')
		print(file_path)
		file1 = open(file_path, 'r')
		Lines = file1.readlines()
		count = 0
		# Strips the newline character
		v_lat = []
		v_lon = []
		for line in Lines:
			count += 1
			for coord in line.split():
				if count == 1:
					v_lat.append(float(coord))
				if count == 2:
					v_lon.append(float(coord))
		self.v_combined = []
		for n in range(len(v_lat)):
			self.v_combined.append(v_lat[n])
			self.v_combined.append(v_lon[n])

	def timer_callback(self):
		msg = Float32MultiArray()
		msg.data = self.v_combined
		self.publisher_.publish(msg)

		self.i += 1


def main(args=None):
	rclpy.init(args=args)

	minimal_publisher = MinimalPublisher()

	rclpy.spin(minimal_publisher)

	# Destroy the node explicitly
	# (optional - otherwise it will be done automatically
	# when the garbage collector destroys the node object)
	minimal_publisher.destroy_node()
	rclpy.shutdown()


if __name__ == '__main__':
	main()




