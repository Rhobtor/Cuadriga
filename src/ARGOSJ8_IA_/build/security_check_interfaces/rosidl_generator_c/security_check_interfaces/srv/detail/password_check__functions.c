// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from security_check_interfaces:srv/PasswordCheck.idl
// generated code does not contain a copyright notice
#include "security_check_interfaces/srv/detail/password_check__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `password`
#include "rosidl_runtime_c/string_functions.h"

bool
security_check_interfaces__srv__PasswordCheck_Request__init(security_check_interfaces__srv__PasswordCheck_Request * msg)
{
  if (!msg) {
    return false;
  }
  // password
  if (!rosidl_runtime_c__String__init(&msg->password)) {
    security_check_interfaces__srv__PasswordCheck_Request__fini(msg);
    return false;
  }
  return true;
}

void
security_check_interfaces__srv__PasswordCheck_Request__fini(security_check_interfaces__srv__PasswordCheck_Request * msg)
{
  if (!msg) {
    return;
  }
  // password
  rosidl_runtime_c__String__fini(&msg->password);
}

bool
security_check_interfaces__srv__PasswordCheck_Request__are_equal(const security_check_interfaces__srv__PasswordCheck_Request * lhs, const security_check_interfaces__srv__PasswordCheck_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // password
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->password), &(rhs->password)))
  {
    return false;
  }
  return true;
}

bool
security_check_interfaces__srv__PasswordCheck_Request__copy(
  const security_check_interfaces__srv__PasswordCheck_Request * input,
  security_check_interfaces__srv__PasswordCheck_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // password
  if (!rosidl_runtime_c__String__copy(
      &(input->password), &(output->password)))
  {
    return false;
  }
  return true;
}

