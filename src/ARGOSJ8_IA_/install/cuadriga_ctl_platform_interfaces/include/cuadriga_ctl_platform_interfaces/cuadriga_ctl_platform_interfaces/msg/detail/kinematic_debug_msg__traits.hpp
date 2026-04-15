// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__TRAITS_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace cuadriga_ctl_platform_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const KinematicDebugMsg & msg,
  std::ostream & out)
{
  out << "{";
  // member: throttle
  {
    out << "throttle: ";
    rosidl_generator_traits::value_to_yaml(msg.throttle, out);
    out << ", ";
  }

  // member: steering
  {
    out << "steering: ";
    rosidl_generator_traits::value_to_yaml(msg.steering, out);
    out << ", ";
  }

  // member: requested_throttle_acc
  {
    out << "requested_throttle_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.requested_throttle_acc, out);
    out << ", ";
  }

  // member: requested_steering_acc
  {
    out << "requested_steering_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.requested_steering_acc, out);
    out << ", ";
  }

  // member: throttle_limited
  {
    out << "throttle_limited: ";
    rosidl_generator_traits::value_to_yaml(msg.throttle_limited, out);
    out << ", ";
  }

  // member: steering_limited
  {
    out << "steering_limited: ";
    rosidl_generator_traits::value_to_yaml(msg.steering_limited, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const KinematicDebugMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: throttle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "throttle: ";
    rosidl_generator_traits::value_to_yaml(msg.throttle, out);
    out << "\n";
  }

  // member: steering
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "steering: ";
    rosidl_generator_traits::value_to_yaml(msg.steering, out);
    out << "\n";
  }

  // member: requested_throttle_acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "requested_throttle_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.requested_throttle_acc, out);
    out << "\n";
  }

  // member: requested_steering_acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "requested_steering_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.requested_steering_acc, out);
    out << "\n";
  }

  // member: throttle_limited
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "throttle_limited: ";
    rosidl_generator_traits::value_to_yaml(msg.throttle_limited, out);
    out << "\n";
  }

  // member: steering_limited
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "steering_limited: ";
    rosidl_generator_traits::value_to_yaml(msg.steering_limited, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const KinematicDebugMsg & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace cuadriga_ctl_platform_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use cuadriga_ctl_platform_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  cuadriga_ctl_platform_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use cuadriga_ctl_platform_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg & msg)
{
  return cuadriga_ctl_platform_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>()
{
  return "cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg";
}

template<>
inline const char * name<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>()
{
  return "cuadriga_ctl_platform_interfaces/msg/KinematicDebugMsg";
}

template<>
struct has_fixed_size<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__TRAITS_HPP_
