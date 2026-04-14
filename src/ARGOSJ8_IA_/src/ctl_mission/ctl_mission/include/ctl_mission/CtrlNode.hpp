#ifndef STANLEY_CTRL_NODE_HPP_
#define STANLEY_CTRL_NODE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <cmath> 
#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "path_manager_interfaces/srv/robot_path.hpp"
#include "nav_msgs/msg/path.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "ctl_mission/Stanley_Ctrl.h"
#include "ctl_mission/Pure_Pursuit.h"
#include "ctl_mission/Dynamic_PP.h"
#include "ctl_mission/Dynamic_LAPP.h"
#include "ctl_mission/Regulated_Pure_Pursuit.h"
#include "ctl_mission_interfaces/srv/config_simple_ctl.hpp"
#include "ctl_mission_interfaces/srv/config_pure_pursuit_ctrl.hpp"
#include "ctl_mission_interfaces/srv/config_dynamic_pure_ctrl.hpp"
#include "ctl_mission_interfaces/srv/config_stanley_ctrl.hpp"
#include "ctl_mission_interfaces/srv/config_dynamic_la_pure_ctrl.hpp"
#include "ctl_mission_interfaces/srv/config_regulated_pure_ctrl.hpp"
#include "ctl_mission_interfaces/srv/change_controller.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

/// CtrlNode inheriting from rclcpp_lifecycle::LifecycleNode

class CtrlNode : public rclcpp_lifecycle::LifecycleNode
{
 
  private:
  // We hold an instance of a lifecycle publisher. This lifecycle publisher
  // can be activated or deactivated regarding on which state the lifecycle node
  // is in.
  // By default, a lifecycle publisher is inactive by creation and has to be
  // activated to publish messages into the ROS world.
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_cmd_vel;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Int32>> pub_look_ahead;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>> pub_look_ahead_distance;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>> pub_local_path;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>> pub_cte;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>> pub_la_distance;
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::PoseStamped>>  pub_la_pose;

  rclcpp::Service<ctl_mission_interfaces::srv::ConfigSimpleCtl>::SharedPtr configSimpleCtl; 
  rclcpp::Service<ctl_mission_interfaces::srv::ChangeController>::SharedPtr changeControllersrv;
  rclcpp::Service<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl>::SharedPtr configPurePursuitCtrlsrv;
  rclcpp::Service<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>::SharedPtr configPureRegulatedCtrlsrv;
  rclcpp::Service<ctl_mission_interfaces::srv::ConfigStanleyCtrl>::SharedPtr configStanleyCtrlsrv;
  rclcpp::Service<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl>::SharedPtr configDynamicPuresrv;
  rclcpp::Service<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>::SharedPtr configDynamicLAPuresrv;

  nav_msgs::msg::Path path_msg;

  std::string robot_name, robot_frame, fixed_local_frame, fixed_global_frame, trajectory_topic_name, cmd_vel_name, current_controler, change_ctrl_srv_name, 
              config_pp_srv_name, config_dynamic_pp_srv_name, config_dynamic_la_pp_srv_name, config_regulated_srv_name;
  bool got_path = false;
  std::shared_ptr<rclcpp::TimerBase> timer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
   std::shared_ptr<rclcpp::TimerBase> gradual_stop_timer_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;  
  rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr trajectoryToFollow; //!Service to change robot max linear speed
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr dist_last_obj_sub; //Suscriber to distance to last obj
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_sub; //Suscriber to distance to last obj
      

  nav_msgs::msg::Path robot_path;
  geometry_msgs::msg::Twist cmd_msg;
  int min_point_idx = 0, points_look_ahead, points_to_local_planner, stan_n_window_size, pp_n_window_size;
  float stan_K_linear_err = 0.2,  stan_K_ang_err = 1, stan_linear_speed, stan_look_ahead_dist, pp_look_ahead_dist;
  float pp_K_linear_err = 0.2,  pp_K_ang_err = 1, pp_linear_speed, controller_frequency, min_speed, breaking_acc, dist_last_obj;
  double v_x_odom, w_z_odom;
  std::vector<std::string> available_controller_types_ = {"pure_pursuit", "regulated_pure_pursuit", "dynamic_pure_pursuit", "dynamic_la_pure_pursuit"};
  float dyn_pp_look_ahead_distance, dyn_pp_max_speed, dyn_pp_max_ang_acc, dyn_pp_max_ang_dec, dyn_pp_max_lin_acc, dyn_pp_max_lin_dec;
  float dyn_la_pp_look_ahead_v_gain, dyn_la_pp_max_speed, dyn_la_pp_max_ang_acc, dyn_la_pp_max_ang_dec, 
  dyn_la_pp_max_lin_acc, dyn_la_pp_max_lin_dec, dyn_la_pp_min_speed;
  float regulated_pure_pursuit_l_ahead, regulated_pure_pursuit_r_min, regulated_pure_pursuit_v_forward;
  rclcpp::Time path_msg_timestamp;
  Stanley_Ctrl stanley_controller;
  PurePursuit pure_pursuit_controller;
  DynamicPP dynamic_pp_controller;
  DynamicLAPP dynamic_la_pp_controller;
  RegulatedPurePursuit regulated_pure_pursuit_controller;

nav_msgs::msg::Path transform_path_to_robot_frame(const nav_msgs::msg::Path& path_in);
void getTrajectory(const nav_msgs::msg::Path::SharedPtr msg) ;
void getDistLastObj(const std_msgs::msg::Float32::SharedPtr msg);
void getOdom(const nav_msgs::msg::Odometry::SharedPtr msg);

public:
  /// CtrlNode constructor
  explicit CtrlNode(const std::string & node_name, bool intra_process_comms = false);

  void configSimpController(const std::shared_ptr<ctl_mission_interfaces::srv::ConfigSimpleCtl::Request> req,
                                    const std::shared_ptr<ctl_mission_interfaces::srv::ConfigSimpleCtl::Response> res);

  void changeControllerType(
        const std::shared_ptr<ctl_mission_interfaces::srv::ChangeController::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ChangeController::Response> response);

  void configPurePursuit(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl::Response> response);
        
  void configRegulatedPure(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl::Response> response);
        
  void configDynamic(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl::Response> response);  

  void configDynamicLA(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl::Response> response);

  double calculateCrossTrackErrorInRobotFrame(const nav_msgs::msg::Path& path);

  /// Callback for walltimer in order to publish the message.
  void publish();
  void gradualStop();
  /// Transition callback for state configuring
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);

  /// Transition callback for state activating
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State & state);

  /// Transition callback for state deactivating
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & state);

  /// Transition callback for state cleaningup
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);

  /// Transition callback for state shutting down
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State & state);
};

#endif // STANLEY_CTRL_NODE_HPP_

