// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigRegulatedPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__TRAITS_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ConfigRegulatedPureCtrl_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: look_ahead_dis
  {
    out << "look_ahead_dis: ";
    rosidl_generator_traits::value_to_yaml(msg.look_ahead_dis, out);
    out << ", ";
  }

  // member: v_forward
  {
    out << "v_forward: ";
    rosidl_generator_traits::value_to_yaml(msg.v_forward, out);
    out << ", ";
  }

  // member: r_min
  {
    out << "r_min: ";
    rosidl_generator_traits::value_to_yaml(msg.r_min, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ConfigRegulatedPureCtrl_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: look_ahead_dis
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "look_ahead_dis: ";
    rosidl_generator_traits::value_to_yaml(msg.look_ahead_dis, out);
    out << "\n";
  }

  // member: v_forward
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "v_forward: ";
    rosidl_generator_traits::value_to_yaml(msg.v_forward, out);
    out << "\n";
  }

  // member: r_min
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "r_min: ";
    rosidl_generator_traits::value_to_yaml(msg.r_min, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ConfigRegulatedPureCtrl_Request & msg, bool use_flow_style = false)
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
  const ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>()
{
  return "ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>()
{
  return "ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl_Request";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace ctl_mission_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ConfigRegulatedPureCtrl_Response & msg,
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
  const ConfigRegulatedPureCtrl_Response & msg,
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

inline std::string to_yaml(const ConfigRegulatedPureCtrl_Response & msg, bool use_flow_style = false)
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
  const ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  ctl_mission_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ctl_mission_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response & msg)
{
  return ctl_mission_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>()
{
  return "ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>()
{
  return "ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl_Response";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>()
{
  return "ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl";
}

template<>
inline const char * name<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>()
{
  return "ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl";
}

template<>
struct has_fixed_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>
  : std::integral_constant<
    bool,
    has_fixed_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>::value &&
    has_fixed_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>::value
  >
{
};

template<>
struct has_bounded_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>
  : std::integral_constant<
    bool,
    has_bounded_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>::value &&
    has_bounded_size<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>::value
  >
{
};

template<>
struct is_service<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>
  : std::true_type
{
};

template<>
struct is_service_request<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>
  : std::true_type
{
};

template<>
struct is_service_response<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__TRAITS_HPP_
