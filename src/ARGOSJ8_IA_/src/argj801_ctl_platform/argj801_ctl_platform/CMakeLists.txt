cmake_minimum_required(VERSION 3.5)
  
project(argj801_ctl_platform)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(argj801_ctl_platform_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(self_test REQUIRED)
find_package(diagnostic_updater REQUIRED)
find_package(diagnostic_msgs REQUIRED)
#find_package(ament_index_cpp REQUIRED)

add_subdirectory(src/argj801_lcm)

add_executable(ARGJ801_ctl_platform
  src/main.cpp
  src/argj801_ctl_platform/argj801_ctl_platform_node.cpp
  src/arduino_interface/serial_arduino.cpp
  src/argj801_ctl_platform/argj801_kinematic_model.cpp)

target_include_directories(ARGJ801_ctl_platform
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)


ament_target_dependencies(ARGJ801_ctl_platform
  rclcpp
  lifecycle_msgs
  rclcpp_lifecycle
  lifecycle_msgs
  geometry_msgs
  argj801_ctl_platform_interfaces
  std_srvs
  self_test
  diagnostic_updater
  diagnostic_msgs
)

target_link_libraries(ARGJ801_ctl_platform argj801_lcm)

install(TARGETS
  ARGJ801_ctl_platform
  DESTINATION lib/${PROJECT_NAME})
  
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}/
)
ament_package()
