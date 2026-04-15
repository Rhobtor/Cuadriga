// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from cuadriga_ctl_platform_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "cuadriga_ctl_platform_interfaces/srv/detail/get_velocity__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetVelocity_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetVelocity_Request & msg, bool use_flow_style = false)
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

}  // namespace cuadriga_ctl_platform_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use cuadriga_ctl_platform_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  cuadriga_ctl_platform_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use cuadriga_ctl_platform_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request & msg)
{
  return cuadriga_ctl_platform_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>()
{
  return "cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request";
}

template<>
inline const char * name<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>()
{
  return "cuadriga_ctl_platform_interfaces/srv/GetVelocity_Request";
}

template<>
struct has_fixed_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetVelocity_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: actual_velocity
  {
    out << "actual_velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.actual_velocity, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetVelocity_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: actual_velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "actual_velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.actual_velocity, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetVelocity_Response & msg, bool use_flow_style = false)
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

}  // namespace cuadriga_ctl_platform_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use cuadriga_ctl_platform_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  cuadriga_ctl_platform_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use cuadriga_ctl_platform_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response & msg)
{
  return cuadriga_ctl_platform_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>()
{
  return "cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response";
}

template<>
inline const char * name<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>()
{
  return "cuadriga_ctl_platform_interfaces/srv/GetVelocity_Response";
}

template<>
struct has_fixed_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<cuadriga_ctl_platform_interfaces::srv::GetVelocity>()
{
  return "cuadriga_ctl_platform_interfaces::srv::GetVelocity";
}

template<>
inline const char * name<cuadriga_ctl_platform_interfaces::srv::GetVelocity>()
{
  return "cuadriga_ctl_platform_interfaces/srv/GetVelocity";
}

template<>
struct has_fixed_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity>
  : std::integral_constant<
    bool,
    has_fixed_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>::value &&
    has_fixed_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>::value
  >
{
};

template<>
struct has_bounded_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity>
  : std::integral_constant<
    bool,
    has_bounded_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>::value &&
    has_bounded_size<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>::value
  >
{
};

template<>
struct is_service<cuadriga_ctl_platform_interfaces::srv::GetVelocity>
  : std::true_type
{
};

template<>
struct is_service_request<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>
  : std::true_type
{
};

template<>
struct is_service_response<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_
