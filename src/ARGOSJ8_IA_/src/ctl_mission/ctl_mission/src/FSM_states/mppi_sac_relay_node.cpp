/**
 * @file mppi_sac_relay_node.cpp
 * @brief Lifecycle state node that relays external AI controller commands to secured cmd_vel.
 *
 * ## Intent
 * The MPPI+SAC controller itself is developed outside this repository.
 * This node is only a *bridge* so the mission FSM can:
 *   - enter a dedicated "AI control" mode
 *   - forward Twist commands to the same channel used by the rest of the stack
 *     (`secured_cmd_vel` by default)
 *
 * ## ROS contract
 * ### Subscriptions
 * - `ai_cmd_topic_name` (default: `external_cmd_vel`) [geometry_msgs/msg/Twist]
 *
 * ### Publications
 * - `secured_cmd_vel_topic_name` (default: `secured_cmd_vel`) [geometry_msgs/msg/Twist]
 *
 * ### Parameters
 * - `queue_size` (default: 10)
 *
 * ## Lifecycle behavior
 * - on_configure(): create subscription + lifecycle publisher.
 * - on_activate(): activate publisher so messages can flow.
 * - on_deactivate(): publisher deactivated (relay stops).
 */

#include "ctl_mission/MppiSacRelayNode.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

MppiSacRelayNode::MppiSacRelayNode(const std::string & node_name, bool intra_process_comms)
: rclcpp_lifecycle::LifecycleNode(
    node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms))
{
  this->declare_parameter("ai_cmd_topic_name", "external_cmd_vel");
  this->declare_parameter("secured_cmd_vel_topic_name", "secured_cmd_vel");
  this->declare_parameter("queue_size", 10);
  this->declare_parameter("start_controller_on_activate", false);

  // External AI controller start handshake (simple TCP message).
  this->declare_parameter("controller_ip", "192.168.1.116");
  this->declare_parameter("controller_port", 5555);
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
MppiSacRelayNode::on_configure(const rclcpp_lifecycle::State &)
{
  this->get_parameter("ai_cmd_topic_name", ai_cmd_topic_);
  this->get_parameter("secured_cmd_vel_topic_name", secured_cmd_topic_);
  this->get_parameter("queue_size", queue_size_);
  this->get_parameter("start_controller_on_activate", start_controller_on_activate_);
  this->get_parameter("controller_ip", controller_ip_);
  this->get_parameter("controller_port", controller_port_);

  pub_secured_cmd_vel_ = this->create_publisher<geometry_msgs::msg::Twist>(secured_cmd_topic_, queue_size_);
  sub_ai_cmd_ = this->create_subscription<geometry_msgs::msg::Twist>(
    ai_cmd_topic_, queue_size_, std::bind(&MppiSacRelayNode::on_ai_cmd, this, std::placeholders::_1));

  RCLCPP_INFO(
    this->get_logger(),
    "Configured MPPI/SAC relay. Subscribing to '%s', publishing to '%s'",
    ai_cmd_topic_.c_str(), secured_cmd_topic_.c_str());

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
MppiSacRelayNode::on_activate(const rclcpp_lifecycle::State & state)
{
  LifecycleNode::on_activate(state);
  if (pub_secured_cmd_vel_) {
    pub_secured_cmd_vel_->on_activate();
  }

  if (start_controller_on_activate_) {
    (void)send_start_to_controller();
  } else {
    RCLCPP_INFO(this->get_logger(), "Controller TCP start handshake disabled; relay will use ROS topics only");
  }

  RCLCPP_INFO(this->get_logger(), "MPPI/SAC relay activated");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
MppiSacRelayNode::on_deactivate(const rclcpp_lifecycle::State & state)
{
  if (pub_secured_cmd_vel_) {
    pub_secured_cmd_vel_->on_deactivate();
  }
  LifecycleNode::on_deactivate(state);
  RCLCPP_INFO(this->get_logger(), "MPPI/SAC relay deactivated");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
MppiSacRelayNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  sub_ai_cmd_.reset();
  pub_secured_cmd_vel_.reset();
  RCLCPP_INFO(this->get_logger(), "MPPI/SAC relay cleaned up");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
MppiSacRelayNode::on_shutdown(const rclcpp_lifecycle::State & state)
{
  sub_ai_cmd_.reset();
  pub_secured_cmd_vel_.reset();
  RCLCPP_INFO(this->get_logger(), "MPPI/SAC relay shutdown from state %s", state.label().c_str());
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

void MppiSacRelayNode::on_ai_cmd(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  // Only publish if active.
  // LifecyclePublisher internally checks activation state when calling publish().
  if (pub_secured_cmd_vel_) {
    pub_secured_cmd_vel_->publish(*msg);
  }
}

bool MppiSacRelayNode::send_start_to_controller()
{
  if (controller_ip_.empty() || controller_port_ <= 0 || controller_port_ > 65535) {
    RCLCPP_WARN(this->get_logger(), "Controller address invalid (ip='%s', port=%d)", controller_ip_.c_str(), controller_port_);
    return false;
  }

  const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    RCLCPP_WARN(this->get_logger(), "Failed to create socket: %s", std::strerror(errno));
    return false;
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(static_cast<uint16_t>(controller_port_));
  if (::inet_pton(AF_INET, controller_ip_.c_str(), &addr.sin_addr) != 1) {
    RCLCPP_WARN(this->get_logger(), "Invalid controller_ip '%s'", controller_ip_.c_str());
    ::close(fd);
    return false;
  }

  // Non-blocking connect with ~1s timeout so activation can't hang.
  int flags = ::fcntl(fd, F_GETFL, 0);
  if (flags >= 0) {
    (void)::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }

  int rc = ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
  if (rc != 0 && errno != EINPROGRESS) {
    RCLCPP_WARN(
      this->get_logger(),
      "Failed to connect to controller %s:%d: %s",
      controller_ip_.c_str(), controller_port_, std::strerror(errno));
    ::close(fd);
    return false;
  }

  if (rc != 0) {
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(fd, &wfds);
    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    const int sel = ::select(fd + 1, nullptr, &wfds, nullptr, &tv);
    if (sel <= 0) {
      RCLCPP_WARN(
        this->get_logger(),
        "Timeout connecting to controller %s:%d",
        controller_ip_.c_str(), controller_port_);
      ::close(fd);
      return false;
    }

    int so_error = 0;
    socklen_t len = sizeof(so_error);
    if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &so_error, &len) != 0 || so_error != 0) {
      RCLCPP_WARN(
        this->get_logger(),
        "Failed to connect to controller %s:%d: %s",
        controller_ip_.c_str(), controller_port_, std::strerror(so_error != 0 ? so_error : errno));
      ::close(fd);
      return false;
    }
  }

  // Restore blocking mode for send.
  if (flags >= 0) {
    (void)::fcntl(fd, F_SETFL, flags);
  }

  static constexpr const char * kStartMsg = "START\n";
  const ssize_t sent = ::send(fd, kStartMsg, std::strlen(kStartMsg), 0);
  if (sent < 0) {
    RCLCPP_WARN(
      this->get_logger(),
      "Failed sending START to controller %s:%d: %s",
      controller_ip_.c_str(), controller_port_, std::strerror(errno));
    ::close(fd);
    return false;
  }

  ::close(fd);
  RCLCPP_INFO(this->get_logger(), "Sent START to controller %s:%d", controller_ip_.c_str(), controller_port_);
  return true;
}
