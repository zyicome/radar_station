//
// Created by dovejh on 2022/3/19.
//
#include "get_depth_node.hpp"
#include <my_msgss/msg/detail/distpoint__struct.hpp>
#include <my_msgss/msg/detail/distpoints__struct.hpp>
#include <sensor_msgs/msg/detail/point_cloud2__struct.hpp>
using namespace std;
using namespace cv;

// uint16_t times = 0;
// std::vector<int> cnt;
// std::vector<double> dists;
// int imgRows = 1024, imgCols = 1280;
// int length_of_cloud_queue = 5;//default length is 5
// int post_pub_flag = 0;
// Point2f outpost_point;
// pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
// std::vector<cv::Mat> far_depth_queue;
// cv::Mat far_camera_matrix = cv::Mat_<double>(3, 3);//相机内参矩阵
// cv::Mat far_uni_matrix = cv::Mat_<double>(3, 4);//相机和雷达的变换矩阵
// cv::Mat far_distortion_coefficient = cv::Mat_<double>(5, 1);
// std::vector<cv::Mat> close_depth_queue;
// cv::Mat close_camera_matrix = cv::Mat_<double>(3, 3);//相机内参矩阵
// cv::Mat close_uni_matrix = cv::Mat_<double>(3, 4);//相机和雷达的变换矩阵
// cv::Mat close_distortion_coefficient = cv::Mat_<double>(5, 1);



// typedef struct {
//     double Last_P;//上次估算协方差 不可以为0 ! ! ! ! !
//     double Now_P;//当前估算协方差
//     double out;//卡尔曼滤波器输出
//     double Kg;//卡尔曼增益
//     double Q;//过程噪声协方差
//     double R;//观测噪声协方差
// } ;

//  Kalman;
// Kalman kfp;

// void GetDepth::Kalman_Init() {
//     kfp.Last_P = 1;
//     kfp.Now_P = 0;
//     kfp.out = 0;
//     kfp.Kg = 0;
//     kfp.Q = 0;
//     kfp.R = 0.01;
// }

// double GetDepth::my_KalmanFilter(Kalman *kfp, double input) {
//     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
//     kfp->Now_P = kfp->Last_P + kfp->Q;
//     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
//     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
//     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
//     kfp->out = kfp->out + kfp->Kg * (input - kfp->out);//因为这一次的预测值就是上一次的输出值
//     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
//     kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
//     return kfp->out;
// }

GetDepth::GetDepth() : Node("GetDepth_node",rclcpp::NodeOptions().allow_undeclared_parameters(true))
{
    robots_init();

    times = 0;
    imgRows = 1024;
    imgCols = 1280;
    length_of_cloud_queue = 5;//default length is 5
    post_pub_flag = 0;
    far_camera_matrix = cv::Mat::zeros(3, 3, CV_64FC1);//相机内参矩阵
    far_uni_matrix = cv::Mat::zeros(3, 4, CV_64FC1);//相机和雷达的变换矩阵
    far_distortion_coefficient = cv::Mat::zeros(5, 1, CV_64FC1);
    close_camera_matrix = cv::Mat::zeros(3, 3, CV_64FC1);//相机内参矩阵
    close_uni_matrix = cv::Mat::zeros(3, 4, CV_64FC1);//相机和雷达的变换矩阵
    close_distortion_coefficient = cv::Mat::zeros(5, 1, CV_64FC1);   
    //this->init_camera_matrix();

    cloud_sub = this->create_subscription<sensor_msgs::msg::PointCloud2>("/livox/lidar", 10, std::bind(&GetDepth::pointCloudCallback, this, std::placeholders::_1));

    far_image_sub = this->create_subscription<sensor_msgs::msg::Image>("/qt/far_qimage", 10, std::bind(&GetDepth::farImageCallback, this, std::placeholders::_1));
    close_image_sub = this->create_subscription<sensor_msgs::msg::Image>("/qt/close_qimage", 10, std::bind(&GetDepth::closeImageCallback, this, std::placeholders::_1));

    far_yolo_sub = this->create_subscription<my_msgss::msg::Yolopoints>("/far_rectangles", 1, std::bind(&GetDepth::far_yoloCallback, this, std::placeholders::_1));
    far_distancePointPub = this->create_publisher<my_msgss::msg::Distpoints>("/sensor_far/distance_point", 10);
    far_depth_qimage_pub = this->create_publisher<sensor_msgs::msg::CompressedImage>("/qt/fardepth_qimage", 1);
    
    close_yolo_sub = this->create_subscription<my_msgss::msg::Yolopoints>("/close_rectangles", 1, std::bind(&GetDepth::close_yoloCallback, this, std::placeholders::_1));
    close_distancePointPub = this->create_publisher<my_msgss::msg::Distpoints>("/sensor_close/distance_point", 10);
    close_depth_qimage_pub = this->create_publisher<sensor_msgs::msg::CompressedImage>("/qt/closedepth_qimage", 1);

    outpost_distancePointPub = this->create_publisher<my_msgss::msg::Distpoint>("sensor_far/outpost", 10);
    outpost_Sub = this->create_subscription<my_msgss::msg::Points>("/sensor_far/calibration", 1, std::bind(&GetDepth::outpost_Callback, this, std::placeholders::_1));

    calibration_result_sub = this->create_subscription<std_msgs::msg::Float64MultiArray>("/calibration_result", 10, std::bind(&GetDepth::calibration_result_Callback, this, std::placeholders::_1));

    paramClient = std::make_shared<rclcpp::SyncParametersClient>(this,"parameter_server");
}

