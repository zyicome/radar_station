#include "map_test.hpp"
#include "CameraDefine.h"
MapTest::MapTest() : Node("map_test")
{
    robots_init();

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
    far_camera_matrix.at<double>(0, 0) = 3066.03905;
    far_camera_matrix.at<double>(0, 1) = 0;
    far_camera_matrix.at<double>(0, 2) = 667.26311;
    far_camera_matrix.at<double>(1, 0) = 0;
    far_camera_matrix.at<double>(1, 1) = 3080.97863;
    far_camera_matrix.at<double>(1, 2) = 549.63631;
    far_camera_matrix.at<double>(2, 0) = 0;
    far_camera_matrix.at<double>(2, 1) = 0;
    far_camera_matrix.at<double>(2, 2) = 1;
    far_distortion_coefficient.at<double>(0,0) = -0.069436;
    far_distortion_coefficient.at<double>(1,0) = 1.005937;
    far_distortion_coefficient.at<double>(2,0) = -0.002552;
    far_distortion_coefficient.at<double>(3,0) = 0.003604;
    far_distortion_coefficient.at<double>(4,0) = 0.000000;
    close_camera_matrix.at<double>(0, 0) = 1563.52174;
    close_camera_matrix.at<double>(0, 1) = 0;
    close_camera_matrix.at<double>(0, 2) = 626.90356;
    close_camera_matrix.at<double>(1, 0) = 0;
    close_camera_matrix.at<double>(1, 1) = 1568.90028;
    close_camera_matrix.at<double>(1, 2) = 488.93524;
    close_camera_matrix.at<double>(2, 0) = 0;
    close_camera_matrix.at<double>(2, 1) = 0;
    close_camera_matrix.at<double>(2, 2) = 1;
    close_distortion_coefficient.at<double>(0,0) = -0.063200;
    close_distortion_coefficient.at<double>(1,0) = -0.005061;
    close_distortion_coefficient.at<double>(2,0) = -0.001755;
    close_distortion_coefficient.at<double>(3,0) = 0.003472;
    close_distortion_coefficient.at<double>(4,0) = 0.000000;
}

void MapTest::get_map()
{
    auto pkg_path = ament_index_cpp::get_package_share_directory("Qt5_displayer");
    string map_path = pkg_path + "/map/bluemap.png";
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
    objectpoint_one.x = 2520.0;
    objectpoint_one.y = 5930.0;
    objectpoint_one.z = 503.0;
    objectPoints.push_back(objectpoint_one);
    Point3d objectpoint_two;
    objectpoint_two.x = 2520.0;
    objectpoint_two.y = 5930.0;
    objectpoint_two.z = 1528.0;
    objectPoints.push_back(objectpoint_two);
    Point3d objectpoint_three;
    objectpoint_three.x = 3775.0;
    objectpoint_three.y = 5930.0;
    objectpoint_three.z = 1528.0;
    objectPoints.push_back(objectpoint_three);
    Point3d objectpoint_four;
    objectpoint_four.x = 3775.0;
    objectpoint_four.y = 5930.0;
    objectpoint_four.z = 503.0;
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

    solvePnP(objectPoints, imagePoints, far_camera_matrix, far_distortion_coefficient, far_Rjacob, far_T);
    solvePnP(objectPoints, imagePoints, close_camera_matrix, close_distortion_coefficient, close_Rjacob, close_T);

    Rodrigues(far_Rjacob, far_R);
    Rodrigues(close_Rjacob, close_R);
    cout << "旋转矩阵:" << far_R << endl;
    cout << "平移矩阵" << far_T << endl;
}



