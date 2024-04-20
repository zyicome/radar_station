#include "pnpwidget.h"
#include "ui_pnpwidget.h"
#include <QPainter>
#include <QPen>
#include <iostream>
#include <QMouseEvent>
#include <QDialog>
#include <QPixmap>
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>

#include <ament_index_cpp/get_package_share_directory.hpp>

// #include "opencv2/opencv.hpp"
// #include <iostream>
// #include <vector>

pnpWidget::pnpWidget() :
    ui(new Ui::pnpWidget)
{
    ui->setupUi(this);
    ui->closeImg->installEventFilter(this);
    ui->farImg->installEventFilter(this);

    QString img_path = QString::fromStdString(ament_index_cpp::get_package_share_directory("Qt5_displayer"));
    QString redimage_path = img_path + "/map/redmap.png";
    QString blueimage_path = img_path + "/map/bluemap.png";

    //QString image_path = QFileDialog::getOpenFileName(this, "打开图片", ".", "Images (*.png *.xpm *.jpg)");
    closeimage.load(redimage_path);
    farimage.load(redimage_path);
    this->isBeginPoint = false;
    this->pnp_img_id=-1;

<<<<<<< HEAD
    connect(ui->beginPoint,SIGNAL(clicked()),this,SLOT(beginPoint()));
    connect(ui->restartPoint,SIGNAL(clicked()),this,SLOT(restartPoint()));
    connect(ui->beginPnp,SIGNAL(clicked()),this,SLOT(beginPnp()));

    parameter_init();
=======
    parameter_init();

    connect(ui->beginPoint,SIGNAL(clicked()),this,SLOT(beginPoint()));
    connect(ui->restartPoint,SIGNAL(clicked()),this,SLOT(restartPoint()));
    connect(ui->beginPnp,SIGNAL(clicked()),this,SLOT(beginPnp()));
>>>>>>> 431e9f5 (2024.4.20)
}

pnpWidget::~pnpWidget()
{
    delete ui;
}

