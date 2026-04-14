#include "data/CompressedImage.hpp"

using namespace SensorData;

CompressedImage::CompressedImage(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id) :
    publisher(publisher), frame_id(frame_id)
{
    
}

void CompressedImage::acceptImage(std::shared_ptr<Visitor::Visitor> visitor) 
{
    visitor->visitImage(shared_from_this());
}

std::string CompressedImage::getFormat() const {
    return format;
}

void CompressedImage::setFormat(const std::string& format) {
    this->format = format;
}

// Implementación de los métodos get y set para 'data'
std::vector<uint8_t> CompressedImage::getData() const {
    return data;
}

void CompressedImage::setData(const std::vector<uint8_t>& data) {
    this->data = data;
}

rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr CompressedImage::getPublisher() const 
{
    return publisher;
}

void CompressedImage::setTime(uint64_t time) 
{
    this->time = time;
}

uint64_t CompressedImage::getTime() const 
{
    return time;
}

const std::string& CompressedImage::getFrameId() const 
{
    return frame_id;
}