# Introduction

Package of mission control for rover J8. The mission mode is controlled by a Finite State Machine.

## Finite State Machine

A Finite state machine (implemented in the file `J8_FSM.cpp`) manage the [states](</argj801_ctl_mission/src/states>)  transitions between different states. These states implement the different functionalities of the robot. In particular:

![image_FSM](./images/FSM.png)

### States

1. **Ready (State 0)**. On start-up, the robot transitions to the ready state. In this state, the robot is not able to move, and only waits for a transition to a state where another functionality is implemented.
2. **Teleoperation (State 1)**. In this state, the robot takes velocity commands from the joystick.
3. **Pathfollowing (State 2)**. In this state, the robot performs a path tracking algorithm using one of the [controllers](</argj801_ctl_mission/src/controllers>) included in the package.
4. **BackHome (State 3)**. The robot asks the Control center for a path to go back to the GPS coordinates defined as home.
5. **E-stop (State 4)**. If the robot catches an error, it automatically transitions to this state. It is only possible to transition back to the Ready state once the error is solved.
 
The states are implementad as one (or a set) of [Lifecycle Nodes](https://design.ros2.org/articles/node_lifecycle). In this way, the control mission node is charge, when a valid transition for the FSM is called, to deactive the nodes corresponding to the current state and activate the nodes of the asked stated.

### Path Following Algorithm

![image_FSM](./images/j8_diagram.png)

The robot follows the path specified in the topic `*/ARGJ801/waypoints*` in order, using the goal GPS position, the robot GPS position, and the global orientation. If the distance to the current goal is less than the one specified in the `*distance_get_goal*`, the current goal would be replaced by the next in the list. Furthermore, to be able to follow complex trajectories, the linear speeds depend on the linear speed as:


