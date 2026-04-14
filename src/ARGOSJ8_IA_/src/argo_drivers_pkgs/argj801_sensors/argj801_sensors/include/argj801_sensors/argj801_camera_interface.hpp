#ifndef ARGJ801_CAMERA_INTERFACE_HPP
#define ARGJ801_CAMERA_INTERFACE_HPP

#include "argj801_sensors/Sensor.hpp"
#include "data/Composite.hpp"
#include "data/CompressedImage.hpp"
#include <sys/time.h>
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>

namespace ARGJ801Sensors {

  class ARGJ801CameraInterface : public Sensor {
  private:

    std::string rtsp_url_;

    std::shared_ptr<SensorData::Composite> compositeCamera;
    std::shared_ptr<SensorData::CompressedImage> cameraData;

    rclcpp::Clock clock;
    cv::VideoCapture cap_;
    std::shared_ptr<std::vector<int>> encodeParams;
    bool resize_image;
    int compression_ratio;

  public:

    ARGJ801CameraInterface();

    ~ARGJ801CameraInterface();

    std::string getID();
    void start();
    void stop();
    void getFastData(std::shared_ptr<Visitor::Visitor> visitor);
    void getSlowData(std::shared_ptr<Visitor::Visitor> visitor);

    void createPreviousConfig(std::string url); 
    void createCameraData(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id);
    int64_t time_helper();
    void setResizeImage(bool resize);
    void setCompressionRatio(int ratio);
  };
}
#endif // ARGJ801_CAMERA_INTERFACE_HPP