// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg __attribute__((deprecated))
#else
# define DEPRECATED__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg __declspec(deprecated)
#endif

namespace cuadriga_ctl_platform_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct KinematicDebugMsg_
{
  using Type = KinematicDebugMsg_<ContainerAllocator>;

  explicit KinematicDebugMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->throttle = 0.0f;
      this->steering = 0.0f;
      this->requested_throttle_acc = 0.0f;
      this->requested_steering_acc = 0.0f;
      this->throttle_limited = false;
      this->steering_limited = false;
    }
  }

  explicit KinematicDebugMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->throttle = 0.0f;
      this->steering = 0.0f;
      this->requested_throttle_acc = 0.0f;
      this->requested_steering_acc = 0.0f;
      this->throttle_limited = false;
      this->steering_limited = false;
    }
  }

  // field types and members
  using _throttle_type =
    float;
  _throttle_type throttle;
  using _steering_type =
    float;
  _steering_type steering;
  using _requested_throttle_acc_type =
    float;
  _requested_throttle_acc_type requested_throttle_acc;
  using _requested_steering_acc_type =
    float;
  _requested_steering_acc_type requested_steering_acc;
  using _throttle_limited_type =
    bool;
  _throttle_limited_type throttle_limited;
  using _steering_limited_type =
    bool;
  _steering_limited_type steering_limited;

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
  Type & set__requested_throttle_acc(
    const float & _arg)
  {
    this->requested_throttle_acc = _arg;
    return *this;
  }
  Type & set__requested_steering_acc(
    const float & _arg)
  {
    this->requested_steering_acc = _arg;
    return *this;
  }
  Type & set__throttle_limited(
    const bool & _arg)
  {
    this->throttle_limited = _arg;
    return *this;
  }
  Type & set__steering_limited(
    const bool & _arg)
  {
    this->steering_limited = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
    std::shared_ptr<cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const KinematicDebugMsg_ & other) const
  {
    if (this->throttle != other.throttle) {
      return false;
    }
    if (this->steering != other.steering) {
      return false;
    }
    if (this->requested_throttle_acc != other.requested_throttle_acc) {
      return false;
    }
    if (this->requested_steering_acc != other.requested_steering_acc) {
      return false;
    }
    if (this->throttle_limited != other.throttle_limited) {
      return false;
    }
    if (this->steering_limited != other.steering_limited) {
      return false;
    }
    return true;
  }
  bool operator!=(const KinematicDebugMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct KinematicDebugMsg_

// alias to use template instance with default allocator
using KinematicDebugMsg =
  cuadriga_ctl_platform_interfaces::msg::KinematicDebugMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace cuadriga_ctl_platform_interfaces

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__STRUCT_HPP_
