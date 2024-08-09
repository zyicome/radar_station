#include "img_sub.hpp"

Img_Sub::Img_Sub() : Node("img_sub")
{
    robots_init();

   this->yolo_init();
   img_far_sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>("/image_far/compressed",1,std::bind(&Img_Sub::img_far_callback,this,std::placeholders::_1));
   this->far_yolopoint_pub_ = this->create_publisher<my_msgss::msg::Yolopoint>("/img/far/yolopoint",1);
   this->test_far_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/far_rectangles",1);
   this->far_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/far_rectangles",1);
   this->far_qimage_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/qt/far_qimage",1);

   img_close_sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>("/image_close/compressed",1,std::bind(&Img_Sub::img_close_callback,this,std::placeholders::_1));
   this->close_yolopoint_pub_ = this->create_publisher<my_msgss::msg::Yolopoint>("/img/close/yolopoint",1);
   this->test_close_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/close_rectangles",1);
   this->close_yolopoints_pub_ = this->create_publisher<my_msgss::msg::Yolopoints>("/close_rectangles",1);
   this->close_qimage_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/qt/close_qimage",1);
   RCLCPP_INFO(this->get_logger(), "yolo begin to init");

   mode_sub_ = this->create_subscription<std_msgs::msg::Int8>("/mode",10,std::bind(&Img_Sub::mode_callback,this,std::placeholders::_1));

    test = false;
}

Img_Sub::~Img_Sub()
{

}

//---------------------------------------------------------------
void Img_Sub::kalman_init()
{
    // EKF
  // xa = x_armor, xc = x_robot_center
  // state: xc, v_xc, yc, v_yc, za, v_za, yaw, v_yaw, r
  // measurement: xa, ya, witch, height
  // f - Process function
  auto f = [this](const Eigen::VectorXd & x) {
    Eigen::VectorXd x_new = x;
    x_new(0) += x(1) * dt_;
    x_new(2) += x(3) * dt_;
    x_new(4) += x(5) * dt_;
    x_new(6) += x(7) * dt_;
    return x_new;
  };
  // J_f - Jacobian of process function
  auto j_f = [this](const Eigen::VectorXd &) {
    Eigen::MatrixXd f(9, 9);
    // clang-format off
    f <<  1,   dt_, 0,   0,   0,   0,   0,   0,   0,
          0,   1,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   1,   dt_, 0,   0,   0,   0,   0, 
          0,   0,   0,   1,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   1,   dt_, 0,   0,   0,
          0,   0,   0,   0,   0,   1,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   1,   dt_, 0,
          0,   0,   0,   0,   0,   0,   0,   1,   0,
          0,   0,   0,   0,   0,   0,   0,   0,   1;
    // clang-format on
    return f;
  };
  // h - Observation function
  auto h = [](const Eigen::VectorXd & x) {
    Eigen::VectorXd z(4);
    double xc = x(0), yc = x(2), width = x(6), height = x(8);
    z(0) = xc;  // xa
    z(1) = yc;  // ya
    z(2) = width;               // weight
    z(3) = height;               // height
    return z;
  };
  // J_h - Jacobian of observation function
  auto j_h = [](const Eigen::VectorXd & x) {
    Eigen::MatrixXd h(4, 9);
    double yaw = x(6), r = x(8);
    // clang-format off
    //    xc   v_xc yc   v_yc width v_width height v_height r
    h <<  1,   0,   0,   0,   0,   0,   0,          0,   0,
          0,   0,   1,   0,   0,   0,   0,          0,   0,
          0,   0,   0,   0,   1,   0,   0,          0,   0,
          0,   0,   0,   0,   0,   0,   1,          0,   0;
    // clang-format on
    return h;
  };
  // update_Q - process noise covariance matrix
  s2qxyz_ = declare_parameter("ekf.sigma2_q_xyz", 20.0);
  s2qyaw_ = declare_parameter("ekf.sigma2_q_yaw", 20.0);
  s2qr_ = declare_parameter("ekf.sigma2_q_r", 800.0);
  auto u_q = [this]() {
    Eigen::MatrixXd q(9, 9);
    double t = dt_, x = s2qxyz_, y = s2qyaw_, r = s2qr_;
    double q_x_x = pow(t, 4) / 4 * x, q_x_vx = pow(t, 3) / 2 * x, q_vx_vx = pow(t, 2) * x;
    double q_y_y = pow(t, 4) / 4 * y, q_y_vy = pow(t, 3) / 2 * x, q_vy_vy = pow(t, 2) * y;
    double q_r = pow(t, 4) / 4 * r;
    // clang-format off
    //    xc      v_xc    yc      v_yc    za      v_za    yaw     v_yaw   r
    q <<  q_x_x,  q_x_vx, 0,      0,      0,      0,      0,      0,      0,
          q_x_vx, q_vx_vx,0,      0,      0,      0,      0,      0,      0,
          0,      0,      q_x_x,  q_x_vx, 0,      0,      0,      0,      0,
          0,      0,      q_x_vx, q_vx_vx,0,      0,      0,      0,      0,
          0,      0,      0,      0,      q_x_x,  q_x_vx, 0,      0,      0,
          0,      0,      0,      0,      q_x_vx, q_vx_vx,0,      0,      0,
          0,      0,      0,      0,      0,      0,      q_y_y,  q_y_vy, 0,
          0,      0,      0,      0,      0,      0,      q_y_vy, q_vy_vy,0,
          0,      0,      0,      0,      0,      0,      0,      0,      q_r;
    // clang-format on
    return q;
  };
  // update_R - measurement noise covariance matrix
  r_xyz_factor = declare_parameter("ekf.r_xyz_factor", 0.05);
  r_yaw = declare_parameter("ekf.r_yaw", 0.05);
  auto u_r = [this](const Eigen::VectorXd & z) {
    Eigen::DiagonalMatrix<double, 4> r;
    double x = r_xyz_factor;
    r.diagonal() << abs(x * z[0]), abs(x * z[1]), abs(x * z[2]), r_yaw;
    return r;
  };
  // P - error estimate covariance matrix
  Eigen::DiagonalMatrix<double, 9> p0;
  p0.setIdentity();

  int number = far_robots.size();

  for(int i = 1;i<number;i++)
  {
    far_robots[i].ekf = ExtendedKalmanFilter{f, h, j_f, j_h, u_q, u_r, p0};
    close_robots[i].ekf = ExtendedKalmanFilter{f, h, j_f, j_h, u_q, u_r, p0};
  }
}

