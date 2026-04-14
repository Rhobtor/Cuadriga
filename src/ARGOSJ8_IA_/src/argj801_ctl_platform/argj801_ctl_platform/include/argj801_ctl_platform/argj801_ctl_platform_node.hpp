#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include <lifecycle_msgs/msg/transition.hpp>
#include "rclcpp/rclcpp.hpp"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include <lifecycle_msgs/msg/state.hpp>
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"

#include "rcutils/logging_macros.h"
#include "geometry_msgs/msg/twist.hpp"

#include "arduino_interface/serial_arduino.h"

#include "argj801_ctl_platform_interfaces/srv/set_velocity.hpp"
#include "argj801_ctl_platform_interfaces/srv/get_velocity.hpp"
#include "argj801_ctl_platform_interfaces/srv/emergency_stop.hpp"
#include "argj801_ctl_platform_interfaces/msg/cmd_throttle_msg.hpp"
#include "argj801_ctl_platform_interfaces/msg/kinematic_debug_msg.hpp"

#include "std_srvs/srv/empty.hpp"

#include "diagnostic_msgs/srv/self_test.hpp"
#include "self_test/test_runner.hpp"
#include "diagnostic_updater/update_functions.hpp"

#include "argj801_lcm/LCM_interface.hpp"

#include "lcmmessages/request_connection_response_msg.hpp"
#include "lcmmessages/dat_station_heartbeat_msg.hpp"

#include "argj801_ctl_platform/argj801_kinematic_model.hpp"



#define MESSAGE_TIMEOUT 1.0 //seconds
#define PLATFORM_ID -1299645807
#define HEARBEAT_COUNT_MAX 3

using namespace std::chrono_literals;

enum class PlatformOperationMode{Arduino,LCM,Gazebo};

/**
 * @class CtlPlatformNode
 * @brief Nodo Lifecycle encargado del control de la plataforma (actuación) del rover.
 *
 * Este nodo recibe comandos de velocidad (Twist) y los transforma en comandos
 * low-level dependiendo del modo de operación:
 *
 * - Arduino: comunica por puerto serie con un microcontrolador.
 * - LCM: envía/recibe mensajes LCM hacia el stack de vehículo.
 * - Gazebo: publica `CmdThrottleMsg` para simulación.
 *
 * Contrato ROS típico (namespace `/ARGJ801` en el launch):
 * - Sub: `/ARGJ801/cmd_vel` (geometry_msgs/Twist)
 * - Pub: `cmd_throttle_msg` (argj801_ctl_platform_interfaces/msg/CmdThrottleMsg) en modos LCM/Gazebo.
 * - Services:
 *   - `emergency_stop` (EmergencyStop)
 *   - `set_velocity`, `get_velocity` (modo Arduino)
 *   - `ping` (std_srvs/Empty)
 *
 * Parámetros relevantes (ver `argj801_setup/config/J8_params.yaml`):
 * - operation_mode (0 Arduino, 1 LCM, 2 Gazebo) [según enum actual]
 * - throttle_topic_name, secured_cmd_vel_topic_name
 * - arduino_params.port, arduino_params.watchdog_active
 * - lcm_params.lcm_config_file
 * - kinematic_parameters.* (efective_radius, xICR, throttle_to_percent, steer_to_percent, ...)
 */
class CtlPlatformNode : public rclcpp_lifecycle::LifecycleNode
{
private:
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription;

  PlatformOperationMode pOpMode;

  rclcpp::TimerBase::SharedPtr timerWatchdog;
  bool watchdog_active;

  std::unique_ptr<self_test::TestRunner> self_test_; 
  std::shared_ptr<diagnostic_updater::Updater> diagnostic_; //! Object that allows the IMU diagnostic
  double desired_freq_;
  std::unique_ptr<diagnostic_updater::FrequencyStatus> freq_diag_; 
  bool self_test_active;

  std::string cmd_vel_namespace, cmd_vel_name_in, cmd_throttle_name, kinematic_debug_topic_name;

