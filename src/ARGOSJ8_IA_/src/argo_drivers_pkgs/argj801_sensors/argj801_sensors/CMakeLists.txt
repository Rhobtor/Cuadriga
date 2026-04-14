cmake_minimum_required(VERSION 3.8)
project(argj801_sensors)

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(argj801_sensors_msgs REQUIRED)
find_package(diagnostic_updater REQUIRED)
find_package(diagnostic_msgs REQUIRED)
#find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(src/argj801_lcm)

add_executable(ARGJ801_sensors_node
    src/main.cpp
    src/argj801_sensors/argj801_sensors_node.cpp
    src/argj801_sensors/argj801_lcm_sensors_builder.cpp
    src/argj801_sensors/argj801_camera_builder.cpp
    src/argj801_sensors/argj801_sensor_interface.cpp
    src/argj801_sensors/argj801_camera_interface.cpp
    src/data/LidarData.cpp
    src/data/Composite.cpp
    src/data/MotorTelemetry.cpp
    src/data/OdometerData.cpp
    src/data/Twist.cpp
    src/data/CompressedImage.cpp
    src/visitor/SensorDataVisitor.cpp)

target_include_directories(ARGJ801_sensors_node
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

ament_target_dependencies(ARGJ801_sensors_node
    rclcpp
    rclcpp_lifecycle
    lifecycle_msgs
    sensor_msgs
    geometry_msgs
    argj801_sensors_msgs
    diagnostic_msgs
    diagnostic_updater
#    image_transport
    cv_bridge
    OpenCV
)

target_link_libraries(ARGJ801_sensors_node argj801_lcm)

install(TARGETS
  ARGJ801_sensors_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

## Install params files.
install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY
  include
  DESTINATION include/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
