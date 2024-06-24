#include "parameter_server.hpp"

Server::Server() : Node("parameter_server",rclcpp::NodeOptions().allow_undeclared_parameters(true))
{
    if(read_xml() == 1)
    {
        set_camera_matrix_parameter();

        set_map_parameter();

        test();
    }
}

void Server::set_camera_matrix_parameter()
{
    //far_camera_matrix
    this->declare_parameter("far_camera_matrix_one",far_camera_matrix.at(0));
    this->declare_parameter("far_camera_matrix_two",far_camera_matrix.at(1));
    this->declare_parameter("far_camera_matrix_three",far_camera_matrix.at(2));
    this->declare_parameter("far_camera_matrix_four",far_camera_matrix.at(3));
    this->declare_parameter("far_camera_matrix_five",far_camera_matrix.at(4));
    this->declare_parameter("far_camera_matrix_six",far_camera_matrix.at(5));
    this->declare_parameter("far_camera_matrix_seven",far_camera_matrix.at(6));
    this->declare_parameter("far_camera_matrix_eight",far_camera_matrix.at(7));
    this->declare_parameter("far_camera_matrix_nine",far_camera_matrix.at(8));

    //far_distortion_coefficient
    this->declare_parameter("far_distortion_coefficient_one",far_distortion_coefficient.at(0));
    this->declare_parameter("far_distortion_coefficient_two",far_distortion_coefficient.at(1));
    this->declare_parameter("far_distortion_coefficient_three",far_distortion_coefficient.at(2));
    this->declare_parameter("far_distortion_coefficient_four",far_distortion_coefficient.at(3));
    this->declare_parameter("far_distortion_coefficient_five",far_distortion_coefficient.at(4));

    //far_uni_matrix
    this->declare_parameter("far_uni_matrix_one",far_uni_matrix.at(0));
    this->declare_parameter("far_uni_matrix_two",far_uni_matrix.at(1));
    this->declare_parameter("far_uni_matrix_three",far_uni_matrix.at(2));
    this->declare_parameter("far_uni_matrix_four",far_uni_matrix.at(3));
    this->declare_parameter("far_uni_matrix_five",far_uni_matrix.at(4));
    this->declare_parameter("far_uni_matrix_six",far_uni_matrix.at(5));
    this->declare_parameter("far_uni_matrix_seven",far_uni_matrix.at(6));
    this->declare_parameter("far_uni_matrix_eight",far_uni_matrix.at(7));
    this->declare_parameter("far_uni_matrix_nine",far_uni_matrix.at(8));
    this->declare_parameter("far_uni_matrix_ten",far_uni_matrix.at(9));
    this->declare_parameter("far_uni_matrix_eleven",far_uni_matrix.at(10));
    this->declare_parameter("far_uni_matrix_twelve",far_uni_matrix.at(11));

    //close_camera_matrix
    this->declare_parameter("close_camera_matrix_one",close_camera_matrix.at(0));
    this->declare_parameter("close_camera_matrix_two",close_camera_matrix.at(1));
    this->declare_parameter("close_camera_matrix_three",close_camera_matrix.at(2));
    this->declare_parameter("close_camera_matrix_four",close_camera_matrix.at(3));
    this->declare_parameter("close_camera_matrix_five",close_camera_matrix.at(4));
    this->declare_parameter("close_camera_matrix_six",close_camera_matrix.at(5));
    this->declare_parameter("close_camera_matrix_seven",close_camera_matrix.at(6));
    this->declare_parameter("close_camera_matrix_eight",close_camera_matrix.at(7));
    this->declare_parameter("close_camera_matrix_nine",close_camera_matrix.at(8));

    //close_distortion_coefficient
    this->declare_parameter("close_distortion_coefficient_one",close_distortion_coefficient.at(0));
    this->declare_parameter("close_distortion_coefficient_two",close_distortion_coefficient.at(1));
    this->declare_parameter("close_distortion_coefficient_three",close_distortion_coefficient.at(2));
    this->declare_parameter("close_distortion_coefficient_four",close_distortion_coefficient.at(3));
    this->declare_parameter("close_distortion_coefficient_five",close_distortion_coefficient.at(4));

    //close_uni_matrix
    this->declare_parameter("close_uni_matrix_one",close_uni_matrix.at(0));
    this->declare_parameter("close_uni_matrix_two",close_uni_matrix.at(1));
    this->declare_parameter("close_uni_matrix_three",close_uni_matrix.at(2));
    this->declare_parameter("close_uni_matrix_four",close_uni_matrix.at(3));
    this->declare_parameter("close_uni_matrix_five",close_uni_matrix.at(4));
    this->declare_parameter("close_uni_matrix_six",close_uni_matrix.at(5));
    this->declare_parameter("close_uni_matrix_seven",close_uni_matrix.at(6));
    this->declare_parameter("close_uni_matrix_eight",close_uni_matrix.at(7));
    this->declare_parameter("close_uni_matrix_nine",close_uni_matrix.at(8));
    this->declare_parameter("close_uni_matrix_ten",close_uni_matrix.at(9));
    this->declare_parameter("close_uni_matrix_eleven",close_uni_matrix.at(10));
    this->declare_parameter("close_uni_matrix_twelve",close_uni_matrix.at(11));
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
    double close_uni_matrix_seven = this->get_parameter("close_uni_matrix_seven").as_double();
    std::cout << "close_uni_matrix_eight: " <<close_uni_matrix_eight <<std::endl;
    std::cout << "close_uni_matrix_seven: " <<close_uni_matrix_seven <<std::endl;

}

