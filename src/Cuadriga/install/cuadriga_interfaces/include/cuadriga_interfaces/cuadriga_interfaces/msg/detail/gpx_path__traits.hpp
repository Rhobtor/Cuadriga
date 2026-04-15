// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from cuadriga_interfaces:msg/GPXPath.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__TRAITS_HPP_
#define CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "cuadriga_interfaces/msg/detail/gpx_path__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'waypoints'
#include "sensor_msgs/msg/detail/nav_sat_fix__traits.hpp"

namespace cuadriga_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const GPXPath & msg,
  std::ostream & out)
{
  out << "{";
  // member: waypoints
  {
    if (msg.waypoints.size() == 0) {
      out << "waypoints: []";
    } else {
      out << "waypoints: [";
      size_t pending_items = msg.waypoints.size();
      for (auto item : msg.waypoints) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GPXPath & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: waypoints
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.waypoints.size() == 0) {
      out << "waypoints: []\n";
    } else {
      out << "waypoints:\n";
      for (auto item : msg.waypoints) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GPXPath & msg, bool use_flow_style = false)
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

}  // namespace cuadriga_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use cuadriga_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const cuadriga_interfaces::msg::GPXPath & msg,
  std::ostream & out, size_t indentation = 0)
{
  cuadriga_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use cuadriga_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const cuadriga_interfaces::msg::GPXPath & msg)
{
  return cuadriga_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<cuadriga_interfaces::msg::GPXPath>()
{
  return "cuadriga_interfaces::msg::GPXPath";
}

template<>
inline const char * name<cuadriga_interfaces::msg::GPXPath>()
{
  return "cuadriga_interfaces/msg/GPXPath";
}

template<>
struct has_fixed_size<cuadriga_interfaces::msg::GPXPath>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<cuadriga_interfaces::msg::GPXPath>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<cuadriga_interfaces::msg::GPXPath>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__TRAITS_HPP_
