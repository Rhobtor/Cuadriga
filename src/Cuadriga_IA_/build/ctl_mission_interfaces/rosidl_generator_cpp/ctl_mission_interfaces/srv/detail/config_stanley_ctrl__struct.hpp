// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigStanleyCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ConfigStanleyCtrl_Request_
{
  using Type = ConfigStanleyCtrl_Request_<ContainerAllocator>;

  explicit ConfigStanleyCtrl_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->v_forward = 0.0f;
      this->l_ahead_dist = 0.0f;
      this->k_error_lineal = 0.0f;
      this->k_error_angular = 0.0f;
    }
  }

  explicit ConfigStanleyCtrl_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->v_forward = 0.0f;
      this->l_ahead_dist = 0.0f;
      this->k_error_lineal = 0.0f;
      this->k_error_angular = 0.0f;
    }
  }

  // field types and members
  using _v_forward_type =
    float;
  _v_forward_type v_forward;
  using _l_ahead_dist_type =
    float;
  _l_ahead_dist_type l_ahead_dist;
  using _k_error_lineal_type =
    float;
  _k_error_lineal_type k_error_lineal;
  using _k_error_angular_type =
    float;
  _k_error_angular_type k_error_angular;

  // setters for named parameter idiom
  Type & set__v_forward(
    const float & _arg)
  {
    this->v_forward = _arg;
    return *this;
  }
  Type & set__l_ahead_dist(
    const float & _arg)
  {
    this->l_ahead_dist = _arg;
    return *this;
  }
  Type & set__k_error_lineal(
    const float & _arg)
  {
    this->k_error_lineal = _arg;
    return *this;
  }
  Type & set__k_error_angular(
    const float & _arg)
  {
    this->k_error_angular = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ConfigStanleyCtrl_Request_ & other) const
  {
    if (this->v_forward != other.v_forward) {
      return false;
    }
    if (this->l_ahead_dist != other.l_ahead_dist) {
      return false;
    }
    if (this->k_error_lineal != other.k_error_lineal) {
      return false;
    }
    if (this->k_error_angular != other.k_error_angular) {
      return false;
    }
    return true;
  }
  bool operator!=(const ConfigStanleyCtrl_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ConfigStanleyCtrl_Request_

// alias to use template instance with default allocator
using ConfigStanleyCtrl_Request =
  ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ConfigStanleyCtrl_Response_
{
  using Type = ConfigStanleyCtrl_Response_<ContainerAllocator>;

  explicit ConfigStanleyCtrl_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ack = false;
    }
  }

  explicit ConfigStanleyCtrl_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ack = false;
    }
  }

  // field types and members
  using _ack_type =
    bool;
  _ack_type ack;

  // setters for named parameter idiom
  Type & set__ack(
    const bool & _arg)
  {
    this->ack = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ConfigStanleyCtrl_Response_ & other) const
  {
    if (this->ack != other.ack) {
      return false;
    }
    return true;
  }
  bool operator!=(const ConfigStanleyCtrl_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ConfigStanleyCtrl_Response_

// alias to use template instance with default allocator
using ConfigStanleyCtrl_Response =
  ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace ctl_mission_interfaces
{

namespace srv
{

struct ConfigStanleyCtrl
{
  using Request = ctl_mission_interfaces::srv::ConfigStanleyCtrl_Request;
  using Response = ctl_mission_interfaces::srv::ConfigStanleyCtrl_Response;
};

}  // namespace srv

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_STANLEY_CTRL__STRUCT_HPP_
