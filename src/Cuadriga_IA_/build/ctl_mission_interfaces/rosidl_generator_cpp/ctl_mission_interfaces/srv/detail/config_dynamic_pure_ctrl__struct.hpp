// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicPureCtrl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__STRUCT_HPP_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ConfigDynamicPureCtrl_Request_
{
  using Type = ConfigDynamicPureCtrl_Request_<ContainerAllocator>;

  explicit ConfigDynamicPureCtrl_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->look_ahead_dis = 0.0f;
      this->max_v_forward = 0.0f;
      this->max_ang_acc = 0.0f;
      this->max_ang_dec = 0.0f;
      this->max_lin_acc = 0.0f;
      this->max_lin_dec = 0.0f;
    }
  }

  explicit ConfigDynamicPureCtrl_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->look_ahead_dis = 0.0f;
      this->max_v_forward = 0.0f;
      this->max_ang_acc = 0.0f;
      this->max_ang_dec = 0.0f;
      this->max_lin_acc = 0.0f;
      this->max_lin_dec = 0.0f;
    }
  }

  // field types and members
  using _look_ahead_dis_type =
    float;
  _look_ahead_dis_type look_ahead_dis;
  using _max_v_forward_type =
    float;
  _max_v_forward_type max_v_forward;
  using _max_ang_acc_type =
    float;
  _max_ang_acc_type max_ang_acc;
  using _max_ang_dec_type =
    float;
  _max_ang_dec_type max_ang_dec;
  using _max_lin_acc_type =
    float;
  _max_lin_acc_type max_lin_acc;
  using _max_lin_dec_type =
    float;
  _max_lin_dec_type max_lin_dec;

  // setters for named parameter idiom
  Type & set__look_ahead_dis(
    const float & _arg)
  {
    this->look_ahead_dis = _arg;
    return *this;
  }
  Type & set__max_v_forward(
    const float & _arg)
  {
    this->max_v_forward = _arg;
    return *this;
  }
  Type & set__max_ang_acc(
    const float & _arg)
  {
    this->max_ang_acc = _arg;
    return *this;
  }
  Type & set__max_ang_dec(
    const float & _arg)
  {
    this->max_ang_dec = _arg;
    return *this;
  }
  Type & set__max_lin_acc(
    const float & _arg)
  {
    this->max_lin_acc = _arg;
    return *this;
  }
  Type & set__max_lin_dec(
    const float & _arg)
  {
    this->max_lin_dec = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ConfigDynamicPureCtrl_Request_ & other) const
  {
    if (this->look_ahead_dis != other.look_ahead_dis) {
      return false;
    }
    if (this->max_v_forward != other.max_v_forward) {
      return false;
    }
    if (this->max_ang_acc != other.max_ang_acc) {
      return false;
    }
    if (this->max_ang_dec != other.max_ang_dec) {
      return false;
    }
    if (this->max_lin_acc != other.max_lin_acc) {
      return false;
    }
    if (this->max_lin_dec != other.max_lin_dec) {
      return false;
    }
    return true;
  }
  bool operator!=(const ConfigDynamicPureCtrl_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ConfigDynamicPureCtrl_Request_

// alias to use template instance with default allocator
using ConfigDynamicPureCtrl_Request =
  ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces


#ifndef _WIN32
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response __attribute__((deprecated))
#else
# define DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response __declspec(deprecated)
#endif

namespace ctl_mission_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ConfigDynamicPureCtrl_Response_
{
  using Type = ConfigDynamicPureCtrl_Response_<ContainerAllocator>;

  explicit ConfigDynamicPureCtrl_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ack = false;
    }
  }

  explicit ConfigDynamicPureCtrl_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response
    std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ConfigDynamicPureCtrl_Response_ & other) const
  {
    if (this->ack != other.ack) {
      return false;
    }
    return true;
  }
  bool operator!=(const ConfigDynamicPureCtrl_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ConfigDynamicPureCtrl_Response_

// alias to use template instance with default allocator
using ConfigDynamicPureCtrl_Response =
  ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ctl_mission_interfaces

namespace ctl_mission_interfaces
{

namespace srv
{

struct ConfigDynamicPureCtrl
{
  using Request = ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Request;
  using Response = ctl_mission_interfaces::srv::ConfigDynamicPureCtrl_Response;
};

}  // namespace srv

}  // namespace ctl_mission_interfaces

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_DYNAMIC_PURE_CTRL__STRUCT_HPP_
