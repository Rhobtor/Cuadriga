# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_cuadriga_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED cuadriga_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(cuadriga_FOUND FALSE)
  elseif(NOT cuadriga_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(cuadriga_FOUND FALSE)
  endif()
  return()
endif()
set(_cuadriga_CONFIG_INCLUDED TRUE)

# output package information
if(NOT cuadriga_FIND_QUIETLY)
  message(STATUS "Found cuadriga: 0.0.0 (${cuadriga_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'cuadriga' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${cuadriga_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(cuadriga_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${cuadriga_DIR}/${_extra}")
endforeach()
