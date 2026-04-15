// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from path_manager_interfaces:srv/GetLLPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__BUILDER_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "path_manager_interfaces/srv/detail/get_ll_path__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace path_manager_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::GetLLPath_Request>()
{
  return ::path_manager_interfaces::srv::GetLLPath_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace path_manager_interfaces


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetLLPath_Response_success
{
public:
  explicit Init_GetLLPath_Response_success(::path_manager_interfaces::srv::GetLLPath_Response & msg)
  : msg_(msg)
  {}
  ::path_manager_interfaces::srv::GetLLPath_Response success(::path_manager_interfaces::srv::GetLLPath_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::GetLLPath_Response msg_;
};

class Init_GetLLPath_Response_path
{
public:
  Init_GetLLPath_Response_path()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetLLPath_Response_success path(::path_manager_interfaces::srv::GetLLPath_Response::_path_type arg)
  {
    msg_.path = std::move(arg);
    return Init_GetLLPath_Response_success(msg_);
  }

private:
  ::path_manager_interfaces::srv::GetLLPath_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::GetLLPath_Response>()
{
  return path_manager_interfaces::srv::builder::Init_GetLLPath_Response_path();
}

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__BUILDER_HPP_
