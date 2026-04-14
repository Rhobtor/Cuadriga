#ifndef DRIVE_LINE_H
#define DRIVE_LINE_H

#include "visitor/Visitor.hpp"
#include <memory>
namespace SensorData {
  class DriveLine
  {
  public:
    virtual void acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor) = 0;
  };
}
#endif // DRIVE_LINE_H