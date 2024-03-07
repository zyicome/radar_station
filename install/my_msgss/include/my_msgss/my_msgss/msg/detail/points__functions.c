// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_msgss:msg/Points.idl
// generated code does not contain a copyright notice
#include "my_msgss/msg/detail/points__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `color`
// Member `text`
#include "rosidl_runtime_c/string_functions.h"
// Member `data`
#include "my_msgss/msg/detail/point__functions.h"

bool
my_msgss__msg__Points__init(my_msgss__msg__Points * msg)
{
  if (!msg) {
    return false;
  }
  // id
  // color
  if (!rosidl_runtime_c__String__init(&msg->color)) {
    my_msgss__msg__Points__fini(msg);
    return false;
  }
  // text
  if (!rosidl_runtime_c__String__init(&msg->text)) {
    my_msgss__msg__Points__fini(msg);
    return false;
  }
  // data
  if (!my_msgss__msg__Point__Sequence__init(&msg->data, 0)) {
    my_msgss__msg__Points__fini(msg);
    return false;
  }
  return true;
}

void
my_msgss__msg__Points__fini(my_msgss__msg__Points * msg)
{
  if (!msg) {
    return;
  }
  // id
  // color
  rosidl_runtime_c__String__fini(&msg->color);
  // text
  rosidl_runtime_c__String__fini(&msg->text);
  // data
  my_msgss__msg__Point__Sequence__fini(&msg->data);
}

bool
my_msgss__msg__Points__are_equal(const my_msgss__msg__Points * lhs, const my_msgss__msg__Points * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // id
  if (lhs->id != rhs->id) {
    return false;
  }
  // color
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->color), &(rhs->color)))
  {
    return false;
  }
  // text
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->text), &(rhs->text)))
  {
    return false;
  }
  // data
  if (!my_msgss__msg__Point__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
my_msgss__msg__Points__copy(
  const my_msgss__msg__Points * input,
  my_msgss__msg__Points * output)
{
  if (!input || !output) {
    return false;
  }
  // id
  output->id = input->id;
  // color
  if (!rosidl_runtime_c__String__copy(
      &(input->color), &(output->color)))
  {
    return false;
  }
  // text
  if (!rosidl_runtime_c__String__copy(
      &(input->text), &(output->text)))
  {
    return false;
  }
  // data
  if (!my_msgss__msg__Point__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

my_msgss__msg__Points *
my_msgss__msg__Points__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Points * msg = (my_msgss__msg__Points *)allocator.allocate(sizeof(my_msgss__msg__Points), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_msgss__msg__Points));
  bool success = my_msgss__msg__Points__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_msgss__msg__Points__destroy(my_msgss__msg__Points * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_msgss__msg__Points__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_msgss__msg__Points__Sequence__init(my_msgss__msg__Points__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Points * data = NULL;

  if (size) {
    data = (my_msgss__msg__Points *)allocator.zero_allocate(size, sizeof(my_msgss__msg__Points), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_msgss__msg__Points__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_msgss__msg__Points__fini(&data[i - 1]);
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
my_msgss__msg__Points__Sequence__fini(my_msgss__msg__Points__Sequence * array)
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
      my_msgss__msg__Points__fini(&array->data[i]);
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

my_msgss__msg__Points__Sequence *
my_msgss__msg__Points__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Points__Sequence * array = (my_msgss__msg__Points__Sequence *)allocator.allocate(sizeof(my_msgss__msg__Points__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_msgss__msg__Points__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_msgss__msg__Points__Sequence__destroy(my_msgss__msg__Points__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_msgss__msg__Points__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_msgss__msg__Points__Sequence__are_equal(const my_msgss__msg__Points__Sequence * lhs, const my_msgss__msg__Points__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_msgss__msg__Points__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_msgss__msg__Points__Sequence__copy(
  const my_msgss__msg__Points__Sequence * input,
  my_msgss__msg__Points__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_msgss__msg__Points);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_msgss__msg__Points * data =
      (my_msgss__msg__Points *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_msgss__msg__Points__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_msgss__msg__Points__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_msgss__msg__Points__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
