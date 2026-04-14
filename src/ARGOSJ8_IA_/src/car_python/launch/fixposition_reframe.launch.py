from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    odom_in = DeclareLaunchArgument("odom_in", default_value="/fixposition/odometry_enu")
    gnss1_in = DeclareLaunchArgument("gnss1_in", default_value="/fixposition/gnss1")
    gnss2_in = DeclareLaunchArgument("gnss2_in", default_value="/fixposition/gnss2")

    odom_out = DeclareLaunchArgument("odom_out", default_value="/j8/fixposition/odometry")
    gnss1_out = DeclareLaunchArgument("gnss1_out", default_value="/j8/fixposition/gnss1")
    gnss2_out = DeclareLaunchArgument("gnss2_out", default_value="/j8/fixposition/gnss2")

    odom_frame = DeclareLaunchArgument("odom_frame", default_value="odom")
    base_link_frame = DeclareLaunchArgument("base_link_frame", default_value="base_link")
    map_frame = DeclareLaunchArgument("map_frame", default_value="map")

    out_reliable = DeclareLaunchArgument("out_reliable", default_value="True")
    out_depth = DeclareLaunchArgument("out_depth", default_value="50")

    preserve_input_stamp = DeclareLaunchArgument("preserve_input_stamp", default_value="True")

    node = Node(
        package="car",
        executable="fixposition_reframe",
        name="fixposition_reframe",
        output="screen",
        parameters=[
            {
                "odom_in": LaunchConfiguration("odom_in"),
                "gnss1_in": LaunchConfiguration("gnss1_in"),
                "gnss2_in": LaunchConfiguration("gnss2_in"),
                "odom_out": LaunchConfiguration("odom_out"),
                "gnss1_out": LaunchConfiguration("gnss1_out"),
                "gnss2_out": LaunchConfiguration("gnss2_out"),
                "odom_frame": LaunchConfiguration("odom_frame"),
                "base_link_frame": LaunchConfiguration("base_link_frame"),
                "map_frame": LaunchConfiguration("map_frame"),
                "out_reliable": LaunchConfiguration("out_reliable"),
                "out_depth": LaunchConfiguration("out_depth"),
                "preserve_input_stamp": LaunchConfiguration("preserve_input_stamp"),
            }
        ],
    )

    return LaunchDescription(
        [
            odom_in,
            gnss1_in,
            gnss2_in,
            odom_out,
            gnss1_out,
            gnss2_out,
            odom_frame,
            base_link_frame,
            map_frame,
            out_reliable,
            out_depth,
            preserve_input_stamp,
            node,
        ]
    )
