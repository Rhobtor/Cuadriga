
# ARGJ801 ROS2 Package Launch Files

This document provides detailed information about the launch files used in the ARGJ801 ROS2 package. Below are explanations of the launch files for both the simulator and the real robot, including which nodes are launched and how the lifecycle transitions are handled.

## Configuration Files

- [ctrl_mission.yaml](../config/J8_config.yaml): Configuration for both the simulator and the real robot.

## Launch Files

The package includes a unified launch file for both the simulator and the real robot. This launch file initializes the nodes and loads the configuration parameters based on the launch arguments provided.

### Unified Launch File

The unified launch file [J8_launch.py](./J8_launch.py) is used to start the nodes for either the simulator or the real robot environment based on the specified launch parameters.

- **File:** `launch/J8_launch.py`

#### Nodes Launched

- `controlmissionNode`: Initializes the FSM control mission node.
- `controllerNode`: Initializes the path tracking controller node.
- `pathfollowingNode`: Initializes the path following node.
- `teleoperationNode`: Initializes the teleoperation node.
- `pathRecordNode`: Initializes the path record node.
- `readyNode`: Initializes the ready node.
- `estopNode`: Initializes the estop node.
- `backhomeNode`: Initializes the back home node.
- `pathManagerNode`: Initializes the path manager node.
- `securityCheckNode`: Initializes the security check node.
- `MPCPlannerNode`: Initializes the MPC planner node.
- `ctrlPlataformNode`: Initializes the control platform node.
- `joystickNode`: Initializes the joystick node.
- `android_server_node`: Initializes the Android server node.
- `laser_segmentation_node`: Initializes the laser segmentation node.
- `fixpositionDriverNode`: Initializes the Fixposition driver node.
- `argj801_sensors`: Initializes the sensors node.
- `tf_node_velodyne`: Publishes static transform for the Velodyne LIDAR.
- `tf_node_sick`: Publishes static transform for the Sick LIDAR.
- `gui_node`: Initializes the GUI node.

#### Lifecycle Transitions

The launch file automatically handles lifecycle transitions to configure and activate the nodes. The nodes are configured and activated with a delay between each transition to ensure proper startup.

#### Launch Parameters

- `robot` (default: `false`): Set to `true` to launch the real robot nodes.
- `simulator` (default: `false`): Set to `true` to launch the simulator nodes.
- `use_gui` (default: `false`): Set to `true` to launch the GUI node.

#### Example Usage

To launch the simulator configuration without the GUI, use the following command:

```bash
ros2 launch argj801_setup J8_launch.py simulator:=true use_gui:=false
```

To launch the real robot configuration, use the following command:

```bash
ros2 launch argj801_setup J8_launch.py robot:=true
```

