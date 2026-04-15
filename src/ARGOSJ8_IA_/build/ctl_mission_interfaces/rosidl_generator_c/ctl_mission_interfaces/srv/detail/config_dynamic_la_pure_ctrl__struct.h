// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicLAPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__STRUCT_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/ConfigDynamicLAPureCtrl in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request
{
  float look_ahead_v_gain;
  float max_v_forward;
  float max_ang_acc;
  float max_ang_dec;
  float max_lin_acc;
  float max_lin_dec;
  float speed_pow;
  float min_look_ahead_d;
} ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request;

// Struct for a sequence of ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request.
typedef struct ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence
{
  ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/ConfigDynamicLAPureCtrl in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response
{
  bool ack;
} ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response;

// Struct for a sequence of ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response.
typedef struct ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence
{
  ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__STRUCT_H_
