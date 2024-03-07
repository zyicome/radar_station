#include "game_map.hpp"
#include <functional>

// 本模块所需传入的参数：

// 本模块所需传出的参数：

// 本模块所需订阅的话题：

// 本模块所需发布的话题：


void onMouse(int event, int x, int y, int flags, void *userdata);//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号

Game_Map::Game_Map() : Node("game_map")
{
    // 初始化
    field_width = 28; //28
    field_height = 15; //15
    imgCols = 1280.0;
    imgRows = 1024.0;
    red_or_blue = 0; //0 is red, 1 is blue
    outpost_calc_flag = 0;
    warn_region_state = 0x0000;
    far_CamMatrix_ = Mat::zeros(3, 3, CV_64FC1);
    far_distCoeffs_ = Mat::zeros(5, 1, CV_64FC1);
    far_Rjacob = Mat::zeros(3, 1, CV_64FC1);
    far_R = Mat::eye(3, 3, CV_64FC1);
    far_T = Mat::zeros(3, 1, CV_64FC1);

    close_CamMatrix_ = Mat::zeros(3, 3, CV_64FC1);
    close_distCoeffs_ = Mat::zeros(5, 1, CV_64FC1);
    close_Rjacob = Mat::zeros(3, 1, CV_64FC1);
    close_R = Mat::eye(3, 3, CV_64FC1);
    close_T = Mat::zeros(3, 1, CV_64FC1);

    // 创建发布者和订阅者
    world_points_pub_ = this->create_publisher<my_msgss::msg::Points>("world_point", 10);
    guard_points_pub_ = this->create_publisher<my_msgss::msg::Points>("guard_pub", 10);
    hero_point_pub_ = this->create_publisher<my_msgss::msg::Point>("hero_pub", 1);
    far_image_sub_ = this->create_subscription<my_msgss::msg::Points>("/sensor_far/calibration", 10, std::bind(&Game_Map::far_calibration, this, std::placeholders::_1));
    far_distpoints_sub_ = this->create_subscription<my_msgss::msg::Distpoints>("/sensor_far/distance_point",10,std::bind(&Game_Map::far_distPointCallback,this,std::placeholders::_1));
    close_image_sub_ = this->create_subscription<my_msgss::msg::Points>("/sensor_close/calibration", 10, std::bind(&Game_Map::close_calibration, this, std::placeholders::_1));
    close_distpoints_sub_ = this->create_subscription<my_msgss::msg::Distpoints>("/sensor_close/distance_point",10,std::bind(&Game_Map::close_distPointCallback,this,std::placeholders::_1));
    outpost_distpoint_sub_ = this->create_subscription<my_msgss::msg::Distpoint>("/sensor_far/outpost",10,std::bind(&Game_Map::outpost_Callback,this,std::placeholders::_1));
    timer_ = this->create_wall_timer(50ms, std::bind(&Game_Map::timer_callback, this));

    warn_regions_init();
    get_all_parameter();
    camera_pnp_calibration();
    get_map_init();
}

//在小地图上画分区域
void Game_Map::warn_regions_init()
{
    our_R1 = {Point(0, 395), Point(0, 562), Point(33, 562), Point(33, 395)};
    our_R2 = {Point(76, 511), Point(160, 569), Point(247, 569), Point(259, 562), Point(235, 532),
                        Point(172, 530), Point(100, 477)};
    our_R3 = {Point(0, 572), Point(0, 705), Point(157, 705), Point(157, 654), Point(31, 572)};
    our_dafu = {Point(370, 558), Point(370, 609), Point(416, 609), Point(416, 558)};
    our_highway = {Point(415, 464), Point(415, 644), Point(450, 644), Point(450, 464)};
    our_outpost = {Point(414, 558), Point(414, 445), Point(317, 445), Point(317, 558)};
    our_half_field = {Point(0, 420), Point(0, 840), Point(450, 840), Point(450, 420)};
    enemy_highway = {Point(35, 376), Point(35, 196), Point(0, 196), Point(0, 376)};
    enemy_dafu = {Point(80, 282), Point(80, 231), Point(34, 231), Point(34, 282)};
    enemy_outpost = {Point(36, 282), Point(36, 395), Point(133, 395), Point(133, 282)};
    enemy_hero_hide = {Point(417, 333), Point(417, 445), Point(450, 445), Point(450, 333)};
    enemy_R3 = {Point(450, 268), Point(450, 135), Point(293, 135), Point(293, 186), Point(419, 268)};
    guard_forbidden_zone = {Point(160, 647), Point(160, 705), Point(287, 705), Point(287, 647)};
    guidao_houbian = {Point(0, 676), Point(0, 840), Point(450, 840), Point(450, 676)};

    our_warn_regions.emplace_back(our_R1);
    our_warn_regions.emplace_back(our_R2);
    our_warn_regions.emplace_back(our_R3);
    our_warn_regions.emplace_back(our_dafu);
    our_warn_regions.emplace_back(our_highway);
    our_warn_regions.emplace_back(our_outpost);
    enemy_warn_regions.emplace_back(enemy_highway);
    enemy_warn_regions.emplace_back(enemy_dafu);
    enemy_warn_regions.emplace_back(enemy_outpost);
    enemy_warn_regions.emplace_back(enemy_hero_hide);
    enemy_warn_regions.emplace_back(enemy_R3);
}

