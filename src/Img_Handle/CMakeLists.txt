cmake_minimum_required(VERSION 3.8)
project(Img_Handle)

set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
enable_language(CUDA)

# include and link dirs of cuda and tensorrt, you need adapt them if yours are different
if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  message("embed_platform on")
  include_directories(/usr/local/cuda/targets/aarch64-linux/include)
  link_directories(/usr/local/cuda/targets/aarch64-linux/lib)
else()
  message("embed_platform off")
  # cuda
  include_directories(/usr/local/cuda/include)
  link_directories(/usr/local/cuda/lib64)

  # tensorrt
  include_directories(/home/mechax/TensorRT-8.6/include)
  link_directories(/home/mechax/TensorRT-8.6/lib)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC)
  add_compile_options(/W0)
else()
  add_compile_options(-w)
endif()


# find dependencies
set(my_msgss_DIR /home/zyb/radar_station/src/my_msgss/install/my_msgss/share/my_msgss/cmake)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(my_msgss REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(include ${my_msgss_INCLUDE_DIRS})
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/plugin)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("/usr/include/eigen3")


endif()

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu)

# Img_Pub的生成
add_executable(Img_Pub src/img_pub.cpp)
ament_target_dependencies(Img_Pub rclcpp sensor_msgs)
target_link_directories(Img_Pub PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(Img_Pub ${OpenCV_LIBS})

# src/img_sub.cpp src/inference.cpp src/number_classifier.cpp src/extended_kalman_filter.cpp
# src/block.cpp src/calibrator.cpp inference_cuda.cpp model.cpp postprocess.cpp 

# Img_Sub的生成
add_executable(Img_Sub src/img_sub.cpp src/inference.cpp src/number_classifier.cpp src/extended_kalman_filter.cpp src/config.cpp
              src/block.cpp src/calibrator.cpp src/inference_cuda.cpp src/model.cpp src/postprocess.cpp src/postprocess.cu src/preprocess.cu
              plugin/yololayer.cu)
ament_target_dependencies(Img_Sub rclcpp sensor_msgs cv_bridge my_msgss Eigen3)
target_link_directories(Img_Sub PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
target_link_libraries(Img_Sub ${OpenCV_LIBS})
target_link_libraries(Img_Sub nvinfer cudart)

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
  Yolov8_weight
  new_model
  config
  plugin
  src
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
