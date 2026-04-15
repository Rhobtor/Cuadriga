// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from fixposition_driver_ros2:msg/Speed.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "fixposition_driver_ros2/msg/detail/speed__rosidl_typesupport_introspection_c.h"
#include "fixposition_driver_ros2/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "fixposition_driver_ros2/msg/detail/speed__functions.h"
#include "fixposition_driver_ros2/msg/detail/speed__struct.h"


// Include directives for member types
// Member `speeds`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  fixposition_driver_ros2__msg__Speed__init(message_memory);
}

void fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_fini_function(void * message_memory)
{
  fixposition_driver_ros2__msg__Speed__fini(message_memory);
}

size_t fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__size_function__Speed__speeds(
  const void * untyped_member)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return member->size;
}

const void * fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_const_function__Speed__speeds(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_function__Speed__speeds(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__fetch_function__Speed__speeds(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int32_t * item =
    ((const int32_t *)
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_const_function__Speed__speeds(untyped_member, index));
  int32_t * value =
    (int32_t *)(untyped_value);
  *value = *item;
}

void fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__assign_function__Speed__speeds(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int32_t * item =
    ((int32_t *)
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_function__Speed__speeds(untyped_member, index));
  const int32_t * value =
    (const int32_t *)(untyped_value);
  *item = *value;
}

bool fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__resize_function__Speed__speeds(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  rosidl_runtime_c__int32__Sequence__fini(member);
  return rosidl_runtime_c__int32__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_member_array[1] = {
  {
    "speeds",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(fixposition_driver_ros2__msg__Speed, speeds),  // bytes offset in struct
    NULL,  // default value
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__size_function__Speed__speeds,  // size() function pointer
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_const_function__Speed__speeds,  // get_const(index) function pointer
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__get_function__Speed__speeds,  // get(index) function pointer
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__fetch_function__Speed__speeds,  // fetch(index, &value) function pointer
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__assign_function__Speed__speeds,  // assign(index, value) function pointer
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__resize_function__Speed__speeds  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_members = {
  "fixposition_driver_ros2__msg",  // message namespace
  "Speed",  // message name
  1,  // number of fields
  sizeof(fixposition_driver_ros2__msg__Speed),
  fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_member_array,  // message members
  fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_init_function,  // function to initialize message memory (memory has to be allocated)
  fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_type_support_handle = {
  0,
  &fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_fixposition_driver_ros2
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, fixposition_driver_ros2, msg, Speed)() {
  if (!fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_type_support_handle.typesupport_identifier) {
    fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &fixposition_driver_ros2__msg__Speed__rosidl_typesupport_introspection_c__Speed_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
