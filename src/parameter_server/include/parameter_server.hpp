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

using namespace std;
using namespace cv;

class Server : public::rclcpp::Node
{
public:
    Server();

    void set_camera_matrix_parameter();

    void set_map_parameter();

    void test();
};

