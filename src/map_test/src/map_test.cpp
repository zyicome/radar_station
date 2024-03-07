#include "map_test.hpp"
#include "CameraDefine.h"
MapTest::MapTest() : Node("map_test")
{
    parameter_init();

    get_map();
    
    calibration();

    far_distpoints_sub_ = this->create_subscription<my_msgss::msg::Distpoints>("/sensor_far/distance_point",10,
                     std::bind(&MapTest::far_callback,this,std::placeholders::_1));

    close_distpoints_sub_ = this->create_subscription<my_msgss::msg::Distpoints>("/sensor_close/distance_point",10,
                     std::bind(&MapTest::close_callback,this,std::placeholders::_1));
                     
    pnp_sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>("/qt/pnp",10,
                     std::bind(&MapTest::pnp_callback,this,std::placeholders::_1));

    far_points_pub_ = this->create_publisher<my_msgss::msg::Points>("/qt/farpoints", 10);
    close_points_pub_ = this->create_publisher<my_msgss::msg::Points>("/qt/closepoints", 10);
}

void MapTest::parameter_init()
{
    //-------------------------------------
    object_height = 11.5;
    object_width = 8;
    //--------------------------------------
    far_CamMatrix_.at<double>(0, 0) = 1474.62545;
    far_CamMatrix_.at<double>(0, 1) = 0.0;
    far_CamMatrix_.at<double>(0, 2) = 367.67245;
    far_CamMatrix_.at<double>(1, 0) = 0.0;
    far_CamMatrix_.at<double>(1, 1) = 1463.59535;
    far_CamMatrix_.at<double>(1, 2) = 274.85727;
    far_CamMatrix_.at<double>(2, 0) = 0.0;
    far_CamMatrix_.at<double>(2, 1) = 0.0;
    far_CamMatrix_.at<double>(2, 2) = 1.0;
    far_distCoeffs_.at<double>(0, 0) = -0.025299;
    far_distCoeffs_.at<double>(0, 1) = -0.874546;
    far_distCoeffs_.at<double>(0, 2) = -0.000261;
    far_distCoeffs_.at<double>(0, 3) = 0.018625;
    far_distCoeffs_.at<double>(0, 4) = 0.0;

    close_CamMatrix_.at<double>(0, 0) = 1474.62545;
    close_CamMatrix_.at<double>(0, 1) = 0.0;
    close_CamMatrix_.at<double>(0, 2) = 367.67245;
    close_CamMatrix_.at<double>(1, 0) = 0.0;
    close_CamMatrix_.at<double>(1, 1) = 1463.59535;
    close_CamMatrix_.at<double>(1, 2) = 274.85727;
    close_CamMatrix_.at<double>(2, 0) = 0.0;
    close_CamMatrix_.at<double>(2, 1) = 0.0;
    close_CamMatrix_.at<double>(2, 2) = 1.0;
    close_distCoeffs_.at<double>(0, 0) = -0.025299;
    close_distCoeffs_.at<double>(0, 1) = -0.874546;
    close_distCoeffs_.at<double>(0, 2) = -0.000261;
    close_distCoeffs_.at<double>(0, 3) = 0.018625;
    close_distCoeffs_.at<double>(0, 4) = 0.0;
}

void MapTest::get_map()
{
    string map_path = "/home/mechax/AAAmy_ws/1.14/radar_station/src/Game_Map/map/test.png";
    test_map = imread(map_path, IMREAD_COLOR);
    resize(test_map, test_map, Size(600, 800));
    image_height = test_map.rows;
    image_width = test_map.cols;
    cout << "image_height:" << image_height << endl;
    cout << "image_width:" << image_width << endl;
}

