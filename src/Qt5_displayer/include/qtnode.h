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

public:
    QImage far_qimage;
    QImage fardepth_qimage;
    QImage close_qimage;
    QImage closedepth_qimage;
    my_msgss::msg::Points far_world_qpoints;
    my_msgss::msg::Points close_world_qpoints;
    my_msgss::msg::Gamestate game_state_msg;
    my_msgss::msg::Radarmark radar_mark_msg;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pnp_pub_;
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr points_pub_;
    rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr color_pub_;
    rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr is_save_video_pub_;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr far_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr close_sub_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr fardepth_sub_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr closedepth_sub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr farpoints_sub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr closepoints_sub_;

    rclcpp::Subscription<my_msgss::msg::Gamestate>::SharedPtr game_state_sub_;
    rclcpp::Subscription<my_msgss::msg::Radarmark>::SharedPtr radar_mark_sub_;

    rclcpp::Node::SharedPtr qnode;

    float FAR_IMAGE_WIDTH = 640;
    float FAR_IMAGE_HEIGHT = 480;
    float DEPTH_IMAGE_WIDTH = 640;
    float DEPTH_IMAGE_HEIGHT = 480;

    std::chrono::high_resolution_clock::time_point far_start_time;
    std::chrono::high_resolution_clock::time_point far_end_time;
    std::chrono::high_resolution_clock::time_point close_start_time;
    std::chrono::high_resolution_clock::time_point close_end_time;
};

#endif // QTNODE_H
