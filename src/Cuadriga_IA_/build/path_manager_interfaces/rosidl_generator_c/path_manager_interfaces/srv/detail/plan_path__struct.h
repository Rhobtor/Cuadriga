// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from path_manager_interfaces:srv/PlanPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__PLAN_PATH__STRUCT_H_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__PLAN_PATH__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'start'
// Member 'goal'
#include "geometry_msgs/msg/detail/point__struct.h"

/// Struct defined in srv/PlanPath in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__PlanPath_Request
{
  geometry_msgs__msg__Point start;
  geometry_msgs__msg__Point goal;
} path_manager_interfaces__srv__PlanPath_Request;

// Struct for a sequence of path_manager_interfaces__srv__PlanPath_Request.
typedef struct path_manager_interfaces__srv__PlanPath_Request__Sequence
{
  path_manager_interfaces__srv__PlanPath_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__PlanPath_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__struct.h"

/// Struct defined in srv/PlanPath in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__PlanPath_Response
{
  bool ack;
  nav_msgs__msg__Path path;
} path_manager_interfaces__srv__PlanPath_Response;

// Struct for a sequence of path_manager_interfaces__srv__PlanPath_Response.
typedef struct path_manager_interfaces__srv__PlanPath_Response__Sequence
{
  path_manager_interfaces__srv__PlanPath_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__PlanPath_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__PLAN_PATH__STRUCT_H_
