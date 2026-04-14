cmake_minimum_required(VERSION 3.8)
project(argj801_setup)

# Este paquete es principalmente un "package de lanzamiento":
# - No compila nodos propios.
# - Instala ficheros de `launch/` y `config/` en share/<package>.
# Su objetivo es orquestar el arranque del stack del rover (ver `launch/J8_launch.py`).

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

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

install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}/)

install(PROGRAMS
  scripts/cuadriga_mock_backend.py
  DESTINATION lib/${PROJECT_NAME})

# No hay targets/ejecutables que instalar: sólo recursos.

ament_package()
