/**
 * @file follow_zed_node.cpp
 * @brief Lifecycle state node that bridges "Follow a person" behavior driven by a Jetson/ZED.
 *
 * ## Why this exists
 * The cuadriga stack can run a vision-based follower on a Jetson (ZED camera). This ROS 2
 * LifecycleNode plays two roles:
 * 1) A simple TCP client to send start/stop/ping commands to the Jetson process.
 * 2) A bridge between Jetson outputs (leader distance, emergency, enable, leader id) and the
 *    rest of the ROS control pipeline (mainly `CtrlNode`).
 *
 * The mission/orchestrator activates/deactivates this node according to the global FSM.
 *
 * ## ROS contract (topics / services / params)
 * ### TCP (Jetson)
 * Controlled by parameters:
 * - `jetson_host`, `jetson_port`
 * - `cmd_start`, `cmd_stop`, `cmd_ping`
 * - `ping_period_s`
 *
 * ### Subscriptions (inputs)
 * - `leader_distance_topic_in` [std_msgs/msg/Float32] distance to the tracked leader (meters)
 * - `follow_enabled_in_topic`  [std_msgs/msg/Bool] external enable switch
 * - `emergency_in_topic`       [std_msgs/msg/Bool] emergency flag from vision system
 * - `leader_id_in_topic`       [std_msgs/msg/Int32] id of the tracked person/leader
 * - `follow_cmd_topic_name`    [geometry_msgs/msg/Twist] external velocity command for follow-me
 * - Gap tuning:
 *   - `gap_incdec_topic` [std_msgs/msg/Int8] discrete gap +/- requests
 *   - `gap_delta_topic`  [std_msgs/msg/Float32] continuous delta to add to gap
 *   - `gap_set_topic`    [std_msgs/msg/Float32] absolute gap set
 * - Optional video relay:
 *   - `video_in_topic` [sensor_msgs/msg/Image]
 *
 * ### Publications (outputs)
 * - `dist_last_obj_topic_out` [std_msgs/msg/Float32]
 *     Distance-to-target published towards the controller layer.
 * - `leader_distance_state_topic` [std_msgs/msg/Float32]
 *     Filtered distance from robot to tracked person, without subtracting the configured gap.
 * - `follow_state_topic` [std_msgs/msg/Bool] (latched)
 *     True when internal FSM is in FOLLOWING.
 * - `secured_cmd_vel_topic_name` [geometry_msgs/msg/Twist]
 *     Forwarded velocity command, but only while the FSM is FOLLOWING and the leader is fresh.
 * - `gap_state_topic` [std_msgs/msg/Float32] (latched)
 *     Current follow gap in meters.
 * - Optional video relay:
 *   - `video_out_topic` [sensor_msgs/msg/Image]
 *
 * ### Services
 * - `set_follow_enabled` [std_srvs/srv/SetBool]
 *     Compatibility helper for GUIs / tools to enable/disable following.
 *
 * ## Internal state machine (inside this node)
 * This file defines a small local FSM (`State::{IDLE, FOLLOWING, EMERGENCY}`) that is
 * independent from the global mission FSM. It decides whether to issue START/STOP commands
 * to the Jetson and what to publish to the rest of the stack.
 *
 * ## Lifecycle behavior
 * - on_configure(): declares/reads parameters and sets up pubs/subs/services.
 * - on_activate(): activates lifecycle publishers, starts timers, connects TCP.
 * - on_deactivate(): stops timers, closes TCP, deactivates publishers.
 *
 * @note The top of this file contains commented legacy code kept for reference.
 */

// /////////////////////////////////////////
// /// Placeholder for FollowZED.Cpp
// //////////////////////////////////////////////


// #include "ctl_mission/FollowZED.hpp"
// #include <chrono>
// using namespace std::chrono_literals;

// /* ---------------- TCP CLIENT ---------------- */

// bool TcpClient::connect(const std::string& host, int port) {
//   close();
//   sock_ = ::socket(AF_INET, SOCK_STREAM, 0);
//   if (sock_ < 0) return false;

//   sockaddr_in addr{};
//   addr.sin_family = AF_INET;
//   addr.sin_port = htons(static_cast<uint16_t>(port));
//   if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
//     ::close(sock_);
//     sock_ = -1;
//     return false;
//   }
//   if (::connect(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
//     ::close(sock_);
//     sock_ = -1;
//     return false;
//   }
//   running_ = true;
//   connected_ = true;
//   reader_ = std::thread(&TcpClient::reader_loop, this);
//   return true;
// }

// void TcpClient::close() {
//   running_ = false;
//   if (sock_ >= 0) {
//     ::shutdown(sock_, SHUT_RDWR);
//     ::close(sock_);
//     sock_ = -1;
//   }
//   if (reader_.joinable()) reader_.join();
//   connected_ = false;
// }