//---------------------------------------------------------------

void Img_Sub::img_far_callback(sensor_msgs::msg::CompressedImage msg)
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
        this->far_robot_boxes.data.clear();
        this->far_robot_boxes.id = 0;
        this->far_robot_boxes.text = "none";
        this->yolo_robot_identify(sub_img_far,far_robot_boxes,far_robots,far_inf_robot,far_inf_armors);
        if(far_robot_boxes.data.size() != 0)
        {
            far_yolopoints_pub_->publish(far_robot_boxes);
        }
        if(test)
        {
            my_msgss::msg::Yolopoint test_far_robot_box;
            test_far_robot_box.id = 5;
            test_far_robot_box.x = this->sub_img_far.cols / 2;
            test_far_robot_box.y = this->sub_img_far.rows / 2;
            test_far_robot_box.width = this->sub_img_far.cols / 8;
            test_far_robot_box.height = this->sub_img_far.rows / 8;
            test_far_robot_box.confidence = 0.85;
            my_msgss::msg::Yolopoints test_far_robot_boxes;
            test_far_robot_boxes.data.push_back(test_far_robot_box);
            test_far_yolopoints_pub_->publish(test_far_robot_boxes);
            this->draw_text_to_img(sub_img_far,test_far_robot_boxes);
        }
        this->draw_text_to_img(sub_img_far,far_robot_boxes);

        far_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_far).toImageMsg()));
}

