cmake_minimum_required(VERSION 3.8)
project(Video_save)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
# set(my_msgss_DIR /home/zyb/radar_station/src/my_msgss/install/my_msgss/share/my_msgss/cmake)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(std_msgs REQUIRED)
# find_package(my_msgss REQUIRED)
find_package(Eigen3 REQUIRED)
# include_directories(include ${my_msgss_INCLUDE_DIRS})
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("/usr/include/eigen3")

add_executable(video_save src/video_save.cpp)
ament_target_dependencies(video_save rclcpp sensor_msgs OpenCV cv_bridge image_transport Eigen3 std_msgs)
target_link_directories(video_save PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(video_save ${OpenCV_LIBS})

install(TARGETS
  video_save
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  video_far_
  video_close_
  DESTINATION share/${PROJECT_NAME}
)

# install(DIRECTORY
#   launch
#   Yolov8_weight
#   new_model
#   DESTINATION share/${PROJECT_NAME}
# )

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