bool GetDepth::is_connect_to_server()
{
    while(!paramClient->wait_for_service(1s))
    {
        if(!rclcpp::ok())
        {
            return false;
        }
        RCLCPP_INFO(this->get_logger(),"服务未连接");
    }
    return true;
}

void GetDepth::init_camera_matrix()
{
    far_camera_matrix.at<double>(0, 0) = paramClient->get_parameter<double>("far_camera_matrix_one");
    far_camera_matrix.at<double>(0, 1) = paramClient->get_parameter<double>("far_camera_matrix_two");
    far_camera_matrix.at<double>(0, 2) = paramClient->get_parameter<double>("far_camera_matrix_three");
    far_camera_matrix.at<double>(1, 0) = paramClient->get_parameter<double>("far_camera_matrix_four");
    far_camera_matrix.at<double>(1, 1) = paramClient->get_parameter<double>("far_camera_matrix_five");
    far_camera_matrix.at<double>(1, 2) = paramClient->get_parameter<double>("far_camera_matrix_six");
    far_camera_matrix.at<double>(2, 0) = paramClient->get_parameter<double>("far_camera_matrix_seven");
    far_camera_matrix.at<double>(2, 1) = paramClient->get_parameter<double>("far_camera_matrix_eight");
    far_camera_matrix.at<double>(2, 2) = paramClient->get_parameter<double>("far_camera_matrix_nine");

    far_distortion_coefficient.at<double>(0,0) = paramClient->get_parameter<double>("far_distortion_coefficient_one");
    far_distortion_coefficient.at<double>(1,0) = paramClient->get_parameter<double>("far_distortion_coefficient_two");
    far_distortion_coefficient.at<double>(2,0) = paramClient->get_parameter<double>("far_distortion_coefficient_three");
    far_distortion_coefficient.at<double>(3,0) = paramClient->get_parameter<double>("far_distortion_coefficient_four");
    far_distortion_coefficient.at<double>(4,0) = paramClient->get_parameter<double>("far_distortion_coefficient_five");

    far_uni_matrix.at<double>(0, 0) = paramClient->get_parameter<double>("far_uni_matrix_one");
    far_uni_matrix.at<double>(0, 1) = paramClient->get_parameter<double>("far_uni_matrix_two");
    far_uni_matrix.at<double>(0, 2) = paramClient->get_parameter<double>("far_uni_matrix_three");
    far_uni_matrix.at<double>(0, 3) = paramClient->get_parameter<double>("far_uni_matrix_four");
    far_uni_matrix.at<double>(1, 0) = paramClient->get_parameter<double>("far_uni_matrix_five");
    far_uni_matrix.at<double>(1, 1) = paramClient->get_parameter<double>("far_uni_matrix_six");
    far_uni_matrix.at<double>(1, 2) = paramClient->get_parameter<double>("far_uni_matrix_seven");
    far_uni_matrix.at<double>(1, 3) = paramClient->get_parameter<double>("far_uni_matrix_eight");
    far_uni_matrix.at<double>(2, 0) = paramClient->get_parameter<double>("far_uni_matrix_nine");
    far_uni_matrix.at<double>(2, 1) = paramClient->get_parameter<double>("far_uni_matrix_ten");
    far_uni_matrix.at<double>(2, 2) = paramClient->get_parameter<double>("far_uni_matrix_eleven");
    far_uni_matrix.at<double>(2, 3) = paramClient->get_parameter<double>("far_uni_matrix_twelve");

    close_camera_matrix.at<double>(0, 0) = paramClient->get_parameter<double>("close_camera_matrix_one");
    close_camera_matrix.at<double>(0, 1) = paramClient->get_parameter<double>("close_camera_matrix_two");
    close_camera_matrix.at<double>(0, 2) = paramClient->get_parameter<double>("close_camera_matrix_three");
    close_camera_matrix.at<double>(1, 0) = paramClient->get_parameter<double>("close_camera_matrix_four");
    close_camera_matrix.at<double>(1, 1) = paramClient->get_parameter<double>("close_camera_matrix_five");
    close_camera_matrix.at<double>(1, 2) = paramClient->get_parameter<double>("close_camera_matrix_six");
    close_camera_matrix.at<double>(2, 0) = paramClient->get_parameter<double>("close_camera_matrix_seven");
    close_camera_matrix.at<double>(2, 1) = paramClient->get_parameter<double>("close_camera_matrix_eight");
    close_camera_matrix.at<double>(2, 2) = paramClient->get_parameter<double>("close_camera_matrix_nine");

    close_distortion_coefficient.at<double>(0,0) = paramClient->get_parameter<double>("close_distortion_coefficient_one");
    close_distortion_coefficient.at<double>(1,0) = paramClient->get_parameter<double>("close_distortion_coefficient_two");
    close_distortion_coefficient.at<double>(2,0) = paramClient->get_parameter<double>("close_distortion_coefficient_three");
    close_distortion_coefficient.at<double>(3,0) = paramClient->get_parameter<double>("close_distortion_coefficient_four");
    close_distortion_coefficient.at<double>(4,0) = paramClient->get_parameter<double>("close_distortion_coefficient_five");

    close_uni_matrix.at<double>(0, 0) = paramClient->get_parameter<double>("close_uni_matrix_one");
    close_uni_matrix.at<double>(0, 1) = paramClient->get_parameter<double>("close_uni_matrix_two");
    close_uni_matrix.at<double>(0, 2) = paramClient->get_parameter<double>("close_uni_matrix_three");
    close_uni_matrix.at<double>(0, 3) = paramClient->get_parameter<double>("close_uni_matrix_four");
    close_uni_matrix.at<double>(1, 0) = paramClient->get_parameter<double>("close_uni_matrix_five");
    close_uni_matrix.at<double>(1, 1) = paramClient->get_parameter<double>("close_uni_matrix_six");
    close_uni_matrix.at<double>(1, 2) = paramClient->get_parameter<double>("close_uni_matrix_seven");
    close_uni_matrix.at<double>(1, 3) = paramClient->get_parameter<double>("close_uni_matrix_eight");
    close_uni_matrix.at<double>(2, 0) = paramClient->get_parameter<double>("close_uni_matrix_nine");
    close_uni_matrix.at<double>(2, 1) = paramClient->get_parameter<double>("close_uni_matrix_ten");
    close_uni_matrix.at<double>(2, 2) = paramClient->get_parameter<double>("close_uni_matrix_eleven");
    close_uni_matrix.at<double>(2, 3) = paramClient->get_parameter<double>("close_uni_matrix_twelve");

    Mat far_R = Mat::zeros(3, 3, CV_64FC1);
    Mat far_Rjacob = Mat::zeros(3, 1, CV_64FC1);
    far_Rjacob.at<double>(0, 0) = 0.02;
    far_Rjacob.at<double>(1, 0) = -0.09;
    far_Rjacob.at<double>(2, 0) = 0.06;
    Rodrigues(far_Rjacob, far_R);
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            std::cout << "far_R[" << i << "][" << j << "]:" << far_R.at<double>(i, j) << std::endl;
        }
    }
}

