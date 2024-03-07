// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Yolopoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINT__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/yolopoint__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Yolopoint_color
{
public:
  explicit Init_Yolopoint_color(::my_msgss::msg::Yolopoint & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Yolopoint color(::my_msgss::msg::Yolopoint::_color_type arg)
  {
    msg_.color = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

class Init_Yolopoint_height
{
public:
  explicit Init_Yolopoint_height(::my_msgss::msg::Yolopoint & msg)
  : msg_(msg)
  {}
  Init_Yolopoint_color height(::my_msgss::msg::Yolopoint::_height_type arg)
  {
    msg_.height = std::move(arg);
    return Init_Yolopoint_color(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

class Init_Yolopoint_width
{
public:
  explicit Init_Yolopoint_width(::my_msgss::msg::Yolopoint & msg)
  : msg_(msg)
  {}
  Init_Yolopoint_height width(::my_msgss::msg::Yolopoint::_width_type arg)
  {
    msg_.width = std::move(arg);
    return Init_Yolopoint_height(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

class Init_Yolopoint_y
{
public:
  explicit Init_Yolopoint_y(::my_msgss::msg::Yolopoint & msg)
  : msg_(msg)
  {}
  Init_Yolopoint_width y(::my_msgss::msg::Yolopoint::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_Yolopoint_width(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

class Init_Yolopoint_x
{
public:
  explicit Init_Yolopoint_x(::my_msgss::msg::Yolopoint & msg)
  : msg_(msg)
  {}
  Init_Yolopoint_y x(::my_msgss::msg::Yolopoint::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Yolopoint_y(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

class Init_Yolopoint_id
{
public:
  Init_Yolopoint_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Yolopoint_x id(::my_msgss::msg::Yolopoint::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Yolopoint_x(msg_);
  }

private:
  ::my_msgss::msg::Yolopoint msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Yolopoint>()
{
  return my_msgss::msg::builder::Init_Yolopoint_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINT__BUILDER_HPP_
