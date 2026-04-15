// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__rosidl_typesupport_introspection_c.h"
#include "cuadriga_ctl_platform_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__functions.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init(message_memory);
}

void cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_fini_function(void * message_memory)
{
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_member_array[6] = {
  {
    "throttle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, throttle),  // bytes offset in struct
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
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, steering),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "requested_throttle_acc",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, requested_throttle_acc),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "requested_steering_acc",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, requested_steering_acc),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "throttle_limited",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, throttle_limited),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "steering_limited",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg, steering_limited),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_members = {
  "cuadriga_ctl_platform_interfaces__msg",  // message namespace
  "KinematicDebugMsg",  // message name
  6,  // number of fields
  sizeof(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg),
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_member_array,  // message members
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_init_function,  // function to initialize message memory (memory has to be allocated)
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_type_support_handle = {
  0,
  &cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_cuadriga_ctl_platform_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, cuadriga_ctl_platform_interfaces, msg, KinematicDebugMsg)() {
  if (!cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_type_support_handle.typesupport_identifier) {
    cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__rosidl_typesupport_introspection_c__KinematicDebugMsg_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
