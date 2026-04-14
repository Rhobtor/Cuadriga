
# Package of platform control of ROVER J8

This node is in charge of working with the robot's hardware to send it speed commands. It has three operating modes controlled by the `operation_mode` parameter. 
- 0: Arduino Mode: The robot is controlled by an Arduino that mimics the Logitech controller. 
- 1: LCM Mode: The ARGO J8 robot uses a [LCM](https://lcm-proj.github.io/lcm/#) set of libraries and tools for message passing and data marshaling. The node uses this library to command the robot. 
- 2: Gazebo Mode: This mode simulates the robot behavior in gazebo. 

# Topics

- Subcriptors: `secured_cmd_vel_topic_name` with message [Twist](https://docs.ros.org/en/noetic/api/geometry_msgs/html/msg/Twist.html)
- Publishers: `cmd_throttle_name` with message [CmdThrottleMsg](https://github.com/Robotics-Mechatronics-UMA/argj801_ctl_platform/blob/master/argj801_ctl_platform_interfaces/msg/CmdThrottleMsg.msg)
- Publishers: `kinematic_debug_topic_name` with message `KinematicDebugMsg` para exponer el estado que antes solo salia por log (`throttle`, `steering`, aceleraciones solicitadas y flags de limitacion)

# Services
- emergency_stop: deactivate the node to stop sending speed commands. 
- set_velocity: set the velocity of the rover. Receives an integer between 0 and 5 (can be limited between 0 and 2 for safety reasons). Only available in Arduino Mode. 
- ping: ping to the arduino to test if it is alive. Only available in Arduino Mode
- get_velocity: get the velocity of the rover. The value could be between 0 and 5 (can be limited between 0 to 2 for safety reasons). Only available in Arduino Mode.


# Params
- secured_cmd_vel_topic_name: the topic to which the node subscribes to receive the velocity commands. 
- operation_mode: indicates the mode in which the node works.
  - 0: Arduino Mode: The robot is controlled by an Arduino that mimics the Logitech controller. 
  - 1: LCM Mode: The ARGO J8 robot uses a [LCM](https://lcm-proj.github.io/lcm/#) set of libraries and tools for message passing and data marshaling. The node uses this library to command the robot. 
  - 2: Gazebo Mode: This mode simulates the robot's behavior in the gazebo. 
- port: Serial port to communicate with Arduino. This param only works in Arduino mode.
- self_test_active: Active the self_test services. For safety, the self_test can be disabled when the rover works on a mission. Until now, this param has only worked in Arduino mode.
- watchdog_active: Active is the watchdog that terminates the node if no message arrives on the topic. It can be deactivated to debug issues. This param only works in Arduino mode.
- lcm_config_file: config file of [argj801_lcm](https://github.com/Robotics-Mechatronics-UMA/argj801_lcm) package.
- throttle_topic_name: name of the topic in which the node publishes the speed commands in Gazebo mode. 
- kinematic_debug_topic_name: name of the topic in which the node publishes the internal kinematic debug state in LCM and Gazebo modes.
- efective_radius: effective wheel radius.
- xICR: X-Coordinate of the Instantaneous Rotation Center
- throttle_to_percent: factor to convert from angular velocity to throttle motor percentage.
- steer_to_percent: factor to go from angular velocity to steering motor percentage.

# Test
To check the working rate of the IMU, it is possible to use the ../diagnostics topic.

To make a node test, use:

`ros2 service call /self_test diagnostic_msgs/srv/SelfTest`

with the node running.
