#include "parameterwidget.h"
#include "ui_parameterwidget.h"
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

#include <cstdlib>

// #include "opencv2/opencv.hpp"
// #include <iostream>
// #include <vector>

parameterWidget::parameterWidget() :
    ui(new Ui::parameterWidget)
{
    ui->setupUi(this);

    connect(ui->save_c,SIGNAL(clicked()),this,SLOT(saveCloseCameraMatrix()));
    connect(ui->save_c2,SIGNAL(clicked()),this,SLOT(saveCloseDistortionCoefficient()));
    connect(ui->save_c3,SIGNAL(clicked()),this,SLOT(saveCloseUniMatrix()));
    connect(ui->save_f,SIGNAL(clicked()),this,SLOT(saveFarCameraMatrix()));
    connect(ui->save_f2,SIGNAL(clicked()),this,SLOT(saveFarDistortionCoefficient()));
    connect(ui->save_f3,SIGNAL(clicked()),this,SLOT(saveFarUniMatrix()));
    connect(ui->mapSave,SIGNAL(clicked()),this,SLOT(saveMapParameter()));
    connect(ui->imageSizeSave,SIGNAL(clicked()),this,SLOT(saveImageSizeParameter()));
}

parameterWidget::~parameterWidget()
{
    delete ui;
}

void parameterWidget::uiParameterUpdate()
{
    ui->one_c->setText(QString::number(close_camera_matrix.at<double>(0, 0)));
    ui->two_c->setText(QString::number(close_camera_matrix.at<double>(0, 1)));
    ui->three_c->setText(QString::number(close_camera_matrix.at<double>(0, 2)));
    ui->four_c->setText(QString::number(close_camera_matrix.at<double>(1, 0)));
    ui->five_c->setText(QString::number(close_camera_matrix.at<double>(1, 1)));
    ui->six_c->setText(QString::number(close_camera_matrix.at<double>(1, 2)));
    ui->seven_c->setText(QString::number(close_camera_matrix.at<double>(2, 0)));
    ui->eight_c->setText(QString::number(close_camera_matrix.at<double>(2, 1)));
    ui->nine_c->setText(QString::number(close_camera_matrix.at<double>(2, 2)));

    ui->one_c2->setText(QString::number(close_distortion_coefficient.at<double>(0, 0)));
    ui->two_c2->setText(QString::number(close_distortion_coefficient.at<double>(1, 0)));
    ui->three_c2->setText(QString::number(close_distortion_coefficient.at<double>(2, 0)));
    ui->four_c2->setText(QString::number(close_distortion_coefficient.at<double>(3, 0)));
    ui->five_c2->setText(QString::number(close_distortion_coefficient.at<double>(4, 0)));

    ui->one_c3->setText(QString::number(close_uni_matrix.at<double>(0, 0)));
    ui->two_c3->setText(QString::number(close_uni_matrix.at<double>(0, 1)));
    ui->three_c3->setText(QString::number(close_uni_matrix.at<double>(0, 2)));
    ui->four_c3->setText(QString::number(close_uni_matrix.at<double>(0, 3)));
    ui->five_c3->setText(QString::number(close_uni_matrix.at<double>(1, 0)));
    ui->six_c3->setText(QString::number(close_uni_matrix.at<double>(1, 1)));
    ui->seven_c3->setText(QString::number(close_uni_matrix.at<double>(1, 2)));
    ui->eight_c3->setText(QString::number(close_uni_matrix.at<double>(1, 3)));
    ui->nine_c3->setText(QString::number(close_uni_matrix.at<double>(2, 0)));
    ui->ten_c3->setText(QString::number(close_uni_matrix.at<double>(2, 1)));
    ui->eleven_c3->setText(QString::number(close_uni_matrix.at<double>(2, 2)));
    ui->twelve_c3->setText(QString::number(close_uni_matrix.at<double>(2, 3)));

    ui->one_f->setText(QString::number(far_camera_matrix.at<double>(0, 0)));
    ui->two_f->setText(QString::number(far_camera_matrix.at<double>(0, 1)));
    ui->three_f->setText(QString::number(far_camera_matrix.at<double>(0, 2)));
    ui->four_f->setText(QString::number(far_camera_matrix.at<double>(1, 0)));
    ui->five_f->setText(QString::number(far_camera_matrix.at<double>(1, 1)));
    ui->six_f->setText(QString::number(far_camera_matrix.at<double>(1, 2)));
    ui->seven_f->setText(QString::number(far_camera_matrix.at<double>(2, 0)));
    ui->eight_f->setText(QString::number(far_camera_matrix.at<double>(2, 1)));
    ui->nine_f->setText(QString::number(far_camera_matrix.at<double>(2, 2)));

    ui->one_f2->setText(QString::number(far_distortion_coefficient.at<double>(0, 0)));
    ui->two_f2->setText(QString::number(far_distortion_coefficient.at<double>(1, 0)));
    ui->three_f2->setText(QString::number(far_distortion_coefficient.at<double>(2, 0)));
    ui->four_f2->setText(QString::number(far_distortion_coefficient.at<double>(3, 0)));
    ui->five_f2->setText(QString::number(far_distortion_coefficient.at<double>(4, 0)));

    ui->one_f3->setText(QString::number(far_uni_matrix.at<double>(0, 0)));
    ui->two_f3->setText(QString::number(far_uni_matrix.at<double>(0, 1)));
    ui->three_f3->setText(QString::number(far_uni_matrix.at<double>(0, 2)));
    ui->four_f3->setText(QString::number(far_uni_matrix.at<double>(0, 3)));
    ui->five_f3->setText(QString::number(far_uni_matrix.at<double>(1, 0)));
    ui->six_f3->setText(QString::number(far_uni_matrix.at<double>(1, 1)));
    ui->seven_f3->setText(QString::number(far_uni_matrix.at<double>(1, 2)));
    ui->eight_f3->setText(QString::number(far_uni_matrix.at<double>(1, 3)));
    ui->nine_f3->setText(QString::number(far_uni_matrix.at<double>(2, 0)));
    ui->ten_f3->setText(QString::number(far_uni_matrix.at<double>(2, 1)));
    ui->eleven_f3->setText(QString::number(far_uni_matrix.at<double>(2, 2)));
    ui->twelve_f3->setText(QString::number(far_uni_matrix.at<double>(2, 3)));

    ui->mapHeight->setText(QString::number(object_height));
    ui->mapWidth->setText(QString::number(object_width));

    ui->imageWidth->setText(QString::number(image_cols));
    ui->imageLength->setText(QString::number(image_rows));
}