void Game_Map::get_all_parameter()
{
    this->declare_parameter("/small_map/small_map_shift_X", 30);
    this->declare_parameter("/small_map/small_map_shift_Y", 5);
    this->get_parameter("/small_map/small_map_shift_X", this->X_shift);
    this->get_parameter("/small_map/small_map_shift_Y", this->Y_shift);
    this->X_shift = 0;
    this->Y_shift = 0;

    //相机标定参数
    this->declare_parameter("/sensor_far/pointone/x", 2461.0);
    this->declare_parameter("/sensor_far/pointone/y", 16706.0);
    this->declare_parameter("/sensor_far/pointone/z", 1581.0);
    this->declare_parameter("/sensor_far/pointtwo/x", 6321.0);
    this->declare_parameter("/sensor_far/pointtwo/y", 8805.0);
    this->declare_parameter("/sensor_far/pointtwo/z", 615.0);
    this->declare_parameter("/sensor_far/pointthree/x", 6981.0);
    this->declare_parameter("/sensor_far/pointthree/y", 8805.0);
    this->declare_parameter("/sensor_far/pointthree/z", 615.0);
    this->declare_parameter("/sensor_far/pointfour/x", 7500.0);
    this->declare_parameter("/sensor_far/pointfour/y", 26302.0);
    this->declare_parameter("/sensor_far/pointfour/z", 1118.0);
    this->declare_parameter("/sensor_far/camera_matrix/zerozero", 1474.62545);
    this->declare_parameter("/sensor_far/camera_matrix/zerotwo", 367.67245);
    this->declare_parameter("/sensor_far/camera_matrix/oneone", 1463.59535);
    this->declare_parameter("/sensor_far/camera_matrix/onetwo", 274.85727);
    this->declare_parameter("/sensor_far/camera_matrix/twotwo", 1.);
    this->declare_parameter("/sensor_far/distortion_coefficient/zero", -0.025299);
    this->declare_parameter("/sensor_far/distortion_coefficient/one", -0.874546);
    this->declare_parameter("/sensor_far/distortion_coefficient/two", -0.000261);
    this->declare_parameter("/sensor_far/distortion_coefficient/three", 0.018625);
    this->declare_parameter("/sensor_far/distortion_coefficient/four", 0.0);

    Point3d far_objectPoint;
    this->get_parameter("/sensor_far/pointone/x", far_objectPoint.x);
    this->get_parameter("/sensor_far/pointone/y", far_objectPoint.y);
    this->get_parameter("/sensor_far/pointone/z", far_objectPoint.z);
    far_objectPoints.push_back(far_objectPoint);
    this->get_parameter("/sensor_far/pointtwo/x", far_objectPoint.x);
    this->get_parameter("/sensor_far/pointtwo/y", far_objectPoint.y);
    this->get_parameter("/sensor_far/pointtwo/z", far_objectPoint.z);
    far_objectPoints.push_back(far_objectPoint);
    this->get_parameter("/sensor_far/pointthree/x", far_objectPoint.x);
    this->get_parameter("/sensor_far/pointthree/y", far_objectPoint.y);
    this->get_parameter("/sensor_far/pointthree/z", far_objectPoint.z);
    far_objectPoints.push_back(far_objectPoint);
    this->get_parameter("/sensor_far/pointfour/x", far_objectPoint.x);
    this->get_parameter("/sensor_far/pointfour/y", far_objectPoint.y);
    this->get_parameter("/sensor_far/pointfour/z", far_objectPoint.z);
    far_objectPoints.push_back(far_objectPoint);

    this->get_parameter("/sensor_far/camera_matrix/zerozero", far_CamMatrix_.at<double>(0, 0));
    this->get_parameter("/sensor_far/camera_matrix/zerotwo", far_CamMatrix_.at<double>(0, 2));
    this->get_parameter("/sensor_far/camera_matrix/oneone", far_CamMatrix_.at<double>(1, 1));
    this->get_parameter("/sensor_far/camera_matrix/onetwo", far_CamMatrix_.at<double>(1, 2));
    this->get_parameter("/sensor_far/camera_matrix/twotwo", far_CamMatrix_.at<double>(2, 2));
    
    int hello_ = 5;
    this->get_parameter("hello",hello_);
    cout << hello_ << endl;

    far_CamMatrix_.at<double>(0, 1) = 0;
    far_CamMatrix_.at<double>(1, 0) = 0;
    far_CamMatrix_.at<double>(2, 0) = 0;
    far_CamMatrix_.at<double>(2, 1) = 0;
    cout << far_CamMatrix_.at<double>(0, 0) << endl;
    cout << far_CamMatrix_.at<double>(0, 2) << endl;
    cout << far_CamMatrix_.at<double>(1, 1) << endl;
    cout << far_CamMatrix_.at<double>(1, 2) << endl;
    cout << far_CamMatrix_.at<double>(2, 2) << endl;
    cout << far_CamMatrix_ << endl;
    this->get_parameter("/sensor_far/distortion_coefficient/zero", far_distCoeffs_.at<double>(0, 0));
    this->get_parameter("/sensor_far/distortion_coefficient/one", far_distCoeffs_.at<double>(1, 0));
    this->get_parameter("/sensor_far/distortion_coefficient/two", far_distCoeffs_.at<double>(2, 0));
    this->get_parameter("/sensor_far/distortion_coefficient/three", far_distCoeffs_.at<double>(3, 0));
    this->get_parameter("/sensor_far/distortion_coefficient/four", far_distCoeffs_.at<double>(4, 0));
    cout << far_distCoeffs_ << endl;
    
    // this->get_parameter("/sensor_close/point1/x", close_objectPoints[0].x);
    // this->get_parameter("/sensor_close/point1/y", close_objectPoints[0].y);
    // this->get_parameter("/sensor_close/point1/z", close_objectPoints[0].z);
    // this->get_parameter("/sensor_close/point2/x", close_objectPoints[1].x);
    // this->get_parameter("/sensor_close/point2/y", close_objectPoints[1].y);
    // this->get_parameter("/sensor_close/point2/z", close_objectPoints[1].z);
    // this->get_parameter("/sensor_close/point3/x", close_objectPoints[2].x);
    // this->get_parameter("/sensor_close/point3/y", close_objectPoints[2].y);
    // this->get_parameter("/sensor_close/point3/z", close_objectPoints[2].z);
    // this->get_parameter("/sensor_close/point4/x", close_objectPoints[3].x);
    // this->get_parameter("/sensor_close/point4/y", close_objectPoints[3].y);
    // this->get_parameter("/sensor_close/point4/z", close_objectPoints[3].z);

    // this->get_parameter("/sensor_close/camera_matrix/zerozero", close_CamMatrix_.at<double>(0, 0));
    // this->get_parameter("/sensor_close/camera_matrix/zerotwo", close_CamMatrix_.at<double>(0, 2));
    // this->get_parameter("/sensor_close/camera_matrix/oneone", close_CamMatrix_.at<double>(1, 1));
    // this->get_parameter("/sensor_close/camera_matrix/onetwo", close_CamMatrix_.at<double>(1, 2));
    // this->get_parameter("/sensor_close/camera_matrix/twotwo", close_CamMatrix_.at<double>(2, 2));
    // close_CamMatrix_.at<double>(0, 1) = 0;
    // close_CamMatrix_.at<double>(1, 0) = 0;
    // close_CamMatrix_.at<double>(2, 0) = 0;
    // close_CamMatrix_.at<double>(2, 1) = 0;
    // cout << close_CamMatrix_ << endl;
    // this->get_parameter("/sensor_close/distortion_coefficient/zero", close_distCoeffs_.at<double>(0, 0));
    // this->get_parameter("/sensor_close/distortion_coefficient/one", close_distCoeffs_.at<double>(1, 0));
    // this->get_parameter("/sensor_close/distortion_coefficient/two", close_distCoeffs_.at<double>(2, 0));
    // this->get_parameter("/sensor_close/distortion_coefficient/three", close_distCoeffs_.at<double>(3, 0));
    // this->get_parameter("/sensor_close/distortion_coefficient/four", close_distCoeffs_.at<double>(4, 0));
    // cout << close_distCoeffs_ << endl;

    this->get_parameter("/battle_state/battle_color", btlcolor);
    if (btlcolor == "red") red_or_blue = 0;
    if (btlcolor == "blue") red_or_blue = 1;
}

