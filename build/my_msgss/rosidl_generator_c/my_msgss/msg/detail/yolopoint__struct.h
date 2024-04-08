// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgss:msg/Yolopoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINT__STRUCT_H_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINT__STRUCT_H_

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

/// Struct defined in msg/Yolopoint in the package my_msgss.
typedef struct my_msgss__msg__Yolopoint
{
  int8_t id;
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
  float confidence;
  rosidl_runtime_c__String color;
} my_msgss__msg__Yolopoint;

// Struct for a sequence of my_msgss__msg__Yolopoint.
typedef struct my_msgss__msg__Yolopoint__Sequence
{
  my_msgss__msg__Yolopoint * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgss__msg__Yolopoint__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINT__STRUCT_H_
