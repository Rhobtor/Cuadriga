#include "data/LidarData.hpp"

using namespace SensorData;

LidarData::LidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) :
    publisher(publisher), frame_id(frame_id)
{
    
}

void LidarData::acceptLidar(std::shared_ptr<Visitor::Visitor> visitor) 
{
    visitor->visitLidar(shared_from_this());
}

// Getters
float LidarData::getAngleMin() const 
{
    return angleMin;
}

float LidarData::getAngleMax() const 
{
    return angleMax;
}

float LidarData::getAngleIncrement() const 
{
    return angle_increment;
}

float LidarData::getTimeIncrement() const 
{
    return time_increment;
}

float LidarData::getScanTime() const 
{
    return scan_time;
}

float LidarData::getRangeMin() const 
{
    return range_min;
}

float LidarData::getRangeMax() const 
{
    return range_max;
}

const std::vector<float>& LidarData::getRanges() const 
{
    return ranges;
}

double LidarData::getTime() const 
{
    return time;
}

const std::string& LidarData::getFrameId() const 
{
    return frame_id;
}

std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> LidarData::getPublisher() const 
{
    return publisher;
}

// Setters
void LidarData::setAngleMin(float angleMin) 
{
    this->angleMin = angleMin;
}

void LidarData::setAngleMax(float angleMax) 
{
    this->angleMax = angleMax;
}

void LidarData::setAngleIncrement(float angle_increment) 
{
    this->angle_increment = angle_increment;
}

void LidarData::setTimeIncrement(float time_increment) 
{
    this->time_increment = time_increment;
}

void LidarData::setScanTime(float scan_time) 
{
    this->scan_time = scan_time;
}

void LidarData::setRangeMin(float range_min) 
{
    this->range_min = range_min;
}

void LidarData::setRangeMax(float range_max) 
{
    this->range_max = range_max;
}

void LidarData::setRanges(const std::vector<float>& ranges) 
{
    this->ranges = ranges;
}

void LidarData::setTime(double time) 
{
    this->time = time;
}