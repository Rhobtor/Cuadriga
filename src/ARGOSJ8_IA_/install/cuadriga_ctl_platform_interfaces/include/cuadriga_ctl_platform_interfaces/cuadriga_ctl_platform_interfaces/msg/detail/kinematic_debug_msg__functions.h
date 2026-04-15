// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice

#ifndef CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__FUNCTIONS_H_
#define CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "cuadriga_ctl_platform_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__struct.h"

/// Initialize msg/KinematicDebugMsg message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
 * )) before or use
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * msg);

/// Finalize msg/KinematicDebugMsg message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
void
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__fini(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * msg);

/// Create msg/KinematicDebugMsg message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg *
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__create();

/// Destroy msg/KinematicDebugMsg message.
/**
 * It calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
void
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__destroy(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * msg);

/// Check for msg/KinematicDebugMsg message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__are_equal(const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * lhs, const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * rhs);

/// Copy a msg/KinematicDebugMsg message.
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
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__copy(
  const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * input,
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * output);

/// Initialize array of msg/KinematicDebugMsg messages.
/**
 * It allocates the memory for the number of elements and calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__init(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * array, size_t size);

/// Finalize array of msg/KinematicDebugMsg messages.
/**
 * It calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
void
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__fini(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * array);

/// Create array of msg/KinematicDebugMsg messages.
/**
 * It allocates the memory for the array and calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence *
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__create(size_t size);

/// Destroy array of msg/KinematicDebugMsg messages.
/**
 * It calls
 * cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
void
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__destroy(cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * array);

/// Check for msg/KinematicDebugMsg message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__are_equal(const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * lhs, const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * rhs);

/// Copy an array of msg/KinematicDebugMsg messages.
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
ROSIDL_GENERATOR_C_PUBLIC_cuadriga_ctl_platform_interfaces
bool
cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__copy(
  const cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * input,
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // CUADRIGA_CTL_PLATFORM_INTERFACES__MSG__DETAIL__KINEMATIC_DEBUG_MSG__FUNCTIONS_H_
