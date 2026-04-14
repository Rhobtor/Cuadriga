import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix, Imu
from std_msgs.msg import Int32, Bool, Int32MultiArray
from std_srvs.srv import Trigger, SetBool
from rclpy import qos
from lifecycle_msgs.srv import GetState
from ctl_mission_interfaces.srv import ChangeMode, ConfigStanleyCtrl, ConfigDynamicPureCtrl
from ctl_mission_interfaces.srv import GetMode, ConfigSimpleCtl, GetPossibleTransitions, ChangeController, ConfigPurePursuitCtrl, ConfigDynamicLAPureCtrl, ConfigRegulatedPureCtrl

from path_manager_interfaces.srv import ReadPathFromFile, WritePathToFile, ReturnRobotPath, PlanPath, RobotPath

from security_check_interfaces.srv import GetSecurityCheckStatus, PasswordCheck


from fixposition_driver_ros2.msg import VRTK
from tf_transformations import euler_from_quaternion
from .geonav_conversions import LLtoUTM
from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from nav_msgs.msg import Path, Odometry

class ROSclass_topics(Node):
    def __init__(self):
        super().__init__('J8_GUI_topics')
        # Declare parameters with default values
        self.declare_parameter('filtered_gps_topic_name', '/fixposition/navsatfix')
        self.declare_parameter('base_gps_topic_name', '/ARGJ801/base/gps/fix')
        self.declare_parameter('rover_gps_topic_name', '/ARGJ801/rover/gps/fix')
        self.declare_parameter('imu_head_topic_name', '/imu_head/data')
        self.declare_parameter('look_ahead_topic_name', '/ARGJ801/look_ahead_point')
        self.declare_parameter('local_path_topic_name', '/ARGJ801/local_trajectory_ll')
        self.declare_parameter('odometry_topic_name', '/fixposition/odometry_enu')
        self.declare_parameter('human_gps_topic_name', '/Human/gps/fix')
        self.declare_parameter('emergency_call_topic_name', '/Human/emergency_call')
        self.declare_parameter('fsm.get_possible_transition_topic_name', 'possible_transitions')
        self.declare_parameter('fsm.get_fsm_topic_name', 'fsm_mode')
        self.declare_parameter('local_fixed_frame', 'FP_ENU0')
        self.declare_parameter('robot_frame', 'FP_POI')
    
        namespace = 'ARGJ801'

        # Get parameters
        filtered_gps_topic_name = self.get_parameter('filtered_gps_topic_name').get_parameter_value().string_value
        base_gps_topic_name = self.get_parameter('base_gps_topic_name').get_parameter_value().string_value
        rover_gps_topic_name = self.get_parameter('rover_gps_topic_name').get_parameter_value().string_value
        imu_head_topic_name = self.get_parameter('imu_head_topic_name').get_parameter_value().string_value
        look_ahead_topic_name = self.get_parameter('look_ahead_topic_name').get_parameter_value().string_value
        local_path_topic_name = self.get_parameter('local_path_topic_name').get_parameter_value().string_value
        odometry_topic_name = self.get_parameter('odometry_topic_name').get_parameter_value().string_value
        human_gps_topic_name = self.get_parameter('human_gps_topic_name').get_parameter_value().string_value
        emergency_call_topic_name = self.get_parameter('emergency_call_topic_name').get_parameter_value().string_value
        fsm_possible_transitions = f'/{namespace}/{self.get_parameter("fsm.get_possible_transition_topic_name").get_parameter_value().string_value}'
        fsm_mode = f'/{namespace}/{self.get_parameter("fsm.get_fsm_topic_name").get_parameter_value().string_value}'
        local_fixed_frame = self.get_parameter('local_fixed_frame').get_parameter_value().string_value
        robot_frame = self.get_parameter('robot_frame').get_parameter_value().string_value




        self.rover_subscription = self.create_subscription(NavSatFix,filtered_gps_topic_name,self.filtered_gps_callback,qos.qos_profile_sensor_data)
        self.base_subscription = self.create_subscription(NavSatFix,base_gps_topic_name,self.base_gps_callback,qos.qos_profile_sensor_data)
        self.rover_subscription = self.create_subscription(NavSatFix,rover_gps_topic_name,self.rover_gps_callback,qos.qos_profile_sensor_data)
        self.human_gps_subscription = self.create_subscription(NavSatFix,human_gps_topic_name,self.human_gps_callback,qos.qos_profile_sensor_data)
        self.head_imu = self.create_subscription(Imu,imu_head_topic_name,self.imu_head_callback,qos.qos_profile_sensor_data)
        self.look_ahead= self.create_subscription(Int32,look_ahead_topic_name,self.look_ahead_callback,qos.qos_profile_sensor_data)
        self.local_path = self.create_subscription(Path,local_path_topic_name,self.local_path_callback,qos.qos_profile_sensor_data)
        self.odom_path = self.create_subscription(Odometry,odometry_topic_name,self.odometry_callback,qos.qos_profile_sensor_data)
        self.emergency_call = self.create_subscription(Bool, '/Human/emergency_call', self.emergency_call_callback,  qos.qos_profile_sensor_data)
        self.fsm_possible_modes = self.create_subscription(Int32MultiArray, fsm_possible_transitions, self.possible_transitions_callback,  qos.qos_profile_sensor_data)
        self.fsm_mode = self.create_subscription(Int32, fsm_mode, self.fsm_mode_callback,  qos.qos_profile_sensor_data)
        self.last_fsm_mode_time = self.get_clock().now()
        self.base_gps_cov = 0.0
        self.rover_gps_cov = 0.0
        self.UTM_lat = 0.0
        self.UTM_lon = 0.0
        self.UGV_UTM_x = 0.0
        self.UGV_UTM_y = 0.0
        self.UGV_yaw = 0.0
        self.rover_x = 0.0
        self.rover_y = 0.0
        self.human_x = 0.0
        self.human_y = 0.0
        self.base_x = 0.0
        self.base_y = 0.0
        self.look_ahead_idx = 0
        self.is_emergency = False
        self.local_path = Path()
        self.possible_transitions = []
        self.fsm_mode = 0
        # Subscribe to the localization status topic
        localization_status_topic_name = "/fixposition/localization_status"
        self.localization_subscription = self.create_subscription(
            VRTK,
            localization_status_topic_name,
            self.localization_status_callback,
            qos.qos_profile_sensor_data
        )
        self.localization_status = 0
         
    def fsm_mode_callback(self, msg):
        self.fsm_mode = msg.data
        self.last_fsm_mode_time = self.get_clock().now()  
        
    def get_ros_time(self):
        return self.get_clock().now()
        
    def possible_transitions_callback(self, msg):
        self.possible_transitions = msg.data.tolist()
        
    def emergency_call_callback(self, msg):
        print('call')
        self.is_emergency = True
         
         
    def localization_status_callback(self, msg):
        self.localization_status = msg.fusion_status
        
        
    def timer_tf_callback(self): 
                 try:
                    t = self.tf_buffer.lookup_transform(
                        self.local_fixed_frame,
                        self.robot_frame ,
                        rclpy.time.Time())
                 except TransformException as ex:
                    #self.get_logger().info(
                    #    f'Could not transform base_link to utm: {ex}')
                    return 
                 rpy= euler_from_quaternion([t.transform.rotation.x,  t.transform.rotation.y, 
                                            t.transform.rotation.z, t.transform.rotation.w])    
                 self.UGV_yaw = rpy[2] 
    def local_path_callback(self, msg):
        self.local_path = msg


    def odometry_callback(self, msg):
        rpy = euler_from_quaternion([msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, 
                                            msg.pose.pose.orientation.z, msg.pose.pose.orientation.w]) 
        self.UGV_yaw = rpy[2] 

    def vehicle_hearthb_callback(self, msg):
        pass

    def look_ahead_callback(self,msg):
        self.look_ahead_idx = int(msg.data / 10) #local path is subsampled by a 10 factor!!!

    def filtered_gps_callback(self,msg):
        self.UTM_lat = msg.latitude
        self.UTM_lon = msg.longitude
        pos = LLtoUTM(msg.latitude,msg.longitude)
        self.UGV_UTM_x = pos[0]
        self.UGV_UTM_y= pos[1]


    def base_gps_callback(self,msg):
        self.base_gps_cov = msg.position_covariance[0] 
        pos = LLtoUTM(msg.latitude,msg.longitude)
        self.base_x = pos[0]
        self.base_y= pos[1]

    def human_gps_callback(self,msg):
        pos = LLtoUTM(msg.latitude,msg.longitude)
        self.human_x = pos[0]
        self.human_y= pos[1]

    def rover_gps_callback(self,msg):
        self.rover_gps_cov = msg.position_covariance[0] 
        pos = LLtoUTM(msg.latitude,msg.longitude)
        self.rover_x = pos[0]
        self.rover_y= pos[1]
        
    def imu_head_callback(self,msg):
        orientation_list = [msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w]
        (roll, pitch, yaw) = euler_from_quaternion(orientation_list)
        #self.UGV_yaw = yaw 

