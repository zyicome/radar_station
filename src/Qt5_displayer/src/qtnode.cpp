#include "qtnode.h"

qtNode::qtNode()
{
    this->start();

    far_start_time = std::chrono::high_resolution_clock::now();
    far_end_time = std::chrono::high_resolution_clock::now();
    close_start_time = std::chrono::high_resolution_clock::now();
    close_end_time = std::chrono::high_resolution_clock::now();
}

qtNode::~qtNode()
{
    
}

bool qtNode::is_connect_to_server()
{
    while(!paramClient->wait_for_service(1s))
    {
        if(!rclcpp::ok())
        {
            return false;
        }
        RCLCPP_INFO(qnode->get_logger(),"服务未连接");
    }
    return true;
}

void qtNode::client_parameter_init()
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
}

void qtNode::farImageCallback(const sensor_msgs::msg::Image msg)
{
    /*far_end_time = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(far_end_time - far_start_time).count() < 5000)
    {
        return;
    }
    far_start_time = std::chrono::high_resolution_clock::now();*/
    cv_bridge::CvImagePtr far_cv_ptr;
    try
    {
        far_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        if(!far_cv_ptr->image.empty())
        {
            Mat far_image = far_cv_ptr->image;
            cv::resize(far_image,far_image,cv::Size(FAR_IMAGE_WIDTH,FAR_IMAGE_HEIGHT));
            far_qimage = QImage((const unsigned char*)(far_image.data),far_image.cols,far_image.rows,QImage::Format_BGR888);
        }
        Q_EMIT updateFarImage();
    }
    catch(cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(qnode->get_logger(),"cv_bridge exception: %s",e.what());
        return;
    }
}

/*void qtNode::closeImageCallback(const sensor_msgs::msg::CompressedImage msg)
{
    close_end_time = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(close_end_time - close_start_time).count() < 5000)
    {
        return;
    }
    close_start_time = std::chrono::high_resolution_clock::now();
    cv_bridge::CvImagePtr close_cv_ptr;
    try
    {
        close_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        if(!close_cv_ptr->image.empty())
        {
            Mat close_image = close_cv_ptr->image;
            cv::resize(close_image,close_image,cv::Size(FAR_IMAGE_WIDTH,FAR_IMAGE_HEIGHT));
            close_qimage = QImage((const unsigned char*)(close_image.data),close_image.cols,close_image.rows,QImage::Format_BGR888);
        }
        Q_EMIT updateCloseImage();
    }
    catch(cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(qnode->get_logger(),"cv_bridge exception: %s",e.what());
        return;
    }
}*/

void qtNode::closeImageCallback(const sensor_msgs::msg::Image msg)
{
    /*close_end_time = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(close_end_time - close_start_time).count() < 5000)
    {
        return;
    }
    close_start_time = std::chrono::high_resolution_clock::now();*/
    cv_bridge::CvImagePtr close_cv_ptr;
    try
    {
        close_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        if(!close_cv_ptr->image.empty())
        {
            Mat close_image = close_cv_ptr->image;
            cv::resize(close_image,close_image,cv::Size(FAR_IMAGE_WIDTH,FAR_IMAGE_HEIGHT));
            close_qimage = QImage((const unsigned char*)(close_image.data),close_image.cols,close_image.rows,QImage::Format_BGR888);
        }
        Q_EMIT updateCloseImage();
    }
    catch(cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(qnode->get_logger(),"cv_bridge exception: %s",e.what());
        return;
    }
}

void qtNode::farDepthImageCallback(const sensor_msgs::msg::CompressedImage msg)
{
    cv_bridge::CvImagePtr far_depth_cv_ptr;
    try
    {
        far_depth_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::TYPE_32FC1);
        if(!far_depth_cv_ptr->image.empty())
        {
            Mat fardepth_image = far_depth_cv_ptr->image;
            cv::resize(fardepth_image,fardepth_image,cv::Size(DEPTH_IMAGE_WIDTH,DEPTH_IMAGE_HEIGHT));
            Mat fardepth_image_8u;
            fardepth_image.convertTo(fardepth_image_8u,CV_8UC1,255.0/10.0);
            Mat fardepth_bgrimage;
            applyColorMap(fardepth_image_8u, fardepth_bgrimage, cv::COLORMAP_JET);
            fardepth_qimage = QImage((const unsigned char*)(fardepth_bgrimage.data),fardepth_bgrimage.cols,fardepth_bgrimage.rows,QImage::Format_BGR888);
            //depth_qimage = QImage((const unsigned char*)(depth_image.data),depth_image.cols,depth_image.rows,QImage::Format_Grayscale8);
        }
        Q_EMIT updateFarDepthImage();
    }
    catch(cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(qnode->get_logger(),"cv_bridge exception: %s",e.what());
        return;
    }
}

