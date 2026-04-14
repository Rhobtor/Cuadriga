

#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <sdf/sdf.hh>
#include <ignition/math/Pose3.hh>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include "argj801_ctl_platform_interfaces/msg/cmd_throttle_msg.hpp"

#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <string>

namespace gazebo {

class GazeboRosJ8WheelsPlugin : public ModelPlugin {
public:
  GazeboRosJ8WheelsPlugin() : ModelPlugin(), alive_(true) {}
  ~GazeboRosJ8WheelsPlugin() override {
    alive_ = false;
    if (spin_thread_.joinable()) spin_thread_.join();
  }

  // Métodos de plugin
  void Load(physics::ModelPtr parent, sdf::ElementPtr sdf) override;
  void UpdateChild();
  void FiniChild() { alive_ = false; if (spin_thread_.joinable()) spin_thread_.join(); }

private:
  // Callbacks ROS
  void cmdThrottleCb(const argj801_ctl_platform_interfaces::msg::CmdThrottleMsg::SharedPtr msg);
  void spinLoop();

  // Funciones internas
  void stepControl(double dt);
  void setJointSpeeds(double wl, double wr);
  void publishOdometry();

  // ROS
  std::shared_ptr<rclcpp::Node> node_;
  rclcpp::Subscription<argj801_ctl_platform_interfaces::msg::CmdThrottleMsg>::SharedPtr sub_thr_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  std::thread spin_thread_;
  bool alive_;
  std::mutex mutex_;

  // Parámetros
  std::string ns_;
  std::string thr_topic_{"cmd_throttle_msg"};
  std::string odom_topic_{"odom_demo"};
  std::string odom_frame_{"odom"};
  std::string base_frame_{"base_link"};
  double update_rate_{200.0};
  double update_period_{0.005};
  bool pub_odom_{true};
  bool pub_odom_tf_{true};

  double wheel_separation_{0.34};
  double wheel_diameter_{0.15};
  double max_wheel_torque_{5.0};
  double max_throttle_acc_{1.86};
  double max_steering_acc_{1.23};
  const double thr_to_rad_{0.254608412};
  const double str_to_rad_{0.05787372};
  // const double thr_to_rad_ = 0.101;   // 100 % → 10 rad/s
  // const double str_to_rad_ = 0.0423;  // 100 % → 4.23 rad/s
  // double max_throttle_acc_ = 0.6;     // rad/s²  (≈0.18 m/s²)
  // double max_steering_acc_ = 0.4;     // rad/s²




  // Estado
  double thr_cmd_{0.0}, str_cmd_{0.0};
  double thr_{0.0}, str_{0.0};
  double wl_{0.0}, wr_{0.0};
  nav_msgs::msg::Odometry odom_;

  // Joints
  enum { RIGHT = 0, LEFT = 1 };
  std::vector<std::string> joint_names_[2];
  std::vector<physics::JointPtr> joints_[2];

