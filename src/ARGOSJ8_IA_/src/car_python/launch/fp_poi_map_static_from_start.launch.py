#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    odom_topic_arg = DeclareLaunchArgument("odom_topic", default_value="/fixposition/odometry_enu")
    local_fixed_frame_arg = DeclareLaunchArgument("local_fixed_frame", default_value="FP_ENU0")
    fixposition_robot_frame_arg = DeclareLaunchArgument("fixposition_robot_frame", default_value="FP_POI")
    map_frame_arg = DeclareLaunchArgument("map_frame", default_value="map")

    node = Node(
        package="car",
        executable="fp_poi_map_static_from_start",
        name="fp_poi_map_static_from_start",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "local_fixed_frame": LaunchConfiguration("local_fixed_frame"),
                "fixposition_robot_frame": LaunchConfiguration("fixposition_robot_frame"),
                "map_frame": LaunchConfiguration("map_frame"),
            }
        ],
    )

    return LaunchDescription(
        [
            odom_topic_arg,
            local_fixed_frame_arg,
            fixposition_robot_frame_arg,
            map_frame_arg,
            node,
        ]
    )
