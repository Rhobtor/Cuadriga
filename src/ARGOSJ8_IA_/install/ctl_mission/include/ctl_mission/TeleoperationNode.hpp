#ifndef TELEOPERATION_NODE_HPP_
#define TELEOPERATION_NODE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/joy.hpp>

using namespace std::chrono_literals;

class TeleoperationNode : public rclcpp_lifecycle::LifecycleNode {
public:
  explicit TeleoperationNode(const std::string &node_name, bool intra_process_comms = false);

protected:
  // Lifecycle stages
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;

private:
  void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);
  void refresh_cmd_vel();

  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_cmd_vel_;
  std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Joy>> sub_joy_;
  std::shared_ptr<rclcpp::TimerBase> timer_;
  
  float x_axis_, z_axis_, x_multiplier, z_multiplier;
  bool dead_man_ = false;
  int dead_man_button_ = 5, x_axis_button_ = 1, z_axis_button_ = 3;
  rclcpp::Time last_joy_msg_timestamp_;
  double max_joy_msg_delay_ = 1.0;  // Adjust as needed
};

#endif  // TELEOPERATION_NODE_HPP_

