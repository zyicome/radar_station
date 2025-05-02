#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QVector>
#include <vector>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

namespace Ui {
class parameterWidget;
}

class parameterWidget : public QWidget
{
    Q_OBJECT

public:
    parameterWidget();
    ~parameterWidget();
    void uiParameterUpdate();

signals:
    void toSaveCloseCameraMatrixParameter(cv::Mat close_camera_matrix);
    void toSaveCloseDistortionCoefficientParameter(cv::Mat close_distortion_coefficient);
    void toSaveCloseUniMatrixParameter(cv::Mat close_uni_matrix);
    void toSaveFarCameraMatrixParameter(cv::Mat far_camera_matrix);
    void toSaveFarDistortionCoefficientParameter(cv::Mat far_distortion_coefficient);
    void toSaveFarUniMatrixParameter(cv::Mat far_uni_matrix);
    void toSaveMapParameter(float object_height, float object_width);
    void toSaveImageSizeParameter(int image_cols, int image_rows);

public slots:
    void saveCloseCameraMatrix();

    void saveCloseDistortionCoefficient();

    void saveCloseUniMatrix();

    void saveFarCameraMatrix();

    void saveFarDistortionCoefficient();

    void saveFarUniMatrix();

    void saveMapParameter();

    void saveImageSizeParameter();

public:
    Ui::parameterWidget *ui;

    cv::Mat close_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat close_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);
    cv::Mat close_uni_matrix = cv::Mat::zeros(3, 4, CV_64FC1);
    cv::Mat far_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat far_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);
    cv::Mat far_uni_matrix = cv::Mat::zeros(3, 4, CV_64FC1);
    float object_height = 0.0;
    float object_width = 0.0;
    int image_cols = 0; //宽
    int image_rows = 0; //高
};

#endif // PARAMETERWIDGET_H
