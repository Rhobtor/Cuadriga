// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from path_manager_interfaces:srv/ReadPathFromFile.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_H_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_H_

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

/// Struct defined in srv/ReadPathFromFile in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__ReadPathFromFile_Request
{
  rosidl_runtime_c__String filename;
} path_manager_interfaces__srv__ReadPathFromFile_Request;

// Struct for a sequence of path_manager_interfaces__srv__ReadPathFromFile_Request.
typedef struct path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence
{
  path_manager_interfaces__srv__ReadPathFromFile_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/ReadPathFromFile in the package path_manager_interfaces.
typedef struct path_manager_interfaces__srv__ReadPathFromFile_Response
{
  bool success;
} path_manager_interfaces__srv__ReadPathFromFile_Response;

// Struct for a sequence of path_manager_interfaces__srv__ReadPathFromFile_Response.
typedef struct path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence
{
  path_manager_interfaces__srv__ReadPathFromFile_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_H_
