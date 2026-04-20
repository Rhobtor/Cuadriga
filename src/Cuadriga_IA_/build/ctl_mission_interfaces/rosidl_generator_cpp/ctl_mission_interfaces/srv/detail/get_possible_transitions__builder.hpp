// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ctl_mission_interfaces:srv/GetPossibleTransitions.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__BUILDER_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ctl_mission_interfaces/srv/detail/get_possible_transitions__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ctl_mission_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::GetPossibleTransitions_Request>()
{
  return ::ctl_mission_interfaces::srv::GetPossibleTransitions_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace ctl_mission_interfaces


namespace ctl_mission_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetPossibleTransitions_Response_possible_transitions
{
public:
  Init_GetPossibleTransitions_Response_possible_transitions()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ctl_mission_interfaces::srv::GetPossibleTransitions_Response possible_transitions(::ctl_mission_interfaces::srv::GetPossibleTransitions_Response::_possible_transitions_type arg)
  {
    msg_.possible_transitions = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ctl_mission_interfaces::srv::GetPossibleTransitions_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ctl_mission_interfaces::srv::GetPossibleTransitions_Response>()
{
  return ctl_mission_interfaces::srv::builder::Init_GetPossibleTransitions_Response_possible_transitions();
}

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__BUILDER_HPP_