void Img_Sub::img_close_callback(sensor_msgs::msg::CompressedImage msg)
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
        this->close_robot_boxes.data.clear();
        this->close_robot_boxes.id = 0;
        this->close_robot_boxes.text = "none";
        
        this->yolo_robot_identify(sub_img_close,close_robot_boxes,close_robots,close_inf_robot,close_inf_armors);
        if(close_robot_boxes.data.size() != 0)
        {
            RCLCPP_INFO(get_logger(), "began to send farrobot_boxes");
            close_yolopoints_pub_->publish(close_robot_boxes);
        }
        if(test)
        {
            my_msgss::msg::Yolopoint test_close_robot_box;
        test_close_robot_box.id = 5;
        test_close_robot_box.x = this->sub_img_far.cols / 2;
        test_close_robot_box.y = this->sub_img_far.rows / 2;
        test_close_robot_box.width = this->sub_img_far.cols / 8;
        test_close_robot_box.height = this->sub_img_far.rows / 8;
        test_close_robot_box.confidence = 0.85;
        my_msgss::msg::Yolopoints test_close_robot_boxes;
        test_close_robot_boxes.data.push_back(test_close_robot_box);
        test_close_yolopoints_pub_->publish(test_close_robot_boxes);
        this->draw_text_to_img(sub_img_close,test_close_robot_boxes);
        }
        this->draw_text_to_img(sub_img_close,close_robot_boxes);
        close_qimage_pub_->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", this->sub_img_close).toImageMsg()));
}

void Img_Sub::mode_callback(const std_msgs::msg::Int8 msg)
{
    if(msg.data == 1)
    {
        test = true;
    }
    else if(msg.data == 0)
    {
        test = false;
    }
}

void Img_Sub::yolo_init()
{
    //-------------------------------------------------------------------------------------------
    //加载无加速神经网络模型，用于无GPU的情况
    /*bool runOnGPU = false;
    auto pkg_path = ament_index_cpp::get_package_share_directory("Img_Handle");

    Inference inf_armors(pkg_path + "/new_model/train2/weights/best.onnx",
                        cv::Size(640, 640),
                        pkg_path + "/new_model/train2/class/armor.txt",
                        runOnGPU);
    Inference inf_robots(pkg_path + "/new_model/train/weights/best.onnx",
                        cv::Size(640, 640),
                        pkg_path + "/new_model/train/class/car.txt",
                        runOnGPU);
    
    this->far_inf_armor = inf_armors;
    this->far_inf_robot = inf_robots;
    this->close_inf_armor = inf_armors;
    this->close_inf_robot = inf_robots;*/
    //-------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------
    //Numberclassifier识别法初始化，小图像识别法，无效已舍弃，可自行升级使用
    /*string model_path = pkg_path + "/Yolov8_weight/new_armor/mlp.onnx";
    string label_path = pkg_path + "/Yolov8_weight/new_armor/label.txt";
    float threshold = 0.2;
    numberclassifier.init(model_path, label_path, threshold);*/
    //-------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------
    //加载cuda加速的神经网络模型
    std::string pkg_path = ament_index_cpp::get_package_share_directory("Img_Handle");

    const std::string wtsName = pkg_path + "/new_model/train/weights/yolo_car_best.wts";
    const std::string engineName = pkg_path + "/new_model/train/weights/yolo_car_best.engine";
    const std::string classes_txt_file = pkg_path + "/new_model/train/class/car.txt";

    const std::string wtsName2= pkg_path + "/new_model/train2/weights/yolo_armor_best.wts";
    const std::string engineName2 = pkg_path + "/new_model/train2/weights/yolo_armor_best.engine";
    const std::string classes_txt_file2 = pkg_path + "/new_model/train2/class/armor.txt";


    far_inf_robot.Inference_cuda_reset(wtsName,engineName,classes_txt_file,1,0.33,0.25);
    close_inf_robot.Inference_cuda_reset(wtsName,engineName,classes_txt_file,1,0.33,0.25);
    far_inf_armors.Inference_cuda_reset(wtsName2,engineName2,classes_txt_file2,12,0.33,0.25);
    close_inf_armors.Inference_cuda_reset(wtsName2,engineName2,classes_txt_file2,12,0.33,0.25);
    //RCLCPP_INFO(this->get_logger(), "begin to init inference_cuda");
    //-------------------------------------------------------------------------------------------
}


