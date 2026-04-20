// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ctl_mission_interfaces:srv/ConfigRegulatedPureCtrl.idl
// generated code does not contain a copyright notice
#include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ctl_mission_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__struct.h"
#include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _ConfigRegulatedPureCtrl_Request__ros_msg_type = ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request;

static bool _ConfigRegulatedPureCtrl_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigRegulatedPureCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigRegulatedPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: look_ahead_dis
  {
    cdr << ros_message->look_ahead_dis;
  }

  // Field name: v_forward
  {
    cdr << ros_message->v_forward;
  }

  // Field name: r_min
  {
    cdr << ros_message->r_min;
  }

  return true;
}

static bool _ConfigRegulatedPureCtrl_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigRegulatedPureCtrl_Request__ros_msg_type * ros_message = static_cast<_ConfigRegulatedPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: look_ahead_dis
  {
    cdr >> ros_message->look_ahead_dis;
  }

  // Field name: v_forward
  {
    cdr >> ros_message->v_forward;
  }

  // Field name: r_min
  {
    cdr >> ros_message->r_min;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigRegulatedPureCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigRegulatedPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name look_ahead_dis
  {
    size_t item_size = sizeof(ros_message->look_ahead_dis);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name v_forward
  {
    size_t item_size = sizeof(ros_message->v_forward);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name r_min
  {
    size_t item_size = sizeof(ros_message->r_min);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ConfigRegulatedPureCtrl_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // member: look_ahead_dis
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: v_forward
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: r_min
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request;
    is_plain =
      (
      offsetof(DataType, r_min) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigRegulatedPureCtrl_Request__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigRegulatedPureCtrl_Request = {
  "ctl_mission_interfaces::srv",
  "ConfigRegulatedPureCtrl_Request",
  _ConfigRegulatedPureCtrl_Request__cdr_serialize,
  _ConfigRegulatedPureCtrl_Request__cdr_deserialize,
  _ConfigRegulatedPureCtrl_Request__get_serialized_size,
  _ConfigRegulatedPureCtrl_Request__max_serialized_size
};

static rosidl_message_type_support_t _ConfigRegulatedPureCtrl_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigRegulatedPureCtrl_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigRegulatedPureCtrl_Request)() {
  return &_ConfigRegulatedPureCtrl_Request__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "ctl_mission_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _ConfigRegulatedPureCtrl_Response__ros_msg_type = ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response;

static bool _ConfigRegulatedPureCtrl_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigRegulatedPureCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigRegulatedPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    cdr << (ros_message->ack ? true : false);
  }

  return true;
}

static bool _ConfigRegulatedPureCtrl_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigRegulatedPureCtrl_Response__ros_msg_type * ros_message = static_cast<_ConfigRegulatedPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->ack = tmp ? true : false;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigRegulatedPureCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigRegulatedPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name ack
  {
    size_t item_size = sizeof(ros_message->ack);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ConfigRegulatedPureCtrl_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // member: ack
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response;
    is_plain =
      (
      offsetof(DataType, ack) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigRegulatedPureCtrl_Response__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigRegulatedPureCtrl_Response = {
  "ctl_mission_interfaces::srv",
  "ConfigRegulatedPureCtrl_Response",
  _ConfigRegulatedPureCtrl_Response__cdr_serialize,
  _ConfigRegulatedPureCtrl_Response__cdr_deserialize,
  _ConfigRegulatedPureCtrl_Response__get_serialized_size,
  _ConfigRegulatedPureCtrl_Response__max_serialized_size
};

static rosidl_message_type_support_t _ConfigRegulatedPureCtrl_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigRegulatedPureCtrl_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigRegulatedPureCtrl_Response)() {
  return &_ConfigRegulatedPureCtrl_Response__type_support;
}

#if defined(__cplusplus)
}
#endif

#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "ctl_mission_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ctl_mission_interfaces/srv/config_regulated_pure_ctrl.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t ConfigRegulatedPureCtrl__callbacks = {
  "ctl_mission_interfaces::srv",
  "ConfigRegulatedPureCtrl",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigRegulatedPureCtrl_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigRegulatedPureCtrl_Response)(),
};

static rosidl_service_type_support_t ConfigRegulatedPureCtrl__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &ConfigRegulatedPureCtrl__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigRegulatedPureCtrl)() {
  return &ConfigRegulatedPureCtrl__handle;
}

#if defined(__cplusplus)
}
#endif
