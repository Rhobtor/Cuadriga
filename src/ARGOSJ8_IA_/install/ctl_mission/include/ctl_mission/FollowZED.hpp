#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/int8.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_srvs/srv/set_bool.hpp>

// POSIX sockets (Linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* ---------------- TCP Client sencillo (línea por línea) ---------------- */
class TcpClient {
public:
  using LineCallback = std::function<void(const std::string&)>;
  TcpClient() = default;
  ~TcpClient() { close(); }

  bool connect(const std::string& host, int port);
  void close();
  bool send_line(const std::string& line);
  void set_on_line(LineCallback cb) { on_line_ = std::move(cb); }
  bool is_connected() const { return connected_; }

private:
  void reader_loop();

  int sock_ = -1;
  std::atomic<bool> running_{false};
  std::atomic<bool> connected_{false};
  std::thread reader_;
  LineCallback on_line_;
  std::mutex send_mtx_;
};

/* ----------------------- FollowZED Lifecycle Node ----------------------- */
class FollowZEDNode : public rclcpp_lifecycle::LifecycleNode {
public:
  explicit FollowZEDNode(const std::string& name);

  // Lifecycle
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State&) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State&) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State&) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State&) override;

private:
  /* --------- FSM --------- */
  enum class State { IDLE, FOLLOWING, EMERGENCY };
  void set_state(State s, const char* reason);
  static const char* state_name(State s);

  /* --------- Callbacks ROS --------- */
  void on_image(const sensor_msgs::msg::Image::SharedPtr msg);
  void on_leader_distance(const std_msgs::msg::Float32::SharedPtr msg);
  void on_follow_enabled_in(const std_msgs::msg::Bool::SharedPtr msg);
  void on_emergency_in(const std_msgs::msg::Bool::SharedPtr msg);
  void on_leader_id_in(const std_msgs::msg::Int32::SharedPtr msg);
  void on_follow_cmd(const geometry_msgs::msg::Twist::SharedPtr msg);
  void on_gap_incdec(const std_msgs::msg::Int8::SharedPtr msg);
  void on_gap_delta(const std_msgs::msg::Float32::SharedPtr msg);
  void on_gap_set(const std_msgs::msg::Float32::SharedPtr msg);
  void on_set_follow_enabled(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> req,
    std::shared_ptr<std_srvs::srv::SetBool::Response> res);

  /* --------- Timers --------- */
  void tick();           // publica dist_last_obj según FSM
  void ping_tick();      // envía PING por TCP si conectado

  /* --------- TCP --------- */
  void on_tcp_line(const std::string& line);
  void start_tcp();
  void stop_tcp();

  /* --------- Helpers --------- */
  void publish_gap_state();
  bool has_fresh_leader_sample(const rclcpp::Time& now) const;
  void publish_stop_cmd();

  /* ================== Parámetros generales (TCP / vídeo) ================== */
  std::string jetson_host_{"127.0.0.1"};
  int         jetson_port_{5000};
  std::string cmd_start_{"START_FOLLOW\n"};
  std::string cmd_stop_ {"STOP_FOLLOW\n"};
  std::string cmd_ping_ {"PING\n"};
  double      ping_period_s_{2.0};
  bool        auto_follow_on_activate_{true};

  bool relay_video_{true};
  std::string video_in_topic_{"/zed/person_follow/image_raw"};
  std::string video_out_topic_{"/follow_zed/image_for_gui"};

  /* ================== Parámetros de tópicos (seguimiento) ================= */
  std::string leader_distance_topic_in_{"/follow_zed/leader_distance_m"};
  std::string follow_enabled_in_topic_ {"/follow_zed/follow_enabled"};
  std::string emergency_in_topic_      {"/follow_zed/emergency_stop"};
  std::string leader_id_in_topic_      {"/follow_zed/leader_id"};
  std::string leader_distance_state_topic_{"/follow_zed/leader_distance_state_m"};
  std::string follow_cmd_topic_name_{"external_cmd_vel"};
  std::string secured_cmd_vel_topic_name_{"secured_cmd_vel"};
  int         cmd_vel_queue_size_{10};

  std::string dist_last_obj_topic_out_ {"dist_last_obj"};
  std::string follow_state_topic_      {"/follow_zed/follow_state"};
  std::string gap_incdec_topic_        {"/follow_zed/gap_incdec"};
  std::string gap_delta_topic_         {"/follow_zed/gap_delta_m"};
  std::string gap_set_topic_           {"/follow_zed/gap_set_m"};
  std::string gap_state_topic_         {"/follow_zed/follow_gap_m"};

  /* ================== Parámetros de filtro/pérdida ================== */
  double      ema_alpha_{0.25};            // 0..1
  int64_t     lost_timeout_ms_{700};       // ms
  std::string publish_on_lost_{"max"};     // "max","nan","hold"
  double      publish_rate_hz_{20.0};

  /* ================== Parámetros de gap ================== */
  double follow_gap_m_{1.0};
  double gap_min_m_{0.4};
  double gap_max_m_{6.0};
  double gap_step_m_{0.2};

  /* ================== ROS I/F ================== */
  // Publishers (Lifecycle)
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Image>::SharedPtr img_pub_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>::SharedPtr dist_last_obj_pub_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>::SharedPtr leader_distance_state_pub_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Bool>::SharedPtr     follow_state_pub_;
  rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>::SharedPtr secured_cmd_vel_pub_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>::SharedPtr  gap_state_pub_;

  // Subscribers
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr  img_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr   leader_dist_sub_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr      follow_enabled_in_sub_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr      emergency_in_sub_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr     leader_id_in_sub_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr follow_cmd_sub_;
  rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr      gap_incdec_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr   gap_delta_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr   gap_set_sub_;

  // Service
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr set_follow_srv_;

  // Timers
  rclcpp::TimerBase::SharedPtr ping_timer_;
  rclcpp::TimerBase::SharedPtr tick_timer_;

  // TCP
  TcpClient client_;

  // Estado runtime
  State state_{State::IDLE};
  int   last_leader_id_{-1};

  float        last_raw_dist_{std::numeric_limits<float>::quiet_NaN()};
  float        last_filtered_dist_{std::numeric_limits<float>::quiet_NaN()};
  rclcpp::Time last_msg_time_;
  bool         last_motion_cmd_allowed_{false};
};
