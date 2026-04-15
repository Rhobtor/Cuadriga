// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from path_manager_interfaces:srv/RobotPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__TRAITS_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "path_manager_interfaces/srv/detail/robot_path__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__traits.hpp"

namespace path_manager_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const RobotPath_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: path
  {
    out << "path: ";
    to_flow_style_yaml(msg.path, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RobotPath_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: path
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "path:\n";
    to_block_style_yaml(msg.path, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RobotPath_Request & msg, bool use_flow_style = false)
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
  const path_manager_interfaces::srv::RobotPath_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::RobotPath_Request & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::RobotPath_Request>()
{
  return "path_manager_interfaces::srv::RobotPath_Request";
}

template<>
inline const char * name<path_manager_interfaces::srv::RobotPath_Request>()
{
  return "path_manager_interfaces/srv/RobotPath_Request";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::RobotPath_Request>
  : std::integral_constant<bool, has_fixed_size<nav_msgs::msg::Path>::value> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::RobotPath_Request>
  : std::integral_constant<bool, has_bounded_size<nav_msgs::msg::Path>::value> {};

template<>
struct is_message<path_manager_interfaces::srv::RobotPath_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace path_manager_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const RobotPath_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: ack
  {
    out << "ack: ";
    rosidl_generator_traits::value_to_yaml(msg.ack, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RobotPath_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ack
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ack: ";
    rosidl_generator_traits::value_to_yaml(msg.ack, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RobotPath_Response & msg, bool use_flow_style = false)
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
  const path_manager_interfaces::srv::RobotPath_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::RobotPath_Response & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::RobotPath_Response>()
{
  return "path_manager_interfaces::srv::RobotPath_Response";
}

template<>
inline const char * name<path_manager_interfaces::srv::RobotPath_Response>()
{
  return "path_manager_interfaces/srv/RobotPath_Response";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::RobotPath_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::RobotPath_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<path_manager_interfaces::srv::RobotPath_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<path_manager_interfaces::srv::RobotPath>()
{
  return "path_manager_interfaces::srv::RobotPath";
}

template<>
inline const char * name<path_manager_interfaces::srv::RobotPath>()
{
  return "path_manager_interfaces/srv/RobotPath";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::RobotPath>
  : std::integral_constant<
    bool,
    has_fixed_size<path_manager_interfaces::srv::RobotPath_Request>::value &&
    has_fixed_size<path_manager_interfaces::srv::RobotPath_Response>::value
  >
{
};

template<>
struct has_bounded_size<path_manager_interfaces::srv::RobotPath>
  : std::integral_constant<
    bool,
    has_bounded_size<path_manager_interfaces::srv::RobotPath_Request>::value &&
    has_bounded_size<path_manager_interfaces::srv::RobotPath_Response>::value
  >
{
};

template<>
struct is_service<path_manager_interfaces::srv::RobotPath>
  : std::true_type
{
};

template<>
struct is_service_request<path_manager_interfaces::srv::RobotPath_Request>
  : std::true_type
{
};

template<>
struct is_service_response<path_manager_interfaces::srv::RobotPath_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__TRAITS_HPP_
