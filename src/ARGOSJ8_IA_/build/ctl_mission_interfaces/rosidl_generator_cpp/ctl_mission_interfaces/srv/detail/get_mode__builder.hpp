// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/GetMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/get_mode__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::GetMode_Request>()
{
  return ::ctl_mission_interfaces::srv::GetMode_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetMode_Response_mode
{
public:
  Init_GetMode_Response_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::GetMode_Response mode(::ctl_mission_interfaces::srv::GetMode_Response::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::GetMode_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::GetMode_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_GetMode_Response_mode();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__GET_MODE__BUILDER_HPP_
