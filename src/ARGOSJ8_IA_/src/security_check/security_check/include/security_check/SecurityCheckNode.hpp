#ifndef SECURITY_CHECK_NODE_HPP_
#define SECURITY_CHECK_NODE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <mutex>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/float32.hpp"
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <std_srvs/srv/set_bool.hpp>
#include "security_check_interfaces/srv/get_security_check_status.hpp"
#include "ctl_mission_interfaces/srv/get_mode.hpp"
#include "ctl_mission_interfaces/srv/change_mode.hpp"
#include "security_check_interfaces/srv/password_check.hpp"
#include "ctl_mission/J8_FSM.h"

class SecurityCheckNode : public rclcpp_lifecycle::LifecycleNode
{
public:
    explicit SecurityCheckNode(const std::string & node_name, bool intra_process_comms = false);
    
    void cmdvelCallbak(const geometry_msgs::msg::Twist::SharedPtr msg);
    void lidarCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    void handle_security_service(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                 std::shared_ptr<std_srvs::srv::SetBool::Response> response);
    void passwordServiceCallback(
    const std::shared_ptr<security_check_interfaces::srv::PasswordCheck::Request> request,
    std::shared_ptr<security_check_interfaces::srv::PasswordCheck::Response> response);

    void perform_security_check();

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_configure(const rclcpp_lifecycle::State &);

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_activate(const rclcpp_lifecycle::State &);

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_deactivate(const rclcpp_lifecycle::State &);

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_cleanup(const rclcpp_lifecycle::State &);

    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_shutdown(const rclcpp_lifecycle::State & state);
    void getSecurity(
        const std::shared_ptr<security_check_interfaces::srv::GetSecurityCheckStatus::Request> request,
        std::shared_ptr<security_check_interfaces::srv::GetSecurityCheckStatus::Response> response);
        
private:
    rclcpp::Time last_msg_time_, last_lidar_msg_time_;
    double message_age_threshold_;
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr security_service_;
    rclcpp::Client<ctl_mission_interfaces::srv::ChangeMode>::SharedPtr change_FSM_mode_srv_;
    rclcpp::Client<ctl_mission_interfaces::srv::GetMode>::SharedPtr get_moed_sevice_;
    rclcpp::Service<security_check_interfaces::srv::PasswordCheck>::SharedPtr password_service_;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_cmd_vel;
    geometry_msgs::msg::Twist::SharedPtr received_cmd_vel, safe_cmd_vel, last_cmd_vel, adjusted_msg;
    std::shared_ptr<rclcpp::Subscription<geometry_msgs::msg::Twist>> sub_cmd_vel;
    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::LaserScan>> sub_lidar;
    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::Float32>> sub_min_distance_;
    rclcpp::Service<security_check_interfaces::srv::GetSecurityCheckStatus>::SharedPtr getSecurityChecksrv;
    std::vector<rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr> lidar_subs_;
    float th1, th2, factor, threshold_path;
    std::mutex mutex_;
    bool obstacle_detected, disable_check, security_enabled_, security_bypass_enabled_;
    std::map<std::string, sensor_msgs::msg::LaserScan> last_lidar_messages_;
    std::string input_cmd_vel_topic, output_cmd_vel_topic, lidar_topic, min_distance_to_path_topic_name, get_security_check_srv_name, password_check_srv_name, 
                dissable_srv_name, change_fsm_mode_srv_name, get_fsm_srv_name;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_future<std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Response>> service_future_fsm;
    std::vector<rclcpp::Client<ctl_mission_interfaces::srv::GetMode>::SharedFuture> future_results_;
    std::shared_ptr<rclcpp::TimerBase> future_check_timer_fsm, mode_timer_, security_check_timer_;
    Mode current_mode_;
    std::string bypass_password;
    void sendChangeModeRequest( const rclcpp::Client<ctl_mission_interfaces::srv::ChangeMode>::SharedPtr& client,
    int transition_id);
    rclcpp::Client<ctl_mission_interfaces::srv::GetMode>::SharedPtr get_mode_client_;
    void checkChangeModeFuture();
    void fetch_fsm_mode();
    void check_futures() ;
    void minDistanceCallback(const std_msgs::msg::Float32::SharedPtr msg);

};

#endif  // SECURITY_CHECK_NODE_HPP_