// bool TcpClient::send_line(const std::string& line) {
//   std::lock_guard<std::mutex> lk(send_mtx_);
//   if (sock_ < 0) return false;
//   const char* data = line.c_str();
//   size_t left = line.size();
//   while (left > 0) {
//     ssize_t n = ::send(sock_, data, left, 0);
//     if (n <= 0) return false;
//     data += n;
//     left -= static_cast<size_t>(n);
//   }
//   return true;
// }

// void TcpClient::reader_loop() {
//   std::string buf;
//   buf.reserve(4096);
//   char tmp[1024];
//   while (running_) {
//     ssize_t n = ::recv(sock_, tmp, sizeof(tmp), 0);
//     if (n <= 0) break;
//     buf.append(tmp, tmp + n);
//     // linhas por '\n'
//     size_t pos;
//     while ((pos = buf.find('\n')) != std::string::npos) {
//       std::string line = buf.substr(0, pos);
//       if (on_line_) on_line_(line);
//       buf.erase(0, pos + 1);
//     }
//   }
//   connected_ = false;
//   running_ = false;
// }

// /* -------------- FOLLOW ZED NODE ------------- */

// FollowZEDNode::FollowZEDNode(const std::string& name)
// : rclcpp_lifecycle::LifecycleNode(name) {
//   // Declara parámetros con defaults
//   this->declare_parameter<std::string>("jetson.host", "192.168.1.14");
//   this->declare_parameter<int>("jetson.port", 5000);
//   this->declare_parameter<std::string>("commands.start", "START_FOLLOW\n");
//   this->declare_parameter<std::string>("commands.stop",  "STOP_FOLLOW\n");
//   this->declare_parameter<std::string>("commands.ping",  "PING\n");
//   this->declare_parameter<bool>("relay_video", true);
//   this->declare_parameter<std::string>("video_in_topic", "/zed/person_follow/image_raw");
//   this->declare_parameter<std::string>("video_out_topic","/follow_zed/image_for_gui");
//   this->declare_parameter<double>("ping_period_s", 2.0);
// }

// auto FollowZEDNode::on_configure(const rclcpp_lifecycle::State&)
// -> rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn {
//   this->get_parameter("jetson.host", jetson_host_);
//   this->get_parameter("jetson.port", jetson_port_);
//   this->get_parameter("commands.start", cmd_start_);
//   this->get_parameter("commands.stop",  cmd_stop_);
//   this->get_parameter("commands.ping",  cmd_ping_);
//   this->get_parameter("relay_video", relay_video_);
//   this->get_parameter("video_in_topic", video_in_topic_);
//   this->get_parameter("video_out_topic", video_out_topic_);
//   this->get_parameter("ping_period_s", ping_period_s_);

//   RCLCPP_INFO(get_logger(), "FollowZED configured. Jetson %s:%d, relay_video=%s",
//               jetson_host_.c_str(), jetson_port_, relay_video_ ? "true" : "false");

//   // Publisher (lifecycle) para la GUI si hacemos relay
//   if (relay_video_) {
//     img_pub_ = this->create_publisher<sensor_msgs::msg::Image>(video_out_topic_, rclcpp::SensorDataQoS());
//     // Sub al tópico que publica la Jetson (si está en mismo dominio DDS aparecerá)
//     img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
//       video_in_topic_, rclcpp::SensorDataQoS(),
//       std::bind(&FollowZEDNode::on_image, this, std::placeholders::_1));
//   }
//   // Callback de líneas TCP desde Jetson
//   client_.set_on_line([this](const std::string& line){ this->on_tcp_line(line); });

//   return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
// }

// auto FollowZEDNode::on_activate(const rclcpp_lifecycle::State&)
// -> rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn {
//   RCLCPP_INFO(get_logger(), "Activating FollowZED: connecting TCP + START");
//   if (relay_video_ && img_pub_) img_pub_->on_activate();

//   start_tcp();
//   if (client_.is_connected()) {
//     client_.send_line(cmd_start_);
//   }

//   // Timer de keepalive/ping
//   ping_timer_ = this->create_wall_timer(
//     std::chrono::duration<double>(ping_period_s_),
//     [this](){
//       if (client_.is_connected() && !cmd_ping_.empty()) {
//         client_.send_line(cmd_ping_);
//       }
//     });

//   return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
// }

// auto FollowZEDNode::on_deactivate(const rclcpp_lifecycle::State&)
// -> rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn {
//   RCLCPP_INFO(get_logger(), "Deactivating FollowZED: STOP + close TCP");
//   if (ping_timer_) ping_timer_->cancel(), ping_timer_.reset();

