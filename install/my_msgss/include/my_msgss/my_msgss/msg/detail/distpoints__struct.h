// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgss:msg/Distpoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINTS__STRUCT_H_
#define MY_MSGSS__MSG__DETAIL__DISTPOINTS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'text'
#include "rosidl_runtime_c/string.h"
// Member 'data'
#include "my_msgss/msg/detail/distpoint__struct.h"

/// Struct defined in msg/Distpoints in the package my_msgss.
typedef struct my_msgss__msg__Distpoints
{
  uint8_t id;
  rosidl_runtime_c__String text;
  my_msgss__msg__Distpoint__Sequence data;
} my_msgss__msg__Distpoints;

// Struct for a sequence of my_msgss__msg__Distpoints.
typedef struct my_msgss__msg__Distpoints__Sequence
{
  my_msgss__msg__Distpoints * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgss__msg__Distpoints__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINTS__STRUCT_H_
