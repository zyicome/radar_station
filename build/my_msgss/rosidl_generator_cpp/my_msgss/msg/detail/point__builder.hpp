// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Point.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINT__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__POINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/point__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Point_z
{
public:
  explicit Init_Point_z(::my_msgss::msg::Point & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Point z(::my_msgss::msg::Point::_z_type arg)
  {
    msg_.z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Point msg_;
};

class Init_Point_y
{
public:
  explicit Init_Point_y(::my_msgss::msg::Point & msg)
  : msg_(msg)
  {}
  Init_Point_z y(::my_msgss::msg::Point::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_Point_z(msg_);
  }

private:
  ::my_msgss::msg::Point msg_;
};

class Init_Point_x
{
public:
  explicit Init_Point_x(::my_msgss::msg::Point & msg)
  : msg_(msg)
  {}
  Init_Point_y x(::my_msgss::msg::Point::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Point_y(msg_);
  }

private:
  ::my_msgss::msg::Point msg_;
};

class Init_Point_id
{
public:
  Init_Point_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Point_x id(::my_msgss::msg::Point::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Point_x(msg_);
  }

private:
  ::my_msgss::msg::Point msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Point>()
{
  return my_msgss::msg::builder::Init_Point_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__POINT__BUILDER_HPP_