//   if (client_.is_connected()) {
//     client_.send_line(cmd_stop_);
//     // breve pausa para que se envíe
//     std::this_thread::sleep_for(100ms);
//   }
//   stop_tcp();

//   if (relay_video_ && img_pub_) img_pub_->on_deactivate();

//   return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
// }

// auto FollowZEDNode::on_cleanup(const rclcpp_lifecycle::State&)
// -> rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn {
//   RCLCPP_INFO(get_logger(), "Cleanup FollowZED");
//   stop_tcp();
//   img_sub_.reset();
//   img_pub_.reset();
//   return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
// }

// /* ----------- helpers & callbacks ---------- */

// void FollowZEDNode::start_tcp() {
//   if (!client_.is_connected()) {
//     if (!client_.connect(jetson_host_, jetson_port_)) {
//       RCLCPP_ERROR(get_logger(), "TCP connect failed to %s:%d", jetson_host_.c_str(), jetson_port_);
//     } else {
//       RCLCPP_INFO(get_logger(), "TCP connected to %s:%d", jetson_host_.c_str(), jetson_port_);
//     }
//   }
// }

// void FollowZEDNode::stop_tcp() { client_.close(); }

// void FollowZEDNode::on_image(const sensor_msgs::msg::Image::SharedPtr msg) {
//   if (relay_video_ && img_pub_ && img_pub_->is_activated()) {
//     img_pub_->publish(*msg); // relay 1:1 para la GUI
//   }
// }

// void FollowZEDNode::on_tcp_line(const std::string& line) {
//   // Aquí parseas eventos/telemetría de la Jetson si envías JSON/CSV por TCP
//   // Ejemplos:
//   //  "STATUS:OK"
//   //  "TARGET:x,y,conf"
//   RCLCPP_DEBUG(get_logger(), "[Jetson] %s", line.c_str());
//   // TODO: publicar tópicos/diagnósticos si te interesa
// }
// // Placeholder para el final del archivo
// //////////////////////////////////////////////////////////////////////////////////
// follow_zed_node.cpp (SIN main())
// Implementa la clase FollowZEDNode que usa follow_zed_main.cpp
#include "ctl_mission/FollowZED.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

/* =============================== TcpClient =============================== */
bool TcpClient::connect(const std::string& host, int port) {
  close();
  sock_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ < 0) return false;

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(static_cast<uint16_t>(port));
  if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
    ::close(sock_);
    sock_ = -1;
    return false;
  }
  if (::connect(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    ::close(sock_);
    sock_ = -1;
    return false;
  }
  connected_ = true;
  running_   = true;
  reader_    = std::thread(&TcpClient::reader_loop, this);
  return true;
}

void TcpClient::close() {
  running_ = false;
  if (sock_ >= 0) {
    ::shutdown(sock_, SHUT_RDWR);
    ::close(sock_);
    sock_ = -1;
  }
  if (reader_.joinable()) reader_.join();
  connected_ = false;
}

bool TcpClient::send_line(const std::string& line) {
  if (!connected_) return false;
  std::lock_guard<std::mutex> lk(send_mtx_);
  const char* data = line.c_str();
  size_t left = line.size();
  while (left > 0) {
    ssize_t n = ::send(sock_, data, left, 0);
    if (n <= 0) {
      connected_ = false;
      return false;
    }
    data += n;
    left -= static_cast<size_t>(n);
  }
  return true;
}

void TcpClient::reader_loop() {
  std::string buf;
  buf.reserve(1024);
  char tmp[512];
  while (running_) {
    ssize_t n = ::recv(sock_, tmp, sizeof(tmp), 0);
    if (n <= 0) {
      connected_ = false;
      break;
    }
    buf.append(tmp, tmp + n);
    size_t pos;
    while ((pos = buf.find('\n')) != std::string::npos) {
      std::string line = buf.substr(0, pos);
      if (on_line_) on_line_(line);
      buf.erase(0, pos + 1);
    }
  }
}

/* ============================ FollowZEDNode ============================== */
using rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface;
using CallbackReturn = LifecycleNodeInterface::CallbackReturn;

FollowZEDNode::FollowZEDNode(const std::string& name)
: rclcpp_lifecycle::LifecycleNode(name)
{
  // Nada aquí; todo en on_configure()
}

/* ---------- Helpers FSM ---------- */
const char* FollowZEDNode::state_name(State s) {
  switch (s) {
    case State::IDLE:      return "IDLE";
    case State::FOLLOWING: return "FOLLOWING";
    case State::EMERGENCY: return "EMERGENCY";
  }
  return "?";
}