void GetDepth::write_csv(std::string filename, std::vector<double> vals) {
    // Create an output filestream object
    ofstream myFile(filename);
    // Send data to the stream
    for (double val: vals) {
        myFile << val << "\n";
    }
    // Close the file
    myFile.close();
};

/**
 * 将点云合并成矩阵，方便一同运算
 * @param input 输入点云
 * @return 输出矩阵
 */
cv::Mat GetDepth::Cloud2Mat(const pcl::PointCloud<pcl::PointXYZ>::Ptr &input) {
    cv::Mat res = cv::Mat::zeros(4, (int) input->size(), CV_64F);
    for (int k = 0; k < res.cols; k++) {
        for (int j = 0; j < 4; j++) {
            res.at<double>(j, k) = input->points[k].data[j];
        }
    }
    return res;
};

/**
 * 用于将点云拼成的矩阵变换至像素坐标
 * @param input_depth 输入的点云矩阵
 * @param input_uv 输出
 * @param Cam_matrix 相机内参
 * @param Uni_matrix 相机外参
 */
void GetDepth::MatProject(cv::Mat &input_depth, cv::Mat &input_uv, cv::Mat &Cam_matrix, cv::Mat &Uni_matrix) {
    cv::Mat res = Cam_matrix * Uni_matrix * input_uv;
    for (int i = 0; i < res.cols; i++) {
        int x = round(res.at<double>(0, i) / res.at<double>(2, i));
        int y = round(res.at<double>(1, i) / res.at<double>(2, i));
        if (x >= 0 && x < imgCols && y >= 0 && y < imgRows) {
            input_depth.at<double>(y, x) = res.at<double>(2, i);
        }
    }
    //RCLCPP_INFO(get_logger(), "MatProject:");
};

