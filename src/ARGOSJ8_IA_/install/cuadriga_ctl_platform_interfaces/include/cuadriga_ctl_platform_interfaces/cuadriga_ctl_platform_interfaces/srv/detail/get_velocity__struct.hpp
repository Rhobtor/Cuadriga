// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from cuadriga_ctl_platform_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_HPP_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request __attribute__((deprecated))
#else
# define DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request __declspec(deprecated)
#endif

namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetVelocity_Request_
{
  using Type = GetVelocity_Request_<ContainerAllocator>;

  explicit GetVelocity_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetVelocity_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetVelocity_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetVelocity_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetVelocity_Request_

// alias to use template instance with default allocator
using GetVelocity_Request =
  cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace cuadriga_ctl_platform_interfaces


#ifndef _WIN32
# define DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response __attribute__((deprecated))
#else
# define DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response __declspec(deprecated)
#endif

namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetVelocity_Response_
{
  using Type = GetVelocity_Response_<ContainerAllocator>;

  explicit GetVelocity_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->actual_velocity = 0;
    }
  }

  explicit GetVelocity_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->actual_velocity = 0;
    }
  }

  // field types and members
  using _actual_velocity_type =
    int8_t;
  _actual_velocity_type actual_velocity;

  // setters for named parameter idiom
  Type & set__actual_velocity(
    const int8_t & _arg)
  {
    this->actual_velocity = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response
    std::shared_ptr<cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetVelocity_Response_ & other) const
  {
    if (this->actual_velocity != other.actual_velocity) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetVelocity_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetVelocity_Response_

// alias to use template instance with default allocator
using GetVelocity_Response =
  cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace cuadriga_ctl_platform_interfaces

namespace cuadriga_ctl_platform_interfaces
{

namespace srv
{

struct GetVelocity
{
  using Request = cuadriga_ctl_platform_interfaces::srv::GetVelocity_Request;
  using Response = cuadriga_ctl_platform_interfaces::srv::GetVelocity_Response;
};

}  // namespace srv

}  // namespace cuadriga_ctl_platform_interfaces

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_HPP_
