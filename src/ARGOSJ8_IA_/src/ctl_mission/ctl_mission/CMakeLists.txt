cmake_minimum_required(VERSION 3.5)
project(ctl_mission)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# Compiler options
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
find_package(sensor_msgs REQUIRED)
find_package(Boost REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ctl_mission_interfaces REQUIRED)
find_package(path_manager_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(GeographicLib REQUIRED)

# Executable: ctl_mission
add_executable(ctl_mission
  src/cuadriga_FSM.cpp
  src/argj08_ctl_node.cpp)

target_include_directories(ctl_mission
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

ament_target_dependencies(ctl_mission
  "lifecycle_msgs"
  "rclcpp_lifecycle"
  "std_msgs"
  "std_srvs"
  "sensor_msgs"
  "Boost"
  "tf2"
  "nav_msgs"
  "ctl_mission_interfaces"
  "tf2_geometry_msgs"
  "tf2_ros"
  "OpenCV")

# Other Executables
add_executable(path_following 
  src/FSM_states/path_following_node.cpp
  src/FSM_states/path_following_node_main.cpp) 
add_executable(path_record_node 
src/FSM_states/path_record_node.cpp
src/FSM_states/path_record_node_main.cpp)
add_executable(estop_node 
src/FSM_states/estop_node.cpp
src/FSM_states/estop_node_main.cpp)
add_executable(back_home_node src/FSM_states/back_home_node.cpp)
add_executable(controller_node
  src/FSM_states/Ctrl_node.cpp
  src/FSM_states/Ctrl_node_main.cpp
  src/controllers/stanley_ctrl.cpp
  src/controllers/pure_pursuit_controller.cpp
  src/controllers/dynamic_pp_controller.cpp
  src/controllers/dynamic_la_pp_controller.cpp
  src/controllers/regulated_pure_pursuit_controller.cpp)



# Include directories for stanley_ctrl_node
target_include_directories(controller_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_include_directories(path_record_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# Executable for Teleoperation Node
add_executable(teleoperation_node
  src/FSM_states/teleoperation_node.cpp
  src/FSM_states/teleoperation_node_main.cpp
)
target_include_directories(teleoperation_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies( teleoperation_node
"lifecycle_msgs"
"rclcpp_lifecycle"
"std_msgs"
"std_srvs"
"sensor_msgs"
"Boost"
"tf2"
"nav_msgs"
"tf2_geometry_msgs"
"tf2_ros"
"ctl_mission_interfaces"
"nav_msgs"
"tf2_geometry_msgs"
"trajectory_msgs"
"geometry_msgs"
"GeographicLib")

# Executable for Ready Node
add_executable(ready_node
  src/FSM_states/ready_node.cpp
  src/FSM_states/ready_node_main.cpp
)
target_include_directories(ready_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies( ready_node
"lifecycle_msgs"
"rclcpp_lifecycle"
"std_msgs"
"std_srvs"
"sensor_msgs"
"Boost"
"tf2"
"nav_msgs"
"tf2_geometry_msgs"
"tf2_ros"
"ctl_mission_interfaces"
"nav_msgs"
"tf2_geometry_msgs"
"trajectory_msgs"
"geometry_msgs"
"GeographicLib")


# Executable for Follow ZED Node
add_executable(follow_zed_node
  src/FSM_states/follow_zed_node.cpp
  src/FSM_states/follow_zed_main.cpp
)

target_include_directories(follow_zed_node
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(follow_zed_node
  "rclcpp"
  "rclcpp_lifecycle"
  "lifecycle_msgs"
  "geometry_msgs"
  "sensor_msgs"
  "std_msgs"
  "std_srvs"
)


# Executable for MPPI/SAC relay node (AI bridge)
add_executable(mppi_sac_relay_node
  src/FSM_states/mppi_sac_relay_node.cpp
  src/FSM_states/mppi_sac_relay_node_main.cpp
)






# Target dependencies for other executables
set(OTHER_EXECUTABLES   path_following  path_record_node controller_node ready_node follow_zed_node estop_node back_home_node mppi_sac_relay_node)

foreach(EXECUTABLE IN LISTS OTHER_EXECUTABLES)
target_include_directories(${EXECUTABLE}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
endforeach()

foreach(EXECUTABLE ${OTHER_EXECUTABLES})
  ament_target_dependencies(${EXECUTABLE}
    "lifecycle_msgs"
    "rclcpp_lifecycle"
    "std_msgs"
    "std_srvs"
    "sensor_msgs"
    "Boost"
    "tf2"
    "nav_msgs"
    "tf2_geometry_msgs"
    "tf2_ros"
    "ctl_mission_interfaces"
    "nav_msgs"
    "tf2_geometry_msgs"
    "trajectory_msgs"
    "geometry_msgs"
    "GeographicLib"
    "path_manager_interfaces")
endforeach()


# Install targets
install(TARGETS
  ctl_mission
  path_following
  teleoperation_node
  path_record_node
  controller_node
  ready_node
  estop_node
  back_home_node
  follow_zed_node
  mppi_sac_relay_node
  DESTINATION lib/${PROJECT_NAME})

  install(DIRECTORY include/
  DESTINATION include
)


# Install Python modules
ament_python_install_package(${PROJECT_NAME})
# Install Python executables
install(PROGRAMS
  scripts/planner_node.py
  scripts/mpc_node.py
  DESTINATION lib/${PROJECT_NAME}
)


# Testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  find_package(ros_testing REQUIRED)
  add_ros_test(
    test/test_lifecycle.py
    TIMEOUT 60
  )
endif()
ament_export_include_directories(include)
ament_package()
