// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ctl_mission_interfaces:srv/ChangeMode.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Request __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Request __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ChangeMode_Request_
{
  using Type = ChangeMode_Request_<ContainerAllocator>;

  explicit ChangeMode_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->transition = 0;
    }
  }

  explicit ChangeMode_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->transition = 0;
    }
  }

  // field types and members
  using _transition_type =
    int8_t;
  _transition_type transition;

  // setters for named parameter idiom
  Type & set__transition(
    const int8_t & _arg)
  {
    this->transition = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ChangeMode_Request_ & other) const
  {
    if (this->transition != other.transition) {
      return false;
    }
    return true;
  }
  bool operator!=(const ChangeMode_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ChangeMode_Request_

// alias to use template instance with default allocator
using ChangeMode_Request =
  ctl_mission_interfaces::srv::ChangeMode_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Response __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Response __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ChangeMode_Response_
{
  using Type = ChangeMode_Response_<ContainerAllocator>;

  explicit ChangeMode_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit ChangeMode_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ChangeMode_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ChangeMode_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const ChangeMode_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ChangeMode_Response_

// alias to use template instance with default allocator
using ChangeMode_Response =
  ctl_mission_interfaces::srv::ChangeMode_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace ctl_mission_interfaces
{

namespace srv
{

struct ChangeMode
{
  using Request = ctl_mission_interfaces::srv::ChangeMode_Request;
  using Response = ctl_mission_interfaces::srv::ChangeMode_Response;
};

}  // namespace srv

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CHANGE_MODE__STRUCT_HPP_
