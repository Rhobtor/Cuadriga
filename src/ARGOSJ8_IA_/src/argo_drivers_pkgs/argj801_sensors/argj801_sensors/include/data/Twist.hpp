#ifndef TWIST_H
#define TWIST_H


#include "data/DriveLine.hpp"
#include "rclcpp/publisher.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include <vector>

namespace SensorData {
  class Twist: public DriveLine, public std::enable_shared_from_this<SensorData::Twist>
  {
  private:
    double linear_x;
    double linear_y;
    double linear_z;

    double angular_x;
    double angular_y;
    double angular_z;


    double time;
    
    std::string frame_id;


    std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher;



  public:
    Twist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id);
    void acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor);

        // Getters
        
    double getLinearX() const;
    double getLinearY() const;
    double getLinearZ() const;
    double getAngularX() const;
    double getAngularY() const;
    double getAngularZ() const;
    double getTime() const;
    std::string getFrameId() const;
    std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> getPublisher() const;


    // Setters
    void setLinearX(double linear_x);
    void setLinearY(double linear_y);
    void setLinearZ(double linear_z);
    void setAngularX(double angular_x);
    void setAngularY(double angular_y);
    void setAngularZ(double angular_z);
    void setTime(double time);

  };
}
#endif // TWIST_H