  rclcpp::CallbackGroup::SharedPtr group1;

  void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
  void timerWatchdogCallback();

  /////////// Arduino Mode ///////////
  SerialArduino serialArduino;
  std::string port;
  
  rclcpp::Service<argj801_ctl_platform_interfaces::srv::SetVelocity>::SharedPtr setVelocityServ; //!Service to set the velocity
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr pingServ; //!Service to make ping to the IMU
  rclcpp::Service<argj801_ctl_platform_interfaces::srv::GetVelocity>::SharedPtr  getVelocityServ; //!Service to get the velocity
  rclcpp::Service<argj801_ctl_platform_interfaces::srv::EmergencyStop>::SharedPtr emergencyStopServ; //!Service to stop the robot

  /////////// End Arduino Mode ///////////
  
  /////////// LCM Mode ///////////
  std::shared_ptr<argj801_lcm::LCMInterface> lcmInterface;
  rclcpp::TimerBase::SharedPtr timerStationHeartbeat;
  rclcpp::TimerBase::SharedPtr timerWellsSpeed;
  rclcpp::TimerBase::SharedPtr timerPlatfomHeartbeat;
  
  lcmmessages::dat_vehicle_heartbeat_msg platformHeartbeatMsg;

  std::string lcm_config_path;
  std::string lcm_config_file;
  bool require_vehicle_heartbeat = true;
  
  std::unique_ptr<Argo_J8_KinematicModel> argo_kinematic_model;

  void timerStationHeartBeatCallback();
  void timerPlatformHeartBeatCallback();

 /////////// Gazebo Mode ///////////
  float xICR;   
  float throttle_to_percent;
  float steer_to_percent; 
  float efective_radius;
  float steer_acc;
  float throttle_acc;
  int platformHearbeatCount;
  rclcpp::Time last_twist_time_;

  float target_x_ = 0;
  float target_rot_ = 0;

  void timerWellsSpeedCallback();
  void publishCmdThrottle(double throttle, double steering);
  void publishKinematicDebug(double throttle, double steering);

  /////////// End LCM Mode ///////////

  rclcpp::Publisher<argj801_ctl_platform_interfaces::msg::CmdThrottleMsg>::SharedPtr cmdThrottlePub;  
  rclcpp::Publisher<argj801_ctl_platform_interfaces::msg::KinematicDebugMsg>::SharedPtr kinematicDebugPub;
  /////////////// End LCM Methods ///////////////

  int cont=0;
public:
  /// LifecycleTalker constructor
  /**
   * The lifecycletalker/lifecyclenode constructor has the same
   * arguments a regular node.
   */
  CtlPlatformNode(const std::string &node_name, bool intra_process_comms = false);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_error(const rclcpp_lifecycle::State &state);

  bool emergencyStop(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::EmergencyStop::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::EmergencyStop::Response> res);
  /////////////// Arduino Methods ///////////////
  bool setVelocity(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::SetVelocity::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::SetVelocity::Response> res);

  bool getVelocity(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::GetVelocity::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::GetVelocity::Response> res);

  bool ping(const std::shared_ptr<std_srvs::srv::Empty::Request> &req,const std::shared_ptr<std_srvs::srv::Empty::Response> &res);

  void arduinoStatus(diagnostic_updater::DiagnosticStatusWrapper &status);

  void interruptionTest(diagnostic_updater::DiagnosticStatusWrapper &status);

  void connectStartTest(diagnostic_updater::DiagnosticStatusWrapper &status);

  void pingTest(diagnostic_updater::DiagnosticStatusWrapper &status); 

  void moveTest(diagnostic_updater::DiagnosticStatusWrapper &status);

  void disconnectTest(diagnostic_updater::DiagnosticStatusWrapper &status);

  void resumeTest(diagnostic_updater::DiagnosticStatusWrapper &status);
  ////////////////// End Arduino Methods ///////////////

  /////////////// LCM Methods ///////////////
    void lcmStatus(diagnostic_updater::DiagnosticStatusWrapper &status);

};