void parameterWidget::saveCloseCameraMatrix()
{   
    close_camera_matrix.at<double>(0, 0) = ui->one_c->text().toDouble();
    close_camera_matrix.at<double>(0, 1) = ui->two_c->text().toDouble();
    close_camera_matrix.at<double>(0, 2) = ui->three_c->text().toDouble();
    close_camera_matrix.at<double>(1, 0) = ui->four_c->text().toDouble();
    close_camera_matrix.at<double>(1, 1) = ui->five_c->text().toDouble();
    close_camera_matrix.at<double>(1, 2) = ui->six_c->text().toDouble();
    close_camera_matrix.at<double>(2, 0) = ui->seven_c->text().toDouble();
    close_camera_matrix.at<double>(2, 1) = ui->eight_c->text().toDouble();
    close_camera_matrix.at<double>(2, 2) = ui->nine_c->text().toDouble();
    Q_EMIT toSaveCloseCameraMatrixParameter(close_camera_matrix);
}

void parameterWidget::saveCloseDistortionCoefficient()
{
    close_distortion_coefficient.at<double>(0, 0) = ui->one_c2->text().toDouble();
    close_distortion_coefficient.at<double>(1, 0) = ui->two_c2->text().toDouble();
    close_distortion_coefficient.at<double>(2, 0) = ui->three_c2->text().toDouble();
    close_distortion_coefficient.at<double>(3, 0) = ui->four_c2->text().toDouble();
    close_distortion_coefficient.at<double>(4, 0) = ui->five_c2->text().toDouble();
    Q_EMIT toSaveCloseDistortionCoefficientParameter(close_distortion_coefficient);
}

