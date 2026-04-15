// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from security_check_interfaces:srv/PasswordCheck.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__TRAITS_HPP_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "security_check_interfaces/srv/detail/password_check__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace security_check_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const PasswordCheck_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: password
  {
    out << "password: ";
    rosidl_generator_traits::value_to_yaml(msg.password, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PasswordCheck_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: password
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "password: ";
    rosidl_generator_traits::value_to_yaml(msg.password, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PasswordCheck_Request & msg, bool use_flow_style = false)
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
  const security_check_interfaces::srv::PasswordCheck_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  security_check_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use security_check_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const security_check_interfaces::srv::PasswordCheck_Request & msg)
{
  return security_check_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<security_check_interfaces::srv::PasswordCheck_Request>()
{
  return "security_check_interfaces::srv::PasswordCheck_Request";
}

template<>
inline const char * name<security_check_interfaces::srv::PasswordCheck_Request>()
{
  return "security_check_interfaces/srv/PasswordCheck_Request";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::PasswordCheck_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<security_check_interfaces::srv::PasswordCheck_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<security_check_interfaces::srv::PasswordCheck_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace security_check_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const PasswordCheck_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PasswordCheck_Response & msg,
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

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PasswordCheck_Response & msg, bool use_flow_style = false)
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
  const security_check_interfaces::srv::PasswordCheck_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  security_check_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use security_check_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const security_check_interfaces::srv::PasswordCheck_Response & msg)
{
  return security_check_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<security_check_interfaces::srv::PasswordCheck_Response>()
{
  return "security_check_interfaces::srv::PasswordCheck_Response";
}

template<>
inline const char * name<security_check_interfaces::srv::PasswordCheck_Response>()
{
  return "security_check_interfaces/srv/PasswordCheck_Response";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::PasswordCheck_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<security_check_interfaces::srv::PasswordCheck_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<security_check_interfaces::srv::PasswordCheck_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<security_check_interfaces::srv::PasswordCheck>()
{
  return "security_check_interfaces::srv::PasswordCheck";
}

template<>
inline const char * name<security_check_interfaces::srv::PasswordCheck>()
{
  return "security_check_interfaces/srv/PasswordCheck";
}

template<>
struct has_fixed_size<security_check_interfaces::srv::PasswordCheck>
  : std::integral_constant<
    bool,
    has_fixed_size<security_check_interfaces::srv::PasswordCheck_Request>::value &&
    has_fixed_size<security_check_interfaces::srv::PasswordCheck_Response>::value
  >
{
};

template<>
struct has_bounded_size<security_check_interfaces::srv::PasswordCheck>
  : std::integral_constant<
    bool,
    has_bounded_size<security_check_interfaces::srv::PasswordCheck_Request>::value &&
    has_bounded_size<security_check_interfaces::srv::PasswordCheck_Response>::value
  >
{
};

template<>
struct is_service<security_check_interfaces::srv::PasswordCheck>
  : std::true_type
{
};

template<>
struct is_service_request<security_check_interfaces::srv::PasswordCheck_Request>
  : std::true_type
{
};

template<>
struct is_service_response<security_check_interfaces::srv::PasswordCheck_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__TRAITS_HPP_
