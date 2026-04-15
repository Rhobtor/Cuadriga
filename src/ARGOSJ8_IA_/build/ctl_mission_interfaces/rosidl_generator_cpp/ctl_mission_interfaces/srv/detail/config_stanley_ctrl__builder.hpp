// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigStanleyCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_stanley_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigStanleyCtrl_Request_k_error_angular
{
public:
  explicit Init_ConfigStanleyCtrl_Request_k_error_angular(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request k_error_angular(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request::_k_error_angular_type arg)
  {
    msg_.k_error_angular = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request msg_;
};

class Init_ConfigStanleyCtrl_Request_k_error_lineal
{
public:
  explicit Init_ConfigStanleyCtrl_Request_k_error_lineal(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigStanleyCtrl_Request_k_error_angular k_error_lineal(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request::_k_error_lineal_type arg)
  {
    msg_.k_error_lineal = std::move(arg);
    return Init_ConfigStanleyCtrl_Request_k_error_angular(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request msg_;
};

class Init_ConfigStanleyCtrl_Request_l_ahead_dist
{
public:
  explicit Init_ConfigStanleyCtrl_Request_l_ahead_dist(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigStanleyCtrl_Request_k_error_lineal l_ahead_dist(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request::_l_ahead_dist_type arg)
  {
    msg_.l_ahead_dist = std::move(arg);
    return Init_ConfigStanleyCtrl_Request_k_error_lineal(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request msg_;
};

class Init_ConfigStanleyCtrl_Request_v_forward
{
public:
  Init_ConfigStanleyCtrl_Request_v_forward()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigStanleyCtrl_Request_l_ahead_dist v_forward(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request::_v_forward_type arg)
  {
    msg_.v_forward = std::move(arg);
    return Init_ConfigStanleyCtrl_Request_l_ahead_dist(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigStanleyCtrl_Request_v_forward();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigStanleyCtrl_Response_ack
{
public:
  Init_ConfigStanleyCtrl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response ack(::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigStanleyCtrl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__BUILDER_HPP_