//神经网络识别机器人
void Img_Sub::yolo_robot_identify(Mat & sub_img, my_msgss::msg::Yolopoints &robot_boxes, vector<Robot> &robots,Inference_cuda &inf_robot,Inference_cuda &inf_armor)
{
        auto start = std::chrono::steady_clock::now();
        // Inference starts here...
        RCLCPP_INFO(this->get_logger(), "Inference starts here...");
        vector<Detection_output> robot_output = inf_robot.runInferenceCuda(sub_img);
        int detections = robot_output.size();

        // feiyull
        // 这里需要resize下，否则结果不对
        cv::resize(sub_img, sub_img, cv::Size(sub_img.cols, sub_img.rows));

        for (int i = 0; i < detections; ++i)
        {
            Detection_output detection = robot_output[i];

            cv::Rect box = detection.box;
            //边缘处理
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

            cv::rectangle(sub_img, box, Scalar(255,0,0), 2);
            cv::Rect bbox = cv::Rect(box.x + box.width /8, box.y + box.height /2, box.width / 4 * 3, box.height /2);
            cv::rectangle(sub_img, bbox, Scalar(255,0,0), 2);
            this->yolo_armor_identify(sub_img,robots,box,inf_armor,robot_output[i]);
        }
        
        //机器人状态更新，方法一，包含追踪功能
        //-------------------------------------------------------------------------------------------
        //allrobots_adjust(robots,robot_boxes);
        //-------------------------------------------------------------------------------------------

        //机器人状态更新，方法二，不包含追踪功能
        //-------------------------------------------------------------------------------------------
        not_tracking_allrobots_adjust(robots,robot_boxes);
        //-------------------------------------------------------------------------------------------

        auto end = std::chrono::steady_clock::now();
        //RCLCPP_INFO(get_logger(), "Inference took %ld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

}

void Img_Sub::yolo_armor_identify(Mat & sub_img, vector<Robot> &robots, cv::Rect &box, Inference_cuda &inf_armor, Detection_output &robot_output)
{
    //边缘处理
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

    //截取装甲板图像进行识别
    Mat armor_img = sub_img(box);
    resize(armor_img, armor_img, Size(300, 300));

    //-------------------------------------------------------------------------------------------
    // 第一种识别方法--大图像
    vector<Detection_output> armor_output = inf_armor.runInferenceCuda(armor_img);
    resize(armor_img, armor_img, Size(300, 300));
    int detections = armor_output.size();
    for(int j = 0; j < detections; j++)
    {
        if(armor_output[j].confidence < min_accept_confidence)
        {
            continue;
        }
        
        //robots_adjust(armor_output[j],robots);

        not_tracking_robots_adjust(armor_output[j],robots,robot_output);
    }
    //-------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------
    // 第二种识别方法--类识别，小图像（无效，已舍弃，需要请自行升级）
    /*numberclassifier.classify(armor_img);
    int armor_number = stoi(numberclassifier.armor_number);
    if(armor_number <= 8 && armor_number != -1)
    {
        if(armor_number != 0)
            {
                robot_box.id = armor_number;
                robot_box.color = "blue";
            }
        }
        else if(armor_number >= 9 && armor_number<= 17)
        {
            if(armor_number != 9)
            {
                robot_box.id = armor_number;
                robot_box.color = "red";
            }
        }
    else
    {
        robot_box.id = -1;
    }*/
   //-------------------------------------------------------------------------------------------
}

