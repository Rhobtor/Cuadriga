#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution


def generate_launch_description():
    params_yaml = PathJoinSubstitution([
        FindPackageShare('car'),
        'config',
        'gnss_goals.yaml',
    ])

    gnss_goal_circuit = Node(
        package='car',
        executable='gnss_goal_circuit',
        name='gnss_goal_circuit',
        parameters=[params_yaml],
    )

    check_goal_gnss = Node(
        package='car',
        executable='check_goal_gnss',
        name='check_goal_gnss',
        parameters=[params_yaml],
    )

    return LaunchDescription([
        gnss_goal_circuit,
        check_goal_gnss,
    ])
