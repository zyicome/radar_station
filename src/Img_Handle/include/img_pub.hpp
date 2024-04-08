#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "opencv2/opencv.hpp"
#include "std_msgs/msg/string.hpp"
#include "opencv4/opencv2/opencv_modules.hpp"

using namespace std;
using namespace cv;

class Img_Pub : public::rclcpp::Node
{
public:
    Img_Pub();
    
    void send_msg();

    void send_str_msg();
    
    string mat_type23encoding(int mat_type);

    Mat my_img;     
    string str;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr img_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr str_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};