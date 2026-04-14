#ifndef COMPRESSEDIMAGE_H
#define COMPRESSEDIMAGE_H


#include "data/ImageData.hpp"
#include "rclcpp/publisher.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include <vector>

namespace SensorData {
  class CompressedImage: public ImageData, public std::enable_shared_from_this<SensorData::CompressedImage>
  {
  private:
    std::string format;

    std::vector<uint8_t> data;

    uint64_t time;
    
    std::string frame_id;


    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher;



  public:
    CompressedImage(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id);
    void acceptImage(std::shared_ptr<Visitor::Visitor> visitor);

    // Getters
    std::string getFormat() const;
    std::vector<uint8_t> getData() const;
    uint64_t getTime() const;
    const std::string& getFrameId() const;
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr getPublisher() const;

    // Setters
    void setFormat(const std::string& format);
    void setData(const std::vector<uint8_t>& data);
    void setTime(uint64_t time);
  };
}
#endif // COMPRESSEDIMAGE_H