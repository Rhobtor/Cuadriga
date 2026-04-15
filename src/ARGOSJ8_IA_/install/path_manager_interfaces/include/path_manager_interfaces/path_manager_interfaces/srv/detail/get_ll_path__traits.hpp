// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from path_manager_interfaces:srv/GetLLPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__TRAITS_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "path_manager_interfaces/srv/detail/get_ll_path__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace path_manager_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetLLPath_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetLLPath_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetLLPath_Request & msg, bool use_flow_style = false)
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
  const path_manager_interfaces::srv::GetLLPath_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::GetLLPath_Request & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::GetLLPath_Request>()
{
  return "path_manager_interfaces::srv::GetLLPath_Request";
}

template<>
inline const char * name<path_manager_interfaces::srv::GetLLPath_Request>()
{
  return "path_manager_interfaces/srv/GetLLPath_Request";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::GetLLPath_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::GetLLPath_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<path_manager_interfaces::srv::GetLLPath_Request>
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
  const GetLLPath_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: path
  {
    out << "path: ";
    to_flow_style_yaml(msg.path, out);
    out << ", ";
  }

  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetLLPath_Response & msg,
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

  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetLLPath_Response & msg, bool use_flow_style = false)
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
  const path_manager_interfaces::srv::GetLLPath_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  path_manager_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use path_manager_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const path_manager_interfaces::srv::GetLLPath_Response & msg)
{
  return path_manager_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<path_manager_interfaces::srv::GetLLPath_Response>()
{
  return "path_manager_interfaces::srv::GetLLPath_Response";
}

template<>
inline const char * name<path_manager_interfaces::srv::GetLLPath_Response>()
{
  return "path_manager_interfaces/srv/GetLLPath_Response";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::GetLLPath_Response>
  : std::integral_constant<bool, has_fixed_size<nav_msgs::msg::Path>::value> {};

template<>
struct has_bounded_size<path_manager_interfaces::srv::GetLLPath_Response>
  : std::integral_constant<bool, has_bounded_size<nav_msgs::msg::Path>::value> {};

template<>
struct is_message<path_manager_interfaces::srv::GetLLPath_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<path_manager_interfaces::srv::GetLLPath>()
{
  return "path_manager_interfaces::srv::GetLLPath";
}

template<>
inline const char * name<path_manager_interfaces::srv::GetLLPath>()
{
  return "path_manager_interfaces/srv/GetLLPath";
}

template<>
struct has_fixed_size<path_manager_interfaces::srv::GetLLPath>
  : std::integral_constant<
    bool,
    has_fixed_size<path_manager_interfaces::srv::GetLLPath_Request>::value &&
    has_fixed_size<path_manager_interfaces::srv::GetLLPath_Response>::value
  >
{
};

template<>
struct has_bounded_size<path_manager_interfaces::srv::GetLLPath>
  : std::integral_constant<
    bool,
    has_bounded_size<path_manager_interfaces::srv::GetLLPath_Request>::value &&
    has_bounded_size<path_manager_interfaces::srv::GetLLPath_Response>::value
  >
{
};

template<>
struct is_service<path_manager_interfaces::srv::GetLLPath>
  : std::true_type
{
};

template<>
struct is_service_request<path_manager_interfaces::srv::GetLLPath_Request>
  : std::true_type
{
};

template<>
struct is_service_response<path_manager_interfaces::srv::GetLLPath_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__TRAITS_HPP_