security_check_interfaces__srv__PasswordCheck_Request *
security_check_interfaces__srv__PasswordCheck_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Request * msg = (security_check_interfaces__srv__PasswordCheck_Request *)allocator.allocate(sizeof(security_check_interfaces__srv__PasswordCheck_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(security_check_interfaces__srv__PasswordCheck_Request));
  bool success = security_check_interfaces__srv__PasswordCheck_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
security_check_interfaces__srv__PasswordCheck_Request__destroy(security_check_interfaces__srv__PasswordCheck_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    security_check_interfaces__srv__PasswordCheck_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
security_check_interfaces__srv__PasswordCheck_Request__Sequence__init(security_check_interfaces__srv__PasswordCheck_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Request * data = NULL;

  if (size) {
    data = (security_check_interfaces__srv__PasswordCheck_Request *)allocator.zero_allocate(size, sizeof(security_check_interfaces__srv__PasswordCheck_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = security_check_interfaces__srv__PasswordCheck_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        security_check_interfaces__srv__PasswordCheck_Request__fini(&data[i - 1]);
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
security_check_interfaces__srv__PasswordCheck_Request__Sequence__fini(security_check_interfaces__srv__PasswordCheck_Request__Sequence * array)
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
      security_check_interfaces__srv__PasswordCheck_Request__fini(&array->data[i]);
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

security_check_interfaces__srv__PasswordCheck_Request__Sequence *
security_check_interfaces__srv__PasswordCheck_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Request__Sequence * array = (security_check_interfaces__srv__PasswordCheck_Request__Sequence *)allocator.allocate(sizeof(security_check_interfaces__srv__PasswordCheck_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = security_check_interfaces__srv__PasswordCheck_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
security_check_interfaces__srv__PasswordCheck_Request__Sequence__destroy(security_check_interfaces__srv__PasswordCheck_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    security_check_interfaces__srv__PasswordCheck_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
security_check_interfaces__srv__PasswordCheck_Request__Sequence__are_equal(const security_check_interfaces__srv__PasswordCheck_Request__Sequence * lhs, const security_check_interfaces__srv__PasswordCheck_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!security_check_interfaces__srv__PasswordCheck_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
security_check_interfaces__srv__PasswordCheck_Request__Sequence__copy(
  const security_check_interfaces__srv__PasswordCheck_Request__Sequence * input,
  security_check_interfaces__srv__PasswordCheck_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(security_check_interfaces__srv__PasswordCheck_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    security_check_interfaces__srv__PasswordCheck_Request * data =
      (security_check_interfaces__srv__PasswordCheck_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!security_check_interfaces__srv__PasswordCheck_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          security_check_interfaces__srv__PasswordCheck_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!security_check_interfaces__srv__PasswordCheck_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
security_check_interfaces__srv__PasswordCheck_Response__init(security_check_interfaces__srv__PasswordCheck_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    security_check_interfaces__srv__PasswordCheck_Response__fini(msg);
    return false;
  }
  return true;
}

void
security_check_interfaces__srv__PasswordCheck_Response__fini(security_check_interfaces__srv__PasswordCheck_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
security_check_interfaces__srv__PasswordCheck_Response__are_equal(const security_check_interfaces__srv__PasswordCheck_Response * lhs, const security_check_interfaces__srv__PasswordCheck_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
security_check_interfaces__srv__PasswordCheck_Response__copy(
  const security_check_interfaces__srv__PasswordCheck_Response * input,
  security_check_interfaces__srv__PasswordCheck_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

security_check_interfaces__srv__PasswordCheck_Response *
security_check_interfaces__srv__PasswordCheck_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Response * msg = (security_check_interfaces__srv__PasswordCheck_Response *)allocator.allocate(sizeof(security_check_interfaces__srv__PasswordCheck_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(security_check_interfaces__srv__PasswordCheck_Response));
  bool success = security_check_interfaces__srv__PasswordCheck_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
security_check_interfaces__srv__PasswordCheck_Response__destroy(security_check_interfaces__srv__PasswordCheck_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    security_check_interfaces__srv__PasswordCheck_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
security_check_interfaces__srv__PasswordCheck_Response__Sequence__init(security_check_interfaces__srv__PasswordCheck_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Response * data = NULL;

  if (size) {
    data = (security_check_interfaces__srv__PasswordCheck_Response *)allocator.zero_allocate(size, sizeof(security_check_interfaces__srv__PasswordCheck_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = security_check_interfaces__srv__PasswordCheck_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        security_check_interfaces__srv__PasswordCheck_Response__fini(&data[i - 1]);
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
security_check_interfaces__srv__PasswordCheck_Response__Sequence__fini(security_check_interfaces__srv__PasswordCheck_Response__Sequence * array)
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
      security_check_interfaces__srv__PasswordCheck_Response__fini(&array->data[i]);
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

security_check_interfaces__srv__PasswordCheck_Response__Sequence *
security_check_interfaces__srv__PasswordCheck_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  security_check_interfaces__srv__PasswordCheck_Response__Sequence * array = (security_check_interfaces__srv__PasswordCheck_Response__Sequence *)allocator.allocate(sizeof(security_check_interfaces__srv__PasswordCheck_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = security_check_interfaces__srv__PasswordCheck_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
security_check_interfaces__srv__PasswordCheck_Response__Sequence__destroy(security_check_interfaces__srv__PasswordCheck_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    security_check_interfaces__srv__PasswordCheck_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
security_check_interfaces__srv__PasswordCheck_Response__Sequence__are_equal(const security_check_interfaces__srv__PasswordCheck_Response__Sequence * lhs, const security_check_interfaces__srv__PasswordCheck_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!security_check_interfaces__srv__PasswordCheck_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
security_check_interfaces__srv__PasswordCheck_Response__Sequence__copy(
  const security_check_interfaces__srv__PasswordCheck_Response__Sequence * input,
  security_check_interfaces__srv__PasswordCheck_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(security_check_interfaces__srv__PasswordCheck_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    security_check_interfaces__srv__PasswordCheck_Response * data =
      (security_check_interfaces__srv__PasswordCheck_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!security_check_interfaces__srv__PasswordCheck_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          security_check_interfaces__srv__PasswordCheck_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!security_check_interfaces__srv__PasswordCheck_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