void qtNode::closeDepthImageCallback(const sensor_msgs::msg::CompressedImage msg)
{
    cv_bridge::CvImagePtr close_depth_cv_ptr;
    try
    {
        close_depth_cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::TYPE_32FC1);
        if(!close_depth_cv_ptr->image.empty())
        {
            Mat closedepth_image = close_depth_cv_ptr->image;
            cv::resize(closedepth_image,closedepth_image,cv::Size(DEPTH_IMAGE_WIDTH,DEPTH_IMAGE_HEIGHT));
            Mat closedepth_image_8u;
            closedepth_image.convertTo(closedepth_image_8u,CV_8UC1,255.0/10.0);
            Mat closedepth_bgrimage;
            applyColorMap(closedepth_image_8u, closedepth_bgrimage, cv::COLORMAP_JET);
            closedepth_qimage = QImage((const unsigned char*)(closedepth_bgrimage.data),closedepth_bgrimage.cols,closedepth_bgrimage.rows,QImage::Format_BGR888);
            //depth_qimage = QImage((const unsigned char*)(depth_image.data),depth_image.cols,depth_image.rows,QImage::Format_Grayscale8);
        }
        Q_EMIT updateCloseDepthImage();
    }
    catch(cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(qnode->get_logger(),"cv_bridge exception: %s",e.what());
        return;
    }
}

void qtNode::farPointsCallback(const my_msgss::msg::Points msg)
{
    far_world_qpoints = msg;
    
    Q_EMIT updateFarPoints();
}

void qtNode::closePointsCallback(const my_msgss::msg::Points msg)
{
    close_world_qpoints = msg;

    Q_EMIT updateClosePoints();
}

void qtNode::gameStateCallback(const my_msgss::msg::Gamestate msg)
{
    game_state_msg = msg;
    
    Q_EMIT updateGameState();
}

void qtNode::radarMarkCallback(const my_msgss::msg::Radarmark msg)
{
    radar_mark_msg = msg;

    Q_EMIT updateRadarMark();
}

void qtNode::radarInfoCallback(const my_msgss::msg::Radarinfo msg)
{
    radar_info_msg = msg;
    
    Q_EMIT updateRadarInfo();
}

void qtNode::hpCallback(const my_msgss::msg::Hp msg)
{
    hp_msg = msg;

    Q_EMIT updateHp();
}

void qtNode::dartCallback(const my_msgss::msg::Dart msg)
{
    dart_msg = msg;

    Q_EMIT updateDart();
}

void qtNode::run()
{   
    cout << "node开始运行" << endl;
    rclcpp::init(0, nullptr);
    qnode = std::make_shared<rclcpp::Node>("qt_node",rclcpp::NodeOptions().allow_undeclared_parameters(true));

    paramClient = std::make_shared<rclcpp::SyncParametersClient>(qnode,"parameter_server");

    bool flag = is_connect_to_server();
    if(!flag)
    {
        cout << "node未连接服务器而关闭" << endl;
        return;
    }
    client_parameter_init();

    pnp_pub_ = qnode->create_publisher<std_msgs::msg::Float32MultiArray>("/qt/pnp", 10);
    points_pub_ = qnode->create_publisher<my_msgss::msg::Points>("/serial/world_points", 10);
    color_pub_ = qnode->create_publisher<std_msgs::msg::Int8>("/our_color", 10);
    mode_pub_ = qnode->create_publisher<std_msgs::msg::Int8>("/mode", 10);
    is_save_video_pub_ = qnode->create_publisher<std_msgs::msg::Int8>("/is_begin_to_save", 10);

    radar_info_pub_ = qnode->create_publisher<my_msgss::msg::Radarinfo>("/serial/radar_info", 10);

    far_sub_ = qnode->create_subscription<sensor_msgs::msg::Image>("/qt/far_qimage", 1, std::bind(&qtNode::farImageCallback, this, std::placeholders::_1));
    fardepth_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/fardepth_qimage", 1, std::bind(&qtNode::farDepthImageCallback, this, std::placeholders::_1));
    farpoints_sub_ = qnode->create_subscription<my_msgss::msg::Points>("/qt/farpoints", 10, std::bind(&qtNode::farPointsCallback, this, std::placeholders::_1));

    close_sub_ = qnode->create_subscription<sensor_msgs::msg::Image>("/qt/close_qimage", 1, std::bind(&qtNode::closeImageCallback, this, std::placeholders::_1));
    closedepth_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/closedepth_qimage", 1, std::bind(&qtNode::closeDepthImageCallback, this, std::placeholders::_1));
    closepoints_sub_ = qnode->create_subscription<my_msgss::msg::Points>("/qt/closepoints", 10, std::bind(&qtNode::closePointsCallback, this, std::placeholders::_1));

    game_state_sub_ = qnode->create_subscription<my_msgss::msg::Gamestate>("/game_state", 10, std::bind(&qtNode::gameStateCallback, this, std::placeholders::_1));
    radar_mark_sub_ = qnode->create_subscription<my_msgss::msg::Radarmark>("/radar_mark", 10, std::bind(&qtNode::radarMarkCallback, this, std::placeholders::_1));
    radar_info_sub_ = qnode->create_subscription<my_msgss::msg::Radarinfo>("/radar_info", 10, std::bind(&qtNode::radarInfoCallback, this, std::placeholders::_1));
    hp_sub_ = qnode->create_subscription<my_msgss::msg::Hp>("/hp", 10, std::bind(&qtNode::hpCallback, this, std::placeholders::_1));
    dart_sub_ = qnode->create_subscription<my_msgss::msg::Dart>("/dart", 10, std::bind(&qtNode::dartCallback, this, std::placeholders::_1));

    rclcpp::spin(qnode);
    cout << "node异常关闭" << endl;
    rclcpp::shutdown();
}