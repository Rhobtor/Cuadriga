#ifndef ARGJ801CAMERABUILDER_H
#define ARGJ801CAMERABUILDER_H
#include <memory>
#include "argj801_sensors/SensorBuilder.hpp"
#include "argj801_sensors/argj801_camera_interface.hpp"

namespace Builder {
    class Argj801CameraBuilder: public SensorBuilder
    {
        private:
            std::shared_ptr<ARGJ801Sensors::ARGJ801CameraInterface> sensor;
        public:
            Argj801CameraBuilder();
            void buildPreviousConfig(std::string url) ;
            void buildVelocine(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id);
            void buildSick(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id);
            void buildTwist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id);
            void buildOdometer(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id);
            void buildMotorTelemetryLeft(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id);
            void buildMotorTelemetryRigth(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id);
            void buildCamera(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id,int compresion_ratio, bool resize);
            std::shared_ptr<Sensor> getSensor();
	};
}
#endif // ARGJ801CAMERABUILDER_H