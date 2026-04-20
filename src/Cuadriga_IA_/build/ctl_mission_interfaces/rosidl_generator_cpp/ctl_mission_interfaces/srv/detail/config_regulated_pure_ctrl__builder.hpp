// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigRegulatedPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_regulated_pure_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigRegulatedPureCtrl_Request_r_min
{
public:
  explicit Init_ConfigRegulatedPureCtrl_Request_r_min(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request r_min(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request::_r_min_type arg)
  {
    msg_.r_min = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request msg_;
};

class Init_ConfigRegulatedPureCtrl_Request_v_forward
{
public:
  explicit Init_ConfigRegulatedPureCtrl_Request_v_forward(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigRegulatedPureCtrl_Request_r_min v_forward(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request::_v_forward_type arg)
  {
    msg_.v_forward = std::move(arg);
    return Init_ConfigRegulatedPureCtrl_Request_r_min(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request msg_;
};

class Init_ConfigRegulatedPureCtrl_Request_look_ahead_dis
{
public:
  Init_ConfigRegulatedPureCtrl_Request_look_ahead_dis()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigRegulatedPureCtrl_Request_v_forward look_ahead_dis(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request::_look_ahead_dis_type arg)
  {
    msg_.look_ahead_dis = std::move(arg);
    return Init_ConfigRegulatedPureCtrl_Request_v_forward(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigRegulatedPureCtrl_Request_look_ahead_dis();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigRegulatedPureCtrl_Response_ack
{
public:
  Init_ConfigRegulatedPureCtrl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response ack(::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigRegulatedPureCtrl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_REGULATED_PURE_CTRL__BUILDER_HPP_
