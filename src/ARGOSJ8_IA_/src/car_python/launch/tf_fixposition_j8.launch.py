from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    # Frames
    fix_robot_frame_arg = DeclareLaunchArgument(
        "fixposition_robot_frame",
        default_value="FP_POI",
        description="Fixposition robot/body frame",
    )
    local_fixed_frame_arg = DeclareLaunchArgument(
        "local_fixed_frame",
        default_value="FP_ENU0",
        description="Fixposition local ENU frame",
    )
    global_frame_arg = DeclareLaunchArgument(
        "global_frame",
        default_value="FP_ENU0",
        description="Parent frame for map anchoring (set to ECEF if you want ECEF->map)",
    )
    map_frame_arg = DeclareLaunchArgument(
        "map_frame",
        default_value="map",
        description="Map frame name",
    )
    odom_frame_arg = DeclareLaunchArgument(
        "odom_frame",
        default_value="odom",
        description="Odom frame name",
    )
    base_link_frame_arg = DeclareLaunchArgument(
        "base_link_frame",
        default_value="base_link",
        description="Base link frame name",
    )

    # Static TF offset (FP_POI -> base_link)
    x_arg = DeclareLaunchArgument("x", default_value="0.0")
    y_arg = DeclareLaunchArgument("y", default_value="0.0")
    z_arg = DeclareLaunchArgument("z", default_value="0.0")
    roll_arg = DeclareLaunchArgument("roll", default_value="0.0")
    pitch_arg = DeclareLaunchArgument("pitch", default_value="0.0")
    yaw_arg = DeclareLaunchArgument("yaw", default_value="0.0")

    # Core behavior
    publish_base_link_in_odom_arg = DeclareLaunchArgument(
        "publish_base_link_in_odom",
        default_value="True",
        description="Publish dynamic TF odom->base_link from Fixposition odometry",
    )
    publish_map_tf_arg = DeclareLaunchArgument(
        "publish_map_tf",
        default_value="True",
        description="Anchor map at startup using first odometry sample",
    )
    publish_odom_alias_tf_arg = DeclareLaunchArgument(
        "publish_odom_alias_tf",
        default_value="True",
        description="Publish identity TF map->odom",
    )
    publish_fixposition_odom_tf_arg = DeclareLaunchArgument(
        "publish_fixposition_odom_tf",
        default_value="True",
        description="Publish TF local_fixed_frame->fixposition_robot_frame from odometry when Fixposition TF is missing",
    )
    align_yaw_arg = DeclareLaunchArgument(
        "align_yaw",
        default_value="False",
        description="If True, rotate map so initial robot yaw is ~0",
    )
    odom_topic_arg = DeclareLaunchArgument(
        "odom_topic",
        default_value="/fixposition/odometry_enu",
        description="Odometry topic",
    )

    tf_publish_rate_hz_arg = DeclareLaunchArgument(
        "tf_publish_rate_hz",
        default_value="30.0",
        description="Max TF publish rate for dynamic broadcasters (0 disables throttling)",
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

    # 1) map anchor: creates map frame based on first odom sample
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

    # 2) map->odom alias (Nav2/SLAM convention)
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

    # 3) local_fixed_frame->fixposition_robot_frame TF from odometry (only if needed)
    fixposition_odom_to_tf = Node(
        condition=publish_fixposition_odom_tf_condition,
        package="car",
        executable="odom_to_tf",
        name="fixposition_odom_to_tf",
        output="screen",
        parameters=[
            {
                "odom_topic": LaunchConfiguration("odom_topic"),
                # Some odometry sources leave header.frame_id / child_frame_id empty.
                # Provide explicit overrides so TF local_fixed_frame->fixposition_robot_frame is published.
                "parent_frame": LaunchConfiguration("local_fixed_frame"),
                "child_frame": LaunchConfiguration("fixposition_robot_frame"),
                "tf_publish_rate_hz": LaunchConfiguration("tf_publish_rate_hz"),
            }
        ],
    )

    # 4) odom->base_link dynamic TF from Fixposition odometry
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
                "tf_publish_rate_hz": LaunchConfiguration("tf_publish_rate_hz"),
            }
        ],
    )

    # Fallback static transform FP_POI->base_link (only when publish_base_link_in_odom:=False)
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
            publish_fixposition_odom_tf_arg,
            align_yaw_arg,
            odom_topic_arg,
            tf_publish_rate_hz_arg,
            map_anchor_node,
            map_to_odom_alias_tf,
            fixposition_odom_to_tf,
            odom_to_base_link_tf,
            fix_to_base_tf,
        ]
    )