void FollowZEDNode::set_state(State s, const char* reason) {
  if (s == state_) return;
  auto prev = state_;
  state_ = s;

  // Publicar estado FOLLOW (true solo en FOLLOWING)
  if (follow_state_pub_) {
    std_msgs::msg::Bool st;
    st.data = (state_ == State::FOLLOWING);
    follow_state_pub_->publish(st);
  }

  // Comandos TCP a Jetson
  if (client_.is_connected()) {
    if (state_ == State::FOLLOWING) client_.send_line(cmd_start_);
    else                             client_.send_line(cmd_stop_);
  }

  if (state_ != State::FOLLOWING) {
    publish_stop_cmd();
    last_motion_cmd_allowed_ = false;
  }

  RCLCPP_WARN(this->get_logger(), "[FSM] %s → %s (%s)",
              state_name(prev), state_name(state_), reason);
}

/* ---------- Lifecycle: CONFIGURE ---------- */
CallbackReturn FollowZEDNode::on_configure(const rclcpp_lifecycle::State&) {
  using std::placeholders::_1;

  // ----------- Parámetros TCP/Video -----------
  this->declare_parameter<std::string>("jetson_host", jetson_host_);
  this->declare_parameter<int>("jetson_port", jetson_port_);
  this->declare_parameter<std::string>("cmd_start", cmd_start_);
  this->declare_parameter<std::string>("cmd_stop",  cmd_stop_);
  this->declare_parameter<std::string>("cmd_ping",  cmd_ping_);
  this->declare_parameter<double>("ping_period_s",  ping_period_s_);
  this->declare_parameter<bool>("auto_follow_on_activate", auto_follow_on_activate_);

  this->declare_parameter<bool>("relay_video",        relay_video_);
  this->declare_parameter<std::string>("video_in_topic",  video_in_topic_);
  this->declare_parameter<std::string>("video_out_topic", video_out_topic_);

  // ----------- Parámetros Follow/GAP -----------
  this->declare_parameter<std::string>("leader_distance_topic_in", leader_distance_topic_in_);
  this->declare_parameter<std::string>("follow_enabled_in_topic",  follow_enabled_in_topic_);
  this->declare_parameter<std::string>("emergency_in_topic",       emergency_in_topic_);
  this->declare_parameter<std::string>("leader_id_in_topic",       leader_id_in_topic_);
  this->declare_parameter<std::string>("leader_distance_state_topic", leader_distance_state_topic_);
  this->declare_parameter<std::string>("follow_cmd_topic_name", follow_cmd_topic_name_);
  this->declare_parameter<std::string>("secured_cmd_vel_topic_name", secured_cmd_vel_topic_name_);
  this->declare_parameter<int>("cmd_vel_queue_size", cmd_vel_queue_size_);

  this->declare_parameter<std::string>("dist_last_obj_topic_out",  dist_last_obj_topic_out_);
  this->declare_parameter<std::string>("follow_state_topic",       follow_state_topic_);
  this->declare_parameter<std::string>("gap_incdec_topic",         gap_incdec_topic_);
  this->declare_parameter<std::string>("gap_delta_topic",          gap_delta_topic_);
  this->declare_parameter<std::string>("gap_set_topic",            gap_set_topic_);
  this->declare_parameter<std::string>("gap_state_topic",          gap_state_topic_);

  this->declare_parameter<double>("leader_distance_ema_alpha", ema_alpha_);
  this->declare_parameter<int>("leader_distance_lost_timeout_ms", lost_timeout_ms_);
  this->declare_parameter<std::string>("leader_distance_publish_on_lost", publish_on_lost_);
  this->declare_parameter<double>("publish_rate_hz", publish_rate_hz_);

  this->declare_parameter<double>("follow_gap_m", follow_gap_m_);
  this->declare_parameter<double>("gap_min_m",   gap_min_m_);
  this->declare_parameter<double>("gap_max_m",   gap_max_m_);
  this->declare_parameter<double>("gap_step_m",  gap_step_m_);

  // Leer params (sobrescriben defaults)
  this->get_parameter("jetson_host", jetson_host_);
  this->get_parameter("jetson_port", jetson_port_);
  this->get_parameter("cmd_start",   cmd_start_);
  this->get_parameter("cmd_stop",    cmd_stop_);
  this->get_parameter("cmd_ping",    cmd_ping_);
  this->get_parameter("ping_period_s", ping_period_s_);
  this->get_parameter("auto_follow_on_activate", auto_follow_on_activate_);

  this->get_parameter("relay_video",       relay_video_);
  this->get_parameter("video_in_topic",    video_in_topic_);
  this->get_parameter("video_out_topic",   video_out_topic_);

  this->get_parameter("leader_distance_topic_in", leader_distance_topic_in_);
  this->get_parameter("follow_enabled_in_topic",  follow_enabled_in_topic_);
  this->get_parameter("emergency_in_topic",       emergency_in_topic_);
  this->get_parameter("leader_id_in_topic",       leader_id_in_topic_);
  this->get_parameter("leader_distance_state_topic", leader_distance_state_topic_);
  this->get_parameter("follow_cmd_topic_name", follow_cmd_topic_name_);
  this->get_parameter("secured_cmd_vel_topic_name", secured_cmd_vel_topic_name_);
  this->get_parameter("cmd_vel_queue_size", cmd_vel_queue_size_);

  this->get_parameter("dist_last_obj_topic_out",  dist_last_obj_topic_out_);
  this->get_parameter("follow_state_topic",       follow_state_topic_);
  this->get_parameter("gap_incdec_topic",         gap_incdec_topic_);
  this->get_parameter("gap_delta_topic",          gap_delta_topic_);
  this->get_parameter("gap_set_topic",            gap_set_topic_);
  this->get_parameter("gap_state_topic",          gap_state_topic_);

  this->get_parameter("leader_distance_ema_alpha", ema_alpha_);
  this->get_parameter("leader_distance_lost_timeout_ms", lost_timeout_ms_);
  this->get_parameter("leader_distance_publish_on_lost", publish_on_lost_);
  this->get_parameter("publish_rate_hz", publish_rate_hz_);

  this->get_parameter("follow_gap_m", follow_gap_m_);
  this->get_parameter("gap_min_m",   gap_min_m_);
  this->get_parameter("gap_max_m",   gap_max_m_);
  this->get_parameter("gap_step_m",  gap_step_m_);

  // ----------- Publishers (Lifecycle) -----------
  // Overlay vídeo (opcional)
  if (relay_video_) {
    img_pub_ = this->create_publisher<sensor_msgs::msg::Image>(
      video_out_topic_, rclcpp::SensorDataQoS());
  }

  // Bridge a CtrlNode
  dist_last_obj_pub_ = this->create_publisher<std_msgs::msg::Float32>(
      dist_last_obj_topic_out_, rclcpp::QoS(10));

  // Estados latched (transient local)
  auto latched = rclcpp::QoS(rclcpp::KeepLast(1)).transient_local();
  leader_distance_state_pub_ = this->create_publisher<std_msgs::msg::Float32>(
      leader_distance_state_topic_, latched);
  follow_state_pub_ = this->create_publisher<std_msgs::msg::Bool>(
      follow_state_topic_, latched);
  secured_cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
      secured_cmd_vel_topic_name_, rclcpp::QoS(std::max(1, cmd_vel_queue_size_)));
  gap_state_pub_ = this->create_publisher<std_msgs::msg::Float32>(
      gap_state_topic_, latched);

  // ----------- Subscribers -----------
  if (relay_video_) {
    img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      video_in_topic_, rclcpp::SensorDataQoS(),
      std::bind(&FollowZEDNode::on_image, this, _1));
  }

  leader_dist_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      leader_distance_topic_in_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_leader_distance, this, _1));

  follow_enabled_in_sub_ = this->create_subscription<std_msgs::msg::Bool>(
      follow_enabled_in_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_follow_enabled_in, this, _1));

  emergency_in_sub_ = this->create_subscription<std_msgs::msg::Bool>(
      emergency_in_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_emergency_in, this, _1));

  leader_id_in_sub_ = this->create_subscription<std_msgs::msg::Int32>(
      leader_id_in_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_leader_id_in, this, _1));

    follow_cmd_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      follow_cmd_topic_name_, rclcpp::QoS(std::max(1, cmd_vel_queue_size_)),
      std::bind(&FollowZEDNode::on_follow_cmd, this, _1));

  gap_incdec_sub_ = this->create_subscription<std_msgs::msg::Int8>(
      gap_incdec_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_gap_incdec, this, _1));

  gap_delta_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      gap_delta_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_gap_delta, this, _1));

  gap_set_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      gap_set_topic_, rclcpp::QoS(10),
      std::bind(&FollowZEDNode::on_gap_set, this, _1));

  // Servicio compatibilidad GUI
  set_follow_srv_ = this->create_service<std_srvs::srv::SetBool>(
      "set_follow_enabled",
      std::bind(&FollowZEDNode::on_set_follow_enabled, this,
                std::placeholders::_1, std::placeholders::_2));

  // Estado inicial
  last_filtered_dist_ = std::numeric_limits<float>::quiet_NaN();
  last_raw_dist_      = std::numeric_limits<float>::quiet_NaN();
  last_msg_time_      = rclcpp::Time(0, 0, this->get_clock()->get_clock_type());
  state_              = State::IDLE;
  last_motion_cmd_allowed_ = false;

  // Callback TCP (recibir líneas)
  client_.set_on_line(std::bind(&FollowZEDNode::on_tcp_line, this, std::placeholders::_1));

  // Publicar gap al arrancar
  publish_gap_state();

  RCLCPP_INFO(this->get_logger(),
    "FollowZED configured. Jetson %s:%d  relay_video=%s  in='%s'  out='%s'  leader_state='%s'  cmd_in='%s'  cmd_out='%s'  gap=%.2fm",
    jetson_host_.c_str(), jetson_port_, relay_video_ ? "ON" : "OFF",
    leader_distance_topic_in_.c_str(), dist_last_obj_topic_out_.c_str(),
    leader_distance_state_topic_.c_str(), follow_cmd_topic_name_.c_str(),
    secured_cmd_vel_topic_name_.c_str(), follow_gap_m_);

  return CallbackReturn::SUCCESS;
}