bool Server::read_xml()
{
    string file_path = "/home/mechax/radar_station/src/parameter_server/xml/parameter.xml";
    //创建xml文件对象
    XMLDocument *doc = new XMLDocument(file_path.c_str());

    //判断文件是否正确
    if(doc->LoadFile(file_path.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
    {
        cout << "文件错误！！！" << endl;
        delete doc;
        return 0;
    }

    //获取根元素    
    XMLElement *root = doc->RootElement();

    //获取子元素
    XMLElement *far_camera_matrix_element = root->FirstChildElement("far_camera_matrix");
    XMLElement *far_distortion_coefficient_element = root->FirstChildElement("far_distortion_coefficient");
    XMLElement *far_uni_matrix_element = root->FirstChildElement("far_uni_matrix");

    XMLElement *close_camera_matrix_element = root->FirstChildElement("close_camera_matrix");
    XMLElement *close_distortion_coefficient_element = root->FirstChildElement("close_distortion_coefficient");
    XMLElement *close_uni_matrix_element = root->FirstChildElement("close_uni_matrix");

    //获取子元素的值
    if (far_camera_matrix_element != nullptr) {
        for(XMLElement *element = far_camera_matrix_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            far_camera_matrix.push_back(value);
        }
    }
    else
    {
        cout << "far_camera_matrix_element is null" << endl;
    }
    
    if (far_distortion_coefficient_element != nullptr) {
        for(XMLElement *element = far_distortion_coefficient_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            far_distortion_coefficient.push_back(value);
        }
    }
    else
    {
        cout << "far_distortion_coefficient_element is null" << endl;
    }

    if (far_uni_matrix_element != nullptr) {
        for(XMLElement *element = far_uni_matrix_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            far_uni_matrix.push_back(value);
        }
    }
    else
    {
        cout << "far_uni_matrix_element is null" << endl;
    }

    if (close_camera_matrix_element != nullptr) {
        for(XMLElement *element = close_camera_matrix_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            close_camera_matrix.push_back(value);
        }
    }
    else
    {
        cout << "close_camera_matrix_element is null" << endl;
    }

    if (close_distortion_coefficient_element != nullptr) {
        for(XMLElement *element = close_distortion_coefficient_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            close_distortion_coefficient.push_back(value);
        }
    }
    else
    {
        cout << "close_distortion_coefficient_element is null" << endl;
    }

    if (close_uni_matrix_element != nullptr) {
        for(XMLElement *element = close_uni_matrix_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            double value = stod(element->GetText());
            close_uni_matrix.push_back(value);
        }
    }
    else
    {
        cout << "close_uni_matrix_element is null" << endl;
    }

    delete doc;
    return 1;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto paramServer= std::make_shared<Server>();
    rclcpp::spin(paramServer);
    rclcpp::shutdown();
    return 0;
}