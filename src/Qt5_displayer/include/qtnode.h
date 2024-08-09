#ifndef QTNODE_H
#define QTNODE_H

#include <QThread>
#include <QImage>
#include <std_msgs/msg/detail/float32_multi_array__struct.hpp>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/int8.hpp"
#include "my_msgss/msg/points.hpp"
#include "my_msgss/msg/gamestate.hpp"
#include "my_msgss/msg/radarmark.hpp"
#include "my_msgss/msg/radarinfo.hpp"
#include "my_msgss/msg/hp.hpp"
#include "my_msgss/msg/dart.hpp"
#include "my_msgss/msg/siteevent.hpp"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"

#include <chrono>

using namespace std;
using namespace cv;

class qtNode : public QThread
{
    Q_OBJECT
public:
    qtNode();
    ~qtNode();

    void farImageCallback(const sensor_msgs::msg::Image msg);
    void farDepthImageCallback(const sensor_msgs::msg::CompressedImage msg);
    void farPointsCallback(const my_msgss::msg::Points msg);


    void closeImageCallback(const sensor_msgs::msg::Image msg);
    void closeDepthImageCallback(const sensor_msgs::msg::CompressedImage msg);
    void closePointsCallback(const my_msgss::msg::Points msg);

    void gameStateCallback(const my_msgss::msg::Gamestate msg);
    void radarMarkCallback(const my_msgss::msg::Radarmark msg);
    void radarInfoCallback(const my_msgss::msg::Radarinfo msg);
    void hpCallback(const my_msgss::msg::Hp msg);
    void dartCallback(const my_msgss::msg::Dart msg);
    void siteEventCallback(const my_msgss::msg::Siteevent msg);

    void run() override;

Q_SIGNALS:
    void updateFarImage();
    void updateCloseImage();
    void updateFarDepthImage();
    void updateCloseDepthImage();
    void updateFarPoints();
    void updateClosePoints();
    void updateGameState();
    void updateRadarMark();
    void updateRadarInfo();
    void updateHp();
    void updateDart();
    void updateSiteEvent();

public:
    QImage far_qimage;
    QImage fardepth_qimage;
    QImage close_qimage;
    QImage closedepth_qimage;
    my_msgss::msg::Points far_world_qpoints;
    my_msgss::msg::Points close_world_qpoints;
    my_msgss::msg::Gamestate game_state_msg;
    my_msgss::msg::Radarmark radar_mark_msg;
    my_msgss::msg::Radarinfo radar_info_msg;
    my_msgss::msg::Hp hp_msg;
    my_msgss::msg::Dart dart_msg;
    my_msgss::msg::Siteevent site_event_msg;

    cv::Mat far_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat far_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);
    cv::Mat close_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat close_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pnp_pub_;
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr points_pub_;
    rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr color_pub_;
    rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr mode_pub_;
    rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr is_save_video_pub_;

    rclcpp::Publisher<my_msgss::msg::Radarinfo>::SharedPtr radar_info_pub_;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr far_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr close_sub_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr fardepth_sub_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr closedepth_sub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr farpoints_sub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr closepoints_sub_;

    rclcpp::Subscription<my_msgss::msg::Gamestate>::SharedPtr game_state_sub_;
    rclcpp::Subscription<my_msgss::msg::Radarmark>::SharedPtr radar_mark_sub_;
    rclcpp::Subscription<my_msgss::msg::Radarinfo>::SharedPtr radar_info_sub_;
    rclcpp::Subscription<my_msgss::msg::Hp>::SharedPtr hp_sub_;
    rclcpp::Subscription<my_msgss::msg::Dart>::SharedPtr dart_sub_;
    rclcpp::Subscription<my_msgss::msg::Siteevent>::SharedPtr site_event_sub_;


    rclcpp::Node::SharedPtr qnode;

    float FAR_IMAGE_WIDTH = 1280;
    float FAR_IMAGE_HEIGHT = 1024;
    float DEPTH_IMAGE_WIDTH = 640;
    float DEPTH_IMAGE_HEIGHT = 480;

    std::chrono::high_resolution_clock::time_point far_start_time;
    std::chrono::high_resolution_clock::time_point far_end_time;
    std::chrono::high_resolution_clock::time_point close_start_time;
    std::chrono::high_resolution_clock::time_point close_end_time;

    //---------------------------------------------------
    rclcpp::SyncParametersClient::SharedPtr paramClient;

    bool is_connect_to_server();

    void client_parameter_init();
    //---------------------------------------------------
};

#endif // QTNODE_H