void pnpWidget::parameter_init()
{
<<<<<<< HEAD
    far_camera_matrix.at<float>(0, 0) = 3135.31292;
    far_camera_matrix.at<float>(0, 1) = 0;
    far_camera_matrix.at<float>(0, 2) = 526.87116;
    far_camera_matrix.at<float>(1, 0) = 0;
    far_camera_matrix.at<float>(1, 1) = 3151.06425;
    far_camera_matrix.at<float>(1, 2) = 695.83061;
    far_camera_matrix.at<float>(2, 0) = 0;
    far_camera_matrix.at<float>(2, 1) = 0;
    far_camera_matrix.at<float>(2, 2) = 1;
    far_distortion_coefficient.at<float>(0,0) = -0.019203;
    far_distortion_coefficient.at<float>(1,0) = 0.252109;
    far_distortion_coefficient.at<float>(2,0) = 0.016576;
    far_distortion_coefficient.at<float>(3,0) = -0.012270;
    far_distortion_coefficient.at<float>(4,0) = 0.000000;

    close_camera_matrix.at<float>(0, 0) = 1563.52174;
    close_camera_matrix.at<float>(0, 1) = 0;
    close_camera_matrix.at<float>(0, 2) = 626.90356;
    close_camera_matrix.at<float>(1, 0) = 0;
    close_camera_matrix.at<float>(1, 1) = 1568.90028;
    close_camera_matrix.at<float>(1, 2) = 488.93524;
    close_camera_matrix.at<float>(2, 0) = 0;
    close_camera_matrix.at<float>(2, 1) = 0;
    close_camera_matrix.at<float>(2, 2) = 1;
    close_distortion_coefficient.at<float>(0,0) = -0.063200;
    close_distortion_coefficient.at<float>(1,0) = -0.005061;
    close_distortion_coefficient.at<float>(2,0) = -0.001755;
    close_distortion_coefficient.at<float>(3,0) = 0.003472;
    close_distortion_coefficient.at<float>(4,0) = 0.000000;

    Point3d objectpoint_one;
    objectpoint_one.x = 2350.0;
    objectpoint_one.y = 7000.0;
    objectpoint_one.z = 0.0;
    objectPoints.push_back(objectpoint_one);
    Point3d objectpoint_two;
    objectpoint_two.x = 2350.0;
    objectpoint_two.y = 7000.0;
    objectpoint_two.z = 500.0;
    objectPoints.push_back(objectpoint_two);
    Point3d objectpoint_three;
    objectpoint_three.x = 4750.0;
    objectpoint_three.y = 6800.0;
    objectpoint_three.z = 500.0;
    objectPoints.push_back(objectpoint_three);
    Point3d objectpoint_four;
    objectpoint_four.x = 4750.0;
    objectpoint_four.y = 6800.0;
    objectpoint_four.z = 0.0;
=======
    far_camera_matrix.at<double>(0, 0) = 3135.31292;
    far_camera_matrix.at<double>(0, 1) = 0;
    far_camera_matrix.at<double>(0, 2) = 526.87116;
    far_camera_matrix.at<double>(1, 0) = 0;
    far_camera_matrix.at<double>(1, 1) = 3151.06425;
    far_camera_matrix.at<double>(1, 2) = 695.83061;
    far_camera_matrix.at<double>(2, 0) = 0.0;
    far_camera_matrix.at<double>(2, 1) = 0.0;
    far_camera_matrix.at<double>(2, 2) = 1.0;
    far_distortion_coefficient.at<double>(0,0) = -0.019203;
    far_distortion_coefficient.at<double>(1,0) = 0.252109;
    far_distortion_coefficient.at<double>(2,0) = 0.016576;
    far_distortion_coefficient.at<double>(3,0) = -0.012270;
    far_distortion_coefficient.at<double>(4,0) = 0.000000;

    close_camera_matrix.at<double>(0, 0) = 1563.52174;
    close_camera_matrix.at<double>(0, 1) = 0;
    close_camera_matrix.at<double>(0, 2) = 626.90356;
    close_camera_matrix.at<double>(1, 0) = 0;
    close_camera_matrix.at<double>(1, 1) = 1568.90028;
    close_camera_matrix.at<double>(1, 2) = 488.93524;
    close_camera_matrix.at<double>(2, 0) = 0;
    close_camera_matrix.at<double>(2, 1) = 0;
    close_camera_matrix.at<double>(2, 2) = 1;
    close_distortion_coefficient.at<double>(0,0) = -0.063200;
    close_distortion_coefficient.at<double>(1,0) = -0.005061;
    close_distortion_coefficient.at<double>(2,0) = -0.001755;
    close_distortion_coefficient.at<double>(3,0) = 0.003472;
    close_distortion_coefficient.at<double>(4,0) = 0.000000;

    /*cout << "far_camera_matrix:" << far_camera_matrix << endl;
    cout << "far_distortion_coefficient:" << far_distortion_coefficient << endl;
    cout << "close_camera_matrix:" << close_camera_matrix << endl;
    cout << "close_distortion_coefficient:" << close_distortion_coefficient << endl;*/

    Point3d objectpoint_one;
    objectpoint_one.x = 2350.0 / 1000;
    objectpoint_one.y = 7000.0 / 1000;
    objectpoint_one.z = 0.0 / 1000;
    objectPoints.push_back(objectpoint_one);
    Point3d objectpoint_two;
    objectpoint_two.x = 2350.0 / 1000;
    objectpoint_two.y = 7000.0 / 1000;
    objectpoint_two.z = 500.0 / 1000;
    objectPoints.push_back(objectpoint_two);
    Point3d objectpoint_three;
    objectpoint_three.x = 4750.0 / 1000;
    objectpoint_three.y = 6800.0 / 1000;
    objectpoint_three.z = 500.0 / 1000;
    objectPoints.push_back(objectpoint_three);
    Point3d objectpoint_four;
    objectpoint_four.x = 4750.0 / 1000;
    objectpoint_four.y = 6800.0 / 1000;
    objectpoint_four.z = 0.0 / 1000;
>>>>>>> 431e9f5 (2024.4.20)
    objectPoints.push_back(objectpoint_four);
}

