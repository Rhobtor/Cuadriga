// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from cuadriga_interfaces:msg/GPXPath.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__BUILDER_HPP_
#define CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "cuadriga_interfaces/msg/detail/gpx_path__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace cuadriga_interfaces
{

namespace msg
{

namespace builder
{

class Init_GPXPath_waypoints
{
public:
  Init_GPXPath_waypoints()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::cuadriga_interfaces::msg::GPXPath waypoints(::cuadriga_interfaces::msg::GPXPath::_waypoints_type arg)
  {
    msg_.waypoints = std::move(arg);
    return std::move(msg_);
  }

private:
  ::cuadriga_interfaces::msg::GPXPath msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::cuadriga_interfaces::msg::GPXPath>()
{
  return cuadriga_interfaces::msg::builder::Init_GPXPath_waypoints();
}

}  // namespace cuadriga_interfaces

#endif  // CUADRIGA_INTERFACES__MSG__DETAIL__GPX_PATH__BUILDER_HPP_
