// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicPureCtrl.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c.h"
#include "ctl_mission_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__init(message_memory);
}

void ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_fini_function(void * message_memory)
{
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_member_array[6] = {
  {
    "look_ahead_dis",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, look_ahead_dis),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_v_forward",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, max_v_forward),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_ang_acc",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, max_ang_acc),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_ang_dec",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, max_ang_dec),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_lin_acc",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, max_lin_acc),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_lin_dec",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request, max_lin_dec),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_members = {
  "ctl_mission_interfaces__srv",  // message namespace
  "ConfigDynamicPureCtrl_Request",  // message name
  6,  // number of fields
  sizeof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request),
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_member_array,  // message members
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_type_support_handle = {
  0,
  &ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ctl_mission_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Request)() {
  if (!ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_type_support_handle.typesupport_identifier) {
    ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ctl_mission_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__init(message_memory);
}

void ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_fini_function(void * message_memory)
{
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_member_array[1] = {
  {
    "ack",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response, ack),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_members = {
  "ctl_mission_interfaces__srv",  // message namespace
  "ConfigDynamicPureCtrl_Response",  // message name
  1,  // number of fields
  sizeof(ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response),
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_member_array,  // message members
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_type_support_handle = {
  0,
  &ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ctl_mission_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Response)() {
  if (!ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_type_support_handle.typesupport_identifier) {
    ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ctl_mission_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_members = {
  "ctl_mission_interfaces__srv",  // service namespace
  "ConfigDynamicPureCtrl",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Request_message_type_support_handle,
  NULL  // response message
  // ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_Response_message_type_support_handle
};

static rosidl_service_type_support_t ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_type_support_handle = {
  0,
  &ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ctl_mission_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl)() {
  if (!ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_type_support_handle.typesupport_identifier) {
    ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ctl_mission_interfaces, srv, ConfigDynamicPureCtrl_Response)()->data;
  }

  return &ctl_mission_interfaces__srv__detail__config_dynamic_pure_ctrl__rosidl_typesupport_introspection_c__ConfigDynamicPureCtrl_service_type_support_handle;
}