void MapTest::pnp_callback(const std_msgs::msg::Float32MultiArray msg)
{
    if(msg.data[12] == 1)
    {
        this->close_R.at<double>(0, 0) = msg.data[0];
    this->close_R.at<double>(0, 1) = msg.data[1];
    this->close_R.at<double>(0, 2) = msg.data[2];
    this->close_R.at<double>(1, 0) = msg.data[3];
    this->close_R.at<double>(1, 1) = msg.data[4];
    this->close_R.at<double>(1, 2) = msg.data[5];
    this->close_R.at<double>(2, 0) = msg.data[6];
    this->close_R.at<double>(2, 1) = msg.data[7];
    this->close_R.at<double>(2, 2) = msg.data[8];
    this->close_T.at<double>(0, 0) = msg.data[9];
    this->close_T.at<double>(1, 0) = msg.data[10];
    this->close_T.at<double>(2, 0) = msg.data[11];
    cout << "接受完毕" << endl;
    cout << "旋转矩阵:" << close_R << endl;
    cout << "平移矩阵" << close_T << endl;
    }
    else{
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
}

Point2f MapTest::calculate_pixel_codi(const map_point &point) {
    Point2f res;
    res.x = point.x / object_width * image_width;
    res.y = image_height - (point.y / object_height * image_height);
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
    invert(far_camera_matrix, invM);
    //求旋转矩阵逆矩阵
    invert(far_R, invR);
    test_map.copyTo(map);

    get_robots(far_robots, msg);
    allrobots_adjust(far_robots);

    my_msgss::msg::Points far_world_points;

    for(int i =1;i<far_robots.size();i++)
    {
        if(far_robots[i].distance != 0.0)
        {
            //RCLCPP_INFO(this->get_logger(), "begin to calculate");
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) far_robots[i].x, (double) far_robots[i].y, 1);
            //以mm为单位，从m->mm 
            //这个相当于公式中的（u，v，1）* d
            x8_pixel *= (1000 * far_robots[i].distance);
            //公式 得到三维世界坐标
            Mat calcWorld = invR * (invM * x8_pixel - far_T);//2D-3D变换
            //换算成m
            calcWorld /= 1000;

            my_msgss::msg::Point far_world_point;
            far_world_point.id = far_robots[i].id;
            far_world_point.confidence = far_robots[i].confidence;
            far_world_point.x = calcWorld.at<double>(0, 0);
            far_world_point.y = calcWorld.at<double>(1, 0);
            far_world_point.z = calcWorld.at<double>(2, 0);
            far_world_points.data.push_back(far_world_point);
        }
    }
    far_points_pub_->publish(far_world_points);
    /*imshow("map", map);
    waitKey(1);*/
}

void MapTest::close_callback(const my_msgss::msg::Distpoints msg)
{
    Mat invR;
    Mat invM;
    //求内参逆矩阵
    invert(close_camera_matrix, invM);
    //求旋转矩阵逆矩阵
    invert(close_R, invR);
    test_map.copyTo(map);
    
    get_robots(close_robots, msg);
    allrobots_adjust(close_robots);

    my_msgss::msg::Points close_world_points;

    for(int i =1;i<close_robots.size();i++)
    {
        if(close_robots[i].distance != 0.0)
        {
            //RCLCPP_INFO(this->get_logger(), "begin to calculate");
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) close_robots[i].x, (double) close_robots[i].y, 1);
            //以mm为单位，从m->mm 
            //这个相当于公式中的（u，v，1）* d
            x8_pixel *= (1000 * close_robots[i].distance);
            //公式 得到三维世界坐标
            Mat calcWorld = invR * (invM * x8_pixel - close_T);//2D-3D变换
            //换算成m
            calcWorld /= 1000;

            my_msgss::msg::Point close_world_point;
            close_world_point.id = close_robots[i].id;
            close_world_point.confidence = close_robots[i].confidence;
            close_world_point.x = calcWorld.at<double>(0, 0);
            close_world_point.y = calcWorld.at<double>(1, 0);
            close_world_point.z = calcWorld.at<double>(2, 0);
            close_world_points.data.push_back(close_world_point);
        }
    }
    close_points_pub_->publish(close_world_points);
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

//-------------------------------------------------------------------------------------------------
// new place

void MapTest::robots_init()
{
    Robot robot;
    robot.id = -1;
    robot.confidence = 0.0;
    robot.is_continue = false;
    for(int i =0;i<19;i++)
    {
        robot.id++;
        far_robots.push_back(robot);
        close_robots.push_back(robot);
    }
}

void MapTest::get_robots(std::vector<Robot> &robots, const my_msgss::msg::Distpoints &input)
{
    int robot_number = 0;

    for(int i = 0;i < input.data.size();i++)
    {
        robot_number = input.data[i].id;
        robots[robot_number].is_continue = true;
        robots[robot_number].confidence = input.data[i].confidence;
        robots[robot_number].x = input.data[i].x;
        robots[robot_number].y = input.data[i].y;
        robots[robot_number].distance = input.data[i].dist;
    }
}

void MapTest::allrobots_adjust(std::vector<Robot> &robots)
{
    for(int i = 0;i<robots.size();i++)
    {
        if(robots[i].is_continue == false)
        {
            robots[i].confidence = 0.0;
            robots[i].x = 0.0;
            robots[i].y = 0.0;
            robots[i].distance = 0.0;
        }

        robots[i].is_continue = false;
    }
}

//-------------------------------------------------------------------------------------------------