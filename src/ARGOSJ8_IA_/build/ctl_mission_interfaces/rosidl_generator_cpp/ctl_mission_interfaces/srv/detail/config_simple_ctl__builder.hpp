// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigSimpleCtl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/config_simple_ctl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigSimpleCtl_Request_l_ahead_dist
{
public:
  explicit Init_ConfigSimpleCtl_Request_l_ahead_dist(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request & msg)
  : msg_(msg)
  {}
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request l_ahead_dist(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request::_l_ahead_dist_type arg)
  {
    msg_.l_ahead_dist = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request msg_;
};

class Init_ConfigSimpleCtl_Request_v_forward
{
public:
  explicit Init_ConfigSimpleCtl_Request_v_forward(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigSimpleCtl_Request_l_ahead_dist v_forward(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request::_v_forward_type arg)
  {
    msg_.v_forward = std::move(arg);
    return Init_ConfigSimpleCtl_Request_l_ahead_dist(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request msg_;
};

class Init_ConfigSimpleCtl_Request_k_position
{
public:
  explicit Init_ConfigSimpleCtl_Request_k_position(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigSimpleCtl_Request_v_forward k_position(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request::_k_position_type arg)
  {
    msg_.k_position = std::move(arg);
    return Init_ConfigSimpleCtl_Request_v_forward(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request msg_;
};

class Init_ConfigSimpleCtl_Request_points_in_window
{
public:
  explicit Init_ConfigSimpleCtl_Request_points_in_window(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request & msg)
  : msg_(msg)
  {}
  Init_ConfigSimpleCtl_Request_k_position points_in_window(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request::_points_in_window_type arg)
  {
    msg_.points_in_window = std::move(arg);
    return Init_ConfigSimpleCtl_Request_k_position(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request msg_;
};

class Init_ConfigSimpleCtl_Request_k_angle
{
public:
  Init_ConfigSimpleCtl_Request_k_angle()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConfigSimpleCtl_Request_points_in_window k_angle(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request::_k_angle_type arg)
  {
    msg_.k_angle = std::move(arg);
    return Init_ConfigSimpleCtl_Request_points_in_window(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigSimpleCtl_Request>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigSimpleCtl_Request_k_angle();
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_ConfigSimpleCtl_Response_ack
{
public:
  Init_ConfigSimpleCtl_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Response ack(::ctl_mission_interfaces::srv::ConfigSimpleCtl_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::ConfigSimpleCtl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::ConfigSimpleCtl_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_ConfigSimpleCtl_Response_ack();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__BUILDER_HPP_