/* ---------- Lifecycle: ACTIVATE ---------- */
CallbackReturn FollowZEDNode::on_activate(const rclcpp_lifecycle::State&) {
  if (img_pub_)           img_pub_->on_activate();
  if (dist_last_obj_pub_) dist_last_obj_pub_->on_activate();
  if (leader_distance_state_pub_) leader_distance_state_pub_->on_activate();
  if (follow_state_pub_)  follow_state_pub_->on_activate();
  if (secured_cmd_vel_pub_) secured_cmd_vel_pub_->on_activate();
  if (gap_state_pub_)     gap_state_pub_->on_activate();

  state_ = auto_follow_on_activate_ ? State::FOLLOWING : State::IDLE;
  last_motion_cmd_allowed_ = false;

  // Timer de ping TCP
  ping_timer_ = this->create_wall_timer(
      std::chrono::duration<double>(std::max(0.25, ping_period_s_)),
      std::bind(&FollowZEDNode::ping_tick, this));

  // Timer principal (bridge FSM)
  const double hz = std::max(2.0, publish_rate_hz_);
  tick_timer_ = this->create_wall_timer(
      std::chrono::duration<double>(1.0 / hz),
      std::bind(&FollowZEDNode::tick, this));

  // Conectar TCP
  start_tcp();

  // Publicar estado actual
  if (follow_state_pub_) {
    std_msgs::msg::Bool st;
    st.data = (state_ == State::FOLLOWING);
    follow_state_pub_->publish(st);
  }
  publish_stop_cmd();

  RCLCPP_INFO(this->get_logger(), "FollowZED activated. initial_state=%s auto_follow_on_activate=%s",
              state_name(state_), auto_follow_on_activate_ ? "true" : "false");
  return CallbackReturn::SUCCESS;
}

