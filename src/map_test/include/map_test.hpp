#include <sensor_msgs/msg/image.hpp>
#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "my_msgss/msg/points.hpp"
#include "my_msgss/msg/point.hpp"
#include "my_msgss/msg/distpoints.hpp"
#include "my_msgss/msg/distpoint.hpp"

#include <bitset>
#include "std_msgs/msg/float32_multi_array.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

using namespace std;
using namespace cv;
using namespace std::chrono_literals;

struct Robot
{
    int id;
    bool is_continue;
    float confidence;
    float x;
    float y;
    float distance;
};

struct map_point
{
    int id;
    double x;
    double y;
    double z;
};

class MapTest : public rclcpp::Node
{
public:
    MapTest();
    
    void parameter_init();

    void get_map();

    void far_callback(const my_msgss::msg::Distpoints msg);

    void close_callback(const my_msgss::msg::Distpoints msg);

    void pnp_callback(const std_msgs::msg::Float32MultiArray msg);

    void calibration();

    void draw_point_on_map(Mat &map,vector<map_point> &map_points);

    Point2f calculate_pixel_codi(const map_point &point);

    bool is_connect_to_server();
private:
    double object_width;
    double object_height;
    float image_width;
    float image_height;

    cv::Mat far_camera_matrix =Mat::zeros(3, 3, CV_64FC1);
    cv::Mat far_distortion_coefficient =Mat::zeros(5, 1, CV_64FC1);
    Mat far_Rjacob = Mat::zeros(3, 1, CV_64FC1);
    Mat far_R = Mat::zeros(3, 3, CV_64FC1);
    Mat far_T = Mat::zeros(3, 1, CV_64FC1);
    my_msgss::msg::Points far_points;

    cv::Mat close_camera_matrix =Mat::zeros(3, 3, CV_64FC1);
    cv::Mat close_distortion_coefficient =Mat::zeros(5, 1, CV_64FC1);
    Mat close_Rjacob = Mat::zeros(3, 1, CV_64FC1);
    Mat close_R = Mat::zeros(3, 3, CV_64FC1);
    Mat close_T = Mat::zeros(3, 1, CV_64FC1);
    my_msgss::msg::Points close_points;

    Mat test_map;
    Mat map;

    vector<map_point> far_map_points;
    vector<map_point> close_map_points;
    
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr far_points_pub_;
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr close_points_pub_;

    rclcpp::Subscription<my_msgss::msg::Distpoints>::SharedPtr far_distpoints_sub_;
    rclcpp::Subscription<my_msgss::msg::Distpoints>::SharedPtr close_distpoints_sub_;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr pnp_sub_;

    //-----------------------------------------------
    std::vector<Robot> far_robots;
    std::vector<Robot> close_robots;

    void robots_init();
    void get_robots(std::vector<Robot> &robots, const my_msgss::msg::Distpoints &input);
    void allrobots_adjust(std::vector<Robot> &robots);
    //-----------------------------------------------
    rclcpp::SyncParametersClient::SharedPtr paramClient;
    //-----------------------------------------------
};