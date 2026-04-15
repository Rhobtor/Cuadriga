// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from cuadriga_ctl_platform_interfaces:msg/CmdThrottleMsg.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__CMD_THROTTLE_MSG__STRUCT_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__CMD_THROTTLE_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg __attribute__((deprecated))
#else
# define DEPRECATED__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg __declspec(deprecated)
#endif

namespace cuadriga_ctl_platform_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CmdThrottleMsg_
{
  using Type = CmdThrottleMsg_<ContainerAllocator>;

  explicit CmdThrottleMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->throttle = 0.0f;
      this->steering = 0.0f;
    }
  }

  explicit CmdThrottleMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->throttle = 0.0f;
      this->steering = 0.0f;
    }
  }

  // field types and members
  using _throttle_type =
    float;
  _throttle_type throttle;
  using _steering_type =
    float;
  _steering_type steering;

  // setters for named parameter idiom
  Type & set__throttle(
    const float & _arg)
  {
    this->throttle = _arg;
    return *this;
  }
  Type & set__steering(
    const float & _arg)
  {
    this->steering = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CmdThrottleMsg_ & other) const
  {
    if (this->throttle != other.throttle) {
      return false;
    }
    if (this->steering != other.steering) {
      return false;
    }
    return true;
  }
  bool operator!=(const CmdThrottleMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CmdThrottleMsg_

// alias to use template instance with default allocator
using CmdThrottleMsg =
  cuadriga_ctl_platform_interfaces::msg::CmdThrottleMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace cuadriga_ctl_platform_interfaces

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__CMD_THROTTLE_MSG__STRUCT_HPP_
