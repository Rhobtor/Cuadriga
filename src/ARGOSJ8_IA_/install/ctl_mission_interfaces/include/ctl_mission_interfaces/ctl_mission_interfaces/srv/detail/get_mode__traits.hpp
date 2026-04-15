// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ctl_mission_interfaces:srv/GetMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__TRAITS_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ctl_mission_interfaces/srv/detail/get_mode__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetMode_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetMode_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetMode_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ctl_mission_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ctl_mission_interfaces::srv::GetMode_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::GetMode_Request & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::GetMode_Request>()
{
  return "ctl_mission_interfaces::srv::GetMode_Request";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::GetMode_Request>()
{
  return "ctl_mission_interfaces/srv/GetMode_Request";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::GetMode_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::GetMode_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::GetMode_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetMode_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: mode
  {
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetMode_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetMode_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ctl_mission_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ctl_mission_interfaces::srv::GetMode_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::GetMode_Response & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::GetMode_Response>()
{
  return "ctl_mission_interfaces::srv::GetMode_Response";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::GetMode_Response>()
{
  return "ctl_mission_interfaces/srv/GetMode_Response";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::GetMode_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::GetMode_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::GetMode_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<ctl_mission_interfaces::srv::GetMode>()
{
  return "ctl_mission_interfaces::srv::GetMode";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::GetMode>()
{
  return "ctl_mission_interfaces/srv/GetMode";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::GetMode>
  : std::integral_constant<
    bool,
    has_fixed_size<ctl_mission_interfaces::srv::GetMode_Request>::value &&
    has_fixed_size<ctl_mission_interfaces::srv::GetMode_Response>::value
  >
{
};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::GetMode>
  : std::integral_constant<
    bool,
    has_bounded_size<ctl_mission_interfaces::srv::GetMode_Request>::value &&
    has_bounded_size<ctl_mission_interfaces::srv::GetMode_Response>::value
  >
{
};

template<>
struct is_service<ctl_mission_interfaces::srv::GetMode>
  : std::true_type
{
};

template<>
struct is_service_request<ctl_mission_interfaces::srv::GetMode_Request>
  : std::true_type
{
};

template<>
struct is_service_response<ctl_mission_interfaces::srv::GetMode_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__TRAITS_HPP_
