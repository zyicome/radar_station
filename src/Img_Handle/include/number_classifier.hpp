#include "opencv2/opencv.hpp"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class NumberClassifier
{
public:
    NumberClassifier();

    NumberClassifier(string model_path, string label_path, float threshold);

    void init(string model_path, string label_path, float threshold);

    void classify(const Mat &armor_img);

    float threshold;

    cv::dnn::Net armor_net_;
    std::vector<std::string> class_names_;

    float armor_confidence;
    string armor_number;
};