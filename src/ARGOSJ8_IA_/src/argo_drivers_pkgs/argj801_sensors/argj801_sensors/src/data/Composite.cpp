#include "data/Composite.hpp"

using namespace SensorData;

Composite::Composite()
{

}

void Composite::acceptLidar(std::shared_ptr<Visitor::Visitor> visitor)
{
  for(auto item: lidarList) {
      item->acceptLidar(visitor);
  }
}

void Composite::acceptDriveLine(std::shared_ptr<Visitor::Visitor> visitor)
{
  for(auto item: driveLineList) {
      item->acceptDriveLine(visitor);
  }
}

void Composite::acceptImage(std::shared_ptr<Visitor::Visitor> visitor)
{
  for(auto item: imageList) {
      item->acceptImage(visitor);
  }
}

void Composite::addLidar(std::shared_ptr<Lidar> ahrs)
{
  lidarList.push_back(ahrs);
}

void Composite::addDriveLine(std::shared_ptr<DriveLine> gnss)
{
  driveLineList.push_back(gnss);
}

void Composite::addImage(std::shared_ptr<ImageData> image)
{
  imageList.push_back(image);
}