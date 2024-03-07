#include "img_sub.hpp"

Img_Sub::Img_Sub() : Node("img_sub")
{
   img_far_sub_ = this->create_subscription<sensor_msgs::msg::Image>("/image/far",1,std::bind(&Img_Sub::img_far_callback,this,std::placeholders::_1));
   this->far_yolopoint_pub_ = this->create_publisher<my_msgss::msg::Yolopoint>("/img/far/yolopoint",1);
   this->far_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/far_rectangles",1);
   this->far_qimage_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/qt/far_qimage",1);

   img_close_sub_ = this->create_subscription<sensor_msgs::msg::Image>("/image/far",1,std::bind(&Img_Sub::img_close_callback,this,std::placeholders::_1));
   this->close_yolopoint_pub_ = this->create_publisher<my_msgss::msg::Yolopoint>("/img/close/yolopoint",1);
   this->far_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/close_rectangles",1);
   this->far_qimage_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/qt/close_qimage",1);
//    this->i = 5;
//    this->declare_parameter("y", 2);
//    this->get_parameter("y", this->i);
//    cout << this->i << endl;
   RCLCPP_INFO(this->get_logger(), "begin to init");
   this->yolo_init();

    is_far_sub = false;
    is_close_sub = false;

    /*far_thread_ = std::thread{[this]() -> void {
        if(is_far_sub == true)
        {
            this->far_robot_boxes.data.clear();
        this->far_robot_boxes.id = 0;
        this->far_robot_boxes.text = "none";
        this->yolo_robot_identify(sub_img_far,far_robot_output,far_robot_boxes,far_inf_robot,far_inf_armor);
        if(far_robot_boxes.data.size() != 0)
        {
            RCLCPP_INFO(get_logger(), "began to send farrobot_boxes");
            far_yolopoints_pub_->publish(far_robot_boxes);
        }
        this->draw_img(sub_img_far,far_robot_boxes);
        far_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_far).toImageMsg()));
        is_far_sub = false;
        }
    }};
    close_thread_ = std::thread{[this]() -> void {
        if(is_close_sub == true)
        {
            this->close_robot_boxes.data.clear();
        this->close_robot_boxes.id = 0;
        this->close_robot_boxes.text = "none";
        this->yolo_robot_identify(sub_img_close,close_robot_output,close_robot_boxes,close_inf_robot,close_inf_armor);
        if(close_robot_boxes.data.size() != 0)
        {
            RCLCPP_INFO(get_logger(), "began to send farrobot_boxes");
            close_yolopoints_pub_->publish(close_robot_boxes);
        }
        this->draw_img(sub_img_close,close_robot_boxes);
        close_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_close).toImageMsg()));
        is_close_sub = false;
        }
    }};*/
   // this->test();
}

Img_Sub::~Img_Sub()
{
    /*if (far_thread_.joinable()) {
      far_thread_.join();
    }

    if(close_thread_.joinable()) {
      close_thread_.join();
    }*/
}

void Img_Sub::img_far_callback(sensor_msgs::msg::Image msg)
{
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
        this->sub_img_far = cv_ptr->image;
        //is_far_sub = true;
        /*if(this->i == 1)
        {
            this->save_img();
            this->i++;
         }*/
        //cout << "this->img_sub.rows = " << this->sub_img.rows << endl;
        //cout << "this->img_sub.cols = " << this->sub_img.cols << endl;
        this->far_robot_boxes.data.clear();
        this->far_robot_boxes.id = 0;
        this->far_robot_boxes.text = "none";
        this->yolo_robot_identify(sub_img_far,far_robot_output,far_robot_boxes,far_inf_robot,far_inf_armor);
        if(far_robot_boxes.data.size() != 0)
        {
            RCLCPP_INFO(get_logger(), "began to send farrobot_boxes");
            far_yolopoints_pub_->publish(far_robot_boxes);
        }
        //this->draw_img(sub_img_far,far_robot_boxes);
        far_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_far).toImageMsg()));
}

void Img_Sub::img_close_callback(sensor_msgs::msg::Image msg)
{
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
        this->sub_img_close = cv_ptr->image;
        //is_close_sub = true;
        /*if(this->i == 1)
        {
            this->save_img();
            this->i++;
         }*/
        //cout << "this->img_sub.rows = " << this->sub_img.rows << endl;
        //cout << "this->img_sub.cols = " << this->sub_img.cols << endl;
        this->close_robot_boxes.data.clear();
        this->close_robot_boxes.id = 0;
        this->close_robot_boxes.text = "none";
        this->yolo_robot_identify(sub_img_close,close_robot_output,close_robot_boxes,close_inf_robot,close_inf_armor);
        if(close_robot_boxes.data.size() != 0)
        {
            RCLCPP_INFO(get_logger(), "began to send farrobot_boxes");
            close_yolopoints_pub_->publish(close_robot_boxes);
        }
        //this->draw_img(sub_img_close,close_robot_boxes);
        close_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_close).toImageMsg()));
}

