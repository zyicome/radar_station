// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from my_msgss:msg/Distpoint.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "my_msgss/msg/detail/distpoint__rosidl_typesupport_introspection_c.h"
#include "my_msgss/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "my_msgss/msg/detail/distpoint__functions.h"
#include "my_msgss/msg/detail/distpoint__struct.h"


// Include directives for member types
// Member `color`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  my_msgss__msg__Distpoint__init(message_memory);
}

void my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_fini_function(void * message_memory)
{
  my_msgss__msg__Distpoint__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_member_array[6] = {
  {
    "id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "dist",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, dist),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "lastdist",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, lastdist),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "color",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgss__msg__Distpoint, color),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_members = {
  "my_msgss__msg",  // message namespace
  "Distpoint",  // message name
  6,  // number of fields
  sizeof(my_msgss__msg__Distpoint),
  my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_member_array,  // message members
  my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_init_function,  // function to initialize message memory (memory has to be allocated)
  my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_type_support_handle = {
  0,
  &my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_my_msgss
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, my_msgss, msg, Distpoint)() {
  if (!my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_type_support_handle.typesupport_identifier) {
    my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &my_msgss__msg__Distpoint__rosidl_typesupport_introspection_c__Distpoint_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
