"""cuadriga_setup.launch.cuadriga_launch

Launch principal del sistema cuadriga.

Este fichero define el *orquestador* de arranque del stack ROS 2 del rover/simulador.
Su objetivo es:

- Cargar una configuración única (`config/cuadriga_params.yaml`) y repartirla a los nodos.
- Lanzar los nodos actualmente disponibles en este repositorio bajo un namespace configurable.
- Gestionar una secuencia de transición de lifecycle (configure/activate) de forma diferida
    para evitar carreras durante el arranque.

Notas importantes:

- Namespace: puede ajustarse con el argumento de launch `robot_namespace`.
- Lifecycle: algunos nodos se configuran siempre, pero sólo unos pocos se activan
    automáticamente en `nodes_to_activate`.
- Este launch sólo referencia paquetes presentes en el workspace actual.

Este archivo *no* contiene lógica de control del rover; únicamente describe el arranque.
"""

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, EmitEvent, LogInfo, TimerAction, GroupAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import LifecycleNode, Node
from lifecycle_msgs.msg import Transition
from ament_index_python.packages import get_package_share_directory
from launch.events import matches_action
from launch_ros.events.lifecycle import ChangeState
import yaml

def load_yaml(file_path):
    """Carga un YAML desde disco.

    Args:
        file_path: ruta absoluta al fichero YAML.

    Returns:
        Diccionario Python con el contenido del YAML.
    """
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)