/**
 * 用于在两帧图像中匹配同一目标，防止z值突变的情况发生
 * @param last_frame 上一帧图像中目标的坐标
 * @param this_frame 这一帧中目标的坐标
 */
void GetDepth::frame_point_match(const my_msgss::msg::Distpoints &last_frame, my_msgss::msg::Distpoints &this_frame) {
    uint8_t match_suc_flag = 0;
    for (int k = 0; k < this_frame.data.size(); k++) {
        for (auto &i: last_frame.data) {
            if ((i.x - this_frame.data[k].x > -50 && i.x - this_frame.data[k].x < 50) &&
                (i.y - this_frame.data[k].y < 50 && i.y - this_frame.data[k].y > -50)) {
                this_frame.data[k].lastdist = i.dist;
                match_suc_flag = 1;
            }
        }
        if (match_suc_flag == 0)this_frame.data[k].lastdist = this_frame.data[k].dist;
        match_suc_flag = 0;
    }
};

//update the car_rects
void GetDepth::far_yoloCallback(const my_msgss::msg::Yolopoints &input) {
    //RCLCPP_INFO(get_logger(), "BEGIN far_yoloCallback");
    cv::Mat far_depthes = cv::Mat::zeros(imgRows, imgCols, CV_64FC1);//initialize the depth img 用于运算的深度图
    cv::Mat far_depth_show = cv::Mat::zeros(imgRows, imgCols, CV_64FC1); //用于显示的深度图
    std::vector<my_msgss::msg::Distpoint>().swap(far_distance_it.data);
    if (cloud) {
        //RCLCPP_INFO(get_logger(), "have cloud");
        cv::Mat far_MatCloud = Cloud2Mat(cloud);
        MatProject(far_depthes, far_MatCloud, far_camera_matrix, far_uni_matrix);
        far_depthes.copyTo(far_depth_show);
        far_depth_queue.push_back(far_depthes); //调整队列
        if (far_depth_queue.size() == length_of_cloud_queue) {
            far_depth_queue.erase(far_depth_queue.begin());
        }
    }

    get_robots(far_robots, input);
    allrobots_adjust(far_robots);
    //RCLCPP_INFO(get_logger(), "far_yoloCallback");
    for(int i = 0;i<far_robots.size();i++)
    {
        if(far_robots[i].tracking == "tracking" || far_robots[i].tracking == "locking" || far_robots[i].tracking == "tracking_locking")
        {
            my_msgss::msg::Distpoint point_it;
            point_it.id = far_robots[i].id;
            point_it.confidence = far_robots[i].confidence;
            point_it.x = far_robots[i].box.x + far_robots[i].box.width / 2;
            point_it.y = far_robots[i].box.y + far_robots[i].box.height / 2;
            point_it.dist = getDepthInRect(far_robots[i].box, far_depth_queue, far_robots[i].id);
            if(point_it.dist > 10000 || point_it.dist < 0 )
            {
                continue;
            }
            if(far_robots[i].id <= 6)
            {
                point_it.color = "blue";
            }
            else
            {
                point_it.color = "red";
            }
            far_distance_it.data.push_back(point_it);
            rectangle(far_depth_show, far_robots[i].box, Scalar(255, 255, 255), 1);
            putText(far_depth_show, std::to_string(point_it.dist), Point(point_it.x, point_it.y),
                    FONT_HERSHEY_COMPLEX_SMALL, 1,
                    Scalar(255, 255, 255), 1, 8, 0);
        }
    }
    far_distancePointPub->publish(far_distance_it);
    resize(far_depth_show, far_depth_show, Size(640, 480));
    far_depth_qimage_pub->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", far_depth_show).toCompressedImageMsg()));
    //cv::Mat far_mixMap = distance_to_image(far_depth_show, far_image);
    //imshow("far_mixMap",far_mixMap);
    imshow("far_depth_show", far_depth_show);
    waitKey(1);
};

