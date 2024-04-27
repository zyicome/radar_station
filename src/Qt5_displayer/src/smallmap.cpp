#include "smallmap.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPen>
#include <QPainter>
#include <iostream>

#include <ament_index_cpp/get_package_share_directory.hpp>

smallMap::smallMap(QWidget * parent)
{
    robots_init();

    this->setParent(parent);
    //connect(this,SIGNAL(clicked(bool)),this,SLOT(drawCircle(QMouthEvent &e)));

    QString img_path = QString::fromStdString(ament_index_cpp::get_package_share_directory("Qt5_displayer"));
    redimage_path = img_path + "/map/UC_redMap.png";
    blueimage_path = img_path + "/map/UC_blueMap.png";

    our_color = 0;
    if(our_color == 0) // red
    {
        image.load(redimage_path);
    }
    else if(our_color == 1) // blue
    {
        image.load(blueimage_path);
    }

    scaleValue = 1.0;
    drawPos = QPointF(0.0,0.0);
    mousePos = QPointF(0.0,0.0);
    rectPixmap = QRect(0.0,0.0,0.0,0.0);
    isMousePress = false;

    SCALL_MAX_VALUE = 3.0;
    SCALL_MIN_VALUE = 0.5;
}

smallMap::~smallMap()
{

}

void smallMap::paintEvent(QPaintEvent *event)
{
//    qDebug() << "height" << this->height();
//    qDebug() << "width" << this->width();
    double height = this->height() * scaleValue;
    double width = this->width() * scaleValue;
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,2));
    QPixmap scalePixmap = image.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    rectPixmap = QRect(drawPos.x(),drawPos.y(),width,height);
    painter.drawPixmap(rectPixmap,scalePixmap);
    for(int i = 1;i<far_robots.size();i++)
    {
        int id;
        if(far_robots[i].confidence != 0.0)
        {
            if(i<=9)
            {
                id = i;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            else if(i > 9)
            {
                id = i - 9;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            painter.drawEllipse(far_robots[i].x,far_robots[i].y,20 * this->scaleValue,20 * this->scaleValue);
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(far_robots[i].x + (20 * this->scaleValue) / 2,far_robots[i].y + (20 * this->scaleValue) / 2,QString::number(id));
        }
    }
    for(int i = 1;i<close_robots.size();i++)
    {
        int id;
        if(close_robots[i].confidence != 0.0)
        {
            if(i<=6)
            {
                id = i;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            else if(i > 6)
            {
                id = i - 6;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            painter.drawEllipse(close_robots[i].x,close_robots[i].y,20 * this->scaleValue,20 * this->scaleValue);
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(close_robots[i].x + (20 * this->scaleValue) / 2,close_robots[i].y + (20 * this->scaleValue) / 2,QString::number(id));
        }
    }
    event->accept();
}

void smallMap::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << "x:" << event->x() << "y:" << event->y();
//    qDebug() << "mapx:" << mapx << "mapy:" << mapy;
//    qDebug() << "mapcols:" << mapcols << "maprows:"<< maprows;
    if(event->button() == Qt::LeftButton)
    {
        mousePos = event->pos();
        isMousePress = true;
    }
}

void smallMap::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        this->drawPos = QPointF(0.0,0.0);
        this->scaleValue = 1.0;
        update();
    }
    else if(event->button() == Qt::LeftButton)
    {
        isMousePress = false;
    }
}

void smallMap::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePress == true)
    {
        int x = event->x() - mousePos.x();
        int y = event->y() - mousePos.y();
        mousePos = event->pos();
        drawPos = QPointF(drawPos.x() + x,drawPos.y() + y);
        update();
    }
}

void smallMap::wheelEvent(QWheelEvent *event)
{
    //qDebug() << "wheel";
    changeScaleValue(event->pos(),event->delta());
    event->accept();
}

void smallMap::changeScaleValue(QPoint event, int angle)
{
    double scale = scaleValue;
    if(angle > 0)
    {
        scaleValue *= 1.1;
    }
    else
    {
        scaleValue *= 0.9;
    }
    if(scaleValue > SCALL_MAX_VALUE)
    {
        scaleValue = SCALL_MAX_VALUE;
    }
    else if(scaleValue < SCALL_MIN_VALUE)
    {
        scaleValue = SCALL_MIN_VALUE;
    }

    if(rectPixmap.contains(event))
    {
        double x = drawPos.x() - (event.x() - drawPos.x())/rectPixmap.width()*(this->width())*(scaleValue-scale);
        double y = drawPos.y() - (event.y() - drawPos.y())/rectPixmap.height()*(this->height())*(scaleValue-scale);
        drawPos = QPointF(x,y);
    }
    else
    {
        double x=drawPos.x()-(this->width()*(scaleValue-scale))/2;
        double y=drawPos.y()-(this->height()*(scaleValue-scale))/2;
        drawPos=QPointF(x,y);
    }
    update();
}

//------------------------------------------------------------------------------------------------------------
// new place

void smallMap::robots_init()
{
    Robot robot;
    robot.id = -1;
    robot.confidence = 0.0;
    robot.is_continue = false;
    robot.x = 0.0;
    robot.y = 0.0;
    for(int i =0;i<13;i++)
    {
        robot.id++;
        far_robots.push_back(robot);
        close_robots.push_back(robot);
    }
}

void smallMap::get_robots(std::vector<Robot> &robots, const my_msgss::msg::Points &input)
{
    int armor_number = 0;

    for(int i =0;i<input.data.size();i++)
    {
        armor_number = input.data[i].id;
        robots[armor_number].is_continue = true;
        robots[armor_number].confidence = input.data[i].confidence;
        robots[armor_number].x = input.data[i].x;
        robots[armor_number].y = input.data[i].y;
    }
}

void smallMap::allrobots_adjust(std::vector<Robot> &robots)
{
    for(int i = 0;i<robots.size();i++)
    {
        if(robots[i].is_continue == false)
        {
            robots[i].confidence = 0.0;
            robots[i].x = 0.0;
            robots[i].y = 0.0;
        }

        robots[i].is_continue = false;
    }
}

//------------------------------------------------------------------------------------------------------------