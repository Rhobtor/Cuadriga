// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ChangeController.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_CONTROLLER__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_CONTROLLER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/change_controller__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ChangeController_Request_controller_type
{
public:
  Init_ChangeController_Request_controller_type()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ChangeController_Request controller_type(::ctl_mission_interfaces::srv::ChangeController_Request::_controller_type_type arg)
  {
    msg_.controller_type = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ChangeController_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ChangeController_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ChangeController_Request_controller_type();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ChangeController_Response_success
{
public:
  Init_ChangeController_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ChangeController_Response success(::ctl_mission_interfaces::srv::ChangeController_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ChangeController_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ChangeController_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ChangeController_Response_success();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_CONTROLLER__BUILDER_HPP_
