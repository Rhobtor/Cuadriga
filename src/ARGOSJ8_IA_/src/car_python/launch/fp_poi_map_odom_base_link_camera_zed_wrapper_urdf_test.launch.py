#!/usr/bin/env python3

import os

import xacro
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Reuse the already verified chain up to ZED wrapper (with TF disabled)
    base_pipeline = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("car"), "launch", "fp_poi_map_odom_base_link_camera_zed_wrapper_test.launch.py"]
            )
        )
    )

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

    # Minimal internal ZED TF tree required by the wrapper when publish_tf=false.
    zed_internal_tf_tree = Node(
        package="car",
        executable="zed_static_tf_tree",
        name="zed_static_tf_tree",
        output="screen",
        parameters=[
            {
                "root_frame": "zed_camera_link",
                "baseline_m": 0.12,
            }
        ],
    )

    return LaunchDescription(
        [
            base_pipeline,
            robot_state_publisher_node,
            joint_state_publisher_node,
            zed_internal_tf_tree,
        ]
    )
