// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ctl_mission_interfaces:srv/ConfigStanleyCtrl.idl
// generated code does not contain a copyright notice
#include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ctl_mission_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__struct.h"
#include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__functions.h"
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


using _ConfigStanleyCtrl_Request__ros_msg_type = ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request;

static bool _ConfigStanleyCtrl_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigStanleyCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigStanleyCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: v_forward
  {
    cdr << ros_message->v_forward;
  }

  // Field name: l_ahead_dist
  {
    cdr << ros_message->l_ahead_dist;
  }

  // Field name: k_error_lineal
  {
    cdr << ros_message->k_error_lineal;
  }

  // Field name: k_error_angular
  {
    cdr << ros_message->k_error_angular;
  }

  return true;
}

static bool _ConfigStanleyCtrl_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigStanleyCtrl_Request__ros_msg_type * ros_message = static_cast<_ConfigStanleyCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: v_forward
  {
    cdr >> ros_message->v_forward;
  }

  // Field name: l_ahead_dist
  {
    cdr >> ros_message->l_ahead_dist;
  }

  // Field name: k_error_lineal
  {
    cdr >> ros_message->k_error_lineal;
  }

  // Field name: k_error_angular
  {
    cdr >> ros_message->k_error_angular;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigStanleyCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigStanleyCtrl_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name v_forward
  {
    size_t item_size = sizeof(ros_message->v_forward);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name l_ahead_dist
  {
    size_t item_size = sizeof(ros_message->l_ahead_dist);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name k_error_lineal
  {
    size_t item_size = sizeof(ros_message->k_error_lineal);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name k_error_angular
  {
    size_t item_size = sizeof(ros_message->k_error_angular);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ConfigStanleyCtrl_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request(
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

  // member: v_forward
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: l_ahead_dist
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: k_error_lineal
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: k_error_angular
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
    using DataType = ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request;
    is_plain =
      (
      offsetof(DataType, k_error_angular) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigStanleyCtrl_Request__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigStanleyCtrl_Request = {
  "ctl_mission_interfaces::srv",
  "ConfigStanleyCtrl_Request",
  _ConfigStanleyCtrl_Request__cdr_serialize,
  _ConfigStanleyCtrl_Request__cdr_deserialize,
  _ConfigStanleyCtrl_Request__get_serialized_size,
  _ConfigStanleyCtrl_Request__max_serialized_size
};

static rosidl_message_type_support_t _ConfigStanleyCtrl_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigStanleyCtrl_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigStanleyCtrl_Request)() {
  return &_ConfigStanleyCtrl_Request__type_support;
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
// #include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__functions.h"
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


using _ConfigStanleyCtrl_Response__ros_msg_type = ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response;

static bool _ConfigStanleyCtrl_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigStanleyCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigStanleyCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    cdr << (ros_message->ack ? true : false);
  }

  return true;
}

static bool _ConfigStanleyCtrl_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigStanleyCtrl_Response__ros_msg_type * ros_message = static_cast<_ConfigStanleyCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->ack = tmp ? true : false;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigStanleyCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigStanleyCtrl_Response__ros_msg_type *>(untyped_ros_message);
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

static uint32_t _ConfigStanleyCtrl_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response(
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
    using DataType = ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response;
    is_plain =
      (
      offsetof(DataType, ack) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigStanleyCtrl_Response__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigStanleyCtrl_Response = {
  "ctl_mission_interfaces::srv",
  "ConfigStanleyCtrl_Response",
  _ConfigStanleyCtrl_Response__cdr_serialize,
  _ConfigStanleyCtrl_Response__cdr_deserialize,
  _ConfigStanleyCtrl_Response__get_serialized_size,
  _ConfigStanleyCtrl_Response__max_serialized_size
};

static rosidl_message_type_support_t _ConfigStanleyCtrl_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigStanleyCtrl_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigStanleyCtrl_Response)() {
  return &_ConfigStanleyCtrl_Response__type_support;
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
#include "ctl_mission_interfaces/srv/config_stanley_ctrl.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t ConfigStanleyCtrl__callbacks = {
  "ctl_mission_interfaces::srv",
  "ConfigStanleyCtrl",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigStanleyCtrl_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigStanleyCtrl_Response)(),
};

static rosidl_service_type_support_t ConfigStanleyCtrl__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &ConfigStanleyCtrl__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigStanleyCtrl)() {
  return &ConfigStanleyCtrl__handle;
}

#if defined(__cplusplus)
}
#endif