void Game_Map::camera_pnp_calibration()
{
    //dovejh 读取默认pnp四点的坐标，防止意外重启造成pnp数据丢失。
    float x = 0, y = 0;
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point1/x", x);
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point1/y", y);
    // close_imagePoints[0] = cv::Point2d(x, y);
    // close_imagePoints[0].x *= imgCols;
    // close_imagePoints[0].y *= imgRows;
    // cout << close_imagePoints[0] << endl;
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point2/x", x);
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point2/y", y);
    // close_imagePoints[1] = cv::Point2d(x, y);
    // close_imagePoints[1].x *= imgCols;
    // close_imagePoints[1].y *= imgRows;
    // cout << close_imagePoints[1] << endl;
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point3/x", x);
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point3/y", y);
    // close_imagePoints[2] = cv::Point2d(x, y);
    // close_imagePoints[2].x *= imgCols;
    // close_imagePoints[2].y *= imgRows;
    // cout << close_imagePoints[2] << endl;
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point4/x", x);
    // this->get_parameter("/camera/list/closeCam/calibrationDefault/point4/y", y);
    // close_imagePoints[3] = cv::Point2d(x, y);
    // close_imagePoints[3].x *= imgCols;
    // close_imagePoints[3].y *= imgRows;
    // cout << close_imagePoints[3] << endl;

    // cout << "已读取到closeCam默认参数值!下面进行SolvePnP求解外参矩阵。" << endl;
    // cout << "close obj points:" << close_objectPoints << endl;
    // cv::solvePnPRansac(close_objectPoints, close_imagePoints, close_CamMatrix_, close_distCoeffs_,
    //                    close_Rjacob, close_T,cv::SOLVEPNP_AP3P);
    // Rodrigues(close_Rjacob, close_R); //将R从雅可比形式转换为罗德里格斯形式,输出的R是3x3的一个矩阵。
    // cout << "close旋转矩阵:" << close_R << endl;
    // cout << "close平移矩阵" << close_T << endl;
    
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointone/x", 0.247);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointone/y", 0.070);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointtwo/x", 0.541);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointtwo/y", 0.411);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointthree/x", 0.632);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointthree/y", 0.411);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointfour/x", 0.614);
    this->declare_parameter("/camera/list/farCam/calibrationDefault/pointfour/y", 0.016);


    this->get_parameter("/camera/list/farCam/calibrationDefault/pointtwo/x", x);
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointtwo/y", y);
    far_imagePoints[0] = cv::Point2d(x, y);
    far_imagePoints[0].x *= imgCols;
    far_imagePoints[0].y *= imgRows;
    cout << far_imagePoints[0] << endl;
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointtwo/x", x);
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointtwo/y", y);
    far_imagePoints[1] = cv::Point2d(x, y);
    far_imagePoints[1].x *= imgCols;
    far_imagePoints[1].y *= imgRows;
    cout << far_imagePoints[1] << endl;
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointthree/x", x);
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointthree/y", y);
    far_imagePoints[2] = cv::Point2d(x, y);
    far_imagePoints[2].x *= imgCols;
    far_imagePoints[2].y *= imgRows;
    cout << far_imagePoints[2] << endl;
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointfour/x", x);
    this->get_parameter("/camera/list/farCam/calibrationDefault/pointfour/y", y);
    far_imagePoints[3] = cv::Point2d(x, y);
    far_imagePoints[3].x *= imgCols;
    far_imagePoints[3].y *= imgRows;
    cout << far_imagePoints[3] << endl;

    cout << "已读取到farCam默认参数值!下面进行SolvePnP求解外参矩阵。" << endl;
    cout << "far obj points:" << far_objectPoints << endl;
    cv::solvePnP(far_objectPoints, far_imagePoints, far_CamMatrix_, far_distCoeffs_,
                       far_Rjacob, far_T);
    Rodrigues(far_Rjacob, far_R); //将R从雅可比形式转换为罗德里格斯形式,输出的R是3x3的一个矩阵。
    cout << "far旋转矩阵:" << far_R << endl;
    cout << "far平移矩阵" << far_T << endl;
}

