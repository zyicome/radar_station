// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Points.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__POINTS__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__POINTS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/points__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Points_data
{
public:
  explicit Init_Points_data(::my_msgss::msg::Points & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Points data(::my_msgss::msg::Points::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Points msg_;
};

class Init_Points_text
{
public:
  explicit Init_Points_text(::my_msgss::msg::Points & msg)
  : msg_(msg)
  {}
  Init_Points_data text(::my_msgss::msg::Points::_text_type arg)
  {
    msg_.text = std::move(arg);
    return Init_Points_data(msg_);
  }

private:
  ::my_msgss::msg::Points msg_;
};

class Init_Points_color
{
public:
  explicit Init_Points_color(::my_msgss::msg::Points & msg)
  : msg_(msg)
  {}
  Init_Points_text color(::my_msgss::msg::Points::_color_type arg)
  {
    msg_.color = std::move(arg);
    return Init_Points_text(msg_);
  }

private:
  ::my_msgss::msg::Points msg_;
};

class Init_Points_id
{
public:
  Init_Points_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Points_color id(::my_msgss::msg::Points::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Points_color(msg_);
  }

private:
  ::my_msgss::msg::Points msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Points>()
{
  return my_msgss::msg::builder::Init_Points_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__POINTS__BUILDER_HPP_
