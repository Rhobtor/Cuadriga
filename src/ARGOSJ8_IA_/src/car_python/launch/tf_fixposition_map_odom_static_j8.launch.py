from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    odom_topic_arg = DeclareLaunchArgument(
        "odom_topic",
        default_value="/fixposition/odometry_enu",
        description="Fixposition odometry topic",
    )

    map_frame_arg = DeclareLaunchArgument("map_frame", default_value="map")
    odom_frame_arg = DeclareLaunchArgument("odom_frame", default_value="odom")
    base_link_frame_arg = DeclareLaunchArgument("base_link_frame", default_value="base_link")

    source_parent_frame_arg = DeclareLaunchArgument(
        "source_parent_frame",
        default_value="",
        description="Override msg.header.frame_id if empty/bad",
    )
    source_child_frame_arg = DeclareLaunchArgument(
        "source_child_frame",
        default_value="",
        description="Override msg.child_frame_id if empty/bad",
    )

    # Offset from Fixposition POI frame to base_link
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    align_yaw_arg = DeclareLaunchArgument(
        "align_yaw",
        default_value="False",
        description="If True, only align yaw at startup (keep map level)",
    )

    publish_source_parent_alias_arg = DeclareLaunchArgument(
        "publish_source_parent_alias",
        default_value="True",
        description="Publish identity TF odom_frame->source_parent_frame (if different)",
    )

    freeze_base_link_static_arg = DeclareLaunchArgument(
        "freeze_base_link_static",
        default_value="False",
        description="If True, publish odom->base_link as static using the first odom sample (freezes pose)",
    )

    node = Node(
        package="car",
        executable="fixposition_map_odom_static_from_odom",
        name="fixposition_map_odom_static_from_odom",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                "map_frame": LaunchConfiguration("map_frame"),
                "odom_frame": LaunchConfiguration("odom_frame"),
                "base_link_frame": LaunchConfiguration("base_link_frame"),
                "source_parent_frame": LaunchConfiguration("source_parent_frame"),
                "source_child_frame": LaunchConfiguration("source_child_frame"),
                "x": LaunchConfiguration("x"),
                "y": LaunchConfiguration("y"),
                "z": LaunchConfiguration("z"),
                "roll": LaunchConfiguration("roll"),
                "pitch": LaunchConfiguration("pitch"),
                "yaw": LaunchConfiguration("yaw"),
                "align_yaw": LaunchConfiguration("align_yaw"),
                "publish_source_parent_alias": LaunchConfiguration("publish_source_parent_alias"),
                "freeze_base_link_static": LaunchConfiguration("freeze_base_link_static"),
            }
        ],
    )

    return LaunchDescription(
        [
            odom_topic_arg,
            map_frame_arg,
            odom_frame_arg,
            base_link_frame_arg,
            source_parent_frame_arg,
            source_child_frame_arg,
            x_arg,
            y_arg,
            z_arg,
            roll_arg,
            pitch_arg,
            yaw_arg,
            align_yaw_arg,
            publish_source_parent_alias_arg,
            freeze_base_link_static_arg,
            node,
        ]
    )
