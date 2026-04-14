from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition  # Add this import
from ament_index_python.packages import get_package_share_directory
import xacro
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('j8_xacro_model')
    xacro_file = os.path.join(pkg_share, 'urdf', 'argo_j8.xacro')
    #xacro_file = os.path.join(pkg_share, 'urdf', 'test.urdf')
    world_file = os.path.join(pkg_share, 'worlds', 'empty_custom.world')

    # Process the xacro file to generate URDF
    robot_description_config = xacro.process_file(xacro_file).toxml()

    # Define initial pose (x, y, z, roll, pitch, yaw)
    initial_pose = ['-x', '7.18', '-y', '6.09', '-z', '0.55', '-R', '0.0', '-P', '0.0', '-Y', '2.22']  # Example pose
    #initial_pose = ['-x', '139.21', '-y', '-129.05', '-z', '0.55', '-R', '0.0', '-P', '0.0', '-Y', '-1']  # Example pose

    # Declare the GUI argument
    gui_arg = DeclareLaunchArgument(
        'gui',
        default_value='true',
        description='Set to "false" to run headless.'
    )

    # Get the value of the GUI argument
    gui = LaunchConfiguration('gui')

    # Determine the command to run based on the GUI argument
    gzserver_cmd = ['gzserver', '--verbose', world_file, '-s', 'libgazebo_ros_factory.so']
    gazebo_cmd = ['gazebo', '--verbose', world_file, '-s', 'libgazebo_ros_factory.so']

    return LaunchDescription([
        gui_arg,

        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description_config}],
            output='screen'),

        ExecuteProcess(
            condition=IfCondition(gui),
            cmd=gazebo_cmd,
            output='screen'),

        ExecuteProcess(
            condition=UnlessCondition(gui),
            cmd=gzserver_cmd,
            output='screen'),

        Node(
            package='gazebo_ros', 
            executable='spawn_entity.py',
            arguments=['-entity', 'my_robot', '-topic', '/robot_description', *initial_pose],
            output='screen')
    ])

