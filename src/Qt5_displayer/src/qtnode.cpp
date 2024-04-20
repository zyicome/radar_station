#include "qtnode.h"

qtNode::qtNode()
{
    this->start();
}

qtNode::~qtNode()
{
    
}

void qtNode::farImageCallback(const sensor_msgs::msg::CompressedImage msg)
{
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

void qtNode::closeImageCallback(const sensor_msgs::msg::CompressedImage msg)
{
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

void qtNode::run()
{   
    cout << "node开始运行" << endl;
    rclcpp::init(0, nullptr);
    qnode = std::make_shared<rclcpp::Node>("qt_node");

    pnp_pub_ = qnode->create_publisher<std_msgs::msg::Float32MultiArray>("/qt/pnp", 10);

    far_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/far_qimage", 1, std::bind(&qtNode::farImageCallback, this, std::placeholders::_1));
    fardepth_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/fardepth_qimage", 1, std::bind(&qtNode::farDepthImageCallback, this, std::placeholders::_1));
    farpoints_sub_ = qnode->create_subscription<my_msgss::msg::Points>("/qt/farpoints", 10, std::bind(&qtNode::farPointsCallback, this, std::placeholders::_1));

    close_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/close_qimage", 1, std::bind(&qtNode::closeImageCallback, this, std::placeholders::_1));
    closedepth_sub_ = qnode->create_subscription<sensor_msgs::msg::CompressedImage>("/qt/closedepth_qimage", 1, std::bind(&qtNode::closeDepthImageCallback, this, std::placeholders::_1));
    closepoints_sub_ = qnode->create_subscription<my_msgss::msg::Points>("/qt/closepoints", 10, std::bind(&qtNode::closePointsCallback, this, std::placeholders::_1));

    rclcpp::spin(qnode);
    cout << "node异常关闭" << endl;
    rclcpp::shutdown();
}