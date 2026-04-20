// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from path_manager_interfaces:srv/RobotPath.idl
// generated code does not contain a copyright notice
#include "path_manager_interfaces/srv/detail/robot_path__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `path`
#include "nav_msgs/msg/detail/path__functions.h"

bool
path_manager_interfaces__srv__RobotPath_Request__init(path_manager_interfaces__srv__RobotPath_Request * msg)
{
  if (!msg) {
    return false;
  }
  // path
  if (!nav_msgs__msg__Path__init(&msg->path)) {
    path_manager_interfaces__srv__RobotPath_Request__fini(msg);
    return false;
  }
  return true;
}

void
path_manager_interfaces__srv__RobotPath_Request__fini(path_manager_interfaces__srv__RobotPath_Request * msg)
{
  if (!msg) {
    return;
  }
  // path
  nav_msgs__msg__Path__fini(&msg->path);
}

bool
path_manager_interfaces__srv__RobotPath_Request__are_equal(const path_manager_interfaces__srv__RobotPath_Request * lhs, const path_manager_interfaces__srv__RobotPath_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // path
  if (!nav_msgs__msg__Path__are_equal(
      &(lhs->path), &(rhs->path)))
  {
    return false;
  }
  return true;
}

bool
path_manager_interfaces__srv__RobotPath_Request__copy(
  const path_manager_interfaces__srv__RobotPath_Request * input,
  path_manager_interfaces__srv__RobotPath_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // path
  if (!nav_msgs__msg__Path__copy(
      &(input->path), &(output->path)))
  {
    return false;
  }
  return true;
}

