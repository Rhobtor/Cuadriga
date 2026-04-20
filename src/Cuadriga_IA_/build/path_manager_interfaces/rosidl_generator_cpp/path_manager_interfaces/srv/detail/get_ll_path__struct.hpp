// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from path_manager_interfaces:srv/GetLLPath.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__STRUCT_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__GetLLPath_Request __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__GetLLPath_Request __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetLLPath_Request_
{
  using Type = GetLLPath_Request_<ContainerAllocator>;

  explicit GetLLPath_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetLLPath_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__GetLLPath_Request
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__GetLLPath_Request
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetLLPath_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetLLPath_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetLLPath_Request_

// alias to use template instance with default allocator
using GetLLPath_Request =
  path_manager_interfaces::srv::GetLLPath_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces


// Include directives for member types
// Member 'path'
#include "nav_msgs/msg/detail/path__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__GetLLPath_Response __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__GetLLPath_Response __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetLLPath_Response_
{
  using Type = GetLLPath_Response_<ContainerAllocator>;

  explicit GetLLPath_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit GetLLPath_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : path(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  // field types and members
  using _path_type =
    nav_msgs::msg::Path_<ContainerAllocator>;
  _path_type path;
  using _success_type =
    bool;
  _success_type success;

  // setters for named parameter idiom
  Type & set__path(
    const nav_msgs::msg::Path_<ContainerAllocator> & _arg)
  {
    this->path = _arg;
    return *this;
  }
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__GetLLPath_Response
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__GetLLPath_Response
    std::shared_ptr<path_manager_interfaces::srv::GetLLPath_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetLLPath_Response_ & other) const
  {
    if (this->path != other.path) {
      return false;
    }
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetLLPath_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetLLPath_Response_

// alias to use template instance with default allocator
using GetLLPath_Response =
  path_manager_interfaces::srv::GetLLPath_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces

namespace path_manager_interfaces
{

namespace srv
{

struct GetLLPath
{
  using Request = path_manager_interfaces::srv::GetLLPath_Request;
  using Response = path_manager_interfaces::srv::GetLLPath_Response;
};

}  // namespace srv

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__GET_LL_PATH__STRUCT_HPP_
