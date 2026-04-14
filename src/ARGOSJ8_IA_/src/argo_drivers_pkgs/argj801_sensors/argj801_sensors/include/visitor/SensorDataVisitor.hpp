#ifndef SENSORDATAVISITOR_H
#define SENSORDATAVISITOR_H
#include <memory>
#include "visitor/Visitor.hpp"
#include "data/LidarData.hpp"
#include "data/MotorTelemetry.hpp"
#include "data/OdometerData.hpp"
#include "data/Twist.hpp"
#include "data/CompressedImage.hpp"
#include "rclcpp/clock.hpp"


namespace Visitor {
  class SensorDataVisitor: public Visitor{
  private:

  public:
    SensorDataVisitor();
    void visitLidar(std::shared_ptr<SensorData::LidarData> lidarData);
    void visitMotorTelemetry(std::shared_ptr<SensorData::MotorTelemetry> motorTelemetry);
    void visitOdometer(std::shared_ptr<SensorData::OdometerData> odometer);
    void visitTwist(std::shared_ptr<SensorData::Twist> twist);
    void visitImage(std::shared_ptr<SensorData::CompressedImage> image);
  };
}
#endif // SENSORDATAVISITOR_H