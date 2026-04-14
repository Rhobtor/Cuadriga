#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "data/Composite.hpp"
#include <map>
#include <vector>
#include <string>

/**
 * @file Sensor.hpp
 * @brief Abstracción de un backend de sensores.
 *
 * La implementación concreta (p.ej. `ARGJ801SensorsInterface`) decide cómo obtener
 * los datos (LCM, RTSP, etc.).
 *
 * El nodo `Argj801SensorsNode` sólo llama a:
 * - start()/stop() cuando activa/desactiva.
 * - getFastData()/getSlowData() según timers.
 */
class Sensor
{
public:
  virtual std::string getID() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  /// Obtención/publicación de streams rápidos (lidars/cámara).
  virtual void getFastData(std::shared_ptr<Visitor::Visitor> visitor) = 0;
  /// Obtención/publicación de streams lentos (driveline/odómetro/twist).
  virtual void getSlowData(std::shared_ptr<Visitor::Visitor> visitor) = 0;

};

class SensorException : public std::exception {
  private:
    std::string error;
  public:
    SensorException(const std::string msg) : error(msg) {}

    const char* what() const noexcept override {
        return error.c_str();
    }
};
#endif // IMU_HPP