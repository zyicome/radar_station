#include <iostream>
#include <vector>
//#include <getopt.h>

#include <opencv2/opencv.hpp>

#include "inference.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    bool runOnGPU = false;

    // 1. 设置你的onnx模型
    // Note that in this example the classes are hard-coded and 'classes.txt' is a place holder.
    Inference inf_armor("/home/mechax/zyb/radar_station/src/Img_Handle/Yolov8_weight/armor/weights/best.onnx",
                        cv::Size(640, 640),
                        "/home/mechax/zyb/radar_station/src/Img_Handle/Yolov8_weight/armor/class/class.txt",
                        runOnGPU); // classes.txt 可以缺失
    Inference inf_robot("/home/mechax/zyb/radar_station/src/Img_Handle/Yolov8_weight/robot/weights/best.onnx",
                       cv::Size(640, 640),
                       "/home/mechax/zyb/radar_station/src/Img_Handle/Yolov8_weight/robot/class/class.txt",
                       runOnGPU);
    // 2. 设置你的输入图片
    std::vector<std::string> imageNames;
    imageNames.push_back("/home/mechax/zyb/radar_station/src/Img_Handle/demo_picture/3.jpg");
    //imageNames.push_back("zidane.jpg");

    for (int i = 0; i < imageNames.size(); ++i)
    {
        cv::Mat frame = cv::imread(imageNames[i]);
        // Inference starts here...
        std::vector<Detection> output = inf_robot.runInference(frame);

        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        // feiyull
        // 这里需要resize下，否则结果不对
        cv::resize(frame, frame, cv::Size(frame.cols, frame.rows));

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];

            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;

            // Detection box
            cv::rectangle(frame, box, color, 2);

            // Detection box text
            std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
            cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
            cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

            cv::rectangle(frame, textBox, color, cv::FILLED);
            cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
        }
        cv::imshow("Inference", frame);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
}
