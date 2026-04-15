"""ROS API contract for cuadriga_gui.

This module centralizes topic/service names and parameter defaults so `cuadriga_gui`
can be a drop-in replacement for the legacy `GUI_pkg`.

Source of truth (legacy): `src/GUI_pkg/GUI_pkg/ros_classes.py`

Design goals:
- Keep the same default names and parameter keys as `GUI_pkg`.
- Allow overriding via ROS parameters.
- Keep namespace handling consistent (default: 'cuadriga').

"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class cuadrigaGuiParams:
    """Parameter keys used by the legacy GUI."""

    # namespace
    namespace: str = "cuadriga"

    # topic params (same keys as GUI_pkg)
    filtered_gps_topic_name: str = "filtered_gps_topic_name"
    base_gps_topic_name: str = "base_gps_topic_name"
    rover_gps_topic_name: str = "rover_gps_topic_name"
    imu_head_topic_name: str = "imu_head_topic_name"
    look_ahead_topic_name: str = "look_ahead_topic_name"
    local_path_topic_name: str = "local_path_topic_name"
    odometry_topic_name: str = "odometry_topic_name"
    human_gps_topic_name: str = "human_gps_topic_name"
    emergency_call_topic_name: str = "emergency_call_topic_name"
    fsm_get_possible_transition_topic_name: str = "fsm.get_possible_transition_topic_name"
    fsm_get_fsm_topic_name: str = "fsm.get_fsm_topic_name"
    local_fixed_frame: str = "local_fixed_frame"
    robot_frame: str = "robot_frame"

    # service params (same keys as GUI_pkg)
    fsm_change_fsm_mode_srv_name: str = "fsm.change_fsm_mode_srv_name"
    fsm_get_fsm_srv_name: str = "fsm.get_fsm_srv_name"
    fsm_get_possible_transition_srv_name: str = "fsm.get_possible_transition_srv_name"
    config_controller_srv_name: str = "config_controller_srv_name"
    read_path_service: str = "read_path_service"
    return_path_service: str = "return_path_service"
    write_path_service: str = "write_path_service"
    enable_security_check_srv_name: str = "enable_security_check_srv_name"
    get_security_check_srv_name: str = "get_security_check_srv_name"
    change_controller_srv_name: str = "change_controller_srv_name"
    config_pure_pursuit_srv_name: str = "config_pure_pursuit_srv_name"
    config_stanley_srv_name: str = "config_stanley_srv_name"
    config_dynamic_pure_srv_name: str = "config_dynamic_pure_srv_name"
    config_dynamic_la_pure_srv_name: str = "config_dynamic_la_pure_srv_name"
    config_regulated_pure_srv_name: str = "config_regulated_pure_srv_name"
    path_planner_srv_name: str = "path_planner_srv_name"
    receive_ll_path_srv_name: str = "receive_ll_path_srv_name"


@dataclass(frozen=True)
class cuadrigaGuiDefaults:
    """Default values matching GUI_pkg."""

    # topics
    filtered_gps_topic_name: str = "/fixposition/navsatfix"
    base_gps_topic_name: str = "/cuadriga/base/gps/fix"
    rover_gps_topic_name: str = "/cuadriga/rover/gps/fix"
    imu_head_topic_name: str = "/imu_head/data"
    look_ahead_topic_name: str = "/cuadriga/look_ahead_point"
    local_path_topic_name: str = "/cuadriga/local_trajectory_ll"
    odometry_topic_name: str = "/fixposition/odometry_enu"
    human_gps_topic_name: str = "/Human/gps/fix"
    emergency_call_topic_name: str = "/Human/emergency_call"

    # fsm topics (names, later namespaced)
    fsm_get_possible_transition_topic_name: str = "possible_transitions"
    fsm_get_fsm_topic_name: str = "fsm_mode"

    # tf
    local_fixed_frame: str = "FP_ENU0"
    robot_frame: str = "FP_POI"

    # localization status
    localization_status_topic_name: str = "/fixposition/localization_status"

    # follow zed image
    followzed_topic_base: str = "/follow_zed/image_for_gui"

    # services (names, later namespaced)
    fsm_change_fsm_mode_srv_name: str = "change_fsm"
    fsm_get_fsm_srv_name: str = "get_fsm_mode"
    fsm_get_possible_transition_srv_name: str = "get_possible_transitions"
    config_controller_srv_name: str = "config_controller"
    read_path_service: str = "read_path_file"
    return_path_service: str = "get_robot_Path"
    write_path_service: str = "write_path_file"
    enable_security_check_srv_name: str = "enable_security_check"
    get_security_check_srv_name: str = "get_security_check"
    change_controller_srv_name: str = "change_controller_type"
    config_pure_pursuit_srv_name: str = "config_pure_pursuit"
    config_stanley_srv_name: str = "config_stanley"
    config_dynamic_pure_srv_name: str = "config_dynamic_pure"
    config_dynamic_la_pure_srv_name: str = "config_dynamic_la_pure"
    config_regulated_pure_srv_name: str = "config_regulated_pure"
    path_planner_srv_name: str = "path_planner"
    receive_ll_path_srv_name: str = "receive_ll_path"


def namespaced(namespace: str, name: str) -> str:
    if not name:
        return name
    if name.startswith("/"):
        return name
    return f"/{namespace}/{name}"