//初始化小地图，根据颜色选择不同的小地图
void Game_Map::get_map_init()
{
    // if (red_or_blue == 0)
    // {
    //     std::string small_map_png = "/home/mechax/AAAmy_ws/1.14/radar_station/src/Game_Map/map/blue_minimap.png";
    //     small_map = imread(small_map_png);
    // }
    // else
    // {
    //     std::string small_map_png = "/home/mechax/AAAmy_ws/1.14/radar_station/src/Game_Map/map/blue_minimap.png";
    //     small_map = imread(small_map_png);
    // }
    //for test
    std::string small_map_png = "/home/mechax/AAAmy_ws/1.14/radar_station/src/Game_Map/map/test.png";
    small_map = imread(small_map_png);
     resize(small_map, small_map, Size(640, 480));
    small_map.copyTo(small_map_copy);
    namedWindow("small_map");
    setMouseCallback("small_map", onMouse, &small_map_copy);
}

/**
 * 鼠标操作
 * @param event 事件
 * @param x 鼠标x坐标
 * @param y 鼠标y坐标
 * @param flags
 * @param userdata
 */
void onMouse(int event, int x, int y, int flags, void *userdata)
{
    if(event == EVENT_LBUTTONDOWN)
    {
        vector<Point> our_R3 = {Point(0, 572), Point(0, 705), Point(157, 705), Point(157, 654), Point(31, 572)};
        circle(*(Mat *) userdata, Point2i(x, y), 5, Scalar(0x27, 0xc1, 0x36), -1);
        int is_in_poly = pointPolygonTest(our_R3, Point2f(x, y), false);
        cout << is_in_poly << "  ";
        if (is_in_poly > 0)cout << "in" << endl;
        else if (is_in_poly < 0)cout << "out" << endl;
        else if (is_in_poly == 0)cout << "edge" << endl;
        imshow("small_map", *(Mat *) userdata);
        cout << Point(x, y) << endl;
    }
}

// 这是干啥的 看起来是计算两点的距离
double Game_Map::Point2PointDist(const my_msgss::msg::Point &a, const Point3f &b)
{
    double res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return res;
}

/**
 * 计算二维点和二维点之间的距离
 * @param a 点a
 * @param b 点b
 * @return 距离
 */
double Game_Map::calculate_dist(const my_msgss::msg::Point &a, const my_msgss::msg::Point &b) {
    double res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return res;
}

/**
 * 判断是否为敌方车辆
 * @param id 车辆ID
 * @return 是否为敌方车辆
 */
// 根据我们的神经网络来调整id
bool Game_Map::is_enemy_car(uint8_t id) {
    if (red_or_blue == 0) {
        if ((id >= 0 && id <= 8))
            return true;
        else return false;
    } else {
        if ((id >= 9 && id <= 17))
            return true;
        else return false;
    }
}

/**
 * 去重算法，去除双相机中重复的部分
 */