/* ---------- Lifecycle: DEACTIVATE ---------- */
CallbackReturn FollowZEDNode::on_deactivate(const rclcpp_lifecycle::State&) {
  // Timers
  ping_timer_.reset();
  tick_timer_.reset();

  // Enviar STOP al salir del modo antes de cerrar TCP
  if (client_.is_connected()) {
    if (state_ != State::IDLE) set_state(State::IDLE, "lifecycle_deactivate");
    else                       client_.send_line(cmd_stop_);
  }
  publish_stop_cmd();
  last_motion_cmd_allowed_ = false;

  // TCP
  stop_tcp();

  if (img_pub_)           img_pub_->on_deactivate();
  if (dist_last_obj_pub_) dist_last_obj_pub_->on_deactivate();
  if (leader_distance_state_pub_) leader_distance_state_pub_->on_deactivate();
  if (follow_state_pub_)  follow_state_pub_->on_deactivate();
  if (secured_cmd_vel_pub_) secured_cmd_vel_pub_->on_deactivate();
  if (gap_state_pub_)     gap_state_pub_->on_deactivate();

  RCLCPP_INFO(this->get_logger(), "FollowZED deactivated.");
  return CallbackReturn::SUCCESS;
}

/* ---------- Lifecycle: CLEANUP ---------- */
CallbackReturn FollowZEDNode::on_cleanup(const rclcpp_lifecycle::State&) {
  img_pub_.reset();
  dist_last_obj_pub_.reset();
  leader_distance_state_pub_.reset();
  follow_state_pub_.reset();
  secured_cmd_vel_pub_.reset();
  gap_state_pub_.reset();

  img_sub_.reset();
  leader_dist_sub_.reset();
  follow_enabled_in_sub_.reset();
  emergency_in_sub_.reset();
  leader_id_in_sub_.reset();
  follow_cmd_sub_.reset();
  gap_incdec_sub_.reset();
  gap_delta_sub_.reset();
  gap_set_sub_.reset();

  set_follow_srv_.reset();

  RCLCPP_INFO(this->get_logger(), "FollowZED cleaned up.");
  return CallbackReturn::SUCCESS;
}

