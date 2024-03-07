# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_Img_Handle_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED Img_Handle_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(Img_Handle_FOUND FALSE)
  elseif(NOT Img_Handle_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(Img_Handle_FOUND FALSE)
  endif()
  return()
endif()
set(_Img_Handle_CONFIG_INCLUDED TRUE)

# output package information
if(NOT Img_Handle_FIND_QUIETLY)
  message(STATUS "Found Img_Handle: 0.0.0 (${Img_Handle_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'Img_Handle' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${Img_Handle_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(Img_Handle_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${Img_Handle_DIR}/${_extra}")
endforeach()