bool pnpWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->closeImg)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                if(isBeginPoint)
                {
                    pnp_img_id = 1;
                    int pointNum = this->points.size();
                    QString pointText = "第" + QString::number(pointNum + 1) + "个点的坐标为 x:";
                    pointText = pointText + QString::number(mouseEvent->x());
                    pointText = pointText + "      y:" + QString::number(mouseEvent->y());
                    mapMessageDisplay(pointText);
                    /*if(pointNum < 5)
                    {
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->closeImg->update();
                    }
                    else
                    {
                        mapMessageDisplay("Too many points");
                    }*/
                    switch (pointNum)
                    {
                    case 0:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->closeImg->update();
                        mapMessageDisplay("请点击第二个标定点");
                        break;
                    case 1:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->closeImg->update();
                        mapMessageDisplay("请点击第三个标定点");
                        break;
                    case 2:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->closeImg->update();
                        mapMessageDisplay("请点击第四个标定点");
                        break;
                    case 3:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->closeImg->update();
                        mapMessageDisplay("选点完成，请选择是否开始pnp解算");
                        isBeginPoint = false;
                        break;
                    default:
                        mapMessageDisplay("够了够了，不要点了");
                    }
                }
            }
        }
        if(event->type() == QEvent::Paint)
        { 
            QPainter painter(ui->closeImg);
            painter.setPen(QPen(Qt::black,2));
            QRect rect(0,0,ui->closeImg->width(),ui->closeImg->height());
            //std::cout << "绘画camerimage" << std::endl;
            painter.drawPixmap(rect,closeimage);
            //qDebug() << "x: " << ui->closeImg->x() << "y: " << ui->closeImg->y();
            //qDebug() << "width: " << ui->closeImg->width() << "height: " << ui->closeImg->height();

            int pointNum = this->points.size();
            if(pointNum == 0)
            {

            }
            else if(pointNum == 1)
            {
                painter.drawEllipse(mousePos,5,5);
            }
            else
            {
                for(int i = 0;i<pointNum;i++)
                {
                    painter.drawEllipse(points[i],5,5);
                    if(i == 3)
                    {
                        painter.drawLine(points[i],points[0]);
                    }
                    if(i != pointNum - 1)
                    {
                        painter.drawLine(points[i],points[i+1]);
                    }
                }
            }
            //qDebug() << "x: " << mousePos.x() << "y: " << mousePos.y();
            return true;
        }
        return false;
    }
    if(obj == ui->farImg)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                if(isBeginPoint)
                {
                    pnp_img_id = 2;
                    int pointNum = this->points.size();
                    QString pointText = "第" + QString::number(pointNum + 1) + "个点的坐标为 x:";
                    pointText = pointText + QString::number(mouseEvent->x());
                    pointText = pointText + "      y:" + QString::number(mouseEvent->y());
                    mapMessageDisplay(pointText);
                    /*if(pointNum < 5)
                    {
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->farImg->update();
                    }
                    else
                    {
                        mapMessageDisplay("Too many points");
                    }*/
                    switch (pointNum)
                    {
                    case 0:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->farImg->update();
                        mapMessageDisplay("请点击第二个标定点");
                        break;
                    case 1:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->farImg->update();
                        mapMessageDisplay("请点击第三个标定点");
                        break;
                    case 2:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->farImg->update();
                        mapMessageDisplay("请点击第四个标定点");
                        break;
                    case 3:
                        lastPos = mousePos;
                        mousePos = mouseEvent->pos();
                        points.push_back(mousePos);
                        ui->farImg->update();
                        mapMessageDisplay("选点完成，请选择是否开始pnp解算");
                        isBeginPoint = false;
                        break;
                    default:
                        mapMessageDisplay("够了够了，不要点了");
                    }
                }
            }
        }
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(ui->farImg);
            painter.setPen(QPen(Qt::black,2));
            QRect rect(0,0,ui->farImg->width(),ui->farImg->height());
            //std::cout << "绘画camerimage" << std::endl;
            painter.drawPixmap(rect,farimage);
            //qDebug() << "x: " << ui->farImg->x() << "y: " << ui->farImg->y();
            //qDebug() << "width: " << ui->farImg->width() << "height: " << ui->farImg->height();

            int pointNum = this->points.size();
            if(pointNum == 0)
            {

            }
            else if(pointNum == 1)
            {
                painter.drawEllipse(mousePos,5,5);
            }
            else
            {
                for(int i = 0;i<pointNum;i++)
                {
                    painter.drawEllipse(points[i],5,5);
                    if(i == 3)
                    {
                        painter.drawLine(points[i],points[0]);
                    }
                    if(i != pointNum - 1)
                    {
                        painter.drawLine(points[i],points[i+1]);
                    }
                }
            }
            //qDebug() << "x: " << mousePos.x() << "y: " << mousePos.y();
            return true;
        }
        return false;
    }
    return QWidget::eventFilter(obj,event);
}

