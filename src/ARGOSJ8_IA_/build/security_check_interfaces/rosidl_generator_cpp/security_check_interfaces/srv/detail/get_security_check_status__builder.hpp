// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from security_check_interfaces:srv/GetSecurityCheckStatus.idl
// generated code does not contain a copyright notice

#ifndef SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__BUILDER_HPP_
#define SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "security_check_interfaces/srv/detail/get_security_check_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace security_check_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::security_check_interfaces::srv::GetSecurityCheckStatus_Request>()
{
  return ::security_check_interfaces::srv::GetSecurityCheckStatus_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace security_check_interfaces


namespace security_check_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetSecurityCheckStatus_Response_status
{
public:
  Init_GetSecurityCheckStatus_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::security_check_interfaces::srv::GetSecurityCheckStatus_Response status(::security_check_interfaces::srv::GetSecurityCheckStatus_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::security_check_interfaces::srv::GetSecurityCheckStatus_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::security_check_interfaces::srv::GetSecurityCheckStatus_Response>()
{
  return security_check_interfaces::srv::builder::Init_GetSecurityCheckStatus_Response_status();
}

}  // namespace security_check_interfaces

#endif  // SECURITY_CHECK_INTERFACES__SRV__DETAIL__GET_SECURITY_CHECK_STATUS__BUILDER_HPP_
