// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgss:msg/Point.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINT__STRUCT_H_
#define MY_MSGSS__MSG__DETAIL__POINT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/Point in the package my_msgss.
typedef struct my_msgss__msg__Point
{
  uint8_t id;
  float x;
  float y;
  float z;
} my_msgss__msg__Point;

// Struct for a sequence of my_msgss__msg__Point.
typedef struct my_msgss__msg__Point__Sequence
{
  my_msgss__msg__Point * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgss__msg__Point__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__POINT__STRUCT_H_
