#ifndef ARGO_J8_KINEMATICMODEL_H
#define ARGO_J8_KINEMATICMODEL_H
#include <iostream>
#include <cmath>

/**
 * @file argj801_kinematic_model.hpp
 * @brief Modelo cinemático simple para convertir (speed, rotation) a (throttle, steering).
 *
 * Uso principal:
 * - Se usa en `CtlPlatformNode` (modos LCM/Gazebo) para convertir el comando
 *   `geometry_msgs/Twist` (v, w) a comandos de actuador, aplicando:
 *   - geometría del rover (effective_radius, xICR)
 *   - escalados `*_to_percent`
 *   - límites de aceleración (steer_acc, throttle_acc)
 */
class Argo_J8_KinematicModel {
public:
    Argo_J8_KinematicModel(double effective_radius, double xICR, double throttle_to_percent,
                             double steer_to_percent, double steer_acc, double throttle_acc, double desired_freq);

    ~Argo_J8_KinematicModel();

    /**
     * @brief Actualiza el modelo para un comando (speed, rotation).
     * @param speed velocidad lineal deseada (m/s típicamente).
     * @param rotation velocidad angular deseada (rad/s típicamente, yaw).
     */
    void update(double speed, double rotation);

    /// @return Throttle calculado (normalizado/porcentaje según parámetros).
    double getThrottle() const;
    /// @return Steering calculado (normalizado/porcentaje según parámetros).
    double getSteering() const;
    /// @return Aceleración de throttle solicitada antes de saturar.
    double getRequestedThrottleAcc() const;
    /// @return Aceleración de steering solicitada antes de saturar.
    double getRequestedSteeringAcc() const;
    /// @return true si el throttle quedó limitado en la última actualización.
    bool isThrottleLimited() const;
    /// @return true si el steering quedó limitado en la última actualización.
    bool isSteeringLimited() const;

private:
    double effective_radius_;
    double xICR_;
    double throttle_to_percent_;
    double steer_to_percent_;

    double throttle_;
    double steering_;
    double throttle_acc;
    double steer_acc;
    double desired_freq;
    double last_throttle_;
    double last_steering_;
    double requested_throttle_acc_;
    double requested_steer_acc_;
    bool throttle_limited_;
    bool steering_limited_;

    /// Implementación interna del cálculo + limitación por aceleración.
    void calculate(double speed, double rotation);
};

#endif // ARGO_J8_KINEMATICMODEL_H