//update the car_rects
void GetDepth::close_yoloCallback(const my_msgss::msg::Yolopoints &input) {
    cv::Mat close_depthes = cv::Mat::zeros(imgRows, imgCols, CV_64FC1);//initialize the depth img
    cv::Mat close_depth_show = cv::Mat::zeros(imgRows, imgCols, CV_64FC1);
    std::vector<my_msgss::msg::Distpoint>().swap(last_close_distance_it.data);
    for (auto &i: close_distance_it.data) {
        last_close_distance_it.data.emplace_back(i);
    }
    std::vector<my_msgss::msg::Distpoint>().swap(close_distance_it.data);
    if (cloud) {
        cv::Mat close_MatCloud = Cloud2Mat(cloud);
        MatProject(close_depthes, close_MatCloud, close_camera_matrix, close_uni_matrix);
        close_depthes.copyTo(close_depth_show);
        close_depth_queue.push_back(close_depthes);
        if (close_depth_queue.size() == length_of_cloud_queue) {
            close_depth_queue.erase(close_depth_queue.begin());
        }
    }

    get_robots(close_robots, input);
    allrobots_adjust(close_robots);
    //RCLCPP_INFO(get_logger(), "close_y;++)
    for(int i = 0;i<close_robots.size();i++)
    {
        if(close_robots[i].tracking == "tracking" || close_robots[i].tracking == "locking" || close_robots[i].tracking == "tracking_locking")
        {
            my_msgss::msg::Distpoint point_it;
            point_it.id = close_robots[i].id;
            point_it.confidence = close_robots[i].confidence;
            point_it.x = close_robots[i].box.x + close_robots[i].box.width / 2;
            point_it.y = close_robots[i].box.y + close_robots[i].box.height / 2;
            point_it.dist = getDepthInRect(close_robots[i].box, close_depth_queue, close_robots[i].id);
            if(point_it.dist > 10000 || point_it.dist < 0)
            {
                continue;
            }
            if(close_robots[i].id <= 6)
            {
                point_it.color = "blue";
            }
            else
            {
                point_it.color = "red";
            }
            close_distance_it.data.push_back(point_it);
            rectangle(close_depth_show, close_robots[i].box, Scalar(255, 255, 255), 1);
            putText(close_depth_show, std::to_string(point_it.dist), Point(point_it.x, point_it.y),
                    FONT_HERSHEY_COMPLEX_SMALL, 1,
                    Scalar(255, 255, 255), 1, 8, 0);
        }
    }
    frame_point_match(last_close_distance_it, close_distance_it);
    close_distancePointPub->publish(close_distance_it);
    resize(close_depth_show, close_depth_show, Size(640, 480));
    close_depth_qimage_pub->publish(*(cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", close_depth_show).toCompressedImageMsg()));
    //cv::Mat close_mixMap = distance_to_image(close_depth_show, close_image);
    //imshow("close_mixMap",close_mixMap);
    imshow("close_depth_show", close_depth_show);
    waitKey(1);
};
/**
 * 将受到的点云消息转换成点云
 * @param input 收到的消息
 */

void GetDepth::pointCloudCallback(const sensor_msgs::msg::PointCloud2 &input) 
{
    //RCLCPP_INFO(get_logger(), "begin to get pointcloud");
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(input, pcl_pc2);
    pcl::fromPCLPointCloud2(pcl_pc2, *cloud);   
    this->cloud = cloud;
    //RCLCPP_INFO(get_logger(), "get pointcloud");
};


/**
 * 用于接收前哨站位置消息
 * @param outpost 前哨站位置
 */
void GetDepth::outpost_Callback(const my_msgss::msg::Points &outpost) {
    post_pub_flag = 1;
    outpost_point.x = outpost.data[0].x * 1280;
    outpost_point.y = outpost.data[0].y * 1024;
};

void GetDepth::calibration_result_Callback(const std_msgs::msg::Float64MultiArray &calibration_result) {
    //std::string pkg_path = ament_index_cpp::get_package_share_directory("get_depth");
    std::string pkg_path = "/home/mechax/radar_station/src/get_depth";
    std::string farfilename = pkg_path + "/result/far_uni_matrix.txt";
    std::string closefilename = pkg_path + "/result/close_uni_matrix.txt";

    if(calibration_result.data[12] == 0)
    {
        close_uni_matrix.at<double>(0, 0) = calibration_result.data[0];
        close_uni_matrix.at<double>(0, 1) = calibration_result.data[1];
        close_uni_matrix.at<double>(0, 2) = calibration_result.data[2];
        close_uni_matrix.at<double>(0, 3) = calibration_result.data[3];
        close_uni_matrix.at<double>(1, 0) = calibration_result.data[4];
        close_uni_matrix.at<double>(1, 1) = calibration_result.data[5];
        close_uni_matrix.at<double>(1, 2) = calibration_result.data[6];
        close_uni_matrix.at<double>(1, 3) = calibration_result.data[7];
        close_uni_matrix.at<double>(2, 0) = calibration_result.data[8];
        close_uni_matrix.at<double>(2, 1) = calibration_result.data[9];
        close_uni_matrix.at<double>(2, 2) = calibration_result.data[10];
        close_uni_matrix.at<double>(2, 3) = calibration_result.data[11];
        std::ofstream outfile(closefilename);
        for (int i = 0; i < 3; i++) {
        outfile << close_uni_matrix.at<double>(i, 0) << "," << close_uni_matrix.at<double>(i, 1) << "," << close_uni_matrix.at<double>(i, 2) << "," << close_uni_matrix.at<double>(i, 3)
            << std::endl;
            }
  std::cout << "get_close_uni_matrix_calibration_result" << std::endl;
    for(int i =0;i<3;i++)
    {
        for(int j = 0;j<4;j++)
        {
            std::cout << "close_uni_matrix[" << i << "][" << j << "]:" << close_uni_matrix.at<double>(i, j) << std::endl;
        }
    }
    }
    else
    {
        far_uni_matrix.at<double>(0, 0) = calibration_result.data[0];
        far_uni_matrix.at<double>(0, 1) = calibration_result.data[1];
        far_uni_matrix.at<double>(0, 2) = calibration_result.data[2];
        far_uni_matrix.at<double>(0, 3) = calibration_result.data[3];
        far_uni_matrix.at<double>(1, 0) = calibration_result.data[4];
        far_uni_matrix.at<double>(1, 1) = calibration_result.data[5];
        far_uni_matrix.at<double>(1, 2) = calibration_result.data[6];
        far_uni_matrix.at<double>(1, 3) = calibration_result.data[7];
        far_uni_matrix.at<double>(2, 0) = calibration_result.data[8];
        far_uni_matrix.at<double>(2, 1) = calibration_result.data[9];
        far_uni_matrix.at<double>(2, 2) = calibration_result.data[10];
        far_uni_matrix.at<double>(2, 3) = calibration_result.data[11];
        std::ofstream outfile(farfilename);
        for (int i = 0; i < 3; i++) {
        outfile << far_uni_matrix.at<double>(i, 0) << "," << far_uni_matrix.at<double>(i, 1) << "," << far_uni_matrix.at<double>(i, 2) << "," << far_uni_matrix.at<double>(i, 3)
            << std::endl;
        }
  std::cout << "get_far_uni_matrix_calibration_result" << std::endl;
    for(int i =0;i<3;i++)
    {
        for(int j = 0;j<4;j++)
        {
            std::cout << "far_uni_matrix[" << i << "][" << j << "]:" << far_uni_matrix.at<double>(i, j) << std::endl;
        }
    }
    }
};

void GetDepth::farImageCallback(const sensor_msgs::msg::Image msg)
{
    std::cout << "farImageCallback begin" << std::endl;
    cv_bridge::CvImagePtr far_cv_ptr;
    try
    {
        far_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        if(!far_cv_ptr->image.empty())
        {
            far_image = far_cv_ptr->image;
            cv::resize(far_image,far_image,cv::Size(640,480));
        }
    }
    catch(cv_bridge::Exception &e)
    {
        std::cout << "farImageCallback error" << std::endl;
        return;
    }
}

void GetDepth::closeImageCallback(const sensor_msgs::msg::Image msg)
{
    std::cout << "closeImageCallback begin" << std::endl;
    cv_bridge::CvImagePtr close_cv_ptr;
    try
    {
        close_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        if(!close_cv_ptr->image.empty())
        {
            close_image = close_cv_ptr->image;
            cv::resize(close_image,close_image,cv::Size(640,480));
        }
    }
    catch(cv_bridge::Exception &e)
    {
        std::cout << "farImageCallback error" << std::endl;
        return;
    }
}

/**far_rectangles
 * 从深度图中获取ROI的深度
 * @param rect ROI
 * @param depth_queue 深度图队列
 * @param id 车辆ID
 * @return 深度值
 */
double GetDepth::getDepthInRect(Rect rect, std::vector<cv::Mat> &depth_queue, my_msgss::msg::Yolopoint::_id_type id) {
    std::vector<double> distances;
    rect.x = rect.x + rect.width / 8;
    rect.y = rect.y + rect.height / 2;
    rect.width = rect.width / 4 * 3;
    rect.height = rect.height / 2;
    //从新到旧遍历深度图队列，直到ROI深度不为0
    for (int i = rect.y; i < (rect.y + rect.height); i++) {
        for (int j = rect.x; j < (rect.x + rect.width); j++) {
            for (uint8_t k = 0; k < depth_queue.size(); k++) {
                if (depth_queue[depth_queue.size() - 1].at<double>(i, j) > 0) {
                    distances.push_back(depth_queue[depth_queue.size() - 1].at<double>(i, j));
                    break;
                } else if (k < depth_queue.size() - 1 && depth_queue[k + 1].at<double>(i, j) == 0 &&
                           depth_queue[k].at<double>(i, j) > 0) {
                    distances.push_back(depth_queue[k].at<double>(i, j));
                    break;
                }
            }
        }
    }
    if (distances.empty()) {
        cout << "No Livox Points in ROI" << rect << endl;
        return 0;
    } else {
        sort(distances.begin(), distances.end());
        /*for(int i = 0;i<distances.size();i++)
        {
            cout << "distances[" << i << "]:" << distances[i] << endl;
        }*/
        distance_filter(distances);
        //cout << "after distance_filter" << endl;
        /*for(int i = 0;i<distances.size();i++)
        {
            cout << "distances{" << i << "}:" << distances[i] << endl;
        }*/
        double mean_distance;
        double sum = 0;
        //根据不同的策略获取深度
        for (double distance: distances) {
            if(distance > 1000 || distance < 0)
            {
                continue;
            }
            sum += distance;
        }
        if(distances.size() == 0)
        {
            return 0;
        }
        mean_distance = sum / distances.size();
        return mean_distance;
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    // Kalman_Init();
    auto paramClient = std::make_shared<GetDepth>();
    bool flag = paramClient->is_connect_to_server();
    if(!flag){
        return 0;
    } 
    paramClient->init_camera_matrix();
    rclcpp::spin(paramClient);
    rclcpp::shutdown();
    return 0;
}

//-----------------------------------------------------------------------------------------
//new place

void GetDepth::robots_init()
{
    Robot robot;
    cv::Rect box(0,0,0,0);
    robot.id = -1;
    robot.record_times = 0;
    robot.confidence = 0.0;
    robot.is_continue = false;
    robot.tracking = "not";
    robot.box = box;
    for(int i =0;i<19;i++)
    {
        robot.id++;
        far_robots.push_back(robot);
        close_robots.push_back(robot);
        robots.push_back(robot);
    }

    min_iou = 0.5;
}

// iou匹配法，判断前后两次识别的机器人是否为同一机器人
bool GetDepth::box_match(const cv::Rect &box, const cv::Rect &new_box,const double & min_iou)
{
    double x_one = 0.0;
    double x_two = 0.0;
    double y_one = 0.0;
    double y_two = 0.0;
    double area_intersection = 0.0;
    double area_box = 0.0;
    double area_new_box = 0.0;
    double iou = 0.0;

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

    if(iou > min_iou)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GetDepth::get_robots(std::vector<Robot> &robots, const my_msgss::msg::Yolopoints &input)
{
    int armor_number = 0;
    bool iou_matched = false;
    
    for(int i = 0;i<input.data.size();i++)
    {
        armor_number = input.data[i].id;
        iou_matched = box_match(robots[armor_number].box, cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height), min_iou);
        if(robots[armor_number].tracking == "not")
        {
            robots[armor_number].tracking = "tracking";
            robots[armor_number].confidence = input.data[i].confidence;
            robots[armor_number].is_continue = true;
            robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height);
        }
        else if(robots[armor_number].tracking == "tracking")
        {
            if(iou_matched)
            {
                robots[armor_number].tracking = "tracking";
                robots[armor_number].confidence = input.data[i].confidence;
                robots[armor_number].is_continue = true;
                robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height);
            }
            else
            {
                if(input.data[i].confidence > robots[armor_number].confidence)
                {
                    robots[armor_number].tracking = "tracking";
                    robots[armor_number].confidence = input.data[i].confidence;
                    robots[armor_number].is_continue = true;
                    robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height);
                }
                else
                {
                    robots[armor_number].tracking = "not";
                    robots[armor_number].confidence = 0.0;
                    robots[armor_number].is_continue = false;
                    robots[armor_number].box = cv::Rect(0,0,0,0);
                }
            }
        }
        else if(robots[armor_number].tracking == "locking")
        {
            robots[armor_number].tracking = "tracking_locking";
            robots[armor_number].confidence = input.data[i].confidence;
            robots[armor_number].is_continue = true;
            robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height);
        }
        else if(robots[armor_number].tracking == "tracking_locking")
        {
            if(iou_matched)
            {
                robots[armor_number].tracking = "tracking_locking";
                robots[armor_number].confidence = input.data[i].confidence;
                robots[armor_number].is_continue = true;
                robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width,input.data[i].height);
            }
            else
            {
                if(input.data[i].confidence > robots[armor_number].confidence)
                {
                    robots[armor_number].tracking = "tracking_locking";
                    robots[armor_number].confidence = input.data[i].confidence;
                    robots[armor_number].is_continue = true;
                    robots[armor_number].box = cv::Rect(input.data[i].x, input.data[i].y, input.data[i].width, input.data[i].height);
                }
                else
                {
                    robots[armor_number].tracking = "locking";
                    robots[armor_number].confidence = robots[armor_number].confidence;
                    robots[armor_number].is_continue = false;
                    robots[armor_number].box = cv::Rect(0,0,0,0);
                }
            }
        }
    }
}

