#ifndef LIDAR_H
#define LIDAR_H


#include "visitor/Visitor.hpp"
#include <memory>
namespace SensorData {
  class Lidar
  {
  public:
    virtual void acceptLidar(std::shared_ptr<Visitor::Visitor> visitor) = 0;
  };
}
#endif // LIDAR_H