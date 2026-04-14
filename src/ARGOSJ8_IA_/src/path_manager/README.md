# Introduction

This repository contains a package to manage the paths to perform autonomous navigation using the package [argj801_ctrl_mission](https://github.com/Robotics-Mechatronics-UMA/argj801_ctl_mission). It implements a series of topics and servers to load, save and advertise the paths that the robot would follow in autonoums mode.

### Frames

Following the fixposition frame convention, the node uses WGS84 and ECEF to express the paths.  
### Topics

+ **/ARGJ801/fixed_frame_topic**. Pulish in ECEF frame the path to be attempted by the robot (stored in the robot_path class variable, in WGS84) in ECEF frame.
+ **/ARGJ801/ll_path_topic**. Pulish in WGS84 frame (Latitude, Longitude, Heigth)  the path to be attempted by the robot.

### Services
+ **/ARGJ801/read_path_service**. Service to read a path, stored in /workspace_ws/install/path_manager/share/path_manager/path specifing its filename, in a gpx format.
 
+ **/ARGJ801/read_path_service**. Service to write and store the current loaded path in /workspace_ws/install/path_manager/share/path_manager/path and it gpx format specifing its filename.

+ **/ARGJ801/get_ll_path_service**. Service get the path stored to be atempted by the robot in WGS84 frame (Latitude, Longitude, Heigth).

+ **/ARGJ801/get_fix_frame_path_service**. Service get the path stored to be atempted by the robot in ECEF frame.


### Global path planner
The path manager also implements a global path planner, to calculate safe global path between a start point (typically the robot position) and a goal. In particular, it implements the [DEM_AIA](https://www.sciencedirect.com/science/article/pii/S0952197623001604) path planner to calculae a safe global path using a  Digital Elevation Map of the robot enviroment. Throught the service **/ARGJ801/plan_path**, specifiying an start and a goal in **UTM** (as the DEM is build using this reference system), the service would return (if exists) a path that goes from start to goal. Also, the varible robot_path will store this new path.



