import rclpy
from rclpy.lifecycle import LifecycleNode
from rclpy.lifecycle import State
from rclpy.lifecycle import TransitionCallbackReturn
from rclpy import qos
from sensor_msgs.msg import NavSatFix, NavSatStatus
from std_msgs.msg import Bool
import socket
import threading
import select
from ctl_mission_interfaces.srv import ChangeMode, GetMode  # Import PlanPath service
from path_manager_interfaces.srv import PlanPath
from geometry_msgs.msg import Point
import utm  # Import UTM conversion library

# Server configuration
HOST = '0.0.0.0'  # Listen on all network interfaces
PORT = 5001

class TcpServerLifecycleNode(LifecycleNode):
    def __init__(self):
        super().__init__('tcp_server_node')
        # Declare parameters
        self.declare_parameter('host', '0.0.0.0')
        self.declare_parameter('port', 5001)
        self.declare_parameter('emergency_topic', '/Human/emergency_call')
        self.declare_parameter('human_gps_fix_topic', '/Human/gps/fix')
        self.declare_parameter('gps_topic', '/fixposition/navsatfix')
        self.declare_parameter('fsm.change_fsm_mode_srv_name', 'change_fsm_mode')
        self.declare_parameter('fsm.get_fsm_srv_name', 'get_fsm_mode')
        self.declare_parameter('plan_path_service', 'path_planner')

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # Set SO_REUSEADDR option
        self.server_socket.bind((HOST, PORT))
        self.server_socket.listen(5)
        self.server_socket.setblocking(False)  # Set the server socket to non-blocking

        self.change_J8_FSM = self.create_client(ChangeMode, 'change_fsm_mode')
        self.get_J8_FSM = self.create_client(GetMode, 'get_fsm_mode')
        self.plan_path_client = self.create_client(PlanPath, 'path_planner')  # PlanPath service client
        self.navsatfix_publisher = self.create_publisher(NavSatFix, '/Human/gps/fix', 10)
        self.emergency_call = self.create_publisher(Bool, '/Human/emergency_call', qos.qos_profile_sensor_data)
        self.robot_gps_subscription = self.create_subscription(NavSatFix, '/fixposition/navsatfix',
                                                               self.robot_gps_callback, qos.qos_profile_sensor_data)
        self.robot_latitude = None
        self.robot_longitude = None
        self.J8_state = 0
        self.latitude = 0.0
        self.longitude = 0.0
        self.get_logger().info(f'Server listening on {HOST}:{PORT}')
        self.planning_ended = False

        self.client_threads = {}
        self.timer_ = None  # Initialize the timer attribute
        self.lock = threading.Lock()

    def on_configure(self, state: State):
        self.get_logger().info('Configuring...')

        # Retrieve parameters
        host = self.get_parameter('host').get_parameter_value().string_value
        port = self.get_parameter('port').get_parameter_value().integer_value

        # Set up server socket
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((host, port))
        self.server_socket.listen(5)
        self.server_socket.setblocking(False)

        # Retrieve topic and service names from parameters
        emergency_topic = self.get_parameter('emergency_topic').get_parameter_value().string_value
        gps_fix_topic = self.get_parameter('human_gps_fix_topic').get_parameter_value().string_value
        navsatfix_subscription_topic = self.get_parameter('gps_topic').get_parameter_value().string_value
        change_mode_service = self.get_parameter('fsm.change_fsm_mode_srv_name').get_parameter_value().string_value
        get_mode_service = self.get_parameter('fsm.get_fsm_srv_name').get_parameter_value().string_value
        plan_path_service = self.get_parameter('plan_path_service').get_parameter_value().string_value

        # Set up clients, publishers, and subscriptions
        self.change_J8_FSM = self.create_client(ChangeMode, change_mode_service)
        self.get_J8_FSM = self.create_client(GetMode, get_mode_service)
        self.plan_path_client = self.create_client(PlanPath, plan_path_service)
        self.navsatfix_publisher = self.create_publisher(NavSatFix, gps_fix_topic, 10)
        self.emergency_call = self.create_publisher(Bool, emergency_topic, qos.qos_profile_sensor_data)
        self.robot_gps_subscription = self.create_subscription(NavSatFix, navsatfix_subscription_topic,
                                                               self.robot_gps_callback, qos.qos_profile_sensor_data)
        return TransitionCallbackReturn.SUCCESS

    def on_activate(self, state: State):
        self.get_logger().info('Activating...')
        self.timer_ = self.create_timer(1.0, self.timer_callback)  # Create a timer with a 1-second interval
        self.create_thread()
        return TransitionCallbackReturn.SUCCESS

    def on_deactivate(self, state: State):
        self.get_logger().info('Deactivating...')
        if self.timer_ is not None:
            self.timer_.cancel()
        # Close all client connections
        with self.lock:
            for client_socket, thread in self.client_threads.values():
                if thread.is_alive():
                    thread.join()
                client_socket.close()
            self.client_threads.clear()

        self.server_socket.close()
        return TransitionCallbackReturn.SUCCESS

    def on_cleanup(self, state: State):
        self.get_logger().info('Cleaning up...')
        self.server_socket.close()
        return TransitionCallbackReturn.SUCCESS

    def on_shutdown(self, state: State):
        self.get_logger().info('Shutting down...')
        self.server_socket.close()
        return TransitionCallbackReturn.SUCCESS

    def timer_callback(self):
        try:
            self.get_J8_FSM_mode()
        except Exception as e:
            self.get_logger().error(f'Error in timer_callback: {e}')

    def send_FSM_change_request(self, transition):
        request = ChangeMode.Request()
        request.transition = transition
        future = self.change_J8_FSM.call_async(request)
        future.add_done_callback(self.FSM_change_request_callback)

    def robot_gps_callback(self, msg):
        self.robot_longitude = msg.longitude
        self.robot_latitude = msg.latitude

    def FSM_change_request_callback(self, future):
        try:
            response = future.result()
            if response is None:
                self.get_logger().warn('Failed to change FSM mode')
            else:
                self.get_logger().info(f'FSM mode changed to: {response.mode}')
        except Exception as e:
            self.get_logger().error(f'Exception in FSM change request: {e}')

    def get_J8_FSM_mode(self):
        request = GetMode.Request()
        future = self.get_J8_FSM.call_async(request)
        future.add_done_callback(self.get_J8_FSM_mode_callback)

    def get_J8_FSM_mode_callback(self, future):
        try:
            response = future.result()
            if response is None:
                self.get_logger().warn('Failed to get FSM mode')
                self.J8_state = -1
            else:
                self.J8_state = int(response.mode)
        except Exception as e:
            self.get_logger().error(f'Exception in GetMode request: {e}')

    def create_thread(self):
        server_thread = threading.Thread(target=self.start_server)
        server_thread.start()

    def start_server(self):
        while rclpy.ok():
            try:
                # Use select to handle socket operations with a timeout
                readable, _, _ = select.select([self.server_socket], [], [], 1.0)
                if self.server_socket in readable:
                    client_socket, addr = self.server_socket.accept()
                    #self.get_logger().info(f'Accepted connection from {addr}')
                    client_handler = threading.Thread(target=self.handle_client, args=(client_socket, addr))
                    with self.lock:
                        self.client_threads[addr] = (client_socket, client_handler)
                    client_handler.start()
            except Exception as e:
                self.get_logger().error(f'Error in start_server: {e}')
                break

    def handle_client(self, client_socket, addr):
        try:
            while rclpy.ok():
                data = client_socket.recv(1024).decode('utf-8').strip()
                if not data:
                    break

                if data == "HEARTBEAT":
                    if self.planning_ended:
                        self.planning_ended = False
                        response = "PLANNED"
                    else:
                        response = "ACK"
                elif data == "STOP":
                    self.send_FSM_change_request(9)  # 9 is the transition number for Estop
                    response = "ACK"
                    self.get_logger().info("Emergency STOP signal received")
                elif data == "GET_STATE":
                    response = f"STATE;{self.J8_state}"
                elif data.startswith("GPS;"):
                    parts = data.split(";")
                    if len(parts) == 3:
                        try:
                            self.latitude = float(parts[1])
                            self.longitude = float(parts[2])
                            self.publish_gps_coordinates(self.latitude, self.longitude)
                            response = "ACK"
                        except ValueError:
                            self.get_logger().error(f'Error parsing GPS coordinates: {parts[1]}, {parts[2]}')
                            response = "INVALID GPS DATA"
                    else:
                        response = "INVALID GPS DATA"
                elif data.startswith("REQUEST;"):
                    parts = data.split(";")
                    if len(parts) == 3:
                        try:
                            bool_msg = Bool()
                            bool_msg.data = True
                            self.emergency_call.publish(bool_msg)
                            request_latitude = float(parts[1])
                            request_longitude = float(parts[2])
                            self.get_logger().info(f'Received request for robot to move to: Latitude={request_latitude}, Longitude={request_longitude}')
                            #success = self.call_plan_path_service(request_latitude, request_longitude)
                            success = True
                            if success:
                                response = "PLANNING_COMPLETE;true"
                            else:
                                response = "PLANNING_COMPLETE;false"
                        except ValueError:
                            self.get_logger().error(f'Error parsing request coordinates: {parts[1]}, {parts[2]}')
                            response = "INVALID REQUEST DATA"
                    else:
                        response = "INVALID REQUEST DATA"
                else:
                    response = "UNKNOWN COMMAND"

                client_socket.send(response.encode('utf-8'))
        except Exception as e:
            self.get_logger().error(f'Exception: {e}')
        finally:
            with self.lock:
                if addr in self.client_threads:
                    del self.client_threads[addr]
            try:
                client_socket.close()
            except Exception as close_exception:
                self.get_logger().error(f'Exception closing socket: {close_exception}')



    def call_plan_path_service(self, latitude, longitude):
        if self.robot_latitude is None or self.robot_longitude is None:
            self.get_logger().error('Robot GPS coordinates are not available')
            return False

        self.get_logger().info('Planning to POI...')
        req = PlanPath.Request()
        
        try:
            start_utm = utm.from_latlon(self.robot_latitude, self.robot_longitude)
            req.start.x = start_utm[0]
            req.start.y = start_utm[1]
            req.start.z = 0.0  # Assuming a flat plane

            goal_utm = utm.from_latlon(latitude, longitude)
            req.goal.x = goal_utm[0]
            req.goal.y = goal_utm[1]
            req.goal.z = 0.0  # Assuming a flat plane

            self.get_logger().info('Calling path planner service...')
            future = self.plan_path_client.call_async(req)
            rclpy.spin_until_future_complete(self, future)
            response = future.result()
            if response is not None:
                self.get_logger().info(f"Planning result: {response.ack}, Path length: {len(response.path.poses)}")

                
                return response.ack
            else:
                self.get_logger().info("Planning failed with no response")
                return False
        except Exception as e:
            self.get_logger().error(f'Exception converting coordinates to UTM: {e}')
            return False


    def plan_path_callback(self, future,):
        try:
            response = future.result()
            if response is not None:
                result = response
                self.get_logger().info(f"Planning result: {result.ack}, Path length: {len(result.path.poses)}")
                bool_msg = Bool()
                bool_msg.data = True
                self.emergency_call.publish(bool_msg)
            else:
                self.get_logger().info("Planning failed with no response")
        except Exception as e:
            self.get_logger().error(f'Exception in plan path service call: {e}')





    def publish_gps_coordinates(self, latitude, longitude):
            navsatfix_msg = NavSatFix()
            navsatfix_msg.header.stamp = self.get_clock().now().to_msg()
            navsatfix_msg.header.frame_id = "human_gps"
            navsatfix_msg.status.status = NavSatStatus.STATUS_FIX
            navsatfix_msg.status.service = NavSatStatus.SERVICE_GPS
            navsatfix_msg.latitude = latitude
            navsatfix_msg.longitude = longitude
            navsatfix_msg.altitude = 0.0  # Placeholder, update if altitude data is available
            self.navsatfix_publisher.publish(navsatfix_msg)
           

def main(args=None):
    rclpy.init(args=args)
    node = TcpServerLifecycleNode()
    executor = rclpy.executors.SingleThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()

if __name__ == '__main__':
    main()
