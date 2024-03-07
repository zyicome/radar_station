// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from my_msgss:msg/Distpoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINTS__FUNCTIONS_H_
#define MY_MSGSS__MSG__DETAIL__DISTPOINTS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "my_msgss/msg/rosidl_generator_c__visibility_control.h"

#include "my_msgss/msg/detail/distpoints__struct.h"

/// Initialize msg/Distpoints message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * my_msgss__msg__Distpoints
 * )) before or use
 * my_msgss__msg__Distpoints__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Distpoints__init(my_msgss__msg__Distpoints * msg);

/// Finalize msg/Distpoints message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Distpoints__fini(my_msgss__msg__Distpoints * msg);

/// Create msg/Distpoints message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * my_msgss__msg__Distpoints__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Distpoints *
my_msgss__msg__Distpoints__create();

/// Destroy msg/Distpoints message.
/**
 * It calls
 * my_msgss__msg__Distpoints__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Distpoints__destroy(my_msgss__msg__Distpoints * msg);

/// Check for msg/Distpoints message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Distpoints__are_equal(const my_msgss__msg__Distpoints * lhs, const my_msgss__msg__Distpoints * rhs);

/// Copy a msg/Distpoints message.
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
my_msgss__msg__Distpoints__copy(
  const my_msgss__msg__Distpoints * input,
  my_msgss__msg__Distpoints * output);

/// Initialize array of msg/Distpoints messages.
/**
 * It allocates the memory for the number of elements and calls
 * my_msgss__msg__Distpoints__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Distpoints__Sequence__init(my_msgss__msg__Distpoints__Sequence * array, size_t size);

/// Finalize array of msg/Distpoints messages.
/**
 * It calls
 * my_msgss__msg__Distpoints__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Distpoints__Sequence__fini(my_msgss__msg__Distpoints__Sequence * array);

/// Create array of msg/Distpoints messages.
/**
 * It allocates the memory for the array and calls
 * my_msgss__msg__Distpoints__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Distpoints__Sequence *
my_msgss__msg__Distpoints__Sequence__create(size_t size);

/// Destroy array of msg/Distpoints messages.
/**
 * It calls
 * my_msgss__msg__Distpoints__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Distpoints__Sequence__destroy(my_msgss__msg__Distpoints__Sequence * array);

/// Check for msg/Distpoints message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Distpoints__Sequence__are_equal(const my_msgss__msg__Distpoints__Sequence * lhs, const my_msgss__msg__Distpoints__Sequence * rhs);

/// Copy an array of msg/Distpoints messages.
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
my_msgss__msg__Distpoints__Sequence__copy(
  const my_msgss__msg__Distpoints__Sequence * input,
  my_msgss__msg__Distpoints__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINTS__FUNCTIONS_H_
