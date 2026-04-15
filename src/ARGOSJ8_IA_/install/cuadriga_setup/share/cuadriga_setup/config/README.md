# cuadriga ROS2 Package Configuration

This document provides detailed information about the configuration parameters used in the cuadriga ROS2 package. Below are explanations of the parameters found in the unified configuration file for both the simulator and the real robot.

## Configuration File

- [ctrl_mission.yaml](./ctrl_mission.yaml): Unified configuration for both the simulator and the real robot.

## Configuration Parameters
The YAML file contains the configuraiton parameters for all the running nodes, along with some global parameter shared between many nodes.

### Global Parameters

- `cmd_vel_topic_name`: (string) Topic name for command velocity.
- `secured_cmd_vel_topic_name`: (string) Topic name for command velocity after the security check is performed.
- `follow_cmd_topic_name`: (string) Topic name for the external Twist used by the FollowZED FSM when a leader is detected.
- `joystick_topic_name`: (string) Topic name for joystick messages.
- `gps_topic`: (string) Topic name for GPS data.
- `ll_path_topic`: (string) Topic name for path in latitude-longitude format.
- `local_path_topic`: (string) Topic name for local path.
- `local_trajectory_topic`: (string) Topic name for local trajectory published by the locaplanner.
- `robot_frame`: (string) Name of the robot frame.
- `local_fixed_frame`: (string) Name of the local fixed frame.
- `global_fixed_frame`: (string) Name of the global fixed frame.
- `velodyne_frame`: (string) Frame name for Velodyne LIDAR.
- `sick_frame`: (string) Frame name for Sick LIDAR.
- `dead_man_button`: (int) Button number for dead man switch.
- `x_axis_button`: (int) Button number for x-axis control.
- `z_axis_button`: (int) Button number for z-axis control.
- `x_multiplier`: (float) Multiplier for x-axis control.
- `z_multiplier`: (float) Multiplier for z-axis control.
- `fsm`: (dict) FSM-related parameters.

### teleoperation_node

Handles teleoperation commands from a joystick.

- `max_joy_msg_delay`: (float) Maximum allowable delay for joystick messages.
- `queue_size`: (int) Size of the message queue.

### controller_node

Responsible for vehicle control using various control algorithms.

- `type`: (string) Control algorithm type (options: "dynamic_la_pure_pursuit", "regulated_pure_pursuit", "dynamic_pure_pursuit", "pure_pursuit").
- `controller_frequency`: (float) Frequency of the controller.
- `min_speed`: (float) Minimum speed.
- `breaking_acc`: (float) Braking acceleration.

#### pure_pursuit
- `K_ang_err`: (float) Gain for angular error.
- `K_linear_err`: (float) Gain for linear error.
- `v_forward`: (float) Forward velocity.
- `l_ahead`: (float) Look-ahead distance.
- `point_window_size`: (int) Size of the point window.

#### regulated_pure_pursuit
- `v_forward`: (float) Forward velocity.
- `l_ahead`: (float) Look-ahead distance.
- `r_min`: (float) Minimum radius.

#### dynamic_pp
- `look_ahead_distance`: (float) Look-ahead distance.
- `max_speed`: (float) Maximum speed.
- `max_ang_acc`: (float) Maximum angular acceleration.
- `max_ang_dec`: (float) Maximum angular deceleration.
- `max_lin_acc`: (float) Maximum linear acceleration.
- `max_lin_dec`: (float) Maximum linear deceleration.

#### dynamic_la_pp
- `look_ahead_v_gain`: (float) Look-ahead velocity gain.
- `max_speed`: (float) Maximum speed.
- `max_ang_acc`: (float) Maximum angular acceleration.
- `max_ang_dec`: (float) Maximum angular deceleration.
- `max_lin_acc`: (float) Maximum linear acceleration.
- `max_lin_dec`: (float) Maximum linear deceleration.
- `min_speed`: (float) Minimum speed.

### path_manager_node

