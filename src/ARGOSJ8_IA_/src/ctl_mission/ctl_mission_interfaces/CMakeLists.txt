cmake_minimum_required(VERSION 3.5)
project(ctl_mission_interfaces)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

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
  "srv/ChangeMode.srv"
  "srv/GetMode.srv"
  "srv/ChangeSpeed.srv"
  "srv/ConfigSimpleCtl.srv"
  "srv/GetPossibleTransitions.srv"
  "srv/ChangeController.srv"
  "srv/ConfigPurePursuitCtrl.srv"
  "srv/ConfigStanleyCtrl.srv"
  "srv/ConfigDynamicPureCtrl.srv"
  "srv/ConfigDynamicLAPureCtrl.srv"
  "srv/ConfigRegulatedPureCtrl.srv"
  DEPENDENCIES std_msgs nav_msgs geometry_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
