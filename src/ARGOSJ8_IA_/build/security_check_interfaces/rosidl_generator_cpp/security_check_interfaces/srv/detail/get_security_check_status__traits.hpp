// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from security_check_interfaces:srv/GetSecurityCheckStatus.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__TRAITS_HPP_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "security_check_interfaces/srv/detail/get_security_check_status__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace security_check_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSecurityCheckStatus_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSecurityCheckStatus_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSecurityCheckStatus_Request & msg, bool use_flow_style = false)
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

}  // namespace security_check_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use security_check_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const security_check_interfaces::srv::GetSecurityCheckStatus_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  security_check_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use security_check_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const security_check_interfaces::srv::GetSecurityCheckStatus_Request & msg)
{
  return security_check_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<security_check_interfaces::srv::GetSecurityCheckStatus_Request>()
{
  return "security_check_interfaces::srv::GetSecurityCheckStatus_Request";
}

template<>
inline const char * name<security_check_interfaces::srv::GetSecurityCheckStatus_Request>()
{
  return "security_check_interfaces/srv/GetSecurityCheckStatus_Request";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::GetSecurityCheckStatus_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<security_check_interfaces::srv::GetSecurityCheckStatus_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<security_check_interfaces::srv::GetSecurityCheckStatus_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace security_check_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSecurityCheckStatus_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSecurityCheckStatus_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: status
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSecurityCheckStatus_Response & msg, bool use_flow_style = false)
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

}  // namespace security_check_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use security_check_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const security_check_interfaces::srv::GetSecurityCheckStatus_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  security_check_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use security_check_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const security_check_interfaces::srv::GetSecurityCheckStatus_Response & msg)
{
  return security_check_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<security_check_interfaces::srv::GetSecurityCheckStatus_Response>()
{
  return "security_check_interfaces::srv::GetSecurityCheckStatus_Response";
}

template<>
inline const char * name<security_check_interfaces::srv::GetSecurityCheckStatus_Response>()
{
  return "security_check_interfaces/srv/GetSecurityCheckStatus_Response";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::GetSecurityCheckStatus_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<security_check_interfaces::srv::GetSecurityCheckStatus_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<security_check_interfaces::srv::GetSecurityCheckStatus_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<security_check_interfaces::srv::GetSecurityCheckStatus>()
{
  return "security_check_interfaces::srv::GetSecurityCheckStatus";
}

template<>
inline const char * name<security_check_interfaces::srv::GetSecurityCheckStatus>()
{
  return "security_check_interfaces/srv/GetSecurityCheckStatus";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::GetSecurityCheckStatus>
  : std::integral_constant<
    bool,
    has_fixed_size<security_check_interfaces::srv::GetSecurityCheckStatus_Request>::value &&
    has_fixed_size<security_check_interfaces::srv::GetSecurityCheckStatus_Response>::value
  >
{
};

template<>
struct has_bounded_size<security_check_interfaces::srv::GetSecurityCheckStatus>
  : std::integral_constant<
    bool,
    has_bounded_size<security_check_interfaces::srv::GetSecurityCheckStatus_Request>::value &&
    has_bounded_size<security_check_interfaces::srv::GetSecurityCheckStatus_Response>::value
  >
{
};

template<>
struct is_service<security_check_interfaces::srv::GetSecurityCheckStatus>
  : std::true_type
{
};

template<>
struct is_service_request<security_check_interfaces::srv::GetSecurityCheckStatus_Request>
  : std::true_type
{
};

template<>
struct is_service_response<security_check_interfaces::srv::GetSecurityCheckStatus_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__TRAITS_HPP_
