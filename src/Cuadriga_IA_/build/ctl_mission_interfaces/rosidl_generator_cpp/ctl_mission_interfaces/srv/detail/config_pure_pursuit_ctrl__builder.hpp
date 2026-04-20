// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigPurePursuitCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_PURE_PURSUIT_CTRL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_PURE_PURSUIT_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_pure_pursuit_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigPurePursuitCtrl_Request_l_ahead_dist
{
public:
  explicit Init_ConfigPurePursuitCtrl_Request_l_ahead_dist(::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request l_ahead_dist(::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request::_l_ahead_dist_type arg)
  {
    msg_.l_ahead_dist = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request msg_;
};

class Init_ConfigPurePursuitCtrl_Request_v_forward
{
public:
  Init_ConfigPurePursuitCtrl_Request_v_forward()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigPurePursuitCtrl_Request_l_ahead_dist v_forward(::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request::_v_forward_type arg)
  {
    msg_.v_forward = std::move(arg);
    return Init_ConfigPurePursuitCtrl_Request_l_ahead_dist(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigPurePursuitCtrl_Request_v_forward();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigPurePursuitCtrl_Response_ack
{
public:
  Init_ConfigPurePursuitCtrl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Response ack(::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigPurePursuitCtrl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigPurePursuitCtrl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_PURE_PURSUIT_CTRL__BUILDER_HPP_