void MapTest::calibration()
{

    vector<cv::Point3d> objectPoints;
    vector<cv::Point2d> imagePoints;

    Point3d objectpoint_one;
    objectpoint_one.x = 2350.0;
    objectpoint_one.y = 7000.0;
    objectpoint_one.z = 0.0;
    objectPoints.push_back(objectpoint_one);
    Point3d objectpoint_two;
    objectpoint_two.x = 2350.0;
    objectpoint_two.y = 7000.0;
    objectpoint_two.z = 500.0;
    objectPoints.push_back(objectpoint_two);
    Point3d objectpoint_three;
    objectpoint_three.x = 4750.0;
    objectpoint_three.y = 6800.0;
    objectpoint_three.z = 500.0;
    objectPoints.push_back(objectpoint_three);
    Point3d objectpoint_four;
    objectpoint_four.x = 4750.0;
    objectpoint_four.y = 6800.0;
    objectpoint_four.z = 0.0;
    objectPoints.push_back(objectpoint_four);
    Point2d imagepoint_one;
    imagepoint_one.x = 67.0;
    imagepoint_one.y = 176.0;
    imagePoints.push_back(imagepoint_one);
    Point2d imagepoint_two;
    imagepoint_two.x = 67.0;
    imagepoint_two.y = 95.0;
    imagePoints.push_back(imagepoint_two);
    Point2d imagepoint_three;
    imagepoint_three.x = 519.0;
    imagepoint_three.y = 107.0;
    imagePoints.push_back(imagepoint_three);
    Point2d imagepoint_four;
    imagepoint_four.x = 519.0;
    imagepoint_four.y = 189.0;
    imagePoints.push_back(imagepoint_four);
    /*cout << "far_imagePoints.size():" << imagePoints.size() << endl;
    cout << "far_objectPoints.size():" << objectPoints.size() << endl;*/
    cout << "开始相机标定" << endl;

    solvePnP(objectPoints, imagePoints, far_CamMatrix_, far_distCoeffs_, far_Rjacob, far_T);
    solvePnP(objectPoints, imagePoints, close_CamMatrix_, close_distCoeffs_, close_Rjacob, close_T);

    Rodrigues(far_Rjacob, far_R);
    Rodrigues(close_Rjacob, close_R);
    cout << "旋转矩阵:" << far_R << endl;
    cout << "平移矩阵" << far_T << endl;
}



void MapTest::pnp_callback(const std_msgs::msg::Float32MultiArray msg)
{
    this->far_R.at<double>(0, 0) = msg.data[0];
    this->far_R.at<double>(0, 1) = msg.data[1];
    this->far_R.at<double>(0, 2) = msg.data[2];
    this->far_R.at<double>(1, 0) = msg.data[3];
    this->far_R.at<double>(1, 1) = msg.data[4];
    this->far_R.at<double>(1, 2) = msg.data[5];
    this->far_R.at<double>(2, 0) = msg.data[6];
    this->far_R.at<double>(2, 1) = msg.data[7];
    this->far_R.at<double>(2, 2) = msg.data[8];
    this->far_T.at<double>(0, 0) = msg.data[9];
    this->far_T.at<double>(1, 0) = msg.data[10];
    this->far_T.at<double>(2, 0) = msg.data[11];
    cout << "接受完毕" << endl;
    cout << "旋转矩阵:" << far_R << endl;
    cout << "平移矩阵" << far_T << endl;
}

Point2f MapTest::calculate_pixel_codi(const map_point &point) {
    Point2f res;
    res.x = point.x / object_width * image_width;
    res.y = 800 - (point.y / object_height * image_height);
    cout << res.x << "        " << res.y << endl;
    return res;
}

void MapTest::draw_point_on_map(Mat &map,vector<map_point> &map_points)
{
    RCLCPP_INFO(this->get_logger(), "begin to draw_point_on_map");
    string id;
    for(int i = 0;i<map_points.size();i++)
    {
        if(map_points[i].x > 0 && map_points[i].x < image_width && map_points[i].y > 0 && map_points[i].y < image_height)
        {
            id = to_string(map_points[i].id);
            Point2f p = calculate_pixel_codi(map_points[i]);
            if(map_points[i].id <= 5)
            {
                circle(map, p, 10, Scalar(255,0,0), -1, LINE_8, 0);
            }
            else
            {
                circle(map, p, 10, Scalar(255,0,0), -1, LINE_8, 0);
            }
            putText(map, id, Point(p.x - 7, p.y - 7) , cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(0xFF, 0xFF, 0xFF), 2);
        }
    }
    RCLCPP_INFO(this->get_logger(), "draw_point_on_map finished");
}