void Img_Sub::draw_text_to_img(Mat & sub_img,my_msgss::msg::Yolopoints &robot_boxes)
{
    for(int i =0 ;i < robot_boxes.data.size() ; i++)
    {
        cv::Rect box;
        string classString = "不确定";
        box.x = robot_boxes.data[i].x;
        box.y = robot_boxes.data[i].y;
        box.width = robot_boxes.data[i].width;
        box.height = robot_boxes.data[i].height;
        cv::rectangle(sub_img, box, Scalar(255,0,0), 2);
        if(robot_boxes.data[i].id < 6)
        {
            classString = std::to_string(robot_boxes.data[i].id) + " blue";
        }
        else if(robot_boxes.data[i].id == 6)
        {
            classString = "蓝方哨兵";
        }
        else if(robot_boxes.data[i].id > 6 && robot_boxes.data[i].id < 12)
        {
            classString = std::to_string(robot_boxes.data[i].id - 6) + " red";
        }
        else if(robot_boxes.data[i].id == 12)
        {
            classString = "红方哨兵";
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

//----------------------------------------------------------------------------
//new place

// 机器人总列表初始化，大致参数设置
void Img_Sub::robots_init()
{
    Robot robot;
    cv::Rect box(0,0,0,0);
    robot.id = -1;
    robot.record_times = 0;
    robot.confidence = 0.0;
    robot.is_continue = false;
    robot.tracking = "not";
    robot.box = box;
    for(int i =0;i<13;i++)
    {
        robot.id++;
        far_robots.push_back(robot);
        close_robots.push_back(robot);
    }

    kalman_init();

    min_accept_confidence = this->declare_parameter("min_accept_confidence", 0.3);
    min_detection_times = this->declare_parameter("min_detection_times", 2);
    max_lost_times = this->declare_parameter("max_lost_times", -50);
    max_tracking_lost_times = this->declare_parameter("max_tracking_lost_times", -25);
    min_iou = this->declare_parameter("min_iou", 0.1);
    max_accept_distance = this->declare_parameter("max_accept_distance", 0.5);

    this->get_parameter("min_accept_confidence",min_accept_confidence);
    min_accept_confidence = this->get_parameter("min_accept_confidence").as_double();
    min_detection_times = this->get_parameter("min_detection_times").as_int();
    max_lost_times = this->get_parameter("max_lost_times").as_int();
    max_tracking_lost_times = -this->get_parameter("max_tracking_lost_times").as_int();
    min_iou = this->get_parameter("min_iou").as_double();
    max_accept_distance = this->get_parameter("max_accept_distance").as_double();
    std::cout << "min_accept_confidence:" << min_accept_confidence << std::endl;
    std::cout << "min_detection_times:" << min_detection_times << std::endl;
    std::cout << "max_lost_times:" << max_lost_times << std::endl;
    std::cout << "max_tracking_lost_times:" << max_tracking_lost_times << std::endl;
    std::cout << "min_iou:" << min_iou << std::endl;
}

// iou匹配法，判断前后两次识别的机器人是否为同一机器人
bool Img_Sub::box_match(const cv::Rect &box, const cv::Rect &new_box)
{
    float x_one = 0.0;
    float x_two = 0.0;
    float y_one = 0.0;
    float y_two = 0.0;
    float area_intersection = 0.0;
    float area_box = 0.0;
    float area_new_box = 0.0;
    float iou = 0.0;

    if((box.x + box.width) <= (new_box.x + new_box.width))
    {
        if((box.x + box.width) >= new_box.x)
        {
            x_one = box.x + box.width;
            if(box.x > new_box.x)
            {
                x_two = box.x;
            }
            else
            {
                x_two = new_box.x;
            }
        }
        else
        {
            return false;
        }
    }
    else if((box.x + box.width) > (new_box.x + new_box.width))
    {
        if(new_box.x + new_box.width >= box.x)
        {
            x_one = new_box.x + new_box.width;
            if(new_box.x >= box.x)
            {
                x_two = new_box.x;
            }
            else
            {
                x_two = box.x;
            }
        }
        else
        {
            return false;
        }
    }

    if((box.y + box.height) <= (new_box.y + new_box.height))
    {
        if((box.y + box.height) >= new_box.y)
        {
            y_one = box.y + box.height;
            if(box.y > new_box.y)
            {
                y_two = box.y;
            }
            else
            {
                y_two = new_box.y;
            }
        }
        else
        {
            return false;
        }
    }
    else if((box.y + box.height) > (new_box.y + new_box.height))
    {
        if(new_box.y + new_box.height >= box.y)
        {
            y_one = new_box.y + new_box.height;
            if(new_box.y >= box.y)
            {
                y_two = new_box.y;
            }
            else
            {
                y_two = box.y;
            }
        }
        else
        {
            return false;
        }
    }

    area_box = box.width * box.height;
    area_new_box = new_box.width * new_box.height;
    area_intersection = (x_one - x_two) * (y_one - y_two);

    iou = area_intersection / (area_box + area_new_box - area_intersection);

    //std::cout << "iou:" << iou << std::endl;
    
    if(iou > min_iou)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Img_Sub::distance_match(const cv::Rect &box, const cv::Rect &new_box)
{
    bool is_matched = false;
    float distance = 0.0;

    distance = sqrt(pow(((box.x + box.width / 2) - (new_box.x + new_box.width/2)),2) + pow(((box.y + box.height / 2)  - (new_box.y + new_box.height / 2)),2));

    if(distance < max_accept_distance)
    {
        is_matched = true;
    }
    else
    {
        is_matched = false;
    }
    return is_matched;
}

// 机器人状态更新，有被神经网络识别到时触发
void Img_Sub::robots_adjust(const Detection_output &armor_output, vector<Robot> &robots, Detection_output &robot_output)
{
    int armor_number = -1;
    float armor_confidence = 0.0;
    cv::Rect box;
    //神经网络分类详见readme.md

    armor_number = std::stoi(armor_output.className);

    armor_confidence = armor_output.confidence;

    box = robot_output.box;

    if(armor_output.confidence <= robots[armor_number].confidence)
    {
        robots[armor_number].is_continue = false;
        robots[armor_number].tracking == "lost";
        return;
    }

    //std::cout << "before update x: " << robots[armor_number].box.x << " y: " << robots[armor_number].box.y << " width: " << robots[armor_number].box.width << " height: " << robots[armor_number].box.height << std::endl;
    if(robots[armor_number].tracking == "not")
    {
        //kalman_init(robots[armor_number]);
        robots[armor_number].box = box;
        initEKF(robots[armor_number]);
    }
    else
    {
        Eigen::VectorXd measurement(4);
        measurement << box.x, box.y, box.width, box.height;
        update(robots[armor_number], measurement);
    }

    //std::cout << "x: " << robots[armor_number].box.x << " y: " << robots[armor_number].box.y << " width: " << robots[armor_number].box.width << " height: " << robots[armor_number].box.height << std::endl;

    //对应到相应机器人并判断状态

    if(robots[armor_number].tracking == "not")
    {
        robots[armor_number].id = armor_number;
        robots[armor_number].record_times = 1;
        robots[armor_number].confidence = armor_confidence;
        robots[armor_number].is_continue = true;
        robots[armor_number].tracking = "detecting";
        robots[armor_number].box = box;
        std::cout << "not->detecting" << std::endl;
    }
    else if(robots[armor_number].tracking == "detecting")
    {
        if(box_match(robots[armor_number].box, box) == true)
        {
            robots[armor_number].record_times += 1;
            robots[armor_number].confidence = armor_confidence;
            robots[armor_number].is_continue = true;
            if(robots[armor_number].record_times >= min_detection_times)
            {
                robots[armor_number].tracking = "tracking";
                std::cout << "detecting++->tracking" << std::endl;
            }
            else
            {
                robots[armor_number].tracking = "detecting";
                std::cout << "detecting++" << std::endl;
            }
            robots[armor_number].box = box;
        }
        else
        {
            robots[armor_number].record_times = 1;
            robots[armor_number].confidence = armor_confidence;
            robots[armor_number].is_continue = true;
            robots[armor_number].tracking = "detecting";
            robots[armor_number].box = box;
            std::cout << "detecting" << std::endl;
        }
    }
    else if(robots[armor_number].tracking == "tracking")
    {
        if(box_match(robots[armor_number].box,box) == true)
        {
            robots[armor_number].confidence = armor_confidence;
            robots[armor_number].is_continue = true;
            robots[armor_number].tracking = "tracking";
            robots[armor_number].box = box;
            std::cout << "tracking" << std::endl;
        }
        else
        {
            robots[armor_number].record_times = 0;
            robots[armor_number].is_continue = false;
            robots[armor_number].tracking = "lost";
            std::cout << "tracking--->losting" << std::endl;
        }
    }
    else if(robots[armor_number].tracking == "lost")
    {
        if(box_match(robots[armor_number].box,box) == true)
        {
            robots[armor_number].record_times = 4;
            robots[armor_number].confidence = armor_confidence;
            robots[armor_number].is_continue = true;
            robots[armor_number].tracking = "detecting";
            robots[armor_number].box = box;
            std::cout << "lost++->detecting" << std::endl;
        }
        else
        {
            robots[armor_number].record_times -= 1;
            robots[armor_number].is_continue = false;
            robots[armor_number].tracking = "lost";
            std::cout << "lost" << std::endl;
        }
    }
}


// 重整更新所有机器人信息，将正在追踪的机器人信息保存发送出去到深度节点处理
// 参数： 机器人总列表    发送的消息类型
void Img_Sub::allrobots_adjust(vector<Robot> &robots, my_msgss::msg::Yolopoints &robot_boxes)
{
    my_msgss::msg::Yolopoint robot_box;
    for(int i = 1;i<robots.size();i++)
    {
        if(robots[i].tracking != "not" && robots[i].tracking != "tracking" && robots[i].is_continue == false)
        {
                robots[i].record_times -= 1;
                if(robots[i].record_times <= max_lost_times)
                {
                    robots[i].tracking = "not";
                    robots[i].confidence = 0.0;
                    std::cout << "become to not" << std::endl;
                }
                else
                {
                    robots[i].tracking = "lost";
                    std::cout << "become to lost" << std::endl;
                }
        }

        if(robots[i].tracking == "tracking" && robots[i].is_continue == false)
        {
            predict(robots[i]);
            robots[i].record_times -= 1;
            if(robots[i].record_times <= max_tracking_lost_times)
            {
                robots[i].tracking = "detecting";
                robots[i].confidence = 0.0;
                std::cout << "become to detecting" << std::endl;
            }
        }

        if(robots[i].tracking == "tracking")
        {
            robot_box.id = robots[i].id;
            robot_box.confidence = robots[i].confidence;
            robot_box.x = robots[i].box.x;
            robot_box.y = robots[i].box.y;
            robot_box.width = robots[i].box.width;
            robot_box.height = robots[i].box.height;

            robot_boxes.data.push_back(robot_box);
        }
        robots[i].is_continue = false;

        if(i == 5)
        {
        std::cout << "robot_record_times" << robots[i].record_times << std::endl;
        }
    }
}

void Img_Sub::initEKF(Robot &robot)
{
    double x = robot.box.x;
    double y = robot.box.y;
    double width = robot.box.width;
    double height = robot.box.height;

    Eigen::VectorXd target_state = Eigen::VectorXd::Zero(9);
    target_state << x, 0, y, 0, width, 0, height, 0, 0;
    robot.ekf.setState(target_state);
}

void Img_Sub::predict(Robot &robot)
{
    // Predict
    Eigen::VectorXd prediction(9);
    prediction = robot.ekf.predict();
    robot.box.x = prediction(0);
    robot.box.y = prediction(2);
    robot.box.width = prediction(4);
    robot.box.height = prediction(6);
}

void Img_Sub::update(Robot &robot, const Eigen::VectorXd & measurement)
{
    // Predict
    Eigen::VectorXd prediction(9);
    prediction = robot.ekf.predict();

    std::cout << "prediction x: " << prediction(0) << " y: " << prediction(2) << " width: " << prediction(4) << " height: " << prediction(6) << std::endl;

    // Update
    Eigen::VectorXd x_post(9);
    x_post = robot.ekf.update(measurement);

    std::cout << "x_post x: " << x_post(0) << " y: " << x_post(2) << " width: " << x_post(4) << " height: " << x_post(6) << std::endl;
    robot.box.x = x_post(0);
    robot.box.y = x_post(2);
    robot.box.width = x_post(4);
    robot.box.height = x_post(6);
}

void Img_Sub::not_tracking_robots_adjust(const Detection_output &armor_output, vector<Robot> &robots, Detection_output &robot_output)
{
    int armor_number = -1;
    float armor_confidence = 0.0;
    cv::Rect box;
    //神经网络分类详见readme.md

    armor_number = std::stoi(armor_output.className);

    armor_confidence = armor_output.confidence;

    box = robot_output.box;

    robots[armor_number].confidence = armor_confidence;
    robots[armor_number].is_continue = true;
    robots[armor_number].box = box;

}

void Img_Sub::not_tracking_allrobots_adjust(vector<Robot> &robots, my_msgss::msg::Yolopoints &robot_boxes)
{
    my_msgss::msg::Yolopoint robot_box;
    for(int i = 1;i<robots.size();i++)
    {
        if(robots[i].is_continue == false)
        {
            continue;
        }
        //std::cout << "publish robot: " << i << std::endl;
        robot_box.id = robots[i].id;
        robot_box.confidence = robots[i].confidence;
        robot_box.x = robots[i].box.x;
        robot_box.y = robots[i].box.y;
        robot_box.width = robots[i].box.width;
        robot_box.height = robots[i].box.height;

        robot_boxes.data.push_back(robot_box);

        robots[i].is_continue = false;
    }
}

//-----------------------------------------------------------------------------