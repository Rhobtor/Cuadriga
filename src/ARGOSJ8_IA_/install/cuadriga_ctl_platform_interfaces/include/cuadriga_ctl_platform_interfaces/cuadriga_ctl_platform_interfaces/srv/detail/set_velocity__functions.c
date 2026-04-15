// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from cuadriga_ctl_platform_interfaces:srv/SetVelocity.idl
// generated code does not contain a copyright notice
#include "cuadriga_ctl_platform_interfaces/srv/detail/set_velocity__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * msg)
{
  if (!msg) {
    return false;
  }
  // new_velocity
  return true;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__fini(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * msg)
{
  if (!msg) {
    return;
  }
  // new_velocity
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__are_equal(const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * lhs, const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // new_velocity
  if (lhs->new_velocity != rhs->new_velocity) {
    return false;
  }
  return true;
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__copy(
  const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * input,
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // new_velocity
  output->new_velocity = input->new_velocity;
  return true;
}

cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request *
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * msg = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request *)allocator.allocate(sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request));
  bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__destroy(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__init(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * data = NULL;

  if (size) {
    data = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request *)allocator.zero_allocate(size, sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__fini(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence *
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * array = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence *)allocator.allocate(sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__destroy(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__are_equal(const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * lhs, const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__copy(
  const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * input,
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request * data =
      (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * msg)
{
  if (!msg) {
    return false;
  }
  // actual_velocity
  return true;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__fini(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * msg)
{
  if (!msg) {
    return;
  }
  // actual_velocity
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__are_equal(const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * lhs, const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // actual_velocity
  if (lhs->actual_velocity != rhs->actual_velocity) {
    return false;
  }
  return true;
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__copy(
  const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * input,
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // actual_velocity
  output->actual_velocity = input->actual_velocity;
  return true;
}

cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response *
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * msg = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response *)allocator.allocate(sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response));
  bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__destroy(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__init(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * data = NULL;

  if (size) {
    data = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response *)allocator.zero_allocate(size, sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__fini(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence *
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * array = (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence *)allocator.allocate(sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__destroy(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__are_equal(const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * lhs, const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__copy(
  const cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * input,
  cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response * data =
      (cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
