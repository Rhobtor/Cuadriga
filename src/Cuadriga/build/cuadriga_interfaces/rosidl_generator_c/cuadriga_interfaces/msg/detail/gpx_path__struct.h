// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from cuadriga_interfaces:msg/GPXPath.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__STRUCT_H_
#define CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'waypoints'
#include "sensor_msgs/msg/detail/nav_sat_fix__struct.h"

/// Struct defined in msg/GPXPath in the package cuadriga_interfaces.
typedef struct cuadriga_interfaces__msg__GPXPath
{
  sensor_msgs__msg__NavSatFix__Sequence waypoints;
} cuadriga_interfaces__msg__GPXPath;

// Struct for a sequence of cuadriga_interfaces__msg__GPXPath.
typedef struct cuadriga_interfaces__msg__GPXPath__Sequence
{
  cuadriga_interfaces__msg__GPXPath * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cuadriga_interfaces__msg__GPXPath__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__STRUCT_H_
