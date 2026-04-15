// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from cuadriga_ctl_platform_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__BUILDER_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "cuadriga_ctl_platform_interfaces/srv/detail/get_velocity__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request>()
{
  return ::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace cuadriga_ctl_platform_interfaces


namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetVelocity_Response_actual_velocity
{
public:
  Init_GetVelocity_Response_actual_velocity()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response actual_velocity(::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response::_actual_velocity_type arg)
  {
    msg_.actual_velocity = std::move(arg);
    return std::move(msg_);
  }

private:
  ::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response>()
{
  return cuadriga_ctl_platform_interfaces::srv::builder::Init_GetVelocity_Response_actual_velocity();
}

}  // namespace cuadriga_ctl_platform_interfaces

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__BUILDER_HPP_
