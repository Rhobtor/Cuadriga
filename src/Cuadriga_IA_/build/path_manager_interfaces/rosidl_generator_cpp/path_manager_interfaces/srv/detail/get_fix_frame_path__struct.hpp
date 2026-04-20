// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from path_manager_interfaces:srv/GetFixFramePath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__STRUCT_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Request __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Request __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetFixFramePath_Request_
{
  using Type = GetFixFramePath_Request_<ContainerAllocator>;

  explicit GetFixFramePath_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ask = false;
    }
  }

  explicit GetFixFramePath_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ask = false;
    }
  }

  // field types and members
  using _ask_type =
    bool;
  _ask_type ask;

  // setters for named parameter idiom
  Type & set__ask(
    const bool & _arg)
  {
    this->ask = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Request
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Request
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetFixFramePath_Request_ & other) const
  {
    if (this->ask != other.ask) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetFixFramePath_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetFixFramePath_Request_

// alias to use template instance with default allocator
using GetFixFramePath_Request =
  path_manager_interfaces::srv::GetFixFramePath_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces


// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Response __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Response __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetFixFramePath_Response_
{
  using Type = GetFixFramePath_Response_<ContainerAllocator>;

  explicit GetFixFramePath_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_init)
  {
    (void)_init;
  }

  explicit GetFixFramePath_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _path_type =
    nav_msgs::msg::Path_<ContainerAllocator>;
  _path_type path;

  // setters for named parameter idiom
  Type & set__path(
    const nav_msgs::msg::Path_<ContainerAllocator> & _arg)
  {
    this->path = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Response
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__GetFixFramePath_Response
    std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetFixFramePath_Response_ & other) const
  {
    if (this->path != other.path) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetFixFramePath_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetFixFramePath_Response_

// alias to use template instance with default allocator
using GetFixFramePath_Response =
  path_manager_interfaces::srv::GetFixFramePath_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces

namespace path_manager_interfaces
{

namespace srv
{

struct GetFixFramePath
{
  using Request = path_manager_interfaces::srv::GetFixFramePath_Request;
  using Response = path_manager_interfaces::srv::GetFixFramePath_Response;
};

}  // namespace srv

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_FIX_FRAME_PATH__STRUCT_HPP_
