// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ctl_mission_interfaces:srv/GetMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__STRUCT_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GetMode in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__GetMode_Request
{
  uint8_t structure_needs_at_least_one_member;
} ctl_mission_interfaces__srv__GetMode_Request;

// Struct for a sequence of ctl_mission_interfaces__srv__GetMode_Request.
typedef struct ctl_mission_interfaces__srv__GetMode_Request__Sequence
{
  ctl_mission_interfaces__srv__GetMode_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__GetMode_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/GetMode in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__GetMode_Response
{
  int8_t mode;
} ctl_mission_interfaces__srv__GetMode_Response;

// Struct for a sequence of ctl_mission_interfaces__srv__GetMode_Response.
typedef struct ctl_mission_interfaces__srv__GetMode_Response__Sequence
{
  ctl_mission_interfaces__srv__GetMode_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__GetMode_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__STRUCT_H_
