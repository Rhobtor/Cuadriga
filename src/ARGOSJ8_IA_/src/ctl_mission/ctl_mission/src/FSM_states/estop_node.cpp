/**
 * @file estop_node.cpp
 * @brief Lifecycle state node that enforces an emergency stop by publishing zero cmd_vel.
 *
 * ## Role in the J8 mission FSM
 * This is the global safety "E-STOP" state. The mission/orchestrator can transition to this
 * state from anywhere (see `J8_FSM::AlltoEstop`).
 *
 * When **active**, the node publishes a zero Twist at `publish_rate` Hz.
 * The intended effect is to override motion by constantly commanding zero velocity.
 *
 * ## ROS contract
 * ### Publications
 * - `cmd_vel_topic` (default: `cmd_vel`) [geometry_msgs/msg/Twist]
 *
 * ### Parameters
 * - `publish_rate` (Hz, default: 50.0)
 *
 * @note This node doesn't (yet) cut power or touch low-level safety IO; it is a software
 *       safety mechanism at the ROS command level.
 */

#include "ctl_mission/EstopNode.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

EstopNode::EstopNode(const std::string &node_name, bool intra_process_comms)
    : rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {
    // Declare parameters
    this->declare_parameter("cmd_vel_topic", "cmd_vel");
    this->declare_parameter("publish_rate", 50.0); // Example rate in Hz
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
EstopNode::on_configure(const rclcpp_lifecycle::State &) {
    std::string cmd_vel_topic;
    double publish_rate;

    // Retrieve the parameters
    this->get_parameter("cmd_vel_topic", cmd_vel_topic);
    this->get_parameter("publish_rate", publish_rate);

    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, 10);
    RCLCPP_INFO(get_logger(), "on_configure() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
EstopNode::on_activate(const rclcpp_lifecycle::State & state) {
    double publish_rate;
    this->get_parameter("publish_rate", publish_rate);

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(static_cast<int>(1000.0 / publish_rate)), 
        [this]() { this->refresh_cmd_vel(); });

    LifecycleNode::on_activate(state);
    RCLCPP_INFO(get_logger(), "on_activate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
EstopNode::on_deactivate(const rclcpp_lifecycle::State & state) {
    timer_.reset();
    RCLCPP_INFO(get_logger(), "on_deactivate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
EstopNode::on_cleanup(const rclcpp_lifecycle::State &) {
    timer_.reset();
    pub_cmd_vel.reset();
    RCLCPP_INFO(get_logger(), "on_cleanup() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
EstopNode::on_shutdown(const rclcpp_lifecycle::State & state) {
    timer_.reset();
    pub_cmd_vel.reset();
    RCLCPP_INFO(get_logger(), "on_shutdown() is called from state %s.", state.label().c_str());
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

void EstopNode::refresh_cmd_vel() {
    cmd_msg.linear.x = 0;
    cmd_msg.angular.z = 0;
    pub_cmd_vel->publish(cmd_msg);
}
