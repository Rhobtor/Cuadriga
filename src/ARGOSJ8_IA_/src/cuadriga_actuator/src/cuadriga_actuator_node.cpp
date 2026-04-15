#include "cuadriga_actuator/cuadriga_actuator_node.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <thread>
#include <utility>

namespace cuadriga_actuator
{

namespace
{

speed_t toBaudRate(int baud_rate)
{
  switch (baud_rate) {
    case 9600:
      return B9600;
    case 19200:
      return B19200;
    case 38400:
      return B38400;
    case 57600:
      return B57600;
    case 115200:
      return B115200;
    default:
      return B9600;
  }
}
}  // namespace

CuadrigaActuatorNode::CuadrigaActuatorNode()
: Node("cuadriga_actuator_node")
{
  declare_parameter("direct_cmd_vel_topic", "/Cuadriga/direct_cmd_vel");
  declare_parameter("fsm_cmd_vel_topic", "/cuadriga/cmd_vel");
  declare_parameter("serial_write_topic", "/Cuadriga/serial_write");
  declare_parameter("active_source_topic", "/Cuadriga/command_source");
  declare_parameter("output_mode", "serial");
  declare_parameter("serial_port", "/dev/ttyUSB0");
  declare_parameter("baud_rate", 9600);
  declare_parameter("serial_bytesize", 7);
  declare_parameter("serial_parity", "E");
  declare_parameter("serial_stop_bits", 1);
  declare_parameter("control_rate_hz", 20.0);
  declare_parameter("direct_cmd_timeout", 0.25);
  declare_parameter("fsm_cmd_timeout", 0.25);
  declare_parameter("command_timeout", 0.5);
  declare_parameter("linear_velocity_max", 1.2);
  declare_parameter("angular_velocity_max", 6.0);
  declare_parameter("pwm_limit", 127);
  declare_parameter("allow_reverse_linear", true);
  declare_parameter("send_angular_first", true);
  declare_parameter("inter_command_delay_ms", 5);
  declare_parameter("log_serial_commands", false);

  direct_cmd_vel_topic_ = get_parameter("direct_cmd_vel_topic").as_string();
  fsm_cmd_vel_topic_ = get_parameter("fsm_cmd_vel_topic").as_string();
  serial_write_topic_ = get_parameter("serial_write_topic").as_string();
  active_source_topic_ = get_parameter("active_source_topic").as_string();
  serial_port_ = get_parameter("serial_port").as_string();
  baud_rate_ = get_parameter("baud_rate").as_int();
  serial_bytesize_ = get_parameter("serial_bytesize").as_int();
  serial_stop_bits_ = get_parameter("serial_stop_bits").as_int();
  control_rate_hz_ = get_parameter("control_rate_hz").as_double();
  direct_cmd_timeout_ = get_parameter("direct_cmd_timeout").as_double();
  fsm_cmd_timeout_ = get_parameter("fsm_cmd_timeout").as_double();
  command_timeout_ = get_parameter("command_timeout").as_double();
  linear_velocity_max_ = get_parameter("linear_velocity_max").as_double();
  angular_velocity_max_ = get_parameter("angular_velocity_max").as_double();
  pwm_limit_ = static_cast<int>(get_parameter("pwm_limit").as_int());
  allow_reverse_linear_ = get_parameter("allow_reverse_linear").as_bool();
  send_angular_first_ = get_parameter("send_angular_first").as_bool();
  inter_command_delay_ms_ = static_cast<int>(get_parameter("inter_command_delay_ms").as_int());
  log_serial_commands_ = get_parameter("log_serial_commands").as_bool();

  const std::string parity = get_parameter("serial_parity").as_string();
  if (!parity.empty()) {
    serial_parity_ = static_cast<char>(std::toupper(parity.front()));
  }

  const std::string output_mode_value = get_parameter("output_mode").as_string();
  output_mode_ = output_mode_value == "topic" ? OutputMode::Topic : OutputMode::Serial;

  direct_command_.stamp = now();
  fsm_command_.stamp = now();

  serial_write_publisher_ = create_publisher<std_msgs::msg::UInt8MultiArray>(serial_write_topic_, 10);

  if (output_mode_ == OutputMode::Serial) {
    openSerialIfNeeded();
  }

  active_source_publisher_ = create_publisher<std_msgs::msg::String>(active_source_topic_, 10);

  direct_cmd_subscription_ = create_subscription<geometry_msgs::msg::Twist>(
    direct_cmd_vel_topic_, 10,
    std::bind(&CuadrigaActuatorNode::directCmdCallback, this, std::placeholders::_1));

  fsm_cmd_subscription_ = create_subscription<geometry_msgs::msg::Twist>(
    fsm_cmd_vel_topic_, 10,
    std::bind(&CuadrigaActuatorNode::fsmCmdCallback, this, std::placeholders::_1));

  const auto period = std::chrono::duration<double>(1.0 / std::max(1.0, control_rate_hz_));
  control_timer_ = create_wall_timer(
    std::chrono::duration_cast<std::chrono::nanoseconds>(period),
    std::bind(&CuadrigaActuatorNode::controlLoop, this));

  RCLCPP_INFO(
    get_logger(),
    "Cuadriga actuator ready. direct='%s' fsm='%s' output_mode='%s'",
    direct_cmd_vel_topic_.c_str(),
    fsm_cmd_vel_topic_.c_str(),
    output_mode_ == OutputMode::Topic ? "topic" : "serial");
}

CuadrigaActuatorNode::~CuadrigaActuatorNode()
{
  sendMotion(0.0, 0.0);
  closeSerial();
}

void CuadrigaActuatorNode::directCmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  direct_command_.linear = msg->linear.x;
  direct_command_.angular = msg->angular.z;
  direct_command_.stamp = now();
  direct_command_.valid = true;
}

