// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from ctl_mission_interfaces:srv/ConfigSimpleCtl.idl
// generated code does not contain a copyright notice

#ifndef CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__FUNCTIONS_H_
#define CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "ctl_mission_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "ctl_mission_interfaces/srv/detail/config_simple_ctl__struct.h"

/// Initialize srv/ConfigSimpleCtl message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request
 * )) before or use
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg);

/// Finalize srv/ConfigSimpleCtl message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg);

/// Create srv/ConfigSimpleCtl message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__create();

/// Destroy srv/ConfigSimpleCtl message.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg);

/// Check for srv/ConfigSimpleCtl message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * rhs);

/// Copy a srv/ConfigSimpleCtl message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * output);

/// Initialize array of srv/ConfigSimpleCtl messages.
/**
 * It allocates the memory for the number of elements and calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array, size_t size);

/// Finalize array of srv/ConfigSimpleCtl messages.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array);

/// Create array of srv/ConfigSimpleCtl messages.
/**
 * It allocates the memory for the array and calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__create(size_t size);

/// Destroy array of srv/ConfigSimpleCtl messages.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array);

/// Check for srv/ConfigSimpleCtl message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * rhs);

/// Copy an array of srv/ConfigSimpleCtl messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * output);

/// Initialize srv/ConfigSimpleCtl message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response
 * )) before or use
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg);

/// Finalize srv/ConfigSimpleCtl message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg);

/// Create srv/ConfigSimpleCtl message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__create();

/// Destroy srv/ConfigSimpleCtl message.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg);

/// Check for srv/ConfigSimpleCtl message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * rhs);

/// Copy a srv/ConfigSimpleCtl message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * output);

/// Initialize array of srv/ConfigSimpleCtl messages.
/**
 * It allocates the memory for the number of elements and calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array, size_t size);

/// Finalize array of srv/ConfigSimpleCtl messages.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array);

/// Create array of srv/ConfigSimpleCtl messages.
/**
 * It allocates the memory for the array and calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__create(size_t size);

/// Destroy array of srv/ConfigSimpleCtl messages.
/**
 * It calls
 * ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array);

/// Check for srv/ConfigSimpleCtl message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * rhs);

/// Copy an array of srv/ConfigSimpleCtl messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_ctl_mission_interfaces
bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // CTL_MISSION_INTERFACES__SRV__DETAIL__CONFIG_SIMPLE_CTL__FUNCTIONS_H_
