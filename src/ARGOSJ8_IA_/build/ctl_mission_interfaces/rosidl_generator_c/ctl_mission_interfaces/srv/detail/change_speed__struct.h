// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ctl_mission_interfaces:srv/ChangeSpeed.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_SPEED__STRUCT_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_SPEED__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/ChangeSpeed in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ChangeSpeed_Request
{
  float speed;
} ctl_mission_interfaces__srv__ChangeSpeed_Request;

// Struct for a sequence of ctl_mission_interfaces__srv__ChangeSpeed_Request.
typedef struct ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence
{
  ctl_mission_interfaces__srv__ChangeSpeed_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/ChangeSpeed in the package ctl_mission_interfaces.
typedef struct ctl_mission_interfaces__srv__ChangeSpeed_Response
{
  bool succes;
} ctl_mission_interfaces__srv__ChangeSpeed_Response;

// Struct for a sequence of ctl_mission_interfaces__srv__ChangeSpeed_Response.
typedef struct ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence
{
  ctl_mission_interfaces__srv__ChangeSpeed_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_SPEED__STRUCT_H_
