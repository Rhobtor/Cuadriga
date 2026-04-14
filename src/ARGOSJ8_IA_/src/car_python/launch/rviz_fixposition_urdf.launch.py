from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

import os
import xacro
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Frames
    robot_frame_arg = DeclareLaunchArgument(
        "fixposition_robot_frame", default_value="FP_POI", description="Fixposition robot frame (parent)"
    )
    base_link_arg = DeclareLaunchArgument(
        "base_link_frame", default_value="base_link", description="URDF root frame (child)"
    )
    local_fixed_arg = DeclareLaunchArgument(
        "local_fixed_frame", default_value="FP_ENU0", description="Fixposition local ENU frame"
    )
    map_frame_arg = DeclareLaunchArgument(
        "map_frame", default_value="map", description="Map frame to alias to local_fixed_frame"
    )

    # Static TF (FP_POI -> base_link) offset
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    publish_map_tf_arg = DeclareLaunchArgument(
        "publish_map_tf",
        default_value="True",
        description="Anchor map at startup using first /fixposition/odometry_enu sample",
    )

    odom_topic_arg = DeclareLaunchArgument(
        "odom_topic",
        default_value="/fixposition/odometry_enu",
        description="Odometry topic whose first sample defines the map origin",
    )

    publish_fixposition_odom_tf_arg = DeclareLaunchArgument(
        "publish_fixposition_odom_tf",
        default_value="True",
        description="Publish TF local_fixed_frame->fixposition_robot_frame from odometry (needed if Fixposition driver doesn't publish TF)",
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

    rviz_arg = DeclareLaunchArgument("rviz", default_value="True")
    gui_arg = DeclareLaunchArgument("gui", default_value="False")

    # URDF
    share_dir = get_package_share_directory("car")
    xacro_file = os.path.join(share_dir, "urdf", "car.xacro")
    robot_description_config = xacro.process_file(xacro_file)
    robot_urdf = robot_description_config.toxml()

    rviz_config_file = os.path.join(share_dir, "config", "display.rviz")

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        parameters=[{"robot_description": robot_urdf}],
        output="screen",
    )

    joint_state_publisher_node = Node(
        condition=IfCondition(LaunchConfiguration("gui")),
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
        name="joint_state_publisher_gui",
        output="screen",
    )

    # Connect Fixposition TF tree to URDF tree
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
    rviz_condition = IfCondition(
        PythonExpression(["'", LaunchConfiguration("rviz"), "'.lower() in ['true','1','yes','on']"])
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
                "align_yaw": LaunchConfiguration("align_yaw"),
            }
        ],
    )

    fixposition_odom_to_tf = Node(
        condition=publish_fixposition_odom_tf_condition,
        package="car",
        executable="odom_to_tf",
        name="fixposition_odom_to_tf",
        output="screen",
        parameters=[{"odom_topic": LaunchConfiguration("odom_topic")}],
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

    rviz_node = Node(
        condition=rviz_condition,
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=["-d", rviz_config_file],
        output="screen",
    )

    return LaunchDescription(
        [
            robot_frame_arg,
            base_link_arg,
            local_fixed_arg,
            map_frame_arg,
            x_arg,
            y_arg,
            z_arg,
            roll_arg,
            pitch_arg,
            yaw_arg,
            publish_map_tf_arg,
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
            rviz_arg,
            gui_arg,
            robot_state_publisher_node,
            joint_state_publisher_node,
            fix_to_base_tf,
            map_anchor_node,
            fixposition_odom_to_tf,
            base_to_camera_tf,
            rviz_node,
        ]
    )
