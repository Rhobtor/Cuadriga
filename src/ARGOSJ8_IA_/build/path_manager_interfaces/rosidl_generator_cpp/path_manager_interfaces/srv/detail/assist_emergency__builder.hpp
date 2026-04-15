// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from path_manager_interfaces:srv/AssistEmergency.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__BUILDER_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "path_manager_interfaces/srv/detail/assist_emergency__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_AssistEmergency_Request_goal
{
public:
  explicit Init_AssistEmergency_Request_goal(::path_manager_interfaces::srv::AssistEmergency_Request & msg)
  : msg_(msg)
  {}
  ::path_manager_interfaces::srv::AssistEmergency_Request goal(::path_manager_interfaces::srv::AssistEmergency_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::AssistEmergency_Request msg_;
};

class Init_AssistEmergency_Request_start
{
public:
  Init_AssistEmergency_Request_start()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AssistEmergency_Request_goal start(::path_manager_interfaces::srv::AssistEmergency_Request::_start_type arg)
  {
    msg_.start = std::move(arg);
    return Init_AssistEmergency_Request_goal(msg_);
  }

private:
  ::path_manager_interfaces::srv::AssistEmergency_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::AssistEmergency_Request>()
{
  return path_manager_interfaces::srv::builder::Init_AssistEmergency_Request_start();
}

}  // namespace path_manager_interfaces


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_AssistEmergency_Response_path
{
public:
  explicit Init_AssistEmergency_Response_path(::path_manager_interfaces::srv::AssistEmergency_Response & msg)
  : msg_(msg)
  {}
  ::path_manager_interfaces::srv::AssistEmergency_Response path(::path_manager_interfaces::srv::AssistEmergency_Response::_path_type arg)
  {
    msg_.path = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::AssistEmergency_Response msg_;
};

class Init_AssistEmergency_Response_success
{
public:
  Init_AssistEmergency_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AssistEmergency_Response_path success(::path_manager_interfaces::srv::AssistEmergency_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_AssistEmergency_Response_path(msg_);
  }

private:
  ::path_manager_interfaces::srv::AssistEmergency_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::AssistEmergency_Response>()
{
  return path_manager_interfaces::srv::builder::Init_AssistEmergency_Response_success();
}

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__BUILDER_HPP_
