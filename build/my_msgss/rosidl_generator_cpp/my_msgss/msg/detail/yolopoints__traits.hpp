// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from my_msgss:msg/Yolopoints.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINTS__TRAITS_HPP_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINTS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "my_msgss/msg/detail/yolopoints__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'data'
#include "my_msgss/msg/detail/yolopoint__traits.hpp"

namespace my_msgss
{

namespace msg
{

inline void to_flow_style_yaml(
  const Yolopoints & msg,
  std::ostream & out)
{
  out << "{";
  // member: id
  {
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << ", ";
  }

  // member: text
  {
    out << "text: ";
    rosidl_generator_traits::value_to_yaml(msg.text, out);
    out << ", ";
  }

  // member: data
  {
    if (msg.data.size() == 0) {
      out << "data: []";
    } else {
      out << "data: [";
      size_t pending_items = msg.data.size();
      for (auto item : msg.data) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Yolopoints & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << "\n";
  }

  // member: text
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "text: ";
    rosidl_generator_traits::value_to_yaml(msg.text, out);
    out << "\n";
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.data.size() == 0) {
      out << "data: []\n";
    } else {
      out << "data:\n";
      for (auto item : msg.data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Yolopoints & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace my_msgss

namespace rosidl_generator_traits
{

[[deprecated("use my_msgss::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const my_msgss::msg::Yolopoints & msg,
  std::ostream & out, size_t indentation = 0)
{
  my_msgss::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use my_msgss::msg::to_yaml() instead")]]
inline std::string to_yaml(const my_msgss::msg::Yolopoints & msg)
{
  return my_msgss::msg::to_yaml(msg);
}

template<>
inline const char * data_type<my_msgss::msg::Yolopoints>()
{
  return "my_msgss::msg::Yolopoints";
}

template<>
inline const char * name<my_msgss::msg::Yolopoints>()
{
  return "my_msgss/msg/Yolopoints";
}

template<>
struct has_fixed_size<my_msgss::msg::Yolopoints>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<my_msgss::msg::Yolopoints>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<my_msgss::msg::Yolopoints>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINTS__TRAITS_HPP_
