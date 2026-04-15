// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from security_check_interfaces:srv/GetSecurityCheckStatus.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__STRUCT_H_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GetSecurityCheckStatus in the package security_check_interfaces.
typedef struct security_check_interfaces__srv__GetSecurityCheckStatus_Request
{
  uint8_t structure_needs_at_least_one_member;
} security_check_interfaces__srv__GetSecurityCheckStatus_Request;

// Struct for a sequence of security_check_interfaces__srv__GetSecurityCheckStatus_Request.
typedef struct security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence
{
  security_check_interfaces__srv__GetSecurityCheckStatus_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/GetSecurityCheckStatus in the package security_check_interfaces.
typedef struct security_check_interfaces__srv__GetSecurityCheckStatus_Response
{
  bool status;
} security_check_interfaces__srv__GetSecurityCheckStatus_Response;

// Struct for a sequence of security_check_interfaces__srv__GetSecurityCheckStatus_Response.
typedef struct security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence
{
  security_check_interfaces__srv__GetSecurityCheckStatus_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__STRUCT_H_
