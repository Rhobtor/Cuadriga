// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicPureCtrl.idl
// generated code does not contain a copyright notice
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ctl_mission_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"
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


using _ConfigDynamicPureCtrl_Request__ros_msg_type = ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request;

static bool _ConfigDynamicPureCtrl_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigDynamicPureCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigDynamicPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: look_ahead_dis
  {
    cdr << ros_message->look_ahead_dis;
  }

  // Field name: max_v_forward
  {
    cdr << ros_message->max_v_forward;
  }

  // Field name: max_ang_acc
  {
    cdr << ros_message->max_ang_acc;
  }

  // Field name: max_ang_dec
  {
    cdr << ros_message->max_ang_dec;
  }

  // Field name: max_lin_acc
  {
    cdr << ros_message->max_lin_acc;
  }

  // Field name: max_lin_dec
  {
    cdr << ros_message->max_lin_dec;
  }

  return true;
}

static bool _ConfigDynamicPureCtrl_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigDynamicPureCtrl_Request__ros_msg_type * ros_message = static_cast<_ConfigDynamicPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: look_ahead_dis
  {
    cdr >> ros_message->look_ahead_dis;
  }

  // Field name: max_v_forward
  {
    cdr >> ros_message->max_v_forward;
  }

  // Field name: max_ang_acc
  {
    cdr >> ros_message->max_ang_acc;
  }

  // Field name: max_ang_dec
  {
    cdr >> ros_message->max_ang_dec;
  }

  // Field name: max_lin_acc
  {
    cdr >> ros_message->max_lin_acc;
  }

  // Field name: max_lin_dec
  {
    cdr >> ros_message->max_lin_dec;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigDynamicPureCtrl_Request__ros_msg_type * ros_message = static_cast<const _ConfigDynamicPureCtrl_Request__ros_msg_type *>(untyped_ros_message);
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
  // field.name max_v_forward
  {
    size_t item_size = sizeof(ros_message->max_v_forward);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name max_ang_acc
  {
    size_t item_size = sizeof(ros_message->max_ang_acc);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name max_ang_dec
  {
    size_t item_size = sizeof(ros_message->max_ang_dec);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name max_lin_acc
  {
    size_t item_size = sizeof(ros_message->max_lin_acc);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name max_lin_dec
  {
    size_t item_size = sizeof(ros_message->max_lin_dec);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _ConfigDynamicPureCtrl_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request(
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
  // member: max_v_forward
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: max_ang_acc
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: max_ang_dec
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: max_lin_acc
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: max_lin_dec
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
    using DataType = ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request;
    is_plain =
      (
      offsetof(DataType, max_lin_dec) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigDynamicPureCtrl_Request__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigDynamicPureCtrl_Request = {
  "ctl_mission_interfaces::srv",
  "ConfigDynamicPureCtrl_Request",
  _ConfigDynamicPureCtrl_Request__cdr_serialize,
  _ConfigDynamicPureCtrl_Request__cdr_deserialize,
  _ConfigDynamicPureCtrl_Request__get_serialized_size,
  _ConfigDynamicPureCtrl_Request__max_serialized_size
};

static rosidl_message_type_support_t _ConfigDynamicPureCtrl_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigDynamicPureCtrl_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Request)() {
  return &_ConfigDynamicPureCtrl_Request__type_support;
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
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"
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


using _ConfigDynamicPureCtrl_Response__ros_msg_type = ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response;

static bool _ConfigDynamicPureCtrl_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _ConfigDynamicPureCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigDynamicPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    cdr << (ros_message->ack ? true : false);
  }

  return true;
}

static bool _ConfigDynamicPureCtrl_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _ConfigDynamicPureCtrl_Response__ros_msg_type * ros_message = static_cast<_ConfigDynamicPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: ack
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->ack = tmp ? true : false;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t get_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ConfigDynamicPureCtrl_Response__ros_msg_type * ros_message = static_cast<const _ConfigDynamicPureCtrl_Response__ros_msg_type *>(untyped_ros_message);
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

static uint32_t _ConfigDynamicPureCtrl_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ctl_mission_interfaces
size_t max_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response(
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
    using DataType = ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response;
    is_plain =
      (
      offsetof(DataType, ack) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _ConfigDynamicPureCtrl_Response__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ConfigDynamicPureCtrl_Response = {
  "ctl_mission_interfaces::srv",
  "ConfigDynamicPureCtrl_Response",
  _ConfigDynamicPureCtrl_Response__cdr_serialize,
  _ConfigDynamicPureCtrl_Response__cdr_deserialize,
  _ConfigDynamicPureCtrl_Response__get_serialized_size,
  _ConfigDynamicPureCtrl_Response__max_serialized_size
};

static rosidl_message_type_support_t _ConfigDynamicPureCtrl_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ConfigDynamicPureCtrl_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Response)() {
  return &_ConfigDynamicPureCtrl_Response__type_support;
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
#include "ctl_mission_interfaces/srv/config_dynamic_pure_ctrl.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t ConfigDynamicPureCtrl__callbacks = {
  "ctl_mission_interfaces::srv",
  "ConfigDynamicPureCtrl",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Response)(),
};

static rosidl_service_type_support_t ConfigDynamicPureCtrl__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &ConfigDynamicPureCtrl__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl)() {
  return &ConfigDynamicPureCtrl__handle;
}

#if defined(__cplusplus)
}
#endif
