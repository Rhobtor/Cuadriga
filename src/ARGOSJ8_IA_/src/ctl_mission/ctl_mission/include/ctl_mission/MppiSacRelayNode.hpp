#ifndef CTL_MISSION__MPPI_SAC_RELAY_NODE_HPP_
#define CTL_MISSION__MPPI_SAC_RELAY_NODE_HPP_

#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>

#include <geometry_msgs/msg/twist.hpp>

/**
 * @file MppiSacRelayNode.hpp
 * @brief Lifecycle state node that relays external AI/MPPI controller commands into the rover.
 *
 * This node is intended to be used as one of the mission FSM states managed by
 * `CtlMissionNode`.
 *
 * The AI controller (MPPI + SAC) runs outside this repository. Inside this repo we
 * only provide a *bridge*:
 *   - Subscribe to a Twist topic published by the AI module.
 *   - Republish that Twist to the rover's secure command channel (typically
 *     `secured_cmd_vel`).
 *
 * When the node is not active, it doesn't forward commands.
 */
class MppiSacRelayNode : public rclcpp_lifecycle::LifecycleNode {
public:
  explicit MppiSacRelayNode(const std::string & node_name, bool intra_process_comms = false);

protected:
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(
    const rclcpp_lifecycle::State &) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State & state) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & state) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(
    const rclcpp_lifecycle::State &) override;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(
    const rclcpp_lifecycle::State & state) override;

private:
  void on_ai_cmd(const geometry_msgs::msg::Twist::SharedPtr msg);
  bool send_start_to_controller();

  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_secured_cmd_vel_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_ai_cmd_;

  std::string ai_cmd_topic_;
  std::string secured_cmd_topic_;
  int queue_size_ = 10;
  bool start_controller_on_activate_ = false;

  std::string controller_ip_;
  int controller_port_ = 5555;
};

#endif  // CTL_MISSION__MPPI_SAC_RELAY_NODE_HPP_
