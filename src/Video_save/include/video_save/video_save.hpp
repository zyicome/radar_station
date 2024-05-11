#include <opencv4/opencv2/opencv.hpp>
#include "image_transport/image_transport.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "rclcpp/rclcpp.hpp"
#include "cv_bridge/cv_bridge.h"

#include "std_msgs/msg/int8.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>


class VideoSave : public rclcpp::Node
{
    public:
        VideoSave();
        ~VideoSave();
        
        cv::Mat sub_video_far_frame;
        cv::Mat sub_video_close_frame;
        void video_far_callback(sensor_msgs::msg::CompressedImage msg);
        void video_close_callback(sensor_msgs::msg::CompressedImage msg);
        void is_begin_to_save_callback(std_msgs::msg::Int8::SharedPtr msg);

        rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr img_far_sub_;
        rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr img_close_sub_;
        rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr is_begin_to_save_sub_;
        cv::VideoWriter out1_;
        cv::VideoWriter out2_;
        rclcpp::Time last_time1;
        rclcpp::Time last_time2;
        bool out1_is_opened = false;
        bool out2_is_opened = false;
        int count1;
        int count2;

        bool is_begin_to_save;
        std::string video_far_path;
        std::string video_close_path;
};