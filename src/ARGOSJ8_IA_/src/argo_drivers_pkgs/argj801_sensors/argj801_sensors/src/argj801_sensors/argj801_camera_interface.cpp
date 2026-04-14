#include "argj801_sensors/argj801_camera_interface.hpp"
#include <fstream> 

using namespace ARGJ801Sensors;
    
ARGJ801CameraInterface::ARGJ801CameraInterface() {
  compositeCamera = std::make_shared<SensorData::Composite>();
  auto vec = std::vector<int>{cv::IMWRITE_JPEG_QUALITY, 75};
  encodeParams = std::make_shared<std::vector<int>>(vec);  


}

ARGJ801CameraInterface::~ARGJ801CameraInterface() {

}

std::string ARGJ801CameraInterface::getID() {
  return "ARGJ801_camera";
}

void ARGJ801CameraInterface::start() {
    auto vec = std::vector<int>{cv::IMWRITE_JPEG_QUALITY, compression_ratio};
    encodeParams = std::make_shared<std::vector<int>>(vec);  


    cap_.open(rtsp_url_);

    if (!cap_.isOpened()) {
          throw SensorException( "No se pudo abrir el stream de la cámara");
    }

    cap_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
}

    
void ARGJ801CameraInterface::stop() {
    if (cap_.isOpened()) {
        cap_.release();
    }
}
void ARGJ801CameraInterface::getFastData(std::shared_ptr<Visitor::Visitor> visitor) {
  
    cv::Mat frame;
    cv::Mat resized_img;
    std::vector<uint8_t> buffer;

    
    if(!cap_.read(frame)){
      return;
    }

    if (frame.empty()) {
        throw ("Frame vacío capturado");
    }

    if(resize_image) {
      int new_width = 850;
      int new_height = 480;
      cv::Size new_size(new_width, new_height);
      cv::resize(frame, resized_img, new_size);
      cv::imencode(".jpg", resized_img, buffer, *encodeParams);
    }  
    else {
      cv::imencode(".jpg", frame, buffer, *encodeParams);
    }
    
    cameraData->setData(buffer);
    cameraData->setTime(time_helper());
    cameraData->setFormat("jpeg");
    compositeCamera->acceptImage(visitor);

}
  
void ARGJ801CameraInterface::getSlowData(std::shared_ptr<Visitor::Visitor> visitor) {}

void ARGJ801CameraInterface::createPreviousConfig(std::string url) {
  rtsp_url_ = url;
}
    
void ARGJ801CameraInterface::createCameraData(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id) {
    cameraData = std::make_shared<SensorData::CompressedImage>(publisher, frame_id);
    compositeCamera->addImage(cameraData);
}

int64_t ARGJ801CameraInterface::time_helper()
{
#if POSIX_TIMERS > 0
  struct timespec curtime;
  clock_gettime(CLOCK_REALTIME, &curtime);
  return (unsigned long long)(curtime.tv_sec) * 1000000000 + (unsigned long long)(curtime.tv_nsec);
#else
  struct timeval timeofday;
  gettimeofday(&timeofday,NULL);
  return (unsigned long long)(timeofday.tv_sec) * 1000000000 + (unsigned long long)(timeofday.tv_usec) * 1000;
#endif
}

void ARGJ801CameraInterface::setResizeImage(bool resize) {
    resize_image = resize;
}

void ARGJ801CameraInterface::setCompressionRatio(int ratio) {
    if (ratio >= 0 && ratio <= 100) {
        compression_ratio = ratio;
    } else {
        throw SensorException("Compression ratio must be between 0 and 100");
    }

}