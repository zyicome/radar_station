// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_msgss:msg/Points.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_HPP_
#define MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'data'
#include "my_msgss/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__my_msgss__msg__Points __attribute__((deprecated))
#else
# define DEPRECATED__my_msgss__msg__Points __declspec(deprecated)
#endif

namespace my_msgss
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Points_
{
  using Type = Points_<ContainerAllocator>;

  explicit Points_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->color = "";
      this->text = "";
    }
  }

  explicit Points_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : color(_alloc),
    text(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->color = "";
      this->text = "";
    }
  }

  // field types and members
  using _id_type =
    uint8_t;
  _id_type id;
  using _color_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _color_type color;
  using _text_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _text_type text;
  using _data_type =
    std::vector<my_msgss::msg::Point_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<my_msgss::msg::Point_<ContainerAllocator>>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__id(
    const uint8_t & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__color(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->color = _arg;
    return *this;
  }
  Type & set__text(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->text = _arg;
    return *this;
  }
  Type & set__data(
    const std::vector<my_msgss::msg::Point_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<my_msgss::msg::Point_<ContainerAllocator>>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_msgss::msg::Points_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_msgss::msg::Points_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_msgss::msg::Points_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_msgss::msg::Points_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Points_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Points_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_msgss::msg::Points_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_msgss::msg::Points_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_msgss::msg::Points_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_msgss::msg::Points_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_msgss__msg__Points
    std::shared_ptr<my_msgss::msg::Points_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_msgss__msg__Points
    std::shared_ptr<my_msgss::msg::Points_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Points_ & other) const
  {
    if (this->id != other.id) {
      return false;
    }
    if (this->color != other.color) {
      return false;
    }
    if (this->text != other.text) {
      return false;
    }
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const Points_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Points_

// alias to use template instance with default allocator
using Points =
  my_msgss::msg::Points_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__POINTS__STRUCT_HPP_
