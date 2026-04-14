#ifndef IMAGE_H
#define IMAGE_H


#include "visitor/Visitor.hpp"
#include <memory>
namespace SensorData {
  class ImageData
  {
  public:
    virtual void acceptImage(std::shared_ptr<Visitor::Visitor> visitor) = 0;
  };
}
#endif // IMAGE_H