#include "video_save/video_save.hpp"


VideoSave::VideoSave() : Node("video_save")
{
    RCLCPP_INFO(get_logger(), "Start video_save node sucessfully!");
    is_begin_to_save = false;
    last_time1 = this->now();
    last_time2 = this->now();
    count1 = 0;
    count2 = 0;
    img_far_sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>("/image_far/compressed",1,std::bind(&VideoSave::video_far_callback,this,std::placeholders::_1));
    img_close_sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>("/image_close/compressed",1,std::bind(&VideoSave::video_close_callback,this,std::placeholders::_1));

    is_begin_to_save_sub_ = this->create_subscription<std_msgs::msg::Int8>("/is_begin_to_save",1,std::bind(&VideoSave::is_begin_to_save_callback,this,std::placeholders::_1)); 

    
    std::string source_path = (_FILE_);
    std_string pkg_path = source_path.substr(0, source_path.find("src") - 1);
    video_far_path = pkg_path + "/video_far_/";
    video_close_path = pkg_path + "/video_close_/";
}
VideoSave::~VideoSave()
{
    out1_.release();
    out2_.release();
}

void VideoSave::is_begin_to_save_callback(std_msgs::msg::Int8::SharedPtr msg)
{
    is_begin_to_save = msg->data;
}

void VideoSave::video_far_callback(sensor_msgs::msg::CompressedImage msg)
{
    if(!is_begin_to_save)
    {
        return;
    }
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch(cv_bridge::Exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }
    this->sub_video_far_frame = cv_ptr->image;
    rclcpp::Time now = this->now();
    double fps = 1.0 / (now - this->last_time1).seconds();
    last_time1 = now;
    if(count1 < 100)
    {
        count1++;
        return;
    }
    RCLCPP_INFO(this->get_logger(), "fps: %f", fps);
    if (!out1_is_opened)
    {
        RCLCPP_INFO(get_logger(), "began to save video_far!");
        std::time_t now = std::time(0);
        char buffer[100];
        std::strftime(buffer, 100, "%Y-%m-%d-%H-%M-%S", std::localtime(&now));
        std::string buffer_str(buffer); // 将字符数组转换为字符串
        out1_.open(video_far_path+buffer_str+".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 24, this->sub_video_far_frame.size());
        out1_is_opened = true;
    }
    out1_.set(cv::CAP_PROP_FPS, fps);
    out1_.write(this->sub_video_far_frame);
}

void VideoSave::video_close_callback(sensor_msgs::msg::CompressedImage msg)
{
    if(!is_begin_to_save)
    {
        return;
    }
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch(cv_bridge::Exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }
    this->sub_video_close_frame = cv_ptr->image;
    rclcpp::Time now2 = this->now();
    double fps = 1.0 / (now2 - this->last_time2).seconds();
    last_time2 = now2;
    if(count2 < 100)
    {
        count2++;
        return;
    }
    RCLCPP_INFO(this->get_logger(), "fps: %f", fps);
    if (!out2_is_opened)
    {
        RCLCPP_INFO(get_logger(), "began to save video_close!");
        std::time_t now = std::time(0);
        char buffer[100];
        std::strftime(buffer, 100, "%Y-%m-%d-%H-%M-%S", std::localtime(&now));
        std::string buffer_str(buffer); // 将字符数组转换为字符串
        out2_.open(video_close_path+buffer_str+".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 24, this->sub_video_close_frame.size());
        out2_is_opened = true;
    }
    out2_.set(cv::CAP_PROP_FPS, fps);
    out2_.write(this->sub_video_close_frame);
}

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VideoSave>();
    
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}