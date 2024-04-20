#include "number_classifier.hpp"

NumberClassifier::NumberClassifier()
{

}

NumberClassifier::NumberClassifier(string model_path, string label_path, float threshold)
 : threshold(threshold)
{
    armor_net_ = cv::dnn::readNetFromONNX(model_path);
    std::ifstream label_file;
    label_file.open(label_path,ios::in);
    if(!label_file.is_open())
    {
        std::cout << "打开文件失败！" << std::endl;
    }
    std::string line;
    while(std::getline(label_file, line))
    {
        class_names_.push_back(line);
    }
}

void NumberClassifier::init(string model_path, string label_path, float threshold)
{
    this->threshold = threshold;
    armor_net_ = cv::dnn::readNetFromONNX(model_path);
    std::ifstream label_file;
    label_file.open(label_path,ios::in);
    if(!label_file.is_open())
    {
        std::cout << "打开文件失败！" << std::endl;
    }
    std::string line;
    while(std::getline(label_file, line))
    {
        class_names_.push_back(line);
    }
}

void NumberClassifier::classify(const Mat &armor_img)
{
    cv::Mat image = armor_img;
    image = image / 255.0;
    cv::Mat blob;
    cv::dnn::blobFromImage(image,blob);
    armor_net_.setInput(blob);
    cv::Mat outputs = armor_net_.forward();
    // Do softmax
    float max_prob = *std::max_element(outputs.begin<float>(), outputs.end<float>());
    cv::Mat softmax_prob;
    cv::exp(outputs - max_prob, softmax_prob);
    float sum = static_cast<float>(cv::sum(softmax_prob)[0]);
    softmax_prob /= sum;

    double confidence;
    cv::Point class_id_point;
    minMaxLoc(softmax_prob.reshape(1, 1), nullptr, &confidence, nullptr, &class_id_point);
    int label_id = class_id_point.x;

    armor_confidence = confidence;
    armor_number = class_names_[label_id];

    if(armor_confidence < threshold)
    {
        armor_number = "-1";
    }
}