/* ---------------------------- ROS Callbacks ----------------------------- */
void FollowZEDNode::on_image(const sensor_msgs::msg::Image::SharedPtr msg) {
  if (!img_pub_) return;
  img_pub_->publish(*msg);
}

void FollowZEDNode::on_leader_distance(const std_msgs::msg::Float32::SharedPtr msg) {
  const float d = msg->data;
  if (!std::isfinite(d) || d < 0.0f) {
    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                         "[follow_zed] Distancia inválida: %f", d);
    return;
  }

  last_raw_dist_ = d;

  // EMA
  if (!std::isfinite(last_filtered_dist_)) {
    last_filtered_dist_ = d;
  } else {
    const double a = std::clamp(ema_alpha_, 0.0, 1.0);
    last_filtered_dist_ = static_cast<float>(a * d + (1.0 - a) * last_filtered_dist_);
  }

  last_msg_time_ = this->get_clock()->now();
}

void FollowZEDNode::on_follow_enabled_in(const std_msgs::msg::Bool::SharedPtr msg) {
  if (msg->data) {
    if (state_ == State::EMERGENCY) set_state(State::FOLLOWING, "resume_from_emergency");
    else                            set_state(State::FOLLOWING, "follow_enabled_in");
  } else {
    if (state_ != State::EMERGENCY) set_state(State::IDLE, "follow_disabled_in");
  }
}

void FollowZEDNode::on_emergency_in(const std_msgs::msg::Bool::SharedPtr msg) {
  if (msg->data) set_state(State::EMERGENCY, "emergency_in");
}

void FollowZEDNode::on_leader_id_in(const std_msgs::msg::Int32::SharedPtr msg) {
  last_leader_id_ = msg->data;
  RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 3000,
                       "[follow_zed] leader_id=%d", last_leader_id_);
}

void FollowZEDNode::on_follow_cmd(const geometry_msgs::msg::Twist::SharedPtr msg) {
  if (!secured_cmd_vel_pub_) {
    return;
  }

  const bool can_forward =
    state_ == State::FOLLOWING && has_fresh_leader_sample(this->get_clock()->now());

  if (!can_forward) {
    if (last_motion_cmd_allowed_) {
      publish_stop_cmd();
      last_motion_cmd_allowed_ = false;
    }
    return;
  }

  secured_cmd_vel_pub_->publish(*msg);
  last_motion_cmd_allowed_ = true;
}

/* GAP */
void FollowZEDNode::on_gap_incdec(const std_msgs::msg::Int8::SharedPtr msg) {
  int step = static_cast<int>(msg->data);
  if (step == 0) return;
  follow_gap_m_ = std::clamp(follow_gap_m_ + step * gap_step_m_, gap_min_m_, gap_max_m_);
  publish_gap_state();
  RCLCPP_INFO(this->get_logger(), "[GAP] incdec=%d → gap=%.2fm", step, follow_gap_m_);
}

void FollowZEDNode::on_gap_delta(const std_msgs::msg::Float32::SharedPtr msg) {
  if (!std::isfinite(msg->data)) return;
  follow_gap_m_ = std::clamp(follow_gap_m_ + static_cast<double>(msg->data), gap_min_m_, gap_max_m_);
  publish_gap_state();
  RCLCPP_INFO(this->get_logger(), "[GAP] delta=%.2f → gap=%.2fm", msg->data, follow_gap_m_);
}

void FollowZEDNode::on_gap_set(const std_msgs::msg::Float32::SharedPtr msg) {
  if (!std::isfinite(msg->data)) return;
  follow_gap_m_ = std::clamp(static_cast<double>(msg->data), gap_min_m_, gap_max_m_);
  publish_gap_state();
  RCLCPP_INFO(this->get_logger(), "[GAP] set=%.2f → gap=%.2fm", msg->data, follow_gap_m_);
}

void FollowZEDNode::publish_gap_state() {
  if (!gap_state_pub_) return;
  std_msgs::msg::Float32 msg;
  msg.data = static_cast<float>(follow_gap_m_);
  gap_state_pub_->publish(msg);
}

bool FollowZEDNode::has_fresh_leader_sample(const rclcpp::Time& now) const {
  if (last_msg_time_.nanoseconds() <= 0 || !std::isfinite(last_filtered_dist_)) {
    return false;
  }

  const auto age_ms = static_cast<int64_t>((now - last_msg_time_).nanoseconds() / 1000000);
  return age_ms <= lost_timeout_ms_;
}

