#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Reuse the verified TF chain up to camera_link / zed_camera_link
    odom_topic_arg = DeclareLaunchArgument("odom_topic", default_value="/fixposition/odometry_enu")
    local_fixed_frame_arg = DeclareLaunchArgument("local_fixed_frame", default_value="FP_ENU0")
    fp_poi_frame_arg = DeclareLaunchArgument("fp_poi_frame", default_value="FP_POI")
    map_frame_arg = DeclareLaunchArgument("map_frame", default_value="map")
    odom_frame_arg = DeclareLaunchArgument("odom_frame", default_value="odom")
    base_link_frame_arg = DeclareLaunchArgument("base_link_frame", default_value="base_link")

    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    disable_if_tf_exists_arg = DeclareLaunchArgument("disable_if_tf_exists", default_value="true")
    tf_publish_rate_hz_arg = DeclareLaunchArgument("tf_publish_rate_hz", default_value="30.0")
    tf_lookup_timeout_sec_arg = DeclareLaunchArgument("tf_lookup_timeout_sec", default_value="0.2")

    camera_frame_arg = DeclareLaunchArgument("camera_frame", default_value="camera_link")
    camera_x_arg = DeclareLaunchArgument("camera_x", default_value="0.9206628")
    camera_y_arg = DeclareLaunchArgument("camera_y", default_value="0.0075201")
    camera_z_arg = DeclareLaunchArgument("camera_z", default_value="0.75542")
    camera_roll_arg = DeclareLaunchArgument("camera_roll", default_value="0.0")
    camera_pitch_arg = DeclareLaunchArgument("camera_pitch", default_value="0.0")
    camera_yaw_arg = DeclareLaunchArgument("camera_yaw", default_value="0.0")

    zed_link_frame_arg = DeclareLaunchArgument("zed_link_frame", default_value="zed_camera_link")
    zed_link_x_arg = DeclareLaunchArgument("zed_link_x", default_value="0.0")
    zed_link_y_arg = DeclareLaunchArgument("zed_link_y", default_value="0.0")
    zed_link_z_arg = DeclareLaunchArgument("zed_link_z", default_value="0.04")
    zed_link_roll_arg = DeclareLaunchArgument("zed_link_roll", default_value="0.0")
    zed_link_pitch_arg = DeclareLaunchArgument("zed_link_pitch", default_value="0.0")
    zed_link_yaw_arg = DeclareLaunchArgument("zed_link_yaw", default_value="0.0")

    # ZED wrapper itself, with TF disabled
    camera_model_arg = DeclareLaunchArgument("camera_model", default_value="zed2i")
    camera_name_arg = DeclareLaunchArgument("camera_name", default_value="zed")
    zed_publish_tf_arg = DeclareLaunchArgument("zed_publish_tf", default_value="false")
    zed_publish_map_tf_arg = DeclareLaunchArgument("zed_publish_map_tf", default_value="false")
    zed_publish_urdf_arg = DeclareLaunchArgument("zed_publish_urdf", default_value="false")

    base_tree = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("car"), "launch", "fp_poi_map_odom_base_link_camera_test.launch.py"])
        ),
        launch_arguments={
            "odom_topic": LaunchConfiguration("odom_topic"),
            "local_fixed_frame": LaunchConfiguration("local_fixed_frame"),
            "fp_poi_frame": LaunchConfiguration("fp_poi_frame"),
            "map_frame": LaunchConfiguration("map_frame"),
            "odom_frame": LaunchConfiguration("odom_frame"),
            "base_link_frame": LaunchConfiguration("base_link_frame"),
            "x": LaunchConfiguration("x"),
            "y": LaunchConfiguration("y"),
            "z": LaunchConfiguration("z"),
            "roll": LaunchConfiguration("roll"),
            "pitch": LaunchConfiguration("pitch"),
            "yaw": LaunchConfiguration("yaw"),
            "disable_if_tf_exists": LaunchConfiguration("disable_if_tf_exists"),
            "tf_publish_rate_hz": LaunchConfiguration("tf_publish_rate_hz"),
            "tf_lookup_timeout_sec": LaunchConfiguration("tf_lookup_timeout_sec"),
            "camera_frame": LaunchConfiguration("camera_frame"),
            "camera_x": LaunchConfiguration("camera_x"),
            "camera_y": LaunchConfiguration("camera_y"),
            "camera_z": LaunchConfiguration("camera_z"),
            "camera_roll": LaunchConfiguration("camera_roll"),
            "camera_pitch": LaunchConfiguration("camera_pitch"),
            "camera_yaw": LaunchConfiguration("camera_yaw"),
            "zed_link_frame": LaunchConfiguration("zed_link_frame"),
            "zed_link_x": LaunchConfiguration("zed_link_x"),
            "zed_link_y": LaunchConfiguration("zed_link_y"),
            "zed_link_z": LaunchConfiguration("zed_link_z"),
            "zed_link_roll": LaunchConfiguration("zed_link_roll"),
            "zed_link_pitch": LaunchConfiguration("zed_link_pitch"),
            "zed_link_yaw": LaunchConfiguration("zed_link_yaw"),
        }.items(),
    )

    zed_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("zed_wrapper"), "launch", "zed_camera.launch.py"])
        ),
        launch_arguments={
            "camera_model": LaunchConfiguration("camera_model"),
            "camera_name": LaunchConfiguration("camera_name"),
            "publish_tf": LaunchConfiguration("zed_publish_tf"),
            "publish_map_tf": LaunchConfiguration("zed_publish_map_tf"),
            "publish_urdf": LaunchConfiguration("zed_publish_urdf"),
        }.items(),
    )

    return LaunchDescription([
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
        camera_frame_arg,
        camera_x_arg,
        camera_y_arg,
        camera_z_arg,
        camera_roll_arg,
        camera_pitch_arg,
        camera_yaw_arg,
        zed_link_frame_arg,
        zed_link_x_arg,
        zed_link_y_arg,
        zed_link_z_arg,
        zed_link_roll_arg,
        zed_link_pitch_arg,
        zed_link_yaw_arg,
        camera_model_arg,
        camera_name_arg,
        zed_publish_tf_arg,
        zed_publish_map_tf_arg,
        zed_publish_urdf_arg,
        base_tree,
        zed_launch,
    ])
