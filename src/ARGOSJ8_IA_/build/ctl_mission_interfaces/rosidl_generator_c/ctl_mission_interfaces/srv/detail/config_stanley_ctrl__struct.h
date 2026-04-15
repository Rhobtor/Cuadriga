// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ctl_mission_interfaces:srv/ConfigStanleyCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/ConfigStanleyCtrl in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request
{
  float v_forward;
  float l_ahead_dist;
  float k_error_lineal;
  float k_error_angular;
} ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request;

// Struct for a sequence of ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request.
typedef struct ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence
{
  ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/ConfigStanleyCtrl in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response
{
  bool ack;
} ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response;

// Struct for a sequence of ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response.
typedef struct ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence
{
  ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_H_
