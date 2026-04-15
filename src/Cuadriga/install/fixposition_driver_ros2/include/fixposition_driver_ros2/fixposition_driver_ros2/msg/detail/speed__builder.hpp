// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from fixposition_driver_ros2:msg/Speed.idl
// generated code does not contain a copyright notice

#ifndef FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__BUILDER_HPP_
#define FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "fixposition_driver_ros2/msg/detail/speed__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace fixposition_driver_ros2
{

namespace msg
{

namespace builder
{

class Init_Speed_speeds
{
public:
  Init_Speed_speeds()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::fixposition_driver_ros2::msg::Speed speeds(::fixposition_driver_ros2::msg::Speed::_speeds_type arg)
  {
    msg_.speeds = std::move(arg);
    return std::move(msg_);
  }

private:
  ::fixposition_driver_ros2::msg::Speed msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::fixposition_driver_ros2::msg::Speed>()
{
  return fixposition_driver_ros2::msg::builder::Init_Speed_speeds();
}

}  // namespace fixposition_driver_ros2

#endif  // FIXPOSITION_DRIVER_ROS2__MSG__DETAIL__SPEED__BUILDER_HPP_
