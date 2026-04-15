// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicLAPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_dynamic_la_pure_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigDynamicLAPureCtrl_Request_min_look_ahead_d
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_min_look_ahead_d(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request min_look_ahead_d(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_min_look_ahead_d_type arg)
  {
    msg_.min_look_ahead_d = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_speed_pow
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_speed_pow(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_min_look_ahead_d speed_pow(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_speed_pow_type arg)
  {
    msg_.speed_pow = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_min_look_ahead_d(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_max_lin_dec
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_max_lin_dec(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_speed_pow max_lin_dec(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_max_lin_dec_type arg)
  {
    msg_.max_lin_dec = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_speed_pow(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_max_lin_acc
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_max_lin_acc(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_max_lin_dec max_lin_acc(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_max_lin_acc_type arg)
  {
    msg_.max_lin_acc = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_max_lin_dec(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_max_ang_dec
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_max_ang_dec(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_max_lin_acc max_ang_dec(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_max_ang_dec_type arg)
  {
    msg_.max_ang_dec = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_max_lin_acc(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_max_ang_acc
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_max_ang_acc(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_max_ang_dec max_ang_acc(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_max_ang_acc_type arg)
  {
    msg_.max_ang_acc = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_max_ang_dec(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_max_v_forward
{
public:
  explicit Init_ConfigDynamicLAPureCtrl_Request_max_v_forward(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_max_ang_acc max_v_forward(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_max_v_forward_type arg)
  {
    msg_.max_v_forward = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_max_ang_acc(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

class Init_ConfigDynamicLAPureCtrl_Request_look_ahead_v_gain
{
public:
  Init_ConfigDynamicLAPureCtrl_Request_look_ahead_v_gain()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigDynamicLAPureCtrl_Request_max_v_forward look_ahead_v_gain(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request::_look_ahead_v_gain_type arg)
  {
    msg_.look_ahead_v_gain = std::move(arg);
    return Init_ConfigDynamicLAPureCtrl_Request_max_v_forward(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigDynamicLAPureCtrl_Request_look_ahead_v_gain();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigDynamicLAPureCtrl_Response_ack
{
public:
  Init_ConfigDynamicLAPureCtrl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response ack(::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigDynamicLAPureCtrl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_LA_PURE_CTRL__BUILDER_HPP_