void Img_Sub::yolo_init()
{
    bool runOnGPU = false;
    // 1. 设置你的onnx模型
    // Note that in this example the classes are hard-coded and 'classes.txt' is a place holder.
    Inference inf_armors("/home/qianli/zyb/radar_station/src/Img_Handle/Yolov8_weight/armor/weights/best.onnx",
                        cv::Size(640, 640),
                        "/home/qianli/zyb/radar_station/src/Img_Handle/Yolov8_weight/armor/class/class.txt",
                        runOnGPU); // classes.txt 可以缺失
    Inference inf_robots("/home/qianli/zyb/radar_station/src/Img_Handle/Yolov8_weight/robot/weights/best.onnx",
                        cv::Size(640, 640),
                        "/home/qianli/zyb/radar_station/src/Img_Handle/Yolov8_weight/robot/class/class.txt",
                        runOnGPU);

    this->far_inf_armor = inf_armors;
    this->far_inf_robot = inf_robots;
    this->close_inf_armor = inf_armors;
    this->close_inf_robot = inf_robots;

    RCLCPP_INFO(this->get_logger(), "begin to init");
}


//神经网络识别机器人
void Img_Sub::yolo_robot_identify(Mat & sub_img,vector<Detection> &robot_output, my_msgss::msg::Yolopoints &robot_boxes,Inference &inf_robot,Inference &inf_armor)
{
        auto start = std::chrono::steady_clock::now();
        // Inference starts here...
        robot_output = inf_robot.runInference(sub_img);

        int detections = robot_output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        // feiyull
        // 这里需要resize下，否则结果不对
        cv::resize(sub_img, sub_img, cv::Size(sub_img.cols, sub_img.rows));

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = robot_output[i];

            cv::Rect box = detection.box;
            if(box.x < 0)
            {
                box.x = 0;
            }
            if(box.y < 0)
            {
                box.y = 0;
            }
            if(box.x + box.width > sub_img.cols)
            {
                box.width = sub_img.cols - box.x;
            }
            if(box.y + box.height > sub_img.rows)
            {
                box.height = sub_img.rows - box.y;
            }
            my_msgss::msg::Yolopoint robot_box;
            robot_box.x = box.x;
            robot_box.y = box.y;
            robot_box.width = box.width;
            robot_box.height = box.height;
            this->yolo_armor_identify(sub_img,robot_box,box,inf_armor);
            robot_boxes.data.push_back(robot_box);
            robot_boxes.id++;
            robot_boxes.text = "have";

            
            /*cv::Scalar color = detection.color;

            // Detection box
            cv::rectangle(this->sub_img, box, color, 2);

            /// Detection box text
            std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
            cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
            cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

            cv::rectangle(this->sub_img, textBox, color, cv::FILLED);
            cv::putText(this->sub_img, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);*/
        }
        /*cv::imshow("Inference", this->sub_img);
        cv::waitKey(0);
        cv::destroyAllWindows();*/
        auto end = std::chrono::steady_clock::now();
        RCLCPP_INFO(get_logger(), "Inference took %ld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

}

void Img_Sub::yolo_armor_identify(Mat & sub_img,my_msgss::msg::Yolopoint &robot_box, cv::Rect &box, Inference &inf_armor)
{
    if(box.x < 0)
    {
        box.x = 0;
    }
    if(box.y < 0)
    {
        box.y = 0;
    }
    if(box.x + box.width > sub_img.cols)
    {
        box.width = sub_img.cols - box.x;
    }
    if(box.y + box.height > sub_img.rows)
    {
        box.height = sub_img.rows - box.y;
    }
    Mat armor_img = sub_img(box);
    resize(armor_img, armor_img, Size(300, 300));
    vector<Detection> armor_output = inf_armor.runInference(armor_img);
    resize(armor_img, armor_img, Size(300, 300));
    int detections = armor_output.size();
    if(detections == 0)
    {
        robot_box.id = -1;
    }
    for(int j = 0; j < detections; j++)
    {
    if(armor_output[j].class_id <= 8)
        {
            if(armor_output[j].class_id != 0)
            {
                robot_box.id = armor_output[j].class_id;
                robot_box.color = "blue";
            }
        }
        else if(armor_output[j].class_id >= 9 && armor_output[j].class_id<= 17)
        {
            if(armor_output[j].class_id != 9)
            {
                robot_box.id = armor_output[j].class_id;
                robot_box.color = "red";
            }
        }
    }
}

void Img_Sub::draw_img(Mat & sub_img,my_msgss::msg::Yolopoints &robot_boxes)
{
    for(int i =0 ;i<robot_boxes.data.size();i++)
    {
        cv::Rect box;
        string classString = "不确定";
        box.x = robot_boxes.data[i].x;
        box.y = robot_boxes.data[i].y;
        box.width = robot_boxes.data[i].width;
        box.height = robot_boxes.data[i].height;
        cv::rectangle(sub_img, box, Scalar(255,0,0), 2);
        if(robot_boxes.data[i].id != 0 || robot_boxes.data[i].id != 9)
        {
            classString = robot_boxes.data[i].id + " "  + robot_boxes.data[i].color;
        }
        else if(robot_boxes.data[i].id == 0 || robot_boxes.data[i].id == 9)
        {
            classString = "shaobin " + robot_boxes.data[i].color;
        }
        else
        {
            classString = "不确定";
        }
        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
        cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);
        cv::rectangle(sub_img, textBox, Scalar(0,0,255), cv::FILLED);
        cv::putText(sub_img, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
    }
    //std::cout << "发送qimage图片" << std::endl;
    /*imshow("Img_Sub::draw_img", this->sub_img);
    waitKey(1);*/
}

void Img_Sub::test()
{
    
}

void Img_Sub::save_img()
{
    imwrite("/home/mechax/zyb/radar_station/src/Img_Handle/demo_picture/2.png", this->sub_img_close);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Img_Sub>());
    rclcpp::shutdown();
    return 0;
}