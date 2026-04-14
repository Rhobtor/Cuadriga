#!/usr/bin/env python3

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch_ros.actions import Node
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import Command
from launch.actions import SetEnvironmentVariable
import os
import xacro


def generate_launch_description():
    # Get the share directory of your package
    share_dir = get_package_share_directory('j8_xacro_model')

    # Instead of using os.path.join to build a string, use PathJoinSubstitution.
    # This creates a substitution that is compatible with the launch system.
    world_arg = PathJoinSubstitution([
        FindPackageShare('car'),
        'worlds',
        'world_1_b1.world'
    ])

    # Get the car URDF by processing the xacro file
    xacro_file = os.path.join(share_dir, 'urdf', 'argo_j8.xacro')
    robot_description_config = xacro.process_file(xacro_file)
    robot_urdf = robot_description_config.toxml()

    # # Nodo para publicar la transformación estática entre 'odom' y 'map'
    # # En este ejemplo, se asume que 'map' coincide con 'odom'
    # static_tf = Node(
    #     package='tf2_ros',
    #     executable='static_transform_publisher',
    #     name='static_transform_publisher_map',
    #     arguments=['0', '0', '0', '0', '0', '0', 'map', 'odom']
    # )

    # static_tf2 = Node(
    #     package='tf2_ros',
    #     executable='static_transform_publisher',
    #     name='static_transform_publisher_map',
    #     arguments=['0', '0', '0', '0', '0', '0', 'odom', 'base_link']
    # )
    goal_yaml = PathJoinSubstitution([
        FindPackageShare('car'),        # paquete donde está goal.yaml
        'config',                       # subcarpeta
        'goal.yaml'                     # nombre del archivo
    ])


    check_goal = Node(
        package='car_cpp',  # Asegúrate de que el paquete se llame 'car' o el que corresponda
        executable='check_goal',  # Nombre del ejecutable (por ejemplo, si instalaste el script con entry_point)
        name='check_goal',  # Nombre del nodo
    )

    points_goal = Node(
        package     = 'car',            # o el paquete que corresponda
        executable  = 'points_goal',    # tu ejecutable
        name        = 'points_goal',
        parameters  = [goal_yaml]       #  ← aquí se inyecta el YAML
    )




    return LaunchDescription([

        # robot_state_publisher_node,
        # gazebo_server,
        # gazebo_client,
        # urdf_spawn_node,
        # filter_points_cloud,
        # frontier_values,
        # octomap,
        # map_odom_tf,
        # navigation_nodes_ground,
        # filtered_navigation_nodes,
        # navigation_nodes,
        # obstacles_in_2d,
        # occupied_nodes_near_obstacles,
        # #move_navigation_nodes,
        check_goal,
        points_goal,
        # colision_zone,
        # navegation_map,
        # memory_map,
        # frontier_centroid,
        # gridmap,
        # slope_terrain,


    ])
