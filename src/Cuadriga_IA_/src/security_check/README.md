# Security Check Node

The `SecurityCheckNode` is responsible for performing security checks on the rover's movements to ensure safe operations. It monitors sensor data and commands to prevent collisions and unsafe maneuvers.

## Topics

### Published Topics
- **`cmd_vel` (`geometry_msgs/msg/Twist`)**: Publishes velocity commands after performing security checks.
- **`min_distance_to_path` (`std_msgs/msg/Float32`)**: Publishes the minimum distance to the path for security checks.

### Subscribed Topics
- **`cmd_vel` (`geometry_msgs/msg/Twist`)**: Receives raw velocity commands to be checked.
- **`lidar_topic` (`sensor_msgs/msg/LaserScan`)**: Receives LIDAR scan data for obstacle detection.
- **`min_distance_to_path_topic_name` (`std_msgs/msg/Float32`)**: Receives the minimum distance to the path for security checks.

## Services

### Provided Services
- **`enable_security_check` (`std_srvs/srv/SetBool`)**: Enables or disables the security check.
- **`get_security_check` (`security_check_interfaces/srv/GetSecurityCheckStatus`)**: Gets the status of the security check.
- **`security_password_check` (`security_check_interfaces/srv/PasswordCheck`)**: Service to bypass or enable security check with a password.

## Parameters

- **`cmd_vel_topic_name`**: Name of the topic for raw velocity commands.
- **`secured_cmd_vel_topic_name`**: Name of the topic for secured velocity commands.
- **`lidar_topic`**: Name of the topic for LIDAR scan data.
- **`lidar_frame`**: Frame of the LIDAR.
- **`robot_frame`**: Frame of the robot.
- **`dissable_service_name`**: Name of the service to disable security check.
- **`threshold_1`**: Distance threshold for immediate stop.
- **`threshold_2`**: Distance threshold for gradual stopping.
- **`threshold_path`**: Threshold distance to the path.
- **`factor`**: Factor for scaling the speed limit.
- **`bypass_password`**: Password to bypass the security check.
- **`get_security_check_srv_name`**: Service name for getting security check status.
- **`password_check_srv_name`**: Service name for password check.
- **`min_distance_to_path_topic_name`**: Topic name for the minimum distance to the path.
- **`fsm.change_fsm_mode_srv_name`**: Service name to change FSM mode.
- **`fsm.get_fsm_srv_name`**: Service name to get FSM mode.


## Common Functions

- **cmdvelCallbak()**: Callback for receiving velocity commands.
- **perform_security_check()**: Performs the security check and publishes adjusted velocity commands.
- **lidarCallback()**: Callback for processing LIDAR scan data.
- **passwordServiceCallback()**: Service callback for handling password checks to bypass security.
- **getSecurity()**: Service callback to get the security check status.
- **checkChangeModeFuture()**: Checks the future for FSM mode change request.
- **sendChangeModeRequest()**: Sends a request to change the FSM mode.
- **handle_security_service()**: Handles enabling or disabling the security check via a service call.
- **fetch_fsm_mode()**: Fetches the current FSM mode.
- **check_futures()**: Checks the status of futures for FSM mode requests.
- **minDistanceCallback()**: Callback for handling minimum distance to path messages.

## Lifecycle Callbacks

- **on_configure()**: Called when the node is configured.
- **on_activate()**: Called when the node is activated.
- **on_deactivate()**: Called when the node is deactivated.
- **on_cleanup()**: Called when the node is cleaned up.
- **on_shutdown()**: Called when the node is shut down.
