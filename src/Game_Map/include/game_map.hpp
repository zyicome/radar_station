#include <sensor_msgs/msg/image.hpp>
#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "my_msgss/msg/points.hpp"
#include "my_msgss/msg/point.hpp"
#include "my_msgss/msg/distpoints.hpp"
#include "my_msgss/msg/distpoint.hpp"

#include <bitset>

using namespace std;
using namespace cv;
using namespace std::chrono_literals;

class Game_Map : public::rclcpp::Node
{
public:
    Game_Map();
private:
    // 函数
    void get_all_parameter();
    void warn_regions_init();
    void get_map_init();
    void camera_pnp_calibration();

    void far_calibration(const my_msgss::msg::Points &msg);//相机标定
    void far_distPointCallback(const my_msgss::msg::Distpoints &input);

    void close_calibration(const my_msgss::msg::Points &msg);//相机标定
    void close_distPointCallback(const my_msgss::msg::Distpoints &input);

    void remove_duplicate();

    double Point2PointDist(const my_msgss::msg::Point &a, const Point3f &b);

    double calculate_dist(const my_msgss::msg::Point &a, const my_msgss::msg::Point &b);

    bool is_enemy_car(uint8_t id);


    void warn_on_map(const my_msgss::msg::Points &points, Mat &image);

    void draw_point_on_map(const my_msgss::msg::Point &point, Mat &image);

    void draw_warn_region(Mat &image, const vector<vector<Point>> &our_regions, const vector<vector<Point>> &enemy_regions);

    my_msgss::msg::Point calculate_relative_codi(const Point3f &guard, const my_msgss::msg::Point &enemy, uint8_t priority_id);

    Point2f calculate_pixel_codi(const my_msgss::msg::Point &point);

    Point2f calculate_pixel_text_codi(const my_msgss::msg::Point &point);

    void outpost_Callback(const my_msgss::msg::Distpoint &msg);

    void timer_callback();





    // 参数
    cv::Mat img;
    int field_width, field_height;
    double imgCols, imgRows;
    int red_or_blue;//0 is red, 1 is blue
    int outpost_calc_flag;
    vector<cv::Point3d> far_objectPoints;
    vector<cv::Point2d> far_imagePoints = {cv::Point2d(0, 0), cv::Point2d(0, 0), cv::Point2d(0, 0), cv::Point2d(0, 0)};
    cv::Mat far_CamMatrix_;
    cv::Mat far_distCoeffs_;
    Mat far_Rjacob;
    Mat far_R;
    Mat far_T;
    vector<cv::Point3d> close_objectPoints;
    vector<cv::Point2d> close_imagePoints;
    cv::Mat close_CamMatrix_;
    cv::Mat close_distCoeffs_;
    Mat close_Rjacob;
    Mat close_R;
    Mat close_T;
    my_msgss::msg::Points far_points;
    my_msgss::msg::Points close_points;
    my_msgss::msg::Points result_points;
    my_msgss::msg::Points relative_coordinates;
    my_msgss::msg::Points guard_relative;
    my_msgss::msg::Distpoint outpost_dist;
    Point3f our_guard;
    Point3f enemy_guard;
    Point3f our_hero;
    Point2f outpost_2d_armour;
    Point3f outpost_3d_armour;
    int X_shift = 0;
    int Y_shift = 0;
    uint8_t in_our_base_cnt;
    uint16_t warn_region_state;
    string btlcolor;

    Mat small_map;
    Mat small_map_copy;

    vector<Point> our_R1;
    vector<Point> our_R2;
    vector<Point> our_R3;
    vector<Point> our_dafu;
    vector<Point> our_highway;
    vector<Point> our_outpost;
    vector<Point> our_half_field;
    vector<Point> enemy_highway;
    vector<Point> enemy_dafu;
    vector<Point> enemy_outpost;
    vector<Point> enemy_hero_hide;
    vector<Point> enemy_R3;
    vector<Point> guard_forbidden_zone;
    vector<Point> guidao_houbian;
    
    vector<vector<Point>> our_warn_regions;
    vector<vector<Point>> enemy_warn_regions;

    string our_region_names[6] = {"our_R1", "our_R2", "our_R3", "our_dafu", "our_highway", "our_outpost"};
    string enemy_region_names[5] = {"enemy_highway", "enemy_dafu", "enemy_outpost", "enemy_hero_hide", "enemy_R3"};

    // ros2发布及订阅者
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr world_points_pub_;
    rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr guard_points_pub_;
    rclcpp::Publisher<my_msgss::msg::Point>::SharedPtr hero_point_pub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr far_image_sub_;
    rclcpp::Subscription<my_msgss::msg::Distpoints>::SharedPtr far_distpoints_sub_;
    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr close_image_sub_;
    rclcpp::Subscription<my_msgss::msg::Distpoints>::SharedPtr close_distpoints_sub_;
    rclcpp::Subscription<my_msgss::msg::Distpoint>::SharedPtr outpost_distpoint_sub_;

    rclcpp::TimerBase::SharedPtr timer_;
};