void GetDepth::allrobots_adjust(std::vector<Robot> &robots)
{
    for(int i = 0;i<robots.size();i++)
    {
        if(robots[i].is_continue == false && robots[i].tracking == "tracking")
        {
            robots[i].tracking = "not";
            robots[i].confidence = 0.0;
            robots[i].is_continue = false;
            robots[i].box = cv::Rect(0,0,0,0);
        }
        else if(robots[i].is_continue == false && robots[i].tracking == "tracking_locking")
        {
            robots[i].tracking = "locking";
            robots[i].confidence = robots[i].confidence;
            robots[i].is_continue = false;
            robots[i].box = cv::Rect(0,0,0,0);
        }

        robots[i].is_continue = false;
    }
}

// 差雷达追踪功能的添加
//--------------------------------------------------------------------------------------------------------------------------
void GetDepth::distance_filter(std::vector<double> & distances)
{
    double sum = 0.0;
    double mean_distance = 0.0;
    std::vector<double> save_distances = distances;
    for(int i = 0;i < distances.size();i++)
    {
        sum += distances[i];
    }
    mean_distance = sum / distances.size();
    for(int i = 0;i < distances.size();i++)
    {
        if(abs(distances[i] - mean_distance) > 0.5)
        {
            distances.erase(distances.begin() + i);
            i--;
        }
    }
    if(distances.size() ==0)
    {
        sort(save_distances.begin(), save_distances.end());
        for(int i = save_distances.size() / 2 - 5;i < save_distances.size() / 2 + 5;i++)
        {
            distances.push_back(save_distances[i]);
        }
    }
}

