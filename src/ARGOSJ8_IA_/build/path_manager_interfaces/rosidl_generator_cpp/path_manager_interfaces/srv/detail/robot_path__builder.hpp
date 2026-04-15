// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from path_manager_interfaces:srv/RobotPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__BUILDER_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "path_manager_interfaces/srv/detail/robot_path__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_RobotPath_Request_path
{
public:
  Init_RobotPath_Request_path()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::RobotPath_Request path(::path_manager_interfaces::srv::RobotPath_Request::_path_type arg)
  {
    msg_.path = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::RobotPath_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::RobotPath_Request>()
{
  return path_manager_interfaces::srv::builder::Init_RobotPath_Request_path();
}

}  // namespace path_manager_interfaces


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_RobotPath_Response_ack
{
public:
  Init_RobotPath_Response_ack()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::RobotPath_Response ack(::path_manager_interfaces::srv::RobotPath_Response::_ack_type arg)
  {
    msg_.ack = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::RobotPath_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::RobotPath_Response>()
{
  return path_manager_interfaces::srv::builder::Init_RobotPath_Response_ack();
}

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__ROBOT_PATH__BUILDER_HPP_