// void Game_Map::remove_duplicate() {
//     RCLCPP_INFO(this->get_logger(), "begin to remove_duplicate");
//     vector<my_msgss::msg::Point>().swap(result_points.data);
//     my_msgss::msg::Points red_no_id_cars;
//     my_msgss::msg::Points blue_no_id_cars;
//     my_msgss::msg::Points left_may_overlap_points;
//     my_msgss::msg::Points right_may_overlap_points;
//     vector<Point2f> left_region = {Point(0, 0), Point(0, 840), Point(168, 840), Point(278, 0)};
//     vector<Point2f> right_region = {Point(278, 0), Point(168, 840), Point(450, 840), Point(450, 0)};
//     for (auto &i: far_points.data) {
//         int test = pointPolygonTest(left_region, calculate_pixel_codi(i), false);
//         int test2=pointPolygonTest(right_region, calculate_pixel_codi(i), false);
//         if (test > 0) {
//             result_points.data.emplace_back(i);
//         } else if (test == 0 && i.x != 200) {
//             result_points.data.emplace_back(i);
//         } else if (pointPolygonTest(right_region, calculate_pixel_codi(i), false)<=0) {
//             left_may_overlap_points.data.emplace_back(i);
//         }
//     }
//     for (auto &i: close_points.data) {
//         int test = pointPolygonTest(right_region, calculate_pixel_codi(i), false);
//         int test2=pointPolygonTest(left_region, calculate_pixel_codi(i), false);
//         if (test > 0) {
//             result_points.data.emplace_back(i);
//         } else if (test == 0 && i.x != 255) {
//             result_points.data.emplace_back(i);
//         } else if (pointPolygonTest(left_region, calculate_pixel_codi(i), false)<=0) {
//             right_may_overlap_points.data.emplace_back(i);
//         }
//     }
//     uint8_t inner_erase_flag = 0;
//     uint8_t outer_erase_flag = 0;
//     for (auto it_left = left_may_overlap_points.data.begin(); it_left < left_may_overlap_points.data.end();) {
//         for (auto it_right = right_may_overlap_points.data.begin(); it_right < right_may_overlap_points.data.end();) {
//             if (it_left->id == it_right->id && it_left->id == 12 && calculate_dist(*it_left, *it_right) < 5) {
//                 my_msgss::msg::Point center;
//                 center.id = 12;
//                 center.x = (it_left->x + it_right->x) / 2;
//                 center.y = (it_left->y + it_right->y) / 2;
//                 result_points.data.emplace_back(center);
//                 if (!left_may_overlap_points.data.empty())left_may_overlap_points.data.erase(it_left);
//                 if (!right_may_overlap_points.data.empty())right_may_overlap_points.data.erase(it_right);
//                 inner_erase_flag = 1;
//                 outer_erase_flag = 1;
//             } else if (it_left->id == it_right->id && it_left->id == 13 && calculate_dist(*it_left, *it_right) < 5) {
//                 my_msgss::msg::Point center;
//                 center.id = 13;
//                 center.x = (it_left->x + it_right->x) / 2;
//                 center.y = (it_left->y + it_right->y) / 2;
//                 result_points.data.emplace_back(center);
//                 if (!left_may_overlap_points.data.empty())left_may_overlap_points.data.erase(it_left);
//                 if (!right_may_overlap_points.data.empty())right_may_overlap_points.data.erase(it_right);
//                 inner_erase_flag = 1;
//                 outer_erase_flag = 1;
//             } else if (it_left->id == it_right->id && it_left->id < 12 && it_left->id > 0) {
//                 my_msgss::msg::Point center;
//                 center.id = it_left->id;
//                 center.x = (it_left->x + it_right->x) / 2;
//                 center.y = (it_left->y + it_right->y) / 2;
//                 result_points.data.emplace_back(center);
//                 if (!left_may_overlap_points.data.empty())left_may_overlap_points.data.erase(it_left);
//                 if (!right_may_overlap_points.data.empty())right_may_overlap_points.data.erase(it_right);
//                 inner_erase_flag = 1;
//                 outer_erase_flag = 1;
//             }
//             if (inner_erase_flag == 1) {
//                 inner_erase_flag = 0;
//                 continue;
//             }
//             it_right++;
//         }
//         if (outer_erase_flag == 1) {
//             outer_erase_flag = 0;
//             continue;
//         }
//         it_left++;
//     }
//     for (auto &i: left_may_overlap_points.data) {
//         result_points.data.emplace_back(i);
//     }
//     for (auto &i: right_may_overlap_points.data) {
//         result_points.data.emplace_back(i);
//     }
//     RCLCPP_INFO(this->get_logger(), "remove_duplicate finished");
// }

