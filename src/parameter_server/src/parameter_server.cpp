#include "parameter_server.hpp"

Server::Server() : Node("parameter_server",rclcpp::NodeOptions().allow_undeclared_parameters(true))
{
    set_camera_matrix_parameter();

    set_map_parameter();

    //test();
}

void Server::set_camera_matrix_parameter()
{
    //far_camera_matrix
    this->declare_parameter("far_camera_matrix_one",3085.70228);
    this->declare_parameter("far_camera_matrix_two",0.0);
    this->declare_parameter("far_camera_matrix_three",0.0);
    this->declare_parameter("far_camera_matrix_four",0.0);
    this->declare_parameter("far_camera_matrix_five",3121.25945);
    this->declare_parameter("far_camera_matrix_six",936.7318);
    this->declare_parameter("far_camera_matrix_seven",0.0);
    this->declare_parameter("far_camera_matrix_eight",0.0);
    this->declare_parameter("far_camera_matrix_nine",1.0);

    //far_distortion_coefficient
    this->declare_parameter("far_distortion_coefficient_one",0.019831);
    this->declare_parameter("far_distortion_coefficient_two",-0.098189);
    this->declare_parameter("far_distortion_coefficient_three",0.035928);
    this->declare_parameter("far_distortion_coefficient_four",-0.007875);
    this->declare_parameter("far_distortion_coefficient_five",0.0);

    //far_uni_matrix
    this->declare_parameter("far_uni_matrix_one",0.250572);
    this->declare_parameter("far_uni_matrix_two",-0.965757);
    this->declare_parameter("far_uni_matrix_three",0.0672872);
    this->declare_parameter("far_uni_matrix_four",-1.06878);
    this->declare_parameter("far_uni_matrix_five",-0.064873);
    this->declare_parameter("far_uni_matrix_six",-0.086098);
    this->declare_parameter("far_uni_matrix_seven",-0.994172);
    this->declare_parameter("far_uni_matrix_eight",1.08281);
    this->declare_parameter("far_uni_matrix_nine",0.965922);
    this->declare_parameter("far_uni_matrix_ten",0.244747);
    this->declare_parameter("far_uni_matrix_eleven",-0.0842255);
    this->declare_parameter("far_uni_matrix_twelve",-0.54173);

    //close_camera_matrix
    this->declare_parameter("close_camera_matrix_one",1563.52174);
    this->declare_parameter("close_camera_matrix_two",0.0);
    this->declare_parameter("close_camera_matrix_three",0.0);
    this->declare_parameter("close_camera_matrix_four",0.0);
    this->declare_parameter("close_camera_matrix_five",626.90356);
    this->declare_parameter("close_camera_matrix_six",0.0);
    this->declare_parameter("close_camera_matrix_seven",0.0);
    this->declare_parameter("close_camera_matrix_eight",0.0);
    this->declare_parameter("close_camera_matrix_nine",1.0);

    //close_distortion_coefficient
    this->declare_parameter("close_distortion_coefficient_one",-0.063200);
    this->declare_parameter("close_distortion_coefficient_two",-0.005061);
    this->declare_parameter("close_distortion_coefficient_three",-0.001755);
    this->declare_parameter("close_distortion_coefficient_four",0.003472);
    this->declare_parameter("close_distortion_coefficient_five",0.0);

    //close_uni_matrix
    this->declare_parameter("close_uni_matrix_one",-0.00872172);
    this->declare_parameter("close_uni_matrix_two",-0.9994);
    this->declare_parameter("close_uni_matrix_three",0.0335313);
    this->declare_parameter("close_uni_matrix_four",0.314484);
    this->declare_parameter("close_uni_matrix_five",0.0346761);
    this->declare_parameter("close_uni_matrix_six",-0.0338147);
    this->declare_parameter("close_uni_matrix_seven",-0.998826);
    this->declare_parameter("close_uni_matrix_eight",0.637911);
    this->declare_parameter("close_uni_matrix_nine",0.999361);
    this->declare_parameter("close_uni_matrix_ten",-0.00754875);
    this->declare_parameter("close_uni_matrix_eleven",0.0349502);
    this->declare_parameter("close_uni_matrix_twelve",-1.57492);
}

void Server::set_map_parameter()
{
    //地图实际大小
    this->declare_parameter("object_height",28);
    this->declare_parameter("object_width",15);

    //传输过程中的图片大小
    this->declare_parameter("image_row",28);
    this->declare_parameter("image_col",15);
}

void Server::test()
{
    double close_uni_matrix_eight = this->get_parameter("close_uni_matrix_eight").as_double();
    std::cout << "close_uni_matrix_eight: " <<close_uni_matrix_eight <<std::endl;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto paramServer= std::make_shared<Server>();
    rclcpp::spin(paramServer);
    rclcpp::shutdown();
    return 0;
}