#include "argj801_ctl_platform/argj801_kinematic_model.hpp"

// Implementación del modelo cinemático.
//
// Modelo diferencial simplificado:
//   w_r = (v + w * xICR) / R
//   w_l = (v - w * xICR) / R
// y a partir de ahí:
//   throttle_speed  = (w_r + w_l) / 2
//   steering_speed  = (w_r - w_l) / 2
//
// Luego se aplican límites de aceleración (steer_acc / throttle_acc) para evitar
// cambios bruscos de mando.
//
// Nota: El código actual fija t_step a 50Hz. Existe `desired_freq` en el objeto,
// pero en esta implementación no se usa. No lo cambiamos por política de
// "comentarios únicamente".

Argo_J8_KinematicModel::Argo_J8_KinematicModel(double effective_radius, double xICR, double throttle_to_percent,
                             double steer_to_percent, double steer_acc, double throttle_acc, double desired_freq)
    : effective_radius_(effective_radius), xICR_(xICR), throttle_to_percent_(throttle_to_percent), steer_to_percent_(steer_to_percent), 
    throttle_(0), steering_(0), steer_acc(steer_acc), throttle_acc(throttle_acc), desired_freq(desired_freq), last_throttle_(0), last_steering_(0),
    requested_throttle_acc_(0), requested_steer_acc_(0), throttle_limited_(false), steering_limited_(false) {
}

Argo_J8_KinematicModel::~Argo_J8_KinematicModel() {
}

void Argo_J8_KinematicModel::update(double speed, double rotation) {
    calculate(speed, rotation);
}

double Argo_J8_KinematicModel::getThrottle() const {
    return throttle_;
}

double Argo_J8_KinematicModel::getSteering() const {
    return steering_;
}

double Argo_J8_KinematicModel::getRequestedThrottleAcc() const {
    return requested_throttle_acc_;
}

double Argo_J8_KinematicModel::getRequestedSteeringAcc() const {
    return requested_steer_acc_;
}

bool Argo_J8_KinematicModel::isThrottleLimited() const {
    return throttle_limited_;
}

bool Argo_J8_KinematicModel::isSteeringLimited() const {
    return steering_limited_;
}

void Argo_J8_KinematicModel::calculate(double speed, double rotation) {
    // Calculate wheel velocities
    double w_right_wheel = (speed + rotation * xICR_) / effective_radius_;
    double w_left_wheel = (speed - rotation * xICR_) / effective_radius_;

    // Calculate throttle and steering speed based on wheel speeds
    double throttle_speed_ = (w_right_wheel + w_left_wheel) / 2;
    double steering_speed_ = (w_right_wheel - w_left_wheel) / 2;

    // Time step for the control loop (fijado a 50Hz).
    double t_step = 1 / 50.0;  // 50 Hz control loop

    // Calculate the requested acceleration for throttle and steering
    requested_throttle_acc_ = (throttle_speed_ - last_throttle_) / t_step;
    requested_steer_acc_ = (steering_speed_ - last_steering_) / t_step;
    throttle_limited_ = false;
    steering_limited_ = false;

    // Limitación por aceleración solicitada (throttle).
    // Sólo limitamos cuando aumenta el valor absoluto, para permitir frenadas
    // más agresivas si hiciera falta.
    if (fabs(throttle_speed_) > fabs(last_throttle_) && fabs(requested_throttle_acc_) > throttle_acc) {
        // Throttle is increasing in absolute value and exceeds the allowed acceleration
        double throttle_change = std::copysign(throttle_acc * t_step, throttle_speed_ - last_throttle_);
        throttle_speed_ = last_throttle_ + throttle_change;
        throttle_limited_ = true;
        std::cout << "Throttle limited to: " << throttle_speed_ * throttle_to_percent_ << std::endl;
    } 

    // Limitación por aceleración solicitada (steering).
    if (fabs(steering_speed_) > fabs(last_steering_) && fabs(requested_steer_acc_) > steer_acc) {
        // Steering is increasing in absolute value and exceeds the allowed acceleration
        double steering_change = std::copysign(steer_acc * t_step, steering_speed_ - last_steering_);
        steering_speed_ = last_steering_ + steering_change;
        steering_limited_ = true;
        std::cout << "Steering limited to: " << steering_speed_ * steer_to_percent_ << std::endl;
    } 

    // Update the last throttle and steering values for the next iteration
    last_throttle_ = throttle_speed_;
    last_steering_ = steering_speed_;
    throttle_ = throttle_speed_ * throttle_to_percent_;
    steering_ = steering_speed_ * steer_to_percent_;

    // Debug output.
    // Ojo: estos prints pueden inundar consola a 50Hz.
    std::cout << "Throttle: " << throttle_ << std::endl;
    std::cout << "Steering: " << steering_ << std::endl;
    std::cout << "Requested Throttle Acc: " << requested_throttle_acc_ << std::endl;
    std::cout << "Requested Steering Acc: " << requested_steer_acc_ << std::endl;
}



