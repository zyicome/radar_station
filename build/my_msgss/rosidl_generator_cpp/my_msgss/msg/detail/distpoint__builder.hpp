// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Distpoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINT__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__DISTPOINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/distpoint__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Distpoint_color
{
public:
  explicit Init_Distpoint_color(::my_msgss::msg::Distpoint & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Distpoint color(::my_msgss::msg::Distpoint::_color_type arg)
  {
    msg_.color = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

class Init_Distpoint_lastdist
{
public:
  explicit Init_Distpoint_lastdist(::my_msgss::msg::Distpoint & msg)
  : msg_(msg)
  {}
  Init_Distpoint_color lastdist(::my_msgss::msg::Distpoint::_lastdist_type arg)
  {
    msg_.lastdist = std::move(arg);
    return Init_Distpoint_color(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

class Init_Distpoint_dist
{
public:
  explicit Init_Distpoint_dist(::my_msgss::msg::Distpoint & msg)
  : msg_(msg)
  {}
  Init_Distpoint_lastdist dist(::my_msgss::msg::Distpoint::_dist_type arg)
  {
    msg_.dist = std::move(arg);
    return Init_Distpoint_lastdist(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

class Init_Distpoint_y
{
public:
  explicit Init_Distpoint_y(::my_msgss::msg::Distpoint & msg)
  : msg_(msg)
  {}
  Init_Distpoint_dist y(::my_msgss::msg::Distpoint::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_Distpoint_dist(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

class Init_Distpoint_x
{
public:
  explicit Init_Distpoint_x(::my_msgss::msg::Distpoint & msg)
  : msg_(msg)
  {}
  Init_Distpoint_y x(::my_msgss::msg::Distpoint::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Distpoint_y(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

class Init_Distpoint_id
{
public:
  Init_Distpoint_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Distpoint_x id(::my_msgss::msg::Distpoint::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Distpoint_x(msg_);
  }

private:
  ::my_msgss::msg::Distpoint msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Distpoint>()
{
  return my_msgss::msg::builder::Init_Distpoint_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINT__BUILDER_HPP_
