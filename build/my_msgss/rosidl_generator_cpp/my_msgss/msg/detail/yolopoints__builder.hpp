// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgss:msg/Yolopoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINTS__BUILDER_HPP_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINTS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_msgss/msg/detail/yolopoints__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_msgss
{

namespace msg
{

namespace builder
{

class Init_Yolopoints_data
{
public:
  explicit Init_Yolopoints_data(::my_msgss::msg::Yolopoints & msg)
  : msg_(msg)
  {}
  ::my_msgss::msg::Yolopoints data(::my_msgss::msg::Yolopoints::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgss::msg::Yolopoints msg_;
};

class Init_Yolopoints_text
{
public:
  explicit Init_Yolopoints_text(::my_msgss::msg::Yolopoints & msg)
  : msg_(msg)
  {}
  Init_Yolopoints_data text(::my_msgss::msg::Yolopoints::_text_type arg)
  {
    msg_.text = std::move(arg);
    return Init_Yolopoints_data(msg_);
  }

private:
  ::my_msgss::msg::Yolopoints msg_;
};

class Init_Yolopoints_id
{
public:
  Init_Yolopoints_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Yolopoints_text id(::my_msgss::msg::Yolopoints::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Yolopoints_text(msg_);
  }

private:
  ::my_msgss::msg::Yolopoints msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgss::msg::Yolopoints>()
{
  return my_msgss::msg::builder::Init_Yolopoints_id();
}

}  // namespace my_msgss

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINTS__BUILDER_HPP_
