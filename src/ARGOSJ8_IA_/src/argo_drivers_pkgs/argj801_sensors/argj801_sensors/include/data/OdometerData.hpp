#ifndef ODOMETERDATA_H
#define ODOMETERDATA_H


#include "data/DriveLine.hpp"
#include "rclcpp/publisher.hpp"
#include "argj801_sensors_msgs/msg/odometer.hpp"
#include <vector>

namespace SensorData {
  class OdometerData: public DriveLine, public std::enable_shared_from_this<SensorData::OdometerData>
  {
  private:

    int64_t odometer;
    double time;
    
    std::string frame_id;
    std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher;

  public:
    OdometerData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id);
    void acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor);

    // Getters
    int64_t getOdometer() const;
    double getTime() const;
    std::string getFrameId() const;
    std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> getPublisher() const;


    // Setters
    void setOdometer(int64_t odometer);
    void setTime(double time);
  };
}
#endif // ODOMETERDATA_H