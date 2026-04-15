cmake_minimum_required(VERSION 3.8)
project(security_check)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(ctl_mission_interfaces REQUIRED)
find_package(ctl_mission REQUIRED)
find_package(security_check_interfaces REQUIRED)

ament_export_dependencies(rosidl_default_runtime)

# Define executable
add_executable(security_check_node
  src/security_check_node.cpp
  src/security_check_node_main.cpp)

target_include_directories(security_check_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp
    ${ctl_mission_INCLUDE_DIRS})

ament_target_dependencies(security_check_node
  "lifecycle_msgs"
  "rclcpp_lifecycle"
  "std_msgs"
  "geometry_msgs"
  "ctl_mission_interfaces"
  "ctl_mission"
  "sensor_msgs"
  "tf2_geometry_msgs"
  "tf2_ros"
  "std_srvs"
  "security_check_interfaces"
  "rosidl_default_runtime")

install(TARGETS
  security_check_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

ament_package()
