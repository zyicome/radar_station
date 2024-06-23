#ifndef PNPWIDGET_H
#define PNPWIDGET_H

#include <QWidget>
#include <QVector>
#include <vector>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

namespace Ui {
class pnpWidget;
}

class pnpWidget : public QWidget
{
    Q_OBJECT

public:
    pnpWidget();
    ~pnpWidget();

    //void mousePressEvent(QMouseEvent *event) override;

    //void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event);

    void mapMessageDisplay(QString text);

    void parameter_init();
Q_SIGNALS:
    void pnpFinished();

public Q_SLOTS:
    void beginPoint();

    void restartPoint();

    void beginPnp();

    void calibrationClose();

    void calibrationFar();

    void blueMode();

    void redMode();
public:
    Ui::pnpWidget *ui;
    QVector<QPointF> points;
    std::vector<Point3d> close_objectpoints;
    std::vector<Point3d> far_objectpoints;
    cv::Mat far_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat far_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);
    cv::Mat far_Rjacob = cv::Mat::zeros(3, 1, CV_64FC1);
    cv::Mat far_R = cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat far_T = cv::Mat::zeros(3, 1, CV_64FC1);

    cv::Mat close_camera_matrix =cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat close_distortion_coefficient =cv::Mat::zeros(5, 1, CV_64FC1);
    cv::Mat close_Rjacob = cv::Mat::zeros(3, 1, CV_64FC1);
    cv::Mat close_R = cv::Mat::zeros(3, 3, CV_64FC1);
    cv::Mat close_T = cv::Mat::zeros(3, 1, CV_64FC1);

    QPointF mousePos;
    QPointF lastPos;
    QPixmap farimage;
    QPixmap closeimage;
    bool isBeginPoint;
    float pnp_img_id;
    bool our_color;
};

#endif // PNPWIDGET_H
