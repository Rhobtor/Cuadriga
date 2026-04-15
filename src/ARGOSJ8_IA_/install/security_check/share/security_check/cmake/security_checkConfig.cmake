# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_security_check_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED security_check_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(security_check_FOUND FALSE)
  elseif(NOT security_check_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(security_check_FOUND FALSE)
  endif()
  return()
endif()
set(_security_check_CONFIG_INCLUDED TRUE)

# output package information
if(NOT security_check_FIND_QUIETLY)
  message(STATUS "Found security_check: 0.0.0 (${security_check_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'security_check' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${security_check_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(security_check_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "ament_cmake_export_dependencies-extras.cmake")
foreach(_extra ${_extras})
  include("${security_check_DIR}/${_extra}")
endforeach()
