#ifndef SENSORBUILDER_H
#define SENSORBUILDER_H
#include <memory>
#include "rclcpp/publisher.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "argj801_sensors_msgs/msg/motor_telemetry.hpp"
#include "argj801_sensors_msgs/msg/odometer.hpp"
#include "argj801_sensors/Sensor.hpp"

/**
 * @file SensorBuilder.hpp
 * @brief Interfaz Builder usada por Argj801SensorsNode para construir sensores y publicadores.
 *
 * Este patrón permite reutilizar el mismo nodo con distintos backends (p.ej.
 * un backend LCM para lidar/driveline o un backend de cámara RTSP), seleccionando
 * componentes vía parámetros.
 */

namespace Builder {
	class SensorBuilder
	{
	public:
    /// Carga configuración previa del backend (YAML LCM o URL RTSP según el caso).
    virtual void buildPreviousConfig(std::string url) = 0;
    /// Construye Velodyne LaserScan + piping hacia el visitor.
    virtual void buildVelocine(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) = 0;
    /// Construye SICK LaserScan + piping hacia el visitor.
    virtual void buildSick(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) = 0;
    /// Construye TwistStamped (driveline).
    virtual void buildTwist(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id) = 0;
    /// Construye Odometer.
    virtual void buildOdometer(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id) = 0;
    /// Construye telemetría motor izquierdo.
    virtual void buildMotorTelemetryLeft(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) = 0;
    /// Construye telemetría motor derecho.
    virtual void buildMotorTelemetryRigth(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) = 0;
    /// Construye cámara (publica CompressedImage).
    virtual void buildCamera(rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher, std::string frame_id,int compresion_ratio, bool resize) = 0;
    /// @return instancia Sensor con el grafo de datos configurado.
    virtual std::shared_ptr<Sensor> getSensor() = 0;
	};

  class NotAvailableOptionException  : public std::exception {
    private:
      std::string error;
    public:
      NotAvailableOptionException(const std::string msg) : error(msg) {}

      const char* what() const noexcept override {
          return error.c_str();
      }
  };
}
#endif // SENSORBUILDER_H