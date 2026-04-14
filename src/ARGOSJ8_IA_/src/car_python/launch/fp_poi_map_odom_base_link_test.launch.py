#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # Input odometry from Fixposition
    odom_topic_arg = DeclareLaunchArgument("odom_topic", default_value="/fixposition/odometry_enu")
    local_fixed_frame_arg = DeclareLaunchArgument("local_fixed_frame", default_value="FP_ENU0")
    fp_poi_frame_arg = DeclareLaunchArgument("fp_poi_frame", default_value="FP_POI")

    # Output TF frames
    map_frame_arg = DeclareLaunchArgument("map_frame", default_value="map")
    odom_frame_arg = DeclareLaunchArgument("odom_frame", default_value="odom")
    base_link_frame_arg = DeclareLaunchArgument("base_link_frame", default_value="base_link")

    # Optional rigid offset FP_POI -> base_link
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    # Base_link TF publishing behavior
    disable_if_tf_exists_arg = DeclareLaunchArgument("disable_if_tf_exists", default_value="true")
    tf_publish_rate_hz_arg = DeclareLaunchArgument("tf_publish_rate_hz", default_value="30.0")
    tf_lookup_timeout_sec_arg = DeclareLaunchArgument("tf_lookup_timeout_sec", default_value="0.2")

    static_map_odom = Node(
        package="car",
        executable="fp_poi_map_odom_static_test",
        name="fp_poi_map_odom_static_test",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "local_fixed_frame": LaunchConfiguration("local_fixed_frame"),
                "fp_poi_frame": LaunchConfiguration("fp_poi_frame"),
                "map_frame": LaunchConfiguration("map_frame"),
                "odom_frame": LaunchConfiguration("odom_frame"),
            }
        ],
    )

    odom_to_base_link = Node(
        package="car",
        executable="fixposition_odom_to_base_link_tf",
        name="fixposition_odom_to_base_link_tf_test",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "target_parent_frame": LaunchConfiguration("odom_frame"),
                "output_child_frame": LaunchConfiguration("base_link_frame"),
                "lookup_target_frame": LaunchConfiguration("map_frame"),
                "source_parent_frame": LaunchConfiguration("local_fixed_frame"),
                "source_child_frame": LaunchConfiguration("fp_poi_frame"),
                "x": LaunchConfiguration("x"),
                "y": LaunchConfiguration("y"),
                "z": LaunchConfiguration("z"),
                "roll": LaunchConfiguration("roll"),
                "pitch": LaunchConfiguration("pitch"),
                "yaw": LaunchConfiguration("yaw"),
                "disable_if_tf_exists": LaunchConfiguration("disable_if_tf_exists"),
                "tf_publish_rate_hz": LaunchConfiguration("tf_publish_rate_hz"),
                "tf_lookup_timeout_sec": LaunchConfiguration("tf_lookup_timeout_sec"),
            }
        ],
    )

    return LaunchDescription(
        [
            odom_topic_arg,
            local_fixed_frame_arg,
            fp_poi_frame_arg,
            map_frame_arg,
            odom_frame_arg,
            base_link_frame_arg,
            x_arg,
            y_arg,
            z_arg,
            roll_arg,
            pitch_arg,
            yaw_arg,
            disable_if_tf_exists_arg,
            tf_publish_rate_hz_arg,
            tf_lookup_timeout_sec_arg,
            static_map_odom,
            odom_to_base_link,
        ]
    )
