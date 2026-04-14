#ifndef LIDARDATA_H
#define LIDARDATA_H


#include "data/Lidar.hpp"
#include "rclcpp/publisher.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>

namespace SensorData {
  class LidarData: public Lidar, public std::enable_shared_from_this<SensorData::LidarData>
  {
  private:
    float angleMin;
    float angleMax;
    float angle_increment;
    
    float time_increment;
    float scan_time;

    float range_min;
    float range_max;

    std::vector<float> ranges;


    double time;
    
    std::string frame_id;


    std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher;



  public:
    LidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id);
    void acceptLidar(std::shared_ptr<Visitor::Visitor> visitor);

     // Getters
    float getAngleMin() const;
    float getAngleMax() const;
    float getAngleIncrement() const;
    float getTimeIncrement() const;
    float getScanTime() const;
    float getRangeMin() const;
    float getRangeMax() const;
    const std::vector<float>& getRanges() const;
    double getTime() const;
    const std::string& getFrameId() const;
    std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> getPublisher() const;

    // Setters
    void setAngleMin(float angleMin);
    void setAngleMax(float angleMax);
    void setAngleIncrement(float angle_increment);
    void setTimeIncrement(float time_increment);
    void setScanTime(float scan_time);
    void setRangeMin(float range_min);
    void setRangeMax(float range_max);
    void setRanges(const std::vector<float>& ranges);
    void setTime(double time);
  };
}
#endif // LIDARDATA_H