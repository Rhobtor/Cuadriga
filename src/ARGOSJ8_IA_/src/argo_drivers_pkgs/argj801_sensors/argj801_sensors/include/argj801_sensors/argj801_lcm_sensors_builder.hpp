#ifndef ARGJ801SENSORBUILDER_H
#define ARGJ801SENSORBUILDER_H
#include <memory>
#include "argj801_sensors/SensorBuilder.hpp"
#include "argj801_sensors/argj801_sensors_interface.hpp"

namespace Builder {
    class Argj801LCMSensorBuilder: public SensorBuilder
    {
        private:
            std::shared_ptr<ARGJ801Sensors::ARGJ801SensorsInterface> sensor;
        public:
            Argj801LCMSensorBuilder();
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
#endif // ARGJ801SENSORBUILDER_H