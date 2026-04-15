// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from cuadriga_interfaces:msg/GPXPath.idl
// generated code does not contain a copyright notice
#include "cuadriga_interfaces/msg/detail/gpx_path__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `waypoints`
#include "sensor_msgs/msg/detail/nav_sat_fix__functions.h"

bool
cuadriga_interfaces__msg__GPXPath__init(cuadriga_interfaces__msg__GPXPath * msg)
{
  if (!msg) {
    return false;
  }
  // waypoints
  if (!sensor_msgs__msg__NavSatFix__Sequence__init(&msg->waypoints, 0)) {
    cuadriga_interfaces__msg__GPXPath__fini(msg);
    return false;
  }
  return true;
}

void
cuadriga_interfaces__msg__GPXPath__fini(cuadriga_interfaces__msg__GPXPath * msg)
{
  if (!msg) {
    return;
  }
  // waypoints
  sensor_msgs__msg__NavSatFix__Sequence__fini(&msg->waypoints);
}

bool
cuadriga_interfaces__msg__GPXPath__are_equal(const cuadriga_interfaces__msg__GPXPath * lhs, const cuadriga_interfaces__msg__GPXPath * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // waypoints
  if (!sensor_msgs__msg__NavSatFix__Sequence__are_equal(
      &(lhs->waypoints), &(rhs->waypoints)))
  {
    return false;
  }
  return true;
}

bool
cuadriga_interfaces__msg__GPXPath__copy(
  const cuadriga_interfaces__msg__GPXPath * input,
  cuadriga_interfaces__msg__GPXPath * output)
{
  if (!input || !output) {
    return false;
  }
  // waypoints
  if (!sensor_msgs__msg__NavSatFix__Sequence__copy(
      &(input->waypoints), &(output->waypoints)))
  {
    return false;
  }
  return true;
}

cuadriga_interfaces__msg__GPXPath *
cuadriga_interfaces__msg__GPXPath__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_interfaces__msg__GPXPath * msg = (cuadriga_interfaces__msg__GPXPath *)allocator.allocate(sizeof(cuadriga_interfaces__msg__GPXPath), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cuadriga_interfaces__msg__GPXPath));
  bool success = cuadriga_interfaces__msg__GPXPath__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cuadriga_interfaces__msg__GPXPath__destroy(cuadriga_interfaces__msg__GPXPath * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cuadriga_interfaces__msg__GPXPath__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cuadriga_interfaces__msg__GPXPath__Sequence__init(cuadriga_interfaces__msg__GPXPath__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_interfaces__msg__GPXPath * data = NULL;

  if (size) {
    data = (cuadriga_interfaces__msg__GPXPath *)allocator.zero_allocate(size, sizeof(cuadriga_interfaces__msg__GPXPath), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cuadriga_interfaces__msg__GPXPath__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cuadriga_interfaces__msg__GPXPath__fini(&data[i - 1]);
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
cuadriga_interfaces__msg__GPXPath__Sequence__fini(cuadriga_interfaces__msg__GPXPath__Sequence * array)
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
      cuadriga_interfaces__msg__GPXPath__fini(&array->data[i]);
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

cuadriga_interfaces__msg__GPXPath__Sequence *
cuadriga_interfaces__msg__GPXPath__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cuadriga_interfaces__msg__GPXPath__Sequence * array = (cuadriga_interfaces__msg__GPXPath__Sequence *)allocator.allocate(sizeof(cuadriga_interfaces__msg__GPXPath__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cuadriga_interfaces__msg__GPXPath__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cuadriga_interfaces__msg__GPXPath__Sequence__destroy(cuadriga_interfaces__msg__GPXPath__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cuadriga_interfaces__msg__GPXPath__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cuadriga_interfaces__msg__GPXPath__Sequence__are_equal(const cuadriga_interfaces__msg__GPXPath__Sequence * lhs, const cuadriga_interfaces__msg__GPXPath__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cuadriga_interfaces__msg__GPXPath__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cuadriga_interfaces__msg__GPXPath__Sequence__copy(
  const cuadriga_interfaces__msg__GPXPath__Sequence * input,
  cuadriga_interfaces__msg__GPXPath__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cuadriga_interfaces__msg__GPXPath);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cuadriga_interfaces__msg__GPXPath * data =
      (cuadriga_interfaces__msg__GPXPath *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cuadriga_interfaces__msg__GPXPath__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cuadriga_interfaces__msg__GPXPath__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cuadriga_interfaces__msg__GPXPath__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
