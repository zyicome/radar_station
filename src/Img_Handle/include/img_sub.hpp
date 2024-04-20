#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/msg/string.hpp"
#include "opencv4/opencv2/opencv_modules.hpp"
#include "inference.h"
#include "my_msgss/msg/yolopoint.hpp"
#include "my_msgss/msg/yolopoints.hpp"
#include <vector>
#include "number_classifier.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "extended_kalman_filter.hpp"

using namespace std;
using namespace cv;

struct Robot
{
    int id;
    int record_times;
    float confidence;
    bool is_continue;
    string tracking;
    cv::Rect box;

    ExtendedKalmanFilter ekf;
};

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

    void img_far_callback(const sensor_msgs::msg::CompressedImage msg);

    void img_close_callback(const sensor_msgs::msg::CompressedImage msg);

    void yolo_init();

    void yolo_robot_identify(Mat & sub_img, my_msgss::msg::Yolopoints &robot_boxes,vector<Robot> &robots,Inference &inf_robot,Inference &inf_armor);

    void yolo_armor_identify(Mat & sub_img,vector<Robot> &robots, cv::Rect &box,Inference &inf_armor,Detection &robot_output);

    void draw_img(Mat & sub_img,my_msgss::msg::Yolopoints &robot_boxes);

    void test();

    //--------------------------------
    //new function
    void robots_init();

    bool box_match(const cv::Rect &box, const cv::Rect &new_box);

    bool distance_match(const cv::Rect &box, const cv::Rect &new_box);

    void robots_adjust(const Detection &armor_output, vector<Robot> &robots, Detection &robot_output);

    void allrobots_adjust(vector<Robot> &robots,my_msgss::msg::Yolopoints &robot_boxes);

    void not_tracking_robots_adjust(const Detection &armor_output, vector<Robot> &robots, Detection &robot_output);

    void not_tracking_allrobots_adjust(vector<Robot> &robots, my_msgss::msg::Yolopoints &robot_boxes);
    //new place
    vector<Robot> far_robots;
    vector<Robot> close_robots;

    int min_detection_times;
    int max_lost_times;
    int max_tracking_lost_times;
    double min_accept_confidence;
    double min_iou;
    double max_accept_distance;
    //
    //---------------------------------
    //---------------------------------
    //kalman
    double dt_;
    double s2qxyz_, s2qyaw_, s2qr_;
    double r_xyz_factor, r_yaw;

    void kalman_init();

    void initEKF(Robot &robot);

    void predict(Robot &robot);

    void update(Robot &robot, const Eigen::VectorXd & measurement);
    //---------------------------------

    my_msgss::msg::Yolopoints far_robot_boxes;

    my_msgss::msg::Yolopoints close_robot_boxes;

    Inference far_inf_armor;
    Inference close_inf_armor;

    Inference far_inf_robot;
    Inference close_inf_robot;

    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr img_far_sub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoint>::SharedPtr far_yolopoint_pub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoints>::SharedPtr far_yolopoints_pub_;
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr far_qimage_pub_;

    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr img_close_sub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoint>::SharedPtr close_yolopoint_pub_;
    rclcpp::Publisher<my_msgss::msg::Yolopoints>::SharedPtr close_yolopoints_pub_;
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr close_qimage_pub_;

    NumberClassifier numberclassifier;

    /*std::thread far_thread_;
    std::thread close_thread_;*/
};