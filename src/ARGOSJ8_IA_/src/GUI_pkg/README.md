
# GUI Node

The `GUI Node` is responsible for providing a graphical user interface for controlling and monitoring the ARGJ801 rover. This node integrates various ROS functionalities with a user-friendly interface built using PySide6.

## Topics

### Published Topics
- **`local_path` (`nav_msgs/msg/Path`)**: Publishes the local path for navigation.
- **`goal` (`geometry_msgs/msg/Point`)**: Publishes the goal position for path planning.

### Subscribed Topics
- **`local_path` (`nav_msgs/msg/Path`)**: Subscribes to the local path for navigation.
- **`pose` (`geometry_msgs/msg/PoseStamped`)**: Subscribes to the current pose of the rover.
- **`fixposition_status` (`std_msgs/msg/String`)**: Subscribes to the localization status.
- **`emergency_call` (`std_msgs/msg/Bool`)**: Subscribes to the emergency call status.
- **`joy` (`sensor_msgs/msg/Joy`)**: Subscribes to joystick inputs.

## Services

### Provided Services
- **`toggle_security_srv` (`std_srvs/srv/SetBool`)**: Toggles the security check on and off.
- **`send_FSM_change_request` (`argj801_ctl_mission_interfaces/srv/ChangeMode`)**: Sends a request to change the FSM mode.
- **`plan_path_srv` (`argj801_ctl_mission_interfaces/srv/PlanPath`)**: Plans a path from the current position to a goal position.
- **`read_path_from_file` (`argj801_ctl_mission_interfaces/srv/ReadPath`)**: Reads a path from a file.
- **`write_path_to_file` (`argj801_ctl_mission_interfaces/srv/WritePath`)**: Writes the current path to a file.
- **`config_pure_pursuit_srv` (`argj801_ctl_mission_interfaces/srv/ConfigPurePursuitCtrl`)**: Configures the pure pursuit controller.
- **`config_dynamic_pure_srv` (`argj801_ctl_mission_interfaces/srv/ConfigDynamicPureCtrl`)**: Configures the dynamic pure pursuit controller.
- **`config_dynamic_la_pure_srv` (`argj801_ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl`)**: Configures the dynamic look-ahead pure pursuit controller.
- **`config_regulated_pure_srv` (`argj801_ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl`)**: Configures the regulated pure pursuit controller.

## Parameters

- **`robot_ip`**: The IP address of the robot for connectivity checks.
- **`map_gui_scale`**: Scale of the GUI map.
- **`path_width`**: Width of the path to be drawn on the map.
- **`UTM_x_up_north`**: UTM coordinate x for the upper north of the map.
- **`UTM_y_up_north`**: UTM coordinate y for the upper north of the map.
- **`m_to_pix_x`**: Conversion factor from meters to pixels in x direction.
- **`m_to_pix_y`**: Conversion factor from meters to pixels in y direction.

## Example Usage

To launch the `GUI Node`, use the following command:
```bash
ros2 launch argj801_ctl_mission gui_node_launch.py
```

## Common Functions

- **mousePressEvent()**: Handles mouse press events for drawing paths and selecting points of interest.
- **mouseMoveEvent()**: Handles mouse move events for drawing paths and updating the zoom rectangle.
- **mouseReleaseEvent()**: Handles mouse release events to finalize drawing paths and the zoom rectangle.
- **timer_fast()**: Fast timer function to update various GUI elements frequently.
- **timer_fsm()**: Timer function to update the FSM dropdown with possible transitions.
- **toggle_security_check()**: Toggles the security check on and off.
- **update_connectivity_label()**: Updates the connectivity status label based on the connectivity level.
- **create_colored_icon()**: Creates a colored icon for status indicators.
- **refresh_img()**: Refreshes the image displayed in the GUI with the latest path and position data.
- **send_transition()**: Sends a transition request to change the FSM mode.
- **send_drawn_path_cb()**: Sends the drawn path for execution.
- **path_file_read()**: Reads a path from a file.
- **path_file_write()**: Writes the current path to a file.
- **plan_path_cb()**: Plans a path from the current position to a goal position.
- **config_pure_pursuit_cb()**: Configures the pure pursuit controller.
- **config_dynamic_pp_cb()**: Configures the dynamic pure pursuit controller.
- **config_dynamic_la_pp_cb()**: Configures the dynamic look-ahead pure pursuit controller.
- **config_regulated_cb()**: Configures the regulated pure pursuit controller.

## Main Node File

The main entry point for the `GUI Node` is the `main` function, which initializes the application and starts the Qt event loop.

```python
def main(args=None):
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget.show()
    sys.exit(app.exec())
```

