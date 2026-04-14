#ifndef ARGJ801_SENSORS_INTERFACE_HPP
#define ARGJ801_SENSORS_INTERFACE_HPP

#include "argj801_sensors/Sensor.hpp"
#include "argj801_lcm/LCM_interface.hpp"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include "data/LidarData.hpp"
#include "data/MotorTelemetry.hpp"
#include "data/OdometerData.hpp"
#include "data/Twist.hpp"
#include "data/Composite.hpp"

namespace ARGJ801Sensors {

  enum class LidarType {
    VELODINE,
    SICK
  };

  /**
   * @brief Implementación `Sensor` basada en LCM para leer sensores del rover.
   *
   * Esta clase crea un `argj801_lcm::LCMInterface` y varios objetos de datos
   * (LidarData, MotorTelemetry, OdometerData, Twist...), que a su vez se conectan
   * a publishers ROS mediante el patrón Visitor.
   */
  class ARGJ801SensorsInterface : public Sensor {
  private:
    std::shared_ptr<argj801_lcm::LCMInterface> lcmInterface;
    std::shared_ptr<SensorData::LidarData> velodineLidarData;
    std::shared_ptr<SensorData::LidarData> sickLidarData;
    std::shared_ptr<SensorData::MotorTelemetry> leftMotorTelemetry;
    std::shared_ptr<SensorData::MotorTelemetry> rightMotorTelemetry;
    std::shared_ptr<SensorData::OdometerData> odometerData;
    std::shared_ptr<SensorData::Twist> twist;
    std::shared_ptr<SensorData::Composite> compositeLidarVelodine;
    std::shared_ptr<SensorData::Composite> compositeLidarSick;
    std::shared_ptr<SensorData::Composite> compositeDriveLine;
    std::string lcm_config_file;

    double velodinePreviousTime;
    double sickPreviousTime;
    bool velodineFirstTime = true;
    bool sickFirstTime = true;

  public:
    // Constructor
    ARGJ801SensorsInterface();

    // Destructor
    ~ARGJ801SensorsInterface();

    std::string getID();
    void start();
    void stop();
    void getFastData(std::shared_ptr<Visitor::Visitor> visitor);
    void getSlowData(std::shared_ptr<Visitor::Visitor> visitor);

    /// Carga el YAML de LCM y construye el interface.
    void createPreviousConfig(std::string config_file); 
    void createVelocineLidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id);
    void createSickLidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id);
    void createLeftMotorTelemetryData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id);
    void createRightMotorTelemetryData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id);
    void createOdometerData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id);
    void createTwistData(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id);

  };
}
#endif // ARGJ801_SENSORS_INTERFACE_HPP