cmake_minimum_required(VERSION 3.5)
project(path_manager)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/share/cmake/geographiclib")

# Find required packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(GeographicLib REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(path_manager_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)

ament_export_dependencies(rosidl_default_runtime)

# Add executable
add_executable(path_manager_node
  src/path_manager.cpp
  src/Demaias_planner/DEMAIAS.cpp)

target_include_directories(path_manager_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp)

ament_target_dependencies(path_manager_node
  "lifecycle_msgs"
  "rclcpp_lifecycle"
  "std_msgs"
  "nav_msgs"
  "GeographicLib"
  "std_srvs"
  "geometry_msgs"
  "path_manager_interfaces"
  "rosidl_default_runtime")

install(TARGETS
  path_manager_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY paths
  DESTINATION share/${PROJECT_NAME})

install(DIRECTORY Demaias_planner_missions
  DESTINATION share/${PROJECT_NAME})

ament_package()
