// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ChangeMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/change_mode__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ChangeMode_Request_transition
{
public:
  Init_ChangeMode_Request_transition()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ChangeMode_Request transition(::ctl_mission_interfaces::srv::ChangeMode_Request::_transition_type arg)
  {
    msg_.transition = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ChangeMode_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ChangeMode_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ChangeMode_Request_transition();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ChangeMode_Response_success
{
public:
  Init_ChangeMode_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ChangeMode_Response success(::ctl_mission_interfaces::srv::ChangeMode_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ChangeMode_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ChangeMode_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ChangeMode_Response_success();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__BUILDER_HPP_
