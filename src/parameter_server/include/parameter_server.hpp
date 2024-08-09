#include <sensor_msgs/msg/image.hpp>
#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "my_msgss/msg/points.hpp"
#include "my_msgss/msg/point.hpp"
#include "my_msgss/msg/distpoints.hpp"
#include "my_msgss/msg/distpoint.hpp"

#include <bitset>
#include "std_msgs/msg/float32_multi_array.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "tinyxml2.h"

using namespace std;
using namespace cv;
using namespace tinyxml2;

class Server : public::rclcpp::Node
{
public:
    Server();

    void set_camera_matrix_parameter();

    void set_map_parameter();

    void test();

    bool read_xml();

    int img_cols;
    int img_rows;
    double object_height;
    double object_width;

    std::vector<double> far_camera_matrix;
    std::vector<double> far_distortion_coefficient;
    std::vector<double> far_uni_matrix;

    std::vector<double> close_camera_matrix;
    std::vector<double> close_distortion_coefficient;
    std::vector<double> close_uni_matrix;
};

