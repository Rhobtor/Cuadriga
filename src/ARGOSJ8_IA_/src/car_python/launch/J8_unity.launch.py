#!/usr/bin/env python3
# robot_with_frames.launch.py

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
import os
import xacro

def generate_launch_description():
    # ========== AJUSTA ESTO A TU PAQUETE/RUTA ==========
    # Paquete que contiene el xacro/urdf y su ruta
    desc_pkg  = 'j8_xacro_model'         # <- cámbialo si tu URDF está en otro paquete
    xacro_path = os.path.join(
        get_package_share_directory(desc_pkg),
        'urdf',
        'argo_j8.xacro'                 # <- cámbialo si el nombre difiere
    )

    # Procesar el xacro → robot_description (XML)
    robot_description_config = xacro.process_file(xacro_path)
    robot_urdf = robot_description_config.toxml()

    # ======= Parámetro opcional para el alias Velodyne_link -> lidar_link =======
    use_lidar_alias_arg = DeclareLaunchArgument(
        'use_lidar_alias',
        default_value='true',
        description='Crear alias TF Velodyne_link -> lidar_link (identidad)'
    )
    use_lidar_alias = LaunchConfiguration('use_lidar_alias')

    # ======= Compatibilidad con frame "mmap" (si RViz lo usa como Fixed Frame) =======
    publish_mmap_alias_arg = DeclareLaunchArgument(
        'publish_mmap_alias',
        default_value='true',
        description='Publicar TF estático identidad mmap -> map para conectar ambos árboles'
    )
    publish_mmap_alias = LaunchConfiguration('publish_mmap_alias')

    # ======= TF odom -> base_link (solo si NO tienes odometría publicándolo ya) =======
    publish_static_odom_to_base_arg = DeclareLaunchArgument(
        'publish_static_odom_to_base',
        default_value='true',
        description='Publicar TF estático identidad odom -> base_link (desactívalo si ya existe odom->base_link dinámico)'
    )
    publish_static_odom_to_base = LaunchConfiguration('publish_static_odom_to_base')

    # ========== NODOS ==========
    # Robot State Publisher (publica TF del URDF)
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_urdf}]
    )

    # Joint State Publisher (si no tienes otro publicador de joints, fija a 0)
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        output='screen',
        parameters=[{'use_gui': False}]
    )

    # map -> odom (identidad provisional)
    map_odom_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='static_map_to_odom',
        arguments=[
            '--frame-id', 'map',
            '--child-frame-id', 'odom',
            '--x', '0', '--y', '0', '--z', '0',
            '--roll', '0', '--pitch', '0', '--yaw', '0',
        ],
        output='screen'
    )


    # odom -> base_link (identidad provisional hasta tener odometría)
    odom_base_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='static_odom_to_base',
        arguments=[
            '--frame-id', 'odom',
            '--child-frame-id', 'base_link',
            '--x', '0', '--y', '0', '--z', '0',
            '--roll', '0', '--pitch', '0', '--yaw', '0',
        ],
        output='screen',
        condition=IfCondition(publish_static_odom_to_base)
    )

    # base_link -> camera_link (posición tomada del URDF que me pasaste; RPY=0 para mapeo "frame base")
    base_camera_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='static_base_to_camera',
        arguments=[
            '--frame-id', 'base_link',
            '--child-frame-id', 'camera_link',
            '--x', '0.9206628', '--y', '0.0075201', '--z', '0.75542',
            '--roll', '0', '--pitch', '0', '--yaw', '0',
        ],
        output='screen'
    )

    velodyne_noise_filter = Node(
        package='car_cpp',
        executable='velodyne_noise_filter_node',
        name='velodyne_noise_filter',
        output='screen',
        parameters=[{
            'input': '/ARGJ801/Velodyne/scan_cloud',
            'output': '/ARGJ801/Velodyne/scan_cloud_filtered',
            'leaf_size': 0.15,
            'support_leaf_size': 0.35,
            'support_radius_voxels': 1,
            'min_support_points': 3,
            'min_range': 1.0,
            'max_range': 60.0,
            'min_z': -3.0,
            'max_z': 3.0,
            'reject_origin_enabled': True,
            'reject_origin_epsilon': 0.05,
            'exclude_box_enabled': True,
            'exclude_box_frame': 'base_link',
            'exclude_box_center_x': 0.35,
            'exclude_box_center_y': 0.0,
            'exclude_box_center_z': 0.35,
            'exclude_box_size_x': 3.10,
            'exclude_box_size_y': 1.80,
            'exclude_box_size_z': 1.40,
            'hood_box_enabled': True,
            'hood_box_center_x': 1.00,
            'hood_box_center_y': 0.0,
            'hood_box_center_z': 0.20,
            'hood_box_size_x': 1.40,
            'hood_box_size_y': 1.20,
            'hood_box_size_z': 1.10,
            'debug_log': True,
        }]
    )

    # (Opcional) alias Velodyne_link -> lidar_link (identidad)
    velodyne_to_lidar_alias = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='velodyne_to_lidar_alias',
        arguments=[
            '--frame-id', 'Velodyne_link',
            '--child-frame-id', 'lidar_link',
            '--x', '0', '--y', '0', '--z', '0',
            '--roll', '0', '--pitch', '0', '--yaw', '0',
        ],
        output='screen',
        condition=IfCondition(use_lidar_alias)
    )

    return LaunchDescription([
        use_lidar_alias_arg,
        publish_mmap_alias_arg,
        publish_static_odom_to_base_arg,
        robot_state_publisher_node,
        joint_state_publisher_node,
        map_odom_tf,
        odom_base_tf,
        base_camera_tf,
        velodyne_noise_filter,
        velodyne_to_lidar_alias,
    ])
