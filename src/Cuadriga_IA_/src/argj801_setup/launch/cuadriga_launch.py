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
- Robot real: este launch usa siempre `cuadriga_actuator` como backend del actuador.
    No existe selector de backend legacy en este paquete.
- Este launch sólo referencia paquetes presentes en el workspace actual.

Este archivo *no* contiene lógica de control del rover; únicamente describe el arranque.
"""

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, EmitEvent, ExecuteProcess, LogInfo, TimerAction, GroupAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import LifecycleNode, Node
from launch_ros.parameter_descriptions import ParameterValue
from lifecycle_msgs.msg import Transition
from ament_index_python.packages import PackageNotFoundError, get_package_share_directory
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

    actuator_params = dict(select_params('cuadriga_actuator_node'))
    fixposition_params = dict(select_params('fixposition_driver_ros2'))
    serial_port = actuator_params.get('serial_port', '/dev/ttyUSB0')
    serial_baud_rate = actuator_params.get('baud_rate', 9600)

    fixposition_fp_output = fixposition_params.get('fp_output', {})
    fixposition_customer_input = fixposition_params.get('customer_input', {})
    default_fixposition_ip = str(fixposition_fp_output.get('ip', '192.168.2.113'))
    default_fixposition_port = str(fixposition_fp_output.get('port', '21000'))

    ld.add_action(
        DeclareLaunchArgument(
            'fixposition_ip',
            default_value=default_fixposition_ip,
            description='IP address of the Fixposition TCP endpoint.',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'fixposition_port',
            default_value=default_fixposition_port,
            description='TCP port of the Fixposition endpoint.',
        )
    )

    try:
        get_package_share_directory('serial_driver')
        use_serial_bridge = True
    except PackageNotFoundError:
        use_serial_bridge = False

    try:
        get_package_share_directory('fixposition_driver_ros2')
        use_fixposition_driver = True
    except PackageNotFoundError:
        use_fixposition_driver = False

    actuator_runtime_params = dict(actuator_params)
    actuator_runtime_params['output_mode'] = 'topic' if use_serial_bridge else 'serial'

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
    fixpositionDriverNode = None
    if use_fixposition_driver:
        fixpositionDriverNode = Node(
            package='fixposition_driver_ros2', executable='fixposition_driver_ros2_exec', name='fixposition_driver_ros2', output='screen',
            respawn=True, respawn_delay=5.0,
            parameters=[{
                'fp_output.formats': fixposition_fp_output.get('formats', ['ODOMETRY', 'LLH', 'RAWIMU', 'CORRIMU', 'TF']),
                'fp_output.type': ParameterValue(str(fixposition_fp_output.get('type', 'tcp')), value_type=str),
                'fp_output.ip': ParameterValue(LaunchConfiguration('fixposition_ip'), value_type=str),
                'fp_output.port': ParameterValue(LaunchConfiguration('fixposition_port'), value_type=str),
                'fp_output.rate': int(fixposition_fp_output.get('rate', 200)),
                'fp_output.reconnect_delay': float(fixposition_fp_output.get('reconnect_delay', 5.0)),
                'customer_input.speed_topic': ParameterValue(str(fixposition_customer_input.get('speed_topic', '/fixposition/speed')), value_type=str),
            }])
    cuadrigaActuatorNode = Node(
        package='cuadriga_actuator', executable='cuadriga_actuator_node', name='cuadriga_actuator_node', namespace=robot_namespace, output='screen',
        parameters=[global_params, actuator_runtime_params])
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

    serialBridgeNode = None
    serial_config = None
    serial_bridge_configure = None
    serial_bridge_activate = None
    if use_serial_bridge:
        serialBridgeNode = LifecycleNode(
            package='serial_driver', executable='serial_bridge', name='serial_bridge', namespace=robot_namespace, output='screen',
            parameters=[{
                'device_name': serial_port,
                'baud_rate': serial_baud_rate,
                'flow_control': 'none',
                'parity': 'even',
                'stop_bits': '1',
            }])

        # El stack original de Cuadriga escribía al puerto a través de serial_bridge.
        # Reproducimos esa ruta y preconfiguramos el TTY antes de configurar el bridge.
        serial_config = ExecuteProcess(
            cmd=['stty', '-F', serial_port, str(serial_baud_rate), 'cs7', 'parenb', '-parodd', '-cstopb'],
            output='screen',
        )
        serial_bridge_configure = EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(serialBridgeNode),
                transition_id=Transition.TRANSITION_CONFIGURE,
            )
        )
        serial_bridge_activate = EmitEvent(
            event=ChangeState(
                lifecycle_node_matcher=matches_action(serialBridgeNode),
                transition_id=Transition.TRANSITION_ACTIVATE,
            )
        )

    # -------------------------------------------------------------------------
    # Alta de acciones al LaunchDescription
    # -------------------------------------------------------------------------
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('simulator')), msg="Launching simulator nodes"))
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="Launching robot nodes"))
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('use_gui')), msg="Launching GUI node"))
    if use_serial_bridge:
        ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="serial_driver found: using serial_bridge transport"))
    else:
        ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="serial_driver not found: falling back to direct serial transport in cuadriga_actuator"))
    if use_fixposition_driver:
        ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="fixposition_driver_ros2 found: GPS/Odometry driver enabled"))
    else:
        ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('robot')), msg="fixposition_driver_ros2 not found: GPS/Odometry topics will be unavailable"))

    # Nodos comunes (se lanzan siempre; su activación depende de los eventos)
    for node in [controlmissionNode, controllerNode, pathfollowingNode, teleoperationNode, pathRecordNode, readyNode, followZEDNode, estopNode, backhomeNode, pathManagerNode, securityCheckNode, MPCPlannerNode, mppiSacRelayNode]:
        ld.add_action(node)

    # Nodos exclusivos de robot real. Si serial_driver está disponible usamos
    # serial_bridge; en caso contrario el actuador vuelve a la ruta serie directa.
    robot_specific_nodes = [cuadrigaActuatorNode, tf_node_sick]
    if fixpositionDriverNode is not None:
        robot_specific_nodes.insert(0, fixpositionDriverNode)
    if serialBridgeNode is not None:
        robot_specific_nodes.insert(0, serialBridgeNode)
    for node in robot_specific_nodes:
        ld.add_action(GroupAction(actions=[node], condition=IfCondition(LaunchConfiguration('robot'))))

    if serial_config is not None and serial_bridge_configure is not None and serial_bridge_activate is not None:
        ld.add_action(GroupAction(actions=[TimerAction(period=1.0, actions=[serial_config])], condition=IfCondition(LaunchConfiguration('robot'))))
        ld.add_action(GroupAction(actions=[TimerAction(period=2.0, actions=[serial_bridge_configure])], condition=IfCondition(LaunchConfiguration('robot'))))
        ld.add_action(GroupAction(actions=[TimerAction(period=3.0, actions=[serial_bridge_activate])], condition=IfCondition(LaunchConfiguration('robot'))))

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

