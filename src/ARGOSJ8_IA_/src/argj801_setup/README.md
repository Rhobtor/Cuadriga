

# cuadriga Setup Package

## Overview

El paquete `cuadriga_setup` centraliza la **configuración** y el **arranque** (launch) del
sistema ROS 2 del rover **cuadriga**. Su función es:

- Cargar parámetros comunes desde `config/cuadriga_params.yaml`.
- Lanzar nodos de control, planificación y sensorización bajo el namespace `/cuadriga`.
- Orquestar transiciones de *Lifecycle* (configure/activate) con retardos para evitar
  carreras durante el arranque.

Este paquete no implementa lógica de control; sólo describe cómo se inicia el sistema.

## Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
  - [Launch the System](#launch-the-system)
  - [Nodes](#nodes)
- [Configuration](#configuration)
  - [Parameters](#parameters)
  - [Environment Variables](#environment-variables)
- [Contributing](#contributing)
- [License](#license)
- [Additional Documentation](#additional-documentation)

## Installation

1. Clone the repository:
   ```sh
   git clone git@github.com:Robotics-Mechatronics-UMA/cuadriga_setup.git
   cd cuadriga_setup
   ```

2. Install dependencies:
   ```sh
   rosdep install --from-paths src --ignore-src -r -y
   ```

3. Build the package:
   ```sh
   colcon build
   ```

## Usage

### Launch the System

El launch principal es `launch/cuadriga_launch.py`. Este fichero puede usarse tanto para
robot real como para simulación, habilitando/deshabilitando subconjuntos de nodos.

#### Parameters

- `robot`: Launch robot-specific nodes if `true` (default: `false`).
- `simulator`: Launch simulator-specific nodes if `true` (default: `false`).
- `use_gui`: Launch the GUI node if `true` (default: `false`).

#### Examples

1. **Launching the Simulator without GUI:**

    ```sh
  ros2 launch cuadriga_setup cuadriga_launch.py simulator:=true use_gui:=false
    ```

2. **Launching the Real Robot:**

    ```sh
  ros2 launch cuadriga_setup cuadriga_launch.py robot:=true
    ```

3. **Launching the Simulator with GUI:**

    ```sh
  ros2 launch cuadriga_setup cuadriga_launch.py simulator:=true use_gui:=true
    ```

By using these parameters, you can easily switch between launching the system for the real robot or the simulator, and optionally include the GUI.
### Nodes

The `cuadriga_setup` package launchs the following nodes:

- `ctl_mission_node`
- `controller_node`
- `path_following_node`
- `teleoperation_node`
- `path_record_node`
- `ready_node`
- `estop_node`
- `back_home_node`
- `path_manager_node`
- `security_check_node`
- `mpc_node`
- `cuadriga_ctrl_platform_node`
- `joy_node`
- `android_server_node`
- `laser_segmentation`
- `static_transform_publisher`
- `gui_node`

Each node is configured and launched with specific parameters to perform its designated function in the system.

### Lifecycle / secuencia de arranque

Muchos de los nodos se lanzan como **LifecycleNode**. En `cuadriga_launch.py` se disparan
transiciones `CONFIGURE` y `ACTIVATE` con un retardo (TimerAction). Este comportamiento
es importante si algunos nodos dependen de TF, servicios o tópicos que tardan en aparecer.

### Launch and congif files

For more details on configuration and launch files, please refer to their respective README documents:

- [Configuration Files README](./config/README.md)
- [Launch Files README](./launch/README.md)

> Nota: en este repositorio existe también un GUI nuevo (`cuadriga_gui`). En `cuadriga_launch.py`
> actualmente se lanza el GUI legacy (`GUI_pkg`) cuando `use_gui:=true`.
## Contributing

Contributions are welcome! Please follow the guidelines in [CONTRIBUTING.md](CONTRIBUTING.md) to submit your changes.

## License

This project is licensed under the Apache License, Version 2.0. See the [LICENSE](LICENSE) file for details.

## Additional Documentation

For more detailed information on each package and node, refer to the individual documentation files:

- [ctl_mission](https://github.com/Robotics-Mechatronics-UMA/cuadriga_ctl_mission.git)
- [path_manager](https://github.com/Robotics-Mechatronics-UMA/path_manager.git)
- [security_check](https://github.com/Robotics-Mechatronics-UMA/security_check.git)
- [cuadriga_ctl_platform](https://github.com/Robotics-Mechatronics-UMA/cuadriga_ctl_platform.git)
- [android_ros2_server](https://github.com/Robotics-Mechatronics-UMA/Android_ros2_server_pkg.git)
- [laser_segmentation](git@github.com:Robotics-Mechatronics-UMA/lidar_process_pkgs.git)
- [GUI_pkg](https://github.com/Robotics-Mechatronics-UMA/cuadriga_GUI.git)

