// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_msgss:msg/Distpoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_HPP_
#define MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__my_msgss__msg__Distpoint __attribute__((deprecated))
#else
# define DEPRECATED__my_msgss__msg__Distpoint __declspec(deprecated)
#endif

namespace my_msgss
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Distpoint_
{
  using Type = Distpoint_<ContainerAllocator>;

  explicit Distpoint_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->dist = 0.0f;
      this->lastdist = 0.0f;
      this->color = "";
    }
  }

  explicit Distpoint_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : color(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->dist = 0.0f;
      this->lastdist = 0.0f;
      this->color = "";
    }
  }

  // field types and members
  using _id_type =
    uint8_t;
  _id_type id;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _dist_type =
    float;
  _dist_type dist;
  using _lastdist_type =
    float;
  _lastdist_type lastdist;
  using _color_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _color_type color;

  // setters for named parameter idiom
  Type & set__id(
    const uint8_t & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__dist(
    const float & _arg)
  {
    this->dist = _arg;
    return *this;
  }
  Type & set__lastdist(
    const float & _arg)
  {
    this->lastdist = _arg;
    return *this;
  }
  Type & set__color(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->color = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_msgss::msg::Distpoint_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_msgss::msg::Distpoint_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_msgss::msg::Distpoint_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_msgss::msg::Distpoint_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Distpoint_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Distpoint_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Distpoint_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Distpoint_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_msgss::msg::Distpoint_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_msgss::msg::Distpoint_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_msgss__msg__Distpoint
    std::shared_ptr<my_msgss::msg::Distpoint_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_msgss__msg__Distpoint
    std::shared_ptr<my_msgss::msg::Distpoint_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Distpoint_ & other) const
  {
    if (this->id != other.id) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->dist != other.dist) {
      return false;
    }
    if (this->lastdist != other.lastdist) {
      return false;
    }
    if (this->color != other.color) {
      return false;
    }
    return true;
  }
  bool operator!=(const Distpoint_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Distpoint_

// alias to use template instance with default allocator
using Distpoint =
  my_msgss::msg::Distpoint_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINT__STRUCT_HPP_
