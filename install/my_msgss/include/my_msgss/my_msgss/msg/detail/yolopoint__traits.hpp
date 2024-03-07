// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from my_msgss:msg/Yolopoint.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGSS__MSG__DETAIL__YOLOPOINT__TRAITS_HPP_
#define MY_MSGSS__MSG__DETAIL__YOLOPOINT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "my_msgss/msg/detail/yolopoint__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace my_msgss
{

namespace msg
{

inline void to_flow_style_yaml(
  const Yolopoint & msg,
  std::ostream & out)
{
  out << "{";
  // member: id
  {
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: width
  {
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << ", ";
  }

  // member: color
  {
    out << "color: ";
    rosidl_generator_traits::value_to_yaml(msg.color, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Yolopoint & msg,
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

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: width
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << "\n";
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }

  // member: color
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "color: ";
    rosidl_generator_traits::value_to_yaml(msg.color, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Yolopoint & msg, bool use_flow_style = false)
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
  const my_msgss::msg::Yolopoint & msg,
  std::ostream & out, size_t indentation = 0)
{
  my_msgss::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use my_msgss::msg::to_yaml() instead")]]
inline std::string to_yaml(const my_msgss::msg::Yolopoint & msg)
{
  return my_msgss::msg::to_yaml(msg);
}

template<>
inline const char * data_type<my_msgss::msg::Yolopoint>()
{
  return "my_msgss::msg::Yolopoint";
}

template<>
inline const char * name<my_msgss::msg::Yolopoint>()
{
  return "my_msgss/msg/Yolopoint";
}

template<>
struct has_fixed_size<my_msgss::msg::Yolopoint>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<my_msgss::msg::Yolopoint>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<my_msgss::msg::Yolopoint>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MY_MSGSS__MSG__DETAIL__YOLOPOINT__TRAITS_HPP_
