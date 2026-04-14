cmake_minimum_required(VERSION 3.5)
  
project(argj801_lcm)

find_package(lcm REQUIRED) 
find_package(yaml-cpp REQUIRED) 

add_library(${PROJECT_NAME} STATIC src/LCM_interface.cpp)

ament_target_dependencies(${PROJECT_NAME}
  lcm
  yaml-cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} lcm #link lcm library
                                      yaml-cpp) #link yaml-cpp library

install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}/
)