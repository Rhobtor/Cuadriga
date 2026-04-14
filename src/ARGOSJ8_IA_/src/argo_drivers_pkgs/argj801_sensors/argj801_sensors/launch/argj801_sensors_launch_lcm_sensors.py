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

"""Launch de `argj801_sensors` en modo LCM (lidars + driveline + opcionalmente cámara).

Este launch arranca dos LifecycleNodes:
- `argj801_sensors` (ARGJ801_sensors_node) que publica los tópicos de sensores.
- `argj801_slow_republisher` (si está instalado) para republcar datos de baja frecuencia.

También publica transforms estáticos (tf2_ros/static_transform_publisher) para los
frames de los lidars respecto a `robot_frame`.

Parámetros:
- Se cargan desde `config/argj801_sensors_params_lcm_sensors.yaml`.
- `configure` y `activate` controlan si se lanzan transiciones lifecycle.
"""
def load_yaml(file_path):
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)
    
_PACKAGE_NAME = 'argj801_sensors'

config_path = os.path.join(get_package_share_directory('argj801_sensors'), 'config', 'argj801_sensors_params_lcm_sensors.yaml')
yaml_config = load_yaml(config_path)
global_params = yaml_config['ARGJ801']['global_parameters']

config_path_slow = os.path.join(get_package_share_directory('argj801_slow_republisher'), 'config', 'params.yaml')
yaml_config_slow = load_yaml(config_path_slow)
    

def generate_launch_description():
    ld = LaunchDescription()

    ld.add_action(DeclareLaunchArgument('configure', default_value='true', description='Whether or not to configure the node on startup'))
    ld.add_action(DeclareLaunchArgument('activate', default_value='true', description='Whether or not to activate the node on startup'))

    # LifecycleNode: ejecutable del paquete que contiene `Argj801SensorsNode`.
    argj801_sensors = LifecycleNode(package='argj801_sensors', executable='ARGJ801_sensors_node',
                      name='ARGJ801_sensors_node_lcm_sensors', namespace='ARGJ801', output='screen', parameters=[global_params, yaml_config['ARGJ801']['argj801_sensors']], arguments=["lcm_sensors"])
    
    argj801_slow_republisher = LifecycleNode(package='argj801_slow_republisher', executable='argj801_slow_republisher_node',
                      name='ARGJ801_slow_republisher_node', namespace='ARGJ801', output='screen', parameters=[yaml_config_slow['ARGJ801']['argj801_slow_republisher']])
  

    # TFs estáticos de los sensores respecto al frame del robot.
    tf_node_velodyne = Node(package='tf2_ros', executable='static_transform_publisher', name='static_transform_publisher', output='screen',
                    arguments=['1.99348', '0', '0.27133', '1', '0', '0','0',yaml_config['ARGJ801']['global_parameters']['robot_frame'], yaml_config['ARGJ801']['global_parameters']['velodyne_frame']])
    ld.add_action(tf_node_velodyne)

    tf_node_sick = Node(package='tf2_ros', executable='static_transform_publisher', name='static_transform_publisher', output='screen',
                   arguments=['1.86558', '0', '0.37865', '1', '0', '0', '0',yaml_config['ARGJ801']['global_parameters']['robot_frame'], yaml_config['ARGJ801']['global_parameters']['sick_frame']])
    ld.add_action(tf_node_sick)

    # Transiciones lifecycle (configure/activate) con retraso.
    config_events = []
    activate_events = []

    nodes_to_configure = [argj801_sensors,argj801_slow_republisher]
    for node in nodes_to_configure:
        config_events.append(EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(node),
                transition_id=Transition.TRANSITION_CONFIGURE
            ),
            condition=LaunchConfigurationEquals('configure', 'true')
        ))

    nodes_to_activate = [argj801_sensors,argj801_slow_republisher]
    for node in nodes_to_activate:
        activate_events.append(EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(node),
                transition_id=Transition.TRANSITION_ACTIVATE
            ),
            condition=LaunchConfigurationEquals('activate', 'true')
        ))# The `
        
    # Adding nodes to launch description
    nodes_to_launch = [argj801_sensors, argj801_slow_republisher]
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