#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "data/Lidar.hpp"
#include "data/DriveLine.hpp"
#include "data/ImageData.hpp"
#include <vector>
namespace SensorData {
  class Composite: public Lidar, public DriveLine, public ImageData
  {
  private:
    std::vector<std::shared_ptr<Lidar>> lidarList;
    std::vector<std::shared_ptr<DriveLine>> driveLineList;
    std::vector<std::shared_ptr<ImageData>> imageList;

  public:
    Composite();
    void acceptLidar(std::shared_ptr<Visitor::Visitor> visitor);
    void acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor);
    void acceptImage(std::shared_ptr<Visitor::Visitor> visitor);
    void addLidar(std::shared_ptr<Lidar> lidar);
    void addDriveLine(std::shared_ptr<DriveLine> driveLine);
    void addImage(std::shared_ptr<ImageData> image);
  };
}
#endif // COMPOSITE_H