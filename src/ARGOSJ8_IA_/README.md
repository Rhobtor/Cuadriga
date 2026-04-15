# Robotics Mechatronics Project

## Overview
This project is designed for advanced robotic control and simulation using ROS2. It integrates multiple packages to manage robot simulations, sensor data processing, and real-time control. The packages are added as [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

## Features
- **Simulation Models**: Gazebo simulation models for various robot configurations.
- **Sensor Integration**: Drivers for different sensors including Fixposition and Ublox.
- **Control Algorithms**: Advanced control algorithms for path management and mission control.
- **User Interface**: GUI for easy interaction and control of the robots.
- **Security Checks**: Modules to ensure safe and secure operation.

## Repository Structure
```
root
├── src/
│   ├── android_ros2_server
│   ├── argo_drivers_pkgs/
│   │   ├── Fixposition_driver_pkg
│   │   ├── argj801_sensors
│   │   ├── ntrip_client
│   │   └── ublox
│   ├── argj801_ctl_platform
│   ├── argj801_setup
│   ├── ctl_mission
│   ├── gazebo_sim_pkgs/
│   │   ├── fix_position_pkg
│   │   └── j8_xacro_model
│   ├── GUI_pkg
│   ├── j8_joystick
│   ├── lidar_process_pkgs
│   ├── path_manager
│   └── security_check
├── .gitmodules
└── README.md
```

## Installation

### Prerequisites
- **ROS2**: Make sure ROS2 is installed on your system. Follow the [ROS2 installation guide](https://docs.ros.org/en/foxy/Installation.html).
- **Dependencies**: Install additional dependencies required for the packages.

### Building the Workspace
```sh
# Clone the repository with submodules
git clone --recurse-submodules git@github.com:Robotics-Mechatronics-UMA/ARGOJ8_ws.git
cd ARGO_ws/

# Install dependencies
./install.sh

# Build the workspace
colcon build --symlink-install
```

## Usage

Launch and configuraiton to run all the nodes can be found in a [dedicated package](https://github.com/Robotics-Mechatronics-UMA/argj801_setup).

### Quick start (local)

Source the workspace:

```sh
source install/setup.bash
```

#### Bringup without control center / without LCM

This is the most robust way to start the stack when you don't have the external control center sending LCM heartbeats.

```sh
ros2 launch argj801_setup J8_launch.py \
  robot:=true \
  platform_mode:=2 \
  sensors_source:=lcm_sensors \
  enable_mpc:=false
```

Notes:

- `platform_mode:=2` forces `argj801_ctrl_platform_node` to run in Gazebo mode (no LCM). Values: `0` Arduino, `1` LCM, `2` Gazebo.
- `sensors_source` is a required CLI argument for `ARGJ801_sensors_node`. Valid values are `lcm_sensors` and `camera`.
- `enable_mpc:=true` requires Python dependency `cvxpy`.

#### Bringup with control center (LCM)

If you want the rover to accept commands from the control center via LCM multicast:

```sh
ros2 launch argj801_setup J8_launch.py robot:=true platform_mode:=1
```

On the rover host, you must have a multicast route for `224.0.0.0/4` on the NIC connected to the rover network:

```sh
sudo ./scripts/ensure_multicast_route.sh <iface>
```

Verify that packets arrive:

```sh
sudo tcpdump -ni <iface> 'udp and (port 5004 or port 5006)'
```

If `tcpdump` shows **0 packets**, the issue is usually upstream (no sender, wrong network path, IGMP snooping/switch rules).

### Docker

The workspace `dockerfile` already installs dependencies and runs `colcon build` for everything under `src/`, so the integrated `cuadriga_actuator` package is included in the normal image build.

Build the image from the workspace root:

```sh
docker build -t cuadriga-j8 -f dockerfile .
```

Run a local integration test without hardware:

```sh
docker run --rm -it \
  --net=host \
  --ipc=host \
  -e DISPLAY=${DISPLAY} \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  cuadriga-j8 \
  ros2 launch argj801_setup J8_launch.py local_test:=true use_gui:=false
```

Run on the real robot with Cuadriga serial access:

```sh
docker run --rm -it \
  --net=host \
  --ipc=host \
  --device=/dev/ttyUSB0:/dev/ttyUSB0 \
  -e DISPLAY=${DISPLAY} \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  cuadriga-j8 \
  ros2 launch argj801_setup J8_launch.py robot:=true use_cuadriga_backend:=true
```

If you want to pass a host-side DDS XML through `CYCLONEDDS_URI`, you must mount that path into the container. Otherwise the entrypoint will ignore it and fall back to the internal generated config:

```sh
docker run --rm -it \
  --net=host \
  --ipc=host \
  --device=/dev/ttyUSB0:/dev/ttyUSB0 \
  -v /home/cuadriga/.dds:/home/cuadriga/.dds:ro \
  -e CYCLONEDDS_URI=file:///home/cuadriga/.dds/cyclone_dds.xml \
  cuadriga-j8 \
  ros2 launch argj801_setup J8_launch.py robot:=true use_cuadriga_backend:=true
```

If the ZeroTier interface name is different on your host, override it with `ZT_INTERFACE=<iface>`. If the interface comes up late, increase `ZT_WAIT_SECONDS`. You can also bypass auto-detection entirely with `ZT_IP=<ipv4>`.

If your Fixposition endpoint is not the default one from `J8_params.yaml`, override it explicitly:

```sh
docker run --rm -it \
  --net=host \
  --ipc=host \
  --device=/dev/ttyUSB0:/dev/ttyUSB0 \
  -e DISPLAY=${DISPLAY} \
  -e XAUTHORITY=${XAUTHORITY} \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v ${XAUTHORITY}:${XAUTHORITY}:ro \
  cuadriga-j8 \
  ros2 launch argj801_setup J8_launch.py \
    robot:=true \
    use_cuadriga_backend:=true \
    fixposition_ip:=192.168.2.113 \
    fixposition_port:=21000
```

For remote X11 forwarding sessions (for example `DISPLAY=localhost:10.0`), mounting `XAUTHORITY` is required for RViz/GUI applications to open from inside Docker.

You can also use the helper script:

```sh
./run_robot_docker.sh
```

To start RViz from the same image once the stack is running:

```sh
docker run --rm -it \
  --net=host \
  --ipc=host \
  -e DISPLAY=${DISPLAY} \
  -e XAUTHORITY=${XAUTHORITY} \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v ${XAUTHORITY}:${XAUTHORITY}:ro \
  cuadriga-j8 \
  rviz2 -d /ros2_ws/src/argo_drivers_pkgs/Fixposition_driver_pkg/fixposition_driver-main/fixposition_driver_ros2/rviz/fixposition_driver_ros2.rviz
```

If the host cannot reach the Fixposition IP on the robot LAN, Docker will not be able to reach it either even with `--net=host`.

The integrated actuator always exposes direct velocity commands on `/Cuadriga/direct_cmd_vel`:

```sh
ros2 topic pub /Cuadriga/direct_cmd_vel geometry_msgs/msg/Twist \
  '{linear: {x: 0.4}, angular: {z: 0.0}}'
```


## Contributing
We welcome contributions! Please follow these guidelines:
- **Issues**: Use the GitHub issue tracker to report bugs or request features.
- **Pull Requests****: Submit pull requests for any improvements or bug fixes.
- **Coding Standards**: Follow the coding standards outlined in the repository.

## License
This project is licensed under the [MIT License](LICENSE).

## Acknowledgments
We would like to thank the contributors and the open-source community for their support and resources.

---

### Package Details
- **[android_ros2_server](https://github.com/Robotics-Mechatronics-UMA/Android_ros2_server_pkg.git)**: Provides an interface for ROS2 communication with Android devices.
- **argo_drivers_pkgs**: Contains drivers for various sensors including Fixposition and Ublox.
  - **[Fixposition_driver_pkg](https://github.com/Robotics-Mechatronics-UMA/Fixposition_driver_pkg.git)**: Driver package for Fixposition sensor integration.
  - **[argj801_sensors](https://github.com/Robotics-Mechatronics-UMA/argj801_sensors.git)**: Drivers for ArgJ801 robot sensors.
  - **[ntrip_client](https://github.com/Robotics-Mechatronics-UMA/ntrip_client.git)**: NTRIP client package for RTCM data streaming.
  - **[ublox](https://github.com/Robotics-Mechatronics-UMA/ublox.git)**: Driver package for Ublox GPS modules.
- **[argj801_ctl_platform](https://github.com/Robotics-Mechatronics-UMA/argj801_ctl_platform.git)**: Platform control algorithms for the ArgJ801 robot.
- **[argj801_setup](https://github.com/Robotics-Mechatronics-UMA/argj801_setup.git)**: Setup configurations for the ArgJ801 robot.
- **[ctl_mission](https://github.com/Robotics-Mechatronics-UMA/argj801_ctl_mission.git)**: Mission control algorithms for autonomous operations.
- **gazebo_sim_pkgs**: Gazebo simulation models and related packages.
  - **[fix_position_pkg](https://github.com/Robotics-Mechatronics-UMA/fix_position_pkg.git)**: Gazebo plugin for fixing positions in simulation.
  - **[j8_xacro_model](https://github.com/Robotics-Mechatronics-UMA/j8_xacro_model.git)**: Xacro models for J8 robot configurations in Gazebo.
- **[GUI_pkg](https://github.com/Robotics-Mechatronics-UMA/J8_GUI.git)**: Graphical User Interface for controlling and monitoring robots.
- **[j8_joystick](https://github.com/Robotics-Mechatronics-UMA/joystick_j8.git)**: Joystick driver for manual robot control.
- **[lidar_process_pkgs](https://github.com/Robotics-Mechatronics-UMA/lidar_process_pkgs.git)**: Packages for processing Lidar data.
- **[path_manager](https://github.com/Robotics-Mechatronics-UMA/path_manager.git)**: Path management algorithms for navigation.
- **[security_check](https://github.com/Robotics-Mechatronics-UMA/security_check.git)**: Modules to ensure safe and secure operation of robots.

## Diagram

A simplified functional diagram of the relations between all the nodes in the workspace is presented in the next 
<img src="imgs/nodes_diagram.png" alt="Description of image" width="1000"/>

# REMOTE CONECTION TO THE CONTROL CENTER

To communicate with the contol center or other robots, the Husarnet VPN is used
## Creating a network.  
  
First register and create a network in Husrnet, or use an existing one:  

## Installing Husarnet VPN client
Execute this command on each physical device you need to connect.

```
curl -s https://install.husarnet.com/install.sh | sudo bash
```

### Creating a Husarnet group
Add your physical devices to the same Husarnet group, by executing following commands on each of them:

```
husarnet join <your-join-code> <device-hostname>

```

![join vpn](https://husarnet.com/assets/images/docs-joincode-1eba003ca62a1732ba30b4642bc5fe47.png)

### Configuration of the DDS:  
  
Husarnet-DDS is a utility that automates the generation of DDS XML configuration files for Husarnet.

First, install Husarnet-DDS on each device in your Husarnet group (eg. on your robot and your laptop) from GitHub releases (available for Linux, MacOS, and Windows), eg. on Ubuntu:

```
RELEASE="v1.3.5"
ARCH="amd64"

sudo curl -L https://github.com/husarnet/husarnet-dds/releases/download/$RELEASE/husarnet-dds-linux-$ARCH -o /usr/local/bin/husarnet-dds
sudo chmod +x /usr/local/bin/husarnet-dds
```

Next, choose the DDS implementation that you want to use with your ROS 2 application. Husarnet-DDS supports both Fast DDS and Cyclone DDS.

Set up the required environment variables based on your chosen DDS implementation on each device in your ROS 2 network (eg. on your robot and your laptop), or add it to the .bashrc.

```
export RMW_IMPLEMENTATION=rmw_fastrtps_cpp
export FASTRTPS_DEFAULT_PROFILES_FILE=/var/tmp/husarnet-fastdds-simple.xml
husarnet-dds singleshot
```
Check that the remote center and the robot are online in the specific network in the [Husarnet Dashboard](https://app.husarnet.com/):  
  
  
<img src="imgs/VPN_status.png" alt="Description of image" width="500
"/>