void CuadrigaActuatorNode::fsmCmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  fsm_command_.linear = msg->linear.x;
  fsm_command_.angular = msg->angular.z;
  fsm_command_.stamp = now();
  fsm_command_.valid = true;
}

bool CuadrigaActuatorNode::shouldUseCommand(
  const CommandState & command,
  double timeout_seconds,
  const rclcpp::Time & now_time) const
{
  if (!command.valid) {
    return false;
  }
  if (timeout_seconds <= 0.0) {
    return true;
  }
  return (now_time - command.stamp).seconds() <= timeout_seconds;
}

CuadrigaActuatorNode::Source CuadrigaActuatorNode::selectSource(const rclcpp::Time & now_time) const
{
  if (shouldUseCommand(direct_command_, direct_cmd_timeout_, now_time)) {
    return Source::Direct;
  }
  if (shouldUseCommand(fsm_command_, fsm_cmd_timeout_, now_time)) {
    return Source::Fsm;
  }
  return Source::None;
}

void CuadrigaActuatorNode::publishSource(Source source)
{
  if (source == last_published_source_) {
    return;
  }

  std_msgs::msg::String msg;
  switch (source) {
    case Source::Direct:
      msg.data = "direct";
      break;
    case Source::Fsm:
      msg.data = "fsm";
      break;
    case Source::None:
    default:
      msg.data = "none";
      break;
  }
  active_source_publisher_->publish(msg);
  last_published_source_ = source;
}

void CuadrigaActuatorNode::controlLoop()
{
  const rclcpp::Time now_time = now();
  const Source source = selectSource(now_time);
  publishSource(source);

  if (source == Source::Direct) {
    stop_sent_without_source_ = false;
    sendMotion(direct_command_.linear, direct_command_.angular);
    return;
  }

  if (source == Source::Fsm) {
    stop_sent_without_source_ = false;
    sendMotion(fsm_command_.linear, fsm_command_.angular);
    return;
  }

  if (!stop_sent_without_source_) {
    sendMotion(0.0, 0.0);
    stop_sent_without_source_ = true;
  }

  if (command_timeout_ > 0.0) {
    const bool direct_recent = shouldUseCommand(direct_command_, command_timeout_, now_time);
    const bool fsm_recent = shouldUseCommand(fsm_command_, command_timeout_, now_time);
    if (direct_recent || fsm_recent) {
      stop_sent_without_source_ = false;
    }
  }
}

bool CuadrigaActuatorNode::sendMotion(double linear_velocity, double angular_velocity)
{
  if (!allow_reverse_linear_) {
    linear_velocity = std::max(0.0, linear_velocity);
  }

  const int linear_pwm = velocityToPwm(linear_velocity, linear_velocity_max_);
  const int angular_pwm = angularToPwm(angular_velocity, angular_velocity_max_);

  const auto angular_command = encodePwmCommand(angular_pwm, 'A', 'a');
  const auto linear_command = encodePwmCommand(linear_pwm, 'B', 'b');
  const auto commands = send_angular_first_
    ? std::array<std::vector<uint8_t>, 2>{angular_command, linear_command}
    : std::array<std::vector<uint8_t>, 2>{linear_command, angular_command};

  for (const auto & command : commands) {
    std_msgs::msg::UInt8MultiArray msg;
    msg.data = command;
    serial_write_publisher_->publish(msg);
  }

  if (output_mode_ == OutputMode::Topic) {
    return true;
  }

  if (!openSerialIfNeeded()) {
    return false;
  }

  for (const auto & command : commands) {
    if (!writeCommand(command)) {
      return false;
    }

    if (tcdrain(serial_fd_) != 0) {
      RCLCPP_WARN(get_logger(), "Failed to drain Cuadriga serial port %s: %s", serial_port_.c_str(), std::strerror(errno));
    }

    if (inter_command_delay_ms_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(inter_command_delay_ms_));
    }
  }
  return true;
}

