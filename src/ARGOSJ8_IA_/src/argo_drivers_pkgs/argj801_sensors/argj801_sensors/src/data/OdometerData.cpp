#include "data/OdometerData.hpp"

using namespace SensorData;

OdometerData::OdometerData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id) :
    publisher(publisher), frame_id(frame_id)
{

}

void OdometerData::acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor) {
  visitor->visitOdometer(shared_from_this());
}

// Getters
int64_t OdometerData::getOdometer() const {
    return odometer;
}

double OdometerData::getTime() const {
    return time;
}

std::string OdometerData::getFrameId() const {
    return frame_id;
}

std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> OdometerData::getPublisher() const {
    return publisher;
}

// Setters
void OdometerData::setOdometer(int64_t odometer) {
    this->odometer = odometer;
}

void OdometerData::setTime(double time) {
    this->time = time;
}