void parameterWidget::saveCloseUniMatrix()
{
    close_uni_matrix.at<double>(0, 0) = ui->one_c3->text().toDouble();
    close_uni_matrix.at<double>(0, 1) = ui->two_c3->text().toDouble();
    close_uni_matrix.at<double>(0, 2) = ui->three_c3->text().toDouble();
    close_uni_matrix.at<double>(0, 3) = ui->four_c3->text().toDouble();
    close_uni_matrix.at<double>(1, 0) = ui->five_c3->text().toDouble();
    close_uni_matrix.at<double>(1, 1) = ui->six_c3->text().toDouble();
    close_uni_matrix.at<double>(1, 2) = ui->seven_c3->text().toDouble();
    close_uni_matrix.at<double>(1, 3) = ui->eight_c3->text().toDouble();
    close_uni_matrix.at<double>(2, 0) = ui->nine_c3->text().toDouble();
    close_uni_matrix.at<double>(2, 1) = ui->ten_c3->text().toDouble();
    close_uni_matrix.at<double>(2, 2) = ui->eleven_c3->text().toDouble();
    close_uni_matrix.at<double>(2, 3) = ui->twelve_c3->text().toDouble();
    Q_EMIT toSaveCloseUniMatrixParameter(close_uni_matrix);
}

void parameterWidget::saveFarCameraMatrix()
{
    far_camera_matrix.at<double>(0, 0) = ui->one_f->text().toDouble();
    far_camera_matrix.at<double>(0, 1) = ui->two_f->text().toDouble();
    far_camera_matrix.at<double>(0, 2) = ui->three_f->text().toDouble();
    far_camera_matrix.at<double>(1, 0) = ui->four_f->text().toDouble();
    far_camera_matrix.at<double>(1, 1) = ui->five_f->text().toDouble();
    far_camera_matrix.at<double>(1, 2) = ui->six_f->text().toDouble();
    far_camera_matrix.at<double>(2, 0) = ui->seven_f->text().toDouble();
    far_camera_matrix.at<double>(2, 1) = ui->eight_f->text().toDouble();
    far_camera_matrix.at<double>(2, 2) = ui->nine_f->text().toDouble();
    Q_EMIT toSaveFarCameraMatrixParameter(far_camera_matrix);
}

void parameterWidget::saveFarDistortionCoefficient()
{
    far_distortion_coefficient.at<double>(0, 0) = ui->one_f2->text().toDouble();
    far_distortion_coefficient.at<double>(1, 0) = ui->two_f2->text().toDouble();
    far_distortion_coefficient.at<double>(2, 0) = ui->three_f2->text().toDouble();
    far_distortion_coefficient.at<double>(3, 0) = ui->four_f2->text().toDouble();
    far_distortion_coefficient.at<double>(4, 0) = ui->five_f2->text().toDouble();
    Q_EMIT toSaveFarDistortionCoefficientParameter(far_distortion_coefficient);
}

void parameterWidget::saveFarUniMatrix()
{
    far_uni_matrix.at<double>(0, 0) = ui->one_f3->text().toDouble();
    far_uni_matrix.at<double>(0, 1) = ui->two_f3->text().toDouble();
    far_uni_matrix.at<double>(0, 2) = ui->three_f3->text().toDouble();
    far_uni_matrix.at<double>(0, 3) = ui->four_f3->text().toDouble();
    far_uni_matrix.at<double>(1, 0) = ui->five_f3->text().toDouble();
    far_uni_matrix.at<double>(1, 1) = ui->six_f3->text().toDouble();
    far_uni_matrix.at<double>(1, 2) = ui->seven_f3->text().toDouble();
    far_uni_matrix.at<double>(1, 3) = ui->eight_f3->text().toDouble();
    far_uni_matrix.at<double>(2, 0) = ui->nine_f3->text().toDouble();
    far_uni_matrix.at<double>(2, 1) = ui->ten_f3->text().toDouble();
    far_uni_matrix.at<double>(2, 2) = ui->eleven_f3->text().toDouble();
    far_uni_matrix.at<double>(2, 3) = ui->twelve_f3->text().toDouble();
    Q_EMIT toSaveFarUniMatrixParameter(far_uni_matrix);   
}

void parameterWidget::saveMapParameter()
{
    object_height = ui->mapHeight->text().toDouble();
    object_width = ui->mapWidth->text().toDouble();
    std::cout << "saveMapParameter" << std::endl;
    Q_EMIT toSaveMapParameter(object_height, object_width);
}

void parameterWidget::saveImageSizeParameter()
{
    image_cols = ui->imageWidth->text().toInt();
    image_rows = ui->imageLength->text().toInt();
    Q_EMIT toSaveImageSizeParameter(image_cols, image_rows);
}

