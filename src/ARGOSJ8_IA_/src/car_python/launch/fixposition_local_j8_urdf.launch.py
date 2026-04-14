from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

import os
import xacro
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Frames (our TF tree)
    base_link_frame_arg = DeclareLaunchArgument("base_link_frame", default_value="base_link")

    publish_base_camera_tf_arg = DeclareLaunchArgument(
        "publish_base_camera_tf",
        default_value="False",
        description="Publish a static TF base_link->camera_link",
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
        default_value="False",
        description=(
            "Publish TF camera_link->zed_camera_link mounting offset. "
            "WARNING: If the ZED already publishes odom->zed_camera_link, this creates a TF conflict (two parents)."
        ),
    )
    zed_link_parent_frame_arg = DeclareLaunchArgument("zed_link_parent_frame", default_value="camera_link")
    zed_link_frame_arg = DeclareLaunchArgument("zed_link_frame", default_value="zed_camera_link")
    zed_link_x_arg = DeclareLaunchArgument("zed_link_x", default_value="0.0")
    zed_link_y_arg = DeclareLaunchArgument("zed_link_y", default_value="0.0")
    zed_link_z_arg = DeclareLaunchArgument("zed_link_z", default_value="0.04")
    zed_link_roll_arg = DeclareLaunchArgument("zed_link_roll", default_value="0.0")
    zed_link_pitch_arg = DeclareLaunchArgument("zed_link_pitch", default_value="0.0")
    zed_link_yaw_arg = DeclareLaunchArgument("zed_link_yaw", default_value="0.0")

    publish_zed_to_base_tf_arg = DeclareLaunchArgument(
        "publish_zed_to_base_tf",
        default_value="True",
        description=(
            "Publish a static TF zed_camera_link->base_link to connect ZED TF tree (map/odom) with the robot URDF. "
            "Enable this when the ZED publishes odom->zed_camera_link and you want base_link reachable from map."
        ),
    )
    zed_to_base_x_arg = DeclareLaunchArgument(
        "zed_to_base_x",
        default_value="-0.9206628",
        description="Static TF translation X from zed_camera_link to base_link (meters)",
    )
    zed_to_base_y_arg = DeclareLaunchArgument(
        "zed_to_base_y",
        default_value="-0.0075201",
        description="Static TF translation Y from zed_camera_link to base_link (meters)",
    )
    zed_to_base_z_arg = DeclareLaunchArgument(
        "zed_to_base_z",
        default_value="-0.75542",
        description="Static TF translation Z from zed_camera_link to base_link (meters)",
    )
    zed_to_base_roll_arg = DeclareLaunchArgument(
        "zed_to_base_roll",
        default_value="0.0",
        description="Static TF roll from zed_camera_link to base_link (radians)",
    )
    zed_to_base_pitch_arg = DeclareLaunchArgument(
        "zed_to_base_pitch",
        default_value="0.0",
        description="Static TF pitch from zed_camera_link to base_link (radians)",
    )
    zed_to_base_yaw_arg = DeclareLaunchArgument(
        "zed_to_base_yaw",
        default_value="0.0",
        description="Static TF yaw from zed_camera_link to base_link (radians)",
    )

    publish_zed_internal_tf_arg = DeclareLaunchArgument(
        "publish_zed_internal_tf",
        default_value="False",
        description="Publish a minimal ZED internal TF tree rooted at zed_camera_link",
    )
    zed_baseline_arg = DeclareLaunchArgument(
        "zed_baseline_m",
        default_value="0.12",
        description="ZED stereo baseline in meters",
    )

    # Conditions
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

    publish_zed_to_base_condition = IfCondition(
        PythonExpression(["'", LaunchConfiguration("publish_zed_to_base_tf"), "'.lower() in ['true','1','yes','on']"])
    )

    publish_zed_internal_condition = IfCondition(
        PythonExpression(
            ["'", LaunchConfiguration("publish_zed_internal_tf"), "'.lower() in ['true','1','yes','on']"]
        )
    )

    # URDF
    desc_pkg = "j8_xacro_model"
    xacro_file = os.path.join(get_package_share_directory(desc_pkg), "urdf", "argo_j8.xacro")
    robot_description_config = xacro.process_file(xacro_file)
    robot_urdf = robot_description_config.toxml()

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        parameters=[{"robot_description": robot_urdf}],
        output="screen",
    )

    joint_state_publisher_node = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher",
        name="joint_state_publisher",
        output="screen",
        parameters=[{"use_gui": False}],
    )

    # IMPORTANT:
    # This launch file assumes the ZED (or another localization source) is already publishing
    # the TF tree for map/odom. Therefore, we DO NOT publish:
    # - map -> odom
    # - odom -> base_link
    # Only URDF/state publishers and optional static mounting transforms are launched here.

    base_to_camera_tf = Node(
        condition=publish_base_camera_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_base_to_camera",
        arguments=[
            LaunchConfiguration("camera_x"),
            LaunchConfiguration("camera_y"),
            LaunchConfiguration("camera_z"),
            LaunchConfiguration("camera_yaw"),
            LaunchConfiguration("camera_pitch"),
            LaunchConfiguration("camera_roll"),
            LaunchConfiguration("camera_parent_frame"),
            LaunchConfiguration("camera_frame"),
        ],
        output="screen",
    )

    camera_to_zed_link_tf = Node(
        condition=publish_camera_to_zed_link_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_camera_to_zed_link",
        arguments=[
            LaunchConfiguration("zed_link_x"),
            LaunchConfiguration("zed_link_y"),
            LaunchConfiguration("zed_link_z"),
            LaunchConfiguration("zed_link_yaw"),
            LaunchConfiguration("zed_link_pitch"),
            LaunchConfiguration("zed_link_roll"),
            LaunchConfiguration("zed_link_parent_frame"),
            LaunchConfiguration("zed_link_frame"),
        ],
        output="screen",
    )

    zed_to_base_tf = Node(
        condition=publish_zed_to_base_condition,
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_zed_to_base",
        arguments=[
            LaunchConfiguration("zed_to_base_x"),
            LaunchConfiguration("zed_to_base_y"),
            LaunchConfiguration("zed_to_base_z"),
            LaunchConfiguration("zed_to_base_yaw"),
            LaunchConfiguration("zed_to_base_pitch"),
            LaunchConfiguration("zed_to_base_roll"),
            LaunchConfiguration("zed_link_frame"),
            LaunchConfiguration("base_link_frame"),
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

    return LaunchDescription(
        [
            base_link_frame_arg,
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
            publish_zed_to_base_tf_arg,
            zed_to_base_x_arg,
            zed_to_base_y_arg,
            zed_to_base_z_arg,
            zed_to_base_roll_arg,
            zed_to_base_pitch_arg,
            zed_to_base_yaw_arg,
            publish_zed_internal_tf_arg,
            zed_baseline_arg,
            robot_state_publisher_node,
            joint_state_publisher_node,
            zed_to_base_tf,
            base_to_camera_tf,
            camera_to_zed_link_tf,
            zed_internal_tf_tree,
        ]
    )
