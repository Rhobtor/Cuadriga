#include "argj801_sensors/argj801_lcm_sensors_builder.hpp"

using namespace Builder;

Argj801LCMSensorBuilder::Argj801LCMSensorBuilder() {
  sensor = std::make_shared<ARGJ801Sensors::ARGJ801SensorsInterface>();
}
void Argj801LCMSensorBuilder::buildPreviousConfig(std::string url) {
  sensor->createPreviousConfig(url);
}
void Argj801LCMSensorBuilder::buildVelocine(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) {
  sensor->createVelocineLidarData(publisher, frame_id);
}
void Argj801LCMSensorBuilder::buildSick(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) {
  sensor->createSickLidarData(publisher, frame_id);
}
void Argj801LCMSensorBuilder::buildTwist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id) {
  sensor->createTwistData(publisher, frame_id);
}
void Argj801LCMSensorBuilder::buildOdometer(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id) {
  sensor->createOdometerData(publisher, frame_id);
}
void Argj801LCMSensorBuilder::buildMotorTelemetryLeft(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) {
  sensor->createLeftMotorTelemetryData(publisher, frame_id);
}
void Argj801LCMSensorBuilder::buildMotorTelemetryRigth(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) {
  sensor->createRightMotorTelemetryData(publisher, frame_id);
}

void Argj801LCMSensorBuilder::buildCamera(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id,int compresion_ratio, bool resize) {
  throw NotAvailableOptionException("Front Camera not available");
}

std::shared_ptr<Sensor> Argj801LCMSensorBuilder::getSensor() {
  return sensor;
}