// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Distpoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__DISTPOINTS__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__DISTPOINTS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/distpoints__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Distpoints_data
{
public:
  explicit Init_Distpoints_data(::my_msgss::msg::Distpoints & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Distpoints data(::my_msgss::msg::Distpoints::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Distpoints msg_;
};

class Init_Distpoints_text
{
public:
  explicit Init_Distpoints_text(::my_msgss::msg::Distpoints & msg)
  : msg_(msg)
  {}
  Init_Distpoints_data text(::my_msgss::msg::Distpoints::_text_type arg)
  {
    msg_.text = std::move(arg);
    return Init_Distpoints_data(msg_);
  }

private:
  ::my_msgss::msg::Distpoints msg_;
};

class Init_Distpoints_id
{
public:
  Init_Distpoints_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Distpoints_text id(::my_msgss::msg::Distpoints::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Distpoints_text(msg_);
  }

private:
  ::my_msgss::msg::Distpoints msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Distpoints>()
{
  return my_msgss::msg::builder::Init_Distpoints_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__DISTPOINTS__BUILDER_HPP_
