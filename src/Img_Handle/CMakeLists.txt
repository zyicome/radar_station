cmake_minimum_required(VERSION 3.8)
project(Img_Handle)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
set(my_msgss_DIR /home/zyb/radar_station/src/my_msgss/install/my_msgss/share/my_msgss/cmake)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(my_msgss REQUIRED)
include_directories(include ${my_msgss_INCLUDE_DIRS})
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Img_Pub的生成
add_executable(Img_Pub src/img_pub.cpp)
ament_target_dependencies(Img_Pub rclcpp sensor_msgs)
target_link_directories(Img_Pub PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(Img_Pub ${OpenCV_LIBS})

# Img_Sub的生成
add_executable(Img_Sub src/img_sub.cpp src/inference.cpp)
ament_target_dependencies(Img_Sub rclcpp sensor_msgs cv_bridge my_msgss)
target_link_directories(Img_Sub PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(Img_Sub ${OpenCV_LIBS})

# main的生成
add_executable(main src/main.cpp src/inference.cpp)
ament_target_dependencies(main cv_bridge)
target_link_directories(main PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(main ${OpenCV_LIBS})

install(TARGETS
  Img_Pub
  Img_Sub
  main
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # comment the line when a copyright and license is added to all source files
#   set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # comment the line when this package is in a git repo and when
#   # a copyright and license is added to all source files
#   set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

ament_package()
