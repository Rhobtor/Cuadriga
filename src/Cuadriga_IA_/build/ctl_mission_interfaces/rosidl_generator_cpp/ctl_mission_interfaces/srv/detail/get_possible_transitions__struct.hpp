// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ctl_mission_interfaces:srv/GetPossibleTransitions.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__STRUCT_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Request __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Request __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetPossibleTransitions_Request_
{
  using Type = GetPossibleTransitions_Request_<ContainerAllocator>;

  explicit GetPossibleTransitions_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetPossibleTransitions_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Request
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Request
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetPossibleTransitions_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetPossibleTransitions_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetPossibleTransitions_Request_

// alias to use template instance with default allocator
using GetPossibleTransitions_Request =
  ctl_mission_interfaces::srv::GetPossibleTransitions_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Response __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Response __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetPossibleTransitions_Response_
{
  using Type = GetPossibleTransitions_Response_<ContainerAllocator>;

  explicit GetPossibleTransitions_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit GetPossibleTransitions_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _possible_transitions_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _possible_transitions_type possible_transitions;

  // setters for named parameter idiom
  Type & set__possible_transitions(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->possible_transitions = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Response
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__GetPossibleTransitions_Response
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetPossibleTransitions_Response_ & other) const
  {
    if (this->possible_transitions != other.possible_transitions) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetPossibleTransitions_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetPossibleTransitions_Response_

// alias to use template instance with default allocator
using GetPossibleTransitions_Response =
  ctl_mission_interfaces::srv::GetPossibleTransitions_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace ctl_mission_interfaces
{

namespace srv
{

struct GetPossibleTransitions
{
  using Request = ctl_mission_interfaces::srv::GetPossibleTransitions_Request;
  using Response = ctl_mission_interfaces::srv::GetPossibleTransitions_Response;
};

}  // namespace srv

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__GET_POSSIBLE_TRANSITIONS__STRUCT_HPP_
