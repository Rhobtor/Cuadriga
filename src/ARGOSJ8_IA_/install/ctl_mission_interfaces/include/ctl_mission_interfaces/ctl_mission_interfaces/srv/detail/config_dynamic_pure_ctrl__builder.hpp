// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigDynamicPureCtrl_Request_max_lin_dec
{
public:
  explicit Init_ConfigDynamicPureCtrl_Request_max_lin_dec(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request max_lin_dec(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_max_lin_dec_type arg)
  {
    msg_.max_lin_dec = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

class Init_ConfigDynamicPureCtrl_Request_max_lin_acc
{
public:
  explicit Init_ConfigDynamicPureCtrl_Request_max_lin_acc(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicPureCtrl_Request_max_lin_dec max_lin_acc(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_max_lin_acc_type arg)
  {
    msg_.max_lin_acc = std::move(arg);
    return Init_ConfigDynamicPureCtrl_Request_max_lin_dec(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

class Init_ConfigDynamicPureCtrl_Request_max_ang_dec
{
public:
  explicit Init_ConfigDynamicPureCtrl_Request_max_ang_dec(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicPureCtrl_Request_max_lin_acc max_ang_dec(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_max_ang_dec_type arg)
  {
    msg_.max_ang_dec = std::move(arg);
    return Init_ConfigDynamicPureCtrl_Request_max_lin_acc(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

class Init_ConfigDynamicPureCtrl_Request_max_ang_acc
{
public:
  explicit Init_ConfigDynamicPureCtrl_Request_max_ang_acc(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicPureCtrl_Request_max_ang_dec max_ang_acc(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_max_ang_acc_type arg)
  {
    msg_.max_ang_acc = std::move(arg);
    return Init_ConfigDynamicPureCtrl_Request_max_ang_dec(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

class Init_ConfigDynamicPureCtrl_Request_max_v_forward
{
public:
  explicit Init_ConfigDynamicPureCtrl_Request_max_v_forward(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicPureCtrl_Request_max_ang_acc max_v_forward(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_max_v_forward_type arg)
  {
    msg_.max_v_forward = std::move(arg);
    return Init_ConfigDynamicPureCtrl_Request_max_ang_acc(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

class Init_ConfigDynamicPureCtrl_Request_look_ahead_dis
{
public:
  Init_ConfigDynamicPureCtrl_Request_look_ahead_dis()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigDynamicPureCtrl_Request_max_v_forward look_ahead_dis(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request::_look_ahead_dis_type arg)
  {
    msg_.look_ahead_dis = std::move(arg);
    return Init_ConfigDynamicPureCtrl_Request_max_v_forward(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigDynamicPureCtrl_Request_look_ahead_dis();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigDynamicPureCtrl_Response_ack
{
public:
  Init_ConfigDynamicPureCtrl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response ack(::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigDynamicPureCtrl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__BUILDER_HPP_
