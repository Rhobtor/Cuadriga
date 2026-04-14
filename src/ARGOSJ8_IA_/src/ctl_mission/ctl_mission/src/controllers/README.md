# Controllers

This directory contains the implementation of various controllers used in the `Ctrl_node.cpp` for the ARGJ801 rover. Each controller is responsible for a specific control algorithm to manage the rover's path following and other behaviors.

## Introduction

All controllers share some common functions and structures:
- **config()**: Configures the controller parameters.
- **calc_actions()**: Calculates the control actions based on the given path and other inputs.
- **Output Structure**: Each controller has an output structure to store the results of the control calculations, such as linear speed, angular speed, and goal index.

## Controllers

1. [Pure Pursuit Controller](#pure-pursuit-controller)
2. [Regulated Pure Pursuit Controller](#regulated-pure-pursuit-controller)
3. [Dynamic Pure Pursuit Controller](#dynamic-pure-pursuit-controller)
4. [Dynamic Look-Ahead Pure Pursuit Controller](#dynamic-look-ahead-pure-pursuit-controller)

### Pure Pursuit Controller

The `pure_pursuit_controller.cpp` file implements the pure pursuit control algorithm, which calculates the linear and angular velocity commands to follow a given path.

- **Parameters:**
  - `v_forward`: Forward velocity.
  - `l_ahead`: Look-ahead distance.

- **Functions:**
  - `calc_pure_pursuit_actions()`: Calculates the control actions based on the path and distance to the last object.
  - `pure_pursuit_output`: Structure to store the output of the pure pursuit controller, including linear speed (`linear_sp`), angular speed (`angular_sp`), and the goal index (`goal_idx`).

#### Example Usage in Ctrl_node:

```cpp
if (current_controler == "pure_pursuit"){
    pure_pursuit_controller.calc_pure_pursuit_actions(path_transformed, dist_last_obj);
    cmd_msg.linear.x = pure_pursuit_controller.pure_pursuit_output.linear_sp;
    cmd_msg.angular.z = -pure_pursuit_controller.pure_pursuit_output.angular_sp;
    std_msgs::msg::Int32 index_msg;
    index_msg.data = pure_pursuit_controller.pure_pursuit_output.goal_idx;
    pub_look_ahead->publish(index_msg);
    std_msgs::msg::Float32 distance_msg;
    distance_msg.data = pure_pursuit_controller.pure_pursuit_output.cte;
    pub_look_ahead_distance->publish(distance_msg);
}
```

### Regulated Pure Pursuit Controller

The `regulated_pure_pursuit_controller.cpp` file implements a regulated pure pursuit control algorithm, which regulates the velocity based on the curvature of the path.

- **Parameters:**
  - `l_ahead`: Look-ahead distance.
  - `v_forward`: Forward velocity.
  - `r_min`: Minimum turning radius.

- **Functions:**
  - `calc_regulated_pure_pursuit_actions()`: Calculates the control actions based on the path and distance to the last object.
  - `regulated_pure_pursuit_output`: Structure to store the output, including linear speed (`linear_sp`), angular speed (`angular_sp`), and the cross-track error (`cte`).

#### Example Usage in Ctrl_node:
```cpp
if (current_controler == "regulated_pure_pursuit"){
    regulated_pure_pursuit_controller.calc_regulated_pure_pursuit_actions(path_transformed, dist_last_obj);
    cmd_msg.linear.x = regulated_pure_pursuit_controller.regulated_pure_pursuit_output.linear_sp;
    cmd_msg.angular.z = -regulated_pure_pursuit_controller.regulated_pure_pursuit_output.angular_sp;
    std_msgs::msg::Float32 distance_msg;
    distance_msg.data = regulated_pure_pursuit_controller.regulated_pure_pursuit_output.cte;
    pub_look_ahead_distance->publish(distance_msg);
}
```

### Dynamic Pure Pursuit Controller

The `dynamic_pp_controller.cpp` file implements a dynamic pure pursuit control algorithm, which adjusts the look-ahead distance dynamically based on the speed.

- **Parameters:**
  - `look_ahead_distance`: Initial look-ahead distance.
  - `max_speed`: Maximum speed.
  - `max_ang_acc`: Maximum angular acceleration.
  - `max_ang_dec`: Maximum angular deceleration.
  - `max_lin_acc`: Maximum linear acceleration.
  - `max_lin_dec`: Maximum linear deceleration.

- **Functions:**
  - `calc_dynamic_pp_actions()`: Calculates the control actions dynamically based on the path.
  - `dynamic_pp_output`: Structure to store the output, including linear speed (`linear_sp`), angular speed (`angular_sp`), and the goal index (`goal_idx`).

#### Example Usage in Ctrl_node:
```cpp
if (current_controler == "dynamic_pure_pursuit"){
    dynamic_pp_controller.calc_dynamic_pp_actions(path_transformed);
    cmd_msg.linear.x = dynamic_pp_controller.dynamic_pp_output.linear_sp;
    cmd_msg.angular.z = dynamic_pp_controller.dynamic_pp_output.angular_sp;
    std_msgs::msg::Int32 index_msg;
    index_msg.data = dynamic_pp_controller.dynamic_pp_output.goal_idx;
    pub_look_ahead->publish(index_msg);
}
```

### Dynamic Look-Ahead Pure Pursuit Controller

The `dynamic_la_pp_controller.cpp` file implements a dynamic look-ahead pure pursuit control algorithm, which adjusts both the look-ahead distance and velocity dynamically.

- **Parameters:**
  - `look_ahead_v_gain`: Look-ahead velocity gain.
  - `max_speed`: Maximum speed.
  - `max_ang_acc`: Maximum angular acceleration.
  - `max_ang_dec`: Maximum angular deceleration.
  - `max_lin_acc`: Maximum linear acceleration.
  - `max_lin_dec`: Maximum linear deceleration.
  - `min_speed`: Minimum speed.

- **Functions:**
  - `calc_dynamic_lapp_actions()`: Calculates the control actions dynamically based on the path, distance to the last object, and current velocities.
  - `dynamic_lapp_output`: Structure to store the output, including linear speed (`linear_sp`), angular speed (`angular_sp`), cross-track error (`cte`), goal index (`goal_idx`), and look-ahead pose (`look_ahead_pose`).

#### Example Usage in Ctrl_node:

```cpp
if (current_controler == "dynamic_la_pure_pursuit"){
    dynamic_la_pp_controller.calc_dynamic_lapp_actions(path_transformed, dist_last_obj, v_x_odom, w_z_odom);
    cmd_msg.linear.x = dynamic_la_pp_controller.dynamic_lapp_output.linear_sp;
    cmd_msg.angular.z = dynamic_la_pp_controller.dynamic_lapp_output.angular_sp;
    std_msgs::msg::Int32 index_msg;
    index_msg.data = dynamic_la_pp_controller.dynamic_lapp_output.goal_idx;
    pub_look_ahead->publish(index_msg);
    std_msgs::msg::Float32 distance_msg;
    distance_msg.data = dynamic_la_pp_controller.dynamic_lapp_output.cte;
    pub_look_ahead_distance->publish(distance_msg);
    pub_la_pose->publish(dynamic_la_pp_controller.dynamic_lapp_output.look_ahead_pose);
}
```