class ROSclass_srv(Node):
    def __init__(self):
        super().__init__('J8_GUI_srv')

        # Declare service parameters with default values
        self.declare_parameter('fsm.change_fsm_mode_srv_name', 'change_fsm')
        self.declare_parameter('fsm.get_fsm_srv_name', 'get_fsm_mode')
        self.declare_parameter('fsm.get_possible_transition_srv_name', 'get_possible_transitions')
        self.declare_parameter('config_controller_srv_name', 'config_controller')
        self.declare_parameter('read_path_service', 'read_path_file')
        self.declare_parameter('return_path_service', 'get_robot_Path')
        self.declare_parameter('write_path_service', 'write_path_file')
        self.declare_parameter('enable_security_check_srv_name', 'enable_security_check')
        self.declare_parameter('get_security_check_srv_name', 'get_security_check')
        self.declare_parameter('change_controller_srv_name', 'change_controller_type')
        self.declare_parameter('config_pure_pursuit_srv_name', 'config_pure_pursuit')
        self.declare_parameter('config_stanley_srv_name', 'config_stanley')
        self.declare_parameter('config_dynamic_pure_srv_name', 'config_dynamic_pure')
        self.declare_parameter('config_dynamic_la_pure_srv_name', 'config_dynamic_la_pure')
        self.declare_parameter('config_regulated_pure_srv_name', 'config_regulated_pure')
        self.declare_parameter('path_planner_srv_name', 'path_planner')
        self.declare_parameter('receive_ll_path_srv_name', 'receive_ll_path')

        namespace = 'ARGJ801'

        # Get service names with the namespace
        change_fsm_mode_srv_name = f'/{namespace}/{self.get_parameter("fsm.change_fsm_mode_srv_name").get_parameter_value().string_value}'
        get_fsm_srv_name = f'/{namespace}/{self.get_parameter("fsm.get_fsm_srv_name").get_parameter_value().string_value}'
        get_possible_transition_srv_name = f'/{namespace}/{self.get_parameter("fsm.get_possible_transition_srv_name").get_parameter_value().string_value}'
        config_controller_srv_name = f'/{namespace}/{self.get_parameter("config_controller_srv_name").get_parameter_value().string_value}'
        read_path_service = f'/{namespace}/{self.get_parameter("read_path_service").get_parameter_value().string_value}'
        return_path_service = f'/{namespace}/{self.get_parameter("return_path_service").get_parameter_value().string_value}'
        write_path_service = f'/{namespace}/{self.get_parameter("write_path_service").get_parameter_value().string_value}'
        enable_security_check_srv_name = f'/{namespace}/{self.get_parameter("enable_security_check_srv_name").get_parameter_value().string_value}'
        get_security_check_srv_name = f'/{namespace}/{self.get_parameter("get_security_check_srv_name").get_parameter_value().string_value}'
        change_controller_srv_name = f'/{namespace}/{self.get_parameter("change_controller_srv_name").get_parameter_value().string_value}'
        config_pure_pursuit_srv_name = f'/{namespace}/{self.get_parameter("config_pure_pursuit_srv_name").get_parameter_value().string_value}'
        config_stanley_srv_name = f'/{namespace}/{self.get_parameter("config_stanley_srv_name").get_parameter_value().string_value}'
        config_dynamic_pure_srv_name = f'/{namespace}/{self.get_parameter("config_dynamic_pure_srv_name").get_parameter_value().string_value}'
        config_dynamic_la_pure_srv_name = f'/{namespace}/{self.get_parameter("config_dynamic_la_pure_srv_name").get_parameter_value().string_value}'
        config_regulated_pure_srv_name = f'/{namespace}/{self.get_parameter("config_regulated_pure_srv_name").get_parameter_value().string_value}'
        path_planner_srv_name = f'/{namespace}/{self.get_parameter("path_planner_srv_name").get_parameter_value().string_value}'
        receive_ll_path_srv_name = f'/{namespace}/{self.get_parameter("receive_ll_path_srv_name").get_parameter_value().string_value}'

    
        self.get_mission_ls_state = self.create_client(GetState, get_fsm_srv_name)
        self.get_pltform_state = self.create_client(GetState, get_fsm_srv_name)
        self.change_J8_FSM = self.create_client(ChangeMode, change_fsm_mode_srv_name)
        self.get_J8_path = self.create_client(ReturnRobotPath, return_path_service)
        self.write_path_file = self.create_client(WritePathToFile, write_path_service)
        self.read_path_file = self.create_client(ReadPathFromFile, read_path_service)
        self.config_controller = self.create_client(ConfigSimpleCtl, config_controller_srv_name)
        self.get_possible_transitions = self.create_client(GetPossibleTransitions, get_possible_transition_srv_name)
        self.toggle_security = self.create_client(SetBool, enable_security_check_srv_name)
        self.get_security_status = self.create_client(GetSecurityCheckStatus, get_security_check_srv_name)
        self.change_ctrl_type = self.create_client(ChangeController, change_controller_srv_name)
        self.config_pure_pursuit = self.create_client(ConfigPurePursuitCtrl, config_pure_pursuit_srv_name)
        self.config_stanley = self.create_client(ConfigStanleyCtrl, config_stanley_srv_name)
        self.config_dynamic_pure = self.create_client(ConfigDynamicPureCtrl, config_dynamic_pure_srv_name)
        self.config_dynamic_la_pure = self.create_client(ConfigDynamicLAPureCtrl, config_dynamic_la_pure_srv_name)
        self.config_regulated_pure = self.create_client(ConfigRegulatedPureCtrl, config_regulated_pure_srv_name)
        self.plan_path = self.create_client(PlanPath, path_planner_srv_name)
        self.send_draw_path = self.create_client(RobotPath, receive_ll_path_srv_name)

        self.resulted_Path = Path()

    def send_draw_path_srv(self, path):
        request = RobotPath.Request()
        request.path = path
        future = self.send_draw_path.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)

    def plan_path_srv(self, start, goal):
        request = PlanPath.Request()
        request.goal = goal
        request.start = start
        future = self.plan_path.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result() 


    def config_regulated_pure_srv(self, v_forward, look_ahead_dis, r_min):
        print('regulated')
        request = ConfigRegulatedPureCtrl.Request()
        request.v_forward = v_forward
        request.look_ahead_dis = look_ahead_dis
        request.r_min = r_min
        future = self.config_regulated_pure.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()   



    def config_dynamic_la_pure_srv(self, look_ahead_v_gain, max_v_forward, max_ang_acc, max_ang_dec, 
                                   max_lin_acc, max_lin_dec, speed_pow, min_lookahead_d):
        request = ConfigDynamicLAPureCtrl.Request()
        request.max_v_forward = max_v_forward
        request.max_ang_acc = max_ang_acc
        request.max_ang_dec = max_ang_dec
        request.max_lin_acc = max_lin_acc
        request.max_lin_dec = max_lin_dec
        request.look_ahead_v_gain = look_ahead_v_gain
        request.speed_pow = speed_pow
        request.min_look_ahead_d = min_lookahead_d
        print ("max_v_forward type " + str(type(max_v_forward)))
        print ("max_ang_acc type " + str(type(max_v_forward)))
        print ("max_ang_dec type " + str(type(max_ang_dec)))
        print ("max_lin_acc type " + str(type(max_lin_acc)))
        print ("max_lin_dec type " + str(type(max_lin_dec)))
        print ("look_ahead_v_gain type " + str(type(look_ahead_v_gain)))
        future = self.config_dynamic_la_pure.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  

    def config_dynamic_pure_srv(self, max_v_forward, max_ang_acc, max_ang_dec, max_lin_acc, max_lin_dec, look_ahead_dis):
        request = ConfigDynamicPureCtrl.Request()
        request.max_v_forward = max_v_forward
        request.max_ang_acc = max_ang_acc
        request.max_ang_dec = max_ang_dec
        request.max_lin_acc = max_lin_acc
        request.max_lin_dec = max_lin_dec
        request.look_ahead_dis = look_ahead_dis
        future = self.config_dynamic_pure.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  


    def config_stanley_srv(self, look_ahead, linear_v, k_error_lineal, k_error_angular):
        request = ConfigStanleyCtrl.Request()
        request.v_forward = linear_v
        request.l_ahead_dist = look_ahead
        request.k_error_angular = k_error_angular
        request.k_error_lineal = k_error_lineal
        future = self.config_stanley.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  
    
    
    def config_pure_pursuit_srv(self, look_ahead, linear_v):
        request = ConfigPurePursuitCtrl.Request()
        request.l_ahead_dist = look_ahead
        request.v_forward = linear_v
        future = self.config_pure_pursuit.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  

    def change_controller_type_srv(self, type):
        request = ChangeController.Request()
        request.controller_type = type
        future = self.change_ctrl_type.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  
            
    def get_security_status_srv(self):
        request = GetSecurityCheckStatus.Request()
        future = self.get_security_status.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        if (future.result() == None):
            return "Not running"
        else:
            return future.result().status
    
    def toggle_security_srv(self, value):
        request = SetBool.Request()
        request.data = value
        future = self.toggle_security.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()  

    def get_pos_transitions(self):
        request = GetPossibleTransitions.Request()
        future = self.get_possible_transitions.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        if (future.result() == None):
            return [-2]
        else:
            return future.result().possible_transitions
    
    def read_path_from_file(self,filename):
        print(filename)
        request = ReadPathFromFile.Request()
        request.filename = filename
        future = self.read_path_file.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()    
    
    def write_path_to_file(self,filename):
        print(filename)
        print(type(filename))
        request = WritePathToFile.Request()
        print(type(request.filename))
        request.filename = filename
        print(type(request.filename))
        future = self.write_path_file.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()
    
    def get_J8_FSM_mode(self):
        request = GetMode.Request()
        future = self.get_J8_FSM.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        if (future.result() == None):
#          print('is notetype')
          return -1
        else: 
          return future.result().mode
        
    
    def get_J8_path_request(self):
        request = ReturnRobotPath.Request()
        future = self.get_J8_path.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()
        
    def send_FSM_change_request(self,transition):
        request = ChangeMode.Request()
        request.transition = transition
        future = self.change_J8_FSM.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)

        #return future.result()
    
    def send_lc_state_request(self):
        request = GetState.Request()
        future = self.get_mission_ls_state.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        if (future.result() == None):
#          print('is notetype')
          return "Not running"
        else: 
          return future.result().current_state.label

    
    def send_pltform_lc_state_request(self):
        request = GetState.Request()
        future = self.get_pltform_state.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        #print(type(future.result().current_state.label))
        if (future.result() == None):
#          print('is notetype')
          return "Not running"
        else: 
          return future.result().current_state.label

    def change_ctl_config(self, k_ang, k_position, n_points_window, l_ahead_dist, v_forward):
        request = ConfigSimpleCtl.Request()
        request.k_angle = k_ang
        request.k_position = k_position
        request.points_in_window = n_points_window
        request.l_ahead_dist = l_ahead_dist
        request.v_forward = v_forward
        future = self.config_controller.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=1.0)
        return future.result()
