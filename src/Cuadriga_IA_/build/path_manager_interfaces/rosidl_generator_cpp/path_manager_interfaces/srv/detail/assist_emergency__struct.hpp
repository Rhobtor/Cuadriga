// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from path_manager_interfaces:srv/AssistEmergency.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__STRUCT_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'start'
// Member 'goal'
#include "geometry_msgs/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Request __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Request __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct AssistEmergency_Request_
{
  using Type = AssistEmergency_Request_<ContainerAllocator>;

  explicit AssistEmergency_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start(_init),
    goal(_init)
  {
    (void)_init;
  }

  explicit AssistEmergency_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start(_alloc, _init),
    goal(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _start_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _start_type start;
  using _goal_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _goal_type goal;

  // setters for named parameter idiom
  Type & set__start(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->start = _arg;
    return *this;
  }
  Type & set__goal(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->goal = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Request
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Request
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const AssistEmergency_Request_ & other) const
  {
    if (this->start != other.start) {
      return false;
    }
    if (this->goal != other.goal) {
      return false;
    }
    return true;
  }
  bool operator!=(const AssistEmergency_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct AssistEmergency_Request_

// alias to use template instance with default allocator
using AssistEmergency_Request =
  path_manager_interfaces::srv::AssistEmergency_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces


// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Response __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Response __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct AssistEmergency_Response_
{
  using Type = AssistEmergency_Response_<ContainerAllocator>;

  explicit AssistEmergency_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit AssistEmergency_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_alloc, _init)
  {
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
  using _path_type =
    nav_msgs::msg::Path_<ContainerAllocator>;
  _path_type path;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__path(
    const nav_msgs::msg::Path_<ContainerAllocator> & _arg)
  {
    this->path = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Response
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__AssistEmergency_Response
    std::shared_ptr<path_manager_interfaces::srv::AssistEmergency_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const AssistEmergency_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->path != other.path) {
      return false;
    }
    return true;
  }
  bool operator!=(const AssistEmergency_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct AssistEmergency_Response_

// alias to use template instance with default allocator
using AssistEmergency_Response =
  path_manager_interfaces::srv::AssistEmergency_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces

namespace path_manager_interfaces
{

namespace srv
{

struct AssistEmergency
{
  using Request = path_manager_interfaces::srv::AssistEmergency_Request;
  using Response = path_manager_interfaces::srv::AssistEmergency_Response;
};

}  // namespace srv

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__ASSIST_EMERGENCY__STRUCT_HPP_
