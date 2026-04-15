// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from cuadriga_ctl_platform_interfaces:msg/CmdThrottleMsg.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "cuadriga_ctl_platform_interfaces/msg/detail/cmd_throttle_msg__rosidl_typesupport_introspection_c.h"
#include "cuadriga_ctl_platform_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/cmd_throttle_msg__functions.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/cmd_throttle_msg__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__init(message_memory);
}

void cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_fini_function(void * message_memory)
{
  cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_member_array[2] = {
  {
    "throttle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg, throttle),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "steering",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg, steering),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_members = {
  "cuadriga_ctl_platform_interfaces__msg",  // message namespace
  "CmdThrottleMsg",  // message name
  2,  // number of fields
  sizeof(cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg),
  cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_member_array,  // message members
  cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_init_function,  // function to initialize message memory (memory has to be allocated)
  cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_type_support_handle = {
  0,
  &cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_cuadriga_ctl_platform_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, cuadriga_ctl_platform_interfaces, msg, CmdThrottleMsg)() {
  if (!cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_type_support_handle.typesupport_identifier) {
    cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__rosidl_typesupport_introspection_c__CmdThrottleMsg_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
