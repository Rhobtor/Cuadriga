// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from security_check_interfaces:srv/PasswordCheck.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__BUILDER_HPP_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "security_check_interfaces/srv/detail/password_check__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace security_check_interfaces
{

namespace srv
{

namespace builder
{

class Init_PasswordCheck_Request_password
{
public:
  Init_PasswordCheck_Request_password()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::security_check_interfaces::srv::PasswordCheck_Request password(::security_check_interfaces::srv::PasswordCheck_Request::_password_type arg)
  {
    msg_.password = std::move(arg);
    return std::move(msg_);
  }

private:
  ::security_check_interfaces::srv::PasswordCheck_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::security_check_interfaces::srv::PasswordCheck_Request>()
{
  return security_check_interfaces::srv::builder::Init_PasswordCheck_Request_password();
}

}  // namespace security_check_interfaces


namespace security_check_interfaces
{

namespace srv
{

namespace builder
{

class Init_PasswordCheck_Response_message
{
public:
  explicit Init_PasswordCheck_Response_message(::security_check_interfaces::srv::PasswordCheck_Response & msg)
  : msg_(msg)
  {}
  ::security_check_interfaces::srv::PasswordCheck_Response message(::security_check_interfaces::srv::PasswordCheck_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::security_check_interfaces::srv::PasswordCheck_Response msg_;
};

class Init_PasswordCheck_Response_success
{
public:
  Init_PasswordCheck_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PasswordCheck_Response_message success(::security_check_interfaces::srv::PasswordCheck_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_PasswordCheck_Response_message(msg_);
  }

private:
  ::security_check_interfaces::srv::PasswordCheck_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::security_check_interfaces::srv::PasswordCheck_Response>()
{
  return security_check_interfaces::srv::builder::Init_PasswordCheck_Response_success();
}

}  // namespace security_check_interfaces

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__PASSWORD_CHECK__BUILDER_HPP_
