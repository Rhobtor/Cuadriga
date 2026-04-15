#ifndef READY_NODE_HPP
#define READY_NODE_HPP

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class ReadyNode : public rclcpp_lifecycle::LifecycleNode {
public:
  explicit ReadyNode(const std::string & node_name, bool intra_process_comms = false);

private:
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_cmd_vel_;
  std::shared_ptr<rclcpp::TimerBase> timer_;
  std::string cmd_vel_topic;
  geometry_msgs::msg::Twist cmd_msg_;

  void refresh_cmd_vel();

  // Lifecycle node interface methods
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);
};

#endif // READY_NODE_HPP

