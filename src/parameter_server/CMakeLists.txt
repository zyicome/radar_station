cmake_minimum_required(VERSION 3.8)
project(parameter_server)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(std_msgs REQUIRED)
find_package(my_msgss REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(include_directories(${CMAKE_SOURCE_DIR}/include))
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib/libtinyxml2.so.10.0.0)

add_executable(parameter_server src/parameter_server.cpp)
ament_target_dependencies(parameter_server rclcpp sensor_msgs OpenCV my_msgss std_msgs)
target_link_directories(parameter_server PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(parameter_server ${OpenCV_LIBS})
target_include_directories(parameter_server PUBLIC
      ${INC_DIR})
target_link_libraries(parameter_server ${LINK_DIR})


install(TARGETS
  parameter_server
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
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