//小地图区域警告
void Game_Map::warn_on_map(const my_msgss::msg::Points &points, Mat &image) {
    vector<my_msgss::msg::Point>().swap(guard_relative.data);
    vector<my_msgss::msg::Point>().swap(relative_coordinates.data);
    warn_region_state = 0x0000;
    Scalar light_green = Scalar(0xcc, 0xff, 0xcc);
    far_points.id = 0;
    for (int i = 0; i < our_warn_regions.size(); i++) {
        for (my_msgss::msg::Point car: points.data) {
            if (is_enemy_car(car.id) && pointPolygonTest(our_half_field,
                                                         calculate_pixel_codi(car),
                                                         false) > 0) {
                relative_coordinates.data.push_back(calculate_relative_codi(our_guard, car, 0));
                if (pointPolygonTest(our_warn_regions[i], calculate_pixel_codi(car), false) > 0) {
                    warn_region_state |= (0x01 << (i + 5));
                    drawContours(image, our_warn_regions, i, light_green, -1);
                }
            }
        }
    }
    double nearest = 50000.0;
    double dist;
    uint8_t position = 0;
    for (int i = 0; i < relative_coordinates.data.size(); i++) {
        dist = Point2PointDist(relative_coordinates.data[i], Point3f(0, 0, 0));
        if (dist < nearest) {
            nearest = dist;
            position = (uint8_t) i;
        }
    }
    if (!relative_coordinates.data.empty()) {
        guard_relative.data.emplace_back(relative_coordinates.data[position]);
    }
    for (int i = 0; i < enemy_warn_regions.size(); i++) {
        for (my_msgss::msg::Point car: points.data) {
            if (is_enemy_car(car.id) && pointPolygonTest(enemy_warn_regions[i],
                                                         calculate_pixel_codi(car),
                                                         false) > 0) {
                warn_region_state |= (0x01 << (i));
                drawContours(image, enemy_warn_regions, i, light_green, -1);
                if (red_or_blue == 0 && guard_relative.data.empty()) {
                    if (i == 4 && car.id == 6) {
                        guard_relative.data.emplace_back(
                                calculate_relative_codi(our_guard, car, 1));//敌方英雄到达敌方公路区，可能会打前哨站

                    } else if (i == 2 && car.id != 7 && guard_relative.data.empty()) {
                        guard_relative.data.emplace_back(
                                calculate_relative_codi(our_guard, car, 2));//敌方车辆到达敌方前哨站(工程除外)
                    }
                } else {
                    if (i == 4 && car.id == 0 && guard_relative.data.empty()) {
                        guard_relative.data.emplace_back(
                                calculate_relative_codi(our_guard, car, 1));//敌方英雄到达敌方公路区，可能会打前哨站
                    } else if (i == 2 && car.id != 1 && guard_relative.data.empty()) {
                        guard_relative.data.emplace_back(
                                calculate_relative_codi(our_guard, car, 2));//敌方车辆到达敌方前哨站(工程除外)
                    }
                }
            }
        }
    }
    unsigned char state[2];
    memcpy(state, &warn_region_state, 2);
    result_points.text = string(state, state + 2);
//    cout<<bitset<8>(guard_relative.text[1])<<bitset<8>(guard_relative.text[0])<<endl;
}

//存入由yolo发来的哨塔的坐标
void Game_Map::outpost_Callback(const my_msgss::msg::Distpoint &msg) {
    outpost_dist.dist = msg.dist;
    outpost_dist.x = msg.x;
    outpost_dist.y = msg.y;
    outpost_calc_flag=1;
}

//计算哨兵和敌方机器人的相对位置
my_msgss::msg::Point Game_Map::calculate_relative_codi(const Point3f &guard, const my_msgss::msg::Point &enemy, uint8_t priority_id) {
    my_msgss::msg::Point re_codi;
    re_codi.x = enemy.x * 15000 - guard.x;
    re_codi.y = enemy.y * 28000 - guard.y;
    re_codi.z = enemy.z * 1000 - guard.z;
    re_codi.id = priority_id;
    return re_codi;
}

//计算三维世界坐标投影到小地图上的坐标
Point2f Game_Map::calculate_pixel_codi(const my_msgss::msg::Point &point) {
    Point2f res;
    res.x = point.x * 450 - (float) X_shift;
    res.y = (1 - point.y) * 840 - (float) Y_shift;
    cout << res.x << "        " << res.y << endl;
    return res;
}

//计算小地图显示文字的坐标，防止重合
Point2f Game_Map::calculate_pixel_text_codi(const my_msgss::msg::Point &point) {
    Point2f res;
    res.x = point.x * 450 - (float) X_shift - 7;
    res.y = (1 - point.y) * 840 - (float) Y_shift + 7;
    return res;
}

// 根据实际神经网络输出修改id
// 将机器人标注在小地图上
void Game_Map::draw_point_on_map(const my_msgss::msg::Point &point, Mat &image) {
    RCLCPP_INFO(this->get_logger(), "begin to draw_point_on_map");
    Scalar scalar;
    string id;

    cout << point.id << endl;
    if (point.id <= 5 || point.id == 12)scalar = Scalar(0, 0, 255);
    else scalar = Scalar(255, 0, 0);
    circle(image, calculate_pixel_codi(point), 10,
           scalar, -1, LINE_8, 0);
    putText(image, id,
                calculate_pixel_text_codi(point), cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(0xFF, 0xFF, 0xFF), 2);
    if (point.id != 12 && point.id != 13) {
        if (point.id <= 5)id = to_string(point.id + 1);
        if (point.id == 5)id = "G";
        if (point.id >= 6)id = to_string(point.id - 5);
        if (point.id == 11)id = "G";
        putText(image, id,
                calculate_pixel_text_codi(point), cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(0xFF, 0xFF, 0xFF), 2);
        //将距离标注在小地图上
        // putText(image, distance,
        //         calculate_pixel_text_codi(point), cv::FONT_HERSHEY_SIMPLEX, 0.7,
        //         cv::Scalar(0xFF, 0xFF, 0xFF), 2);
    }
    RCLCPP_INFO(this->get_logger(), "draw_point_on_map finished");
}

// 在小地图上框出危险区域
void Game_Map::draw_warn_region(Mat &image, const vector<vector<Point>> &our_regions, const vector<vector<Point>> &enemy_regions) {
    Scalar our_scalar;
    Scalar enemy_scalar;
    if (red_or_blue == 0) {
        our_scalar = Scalar(0, 0, 255);
        enemy_scalar = Scalar(255, 0, 0);
    } else {
        enemy_scalar = Scalar(0, 0, 255);
        our_scalar = Scalar(255, 0, 0);
    }
    for (const vector<Point> &region: our_regions) {
        polylines(image, our_regions, 1, our_scalar, 2);
    }
    for (const vector<Point> &region: enemy_regions) {
        polylines(image, enemy_regions, 1, enemy_scalar, 2);
    }
}

