#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "cv_bridge/cv_bridge.h"
#include <iostream>
#include <fstream>
#include <rclcpp/rclcpp.hpp>
#include <pcl/point_cloud.h>
#include <sensor_msgs/msg/detail/image__struct.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <pcl/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl_conversions/pcl_conversions.h>
#include <vector>

#include "my_msgss/msg/distpoint.hpp"
#include "my_msgss/msg/distpoints.hpp"
#include "my_msgss/msg/yolopoints.hpp"
#include "my_msgss/msg/points.hpp"

#include <std_msgs/msg/float64_multi_array.hpp>

#include <ament_index_cpp/get_package_share_directory.hpp>


#include <fstream>

struct Robot
{
  int id;
  int record_times;
  double confidence;
  bool is_continue;
  std::string tracking;
  cv::Rect box;
};

class GetDepth : public::rclcpp::Node

{

public:
  GetDepth();

  //声明所有的函数
  // void Kalman_Init();
  // double my_KalmanFilter(Kalman *kfp, double input);
  void parameter_init();
  void write_csv(std::string filename, std::vector<double> vals);
  cv::Mat Cloud2Mat(const pcl::PointCloud<pcl::PointXYZ>::Ptr &input);//Convert PointCloud to Mat
  void MatProject(cv::Mat &input_depth, cv::Mat &input_uv, cv::Mat &Cam_matrix, cv::Mat &Uni_matrix);
  void frame_point_match(const my_msgss::msg::Distpoints &last_frame, my_msgss::msg::Distpoints &this_frame);
  void far_yoloCallback(const my_msgss::msg::Yolopoints &input);
  void close_yoloCallback(const my_msgss::msg::Yolopoints &input);
  void pointCloudCallback(const sensor_msgs::msg::PointCloud2 &input);
  void outpost_Callback(const my_msgss::msg::Points &outpost);
  void calibration_result_Callback(const std_msgs::msg::Float64MultiArray &calibration_result);
  double getDepthInRect(cv::Rect rect, std::vector<cv::Mat>& depth_queue, my_msgss::msg::Yolopoint::_id_type id);//得到ROI中点的深度
  void closeImageCallback(const sensor_msgs::msg::Image msg);
  void farImageCallback(const sensor_msgs::msg::Image msg);
  void distance_filter(std::vector<double> & distances);
  cv::Mat distance_to_image(cv::Mat depth, cv::Mat image);
  //声明publisher
  rclcpp::Publisher<my_msgss::msg::Distpoints>::SharedPtr far_distancePointPub;
  rclcpp::Publisher<my_msgss::msg::Distpoints>::SharedPtr close_distancePointPub;
  rclcpp::Publisher<my_msgss::msg::Distpoint>::SharedPtr outpost_distancePointPub;
  rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr far_depth_qimage_pub;
  rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr close_depth_qimage_pub;

  //声明消息
  my_msgss::msg::Distpoints far_distance_it;
  my_msgss::msg::Distpoints close_distance_it;
  my_msgss::msg::Distpoints last_far_distance_it;
  my_msgss::msg::Distpoints last_close_distance_it;
  my_msgss::msg::Distpoint outpost_distance_it;


  //申明subscriber
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr cloud_sub;
  rclcpp::Subscription<my_msgss::msg::Yolopoints>::SharedPtr far_yolo_sub;
  rclcpp::Subscription<my_msgss::msg::Yolopoints>::SharedPtr close_yolo_sub;
  rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr outpost_Sub;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr far_image_sub;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr close_image_sub;

  rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr calibration_result_sub;
  

  uint16_t times;
  std::vector<int> cnt;
  std::vector<double> dists;
  int imgRows, imgCols;
  int length_of_cloud_queue;//default length is 5
  int post_pub_flag = 0;
  cv::Point2f outpost_point;
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
  std::vector<cv::Mat> far_depth_queue;
  cv::Mat far_camera_matrix;//相机内参矩阵
  cv::Mat far_uni_matrix;//相机和雷达的变换矩阵
  cv::Mat far_distortion_coefficient;
  std::vector<cv::Mat> close_depth_queue;
  cv::Mat close_camera_matrix;//相机内参矩阵
  cv::Mat close_uni_matrix;//相机和雷达的变换矩阵
  cv::Mat close_distortion_coefficient;

  cv::Mat far_image;
  cv::Mat close_image;

  //---------------------------------------------------
  std::vector<Robot> robots;
  std::vector<Robot> far_robots;
  std::vector<Robot> close_robots;

  double min_iou;

  void robots_init();
  bool box_match(const cv::Rect &box, const cv::Rect &new_box,const double & min_iou);
  void get_robots(std::vector<Robot> &robots, const my_msgss::msg::Yolopoints &input);
  void allrobots_adjust(std::vector<Robot> &robots);
  //---------------------------------------------------
  rclcpp::SyncParametersClient::SharedPtr paramClient;

  bool is_connect_to_server();
  //---------------------------------------------------
} ;

