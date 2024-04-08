// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from my_msgss:msg/Points.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINTS__FUNCTIONS_H_
#define MY_MSGSS__MSG__DETAIL__POINTS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "my_msgss/msg/rosidl_generator_c__visibility_control.h"

#include "my_msgss/msg/detail/points__struct.h"

/// Initialize msg/Points message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * my_msgss__msg__Points
 * )) before or use
 * my_msgss__msg__Points__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__init(my_msgss__msg__Points * msg);

/// Finalize msg/Points message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Points__fini(my_msgss__msg__Points * msg);

/// Create msg/Points message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * my_msgss__msg__Points__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Points *
my_msgss__msg__Points__create();

/// Destroy msg/Points message.
/**
 * It calls
 * my_msgss__msg__Points__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Points__destroy(my_msgss__msg__Points * msg);

/// Check for msg/Points message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__are_equal(const my_msgss__msg__Points * lhs, const my_msgss__msg__Points * rhs);

/// Copy a msg/Points message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__copy(
  const my_msgss__msg__Points * input,
  my_msgss__msg__Points * output);

/// Initialize array of msg/Points messages.
/**
 * It allocates the memory for the number of elements and calls
 * my_msgss__msg__Points__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__Sequence__init(my_msgss__msg__Points__Sequence * array, size_t size);

/// Finalize array of msg/Points messages.
/**
 * It calls
 * my_msgss__msg__Points__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Points__Sequence__fini(my_msgss__msg__Points__Sequence * array);

/// Create array of msg/Points messages.
/**
 * It allocates the memory for the array and calls
 * my_msgss__msg__Points__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Points__Sequence *
my_msgss__msg__Points__Sequence__create(size_t size);

/// Destroy array of msg/Points messages.
/**
 * It calls
 * my_msgss__msg__Points__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Points__Sequence__destroy(my_msgss__msg__Points__Sequence * array);

/// Check for msg/Points message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__Sequence__are_equal(const my_msgss__msg__Points__Sequence * lhs, const my_msgss__msg__Points__Sequence * rhs);

/// Copy an array of msg/Points messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Points__Sequence__copy(
  const my_msgss__msg__Points__Sequence * input,
  my_msgss__msg__Points__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__POINTS__FUNCTIONS_H_
