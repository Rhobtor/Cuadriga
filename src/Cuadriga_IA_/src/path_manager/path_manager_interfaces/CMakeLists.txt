cmake_minimum_required(VERSION 3.5)
project(path_manager_interfaces)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# Generate messages and services
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/WritePathToFile.srv"
  "srv/ReadPathFromFile.srv"
  "srv/ReturnRobotPath.srv"
  "srv/RobotPath.srv"
  "srv/GetFixFramePath.srv"
  "srv/GetLLPath.srv"
  "srv/PlanPath.srv"
  "srv/AssistEmergency.srv"
  DEPENDENCIES std_msgs nav_msgs geometry_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
