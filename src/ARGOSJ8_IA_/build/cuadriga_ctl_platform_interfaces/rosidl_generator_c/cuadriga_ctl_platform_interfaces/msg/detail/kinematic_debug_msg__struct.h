// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_H_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/KinematicDebugMsg in the package cuadriga_ctl_platform_interfaces.
/**
  * KinematicDebugMsg.msg
  *
  * Estado de depuracion del modelo cinemático usado para generar
  * el mando low-level a partir de cmd_vel.
  *
  * Todos los campos escalarizados de throttle/steering se publican en las
  * mismas unidades que CmdThrottleMsg. Las aceleraciones solicitadas se
  * expresan antes de aplicar la saturacion configurada.
 */
typedef struct cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
{
  float throttle;
  float steering;
  float requested_throttle_acc;
  float requested_steering_acc;
  bool throttle_limited;
  bool steering_limited;
} cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg;

// Struct for a sequence of cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg.
typedef struct cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence
{
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_H_
