// C++ system
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "my_msgss/msg/gamestate.hpp"
#include "my_msgss/msg/supplyaction.hpp"
#include "my_msgss/msg/refereewarning.hpp"
#include "my_msgss/msg/points.hpp"

#include "my_msgss/msg/radarmark.hpp"

#include "std_msgs/msg/int8.hpp"

#include "opencv2/opencv.hpp"

struct robot
{
    int id;
    float radar_mark_progress;
    float last_radar_mark_progress;
    int x;
};

class Judge : public rclcpp::Node
{
public:
    Judge();

    void worldPointsCallback(const my_msgss::msg::Points::SharedPtr msg);

    void colorCallback(const std_msgs::msg::Int8::SharedPtr msg);

    void sendRadarMarkProgress();

    void robots_init();

    int our_color;

    std::vector<robot> robots;

    rclcpp::Publisher<my_msgss::msg::Radarmark>::SharedPtr radar_mark_pub;

    rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr worldpoints_sub;

    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr color_sub;

    rclcpp::TimerBase::SharedPtr send_timer;
};