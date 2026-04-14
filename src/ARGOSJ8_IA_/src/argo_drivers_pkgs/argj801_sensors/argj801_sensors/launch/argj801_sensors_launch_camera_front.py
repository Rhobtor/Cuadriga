import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, EmitEvent
from launch.conditions import LaunchConfigurationEquals
from launch_ros.actions import LifecycleNode
from lifecycle_msgs.msg import Transition
from ament_index_python.packages import get_package_share_directory
from launch.events import matches_action
from launch_ros.events.lifecycle import ChangeState
from launch.actions import TimerAction
import yaml
def load_yaml(file_path):
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)
    
_PACKAGE_NAME = 'argj801_sensors'

config_path = os.path.join(get_package_share_directory('argj801_sensors'), 'config', 'argj801_sensors_params_camera_front.yaml')
yaml_config = load_yaml(config_path)
global_params = yaml_config['ARGJ801']['global_parameters']


    

def generate_launch_description():
    ld = LaunchDescription()

    ld.add_action(DeclareLaunchArgument('configure', default_value='true', description='Whether or not to configure the node on startup'))
    ld.add_action(DeclareLaunchArgument('activate', default_value='true', description='Whether or not to activate the node on startup'))

    argj801_sensors = LifecycleNode(package='argj801_sensors', executable='ARGJ801_sensors_node',
                      name='ARGJ801_sensors_node_camera_front', namespace='ARGJ801', output='screen', parameters=[global_params, yaml_config['ARGJ801']['argj801_sensors']], arguments=["camera"])
    

    # Configuration and activation events
    config_events = []
    activate_events = []

    nodes_to_configure = [argj801_sensors]
    for node in nodes_to_configure:
        config_events.append(EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(node),
                transition_id=Transition.TRANSITION_CONFIGURE
            ),
            condition=LaunchConfigurationEquals('configure', 'true')
        ))

    nodes_to_activate = [argj801_sensors]
    for node in nodes_to_activate:
        activate_events.append(EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(node),
                transition_id=Transition.TRANSITION_ACTIVATE
            ),
            condition=LaunchConfigurationEquals('activate', 'true')
        ))# The `
        
    # Adding nodes to launch description
    nodes_to_launch = [argj801_sensors]
    for node in nodes_to_launch:
        ld.add_action(node)
        
    delay_time = 5.0  # Seconds
    ld.add_action(TimerAction(
        period=delay_time,
        actions=config_events
    ))

    # Introduce a delay between configuration and activation
    delay_time = 5.0  # Seconds
    ld.add_action(TimerAction(
        period=delay_time,
        actions=activate_events
    ))

    return ld