// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from cuadriga_ctl_platform_interfaces:srv/EmergencyStop.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__EMERGENCY_STOP__STRUCT_H_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__EMERGENCY_STOP__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/EmergencyStop in the package cuadriga_ctl_platform_interfaces.
typedef struct cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request
{
  bool stop;
} cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request;

// Struct for a sequence of cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request.
typedef struct cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence
{
  cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/EmergencyStop in the package cuadriga_ctl_platform_interfaces.
typedef struct cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response
{
  uint8_t structure_needs_at_least_one_member;
} cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response;

// Struct for a sequence of cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response.
typedef struct cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence
{
  cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__EMERGENCY_STOP__STRUCT_H_
