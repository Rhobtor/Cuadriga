// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from path_manager_interfaces:srv/AssistEmergency.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__TRAITS_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "path_manager_interfaces/srv/detail/assist_emergency__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'start'
// Member 'goal'
#include "geometry_msgs/msg/detail/point__traits.hpp"

namespace path_manager_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const AssistEmergency_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: start
  {
    out << "start: ";
    to_flow_style_yaml(msg.start, out);
    out << ", ";
  }

  // member: goal
  {
    out << "goal: ";
    to_flow_style_yaml(msg.goal, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const AssistEmergency_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: start
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "start:\n";
    to_block_style_yaml(msg.start, out, indentation + 2);
  }

  // member: goal
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "goal:\n";
    to_block_style_yaml(msg.goal, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const AssistEmergency_Request & msg, bool use_flow_style = false)
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

}  // namespace path_manager_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use path_manager_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const path_manager_interfaces::srv::AssistEmergency_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::AssistEmergency_Request & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::AssistEmergency_Request>()
{
  return "path_manager_interfaces::srv::AssistEmergency_Request";
}

template<>
inline const char * name<path_manager_interfaces::srv::AssistEmergency_Request>()
{
  return "path_manager_interfaces/srv/AssistEmergency_Request";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::AssistEmergency_Request>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::Point>::value> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::AssistEmergency_Request>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::Point>::value> {};

template<>
struct is_message<path_manager_interfaces::srv::AssistEmergency_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__traits.hpp"

namespace path_manager_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const AssistEmergency_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: path
  {
    out << "path: ";
    to_flow_style_yaml(msg.path, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const AssistEmergency_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: path
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "path:\n";
    to_block_style_yaml(msg.path, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const AssistEmergency_Response & msg, bool use_flow_style = false)
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

}  // namespace path_manager_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use path_manager_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const path_manager_interfaces::srv::AssistEmergency_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::AssistEmergency_Response & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::AssistEmergency_Response>()
{
  return "path_manager_interfaces::srv::AssistEmergency_Response";
}

template<>
inline const char * name<path_manager_interfaces::srv::AssistEmergency_Response>()
{
  return "path_manager_interfaces/srv/AssistEmergency_Response";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::AssistEmergency_Response>
  : std::integral_constant<bool, has_fixed_size<nav_msgs::msg::Path>::value> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::AssistEmergency_Response>
  : std::integral_constant<bool, has_bounded_size<nav_msgs::msg::Path>::value> {};

template<>
struct is_message<path_manager_interfaces::srv::AssistEmergency_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<path_manager_interfaces::srv::AssistEmergency>()
{
  return "path_manager_interfaces::srv::AssistEmergency";
}

template<>
inline const char * name<path_manager_interfaces::srv::AssistEmergency>()
{
  return "path_manager_interfaces/srv/AssistEmergency";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::AssistEmergency>
  : std::integral_constant<
    bool,
    has_fixed_size<path_manager_interfaces::srv::AssistEmergency_Request>::value &&
    has_fixed_size<path_manager_interfaces::srv::AssistEmergency_Response>::value
  >
{
};

template<>
struct has_bounded_size<path_manager_interfaces::srv::AssistEmergency>
  : std::integral_constant<
    bool,
    has_bounded_size<path_manager_interfaces::srv::AssistEmergency_Request>::value &&
    has_bounded_size<path_manager_interfaces::srv::AssistEmergency_Response>::value
  >
{
};

template<>
struct is_service<path_manager_interfaces::srv::AssistEmergency>
  : std::true_type
{
};

template<>
struct is_service_request<path_manager_interfaces::srv::AssistEmergency_Request>
  : std::true_type
{
};

template<>
struct is_service_response<path_manager_interfaces::srv::AssistEmergency_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__TRAITS_HPP_
