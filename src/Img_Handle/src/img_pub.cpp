#include "img_pub.hpp"

using namespace std::chrono_literals;

Img_Pub::Img_Pub() : Node("img_pub")
{
    img_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/img/send",10);
    str_pub_ = this->create_publisher<std_msgs::msg::String>("/img/str",10);
    timer_ = this->create_wall_timer(1s, std::bind(&Img_Pub::send_msg,this));
    this->my_img = imread("/home/mechax/zyb/radar_station/src/Img_Handle/demo_picture/1.png", IMREAD_COLOR);
    imshow("img", this->my_img);
    waitKey(0);
    RCLCPP_INFO(this->get_logger(), "begin to init");
}

string Img_Pub::mat_type23encoding(int mat_type)
{
    switch(mat_type)
    {
    case CV_8UC1:
       return "mono8";
    case CV_8UC3:
       return "bgr8";
    case CV_16SC1:
       return "mono16";
    case CV_8UC4:
       return "rgba8";
    default:
       return "";
    };
}

void Img_Pub::send_str_msg()
{
    RCLCPP_INFO(this->get_logger(), "began to send str");
    std_msgs::msg::String st;
    this->str = "hello";
    st.data = this->str;
    str_pub_->publish(st);
    RCLCPP_INFO(this->get_logger(), "send_str");
}

void Img_Pub::send_msg()
{
   RCLCPP_INFO(this->get_logger(), "began to send img");
   rclcpp::Clock my_clock;
   auto stamp = my_clock.now();
   sensor_msgs::msg::Image img_msg;
   img_msg.header.stamp = stamp;
   img_msg.header.frame_id = "";
   img_msg.height = this->my_img.rows;
   img_msg.width = this->my_img.cols;
   img_msg.encoding = this->mat_type23encoding(this->my_img.type());
   img_msg.is_bigendian = false;
   img_msg.step = static_cast<sensor_msgs::msg::Image::_step_type>(this->my_img.step);
   img_msg.data.assign(this->my_img.datastart, this->my_img.dataend);
   img_pub_->publish(img_msg);
   RCLCPP_INFO(this->get_logger(), "send_img");
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Img_Pub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}