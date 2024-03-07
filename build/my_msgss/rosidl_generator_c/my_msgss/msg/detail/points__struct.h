// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgss:msg/Points.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_H_
#define MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'color'
// Member 'text'
#include "rosidl_runtime_c/string.h"
// Member 'data'
#include "my_msgss/msg/detail/point__struct.h"

/// Struct defined in msg/Points in the package my_msgss.
typedef struct my_msgss__msg__Points
{
  uint8_t id;
  rosidl_runtime_c__String color;
  rosidl_runtime_c__String text;
  my_msgss__msg__Point__Sequence data;
} my_msgss__msg__Points;

// Struct for a sequence of my_msgss__msg__Points.
typedef struct my_msgss__msg__Points__Sequence
{
  my_msgss__msg__Points * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgss__msg__Points__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_H_
