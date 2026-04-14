
# FSM States

This directory contains the implementation of the various states for the Finite State Machine (FSM) controlling the ARGJ801 rover. Each state is responsible for a specific functionality of the robot.

## States

1. [Back Home Node](#back-home-node)
2. [Control Node](#control-node)
3. [E-stop Node](#e-stop-node)
4. [Path Following Node](#path-following-node)
5. [Path Record Node](#path-record-node)
6. [Ready Node](#ready-node)
7. [Teleoperation Node](#teleoperation-node)

### Back Home Node

The `back_home_node.cpp` file implements the functionality for returning the rover to a predefined home location (**TO BE IMPLEMENTED**).

- **Published Topics:**
  - `cmd_vel` (`geometry_msgs/Twist`): Publishes velocity commands to control the rover.
- **Subscribed Topics:**
  - None
- **Services:**
  - None

### Control Node

The `Ctrl_node.cpp` file contains  implementation for the path tracking algorithm for the robot.

- **Published Topics:**
      - `look_ahead_distance` (`std_msgs/msg/Float32`): Distance to the look-ahead point.
      - `look_ahead_point` (`std_msgs/msg/Int32`): Index of the look-ahead point in the path.
      - `cte` (`std_msgs/msg/Float32`): Cross-track error.
      - `la_distance` (`std_msgs/msg/Float32`): Look-ahead distance.
      - `pose_stamped` (`geometry_msgs/msg/PoseStamped`): Current pose of the rover.
      - `ctrl_local_path` (`nav_msgs/msg/Path`): Local path for control purposes.
      - `cmd_vel` (`geometry_msgs/msg/Twist`): Velocity commands for the rover.
- **Subscribed Topics:**
      - `trajectoryToFollow` (`nav_msgs/msg/Path`): The trajectory the rover should follow.
      - `dist_last_obj` (`std_msgs/msg/Float32`): Distance to the last object detected.
      - `odometry` (`nav_msgs/msg/Odometry`): Odometry information for the rover.
- **Services:**
      - `config_controller` (`argj801_ctl_mission_interfaces/srv/ConfigController`): Service to configure the controller parameters.
      - `change_controller_type` (`argj801_ctl_mission_interfaces/srv/ChangeController`): Service to change the controller type.
      - `config_pure_pursuit` (`argj801_ctl_mission_interfaces/srv/ConfigPurePursuitCtrl`): Service to configure the pure pursuit controller.
      - `config_dynamic_pure` (`argj801_ctl_mission_interfaces/srv/ConfigDynamicPureCtrl`): Service to configure the dynamic pure pursuit controller.
      - `config_dynamic_la_pure` (`argj801_ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl`): Service to configure the dynamic look-ahead pure pursuit controller.
      - `config_regulated_pure` (`argj801_ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl`): Service to configure the regulated pure pursuit controller.
    
- ** Parameters**:
    - `local_trajectory_topic`: The topic name for the local trajectory.
    - `cmd_vel_topic_name`: Name of the topic to publish velocity commands.
    - `robot_frame`: The frame of reference for the robot.
    - `local_fixed_frame`: The fixed frame used locally.
    - `regulated_pure_pursuit.l_ahead`: Look-ahead distance for the regulated pure pursuit controller.
    - `regulated_pure_pursuit.v_forward`: Forward velocity for the regulated pure pursuit controller.
    - `regulated_pure_pursuit.r_min`: Minimum radius for the regulated pure pursuit controller.
    - `pure_pursuit.v_forward`: Forward velocity for the pure pursuit controller.
    - `pure_pursuit.l_ahead`: Look-ahead distance for the pure pursuit controller.
    - `dynamic_pp.look_ahead_distance`: Look-ahead distance for the dynamic pure pursuit controller.
    - `dynamic_pp.max_speed`: Maximum speed for the dynamic pure pursuit controller.
    - `dynamic_pp.max_ang_acc`: Maximum angular acceleration for the dynamic pure pursuit controller.
    - `dynamic_pp.max_ang_dec`: Maximum angular deceleration for the dynamic pure pursuit controller.
    - `dynamic_pp.max_lin_acc`: Maximum linear acceleration for the dynamic pure pursuit controller.
    - `dynamic_pp.max_lin_dec`: Maximum linear deceleration for the dynamic pure pursuit controller.
    - `controller_frequency`: Frequency at which the controller operates.
    - `dynamic_la_pp.look_ahead_v_gain`: Look-ahead velocity gain for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.max_speed`: Maximum speed for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.max_ang_acc`: Maximum angular acceleration for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.max_ang_dec`: Maximum angular deceleration for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.max_lin_acc`: Maximum linear acceleration for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.max_lin_dec`: Maximum linear deceleration for the dynamic look-ahead pure pursuit controller.
    - `dynamic_la_pp.min_speed`: Minimum speed for the dynamic look-ahead pure pursuit controller.
    - `min_speed`: Minimum speed for the controller.
    - `breaking_acc`: Braking acceleration for the controller.

A detailed overview of the implemented controllers is included [here](../controllers/README.md).

### E-stop Node

The `estop_node.cpp` file implements the emergency stop functionality, which halts the rover when an error is detected.

- **Published Topics:**
    - `cmd_vel` (`geometry_msgs/Twist`): Publishes zero velocity commands to stop the rover.
- **Subscribed Topics:**
    - None
- **Services:**
    - None



### Path Following Node

The `path_following_node.cpp` file, along with `mpc_node.py` from the `scripts` folder, implements the path following functionality. The MPC (Model Predictive Control) node is responsible for computing the optimal control actions for the rover to follow a given path.

- **Published Topics:**
      - `lifecycle_chatter` (`std_msgs/msg/String`): Lifecycle state information.
      - `local_path` (`nav_msgs/msg/Path`): Local path for the rover.
      - `poi_path` (`nav_msgs/msg/Path`): Path of interest.
      - `ecef_path` (`nav_msgs/msg/Path`): ECEF path.
      - `enu_path` (`nav_msgs/msg/Path`): ENU path.
      - `dist_last_obj` (`std_msgs/msg/Float32`): Distance to the last object.
      - `min_distance_to_path` (`std_msgs/msg/Float32`): Minimum distance to the path.
      - `cmd_vel` (`geometry_msgs/msg/Twist`): Publishes velocity commands from a joystick. For debugging.
- **Subscribed Topics:**
      - `odometry` (`nav_msgs/msg/Odometry`): Odometry information for the rover.
      - `path` (`nav_msgs/msg/Path`): Path to be followed by the rover.
      - `joy` (`sensor_msgs/msg/Joy`): Joystick inputs for controlling the rover. For debugging.
      - `fixposition/navsatfix` (`sensor_msgs/msg/NavSatFix`): GPS fix information.
- **Services:**
      - `path_manager_interfaces/srv/GetFixFramePath`: Service to get the fixed frame path from the path_manage package.
      - `argj801_ctl_mission_interfaces/srv/ChangeMode`: Service to change the FSM mode.

- **Parameters**:
    - `namespace.robot_name`: The name of the robot namespace.
    - `path_service_name`: The service name for the path.
    - `robot_frame`: The frame of reference for the robot.
    - `local_fixed_frame`: The fixed frame used locally.
    - `global_fixed_frame`: The fixed frame used globally.
    - `path_params.look_points_ahead`: Number of points to look ahead on the path.
    - `cmd_vel_topic_name`: Name of the topic to publish velocity commands.
    - `target_distance`: The target distance for path following.
    - `node_frequency`: The frequency at which the node operates.
    - `change_fsm_mode_srv_name`: The service name to change the FSM mode.
    


### Path Record Node

The `path_record_node.cpp` file implements the functionality for recording the path traversed by the rover.

- **Published Topics:**
      - `cmd_vel` (`geometry_msgs/msg/Twist`): Publishes velocity commands based on joystick input for recording paths.
      - `ll_path` (`nav_msgs/msg/Path`): Publishes the recorded path in the local level frame.
- **Subscribed Topics:**
      - `joy` (`sensor_msgs/msg/Joy`): Joystick inputs for controlling the rover.
      - `gps_topic` (`sensor_msgs/msg/NavSatFix`): GPS fix information.
- **Services:**
      - None

- **Parameters:**
    - `distance_between_record_points`: Distance between recorded points.
    - `gps_topic`: Name of the topic to subscribe to for GPS information.
    - `cmd_vel_topic_name`: Name of the topic to publish velocity commands.
    - `ll_path_topic`: Name of the topic to publish the recorded path.
    - `queue_size`: The size of the message queue.
    - `dead_man_button`: The button used as the dead man switch.
    - `x_axis_button`: The button used for the x-axis control.
    - `z_axis_button`: The button used for the z-axis control.
    - `joy_topic`: Name of the topic to subscribe to for joystick inputs.
    - `x_multiplier`: Multiplier for the x-axis control.
    - `z_multiplier`: Multiplier for the z-axis control.



### Ready Node

The `ready_node.cpp` file implements the functionality for the ready state, where the rover waits for commands to transition to other states.

- **Published Topics:**
    - `cmd_vel` (`geometry_msgs/Twist`): Publishes zero velocity commands while the rover is in the ready state.
- **Subscribed Topics:**
    - None
- **Services:**
    - None

### Teleoperation Node

The `teleoperation_node.cpp` file implements the functionality for teleoperation, allowing the rover to be controlled manually via a joystick.

- **Published Topics:**
    - `cmd_vel` (`geometry_msgs/Twist`): Publishes velocity commands based on joystick input.
- **Subscribed Topics:**
    - `joy` (`sensor_msgs/Joy`): Joystick inputs for controlling the rover.
- **Services:**
    - None

- ** Parameters:**
    - `max_joy_msg_delay`: Maximum delay allowed for joystick messages.
    - `joy_topic`: Name of the topic to subscribe to for joystick inputs.
    - `cmd_vel_topic_name`: Name of the topic to publish velocity commands.
    - `queue_size`: The size of the message queue.
    - `dead_man_button`: The button used as the dead man switch.
    - `x_axis_button`: The button used for the x-axis control.
    - `z_axis_button`: The button used for the z-axis control.
    - `x_multiplier`: Multiplier for the x-axis control.
    - `z_multiplier`: Multiplier for the z-axis control.



### Main Node Files

Each state node has a corresponding `_main.cpp` file that initializes the ROS2 node and starts the node's lifecycle. These files are mostly boilerplate code to set up the node and handle its lifecycle transitions.

For example, the content of `Ctrl_node_main.cpp`, `estop_node_main.cpp`, `path_following_node_main.cpp`, `path_record_node_main.cpp`, `ready_node_main.cpp`, and `teleoperation_node_main.cpp` follow a similar pattern:

```cpp
#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "<node_header_file>.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<<NodeClass>>();
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    return 0;
}
```

Replace `<NodeClass>` with the appropriate class name and `<node_header_file>` with the corresponding header file for each node.

