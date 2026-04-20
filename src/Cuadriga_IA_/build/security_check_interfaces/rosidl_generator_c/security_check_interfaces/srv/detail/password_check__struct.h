// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from security_check_interfaces:srv/PasswordCheck.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__STRUCT_H_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'password'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/PasswordCheck in the package security_check_interfaces.
typedef struct security_check_interfaces__srv__PasswordCheck_Request
{
  rosidl_runtime_c__String password;
} security_check_interfaces__srv__PasswordCheck_Request;

// Struct for a sequence of security_check_interfaces__srv__PasswordCheck_Request.
typedef struct security_check_interfaces__srv__PasswordCheck_Request__Sequence
{
  security_check_interfaces__srv__PasswordCheck_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} security_check_interfaces__srv__PasswordCheck_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/PasswordCheck in the package security_check_interfaces.
typedef struct security_check_interfaces__srv__PasswordCheck_Response
{
  bool success;
  rosidl_runtime_c__String message;
} security_check_interfaces__srv__PasswordCheck_Response;

// Struct for a sequence of security_check_interfaces__srv__PasswordCheck_Response.
typedef struct security_check_interfaces__srv__PasswordCheck_Response__Sequence
{
  security_check_interfaces__srv__PasswordCheck_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} security_check_interfaces__srv__PasswordCheck_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__STRUCT_H_
