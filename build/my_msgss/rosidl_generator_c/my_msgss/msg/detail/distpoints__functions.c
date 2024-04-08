// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_msgss:msg/Distpoints.idl
// generated code does not contain a copyright notice
#include "my_msgss/msg/detail/distpoints__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `text`
#include "rosidl_runtime_c/string_functions.h"
// Member `data`
#include "my_msgss/msg/detail/distpoint__functions.h"

bool
my_msgss__msg__Distpoints__init(my_msgss__msg__Distpoints * msg)
{
  if (!msg) {
    return false;
  }
  // id
  // text
  if (!rosidl_runtime_c__String__init(&msg->text)) {
    my_msgss__msg__Distpoints__fini(msg);
    return false;
  }
  // data
  if (!my_msgss__msg__Distpoint__Sequence__init(&msg->data, 0)) {
    my_msgss__msg__Distpoints__fini(msg);
    return false;
  }
  return true;
}

void
my_msgss__msg__Distpoints__fini(my_msgss__msg__Distpoints * msg)
{
  if (!msg) {
    return;
  }
  // id
  // text
  rosidl_runtime_c__String__fini(&msg->text);
  // data
  my_msgss__msg__Distpoint__Sequence__fini(&msg->data);
}

bool
my_msgss__msg__Distpoints__are_equal(const my_msgss__msg__Distpoints * lhs, const my_msgss__msg__Distpoints * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // id
  if (lhs->id != rhs->id) {
    return false;
  }
  // text
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->text), &(rhs->text)))
  {
    return false;
  }
  // data
  if (!my_msgss__msg__Distpoint__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
my_msgss__msg__Distpoints__copy(
  const my_msgss__msg__Distpoints * input,
  my_msgss__msg__Distpoints * output)
{
  if (!input || !output) {
    return false;
  }
  // id
  output->id = input->id;
  // text
  if (!rosidl_runtime_c__String__copy(
      &(input->text), &(output->text)))
  {
    return false;
  }
  // data
  if (!my_msgss__msg__Distpoint__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

my_msgss__msg__Distpoints *
my_msgss__msg__Distpoints__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Distpoints * msg = (my_msgss__msg__Distpoints *)allocator.allocate(sizeof(my_msgss__msg__Distpoints), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_msgss__msg__Distpoints));
  bool success = my_msgss__msg__Distpoints__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_msgss__msg__Distpoints__destroy(my_msgss__msg__Distpoints * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_msgss__msg__Distpoints__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_msgss__msg__Distpoints__Sequence__init(my_msgss__msg__Distpoints__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Distpoints * data = NULL;

  if (size) {
    data = (my_msgss__msg__Distpoints *)allocator.zero_allocate(size, sizeof(my_msgss__msg__Distpoints), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_msgss__msg__Distpoints__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_msgss__msg__Distpoints__fini(&data[i - 1]);
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
my_msgss__msg__Distpoints__Sequence__fini(my_msgss__msg__Distpoints__Sequence * array)
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
      my_msgss__msg__Distpoints__fini(&array->data[i]);
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

my_msgss__msg__Distpoints__Sequence *
my_msgss__msg__Distpoints__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Distpoints__Sequence * array = (my_msgss__msg__Distpoints__Sequence *)allocator.allocate(sizeof(my_msgss__msg__Distpoints__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_msgss__msg__Distpoints__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_msgss__msg__Distpoints__Sequence__destroy(my_msgss__msg__Distpoints__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_msgss__msg__Distpoints__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_msgss__msg__Distpoints__Sequence__are_equal(const my_msgss__msg__Distpoints__Sequence * lhs, const my_msgss__msg__Distpoints__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_msgss__msg__Distpoints__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_msgss__msg__Distpoints__Sequence__copy(
  const my_msgss__msg__Distpoints__Sequence * input,
  my_msgss__msg__Distpoints__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_msgss__msg__Distpoints);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_msgss__msg__Distpoints * data =
      (my_msgss__msg__Distpoints *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_msgss__msg__Distpoints__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_msgss__msg__Distpoints__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_msgss__msg__Distpoints__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
