"""argj801_setup.launch.J8_launch

Launch principal del sistema ARGJ801.

Este fichero define el *orquestador* de arranque del stack ROS 2 del rover/simulador.
Su objetivo es:

- Cargar una configuración única (`config/J8_params.yaml`) y repartirla a los nodos.
- Lanzar un conjunto de nodos (muchos de ellos *LifecycleNodes*) bajo el namespace `ARGJ801`.
- Gestionar una secuencia de transición de lifecycle (configure/activate) de forma diferida
    para evitar carreras durante el arranque.

Notas importantes:

- Namespace: se usa `ARGJ801` de forma fija en este launch. Si en el futuro se quiere
    soportar múltiples rovers, lo ideal es convertirlo en argumento de launch.
- Lifecycle: algunos nodos se configuran siempre, pero sólo unos pocos se activan
    automáticamente en `nodes_to_activate`.
- Modo operación plataforma: el nodo `argj801_ctrl_platform_node` se crea mediante
    `OpaqueFunction` para poder calcular `operation_mode` en runtime según el argumento
    `robot` (robot real vs simulación).

Este archivo *no* contiene lógica de control del rover; únicamente describe el arranque.
"""

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, EmitEvent, LogInfo, TimerAction, GroupAction, OpaqueFunction
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
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

    # Load the configuration
    config_path = os.path.join(get_package_share_directory('argj801_setup'), 'config', 'J8_params.yaml')
    yaml_config = load_yaml(config_path)
    global_params = yaml_config['ARGJ801']['global_parameters']

    # Declare launch arguments
    ld.add_action(DeclareLaunchArgument('robot', default_value='false', description='Launch robot nodes if true'))
    ld.add_action(
        DeclareLaunchArgument(
            'fixposition_ip',
            default_value=yaml_config['ARGJ801']['fixposition_driver_ros2']['fp_output']['ip'],
            description='IP address of the Fixposition TCP endpoint.',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'fixposition_port',
            default_value=str(yaml_config['ARGJ801']['fixposition_driver_ros2']['fp_output']['port']),
            description='TCP port of the Fixposition endpoint.',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'local_test',
            default_value='false',
            description='Launch a PC-only test setup without robot hardware, but keeping FSM/GUI/Cuadriga actuator available.',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'platform_mode',
            default_value='',
            description=(
                'Override argj801_ctrl_platform_node operation_mode. '
                'Empty = auto (robot:=true -> 1 LCM, else 2 Gazebo). '
                'Values: 0 Arduino, 1 LCM, 2 Gazebo.'
            ),
        )
    )
    ld.add_action(DeclareLaunchArgument('simulator', default_value='false', description='Launch simulator nodes if true'))
    ld.add_action(DeclareLaunchArgument('use_gui', default_value='false', description='Launch GUI node if true'))
    ld.add_action(
        DeclareLaunchArgument(
            'gui_variant',
            default_value='j8_gui',
            description="GUI implementation to launch: 'j8_gui' or 'legacy'.",
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'sensors_source',
            default_value='lcm_sensors',
            description='argj801_sensors source argument (e.g. lcm_sensors, camera).',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'enable_mpc',
            default_value='false',
            description='Launch MPC node if true (requires Python package cvxpy).',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'require_vehicle_heartbeat',
            default_value='true',
            description=(
                'LCM safety: require dat_vehicle_heartbeat_msg to keep the platform node active. '
                'Set to false ONLY for debugging/bringup when vehicle heartbeat is not available.'
            ),
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'use_cuadriga_backend',
            default_value='true',
            description='Use the Cuadriga serial backend instead of argj801_ctl_platform.',
        )
    )
    ld.add_action(
        DeclareLaunchArgument(
            'use_cuadriga_mock',
            default_value='true',
            description='In local_test mode, launch a fake Cuadriga backend that publishes simulated telemetry.',
        )
    )

    robot_condition = IfCondition(LaunchConfiguration('robot'))
    simulator_condition = IfCondition(LaunchConfiguration('simulator'))
    use_gui_condition = IfCondition(LaunchConfiguration('use_gui'))
    robot_or_local_test_condition = IfCondition(
        PythonExpression([
            "'", LaunchConfiguration('robot'), "' == 'true' or '", LaunchConfiguration('local_test'), "' == 'true'",
        ])
    )
    simulator_or_local_test_condition = IfCondition(
        PythonExpression([
            "'", LaunchConfiguration('simulator'), "' == 'true' or '", LaunchConfiguration('local_test'), "' == 'true'",
        ])
    )
    joystick_condition = IfCondition(
        PythonExpression([
            "'", LaunchConfiguration('robot'), "' == 'true' or '", LaunchConfiguration('simulator'), "' == 'true' or '",
            LaunchConfiguration('local_test'), "' == 'true'",
        ])
    )
    cuadriga_backend_robot_condition = IfCondition(
        PythonExpression([
            "'", LaunchConfiguration('robot'), "' == 'true' and '", LaunchConfiguration('use_cuadriga_backend'), "' == 'true'",
        ])
    )
    cuadriga_backend_local_or_robot_condition = IfCondition(
        PythonExpression([
            "('", LaunchConfiguration('robot'), "' == 'true' or '", LaunchConfiguration('local_test'), "' == 'true') and '",
            LaunchConfiguration('use_cuadriga_backend'),
            "' == 'true'",
        ])
    )
    cuadriga_mock_local_test_condition = IfCondition(
        PythonExpression([
            "'", LaunchConfiguration('local_test'), "' == 'true' and '",
            LaunchConfiguration('use_cuadriga_backend'),
            "' == 'true' and '",
            LaunchConfiguration('use_cuadriga_mock'),
            "' == 'true'",
        ])
    )

    # Function to select appropriate parameters
    def select_params(node_name):
        """Selecciona la sección de parámetros de un nodo.

        En `J8_params.yaml` cada nodo tiene un bloque opcional con su nombre.
        Si no existe, se devuelve `{}` para no romper el launch.
        """
        return yaml_config['ARGJ801'].get(node_name, {})

    # Function to set operation_mode parameter
    def get_ctrl_plataform_node(context):
        """Crea el LifecycleNode de plataforma calculando el modo de operación.

        - operation_mode = 1: robot real (argumento `robot:=true`)
        - operation_mode = 2: simulación / entorno no-robot

        Se usa OpaqueFunction para poder leer el LaunchConfiguration ya resuelto.
        """
        robot = LaunchConfiguration('robot').perform(context)
        platform_mode = LaunchConfiguration('platform_mode').perform(context).strip()
        require_vehicle_heartbeat_str = LaunchConfiguration('require_vehicle_heartbeat').perform(context).strip().lower()
        require_vehicle_heartbeat = require_vehicle_heartbeat_str in ('true', '1', 'yes', 'y')
        if platform_mode:
            operation_mode = int(platform_mode)
        else:
            operation_mode = 1 if robot == 'true' else 2
        ctrlPlataformNode = LifecycleNode(
            package='argj801_ctl_platform', executable='ARGJ801_ctl_platform', name='argj801_ctrl_platform_node', namespace='ARGJ801', output='screen',
            parameters=[
                global_params,
                select_params('argj801_ctrl_platform_node'),
                {
                    'operation_mode': operation_mode,
                    'lcm_params.require_vehicle_heartbeat': require_vehicle_heartbeat,
                },
            ]
        )

        # Añadimos también su configuración/activación a las listas globales.
        # Nota: estas acciones se ejecutan más tarde vía TimerAction.
        config_events.append(
            EmitEvent(
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(ctrlPlataformNode),
                    transition_id=Transition.TRANSITION_CONFIGURE
                )
            )
        )
        activate_events.append(
            EmitEvent(
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(ctrlPlataformNode),
                    transition_id=Transition.TRANSITION_ACTIVATE
                )
            )
        )
        return [ctrlPlataformNode]

    # -------------------------------------------------------------------------
    # Definición de nodos
    # -------------------------------------------------------------------------
    # Nota general de diseño:
    # - La mayoría son LifecycleNodes (gestionan configure/activate).
    # - Se ejecutan bajo el namespace `ARGJ801` para que el GUI y el resto del
    #   sistema trabajen con nombres como `/ARGJ801/fsm_mode`, etc.
    # - Los parámetros se componen como [global_params, params_del_nodo].
    controlmissionNode = LifecycleNode(
        package='ctl_mission', executable='ctl_mission', name='ctl_mission_node', namespace='ARGJ801', output='screen', parameters=[global_params])
    controllerNode = LifecycleNode(
        package='ctl_mission', executable='controller_node', name='controller_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('controller_node')])
    pathfollowingNode = LifecycleNode(
        package='ctl_mission', executable='path_following', name='path_following_node', namespace='ARGJ801', output='screen', parameters=[global_params, select_params('path_following_node')])
    teleoperationNode = LifecycleNode(
        package='ctl_mission', executable='teleoperation_node', name='teleoperation_node', namespace='ARGJ801', output='screen', parameters=[global_params, select_params('teleoperation_node')])
    pathRecordNode = LifecycleNode(
        package='ctl_mission', executable='path_record_node', name='path_record_node', namespace='ARGJ801', output='screen', parameters=[global_params, select_params('path_record_node')])
    readyNode = LifecycleNode(
        package='ctl_mission', executable='ready_node', name='ready_node', namespace='ARGJ801', output='screen', parameters=[global_params])
    followZEDNode = LifecycleNode(
        package='ctl_mission', executable='follow_zed_node', name='follow_zed_node', namespace='ARGJ801', output='screen', parameters=[global_params],
        condition=robot_condition)

    mppiSacRelayNode = LifecycleNode(
        package='ctl_mission', executable='mppi_sac_relay_node', name='mppi_sac_relay_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('mppi_sac_relay_node')])
    
    estopNode = LifecycleNode(
        package='ctl_mission', executable='estop_node', name='estop_node', namespace='ARGJ801', output='screen', parameters=[global_params])
    backhomeNode = LifecycleNode(
        package='ctl_mission', executable='back_home_node', name='back_home_node', namespace='ARGJ801', output='screen', parameters=[global_params])
    pathManagerNode = LifecycleNode(
        package='path_manager', executable='path_manager_node', name='path_manager_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('path_manager_node')])
    MPCPlannerNode = LifecycleNode(
        package='ctl_mission', executable='mpc_node.py', name='mpc_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('mpc_node')],
        condition=IfCondition(LaunchConfiguration('enable_mpc')),
    )
    joystickNode = LifecycleNode(
        package='joy', executable='joy_node', name='joy_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('argj801_ctrl_platform_node')],
        condition=joystick_condition)
    fixpositionDriverNode = Node(
        package='fixposition_driver_ros2', executable='fixposition_driver_ros2_exec', name='fixposition_driver_ros2', output='screen',
        parameters=[
            global_params,
            select_params('fixposition_driver_ros2'),
            {
                'fp_output.ip': LaunchConfiguration('fixposition_ip'),
                'fp_output.port': LaunchConfiguration('fixposition_port'),
            },
        ])
    argj801_sensors = LifecycleNode(
        package='argj801_sensors', executable='ARGJ801_sensors_node', name='ARGJ801_sensors_node', namespace='ARGJ801', output='screen',
        parameters=[global_params, select_params('argj801_sensors')],
        arguments=[LaunchConfiguration('sensors_source')],
    )
    android_server_node = LifecycleNode(
        package='android_ros2_server', executable='tcp_server_node', name='android_server', namespace='ARGJ801', output='log',
        parameters=[global_params, select_params('android_server_node')],
        condition=robot_condition)
    tf_node_velodyne = Node(
        package='tf2_ros', executable='static_transform_publisher', name='static_transform_velodyne', output='screen',
        arguments=['1.99348', '0', '0.27133', '1', '0', '0', '0', yaml_config['ARGJ801']['global_parameters']['robot_frame'], yaml_config['ARGJ801']['global_parameters']['velodyne_frame']])
    laser_segmentation_node = LifecycleNode(
        package='laser_segmentation', namespace='ARGJ801', executable='laser_segmentation', name='laser_segmentation',
        parameters=[global_params, select_params('laser_segmentation')])
    tf_node_sick = Node(
        package='tf2_ros', executable='static_transform_publisher', name='static_transform_sick', output='screen',
        arguments=['1.86558', '0', '0.37865', '1', '0', '0', '0', yaml_config['ARGJ801']['global_parameters']['robot_frame'], yaml_config['ARGJ801']['global_parameters']['sick_frame']])
    gui_node = Node(
        package='j8_gui', executable='gui', name='gui_node', namespace='ARGJ801', output='screen',
        parameters=[global_params],
        condition=IfCondition(
            PythonExpression([
                "'", LaunchConfiguration('use_gui'), "' == 'true' and '", LaunchConfiguration('gui_variant'), "' == 'j8_gui'"
            ])
        ))
    legacy_gui_node = Node(
        package='GUI_pkg', executable='gui_node', name='legacy_gui_node', namespace='ARGJ801', output='screen',
        parameters=[global_params],
        condition=IfCondition(
            PythonExpression([
                "'", LaunchConfiguration('use_gui'), "' == 'true' and '", LaunchConfiguration('gui_variant'), "' == 'legacy'"
            ])
        ))
    cuadrigaActuatorNode = Node(
        package='cuadriga_actuator', executable='cuadriga_actuator_node', name='cuadriga_actuator_node', output='screen',
        parameters=[
            select_params('cuadriga_actuator_node'),
            {
                'output_mode': PythonExpression([
                    "'serial' if '", LaunchConfiguration('robot'), "' == 'true' else 'topic'"
                ])
            },
        ],
        condition=cuadriga_backend_local_or_robot_condition)
    cuadrigaMockBackendNode = Node(
        package='argj801_setup', executable='cuadriga_mock_backend.py', name='cuadriga_mock_backend', output='screen',
        condition=cuadriga_mock_local_test_condition)

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
        (controlmissionNode, None),
        (controllerNode, None),
        (pathfollowingNode, None),
        (backhomeNode, None),
        (MPCPlannerNode, None),
        (android_server_node, robot_condition),
        (laser_segmentation_node, None),
        (pathRecordNode, None),
        (readyNode, None),
        (followZEDNode, robot_condition),
        (estopNode, None),
        (teleoperationNode, None),
        (pathManagerNode, None),
        (mppiSacRelayNode, None),
    ]

    for node, condition in nodes_to_configure:
        config_events.append(
            EmitEvent(
                condition=condition,
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(node),
                    transition_id=Transition.TRANSITION_CONFIGURE
                )
            )
        )

    nodes_to_activate = [
        (controlmissionNode, None),
        (readyNode, None),
        (pathManagerNode, None),
        (followZEDNode, robot_condition),
        (android_server_node, robot_condition),
        (laser_segmentation_node, None),
    ]

    for node, condition in nodes_to_activate:
        activate_events.append(
            EmitEvent(
                condition=condition,
                event=ChangeState(
                    lifecycle_node_matcher=matches_action(node),
                    transition_id=Transition.TRANSITION_ACTIVATE
                )
            )
        )

    # -------------------------------------------------------------------------
    # Alta de acciones al LaunchDescription
    # -------------------------------------------------------------------------
    ld.add_action(LogInfo(condition=simulator_condition, msg="Launching simulator nodes"))
    ld.add_action(LogInfo(condition=robot_condition, msg="Launching robot nodes"))
    ld.add_action(LogInfo(condition=IfCondition(LaunchConfiguration('local_test')), msg="Launching PC-only local test mode"))
    ld.add_action(LogInfo(condition=use_gui_condition, msg="Launching GUI node"))

    # Nodos comunes (se lanzan siempre; su activación depende de los eventos)
    for node in [controlmissionNode, controllerNode, pathfollowingNode, teleoperationNode, pathRecordNode, readyNode, followZEDNode, estopNode, backhomeNode, pathManagerNode, MPCPlannerNode, android_server_node, laser_segmentation_node, mppiSacRelayNode]:
        ld.add_action(node)

    # Nodos exclusivos de robot real (sensores/TF/hardware)
    robot_specific_nodes = [
        fixpositionDriverNode,
        tf_node_velodyne,
        tf_node_sick,
        argj801_sensors,
    ]
    for node in robot_specific_nodes:
        ld.add_action(GroupAction(actions=[node], condition=robot_condition))

    ld.add_action(cuadrigaActuatorNode)
    ld.add_action(cuadrigaMockBackendNode)

    # Nodos exclusivos de simulación
    simulator_specific_nodes = [joystickNode]
    for node in simulator_specific_nodes:
        ld.add_action(node)

    # GUI. `j8_gui` es la implementación activa; `GUI_pkg` queda como legacy.
    ld.add_action(gui_node)
    ld.add_action(legacy_gui_node)

    # Disparo diferido de lifecycle (configure/activate)
    ld.add_action(TimerAction(period=5.0, actions=config_events))
    ld.add_action(TimerAction(period=10.0, actions=activate_events))

    # Añade el nodo de plataforma con operation_mode calculado según `robot`.
    ld.add_action(
        GroupAction(
            actions=[OpaqueFunction(function=get_ctrl_plataform_node)],
            condition=UnlessCondition(LaunchConfiguration('use_cuadriga_backend')),
        )
    )

    return ld

