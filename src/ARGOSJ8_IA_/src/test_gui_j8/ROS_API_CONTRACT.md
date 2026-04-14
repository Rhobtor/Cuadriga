# ROS API contract — `j8_gui` (reemplazo de `GUI_pkg`)

Este documento resume los **topics**, **servicios** y **parámetros** que el GUI legacy (`GUI_pkg`) usa para comunicarse con el stack del J8.

Fuente legacy (en este repo):
- `src/GUI_pkg/GUI_pkg/ros_classes.py`

> Objetivo: que `src/test_gui_j8/src/j8_gui` pueda sustituir a `GUI_pkg` sin cambiar el resto del sistema.

---

## Topics (suscripciones) — legacy `GUI_pkg`

Definidos en `ROSclass_topics.__init__`.

| Tipo | Nombre (default) | Msg | QoS | Parámetro |
|---|---|---|---|---|
| Sub | `/fixposition/navsatfix` | `sensor_msgs/msg/NavSatFix` | `qos_profile_sensor_data` | `filtered_gps_topic_name` |
| Sub | `/ARGJ801/base/gps/fix` | `sensor_msgs/msg/NavSatFix` | `qos_profile_sensor_data` | `base_gps_topic_name` |
| Sub | `/ARGJ801/rover/gps/fix` | `sensor_msgs/msg/NavSatFix` | `qos_profile_sensor_data` | `rover_gps_topic_name` |
| Sub | `/Human/gps/fix` | `sensor_msgs/msg/NavSatFix` | `qos_profile_sensor_data` | `human_gps_topic_name` |
| Sub | `/imu_head/data` | `sensor_msgs/msg/Imu` | `qos_profile_sensor_data` | `imu_head_topic_name` |
| Sub | `/ARGJ801/look_ahead_point` | `std_msgs/msg/Int32` | `qos_profile_sensor_data` | `look_ahead_topic_name` |
| Sub | `/ARGJ801/local_trajectory_ll` | `nav_msgs/msg/Path` | `qos_profile_sensor_data` | `local_path_topic_name` |
| Sub | `/fixposition/odometry_enu` | `nav_msgs/msg/Odometry` | `qos_profile_sensor_data` | `odometry_topic_name` |
| Sub | `/Human/emergency_call` | `std_msgs/msg/Bool` | `qos_profile_sensor_data` | `emergency_call_topic_name` *(ojo: legacy lo hardcodea)* |
| Sub | `/ARGJ801/possible_transitions` | `std_msgs/msg/Int32MultiArray` | `qos_profile_sensor_data` | `fsm.get_possible_transition_topic_name` |
| Sub | `/ARGJ801/fsm_mode` | `std_msgs/msg/Int32` | `qos_profile_sensor_data` | `fsm.get_fsm_topic_name` |
| Sub | `/fixposition/localization_status` | `fixposition_driver_ros2/msg/VRTK` | `qos_profile_sensor_data` | *(hardcoded)* |

### Topic extra (GUI legacy, pestaña Follow)

En `GUI_pkg/mainwindow.py`:

| Tipo | Nombre | Msg |
|---|---|---|
| Sub | `/follow_zed/image_for_gui` | `sensor_msgs/msg/Image` |
| Sub (fallback) | `/follow_zed/image_for_gui/compressed` | `sensor_msgs/msg/CompressedImage` |

---

## Servicios (clientes) — legacy `GUI_pkg`

Definidos en `ROSclass_srv.__init__`.

Todos los servicios se forman como: `/<namespace>/<name>` con `namespace='ARGJ801'`.

| Tipo | Servicio (default final) | Srv |
|---|---|---|
| Client | `/ARGJ801/get_fsm_mode` | `lifecycle_msgs/srv/GetState` |
| Client | `/ARGJ801/change_fsm` | `ctl_mission_interfaces/srv/ChangeMode` |
| Client | `/ARGJ801/get_possible_transitions` | `ctl_mission_interfaces/srv/GetPossibleTransitions` |
| Client | `/ARGJ801/config_controller` | `ctl_mission_interfaces/srv/ConfigSimpleCtl` |
| Client | `/ARGJ801/change_controller_type` | `ctl_mission_interfaces/srv/ChangeController` |
| Client | `/ARGJ801/config_pure_pursuit` | `ctl_mission_interfaces/srv/ConfigPurePursuitCtrl` |
| Client | `/ARGJ801/config_stanley` | `ctl_mission_interfaces/srv/ConfigStanleyCtrl` |
| Client | `/ARGJ801/config_dynamic_pure` | `ctl_mission_interfaces/srv/ConfigDynamicPureCtrl` |
| Client | `/ARGJ801/config_dynamic_la_pure` | `ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl` |
| Client | `/ARGJ801/config_regulated_pure` | `ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl` |
| Client | `/ARGJ801/path_planner` | `path_manager_interfaces/srv/PlanPath` |
| Client | `/ARGJ801/receive_ll_path` | `path_manager_interfaces/srv/RobotPath` |
| Client | `/ARGJ801/get_robot_Path` | `path_manager_interfaces/srv/ReturnRobotPath` |
| Client | `/ARGJ801/write_path_file` | `path_manager_interfaces/srv/WritePathToFile` |
| Client | `/ARGJ801/read_path_file` | `path_manager_interfaces/srv/ReadPathFromFile` |
| Client | `/ARGJ801/enable_security_check` | `std_srvs/srv/SetBool` |
| Client | `/ARGJ801/get_security_check` | `security_check_interfaces/srv/GetSecurityCheckStatus` |

---

## Parámetros que conviene conservar en `j8_gui`

### Topics (`ROSclass_topics`)
- `filtered_gps_topic_name`
- `base_gps_topic_name`
- `rover_gps_topic_name`
- `imu_head_topic_name`
- `look_ahead_topic_name`
- `local_path_topic_name`
- `odometry_topic_name`
- `human_gps_topic_name`
- `emergency_call_topic_name`
- `fsm.get_possible_transition_topic_name`
- `fsm.get_fsm_topic_name`
- `local_fixed_frame`
- `robot_frame`

### Services (`ROSclass_srv`)
- `fsm.change_fsm_mode_srv_name`
- `fsm.get_fsm_srv_name`
- `fsm.get_possible_transition_srv_name`
- `config_controller_srv_name`
- `read_path_service`
- `return_path_service`
- `write_path_service`
- `enable_security_check_srv_name`
- `get_security_check_srv_name`
- `change_controller_srv_name`
- `config_pure_pursuit_srv_name`
- `config_stanley_srv_name`
- `config_dynamic_pure_srv_name`
- `config_dynamic_la_pure_srv_name`
- `config_regulated_pure_srv_name`
- `path_planner_srv_name`
- `receive_ll_path_srv_name`

---

## Estado actual en `test_gui_j8`

- El prototipo `j8_gui` ya usa el topic de imagen FollowZED por defecto: `/follow_zed/image_for_gui`.
- Se han alineado los defaults de telemetría en `j8_gui/app/state_widget.py` con los topics de `GUI_pkg`.
- `j8_gui/app/main_window.py` incluye una capa de compatibilidad para llamar servicios (`ChangeMode`, `RobotPath`) además de publicar los topics de prueba `/gui/*`.

> Si vas a eliminar los topics de prueba `/gui/*`, el siguiente paso es pasar la UI para que **siempre** llame al servicio del FSM y al servicio de envío de path.
