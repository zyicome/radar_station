// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_msgss:msg/Yolopoint.idl
// generated code does not contain a copyright notice
#include "my_msgss/msg/detail/yolopoint__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `color`
#include "rosidl_runtime_c/string_functions.h"

bool
my_msgss__msg__Yolopoint__init(my_msgss__msg__Yolopoint * msg)
{
  if (!msg) {
    return false;
  }
  // id
  // x
  // y
  // width
  // height
  // color
  if (!rosidl_runtime_c__String__init(&msg->color)) {
    my_msgss__msg__Yolopoint__fini(msg);
    return false;
  }
  return true;
}

void
my_msgss__msg__Yolopoint__fini(my_msgss__msg__Yolopoint * msg)
{
  if (!msg) {
    return;
  }
  // id
  // x
  // y
  // width
  // height
  // color
  rosidl_runtime_c__String__fini(&msg->color);
}

bool
my_msgss__msg__Yolopoint__are_equal(const my_msgss__msg__Yolopoint * lhs, const my_msgss__msg__Yolopoint * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // id
  if (lhs->id != rhs->id) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // width
  if (lhs->width != rhs->width) {
    return false;
  }
  // height
  if (lhs->height != rhs->height) {
    return false;
  }
  // color
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->color), &(rhs->color)))
  {
    return false;
  }
  return true;
}

bool
my_msgss__msg__Yolopoint__copy(
  const my_msgss__msg__Yolopoint * input,
  my_msgss__msg__Yolopoint * output)
{
  if (!input || !output) {
    return false;
  }
  // id
  output->id = input->id;
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // width
  output->width = input->width;
  // height
  output->height = input->height;
  // color
  if (!rosidl_runtime_c__String__copy(
      &(input->color), &(output->color)))
  {
    return false;
  }
  return true;
}

my_msgss__msg__Yolopoint *
my_msgss__msg__Yolopoint__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoint * msg = (my_msgss__msg__Yolopoint *)allocator.allocate(sizeof(my_msgss__msg__Yolopoint), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_msgss__msg__Yolopoint));
  bool success = my_msgss__msg__Yolopoint__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_msgss__msg__Yolopoint__destroy(my_msgss__msg__Yolopoint * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_msgss__msg__Yolopoint__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_msgss__msg__Yolopoint__Sequence__init(my_msgss__msg__Yolopoint__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoint * data = NULL;

  if (size) {
    data = (my_msgss__msg__Yolopoint *)allocator.zero_allocate(size, sizeof(my_msgss__msg__Yolopoint), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_msgss__msg__Yolopoint__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_msgss__msg__Yolopoint__fini(&data[i - 1]);
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
my_msgss__msg__Yolopoint__Sequence__fini(my_msgss__msg__Yolopoint__Sequence * array)
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
      my_msgss__msg__Yolopoint__fini(&array->data[i]);
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

my_msgss__msg__Yolopoint__Sequence *
my_msgss__msg__Yolopoint__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_msgss__msg__Yolopoint__Sequence * array = (my_msgss__msg__Yolopoint__Sequence *)allocator.allocate(sizeof(my_msgss__msg__Yolopoint__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_msgss__msg__Yolopoint__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_msgss__msg__Yolopoint__Sequence__destroy(my_msgss__msg__Yolopoint__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_msgss__msg__Yolopoint__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_msgss__msg__Yolopoint__Sequence__are_equal(const my_msgss__msg__Yolopoint__Sequence * lhs, const my_msgss__msg__Yolopoint__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_msgss__msg__Yolopoint__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_msgss__msg__Yolopoint__Sequence__copy(
  const my_msgss__msg__Yolopoint__Sequence * input,
  my_msgss__msg__Yolopoint__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_msgss__msg__Yolopoint);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_msgss__msg__Yolopoint * data =
      (my_msgss__msg__Yolopoint *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_msgss__msg__Yolopoint__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_msgss__msg__Yolopoint__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_msgss__msg__Yolopoint__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