def generate_launch_description():
    ld = LaunchDescription()

    # Declare launch arguments
    ld.add_action(DeclareLaunchArgument('robot', default_value='false', description='Launch robot nodes if true'))
    ld.add_action(
        DeclareLaunchArgument(
            'robot_namespace',
            default_value='cuadriga',
            description='ROS namespace used by the cuadriga stack and GUI-facing topics/services.',
        )
    )
    ld.add_action(DeclareLaunchArgument('simulator', default_value='false', description='Launch simulator nodes if true'))
    ld.add_action(DeclareLaunchArgument('use_gui', default_value='false', description='Launch GUI node if true'))
    ld.add_action(
        DeclareLaunchArgument(
            'enable_mpc',
            default_value='false',
            description='Launch MPC node if true (requires Python package cvxpy).',
        )
    )

    # Load the configuration
    config_path = os.path.join(get_package_share_directory('cuadriga_setup'), 'config', 'cuadriga_params.yaml')
    yaml_config = load_yaml(config_path)
    global_params = yaml_config['cuadriga']['global_parameters']
    robot_namespace = LaunchConfiguration('robot_namespace')

    # Function to select appropriate parameters
    def select_params(node_name):
        """Selecciona la sección de parámetros de un nodo.

        En `cuadriga_params.yaml` cada nodo tiene un bloque opcional con su nombre.
        Si no existe, se devuelve `{}` para no romper el launch.
        """
        return yaml_config['cuadriga'].get(node_name, {})

    # -------------------------------------------------------------------------
    # Definición de nodos
    # -------------------------------------------------------------------------
    # Nota general de diseño:
    # - La mayoría son LifecycleNodes (gestionan configure/activate).
    # - Se ejecutan bajo el namespace configurado para que el GUI y el resto del
    #   sistema trabajen con nombres como `/<robot_namespace>/fsm_mode`, etc.
    # - Los parámetros se componen como [global_params, params_del_nodo].
    controlmissionNode = LifecycleNode(
        package='ctl_mission', executable='ctl_mission', name='ctl_mission_node', namespace=robot_namespace, output='screen', parameters=[global_params])
    controllerNode = LifecycleNode(
        package='ctl_mission', executable='controller_node', name='controller_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('controller_node')])
    pathfollowingNode = LifecycleNode(
        package='ctl_mission', executable='path_following', name='path_following_node', namespace=robot_namespace, output='screen', parameters=[global_params, select_params('path_following_node')])
    teleoperationNode = LifecycleNode(
        package='ctl_mission', executable='teleoperation_node', name='teleoperation_node', namespace=robot_namespace, output='screen', parameters=[global_params, select_params('teleoperation_node')])
    pathRecordNode = LifecycleNode(
        package='ctl_mission', executable='path_record_node', name='path_record_node', namespace=robot_namespace, output='screen', parameters=[global_params, select_params('path_record_node')])
    readyNode = LifecycleNode(
        package='ctl_mission', executable='ready_node', name='ready_node', namespace=robot_namespace, output='screen', parameters=[global_params])
    followZEDNode = LifecycleNode(
        package='ctl_mission', executable='follow_zed_node', name='follow_zed_node', namespace=robot_namespace, output='screen', parameters=[global_params])

    mppiSacRelayNode = LifecycleNode(
        package='ctl_mission', executable='mppi_sac_relay_node', name='mppi_sac_relay_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('mppi_sac_relay_node')])

    estopNode = LifecycleNode(
        package='ctl_mission', executable='estop_node', name='estop_node', namespace=robot_namespace, output='screen', parameters=[global_params])
    backhomeNode = LifecycleNode(
        package='ctl_mission', executable='back_home_node', name='back_home_node', namespace=robot_namespace, output='screen', parameters=[global_params])
    pathManagerNode = LifecycleNode(
        package='path_manager', executable='path_manager_node', name='path_manager_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('path_manager_node')])
    securityCheckNode = LifecycleNode(
        package='security_check', executable='security_check_node', name='security_check_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('security_check_node')])
    MPCPlannerNode = LifecycleNode(
        package='ctl_mission', executable='mpc_node.py', name='mpc_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('mpc_node')],
        condition=IfCondition(LaunchConfiguration('enable_mpc')),
    )
    joystickNode = LifecycleNode(
        package='joy', executable='joy_node', name='joy_node', namespace=robot_namespace, output='screen',
        parameters=[global_params])
    cuadrigaActuatorNode = Node(
        package='cuadriga_actuator', executable='cuadriga_actuator_node', name='cuadriga_actuator_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, select_params('cuadriga_actuator_node')])
    tf_node_sick = Node(
        package='tf2_ros', executable='static_transform_publisher', name='static_transform_sick', output='screen',
        arguments=['1.86558', '0', '0.37865', '1', '0', '0', '0', yaml_config['cuadriga']['global_parameters']['robot_frame'], yaml_config['cuadriga']['global_parameters']['sick_frame']])
    gui_node = Node(
        package='cuadriga_gui', executable='gui', name='gui_node', namespace=robot_namespace, output='screen',
        parameters=[global_params])

    # -------------------------------------------------------------------------
    # Secuencia de lifecycle: configurar y activar con delays
    # -------------------------------------------------------------------------
    # Usamos dos listas de eventos:
    # - config_events: Transition.TRANSITION_CONFIGURE
    # - activate_events: Transition.TRANSITION_ACTIVATE
    # y las lanzamos con TimerAction (5s y 10s). Esto evita que algunos nodos
    # empiecen a trabajar antes de que existan sus dependencias/TF/topics.
    global config_events, activate_events
    config_events = []
    activate_events = []

    nodes_to_configure = [
        controlmissionNode, controllerNode, pathfollowingNode, backhomeNode, MPCPlannerNode,
        pathRecordNode, readyNode,followZEDNode, estopNode,
        teleoperationNode, pathManagerNode, securityCheckNode, mppiSacRelayNode
    ]

    for node in nodes_to_configure:
        config_events.append(
            EmitEvent(
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(node),
                    transition_id=Transition.TRANSITION_CONFIGURE
                )
            )
        )

    nodes_to_activate = [
        controlmissionNode, readyNode, pathManagerNode, followZEDNode,
        securityCheckNode
    ]

    for node in nodes_to_activate:
        activate_events.append(
            EmitEvent(
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(node),
                    transition_id=Transition.TRANSITION_ACTIVATE
                )
            )
        )

    # -------------------------------------------------------------------------
    # Alta de acciones al LaunchDescription
    # -------------------------------------------------------------------------
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('simulator')), msg="Launching simulator nodes"))
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="Launching robot nodes"))
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('use_gui')), msg="Launching GUI node"))

    # Nodos comunes (se lanzan siempre; su activación depende de los eventos)
    for node in [controlmissionNode, controllerNode, pathfollowingNode, teleoperationNode, pathRecordNode, readyNode, followZEDNode, estopNode, backhomeNode, pathManagerNode, securityCheckNode, MPCPlannerNode, mppiSacRelayNode]:
        ld.add_action(node)

    # Nodos exclusivos de robot real (hardware presente en este repositorio)
    robot_specific_nodes = [cuadrigaActuatorNode, tf_node_sick]
    for node in robot_specific_nodes:
        ld.add_action(GroupAction(actions=[node], condition=IfCondition(LaunchConfiguration('robot'))))

    # Nodos exclusivos de simulación
    simulator_specific_nodes = [joystickNode]
    for node in simulator_specific_nodes:
        ld.add_action(GroupAction(actions=[node], condition=IfCondition(LaunchConfiguration('simulator'))))

    # GUI actual del proyecto.
    ld.add_action(GroupAction(actions=[gui_node], condition=IfCondition(LaunchConfiguration('use_gui'))))

    # Disparo diferido de lifecycle (configure/activate)
    ld.add_action(TimerAction(period=5.0, actions=config_events))
    ld.add_action(TimerAction(period=10.0, actions=activate_events))

    return ld

