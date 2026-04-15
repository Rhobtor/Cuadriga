// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from ctl_mission_interfaces:srv/ChangeController.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "ctl_mission_interfaces/srv/detail/change_controller__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace ctl_mission_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _ChangeController_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ChangeController_Request_type_support_ids_t;

static const _ChangeController_Request_type_support_ids_t _ChangeController_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ChangeController_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ChangeController_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ChangeController_Request_type_support_symbol_names_t _ChangeController_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ctl_mission_interfaces, srv, ChangeController_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ctl_mission_interfaces, srv, ChangeController_Request)),
  }
};

typedef struct _ChangeController_Request_type_support_data_t
{
  void * data[2];
} _ChangeController_Request_type_support_data_t;

static _ChangeController_Request_type_support_data_t _ChangeController_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ChangeController_Request_message_typesupport_map = {
  2,
  "ctl_mission_interfaces",
  &_ChangeController_Request_message_typesupport_ids.typesupport_identifier[0],
  &_ChangeController_Request_message_typesupport_symbol_names.symbol_name[0],
  &_ChangeController_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ChangeController_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ChangeController_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ctl_mission_interfaces::srv::ChangeController_Request>()
{
  return &::ctl_mission_interfaces::srv::rosidl_typesupport_cpp::ChangeController_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, ctl_mission_interfaces, srv, ChangeController_Request)() {
  return get_message_type_support_handle<ctl_mission_interfaces::srv::ChangeController_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/change_controller__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ctl_mission_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _ChangeController_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ChangeController_Response_type_support_ids_t;

static const _ChangeController_Response_type_support_ids_t _ChangeController_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ChangeController_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ChangeController_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ChangeController_Response_type_support_symbol_names_t _ChangeController_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ctl_mission_interfaces, srv, ChangeController_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ctl_mission_interfaces, srv, ChangeController_Response)),
  }
};

typedef struct _ChangeController_Response_type_support_data_t
{
  void * data[2];
} _ChangeController_Response_type_support_data_t;

static _ChangeController_Response_type_support_data_t _ChangeController_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ChangeController_Response_message_typesupport_map = {
  2,
  "ctl_mission_interfaces",
  &_ChangeController_Response_message_typesupport_ids.typesupport_identifier[0],
  &_ChangeController_Response_message_typesupport_symbol_names.symbol_name[0],
  &_ChangeController_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ChangeController_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ChangeController_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ctl_mission_interfaces::srv::ChangeController_Response>()
{
  return &::ctl_mission_interfaces::srv::rosidl_typesupport_cpp::ChangeController_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, ctl_mission_interfaces, srv, ChangeController_Response)() {
  return get_message_type_support_handle<ctl_mission_interfaces::srv::ChangeController_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/change_controller__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ctl_mission_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _ChangeController_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ChangeController_type_support_ids_t;

static const _ChangeController_type_support_ids_t _ChangeController_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ChangeController_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ChangeController_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ChangeController_type_support_symbol_names_t _ChangeController_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ctl_mission_interfaces, srv, ChangeController)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ctl_mission_interfaces, srv, ChangeController)),
  }
};

typedef struct _ChangeController_type_support_data_t
{
  void * data[2];
} _ChangeController_type_support_data_t;

static _ChangeController_type_support_data_t _ChangeController_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ChangeController_service_typesupport_map = {
  2,
  "ctl_mission_interfaces",
  &_ChangeController_service_typesupport_ids.typesupport_identifier[0],
  &_ChangeController_service_typesupport_symbol_names.symbol_name[0],
  &_ChangeController_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t ChangeController_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ChangeController_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<ctl_mission_interfaces::srv::ChangeController>()
{
  return &::ctl_mission_interfaces::srv::rosidl_typesupport_cpp::ChangeController_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, ctl_mission_interfaces, srv, ChangeController)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<ctl_mission_interfaces::srv::ChangeController>();
}

#ifdef __cplusplus
}
#endif