path_manager_interfaces__srv__RobotPath_Request *
path_manager_interfaces__srv__RobotPath_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Request * msg = (path_manager_interfaces__srv__RobotPath_Request *)allocator.allocate(sizeof(path_manager_interfaces__srv__RobotPath_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(path_manager_interfaces__srv__RobotPath_Request));
  bool success = path_manager_interfaces__srv__RobotPath_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
path_manager_interfaces__srv__RobotPath_Request__destroy(path_manager_interfaces__srv__RobotPath_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    path_manager_interfaces__srv__RobotPath_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
path_manager_interfaces__srv__RobotPath_Request__Sequence__init(path_manager_interfaces__srv__RobotPath_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Request * data = NULL;

  if (size) {
    data = (path_manager_interfaces__srv__RobotPath_Request *)allocator.zero_allocate(size, sizeof(path_manager_interfaces__srv__RobotPath_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = path_manager_interfaces__srv__RobotPath_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        path_manager_interfaces__srv__RobotPath_Request__fini(&data[i - 1]);
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
path_manager_interfaces__srv__RobotPath_Request__Sequence__fini(path_manager_interfaces__srv__RobotPath_Request__Sequence * array)
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
      path_manager_interfaces__srv__RobotPath_Request__fini(&array->data[i]);
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

path_manager_interfaces__srv__RobotPath_Request__Sequence *
path_manager_interfaces__srv__RobotPath_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Request__Sequence * array = (path_manager_interfaces__srv__RobotPath_Request__Sequence *)allocator.allocate(sizeof(path_manager_interfaces__srv__RobotPath_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = path_manager_interfaces__srv__RobotPath_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
path_manager_interfaces__srv__RobotPath_Request__Sequence__destroy(path_manager_interfaces__srv__RobotPath_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    path_manager_interfaces__srv__RobotPath_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
path_manager_interfaces__srv__RobotPath_Request__Sequence__are_equal(const path_manager_interfaces__srv__RobotPath_Request__Sequence * lhs, const path_manager_interfaces__srv__RobotPath_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!path_manager_interfaces__srv__RobotPath_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
path_manager_interfaces__srv__RobotPath_Request__Sequence__copy(
  const path_manager_interfaces__srv__RobotPath_Request__Sequence * input,
  path_manager_interfaces__srv__RobotPath_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(path_manager_interfaces__srv__RobotPath_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    path_manager_interfaces__srv__RobotPath_Request * data =
      (path_manager_interfaces__srv__RobotPath_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!path_manager_interfaces__srv__RobotPath_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          path_manager_interfaces__srv__RobotPath_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!path_manager_interfaces__srv__RobotPath_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
path_manager_interfaces__srv__RobotPath_Response__init(path_manager_interfaces__srv__RobotPath_Response * msg)
{
  if (!msg) {
    return false;
  }
  // ack
  return true;
}

void
path_manager_interfaces__srv__RobotPath_Response__fini(path_manager_interfaces__srv__RobotPath_Response * msg)
{
  if (!msg) {
    return;
  }
  // ack
}

bool
path_manager_interfaces__srv__RobotPath_Response__are_equal(const path_manager_interfaces__srv__RobotPath_Response * lhs, const path_manager_interfaces__srv__RobotPath_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // ack
  if (lhs->ack != rhs->ack) {
    return false;
  }
  return true;
}

bool
path_manager_interfaces__srv__RobotPath_Response__copy(
  const path_manager_interfaces__srv__RobotPath_Response * input,
  path_manager_interfaces__srv__RobotPath_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // ack
  output->ack = input->ack;
  return true;
}

path_manager_interfaces__srv__RobotPath_Response *
path_manager_interfaces__srv__RobotPath_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Response * msg = (path_manager_interfaces__srv__RobotPath_Response *)allocator.allocate(sizeof(path_manager_interfaces__srv__RobotPath_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(path_manager_interfaces__srv__RobotPath_Response));
  bool success = path_manager_interfaces__srv__RobotPath_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
path_manager_interfaces__srv__RobotPath_Response__destroy(path_manager_interfaces__srv__RobotPath_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    path_manager_interfaces__srv__RobotPath_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
path_manager_interfaces__srv__RobotPath_Response__Sequence__init(path_manager_interfaces__srv__RobotPath_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Response * data = NULL;

  if (size) {
    data = (path_manager_interfaces__srv__RobotPath_Response *)allocator.zero_allocate(size, sizeof(path_manager_interfaces__srv__RobotPath_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = path_manager_interfaces__srv__RobotPath_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        path_manager_interfaces__srv__RobotPath_Response__fini(&data[i - 1]);
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
path_manager_interfaces__srv__RobotPath_Response__Sequence__fini(path_manager_interfaces__srv__RobotPath_Response__Sequence * array)
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
      path_manager_interfaces__srv__RobotPath_Response__fini(&array->data[i]);
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

path_manager_interfaces__srv__RobotPath_Response__Sequence *
path_manager_interfaces__srv__RobotPath_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  path_manager_interfaces__srv__RobotPath_Response__Sequence * array = (path_manager_interfaces__srv__RobotPath_Response__Sequence *)allocator.allocate(sizeof(path_manager_interfaces__srv__RobotPath_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = path_manager_interfaces__srv__RobotPath_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
path_manager_interfaces__srv__RobotPath_Response__Sequence__destroy(path_manager_interfaces__srv__RobotPath_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    path_manager_interfaces__srv__RobotPath_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
path_manager_interfaces__srv__RobotPath_Response__Sequence__are_equal(const path_manager_interfaces__srv__RobotPath_Response__Sequence * lhs, const path_manager_interfaces__srv__RobotPath_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!path_manager_interfaces__srv__RobotPath_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
path_manager_interfaces__srv__RobotPath_Response__Sequence__copy(
  const path_manager_interfaces__srv__RobotPath_Response__Sequence * input,
  path_manager_interfaces__srv__RobotPath_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(path_manager_interfaces__srv__RobotPath_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    path_manager_interfaces__srv__RobotPath_Response * data =
      (path_manager_interfaces__srv__RobotPath_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!path_manager_interfaces__srv__RobotPath_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          path_manager_interfaces__srv__RobotPath_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!path_manager_interfaces__srv__RobotPath_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
