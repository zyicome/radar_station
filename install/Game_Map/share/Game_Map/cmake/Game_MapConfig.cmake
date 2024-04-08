# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_Game_Map_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED Game_Map_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(Game_Map_FOUND FALSE)
  elseif(NOT Game_Map_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(Game_Map_FOUND FALSE)
  endif()
  return()
endif()
set(_Game_Map_CONFIG_INCLUDED TRUE)

# output package information
if(NOT Game_Map_FIND_QUIETLY)
  message(STATUS "Found Game_Map: 0.0.0 (${Game_Map_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'Game_Map' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${Game_Map_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(Game_Map_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${Game_Map_DIR}/${_extra}")
endforeach()
