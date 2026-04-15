// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from cuadriga_interfaces:msg/GPXPath.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "cuadriga_interfaces/msg/detail/gpx_path__rosidl_typesupport_introspection_c.h"
#include "cuadriga_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "cuadriga_interfaces/msg/detail/gpx_path__functions.h"
#include "cuadriga_interfaces/msg/detail/gpx_path__struct.h"


// Include directives for member types
// Member `waypoints`
#include "sensor_msgs/msg/nav_sat_fix.h"
// Member `waypoints`
#include "sensor_msgs/msg/detail/nav_sat_fix__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  cuadriga_interfaces__msg__GPXPath__init(message_memory);
}

void cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_fini_function(void * message_memory)
{
  cuadriga_interfaces__msg__GPXPath__fini(message_memory);
}

size_t cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__size_function__GPXPath__waypoints(
  const void * untyped_member)
{
  const sensor_msgs__msg__NavSatFix__Sequence * member =
    (const sensor_msgs__msg__NavSatFix__Sequence *)(untyped_member);
  return member->size;
}

const void * cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_const_function__GPXPath__waypoints(
  const void * untyped_member, size_t index)
{
  const sensor_msgs__msg__NavSatFix__Sequence * member =
    (const sensor_msgs__msg__NavSatFix__Sequence *)(untyped_member);
  return &member->data[index];
}

void * cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_function__GPXPath__waypoints(
  void * untyped_member, size_t index)
{
  sensor_msgs__msg__NavSatFix__Sequence * member =
    (sensor_msgs__msg__NavSatFix__Sequence *)(untyped_member);
  return &member->data[index];
}

void cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__fetch_function__GPXPath__waypoints(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const sensor_msgs__msg__NavSatFix * item =
    ((const sensor_msgs__msg__NavSatFix *)
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_const_function__GPXPath__waypoints(untyped_member, index));
  sensor_msgs__msg__NavSatFix * value =
    (sensor_msgs__msg__NavSatFix *)(untyped_value);
  *value = *item;
}

void cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__assign_function__GPXPath__waypoints(
  void * untyped_member, size_t index, const void * untyped_value)
{
  sensor_msgs__msg__NavSatFix * item =
    ((sensor_msgs__msg__NavSatFix *)
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_function__GPXPath__waypoints(untyped_member, index));
  const sensor_msgs__msg__NavSatFix * value =
    (const sensor_msgs__msg__NavSatFix *)(untyped_value);
  *item = *value;
}

bool cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__resize_function__GPXPath__waypoints(
  void * untyped_member, size_t size)
{
  sensor_msgs__msg__NavSatFix__Sequence * member =
    (sensor_msgs__msg__NavSatFix__Sequence *)(untyped_member);
  sensor_msgs__msg__NavSatFix__Sequence__fini(member);
  return sensor_msgs__msg__NavSatFix__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_member_array[1] = {
  {
    "waypoints",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(cuadriga_interfaces__msg__GPXPath, waypoints),  // bytes offset in struct
    NULL,  // default value
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__size_function__GPXPath__waypoints,  // size() function pointer
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_const_function__GPXPath__waypoints,  // get_const(index) function pointer
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__get_function__GPXPath__waypoints,  // get(index) function pointer
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__fetch_function__GPXPath__waypoints,  // fetch(index, &value) function pointer
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__assign_function__GPXPath__waypoints,  // assign(index, value) function pointer
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__resize_function__GPXPath__waypoints  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_members = {
  "cuadriga_interfaces__msg",  // message namespace
  "GPXPath",  // message name
  1,  // number of fields
  sizeof(cuadriga_interfaces__msg__GPXPath),
  cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_member_array,  // message members
  cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_init_function,  // function to initialize message memory (memory has to be allocated)
  cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_type_support_handle = {
  0,
  &cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_cuadriga_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, cuadriga_interfaces, msg, GPXPath)() {
  cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sensor_msgs, msg, NavSatFix)();
  if (!cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_type_support_handle.typesupport_identifier) {
    cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &cuadriga_interfaces__msg__GPXPath__rosidl_typesupport_introspection_c__GPXPath_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
