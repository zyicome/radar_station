#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/msg/string.hpp"
#include "opencv4/opencv2/opencv_modules.hpp"
#include "inference.h"
#include "my_msgss/msg/yolopoint.hpp"
#include "my_msgss/msg/yolopoints.hpp"
#include <vector>

using namespace std;
using namespace cv;

class Img_Sub : public::rclcpp::Node
{
public:
    Img_Sub();

    ~Img_Sub();
    
    Mat sub_img_far;
    Mat sub_img_close;

    int i;

    bool is_far_sub;
    bool is_close_sub;

    void save_img();

    void img_far_callback(const sensor_msgs::msg::Image msg);

    void img_close_callback(const sensor_msgs::msg::Image msg);

    void yolo_init();

    void yolo_robot_identify(Mat & sub_img,vector<Detection> &robot_output, my_msgss::msg::Yolopoints &robot_boxes,Inference &inf_robot,Inference &inf_armor);

    void yolo_armor_identify(Mat & sub_img,my_msgss::msg::Yolopoint &robot_box, cv::Rect &box,Inference &inf_armor);

    void draw_img(Mat & sub_img,my_msgss::msg::Yolopoints &robot_boxes);

    void test();

    my_msgss::msg::Yolopoints far_robot_boxes;
    vector<Detection> far_robot_output;

    my_msgss::msg::Yolopoints close_robot_boxes;
    vector<Detection> close_robot_output;

    Inference far_inf_armor;
    Inference close_inf_armor;

    Inference far_inf_robot;
    Inference close_inf_robot;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_far_sub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoint>::SharedPtr far_yolopoint_pub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoints>::SharedPtr far_yolopoints_pub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr far_qimage_pub_;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_close_sub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoint>::SharedPtr close_yolopoint_pub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoints>::SharedPtr close_yolopoints_pub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr close_qimage_pub_;

    /*std::thread far_thread_;
    std::thread close_thread_;*/
};