void FollowZEDNode::publish_stop_cmd() {
  if (!secured_cmd_vel_pub_) {
    return;
  }

  geometry_msgs::msg::Twist stop_cmd;
  secured_cmd_vel_pub_->publish(stop_cmd);
}

/* Servicio compatibilidad GUI */
void FollowZEDNode::on_set_follow_enabled(
  const std::shared_ptr<std_srvs::srv::SetBool::Request> req,
  std::shared_ptr<std_srvs::srv::SetBool::Response> res)
{
  if (req->data) {
    if (state_ == State::EMERGENCY) set_state(State::FOLLOWING, "service_resume_from_emergency");
    else                            set_state(State::FOLLOWING, "service_follow_on");
  } else {
    if (state_ != State::EMERGENCY) set_state(State::IDLE, "service_follow_off");
  }
  res->success = true;
  res->message = req->data ? "FOLLOW enabled" : "FOLLOW disabled";
}

/* ------------------------------ Timers ---------------------------------- */
void FollowZEDNode::ping_tick() {
  if (client_.is_connected()) {
    client_.send_line(cmd_ping_);
  }
}

void FollowZEDNode::tick() {
  if (!dist_last_obj_pub_) return;

  std_msgs::msg::Float32 out;
  std_msgs::msg::Float32 leader_distance_out;

  const rclcpp::Time now = this->get_clock()->now();
  const bool fresh = has_fresh_leader_sample(now);

  if (fresh && std::isfinite(last_filtered_dist_)) {
    leader_distance_out.data = last_filtered_dist_;
  } else if (publish_on_lost_ == "nan") {
    leader_distance_out.data = std::numeric_limits<float>::quiet_NaN();
  } else if (publish_on_lost_ == "hold") {
    leader_distance_out.data = std::isfinite(last_filtered_dist_) ?
      last_filtered_dist_ : std::numeric_limits<float>::quiet_NaN();
  } else {
    leader_distance_out.data = std::numeric_limits<float>::max();
  }

  if (leader_distance_state_pub_) {
    leader_distance_state_pub_->publish(leader_distance_out);
  }

  const bool motion_allowed = state_ == State::FOLLOWING && fresh && std::isfinite(last_filtered_dist_);
  if (!motion_allowed && last_motion_cmd_allowed_) {
    publish_stop_cmd();
    last_motion_cmd_allowed_ = false;
  }

  // EMERGENCIA: paro inmediato
  if (state_ == State::EMERGENCY) {
    out.data = 0.0f;
    dist_last_obj_pub_->publish(out);
    return;
  }

  if (state_ == State::FOLLOWING) {
    if (fresh && std::isfinite(last_filtered_dist_)) {
      float eff = last_filtered_dist_ - static_cast<float>(follow_gap_m_);
      if (!std::isfinite(eff)) eff = std::numeric_limits<float>::max();
      if (eff < 0.f) eff = 0.f; // no negativos
      out.data = eff;
    } else {
      // perdido: política
      if (publish_on_lost_ == "nan") {
        out.data = std::numeric_limits<float>::quiet_NaN();
      } else if (publish_on_lost_ == "hold") {
        out.data = std::isfinite(last_filtered_dist_) ?
                   last_filtered_dist_ : std::numeric_limits<float>::max();
      } else { // "max"
        out.data = std::numeric_limits<float>::max();
      }
    }
  } else { // IDLE
    out.data = std::numeric_limits<float>::max();
  }

  dist_last_obj_pub_->publish(out);
}

/* ------------------------------ TCP ------------------------------------- */
void FollowZEDNode::on_tcp_line(const std::string& line) {
  // Logs simples
  if (line == "PONG" || line == "OK") {
    RCLCPP_DEBUG(this->get_logger(), "[TCP] %s", line.c_str());
  } else {
    RCLCPP_INFO(this->get_logger(), "[TCP] %s", line.c_str());
  }
}

void FollowZEDNode::start_tcp() {
  if (client_.connect(jetson_host_, jetson_port_)) {
    RCLCPP_INFO(this->get_logger(), "TCP conectado a %s:%d", jetson_host_.c_str(), jetson_port_);
    // Sincroniza estado actual al conectar
    if (state_ == State::FOLLOWING) client_.send_line(cmd_start_);
    else                            client_.send_line(cmd_stop_);
  } else {
    RCLCPP_WARN(this->get_logger(), "No se pudo conectar TCP a %s:%d", jetson_host_.c_str(), jetson_port_);
  }
}

void FollowZEDNode::stop_tcp() {
  if (client_.is_connected()) {
    client_.close();
    RCLCPP_INFO(this->get_logger(), "TCP desconectado");
  }
}
