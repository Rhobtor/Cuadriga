#include "data/MotorTelemetry.hpp"

using namespace SensorData;

MotorTelemetry::MotorTelemetry(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) :
    publisher(publisher), frame_id(frame_id)
{

}

void MotorTelemetry::acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor) {
  visitor->visitMotorTelemetry(shared_from_this());
}

// Getters
int64_t MotorTelemetry::getEncoder() const {
    return encoder;
}

float MotorTelemetry::getRpm() const {
    return rpm;
}

double MotorTelemetry::getTime() const {
    return time;
}

std::string MotorTelemetry::getFrameId() const {
    return frame_id;
}

std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> MotorTelemetry::getPublisher() const {
    return publisher;
}

// Setters
void MotorTelemetry::setEncoder(int64_t encoder) {
    this->encoder = encoder;
}

void MotorTelemetry::setRpm(float rpm) {
    this->rpm = rpm;
}

void MotorTelemetry::setTime(double time) {
    this->time = time;
}

