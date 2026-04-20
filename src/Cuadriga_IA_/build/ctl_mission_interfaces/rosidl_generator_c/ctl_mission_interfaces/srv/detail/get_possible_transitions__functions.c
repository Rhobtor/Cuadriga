// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ctl_mission_interfaces:srv/GetPossibleTransitions.idl
// generated code does not contain a copyright notice
#include "ctl_mission_interfaces/srv/detail/get_possible_transitions__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(ctl_mission_interfaces__srv__GetPossibleTransitions_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__fini(ctl_mission_interfaces__srv__GetPossibleTransitions_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__are_equal(const ctl_mission_interfaces__srv__GetPossibleTransitions_Request * lhs, const ctl_mission_interfaces__srv__GetPossibleTransitions_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // structure_needs_at_least_one_member
  if (lhs->structure_needs_at_least_one_member != rhs->structure_needs_at_least_one_member) {
    return false;
  }
  return true;
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__copy(
  const ctl_mission_interfaces__srv__GetPossibleTransitions_Request * input,
  ctl_mission_interfaces__srv__GetPossibleTransitions_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // structure_needs_at_least_one_member
  output->structure_needs_at_least_one_member = input->structure_needs_at_least_one_member;
  return true;
}

ctl_mission_interfaces__srv__GetPossibleTransitions_Request *
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Request * msg = (ctl_mission_interfaces__srv__GetPossibleTransitions_Request *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Request));
  bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__destroy(ctl_mission_interfaces__srv__GetPossibleTransitions_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ctl_mission_interfaces__srv__GetPossibleTransitions_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__init(ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Request * data = NULL;

  if (size) {
    data = (ctl_mission_interfaces__srv__GetPossibleTransitions_Request *)allocator.zero_allocate(size, sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ctl_mission_interfaces__srv__GetPossibleTransitions_Request__fini(&data[i - 1]);
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
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__fini(ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * array)
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
      ctl_mission_interfaces__srv__GetPossibleTransitions_Request__fini(&array->data[i]);
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

ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence *
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * array = (ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__destroy(ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__are_equal(const ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * lhs, const ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__copy(
  const ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * input,
  ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ctl_mission_interfaces__srv__GetPossibleTransitions_Request * data =
      (ctl_mission_interfaces__srv__GetPossibleTransitions_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ctl_mission_interfaces__srv__GetPossibleTransitions_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `possible_transitions`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(ctl_mission_interfaces__srv__GetPossibleTransitions_Response * msg)
{
  if (!msg) {
    return false;
  }
  // possible_transitions
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->possible_transitions, 0)) {
    ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(msg);
    return false;
  }
  return true;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(ctl_mission_interfaces__srv__GetPossibleTransitions_Response * msg)
{
  if (!msg) {
    return;
  }
  // possible_transitions
  rosidl_runtime_c__int32__Sequence__fini(&msg->possible_transitions);
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__are_equal(const ctl_mission_interfaces__srv__GetPossibleTransitions_Response * lhs, const ctl_mission_interfaces__srv__GetPossibleTransitions_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // possible_transitions
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->possible_transitions), &(rhs->possible_transitions)))
  {
    return false;
  }
  return true;
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__copy(
  const ctl_mission_interfaces__srv__GetPossibleTransitions_Response * input,
  ctl_mission_interfaces__srv__GetPossibleTransitions_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // possible_transitions
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->possible_transitions), &(output->possible_transitions)))
  {
    return false;
  }
  return true;
}

ctl_mission_interfaces__srv__GetPossibleTransitions_Response *
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Response * msg = (ctl_mission_interfaces__srv__GetPossibleTransitions_Response *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Response));
  bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__destroy(ctl_mission_interfaces__srv__GetPossibleTransitions_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__init(ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Response * data = NULL;

  if (size) {
    data = (ctl_mission_interfaces__srv__GetPossibleTransitions_Response *)allocator.zero_allocate(size, sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(&data[i - 1]);
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
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__fini(ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * array)
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
      ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(&array->data[i]);
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

ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence *
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * array = (ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence *)allocator.allocate(sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__destroy(ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__are_equal(const ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * lhs, const ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__copy(
  const ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * input,
  ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ctl_mission_interfaces__srv__GetPossibleTransitions_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ctl_mission_interfaces__srv__GetPossibleTransitions_Response * data =
      (ctl_mission_interfaces__srv__GetPossibleTransitions_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ctl_mission_interfaces__srv__GetPossibleTransitions_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ctl_mission_interfaces__srv__GetPossibleTransitions_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
