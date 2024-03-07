// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_msgss:msg/Point.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINT__STRUCT_HPP_
#define MY_MSGSS__MSG__DETAIL__POINT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__my_msgss__msg__Point __attribute__((deprecated))
#else
# define DEPRECATED__my_msgss__msg__Point __declspec(deprecated)
#endif

namespace my_msgss
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Point_
{
  using Type = Point_<ContainerAllocator>;

  explicit Point_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
    }
  }

  explicit Point_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
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
  using _z_type =
    float;
  _z_type z;

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
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_msgss::msg::Point_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_msgss::msg::Point_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_msgss::msg::Point_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_msgss::msg::Point_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Point_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Point_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Point_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Point_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_msgss::msg::Point_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_msgss::msg::Point_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_msgss__msg__Point
    std::shared_ptr<my_msgss::msg::Point_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_msgss__msg__Point
    std::shared_ptr<my_msgss::msg::Point_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Point_ & other) const
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
    if (this->z != other.z) {
      return false;
    }
    return true;
  }
  bool operator!=(const Point_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Point_

// alias to use template instance with default allocator
using Point =
  my_msgss::msg::Point_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__POINT__STRUCT_HPP_
