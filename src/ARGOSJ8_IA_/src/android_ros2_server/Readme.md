# TCP Server Lifecycle Node for ROS 2

This package contains a ROS 2 lifecycle node that sets up a TCP server for interacting with a robot's Finite State Machine (FSM) and GPS system. The node listens for client connections from the [Android Emergency Stop App ](https://github.com/Robotics-Mechatronics-UMA/Stop_robot_android_app) and the [Request Robot App ](https://github.com/Robotics-Mechatronics-UMA/Stop_robot_android_app). 

## Features

- ROS 2 Lifecycle Node implementation
- TCP server for client communication
- Interaction with FSM services (`ChangeMode`, `GetMode`)
- GPS coordinate publishing and subscription
- Path planning service integration
- Emergency stop functionality
## Parameters

- `host`: IP address to bind the TCP server (default: `0.0.0.0`).
- `port`: Port number for the TCP server (default: `5001`).
- `emergency_topic`: Topic for emergency calls (default: `/Human/emergency_call`).
- `gps_fix_topic`: Topic for publishing GPS fixes (default: `/Human/gps/fix`).
- `navsatfix_subscription_topic`: Topic to subscribe to robot's GPS data (default: `/fixposition/navsatfix`).
- `change_mode_service`: Service for changing FSM mode (default: `/ARGJ801/change_fsm_mode`).
- `get_mode_service`: Service for getting FSM mode (default: `/ARGJ801/get_fsm_mode`).
- `plan_path_service`: Service for path planning (default: `/ARGJ801/path_planner`).

## Node Description

The `TcpServerLifecycleNode` is a ROS 2 lifecycle node that manages a TCP server. It handles client commands to:

- Check server status (`HEARTBEAT`).
- Issue an emergency stop (`STOP`).
- Retrieve the current FSM state (`GET_STATE`).
- Publish GPS coordinates (`GPS;lat;lon`).
- Request path planning to a specified location (`REQUEST;lat;lon`).

### Published Topics

- `/Human/gps/fix` (`sensor_msgs/NavSatFix`): Publishes GPS coordinates received from clients.
- `/Human/emergency_call` (`std_msgs/Bool`): Publishes an emergency call signal.

### Subscribed Topics

- `/fixposition/navsatfix` (`sensor_msgs/NavSatFix`): Subscribes to the robot's GPS data.

### ROS 2 Services Used

- `/ARGJ801/change_fsm_mode` (`ChangeMode`): Changes the FSM mode of the robot.
- `/ARGJ801/get_fsm_mode` (`GetMode`): Retrieves the current FSM mode of the robot.
- `/ARGJ801/path_planner` (`AssistEmergency`): Requests path planning to a specified GPS location.

-   ROS TF Tree:

        
    ```mermaid
    graph TD;
    ECEF-->FP_POI-->FP_VRTK-->FP_CAM
    FP_POI-->FP_IMUH
    ECEF-->FP_ENU
    ECEF-->FP_ENU0
    ```




