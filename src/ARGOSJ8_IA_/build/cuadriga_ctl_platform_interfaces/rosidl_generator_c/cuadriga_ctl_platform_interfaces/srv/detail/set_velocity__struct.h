// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from cuadriga_ctl_platform_interfaces:srv/SetVelocity.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__SET_VELOCITY__STRUCT_H_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__SET_VELOCITY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetVelocity in the package cuadriga_ctl_platform_interfaces.
typedef struct cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request
{
  int8_t new_velocity;
} cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request;

// Struct for a sequence of cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request.
typedef struct cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence
{
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/SetVelocity in the package cuadriga_ctl_platform_interfaces.
typedef struct cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response
{
  int8_t actual_velocity;
} cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response;

// Struct for a sequence of cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response.
typedef struct cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence
{
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__SET_VELOCITY__STRUCT_H_
