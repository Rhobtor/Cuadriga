#include "argj801_sensors/argj801_camera_builder.hpp"

using namespace Builder;

Argj801CameraBuilder::Argj801CameraBuilder()
{
    sensor = std::make_shared<ARGJ801Sensors::ARGJ801CameraInterface>();
}

void Argj801CameraBuilder::buildPreviousConfig(std::string url)
{
    sensor->createPreviousConfig(url);
}

void Argj801CameraBuilder::buildVelocine(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Velocine sensor is not available");
}

void Argj801CameraBuilder::buildSick(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Sick sensor is not available");
}

void Argj801CameraBuilder::buildTwist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Twist sensor is not available");
}

void Argj801CameraBuilder::buildOdometer(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Odometer sensor is not available");
}

void Argj801CameraBuilder::buildMotorTelemetryLeft(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Motor telemetry left sensor is not available");
}

void Argj801CameraBuilder::buildMotorTelemetryRigth(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id)
{
    throw NotAvailableOptionException("Motor telemetry right sensor is not available");
}

void Argj801CameraBuilder::buildCamera(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id,int compresion_ratio, bool resize)
{
    sensor->createCameraData(publisher, frame_id);
    sensor->setCompressionRatio(compresion_ratio);
    sensor->setResizeImage(resize);
    
}

std::shared_ptr<Sensor> Argj801CameraBuilder::getSensor()
{
    return sensor;
}