void Game_Map::far_distPointCallback(const my_msgss::msg::Distpoints &input) {
    Mat invR;
    Mat invM;
    //求内参逆矩阵
    invert(far_CamMatrix_, invM);
    //求旋转矩阵逆矩阵
    invert(far_R, invR);
    std::vector<my_msgss::msg::Point>().swap(far_points.data);
    for (int i = 0; i < input.data.size(); i++) {
        if (input.data[i].dist > 0) {
            RCLCPP_INFO(this->get_logger(), "begin to calculate");
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) input.data[i].x, (double) input.data[i].y, 1);
            //以mm为单位，从m->mm 
            //这个相当于公式中的（u，v，1）* d
            x8_pixel *= (1000 * input.data[i].dist);
            //公式 得到三维世界坐标
            Mat calcWorld = invR * (invM * x8_pixel - far_T);//2D-3D变换
            //换算成m
            calcWorld /= 1000;
            double x = calcWorld.at<double>(0, 0);
            double y = calcWorld.at<double>(1, 0);
            //计算在场地小地图中的坐标
            x /= field_height;
            y /= field_width;
            my_msgss::msg::Point point;
            point.x = x;
            point.y = y;
            point.z = calcWorld.at<double>(2, 0);
            if (red_or_blue == 0) {
                if (input.data[i].id == 5) {
                    point.y = 0.19048;
                    our_guard.x = x * 15000;
                }
                if (input.data[i].id == 11) {
                    point.y = 0.80952;
                    enemy_guard.x = x * 15000;
                }
                if (input.data[i].id == 0) {
                    // 把英雄的世界坐标存入our_hero，并从m->mm
                    our_hero.x = point.x * 15000;
                    our_hero.y = point.y * 28000;
                    our_hero.z = point.z * 1000;
                }
            } else {
                if (input.data[i].id == 11) {
                    point.y = 0.19048;
                    our_guard.x = x * 15000;
                }
                if (input.data[i].id == 5) {
                    point.y = 0.80952;
                    enemy_guard.x = x * 15000;
                }
                if (input.data[i].id == 6) {
                    our_hero.x = point.x * 15000;
                    our_hero.y = point.y * 28000;
                    our_hero.z = point.z * 1000;
                }
            }
            point.id = input.data[i].id;
            far_points.data.push_back(point);
        }
    }
    if (outpost_calc_flag==1 && outpost_dist.dist>0 &&outpost_dist.dist<24) {
        Mat x8_pixel;
        x8_pixel = (Mat_<double>(3, 1) << (double) outpost_dist.x, (double) outpost_dist.y, 1);
        x8_pixel *= (1000 * outpost_dist.dist);
        Mat calcWorld = invR * (invM * x8_pixel - far_T);//2D-3D变换
        outpost_3d_armour.x = calcWorld.at<double>(0, 0);
        outpost_3d_armour.y = calcWorld.at<double>(1, 0);
        outpost_3d_armour.z = calcWorld.at<double>(2, 0);
    }
    RCLCPP_INFO(this->get_logger(),"end");
}

void Game_Map::far_calibration(const my_msgss::msg::Points &msg) {
    for (const auto &point: msg.data) {
        far_imagePoints[point.id] = cv::Point2d(point.x, point.y);
        far_imagePoints[point.id].x *= imgCols;
        far_imagePoints[point.id].y *= imgRows;
        cout << far_imagePoints[point.id] << endl;
    }
    cout << "已经选出了4个点!下面进行SolvePnP求解外参矩阵。" << endl;
    outpost_2d_armour.x = far_imagePoints[0].x;
    outpost_2d_armour.y = far_imagePoints[0].y;
    cv::Mat inlier;
    int suc = cv::solvePnPRansac(far_objectPoints, far_imagePoints, far_CamMatrix_, far_distCoeffs_, far_Rjacob,
                                 far_T,
                                 false, 100, 8.0, 0.99,
                                 inlier, cv::SOLVEPNP_AP3P);
    Rodrigues(far_Rjacob, far_R); //将R从雅可比形式转换为罗德里格斯形式,输出的R是3x3的一个矩阵。
    cout << "suc:" << suc << endl;
    cout << "旋转矩阵:" << far_R << endl;
    cout << "平移矩阵" << far_T << endl;
}