cv::Mat GetDepth::distance_to_image(cv::Mat depth, cv::Mat image)
{
    // double min = 0.0;
    // double max = 0.0;
    // cv::minMaxIdx(depth, &min, &max);
    // cv::Mat adjMap;
    // cv::convertScaleAbs(depth, adjMap, 255 / max);
    // cv::applyColorMap(adjMap, image, cv::COLORMAP_JET);
    // return adjMap;

    std::cout << "depth.rows:" << depth.rows << std::endl;
    std::cout << "depth.cols:" << depth.cols << std::endl;
    std::cout << "image.rows:" << image.rows << std::endl;
    std::cout << "image.cols:" << image.cols << std::endl;
    cv::Mat adjMap = cv::Mat::zeros(depth.rows, depth.cols, CV_8UC3);
    adjMap = image.clone();
    for(int i = 0;i<depth.rows;i++)
    {
        for(int j = 0;j<depth.cols;j++)
        {
            /*if(depth.at<double>(i, j) == 0)
            {
                adjMap.at<cv::Vec3b>(i, j)[0] = 255;
                adjMap.at<cv::Vec3b>(i, j)[1] = 0;
                adjMap.at<cv::Vec3b>(i, j)[2] = 0;
            }*/
            if(depth.at<double>(i, j) > 0)
            {
                adjMap.at<cv::Vec3b>(i, j)[0] = 255;
                adjMap.at<cv::Vec3b>(i, j)[1] = 0;
                adjMap.at<cv::Vec3b>(i, j)[2] = 0;
            }
        }
    }
    return adjMap;
}