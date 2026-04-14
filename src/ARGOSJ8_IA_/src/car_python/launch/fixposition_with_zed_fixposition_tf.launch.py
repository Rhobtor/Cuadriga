#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution


def generate_launch_description():
    # ZED wrapper args
    camera_model_arg = DeclareLaunchArgument("camera_model", default_value="zed2i")
    camera_name_arg = DeclareLaunchArgument("camera_name", default_value="zed")

    # IMPORTANT: keep TF disabled to avoid conflicts; Fixposition provides map/odom/base_link
    zed_publish_tf_arg = DeclareLaunchArgument("zed_publish_tf", default_value="false")
    zed_publish_map_tf_arg = DeclareLaunchArgument("zed_publish_map_tf", default_value="false")
    zed_publish_urdf_arg = DeclareLaunchArgument("zed_publish_urdf", default_value="false")

    # Fixposition odometry input + frames
    map_frame_arg = DeclareLaunchArgument("map_frame", default_value="map")
    odom_frame_arg = DeclareLaunchArgument("odom_frame", default_value="odom")
    base_link_frame_arg = DeclareLaunchArgument("base_link_frame", default_value="base_link")

    odom_topic_arg = DeclareLaunchArgument("odom_topic", default_value="/fixposition/odometry_enu")
    fp_frame_arg = DeclareLaunchArgument("fp_frame", default_value="FP_ENU0")
    fixposition_robot_frame_arg = DeclareLaunchArgument("fixposition_robot_frame", default_value="FP_POI")

    align_yaw_arg = DeclareLaunchArgument("align_yaw", default_value="false")

    # Optional rigid offset Fixposition robot frame -> base_link
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    # Mounting TFs for ZED placement
    publish_base_camera_tf_arg = DeclareLaunchArgument("publish_base_camera_tf", default_value="true")
    publish_camera_to_zed_link_tf_arg = DeclareLaunchArgument("publish_camera_to_zed_link_tf", default_value="true")

    camera_parent_frame_arg = DeclareLaunchArgument("camera_parent_frame", default_value="base_link")
    camera_frame_arg = DeclareLaunchArgument("camera_frame", default_value="camera_link")
    camera_x_arg = DeclareLaunchArgument("camera_x", default_value="0.9206628")
    camera_y_arg = DeclareLaunchArgument("camera_y", default_value="0.0075201")
    camera_z_arg = DeclareLaunchArgument("camera_z", default_value="0.75542")
    camera_roll_arg = DeclareLaunchArgument("camera_roll", default_value="0.0")
    camera_pitch_arg = DeclareLaunchArgument("camera_pitch", default_value="0.0")
    camera_yaw_arg = DeclareLaunchArgument("camera_yaw", default_value="0.0")

    zed_link_parent_frame_arg = DeclareLaunchArgument("zed_link_parent_frame", default_value="camera_link")
    zed_link_frame_arg = DeclareLaunchArgument("zed_link_frame", default_value="zed_camera_link")
    zed_link_x_arg = DeclareLaunchArgument("zed_link_x", default_value="0.0")
    zed_link_y_arg = DeclareLaunchArgument("zed_link_y", default_value="0.0")
    zed_link_z_arg = DeclareLaunchArgument("zed_link_z", default_value="0.04")
    zed_link_roll_arg = DeclareLaunchArgument("zed_link_roll", default_value="0.0")
    zed_link_pitch_arg = DeclareLaunchArgument("zed_link_pitch", default_value="0.0")
    zed_link_yaw_arg = DeclareLaunchArgument("zed_link_yaw", default_value="0.0")

    # Include: Fixposition TF tree + URDF + mounting (single launch)
    fixposition_urdf_and_tf = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("car"), "launch", "fixposition_local_j8_urdf_fixposition.launch.py"])
        ),
        launch_arguments={
            "map_frame": LaunchConfiguration("map_frame"),
            "odom_frame": LaunchConfiguration("odom_frame"),
            "base_link_frame": LaunchConfiguration("base_link_frame"),
            "odom_topic": LaunchConfiguration("odom_topic"),
            "fp_frame": LaunchConfiguration("fp_frame"),
            "fixposition_robot_frame": LaunchConfiguration("fixposition_robot_frame"),
            "x": LaunchConfiguration("x"),
            "y": LaunchConfiguration("y"),
            "z": LaunchConfiguration("z"),
            "roll": LaunchConfiguration("roll"),
            "pitch": LaunchConfiguration("pitch"),
            "yaw": LaunchConfiguration("yaw"),
            "align_yaw": LaunchConfiguration("align_yaw"),
            "publish_base_camera_tf": LaunchConfiguration("publish_base_camera_tf"),
            "camera_parent_frame": LaunchConfiguration("camera_parent_frame"),
            "camera_frame": LaunchConfiguration("camera_frame"),
            "camera_x": LaunchConfiguration("camera_x"),
            "camera_y": LaunchConfiguration("camera_y"),
            "camera_z": LaunchConfiguration("camera_z"),
            "camera_roll": LaunchConfiguration("camera_roll"),
            "camera_pitch": LaunchConfiguration("camera_pitch"),
            "camera_yaw": LaunchConfiguration("camera_yaw"),
            "publish_camera_to_zed_link_tf": LaunchConfiguration("publish_camera_to_zed_link_tf"),
            "zed_link_parent_frame": LaunchConfiguration("zed_link_parent_frame"),
            "zed_link_frame": LaunchConfiguration("zed_link_frame"),
            "zed_link_x": LaunchConfiguration("zed_link_x"),
            "zed_link_y": LaunchConfiguration("zed_link_y"),
            "zed_link_z": LaunchConfiguration("zed_link_z"),
            "zed_link_roll": LaunchConfiguration("zed_link_roll"),
            "zed_link_pitch": LaunchConfiguration("zed_link_pitch"),
            "zed_link_yaw": LaunchConfiguration("zed_link_yaw"),
        }.items(),
    )

    # Include: ZED wrapper (TF disabled)
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

    return LaunchDescription(
        [
            camera_model_arg,
            camera_name_arg,
            zed_publish_tf_arg,
            zed_publish_map_tf_arg,
            zed_publish_urdf_arg,
            map_frame_arg,
            odom_frame_arg,
            base_link_frame_arg,
            odom_topic_arg,
            fp_frame_arg,
            fixposition_robot_frame_arg,
            align_yaw_arg,
            x_arg,
            y_arg,
            z_arg,
            roll_arg,
            pitch_arg,
            yaw_arg,
            publish_base_camera_tf_arg,
            publish_camera_to_zed_link_tf_arg,
            camera_parent_frame_arg,
            camera_frame_arg,
            camera_x_arg,
            camera_y_arg,
            camera_z_arg,
            camera_roll_arg,
            camera_pitch_arg,
            camera_yaw_arg,
            zed_link_parent_frame_arg,
            zed_link_frame_arg,
            zed_link_x_arg,
            zed_link_y_arg,
            zed_link_z_arg,
            zed_link_roll_arg,
            zed_link_pitch_arg,
            zed_link_yaw_arg,
            fixposition_urdf_and_tf,
            zed_launch,
        ]
    )