int CuadrigaActuatorNode::velocityToPwm(double value, double max_value) const
{
  return scaleToPwm(value, max_value);
}

int CuadrigaActuatorNode::angularToPwm(double value, double max_value) const
{
  const double signed_value = value >= 0.0 ? -std::abs(value) : std::abs(value);
  return scaleToPwm(signed_value, max_value);
}

int CuadrigaActuatorNode::scaleToPwm(double value, double max_value) const
{
  if (max_value <= std::numeric_limits<double>::epsilon()) {
    return 0;
  }

  const double clamped = std::clamp(value, -max_value, max_value);
  const double normalized = std::abs(clamped) / max_value;
  int pwm_value = static_cast<int>(std::lround(normalized * static_cast<double>(pwm_limit_)));
  pwm_value = std::clamp(pwm_value, 0, pwm_limit_);
  return clamped < 0.0 ? -pwm_value : pwm_value;
}

std::vector<uint8_t> CuadrigaActuatorNode::encodePwmCommand(
  int pwm_value,
  char positive_prefix,
  char negative_prefix) const
{
  const char prefix = pwm_value < 0 ? negative_prefix : positive_prefix;
  const int magnitude = std::clamp(std::abs(pwm_value), 0, pwm_limit_);
  char buffer[6] = {0};
  std::snprintf(buffer, sizeof(buffer), "!%c%02X\r", prefix, magnitude);
  return std::vector<uint8_t>(buffer, buffer + std::strlen(buffer));
}

std::string CuadrigaActuatorNode::formatCommandForLog(const std::vector<uint8_t> & command) const
{
  std::string formatted;
  formatted.reserve(command.size() * 4);

  for (const auto byte : command) {
    if (byte == '\r') {
      formatted += "\\r";
    } else if (byte >= 32 && byte <= 126) {
      formatted.push_back(static_cast<char>(byte));
    } else {
      char hex_buffer[5] = {0};
      std::snprintf(hex_buffer, sizeof(hex_buffer), "\\x%02X", byte);
      formatted += hex_buffer;
    }
  }

  return formatted;
}

bool CuadrigaActuatorNode::openSerialIfNeeded()
{
  if (serial_fd_ >= 0) {
    return true;
  }

  serial_fd_ = ::open(serial_port_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (serial_fd_ < 0) {
    RCLCPP_ERROR(get_logger(), "Failed to open serial port %s: %s", serial_port_.c_str(), std::strerror(errno));
    return false;
  }

  struct termios tty;
  std::memset(&tty, 0, sizeof(tty));
  if (tcgetattr(serial_fd_, &tty) != 0) {
    RCLCPP_ERROR(get_logger(), "Failed to read serial config for %s: %s", serial_port_.c_str(), std::strerror(errno));
    closeSerial();
    return false;
  }

  cfsetospeed(&tty, toBaudRate(baud_rate_));
  cfsetispeed(&tty, toBaudRate(baud_rate_));

  cfmakeraw(&tty);

  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= serial_bytesize_ == 8 ? CS8 : CS7;
  tty.c_cflag |= CLOCAL | CREAD;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag &= ~CSTOPB;
  if (serial_stop_bits_ == 2) {
    tty.c_cflag |= CSTOPB;
  }

  tty.c_cflag &= ~(PARENB | PARODD);
  if (serial_parity_ == 'E') {
    tty.c_cflag |= PARENB;
  } else if (serial_parity_ == 'O') {
    tty.c_cflag |= PARENB | PARODD;
  }

  tty.c_iflag = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 1;

  if (tcsetattr(serial_fd_, TCSANOW, &tty) != 0) {
    RCLCPP_ERROR(get_logger(), "Failed to configure serial port %s: %s", serial_port_.c_str(), std::strerror(errno));
    closeSerial();
    return false;
  }

  RCLCPP_INFO(get_logger(), "Opened Cuadriga serial port %s at %d baud", serial_port_.c_str(), baud_rate_);
  return true;
}

void CuadrigaActuatorNode::closeSerial()
{
  if (serial_fd_ >= 0) {
    ::close(serial_fd_);
    serial_fd_ = -1;
  }
}

bool CuadrigaActuatorNode::writeCommand(const std::vector<uint8_t> & command)
{
  if (serial_fd_ < 0) {
    return false;
  }

  if (log_serial_commands_) {
    RCLCPP_INFO(get_logger(), "Cuadriga serial TX: %s", formatCommandForLog(command).c_str());
  }

  const ssize_t written = ::write(serial_fd_, command.data(), command.size());
  if (written < 0 || static_cast<size_t>(written) != command.size()) {
    RCLCPP_ERROR(get_logger(), "Failed to write Cuadriga command to serial: %s", std::strerror(errno));
    closeSerial();
    return false;
  }
  return true;
}

}  // namespace cuadriga_actuator