  // Gazebo
  physics::ModelPtr model_;
  physics::WorldPtr world_;
  common::Time last_update_;
  event::ConnectionPtr update_connection_;
};

void GazeboRosJ8WheelsPlugin::Load(physics::ModelPtr parent, sdf::ElementPtr sdf) {
  model_ = parent;
  world_ = parent->GetWorld();

  // Leer parámetros genéricos
  auto getStr = [&](const std::string &tag, const std::string &def) {
    return sdf->HasElement(tag) ? sdf->Get<std::string>(tag) : def;
  };
  auto getNum = [&](const std::string &tag, double def) {
    return sdf->HasElement(tag) ? sdf->Get<double>(tag) : def;
  };

  update_rate_   = getNum("update_rate", update_rate_);
  update_period_ = update_rate_ > 0.0 ? 1.0/update_rate_ : 0.0;
  max_throttle_acc_ = getNum("max_throttle_acceleration", max_throttle_acc_);
  max_steering_acc_ = getNum("max_steer_acceleration", max_steering_acc_);
  wheel_separation_ = getNum("wheel_separation", wheel_separation_);
  wheel_diameter_   = getNum("wheel_diameter", wheel_diameter_);
  max_wheel_torque_ = getNum("max_wheel_torque", max_wheel_torque_);

  thr_topic_ = getStr("commandThrottleTopic", thr_topic_);
  odom_topic_ = getStr("ros/remapping", odom_topic_);
  pub_odom_    = sdf->Get<bool>("publish_odom", pub_odom_).first;
  pub_odom_tf_ = sdf->Get<bool>("publish_odom_tf", pub_odom_tf_).first;

  // Namespace ROS en <ros><namespace>
  if (sdf->HasElement("ros")) {
    auto rosElm = sdf->GetElement("ros");
    if (rosElm->HasElement("namespace")) {
      ns_ = rosElm->Get<std::string>("namespace");
      if (!ns_.empty() && ns_.front()=='/') ns_.erase(0,1);
    }
  }

  // Limpiar '/' inicial en tópicos relativos
  auto stripSlash = [&](std::string &s){ if(!s.empty() && s.front()=='/') s.erase(0,1); };
  stripSlash(thr_topic_);
  stripSlash(odom_topic_);

  // Leer los joints
  for (auto e = sdf->GetElement("left_joint"); e; e = e->GetNextElement("left_joint"))
    joint_names_[LEFT].push_back(e->Get<std::string>());
  for (auto e = sdf->GetElement("right_joint"); e; e = e->GetNextElement("right_joint"))
    joint_names_[RIGHT].push_back(e->Get<std::string>());
  if (joint_names_[LEFT].size() != joint_names_[RIGHT].size() || joint_names_[LEFT].empty())
    gzthrow("Mismatch in left/right joint count");

  for (int side=0; side<2; ++side) {
    for (auto &jn : joint_names_[side]) {
      auto j = model_->GetJoint(jn);
      if (!j) gzthrow("Joint '" + jn + "' not found");
      j->SetEffortLimit(0, max_wheel_torque_);
      joints_[side].push_back(j);
    }
  }

  // Inicializar ROS
  rclcpp::NodeOptions opts;
  opts.arguments({"--ros-args"});
  node_ = rclcpp::Node::make_shared("gazebo_ros_j8_wheels_plugin", ns_, opts);
  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(node_);

  sub_thr_  = node_->create_subscription<argj801_ctl_platform_interfaces::msg::CmdThrottleMsg>(
    thr_topic_, 10, std::bind(&GazeboRosJ8WheelsPlugin::cmdThrottleCb, this, std::placeholders::_1));
  odom_pub_ = node_->create_publisher<nav_msgs::msg::Odometry>(odom_topic_, 10);

  spin_thread_ = std::thread(&GazeboRosJ8WheelsPlugin::spinLoop, this);

  last_update_ = world_->SimTime();
  update_connection_ = event::Events::ConnectWorldUpdateBegin(
    std::bind(&GazeboRosJ8WheelsPlugin::UpdateChild, this));
}

void GazeboRosJ8WheelsPlugin::cmdThrottleCb(const argj801_ctl_platform_interfaces::msg::CmdThrottleMsg::SharedPtr msg) {
  std::lock_guard<std::mutex> lk(mutex_);
  double thr = std::clamp<double>(static_cast<double>(msg->throttle), -100.0, 100.0);
  double str = std::clamp<double>(static_cast<double>(msg->steering), -100.0, 100.0);
  thr_cmd_ = thr * thr_to_rad_;  // 100% throttle → thr_to_rad_ * 100 = ~25.46 rad/s
  str_cmd_ = str * str_to_rad_;  // 100% steering → str_to_rad_ * 100 = ~5.79 rad/s
}

void GazeboRosJ8WheelsPlugin::spinLoop() {
  rclcpp::Rate rate(100);
  while (alive_ && rclcpp::ok()) {
    rclcpp::spin_some(node_);
    rate.sleep();
  }
}

void GazeboRosJ8WheelsPlugin::stepControl(double dt) {
  std::lock_guard<std::mutex> lk(mutex_);
  double d_thr = thr_cmd_ - thr_;
  double max_dthr = max_throttle_acc_ * dt;
  thr_ += std::clamp(d_thr, -max_dthr, max_dthr);
  double d_str = str_cmd_ - str_;
  double max_dstr = max_steering_acc_ * dt;
  str_ += std::clamp(d_str, -max_dstr, max_dstr);
  wl_ = thr_ - str_;
  wr_ = thr_ + str_;
}

void GazeboRosJ8WheelsPlugin::setJointSpeeds(double wl, double wr) {
  for (size_t i = 0; i < joints_[LEFT].size(); ++i) {
    joints_[LEFT][i]->SetVelocity(0, wl);
    joints_[RIGHT][i]->SetVelocity(0, wr);
  }
}

void GazeboRosJ8WheelsPlugin::publishOdometry() {
  if (!pub_odom_ && !pub_odom_tf_) return;
  auto pose = model_->WorldPose();
  auto sim = world_->SimTime();
  builtin_interfaces::msg::Time ts;
  ts.sec = sim.sec;
  ts.nanosec = sim.nsec;

  geometry_msgs::msg::TransformStamped tfm;
  tfm.header.stamp = ts;
  tfm.header.frame_id = odom_frame_;
  tfm.child_frame_id = base_frame_;
  tfm.transform.translation.x = pose.Pos().X();
  tfm.transform.translation.y = pose.Pos().Y();
  tfm.transform.translation.z = pose.Pos().Z();
  tfm.transform.rotation.x = pose.Rot().X();
  tfm.transform.rotation.y = pose.Rot().Y();
  tfm.transform.rotation.z = pose.Rot().Z();
  tfm.transform.rotation.w = pose.Rot().W();

  if (pub_odom_tf_) tf_broadcaster_->sendTransform(tfm);

  if (pub_odom_) {
    odom_.header.stamp = ts;
    odom_.header.frame_id = odom_frame_;
    odom_.child_frame_id = base_frame_;
    odom_.pose.pose.position.x = pose.Pos().X();
    odom_.pose.pose.position.y = pose.Pos().Y();
    odom_.pose.pose.position.z = pose.Pos().Z();
    odom_.pose.pose.orientation.x = pose.Rot().X();
    odom_.pose.pose.orientation.y = pose.Rot().Y();
    odom_.pose.pose.orientation.z = pose.Rot().Z();
    odom_.pose.pose.orientation.w = pose.Rot().W();
    odom_pub_->publish(odom_);
  }
}

void GazeboRosJ8WheelsPlugin::UpdateChild() {
  auto now = world_->SimTime();
  double dt = (now - last_update_).Double();
  if (dt < update_period_) return;
  stepControl(dt);
  setJointSpeeds(wl_, wr_);
  publishOdometry();
  last_update_ = now;
}

GZ_REGISTER_MODEL_PLUGIN(GazeboRosJ8WheelsPlugin)

} // namespace gazebo