void pnpWidget::beginPoint()
{
    this->points.clear();
    this->isBeginPoint = true;
    mapMessageDisplay("开始标点，请点击第一个标定点");
}

void pnpWidget::restartPoint()
{
    if(points.size() == 0)
    {
        mapMessageDisplay("还没有点的数据，请点击开始标点!");
        return;
    }
    this->points.clear();
    ui->closeImg->update();
    ui->farImg->update();
    this->isBeginPoint = true;
    mapMessageDisplay("请重新选择第一个点");
}

void pnpWidget::beginPnp()
{
    if(points.size() == 4)
    {
        mapMessageDisplay("开始解算");
        std::vector<Point2d> image_points;
        Point2d p1(points[0].x(),points[0].y());
        Point2d p2(points[1].x(),points[1].y());
        Point2d p3(points[2].x(),points[2].y());
        Point2d p4(points[3].x(),points[3].y());
        image_points.push_back(p1);
        image_points.push_back(p2);
        image_points.push_back(p3);
        image_points.push_back(p4);
        cout << "image_points.size():" << image_points.size() << endl;
        cout << "objectPoints.size():" << objectPoints.size() << endl;

        if(pnp_img_id == 1)
        {
<<<<<<< HEAD
            solvePnP(objectPoints,image_points,close_camera_matrix,close_distortion_coefficient,close_Rjacob,close_T);
            cout << "solvePnP" << endl;
            Rodrigues(close_Rjacob, far_R);
=======
            cout << "image_points[0].x : " << image_points[0].x << "image_points[0].y : " << image_points[0].y << endl;
            cout << "image_points[1].x : " << image_points[1].x << "image_points[1].y : " << image_points[1].y << endl;
            cout << "image_points[2].x : " << image_points[2].x << "image_points[2].y : " << image_points[2].y << endl;
            cout << "image_points[3].x : " << image_points[3].x << "image_points[3].y : " << image_points[3].y << endl;
            cout << "objectPoints[0].x : " << objectPoints[0].x << "objectPoints[0].y : " << objectPoints[0].y << "objectPoints[0].z : " << objectPoints[0].z << endl;
            cout << "objectPoints[1].x : " << objectPoints[1].x << "objectPoints[1].y : " << objectPoints[1].y << "objectPoints[1].z : " << objectPoints[1].z << endl;
            cout << "objectPoints[2].x : " << objectPoints[2].x << "objectPoints[2].y : " << objectPoints[2].y << "objectPoints[2].z : " << objectPoints[2].z << endl;
            cout << "objectPoints[3].x : " << objectPoints[3].x << "objectPoints[3].y : " << objectPoints[3].y << "objectPoints[3].z : " << objectPoints[3].z << endl;
            cout << "close_camera_matrix:" << close_camera_matrix << endl;
            cout << "close_distortion_coefficient:" << close_distortion_coefficient << endl;
            solvePnP(objectPoints,image_points,close_camera_matrix,close_distortion_coefficient,close_Rjacob,close_T);
            cout << "cosed_Rjacob:" << close_Rjacob << endl;
            cout << "close_T:" << close_T << endl;
            cout << "solvePnP" << endl;
            Rodrigues(close_Rjacob, close_R);
>>>>>>> 431e9f5 (2024.4.20)
            cout << "Rodrigues" << endl;
            QString closeR_message = "广角相机旋转矩阵:" + QString::number(close_R.at<double>(0,0)) + " " + QString::number(close_R.at<double>(0,1)) + " " + QString::number(close_R.at<double>(0,2)) + "\n"
                    + QString::number(close_R.at<double>(1,0)) + " " + QString::number(close_R.at<double>(1,1)) + " " + QString::number(close_R.at<double>(1,2)) + "\n"
                    + QString::number(close_R.at<double>(2,0)) + " " + QString::number(close_R.at<double>(2,1)) + " " + QString::number(close_R.at<double>(2,2));
            cout << "closeR_message" << endl;
            QString closeT_message = "广角相机平移矩阵:" + QString::number(close_T.at<double>(0,0)) + "\n" + QString::number(close_T.at<double>(1,0)) + "\n" + QString::number(close_T.at<double>(2,0));
            cout << "closeT_message" << endl;
            mapMessageDisplay("解算完成");
            mapMessageDisplay(closeR_message);
            mapMessageDisplay(closeT_message);
            cout << "message" <<endl;
        }
<<<<<<< HEAD
        else if(pnp_img_id = 2)
=======
        else if(pnp_img_id == 2)
>>>>>>> 431e9f5 (2024.4.20)
        {
            solvePnP(objectPoints,image_points,far_camera_matrix,far_distortion_coefficient,far_Rjacob,far_T);
            cout << "solvePnP" << endl;
            Rodrigues(far_Rjacob, far_R);
            cout << "Rodrigues" << endl;
            QString farR_message = "长焦相机旋转矩阵:" + QString::number(far_R.at<double>(0,0)) + " " + QString::number(far_R.at<double>(0,1)) + " " + QString::number(far_R.at<double>(0,2)) + "\n"
                    + QString::number(far_R.at<double>(1,0)) + " " + QString::number(far_R.at<double>(1,1)) + " " + QString::number(far_R.at<double>(1,2)) + "\n"
                    + QString::number(far_R.at<double>(2,0)) + " " + QString::number(far_R.at<double>(2,1)) + " " + QString::number(far_R.at<double>(2,2));
            cout << "farR_message" << endl;
            QString farT_message = "长焦相机平移矩阵:" + QString::number(far_T.at<double>(0,0)) + "\n" + QString::number(far_T.at<double>(1,0)) + "\n" + QString::number(far_T.at<double>(2,0));
            cout << "farT_message" << endl;
            mapMessageDisplay("解算完成");
            mapMessageDisplay(farR_message);
            mapMessageDisplay(farT_message);
            cout << "message" <<endl;
        }
        points.clear();
        ui->closeImg->update();
        ui->farImg->update();
        Q_EMIT pnpFinished();
        cout << "end" << endl;
    }
    else
    {
        mapMessageDisplay("老哥点不够!");
    }
}

void pnpWidget::mapMessageDisplay(QString text)
{
     QPlainTextEdit *messageEdit = ui->message;
     if(!messageEdit->isReadOnly())
     {
         messageEdit->setReadOnly(true);
     }
     //messageEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
     if(messageEdit->toPlainText().size() > 1024)
     {
         messageEdit->clear();
     }
     //messageEdit->insertPlainText(text);
     messageEdit->appendPlainText(text);
     QScrollBar * scrollbar = messageEdit->verticalScrollBar();
     if(scrollbar)
     {
         scrollbar->setSliderPosition(scrollbar->maximum());
     }
}