Manages the write/read in a .gpx and integrate the global planner to generate safe global paths between two points`.

- `localization_method`: (string) Localization method (options: "Fixposition", "Robot_localization").
- `read_path_service`: (string) Service name to read path file with a filename.
- `return_path_service`: (string) Service name to return the robot path stored in its internal variable.
- `write_path_service`: (string) Service name to write path file specifiying a filename.
- `get_ll_path_service`: (string) Service name to get the path in latitude-longitude format.
- `fixed_frame_topic`: (string) Topic name for fixed frame.
- `ll_path_topic`: (string) Topic name for the path in latitude-longitude format.

### path_following_node

Handles path following behavior.

- `node_frequency`: (float) Frequency of the node.
- `target_distance`: (float) Length of the path that will be passed to the loccal planner.

### mpc_node

Model Predictive Control (MPC) node for local path planning.

- `test_param`: (string) Test parameter.
- `node_frequency`: (float) Frequency of the node.
- `odometry_topic`: (string) Topic name for odometry.

### cuadriga_ctrl_platform_node

Platform control node for cuadriga.

- **kinematic_parameters:**
  - `efective_radius`: (float) Effective radius.
  - `xICR`: (float) x-coordinate of Instantaneous Center of Rotation.
  - `throttle_to_percent`: (float) Throttle to percent conversion.
  - `steer_to_percent`: (float) Steer to percent conversion.
  - `component_id`: (int) Component ID.
  - `vehicle_id`: (int) Vehicle ID.
  - `station_id`: (int) Station ID.
  
- **arduino_params:**
  - `port`: (string) Arduino port.
  - `watchdog_active`: (bool) Watchdog activation status.

- **lcm_params:**
  - `lcm_config_file`: (string) LCM configuration file.

- `throttle_topic_name`: (string) Topic name for throttle commands.
- `operation_mode`: (int) Operation mode (1 for real robot, 2 for simulator).
- `self_test_active`: (bool) Self-test activation status.

### security_check_node

Node for security checks using LIDAR.

- `lidar_topic`: (string) Topic name for LIDAR data.
- `dissable_service_name`: (string) Service name to enable/disable security check.
- `threshold_1`: (float) First threshold value.
- `threshold_2`: (float) Second threshold value.
- `factor`: (float) Factor value.
- `threshold_path`: (float) Path threshold value.

### andoid_server_node

Node for handling communication with an Android server.

- `host`: (string) Host address.
- `port`: (int) Port number.
- `emergency_topic`: (string) Topic name for emergency calls.
- `gps_fix_topic`: (string) Topic name for GPS fixes.

### laser_segmentation

Node for segmenting laser scan data.

- `distance_threshold`: (float) Distance threshold for segmentation.
- `max_avg_distance_from_sensor`: (float) Maximum average distance from sensor.
- `max_points_segment`: (int) Maximum points per segment.
- `max_segment_width`: (float) Maximum segment width.
- `method_threshold`: (string) Method threshold.
- `min_avg_distance_from_sensor`: (float) Minimum average distance from sensor.
- `min_points_segment`: (int) Minimum points per segment.
- `min_segment_width`: (float) Minimum segment width.
- `noise_reduction`: (float) Noise reduction factor.
- `restore_defaults`: (bool) Restore default settings.
- `scan_topic`: (string) Topic name for laser scans.
- `segmentation_type`: (string) Type of segmentation.
- `segments_topic`: (string) Topic name for segments.
- `side_vision_limit`: (float) Side vision limit.
- `weight_factor`: (float) Weight factor.
- `angle_increment`: (float) Angle increment.
- `range_min`: (float) Minimum range.
- `range_max`: (float) Maximum range.
- `buffer_size`: (int) Buffer size.
- `detection_threshold`: (int) Detection threshold.
- `spatial_threshold`: (float) Spatial threshold.
- `inner_zone_angle`: (float) Inner zone angle.
- `middle_zone_angle`: (float) Middle zone angle.

### fixposition_driver_ros2

Configuration for the Fixposition driver.

- **fp_output:**
  - `formats`: (list) Output formats.
  - `type`: (string) Output type.
  - `port`: (string) Output port.
  - `ip`: (string) IP address.
  - `rate`: (int) Output rate.
  - `reconnect_delay`: (float) Reconnect delay.

- **customer_input:**
  - `speed_topic`: (string) Speed topic.

