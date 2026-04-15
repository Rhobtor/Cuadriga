// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from path_manager_interfaces:srv/WritePathToFile.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__STRUCT_H_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'filename'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/WritePathToFile in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__WritePathToFile_Request
{
  rosidl_runtime_c__String filename;
} path_manager_interfaces__srv__WritePathToFile_Request;

// Struct for a sequence of path_manager_interfaces__srv__WritePathToFile_Request.
typedef struct path_manager_interfaces__srv__WritePathToFile_Request__Sequence
{
  path_manager_interfaces__srv__WritePathToFile_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__WritePathToFile_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/WritePathToFile in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__WritePathToFile_Response
{
  bool success;
} path_manager_interfaces__srv__WritePathToFile_Response;

// Struct for a sequence of path_manager_interfaces__srv__WritePathToFile_Response.
typedef struct path_manager_interfaces__srv__WritePathToFile_Response__Sequence
{
  path_manager_interfaces__srv__WritePathToFile_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__WritePathToFile_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__STRUCT_H_
