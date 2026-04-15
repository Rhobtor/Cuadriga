// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from path_manager_interfaces:srv/GetFixFramePath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__BUILDER_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "path_manager_interfaces/srv/detail/get_fix_frame_path__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetFixFramePath_Request_ask
{
public:
  Init_GetFixFramePath_Request_ask()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::GetFixFramePath_Request ask(::path_manager_interfaces::srv::GetFixFramePath_Request::_ask_type arg)
  {
    msg_.ask = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::GetFixFramePath_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::GetFixFramePath_Request>()
{
  return path_manager_interfaces::srv::builder::Init_GetFixFramePath_Request_ask();
}

}  // namespace path_manager_interfaces


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetFixFramePath_Response_path
{
public:
  Init_GetFixFramePath_Response_path()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::GetFixFramePath_Response path(::path_manager_interfaces::srv::GetFixFramePath_Response::_path_type arg)
  {
    msg_.path = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::GetFixFramePath_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::GetFixFramePath_Response>()
{
  return path_manager_interfaces::srv::builder::Init_GetFixFramePath_Response_path();
}

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__BUILDER_HPP_
