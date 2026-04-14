
#ifndef VISITOR_H
#define VISITOR_H

#include <memory>

const int X = 0; //! Index of the first element of array that corresponds to x component of a point
const int Y = 1; //! Index of the first element of array that corresponds to y component of a point
const int Z = 2; //! Index of the first element of array that corresponds to z component of a point


namespace SensorData {
  class LidarData;
  class MotorTelemetry;
  class OdometerData;
  class Twist;
  class CompressedImage;
}

namespace Visitor {
  class Visitor {
  public:
    virtual void visitLidar(std::shared_ptr<SensorData::LidarData> lidarData) =0;
    virtual void visitMotorTelemetry(std::shared_ptr<SensorData::MotorTelemetry> motorTelemetry) =0;
    virtual void visitOdometer(std::shared_ptr<SensorData::OdometerData> odometer) = 0;
    virtual void visitTwist(std::shared_ptr<SensorData::Twist> twist) = 0;
    virtual void visitImage(std::shared_ptr<SensorData::CompressedImage> image) = 0;
  };
}
#endif // VISITOR_H
