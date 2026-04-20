// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from fixposition_driver_ros2:msg/Speed.idl
// generated code does not contain a copyright notice

#ifndef FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__TRAITS_HPP_
#define FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "fixposition_driver_ros2/msg/detail/speed__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace fixposition_driver_ros2
{

namespace msg
{

inline void to_flow_style_yaml(
  const Speed & msg,
  std::ostream & out)
{
  out << "{";
  // member: speeds
  {
    if (msg.speeds.size() == 0) {
      out << "speeds: []";
    } else {
      out << "speeds: [";
      size_t pending_items = msg.speeds.size();
      for (auto item : msg.speeds) {
        rosidl_generator_traits::value_to_yaml(item, out);
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
  const Speed & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: speeds
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.speeds.size() == 0) {
      out << "speeds: []\n";
    } else {
      out << "speeds:\n";
      for (auto item : msg.speeds) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Speed & msg, bool use_flow_style = false)
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

}  // namespace fixposition_driver_ros2

namespace rosidl_generator_traits
{

[[deprecated("use fixposition_driver_ros2::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const fixposition_driver_ros2::msg::Speed & msg,
  std::ostream & out, size_t indentation = 0)
{
  fixposition_driver_ros2::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use fixposition_driver_ros2::msg::to_yaml() instead")]]
inline std::string to_yaml(const fixposition_driver_ros2::msg::Speed & msg)
{
  return fixposition_driver_ros2::msg::to_yaml(msg);
}

template<>
inline const char * data_type<fixposition_driver_ros2::msg::Speed>()
{
  return "fixposition_driver_ros2::msg::Speed";
}

template<>
inline const char * name<fixposition_driver_ros2::msg::Speed>()
{
  return "fixposition_driver_ros2/msg/Speed";
}

template<>
struct has_fixed_size<fixposition_driver_ros2::msg::Speed>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<fixposition_driver_ros2::msg::Speed>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<fixposition_driver_ros2::msg::Speed>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__TRAITS_HPP_
