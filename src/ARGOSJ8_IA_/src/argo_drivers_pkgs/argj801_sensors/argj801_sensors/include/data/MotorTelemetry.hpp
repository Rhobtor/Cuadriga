#ifndef MOTORTELEMETRY_H
#define MOTORTELEMETRY_H


#include "data/DriveLine.hpp"
#include "rclcpp/publisher.hpp"
#include "argj801_sensors_msgs/msg/motor_telemetry.hpp"
#include <vector>

namespace SensorData {
  class MotorTelemetry: public DriveLine, public std::enable_shared_from_this<SensorData::MotorTelemetry>
  {
  private:

    int64_t encoder;
    float rpm;
    double time;
    
    std::string frame_id;
    std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher;

  public:
    MotorTelemetry(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id);
    void acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor);

    // Getters
    int64_t getEncoder() const;
    float getRpm() const;
    double getTime() const;
    std::string getFrameId() const;
    std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> getPublisher() const;


    // Setters
    void setEncoder(int64_t encoder);
    void setRpm(float rpm);
    void setTime(double time);
  };
}
#endif // MOTORTELEMETRY_H