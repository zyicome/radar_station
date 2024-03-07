cmake_minimum_required(VERSION 3.8)
project(get_depth)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
set(CMAKE_EXPORT_CONPILE_COMMANDS "ON")
set(my_msgss_DIR /home/zyb/radar_station/src/my_msgss/install/my_msgss/share/my_msgss/cmake)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
# find_package(radar_msgs REQUIRED)
# find_package(tf REQUIRED)
find_package(PCL REQUIRED)
find_package(OpenCV)
find_package(my_msgss REQUIRED)
find_package(pcl_conversions REQUIRED)
include_directories(include ${my_msgss_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


add_executable(${PROJECT_NAME}_node src/get_depth_node.cpp)
ament_target_dependencies(${PROJECT_NAME}_node
        rclcpp
        rclpy
        std_msgs
        cv_bridge
        sensor_msgs
        my_msgss
        pcl_conversions
        # radar_msgs
        # tf
)
target_link_directories(${PROJECT_NAME}_node PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include >
      $<INSTALL_INTERFACE:include>)


# add_dependencies(${PROJECT_NAME}_node radar_msgs_generate_messages)
target_link_libraries(${PROJECT_NAME}_node
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}
        )

install(TARGETS
        ${PROJECT_NAME}_node
        DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME}/
)


ament_package()
