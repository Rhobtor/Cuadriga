cmake_minimum_required(VERSION 3.8)
project(j8_xacro_model)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(gazebo_dev REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(sensor_msgs REQUIRED )
find_package(argj801_ctl_platform_interfaces REQUIRED )
# gazebo_ros_j8_wheels_speed
add_library(gazebo_ros_j8_wheels_plugin SHARED
  src/gazebo_ros_j8_wheels_speed.cpp
)



target_include_directories(gazebo_ros_j8_wheels_plugin PUBLIC include)
ament_target_dependencies(gazebo_ros_j8_wheels_plugin
  "gazebo_dev"
  "gazebo_ros"
  "geometry_msgs"
  "nav_msgs"
  "rclcpp"
  "tf2"
  "tf2_geometry_msgs"
  "tf2_ros"
  "argj801_ctl_platform_interfaces"
)

target_include_directories(gazebo_ros_j8_wheels_plugin
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

ament_export_libraries(gazebo_ros_j8_wheels_plugin)
if(ENABLE_PROFILER)
  target_include_directories(gazebo_ros_j8_wheels_plugin PUBLIC ${ignition-common3_INCLUDE_DIRS})
  target_link_libraries(gazebo_ros_j8_wheels_plugin ${ignition-common3_LIBRARIES})
endif()

#
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
gazebo_ros_j8_wheels_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)


# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

# Install world files
install(DIRECTORY
  worlds
  DESTINATION share/${PROJECT_NAME}/
)

# Install URDF/Xacro files
install(DIRECTORY
  urdf
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY
  meshes
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()

