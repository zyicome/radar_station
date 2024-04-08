// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from my_msgss:msg/Yolopoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINTS__FUNCTIONS_H_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINTS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "my_msgss/msg/rosidl_generator_c__visibility_control.h"

#include "my_msgss/msg/detail/yolopoints__struct.h"

/// Initialize msg/Yolopoints message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * my_msgss__msg__Yolopoints
 * )) before or use
 * my_msgss__msg__Yolopoints__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Yolopoints__init(my_msgss__msg__Yolopoints * msg);

/// Finalize msg/Yolopoints message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Yolopoints__fini(my_msgss__msg__Yolopoints * msg);

/// Create msg/Yolopoints message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * my_msgss__msg__Yolopoints__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Yolopoints *
my_msgss__msg__Yolopoints__create();

/// Destroy msg/Yolopoints message.
/**
 * It calls
 * my_msgss__msg__Yolopoints__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Yolopoints__destroy(my_msgss__msg__Yolopoints * msg);

/// Check for msg/Yolopoints message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Yolopoints__are_equal(const my_msgss__msg__Yolopoints * lhs, const my_msgss__msg__Yolopoints * rhs);

/// Copy a msg/Yolopoints message.
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
my_msgss__msg__Yolopoints__copy(
  const my_msgss__msg__Yolopoints * input,
  my_msgss__msg__Yolopoints * output);

/// Initialize array of msg/Yolopoints messages.
/**
 * It allocates the memory for the number of elements and calls
 * my_msgss__msg__Yolopoints__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Yolopoints__Sequence__init(my_msgss__msg__Yolopoints__Sequence * array, size_t size);

/// Finalize array of msg/Yolopoints messages.
/**
 * It calls
 * my_msgss__msg__Yolopoints__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Yolopoints__Sequence__fini(my_msgss__msg__Yolopoints__Sequence * array);

/// Create array of msg/Yolopoints messages.
/**
 * It allocates the memory for the array and calls
 * my_msgss__msg__Yolopoints__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
my_msgss__msg__Yolopoints__Sequence *
my_msgss__msg__Yolopoints__Sequence__create(size_t size);

/// Destroy array of msg/Yolopoints messages.
/**
 * It calls
 * my_msgss__msg__Yolopoints__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
void
my_msgss__msg__Yolopoints__Sequence__destroy(my_msgss__msg__Yolopoints__Sequence * array);

/// Check for msg/Yolopoints message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_my_msgss
bool
my_msgss__msg__Yolopoints__Sequence__are_equal(const my_msgss__msg__Yolopoints__Sequence * lhs, const my_msgss__msg__Yolopoints__Sequence * rhs);

/// Copy an array of msg/Yolopoints messages.
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
my_msgss__msg__Yolopoints__Sequence__copy(
  const my_msgss__msg__Yolopoints__Sequence * input,
  my_msgss__msg__Yolopoints__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINTS__FUNCTIONS_H_
