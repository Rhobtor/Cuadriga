// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ctl_mission_interfaces:srv/ChangeMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/ChangeMode in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ChangeMode_Request
{
  int8_t transition;
} ctl_mission_interfaces__srv__ChangeMode_Request;

// Struct for a sequence of ctl_mission_interfaces__srv__ChangeMode_Request.
typedef struct ctl_mission_interfaces__srv__ChangeMode_Request__Sequence
{
  ctl_mission_interfaces__srv__ChangeMode_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ChangeMode_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/ChangeMode in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ChangeMode_Response
{
  bool success;
} ctl_mission_interfaces__srv__ChangeMode_Response;

// Struct for a sequence of ctl_mission_interfaces__srv__ChangeMode_Response.
typedef struct ctl_mission_interfaces__srv__ChangeMode_Response__Sequence
{
  ctl_mission_interfaces__srv__ChangeMode_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ChangeMode_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_H_
