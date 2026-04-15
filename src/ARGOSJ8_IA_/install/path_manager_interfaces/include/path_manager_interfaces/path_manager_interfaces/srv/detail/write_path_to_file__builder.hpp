// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from path_manager_interfaces:srv/WritePathToFile.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__BUILDER_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "path_manager_interfaces/srv/detail/write_path_to_file__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_WritePathToFile_Request_filename
{
public:
  Init_WritePathToFile_Request_filename()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::WritePathToFile_Request filename(::path_manager_interfaces::srv::WritePathToFile_Request::_filename_type arg)
  {
    msg_.filename = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::WritePathToFile_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::WritePathToFile_Request>()
{
  return path_manager_interfaces::srv::builder::Init_WritePathToFile_Request_filename();
}

}  // namespace path_manager_interfaces


namespace path_manager_interfaces
{

namespace srv
{

namespace builder
{

class Init_WritePathToFile_Response_success
{
public:
  Init_WritePathToFile_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::path_manager_interfaces::srv::WritePathToFile_Response success(::path_manager_interfaces::srv::WritePathToFile_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::path_manager_interfaces::srv::WritePathToFile_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::path_manager_interfaces::srv::WritePathToFile_Response>()
{
  return path_manager_interfaces::srv::builder::Init_WritePathToFile_Response_success();
}

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__WRITE_PATH_TO_FILE__BUILDER_HPP_
