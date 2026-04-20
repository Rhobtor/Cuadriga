// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicLAPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__TRAITS_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ctl_mission_interfaces/srv/detail/config_dynamic_la_pure_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ConfigDynamicLAPureCtrl_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: look_ahead_v_gain
  {
    out << "look_ahead_v_gain: ";
    rosidl_generator_traits::value_to_yaml(msg.look_ahead_v_gain, out);
    out << ", ";
  }

  // member: max_v_forward
  {
    out << "max_v_forward: ";
    rosidl_generator_traits::value_to_yaml(msg.max_v_forward, out);
    out << ", ";
  }

  // member: max_ang_acc
  {
    out << "max_ang_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.max_ang_acc, out);
    out << ", ";
  }

  // member: max_ang_dec
  {
    out << "max_ang_dec: ";
    rosidl_generator_traits::value_to_yaml(msg.max_ang_dec, out);
    out << ", ";
  }

  // member: max_lin_acc
  {
    out << "max_lin_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.max_lin_acc, out);
    out << ", ";
  }

  // member: max_lin_dec
  {
    out << "max_lin_dec: ";
    rosidl_generator_traits::value_to_yaml(msg.max_lin_dec, out);
    out << ", ";
  }

  // member: speed_pow
  {
    out << "speed_pow: ";
    rosidl_generator_traits::value_to_yaml(msg.speed_pow, out);
    out << ", ";
  }

  // member: min_look_ahead_d
  {
    out << "min_look_ahead_d: ";
    rosidl_generator_traits::value_to_yaml(msg.min_look_ahead_d, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ConfigDynamicLAPureCtrl_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: look_ahead_v_gain
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "look_ahead_v_gain: ";
    rosidl_generator_traits::value_to_yaml(msg.look_ahead_v_gain, out);
    out << "\n";
  }

  // member: max_v_forward
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "max_v_forward: ";
    rosidl_generator_traits::value_to_yaml(msg.max_v_forward, out);
    out << "\n";
  }

  // member: max_ang_acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "max_ang_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.max_ang_acc, out);
    out << "\n";
  }

  // member: max_ang_dec
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "max_ang_dec: ";
    rosidl_generator_traits::value_to_yaml(msg.max_ang_dec, out);
    out << "\n";
  }

  // member: max_lin_acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "max_lin_acc: ";
    rosidl_generator_traits::value_to_yaml(msg.max_lin_acc, out);
    out << "\n";
  }

  // member: max_lin_dec
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "max_lin_dec: ";
    rosidl_generator_traits::value_to_yaml(msg.max_lin_dec, out);
    out << "\n";
  }

  // member: speed_pow
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed_pow: ";
    rosidl_generator_traits::value_to_yaml(msg.speed_pow, out);
    out << "\n";
  }

  // member: min_look_ahead_d
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "min_look_ahead_d: ";
    rosidl_generator_traits::value_to_yaml(msg.min_look_ahead_d, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ConfigDynamicLAPureCtrl_Request & msg, bool use_flow_style = false)
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
  const ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>()
{
  return "ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>()
{
  return "ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl_Request";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ConfigDynamicLAPureCtrl_Response & msg,
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
  const ConfigDynamicLAPureCtrl_Response & msg,
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

inline std::string to_yaml(const ConfigDynamicLAPureCtrl_Response & msg, bool use_flow_style = false)
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
  const ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>()
{
  return "ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>()
{
  return "ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl_Response";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>()
{
  return "ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>()
{
  return "ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>
  : std::integral_constant<
    bool,
    has_fixed_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>::value &&
    has_fixed_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>::value
  >
{
};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>
  : std::integral_constant<
    bool,
    has_bounded_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>::value &&
    has_bounded_size<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>::value
  >
{
};

template<>
struct is_service<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>
  : std::true_type
{
};

template<>
struct is_service_request<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>
  : std::true_type
{
};

template<>
struct is_service_response<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__TRAITS_HPP_
