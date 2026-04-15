// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace cuadriga_ctl_platform_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void KinematicDebugMsg_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg(_init);
}

void KinematicDebugMsg_fini_function(void * message_memory)
{
  auto typed_message = static_cast<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg *>(message_memory);
  typed_message->~KinematicDebugMsg();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember KinematicDebugMsg_message_member_array[6] = {
  {
    "throttle",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, throttle),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "steering",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, steering),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "requested_throttle_acc",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, requested_throttle_acc),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "requested_steering_acc",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, requested_steering_acc),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "throttle_limited",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, throttle_limited),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "steering_limited",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg, steering_limited),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers KinematicDebugMsg_message_members = {
  "cuadriga_ctl_platform_interfaces::msg",  // message namespace
  "KinematicDebugMsg",  // message name
  6,  // number of fields
  sizeof(cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg),
  KinematicDebugMsg_message_member_array,  // message members
  KinematicDebugMsg_init_function,  // function to initialize message memory (memory has to be allocated)
  KinematicDebugMsg_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t KinematicDebugMsg_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &KinematicDebugMsg_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace cuadriga_ctl_platform_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>()
{
  return &::cuadriga_ctl_platform_interfaces::msg::rosidl_typesupport_introspection_cpp::KinematicDebugMsg_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cuadriga_ctl_platform_interfaces, msg, KinematicDebugMsg)() {
  return &::cuadriga_ctl_platform_interfaces::msg::rosidl_typesupport_introspection_cpp::KinematicDebugMsg_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
