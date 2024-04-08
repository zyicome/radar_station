#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "cv_bridge/cv_bridge.h"
#include <iostream>
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


#include <fstream>

struct Robot
{
  int id;
  int record_times;
  float confidence;
  bool is_continue;
  std::string tracking;
  cv::Rect box;
};

class GetDepth : public::rclcpp::Node

{

public:
  GetDepth();

private:
  //声明所有的函数
  // void Kalman_Init();
  // float my_KalmanFilter(Kalman *kfp, float input);
  void init_camera_matrix();
  void write_csv(std::string filename, std::vector<float> vals);
  cv::Mat Cloud2Mat(const pcl::PointCloud<pcl::PointXYZ>::Ptr &input);//Convert PointCloud to Mat
  void MatProject(cv::Mat &input_depth, cv::Mat &input_uv, cv::Mat &Cam_matrix, cv::Mat &Uni_matrix);
  void frame_point_match(const my_msgss::msg::Distpoints &last_frame, my_msgss::msg::Distpoints &this_frame);
  void far_yoloCallback(const my_msgss::msg::Yolopoints &input);
  void close_yoloCallback(const my_msgss::msg::Yolopoints &input);
  void pointCloudCallback(const sensor_msgs::msg::PointCloud2 &input);
  void outpost_Callback(const my_msgss::msg::Points &outpost);
  float getDepthInRect(cv::Rect rect, std::vector<cv::Mat>& depth_queue, my_msgss::msg::Yolopoint::_id_type id);//得到ROI中点的深度
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
  

  uint16_t times;
  std::vector<int> cnt;
  std::vector<float> dists;
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

  //---------------------------------------------------
  std::vector<Robot> robots;
  std::vector<Robot> far_robots;
  std::vector<Robot> close_robots;

  float min_iou;

  void robots_init();
  bool box_match(const cv::Rect &box, const cv::Rect &new_box,const float & min_iou);
  void get_robots(std::vector<Robot> &robots, const my_msgss::msg::Yolopoints &input);
  void allrobots_adjust(std::vector<Robot> &robots);
  //---------------------------------------------------
} ;