void MapTest::far_callback(const my_msgss::msg::Distpoints msg)
{
    Mat invR;
    Mat invM;
    //求内参逆矩阵
    invert(far_CamMatrix_, invM);
    //求旋转矩阵逆矩阵
    invert(far_R, invR);
    test_map.copyTo(map);
    for(int i =0 ;i<msg.data.size();i++)
    {
        if(msg.data[i].dist > 0) {
            RCLCPP_INFO(this->get_logger(), "begin to calculate");
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) msg.data[i].x, (double) msg.data[i].y, 1);
            //以mm为单位，从m->mm 
            //这个相当于公式中的（u，v，1）* d
            x8_pixel *= (1000 * msg.data[i].dist);
            //公式 得到三维世界坐标
            Mat calcWorld = invR * (invM * x8_pixel - far_T);//2D-3D变换
            //换算成m
            calcWorld /= 1000;
            map_point a_map_point;
            a_map_point.id = msg.data[i].id;
            a_map_point.x = calcWorld.at<double>(0, 0);
            a_map_point.y = calcWorld.at<double>(1, 0);
            a_map_point.z = calcWorld.at<double>(2, 0);
            far_map_points.push_back(a_map_point);
        }
    }
    my_msgss::msg::Points world_points;
    for(int i = 0 ;i<far_map_points.size();i++)
    {
        my_msgss::msg::Point world_point;
        world_point.id = far_map_points[i].id;
        world_point.x = far_map_points[i].x;
        world_point.y = far_map_points[i].y;
        world_point.z = far_map_points[i].z;
        world_points.data.push_back(world_point);
    }
    far_points_pub_->publish(world_points);
    far_map_points.clear();
    /*imshow("map", map);
    waitKey(1);*/
}

void MapTest::close_callback(const my_msgss::msg::Distpoints msg)
{
    Mat invR;
    Mat invM;
    //求内参逆矩阵
    invert(close_CamMatrix_, invM);
    //求旋转矩阵逆矩阵
    invert(close_R, invR);
    test_map.copyTo(map);
    for(int i =0 ;i<msg.data.size();i++)
    {
        if(msg.data[i].dist > 0) {
            RCLCPP_INFO(this->get_logger(), "begin to calculate");
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) msg.data[i].x, (double) msg.data[i].y, 1);
            //以mm为单位，从m->mm 
            //这个相当于公式中的（u，v，1）* d
            x8_pixel *= (1000 * msg.data[i].dist);
            //公式 得到三维世界坐标
            Mat calcWorld = invR * (invM * x8_pixel - far_T);//2D-3D变换
            //换算成m
            calcWorld /= 1000;
            map_point a_map_point;
            a_map_point.id = msg.data[i].id;
            a_map_point.x = calcWorld.at<double>(0, 0);
            a_map_point.y = calcWorld.at<double>(1, 0);
            a_map_point.z = calcWorld.at<double>(2, 0);
            close_map_points.push_back(a_map_point);
        }
    }
    my_msgss::msg::Points world_points;
    for(int i = 0 ;i<close_map_points.size();i++)
    {
        my_msgss::msg::Point world_point;
        world_point.id = close_map_points[i].id;
        world_point.x = close_map_points[i].x;
        world_point.y = close_map_points[i].y;
        world_point.z = close_map_points[i].z;
        world_points.data.push_back(world_point);
    }
    close_points_pub_->publish(world_points);
    close_map_points.clear();
    /*imshow("map", map);
    waitKey(1);*/
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MapTest>());
    rclcpp::shutdown();
    return 0;
}