void Game_Map::close_distPointCallback(const my_msgss::msg::Distpoints &input) {
    Mat invR;
    Mat invM;
    invert(close_CamMatrix_, invM);
    invert(close_R, invR);
    std::vector<my_msgss::msg::Point>().swap(close_points.data);
    for (int i = 0; i < input.data.size(); i++) {
        if (input.data[i].dist > 0) {
            Mat x8_pixel;
            x8_pixel = (Mat_<double>(3, 1) << (double) input.data[i].x, (double) input.data[i].y, 1);
            x8_pixel *= (1000 * input.data[i].dist);
            Mat calcWorld = invR * (invM * x8_pixel - close_T);//2D-3D变换
            calcWorld /= 1000;
            double x = calcWorld.at<double>(0, 0);
            double y = calcWorld.at<double>(1, 0);
            x /= field_height;
            y /= field_width;
            my_msgss::msg::Point point;
            point.x = x;
            point.y = y;
            point.z = calcWorld.at<double>(2, 0);
            if (red_or_blue == 0) {
                if (input.data[i].id == 5) {
                    point.y = 0.19048;
                    our_guard.x = x * 15000;
                }
                if (input.data[i].id == 11) {
                    point.y = 0.80952;
                    enemy_guard.x = x * 15000;
                }
                if (input.data[i].id == 0) {
                    our_hero.x = point.x * 15000;
                    our_hero.y = point.y * 28000;
                    our_hero.z = point.z * 1000;
                }
            } else {
                if (input.data[i].id == 11) {
                    point.y = 0.19048;
                    our_guard.x = x * 15000;
                }
                if (input.data[i].id == 5) {
                    point.y = 0.80952;
                    enemy_guard.x = x * 15000;
                }
                if (input.data[i].id == 6) {
                    our_hero.x = point.x * 15000;
                    our_hero.y = point.y * 28000;
                    our_hero.z = point.z * 1000;
                }
            }
            point.id = input.data[i].id;
            close_points.data.push_back(point);
        }
    }
}

void Game_Map::close_calibration(const my_msgss::msg::Points &msg) {
    for (const auto &point: msg.data) {
        close_imagePoints[point.id] = cv::Point2d(point.x, point.y);
        close_imagePoints[point.id].x *= imgCols;
        close_imagePoints[point.id].y *= imgRows;
        cout << close_imagePoints[point.id] << endl;
    }
    cout << "已经选出了4个点!下面进行SolvePnP求解外参矩阵。" << endl;
    cv::Mat inlier;
    cout << "close obj points:" << close_objectPoints << endl;
    int suc = cv::solvePnPRansac(close_objectPoints, close_imagePoints, close_CamMatrix_, close_distCoeffs_,
                                 close_Rjacob, close_T, false, 100, 8.0, 0.99,
                                 inlier, cv::SOLVEPNP_AP3P);
    Rodrigues(close_Rjacob, close_R); //将R从雅可比形式转换为罗德里格斯形式,输出的R是3x3的一个矩阵。
    cout << "suc:" << suc << endl;
    cout << "旋转矩阵:" << close_R << endl;
    cout << "平移矩阵" << close_T << endl;
}

void Game_Map::timer_callback()
{
    if(rclcpp::ok())
    {
        small_map.copyTo(small_map_copy);
        //draw_warn_region(small_map_copy, our_warn_regions, enemy_warn_regions);
        warn_on_map(result_points, small_map_copy);
        //remove_duplicate();
        vector<my_msgss::msg::Point>().swap(result_points.data);
        for (auto &i: far_points.data) {
            result_points.data.emplace_back(i);
        }
        warn_on_map(result_points, small_map_copy);
        for (auto &i: result_points.data) {
            draw_point_on_map(i, small_map_copy);
        }
            world_points_pub_->publish(result_points);
        if (!guard_relative.data.empty()) {
            Point2f ab;
            ab.x = (guard_relative.data[0].x + our_guard.x) / 15000 * 450 - X_shift;
            ab.y = 840 - (guard_relative.data[0].y + our_guard.y) / 28000 * 840 - Y_shift;

            //cout << (int) in_our_base_cnt << endl;
            // pointPolygonTest用于判断一个点是否在多边形内部，返回值为正数则在内部，为负数则在外部，为0则在边界上
            // measureDist如果是Ture 则返回 positive value if the point is inside the contour, negative value if outside, zero if on the contour
            if (pointPolygonTest(guidao_houbian, ab, false) >= 0) {
                in_our_base_cnt++;
                if (in_our_base_cnt > 10) {
                    in_our_base_cnt = 10;
                    guard_points_pub_->publish(guard_relative);
                }
                circle(small_map_copy, ab, 10, Scalar(255, 255, 255), -1, LINE_8, 0);
            } else if (pointPolygonTest(guard_forbidden_zone, ab, false) <= 0) {
                in_our_base_cnt = 0;
                guard_relative.data[0].x+=300;
                guard_relative.data[0].y+=250;
                guard_relative.data[0].z-=300;
                guard_points_pub_->publish(guard_relative);
                circle(small_map_copy, ab, 10, Scalar(255, 255, 255), -1, LINE_8, 0);
            } else {
                circle(small_map_copy, ab, 10, Scalar(0, 255, 255), -1, LINE_8, 0);
            }
        } else {
            my_msgss::msg::Point abc;
            abc.x = 30000;
            abc.y = 30000;
            guard_relative.data.emplace_back(abc);
            guard_points_pub_->publish(guard_relative);
        }
        if(outpost_3d_armour.x>0&&our_hero.x>0){
            float h_o_dist = sqrt(pow(outpost_3d_armour.x - our_hero.x, 2) + pow(outpost_3d_armour.y - our_hero.y, 2)+ pow(outpost_3d_armour.z-our_hero.z,2));
            h_o_dist-=609;
            my_msgss::msg::Point hero;
            hero.x=h_o_dist;
            hero_point_pub_->publish(hero);
        }
        else {
            my_msgss::msg::Point hero;
            hero.x=30000;
            hero_point_pub_->publish(hero);
        }
        imshow("small_map", small_map_copy);
        waitKey(1);
    }
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Game_Map>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}