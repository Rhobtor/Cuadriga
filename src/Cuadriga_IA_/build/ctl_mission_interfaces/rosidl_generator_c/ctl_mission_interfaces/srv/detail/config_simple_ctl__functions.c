// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ctl_mission_interfaces:srv/ConfigSimpleCtl.idl
// generated code does not contain a copyright notice
#include "ctl_mission_interfaces/srv/detail/config_simple_ctl__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg)
{
  if (!msg) {
    return false;
  }
  // k_angle
  // points_in_window
  // k_position
  // v_forward
  // l_ahead_dist
  return true;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg)
{
  if (!msg) {
    return;
  }
  // k_angle
  // points_in_window
  // k_position
  // v_forward
  // l_ahead_dist
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // k_angle
  if (lhs->k_angle != rhs->k_angle) {
    return false;
  }
  // points_in_window
  if (lhs->points_in_window != rhs->points_in_window) {
    return false;
  }
  // k_position
  if (lhs->k_position != rhs->k_position) {
    return false;
  }
  // v_forward
  if (lhs->v_forward != rhs->v_forward) {
    return false;
  }
  // l_ahead_dist
  if (lhs->l_ahead_dist != rhs->l_ahead_dist) {
    return false;
  }
  return true;
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // k_angle
  output->k_angle = input->k_angle;
  // points_in_window
  output->points_in_window = input->points_in_window;
  // k_position
  output->k_position = input->k_position;
  // v_forward
  output->v_forward = input->v_forward;
  // l_ahead_dist
  output->l_ahead_dist = input->l_ahead_dist;
  return true;
}

ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request));
  bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * data = NULL;

  if (size) {
    data = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *)allocator.zero_allocate(size, sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(&data[i - 1]);
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
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array)
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
      ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(&array->data[i]);
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

ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * data =
      (ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg)
{
  if (!msg) {
    return false;
  }
  // ack
  return true;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg)
{
  if (!msg) {
    return;
  }
  // ack
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * rhs)
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
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // ack
  output->ack = input->ack;
  return true;
}

ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response));
  bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * data = NULL;

  if (size) {
    data = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *)allocator.zero_allocate(size, sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(&data[i - 1]);
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
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array)
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
      ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(&array->data[i]);
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

ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence *
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__destroy(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__are_equal(const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * lhs, const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__copy(
  const ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * input,
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ctl_mission_interfaces__srv__ConfigSimpleCtl_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * data =
      (ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
