from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

import os
import xacro
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Fixposition frames
    fix_robot_frame_arg = DeclareLaunchArgument(
        "fixposition_robot_frame",
        default_value="FP_POI",
        description="Fixposition robot/body frame (parent of base_link)",
    )
    local_fixed_frame_arg = DeclareLaunchArgument(
        "local_fixed_frame",
        default_value="FP_ENU0",
        description="Fixposition local ENU frame (recommended RViz fixed frame)",
    )
    global_frame_arg = DeclareLaunchArgument(
        "global_frame",
        default_value="FP_ENU0",
        description="Parent frame for map anchoring (set to ECEF if you want ECEF->map)",
    )
    map_frame_arg = DeclareLaunchArgument(
        "map_frame",
        default_value="map",
        description="Optional alias frame for RViz/SLAM stacks",
    )

    odom_frame_arg = DeclareLaunchArgument(
        "odom_frame",
        default_value="odom",
        description="Odom frame name (Nav2/SLAM convention)",
    )

    # Connect Fixposition -> URDF
    base_link_frame_arg = DeclareLaunchArgument(
        "base_link_frame",
        default_value="base_link",
        description="URDF root link name",
    )

    # Static TF offset (FP_POI -> base_link)
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    publish_base_link_in_odom_arg = DeclareLaunchArgument(
        "publish_base_link_in_odom",
        default_value="True",
        description="Publish dynamic TF odom->base_link using Fixposition odometry (recommended for a single TF tree: map->odom->base_link)",
    )

    publish_map_tf_arg = DeclareLaunchArgument(
        "publish_map_tf",
        default_value="True",
        description="Anchor map at startup using first /fixposition/odometry_enu sample",
    )

    publish_odom_alias_tf_arg = DeclareLaunchArgument(
        "publish_odom_alias_tf",
        default_value="True",
        description="Publish identity TF map->odom (Nav2 convention) without re-parenting Fixposition frames",
    )

    odom_topic_arg = DeclareLaunchArgument(
        "odom_topic",
        default_value="/fixposition/odometry_enu",
        description="Odometry topic whose first sample defines the map origin",
    )

    publish_fixposition_odom_tf_arg = DeclareLaunchArgument(
        "publish_fixposition_odom_tf",
        default_value="True",
        description="Publish TF local_fixed_frame->fixposition_robot_frame from odometry (connects the TF tree when Fixposition TF is missing)",
    )
    align_yaw_arg = DeclareLaunchArgument(
        "align_yaw",
        default_value="False",
        description="If True, rotate map so initial robot yaw is ~0",
    )

    publish_base_camera_tf_arg = DeclareLaunchArgument(
        "publish_base_camera_tf",
        default_value="True",
        description="Publish a static TF base_link->camera_link (useful if URDF/ZED driver doesn't provide it)",
    )
    camera_parent_frame_arg = DeclareLaunchArgument("camera_parent_frame", default_value="base_link")
    camera_frame_arg = DeclareLaunchArgument("camera_frame", default_value="camera_link")
    camera_x_arg = DeclareLaunchArgument("camera_x", default_value="0.9206628")
    camera_y_arg = DeclareLaunchArgument("camera_y", default_value="0.0075201")
    camera_z_arg = DeclareLaunchArgument("camera_z", default_value="0.75542")
    camera_roll_arg = DeclareLaunchArgument("camera_roll", default_value="0.0")
    camera_pitch_arg = DeclareLaunchArgument("camera_pitch", default_value="0.0")
    camera_yaw_arg = DeclareLaunchArgument("camera_yaw", default_value="0.0")

    publish_camera_to_zed_link_tf_arg = DeclareLaunchArgument(
        "publish_camera_to_zed_link_tf",
        default_value="True",
        description="Publish identity TF camera_link->zed_camera_link (common ZED root frame)",
    )
    zed_link_parent_frame_arg = DeclareLaunchArgument("zed_link_parent_frame", default_value="camera_link")
    zed_link_frame_arg = DeclareLaunchArgument("zed_link_frame", default_value="zed_camera_link")
    zed_link_x_arg = DeclareLaunchArgument("zed_link_x", default_value="0.0")
    zed_link_y_arg = DeclareLaunchArgument("zed_link_y", default_value="0.0")
    zed_link_z_arg = DeclareLaunchArgument("zed_link_z", default_value="0.04")
    zed_link_roll_arg = DeclareLaunchArgument("zed_link_roll", default_value="0.0")
    zed_link_pitch_arg = DeclareLaunchArgument("zed_link_pitch", default_value="0.0")
    zed_link_yaw_arg = DeclareLaunchArgument("zed_link_yaw", default_value="0.0")

    publish_zed_link_to_center_tf_arg = DeclareLaunchArgument(
        "publish_zed_link_to_center_tf",
        default_value="False",
        description="Publish TF zed_camera_link->zed_camera_center mounting offset",
    )
    zed_center_parent_frame_arg = DeclareLaunchArgument("zed_center_parent_frame", default_value="zed_camera_link")
    zed_center_frame_arg = DeclareLaunchArgument("zed_center_frame", default_value="zed_camera_center")
    zed_center_x_arg = DeclareLaunchArgument("zed_center_x", default_value="0.0")
    zed_center_y_arg = DeclareLaunchArgument("zed_center_y", default_value="0.0")
    zed_center_z_arg = DeclareLaunchArgument("zed_center_z", default_value="0.04")
    zed_center_roll_arg = DeclareLaunchArgument("zed_center_roll", default_value="0.0")
    zed_center_pitch_arg = DeclareLaunchArgument("zed_center_pitch", default_value="0.0")
    zed_center_yaw_arg = DeclareLaunchArgument("zed_center_yaw", default_value="0.0")

    publish_zed_static_tf_arg = DeclareLaunchArgument(
        "publish_zed_static_tf",
        default_value="False",
        description="Publish a static TF for ZED if your ZED driver does not publish TF",
    )

    publish_zed_internal_tf_arg = DeclareLaunchArgument(
        "publish_zed_internal_tf",
        default_value="True",
        description="Publish a minimal ZED internal TF tree rooted at zed_camera_link (use when ZED wrapper is not publishing TF)",
    )

    zed_baseline_arg = DeclareLaunchArgument(
        "zed_baseline_m",
        default_value="0.12",
        description="ZED stereo baseline in meters (used for zed_left/right_camera_frame)",
    )
    zed_parent_frame_arg = DeclareLaunchArgument("zed_parent_frame", default_value="Bridge")
    zed_frame_arg = DeclareLaunchArgument("zed_frame", default_value="zed_camera_link")
    zed_x_arg = DeclareLaunchArgument("zed_x", default_value="0.11")
    zed_y_arg = DeclareLaunchArgument("zed_y", default_value="0.0")
    zed_z_arg = DeclareLaunchArgument("zed_z", default_value="0.23")
    zed_roll_arg = DeclareLaunchArgument("zed_roll", default_value="0.0")
    zed_pitch_arg = DeclareLaunchArgument("zed_pitch", default_value="3.141592653589793")
    zed_yaw_arg = DeclareLaunchArgument("zed_yaw", default_value="0.0")

    # Running RViz on the robot can easily saturate CPU/GPU and DDS traffic.
    # Default to off; run RViz on the PC instead.
    rviz_arg = DeclareLaunchArgument("rviz", default_value="False")

    publish_goal_local_marker_arg = DeclareLaunchArgument(
        "publish_goal_local_marker",
        default_value="False",
        description="Publish RViz markers for /goal_local PoseArray (large spheres + optional line strip)",
    )

    # Load the SAME xacro as J8_unity.launch.py
    desc_pkg = "j8_xacro_model"
    xacro_file = os.path.join(get_package_share_directory(desc_pkg), "urdf", "argo_j8.xacro")
    robot_description_config = xacro.process_file(xacro_file)
    robot_urdf = robot_description_config.toxml()

    # Reuse existing RViz config if available (falls back to default)
    rviz_config_fallback = os.path.join(get_package_share_directory("car"), "config", "display.rviz")

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        parameters=[{"robot_description": robot_urdf}],
        output="screen",
    )

    # If you don't have a joint state source on the real robot, this at least publishes zeros.
    joint_state_publisher_node = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher",
        name="joint_state_publisher",
        output="screen",
        parameters=[{"use_gui": False}],
    )

    fix_to_base_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="fixposition_to_base_link",
        arguments=[
            "--frame-id",
            LaunchConfiguration("fixposition_robot_frame"),
            "--child-frame-id",
            LaunchConfiguration("base_link_frame"),
            "--x",
            LaunchConfiguration("x"),
            "--y",
            LaunchConfiguration("y"),
            "--z",
            LaunchConfiguration("z"),
            "--roll",
            LaunchConfiguration("roll"),
            "--pitch",
            LaunchConfiguration("pitch"),
            "--yaw",
            LaunchConfiguration("yaw"),
        ],
        output="screen",
    )

    publish_map_condition = IfCondition(
        PythonExpression(["'", LaunchConfiguration("publish_map_tf"), "'.lower() in ['true','1','yes','on']"])
    )
    publish_odom_alias_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_odom_alias_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    publish_fixposition_odom_tf_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_fixposition_odom_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    publish_base_camera_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_base_camera_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    publish_camera_to_zed_link_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_camera_to_zed_link_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    publish_zed_link_to_center_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_zed_link_to_center_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    rviz_condition = IfCondition(
        PythonExpression(["'", LaunchConfiguration("rviz"), "'.lower() in ['true','1','yes','on']"])
    )

    publish_goal_local_marker_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_goal_local_marker"), "'.lower() in ['true','1','yes','on']"]
        )
    )
    publish_zed_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_zed_static_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )

    publish_zed_internal_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_zed_internal_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )

    map_anchor_node = Node(
        condition=publish_map_condition,
        package="car",
        executable="fixposition_map_anchor",
        name="fixposition_map_anchor",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "map_frame": LaunchConfiguration("map_frame"),
                "local_fixed_frame": LaunchConfiguration("local_fixed_frame"),
                "global_frame": LaunchConfiguration("global_frame"),
                "align_yaw": LaunchConfiguration("align_yaw"),
            }
        ],
    )

    # Create an 'odom' alias under map so Nav2/SLAM conventions work.
    map_to_odom_alias_tf = Node(
        condition=publish_odom_alias_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="map_to_odom_alias",
        arguments=[
            "--frame-id",
            LaunchConfiguration("map_frame"),
            "--child-frame-id",
            LaunchConfiguration("odom_frame"),
            "--x",
            "0",
            "--y",
            "0",
            "--z",
            "0",
            "--roll",
            "0",
            "--pitch",
            "0",
            "--yaw",
            "0",
        ],
        output="screen",
    )

    # If Fixposition odometry is present but TF is missing, generate FP_ENU0->FP_POI from /fixposition/odometry_enu.
    fixposition_odom_to_tf = Node(
        condition=publish_fixposition_odom_tf_condition,
        package="car",
        executable="odom_to_tf",
        name="fixposition_odom_to_tf",
        output="screen",
        parameters=[{"odom_topic": LaunchConfiguration("odom_topic")}],
    )

    publish_base_link_in_odom_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_base_link_in_odom"), "'.lower() in ['true','1','yes','on']"]
        )
    )

    publish_static_fix_to_base_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_base_link_in_odom"), "'.lower() not in ['true','1','yes','on']"]
        )
    )

    # Preferred mode: publish odom->base_link dynamically from /fixposition/odometry_enu,
    # using TF to convert from the odometry frame into odom.
    odom_to_base_link_tf = Node(
        condition=publish_base_link_in_odom_condition,
        package="car",
        executable="fixposition_odom_to_base_link_tf",
        name="fixposition_odom_to_base_link_tf",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "target_parent_frame": LaunchConfiguration("odom_frame"),
                "output_child_frame": LaunchConfiguration("base_link_frame"),
                "lookup_target_frame": LaunchConfiguration("map_frame"),
                "source_parent_frame": LaunchConfiguration("local_fixed_frame"),
                "source_child_frame": LaunchConfiguration("fixposition_robot_frame"),
                "x": LaunchConfiguration("x"),
                "y": LaunchConfiguration("y"),
                "z": LaunchConfiguration("z"),
                "roll": LaunchConfiguration("roll"),
                "pitch": LaunchConfiguration("pitch"),
                "yaw": LaunchConfiguration("yaw"),
                "disable_if_tf_exists": True,
            }
        ],
    )

    # Legacy mode: static (fixposition_robot_frame -> base_link)
    fix_to_base_tf = Node(
        condition=publish_static_fix_to_base_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="fixposition_to_base_link",
        arguments=[
            "--frame-id",
            LaunchConfiguration("fixposition_robot_frame"),
            "--child-frame-id",
            LaunchConfiguration("base_link_frame"),
            "--x",
            LaunchConfiguration("x"),
            "--y",
            LaunchConfiguration("y"),
            "--z",
            LaunchConfiguration("z"),
            "--roll",
            LaunchConfiguration("roll"),
            "--pitch",
            LaunchConfiguration("pitch"),
            "--yaw",
            LaunchConfiguration("yaw"),
        ],
        output="screen",
    )

    base_to_camera_tf = Node(
        condition=publish_base_camera_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_base_to_camera",
        arguments=[
            "--frame-id",
            LaunchConfiguration("camera_parent_frame"),
            "--child-frame-id",
            LaunchConfiguration("camera_frame"),
            "--x",
            LaunchConfiguration("camera_x"),
            "--y",
            LaunchConfiguration("camera_y"),
            "--z",
            LaunchConfiguration("camera_z"),
            "--roll",
            LaunchConfiguration("camera_roll"),
            "--pitch",
            LaunchConfiguration("camera_pitch"),
            "--yaw",
            LaunchConfiguration("camera_yaw"),
        ],
        output="screen",
    )

    camera_to_zed_link_tf = Node(
        condition=publish_camera_to_zed_link_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_camera_to_zed_link",
        arguments=[
            "--frame-id",
            LaunchConfiguration("zed_link_parent_frame"),
            "--child-frame-id",
            LaunchConfiguration("zed_link_frame"),
            "--x",
            LaunchConfiguration("zed_link_x"),
            "--y",
            LaunchConfiguration("zed_link_y"),
            "--z",
            LaunchConfiguration("zed_link_z"),
            "--roll",
            LaunchConfiguration("zed_link_roll"),
            "--pitch",
            LaunchConfiguration("zed_link_pitch"),
            "--yaw",
            LaunchConfiguration("zed_link_yaw"),
        ],
        output="screen",
    )

    zed_link_to_center_tf = Node(
        condition=publish_zed_link_to_center_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_zed_link_to_center",
        arguments=[
            "--frame-id",
            LaunchConfiguration("zed_center_parent_frame"),
            "--child-frame-id",
            LaunchConfiguration("zed_center_frame"),
            "--x",
            LaunchConfiguration("zed_center_x"),
            "--y",
            LaunchConfiguration("zed_center_y"),
            "--z",
            LaunchConfiguration("zed_center_z"),
            "--roll",
            LaunchConfiguration("zed_center_roll"),
            "--pitch",
            LaunchConfiguration("zed_center_pitch"),
            "--yaw",
            LaunchConfiguration("zed_center_yaw"),
        ],
        output="screen",
    )

    zed_static_tf = Node(
        condition=publish_zed_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="zed_static_tf",
        arguments=[
            "--frame-id",
            LaunchConfiguration("zed_parent_frame"),
            "--child-frame-id",
            LaunchConfiguration("zed_frame"),
            "--x",
            LaunchConfiguration("zed_x"),
            "--y",
            LaunchConfiguration("zed_y"),
            "--z",
            LaunchConfiguration("zed_z"),
            "--roll",
            LaunchConfiguration("zed_roll"),
            "--pitch",
            LaunchConfiguration("zed_pitch"),
            "--yaw",
            LaunchConfiguration("zed_yaw"),
        ],
        output="screen",
    )

    zed_internal_tf_tree = Node(
        condition=publish_zed_internal_condition,
        package="car",
        executable="zed_static_tf_tree",
        name="zed_static_tf_tree",
        output="screen",
        parameters=[
            {
                "root_frame": LaunchConfiguration("zed_link_frame"),
                "baseline_m": LaunchConfiguration("zed_baseline_m"),
            }
        ],
    )

    rviz_node = Node(
        condition=rviz_condition,
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=["-d", rviz_config_fallback],
        output="screen",
    )

    goal_local_marker_node = Node(
        condition=publish_goal_local_marker_condition,
        package="car",
        executable="goal_local_marker",
        name="goal_local_marker",
        output="screen",
        parameters=[
            {
                "input_topic": "/goal_local",
                "output_topic": "/goal_local_markers",
                "scale_xyz": 1.2,
                "publish_line": True,
                "line_width": 0.20,
            }
        ],
    )

    return LaunchDescription(
        [
            fix_robot_frame_arg,
            local_fixed_frame_arg,
            global_frame_arg,
            map_frame_arg,
            odom_frame_arg,
            base_link_frame_arg,
            x_arg,
            y_arg,
            z_arg,
            roll_arg,
            pitch_arg,
            yaw_arg,
            publish_base_link_in_odom_arg,
            publish_map_tf_arg,
            publish_odom_alias_tf_arg,
            odom_topic_arg,
            publish_fixposition_odom_tf_arg,
            align_yaw_arg,
            publish_base_camera_tf_arg,
            camera_parent_frame_arg,
            camera_frame_arg,
            camera_x_arg,
            camera_y_arg,
            camera_z_arg,
            camera_roll_arg,
            camera_pitch_arg,
            camera_yaw_arg,
            publish_camera_to_zed_link_tf_arg,
            zed_link_parent_frame_arg,
            zed_link_frame_arg,
            zed_link_x_arg,
            zed_link_y_arg,
            zed_link_z_arg,
            zed_link_roll_arg,
            zed_link_pitch_arg,
            zed_link_yaw_arg,
            publish_zed_link_to_center_tf_arg,
            zed_center_parent_frame_arg,
            zed_center_frame_arg,
            zed_center_x_arg,
            zed_center_y_arg,
            zed_center_z_arg,
            zed_center_roll_arg,
            zed_center_pitch_arg,
            zed_center_yaw_arg,
            publish_zed_static_tf_arg,

            publish_zed_internal_tf_arg,
            zed_baseline_arg,
            zed_parent_frame_arg,
            zed_frame_arg,
            zed_x_arg,
            zed_y_arg,
            zed_z_arg,
            zed_roll_arg,
            zed_pitch_arg,
            zed_yaw_arg,
            rviz_arg,
            publish_goal_local_marker_arg,
            robot_state_publisher_node,
            joint_state_publisher_node,
            odom_to_base_link_tf,
            fix_to_base_tf,
            map_anchor_node,
            map_to_odom_alias_tf,
            fixposition_odom_to_tf,
            base_to_camera_tf,
            camera_to_zed_link_tf,
            zed_link_to_center_tf,
            zed_static_tf,
            zed_internal_tf_tree,
            goal_local_marker_node,
            rviz_node,
        ]
    )
