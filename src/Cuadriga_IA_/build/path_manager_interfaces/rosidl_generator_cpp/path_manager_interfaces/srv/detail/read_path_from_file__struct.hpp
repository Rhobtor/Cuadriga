// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from path_manager_interfaces:srv/ReadPathFromFile.idl
// generated code does not contain a copyright notice

#ifndef PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_HPP_
#define PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Request __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Request __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ReadPathFromFile_Request_
{
  using Type = ReadPathFromFile_Request_<ContainerAllocator>;

  explicit ReadPathFromFile_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->filename = "";
    }
  }

  explicit ReadPathFromFile_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : filename(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->filename = "";
    }
  }

  // field types and members
  using _filename_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _filename_type filename;

  // setters for named parameter idiom
  Type & set__filename(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->filename = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Request
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Request
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ReadPathFromFile_Request_ & other) const
  {
    if (this->filename != other.filename) {
      return false;
    }
    return true;
  }
  bool operator!=(const ReadPathFromFile_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ReadPathFromFile_Request_

// alias to use template instance with default allocator
using ReadPathFromFile_Request =
  path_manager_interfaces::srv::ReadPathFromFile_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces


#ifndef _WIN32
# define DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Response __attribute__((deprecated))
#else
# define DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Response __declspec(deprecated)
#endif

namespace path_manager_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ReadPathFromFile_Response_
{
  using Type = ReadPathFromFile_Response_<ContainerAllocator>;

  explicit ReadPathFromFile_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit ReadPathFromFile_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Response
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__path_manager_interfaces__srv__ReadPathFromFile_Response
    std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ReadPathFromFile_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const ReadPathFromFile_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ReadPathFromFile_Response_

// alias to use template instance with default allocator
using ReadPathFromFile_Response =
  path_manager_interfaces::srv::ReadPathFromFile_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace path_manager_interfaces

namespace path_manager_interfaces
{

namespace srv
{

struct ReadPathFromFile
{
  using Request = path_manager_interfaces::srv::ReadPathFromFile_Request;
  using Response = path_manager_interfaces::srv::ReadPathFromFile_Response;
};

}  // namespace srv

}  // namespace path_manager_interfaces

#endif  // PATH_MANAGER_INTERFACES__SRV__DETAIL__READ_PATH_FROM_FILE__STRUCT_HPP_
