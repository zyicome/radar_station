// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_msgss:msg/Yolopoints.idl
// generated code does not contain a copyright notice
#include "my_msgss/msg/detail/yolopoints__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `text`
#include "rosidl_runtime_c/string_functions.h"
// Member `data`
#include "my_msgss/msg/detail/yolopoint__functions.h"

bool
my_msgss__msg__Yolopoints__init(my_msgss__msg__Yolopoints * msg)
{
  if (!msg) {
    return false;
  }
  // id
  // text
  if (!rosidl_runtime_c__String__init(&msg->text)) {
    my_msgss__msg__Yolopoints__fini(msg);
    return false;
  }
  // data
  if (!my_msgss__msg__Yolopoint__Sequence__init(&msg->data, 0)) {
    my_msgss__msg__Yolopoints__fini(msg);
    return false;
  }
  return true;
}

void
my_msgss__msg__Yolopoints__fini(my_msgss__msg__Yolopoints * msg)
{
  if (!msg) {
    return;
  }
  // id
  // text
  rosidl_runtime_c__String__fini(&msg->text);
  // data
  my_msgss__msg__Yolopoint__Sequence__fini(&msg->data);
}

bool
my_msgss__msg__Yolopoints__are_equal(const my_msgss__msg__Yolopoints * lhs, const my_msgss__msg__Yolopoints * rhs)
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
  if (!my_msgss__msg__Yolopoint__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
my_msgss__msg__Yolopoints__copy(
  const my_msgss__msg__Yolopoints * input,
  my_msgss__msg__Yolopoints * output)
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
  if (!my_msgss__msg__Yolopoint__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

my_msgss__msg__Yolopoints *
my_msgss__msg__Yolopoints__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoints * msg = (my_msgss__msg__Yolopoints *)allocator.allocate(sizeof(my_msgss__msg__Yolopoints), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_msgss__msg__Yolopoints));
  bool success = my_msgss__msg__Yolopoints__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_msgss__msg__Yolopoints__destroy(my_msgss__msg__Yolopoints * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_msgss__msg__Yolopoints__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_msgss__msg__Yolopoints__Sequence__init(my_msgss__msg__Yolopoints__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoints * data = NULL;

  if (size) {
    data = (my_msgss__msg__Yolopoints *)allocator.zero_allocate(size, sizeof(my_msgss__msg__Yolopoints), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_msgss__msg__Yolopoints__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_msgss__msg__Yolopoints__fini(&data[i - 1]);
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
my_msgss__msg__Yolopoints__Sequence__fini(my_msgss__msg__Yolopoints__Sequence * array)
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
      my_msgss__msg__Yolopoints__fini(&array->data[i]);
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

my_msgss__msg__Yolopoints__Sequence *
my_msgss__msg__Yolopoints__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoints__Sequence * array = (my_msgss__msg__Yolopoints__Sequence *)allocator.allocate(sizeof(my_msgss__msg__Yolopoints__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_msgss__msg__Yolopoints__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_msgss__msg__Yolopoints__Sequence__destroy(my_msgss__msg__Yolopoints__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_msgss__msg__Yolopoints__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_msgss__msg__Yolopoints__Sequence__are_equal(const my_msgss__msg__Yolopoints__Sequence * lhs, const my_msgss__msg__Yolopoints__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_msgss__msg__Yolopoints__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_msgss__msg__Yolopoints__Sequence__copy(
  const my_msgss__msg__Yolopoints__Sequence * input,
  my_msgss__msg__Yolopoints__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_msgss__msg__Yolopoints);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_msgss__msg__Yolopoints * data =
      (my_msgss__msg__Yolopoints *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_msgss__msg__Yolopoints__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_msgss__msg__Yolopoints__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_msgss__msg__Yolopoints__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
