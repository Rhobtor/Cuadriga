#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include <lifecycle_msgs/msg/transition.hpp>
#include "rclcpp/rclcpp.hpp"
#include <lifecycle_msgs/msg/state.hpp>
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"

#include "rcutils/logging_macros.h"

#include "argj801_sensors/SensorBuilder.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "argj801_sensors_msgs/msg/motor_telemetry.hpp"
#include "argj801_sensors_msgs/msg/odometer.hpp"
#include "argj801_sensors/Sensor.hpp"

#include "visitor/SensorDataVisitor.hpp"

#include "diagnostic_updater/update_functions.hpp"

#include "argj801_sensors/argj801_sensors_interface.hpp"

using namespace std::chrono_literals;

/**
 * @brief Nodo Lifecycle que publica la telemetría/sensores del rover.
 *
 * Este nodo se construye alrededor de un patrón Builder + Visitor:
 * - `Builder::SensorBuilder` instancia y configura qué sensores están activos
 *   (lidars, driveline, odómetro, twist, cámara).
 * - `Sensor` representa el dispositivo/stack que produce datos.
 * - `Visitor::SensorDataVisitor` visita los objetos de datos y dispara la
 *   publicación en ROS.
 *
 * Contrato ROS (tópicos publicados, relativos al namespace del nodo, p.ej. `/ARGJ801`):
 * - `velodyne_scan` (sensor_msgs/LaserScan) [si `velodyne`]
 * - `sick_scan` (sensor_msgs/LaserScan) [si `sick`]
 * - `left_motor_telemetry`, `right_motor_telemetry` (argj801_sensors_msgs/MotorTelemetry)
 * - `odometer` (argj801_sensors_msgs/Odometer)
 * - `twist` (geometry_msgs/TwistStamped)
 * - `camera_topic` (sensor_msgs/CompressedImage) [si `camera`]
 *
 * Parámetros:
 * - lcm_params.lcm_config_file: YAML para URLs/canales LCM (modo sensores).
 * - robot_frame / velodyne_frame / sick_frame / camera_frame: frame_id de los mensajes.
 * - flags bool: velodyne, sick, left_telemetry, right_telemetry, odometer, twist, camera.
 * - camera_url, camera_topic, image_compression_ratio, resize_image.
 */
class Argj801SensorsNode : public rclcpp_lifecycle::LifecycleNode
{
private:

  // diagnostic_updater se usa para exponer frecuencia efectiva de publicación.
  std::unique_ptr<diagnostic_updater::Updater> diagnostic_;
  double lidar_desired_freq_;
  std::unique_ptr<diagnostic_updater::FrequencyStatus> fast_freq_diag_; 
  double drive_line_desired_freq_;
  std::unique_ptr<diagnostic_updater::FrequencyStatus> slow_freq_diag_; 
  double camera_desired_freq_;

  std::shared_ptr<Builder::SensorBuilder> builder;
  std::shared_ptr<Sensor> sensor;

  std::shared_ptr<Visitor::Visitor> visitor;

  std::string lcm_config_file;
  std::string robot_frame;
  std::string velodyne_frame;
  std::string sick_frame;
  bool velodyne;
  bool sick;
  bool left_motor;
  bool right_motor;
  bool odometer;
  bool twist;
  bool camera;
  std::string camera_url;
  std::string camera_frame;
  std::string camera_topic;
  bool resize_image;
  int compression_ratio;


  std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> velodyne_publisher;
  std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> sick_publisher;
  std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> left_motor_publisher;
  std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> right_motor_publisher;
  std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> odometer_publisher;
  std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> twist_publisher;
  rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr camera_publisher;

  // Timers de adquisición. El periodo define el "target rate".
  rclcpp::TimerBase::SharedPtr lidarTimer;
  rclcpp::TimerBase::SharedPtr driveLineTimer;
  rclcpp::TimerBase::SharedPtr cameraTimer;

  rclcpp::CallbackGroup::SharedPtr group1;

  /// Datos de alta frecuencia (principalmente lidar/cámara).
  void getFastData();
  /// Datos de baja frecuencia (driveline/odómetro/twist).
  void getSlowData();
  
public:
  Argj801SensorsNode(std::shared_ptr<Builder::SensorBuilder> builder);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_error(const rclcpp_lifecycle::State &state);

};

