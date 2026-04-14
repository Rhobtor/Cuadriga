#ifndef CUADRIGA_ACTUATOR__CUADRIGA_ACTUATOR_NODE_HPP_
#define CUADRIGA_ACTUATOR__CUADRIGA_ACTUATOR_NODE_HPP_

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cuadriga_actuator
{

class CuadrigaActuatorNode : public rclcpp::Node
{
public:
  CuadrigaActuatorNode();
  ~CuadrigaActuatorNode() override;

private:
  struct CommandState
  {
    double linear{0.0};
    double angular{0.0};
    rclcpp::Time stamp{0, 0, RCL_ROS_TIME};
    bool valid{false};
  };

  enum class OutputMode
  {
    Serial,
    Topic,
  };

  enum class Source
  {
    None,
    Direct,
    Fsm,
  };

  void directCmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
  void fsmCmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
  void controlLoop();

  bool shouldUseCommand(const CommandState & command, double timeout_seconds, const rclcpp::Time & now) const;
  Source selectSource(const rclcpp::Time & now) const;
  void publishSource(Source source);
  bool sendMotion(double linear_velocity, double angular_velocity);
  int velocityToPwm(double value, double max_value) const;
  int angularToPwm(double value, double max_value) const;
  int scaleToPwm(double value, double max_value) const;
  std::vector<uint8_t> encodePwmCommand(int pwm_value, char positive_prefix, char negative_prefix) const;
  std::string formatCommandForLog(const std::vector<uint8_t> & command) const;

  bool openSerialIfNeeded();
  void closeSerial();
  bool writeCommand(const std::vector<uint8_t> & command);

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr direct_cmd_subscription_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr fsm_cmd_subscription_;
  rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr serial_write_publisher_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr active_source_publisher_;
  rclcpp::TimerBase::SharedPtr control_timer_;

  CommandState direct_command_;
  CommandState fsm_command_;
  Source last_published_source_{Source::None};
  bool stop_sent_without_source_{false};

  std::string direct_cmd_vel_topic_;
  std::string fsm_cmd_vel_topic_;
  std::string serial_write_topic_;
  std::string active_source_topic_;
  std::string serial_port_;
  OutputMode output_mode_{OutputMode::Serial};

  double control_rate_hz_{20.0};
  double direct_cmd_timeout_{0.25};
  double fsm_cmd_timeout_{0.25};
  double command_timeout_{0.5};
  double linear_velocity_max_{1.2};
  double angular_velocity_max_{6.0};
  int pwm_limit_{127};
  int baud_rate_{9600};
  int serial_bytesize_{7};
  int serial_stop_bits_{1};
  int inter_command_delay_ms_{5};
  char serial_parity_{'E'};
  bool allow_reverse_linear_{true};
  bool send_angular_first_{true};
  bool log_serial_commands_{false};

  int serial_fd_{-1};
};

}  // namespace cuadriga_actuator

#endif  // CUADRIGA_ACTUATOR__CUADRIGA_ACTUATOR_NODE_HPP_