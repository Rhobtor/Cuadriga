#include "data/Twist.hpp"

using namespace SensorData;

Twist::Twist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id) :
    publisher(publisher), frame_id(frame_id)
{

}

void Twist::acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor) {
  visitor->visitTwist(shared_from_this());
}

// Getters
double Twist::getLinearX() const {
    return linear_x;
}

double Twist::getLinearY() const {
    return linear_y;
}

double Twist::getLinearZ() const {
    return linear_z;
}

double Twist::getAngularX() const {
    return angular_x;
}

double Twist::getAngularY() const {
    return angular_y;
}

double Twist::getAngularZ() const {
    return angular_z;
}

double Twist::getTime() const {
    return time;
}

std::string Twist::getFrameId() const {
    return frame_id;
}

std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> Twist::getPublisher() const {
    return publisher;
}

// Setters
void Twist::setLinearX(double linear_x) {
    this->linear_x = linear_x;
}

void Twist::setLinearY(double linear_y) {
    this->linear_y = linear_y;
}

void Twist::setLinearZ(double linear_z) {
    this->linear_z = linear_z;
}

void Twist::setAngularX(double angular_x) {
    this->angular_x = angular_x;
}

void Twist::setAngularY(double angular_y) {
    this->angular_y = angular_y;
}

void Twist::setAngularZ(double angular_z) {
    this->angular_z = angular_z;
}

void Twist::setTime(double time) {
    this->time = time;
}

