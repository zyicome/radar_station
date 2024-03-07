// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgss:msg/Distpoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_H_
#define MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_H_

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
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/Distpoint in the package my_msgss.
typedef struct my_msgss__msg__Distpoint
{
  uint8_t id;
  float x;
  float y;
  float dist;
  float lastdist;
  rosidl_runtime_c__String color;
} my_msgss__msg__Distpoint;

// Struct for a sequence of my_msgss__msg__Distpoint.
typedef struct my_msgss__msg__Distpoint__Sequence
{
  my_msgss__msg__Distpoint * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgss__msg__Distpoint__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_H_
