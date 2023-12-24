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

    // 1. �������onnxģ��
    // Note that in this example the classes are hard-coded and 'classes.txt' is a place holder.
    Inference inf("D:/CodePython/ultralytics/yolov8s.onnx", cv::Size(640, 480), "classes.txt", runOnGPU); // classes.txt ����ȱʧ

    // 2. �����������ͼƬ
    std::vector<std::string> imageNames;
    imageNames.push_back("bus.jpg");
    //imageNames.push_back("zidane.jpg");

    for (int i = 0; i < imageNames.size(); ++i)
    {
        cv::Mat frame = cv::imread(imageNames[i]);

        // Inference starts here...
        std::vector<Detection> output = inf.runInference(frame);

        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        // feiyull
        // ������Ҫresize�£�����������
        cv::resize(frame, frame, cv::Size(480, 640));

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
