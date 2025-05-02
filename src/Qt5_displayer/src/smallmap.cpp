#include "smallmap.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPen>
#include <QPainter>
#include <iostream>

smallMap::smallMap(QWidget * parent)
{
    robots_init();

    this->setParent(parent);
    //connect(this,SIGNAL(clicked(bool)),this,SLOT(drawCircle(QMouthEvent &e)));

    std::string image_path = ament_index_cpp::get_package_share_directory("Qt5_displayer");
    std::string redimage_path = image_path + "/map/UC_redMap.png";
    std::string blueimage_path = image_path + "/map/UC_blueMap.png";
    blueMap = cv::imread(blueimage_path);
    redMap = cv::imread(redimage_path);
    cv::resize(blueMap,blueMap,cv::Size(this->width(),this->height()));
    cv::resize(redMap,redMap,cv::Size(this->width(),this->height()));

    gainRegionInit();

    redImage = QImage((const unsigned char*)(redMap.data),
                           redMap.cols,
                           redMap.rows,
                           redMap.step,
                           QImage::Format_RGB888).rgbSwapped();

    blueImage = QImage((const unsigned char*)(blueMap.data),
                           blueMap.cols,
                           blueMap.rows,
                           blueMap.step,
                           QImage::Format_RGB888).rgbSwapped();
    
    our_color = 0;
    if(our_color == 0) // red
    {
        // 将QImage转换为QPixmap
        image = QPixmap::fromImage(redImage);
        //image.load(redimage_path);
    }
    else if(our_color == 1) // blue
    {
        // 将QImage转换为QPixmap
        image = QPixmap::fromImage(blueImage);
        //image.load(blueimage_path);
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

void smallMap::gainRegionInit()
{
    our_supply_region = {cv::Point2f(1.20,2.95),cv::Point2f(3.09,2.95),cv::Point2f(3.27,2.80),cv::Point2f(3.27,1.85),cv::Point2f(1.20,1.85)};
    enemy_supply_region = {cv::Point2f(24.72,13.20),cv::Point2f(26.84,13.20),cv::Point2f(26.84,12.08),cv::Point2f(24.91,12.08),cv::Point2f(24.75,12.22)};
    our_exchange_region = {cv::Point2f(1.53,1.82),cv::Point2f(3.40,1.82),cv::Point2f(3.40,0.10),cv::Point2f(1.53,0.10)};
    enemy_exchange_region = {cv::Point2f(24.70,14.92),cv::Point2f(26.51,14.92),cv::Point2f(26.51,13.21),cv::Point2f(24.70,13.21)};
    our_base_region = {cv::Point2f(0.83,8.67),cv::Point2f(2.23,9.50),cv::Point2f(4.22,8.34),cv::Point2f(4.22,6.68),cv::Point2f(2.23,5.53),cv::Point2f(0.83,6.33)};
    enemy_base_region = {cv::Point2f(23.81,8.33),cv::Point2f(25.81,9.50),cv::Point2f(27.20,8.67),cv::Point2f(27.20,6.34),cv::Point2f(25.80,5.53),cv::Point2f(23.81,6.68)};
    our_energy_region = {cv::Point2f(12.99,8.10),cv::Point2f(13.44,8.54),cv::Point2f(14.34,8.54),cv::Point2f(12.99,7.13)};
    enemy_energy_region = {cv::Point2f(14.60,6.50),cv::Point2f(15.04,6.93),cv::Point2f(15.04,7.85),cv::Point2f(13.69,6.50)};
    enemy_fly_region = {cv::Point2f(14.26,14.94),cv::Point2f(18.04,14.94),cv::Point2f(18.04,13.95),cv::Point2f(14.36,13.95)};
    enemy_fly_down_region = {cv::Point2f(11.13,13.94),cv::Point2f(13.53,13.94),cv::Point2f(13.53,14.92),cv::Point2f(11.13,14.92)};
    our_fly_region = {cv::Point2f(9.97,1.08),cv::Point2f(13.73,1.08),cv::Point2f(13.73,0.10),cv::Point2f(9.97,0.10)};
    our_fly_down_region = {cv::Point2f(14.50,1.08),cv::Point2f(16.89,1.08),cv::Point2f(16.89,0.12),cv::Point2f(14.50,0.12)};
    our_high_region = {cv::Point2f(5.53,12.34),cv::Point2f(9.40,12.34),cv::Point2f(9.77,11.94),cv::Point2f(8.96,11.14),cv::Point2f(5.53,11.14)};
    enemy_high_region = {cv::Point2f(22.52,2.68),cv::Point2f(18.62,2.68),cv::Point2f(18.24,3.08),cv::Point2f(19.06,3.88),cv::Point2f(22.52,3.88)};
    our_central_high_region = {cv::Point2f(10.38,6.74),cv::Point2f(10.96,6.74),cv::Point2f(11.20,6.95),cv::Point2f(11.20,9.15),cv::Point2f(12.62,11.18),cv::Point2f(12.62,12.62),cv::Point2f(10.34,9.42)};
    enemy_central_high_region = {cv::Point2f(17.69,8.30),cv::Point2f(17.05,8.30),cv::Point2f(16.84,8.08),cv::Point2f(16.84,5.89),cv::Point2f(15.40,3.84),cv::Point2f(15.40,2.40),cv::Point2f(17.68,5.62)};
    our_out_station_region= {cv::Point2f(10.57,4.23),cv::Point2f(11.81,4.77),cv::Point2f(11.81,3.27),cv::Point2f(11.27,2.72),cv::Point2f(10.25,2.73),cv::Point2f(10.61,3.24)};
    enemy_out_station_region = {cv::Point2f(17.46,10.77),cv::Point2f(16.19,10.23),cv::Point2f(16.19,11.73),cv::Point2f(16.73,12.28),cv::Point2f(17.75,12.27),cv::Point2f(17.39,11.76)};

    our_basition_region = {cv::Point2f(6.10,7.52),cv::Point2f(6.36,7.96),cv::Point2f(6.86,7.96),cv::Point2f(7.12,7.52),cv::Point2f(6.86,7.07),cv::Point2f(6.36,7.07)};
    enemy_basition_region = {cv::Point2f(20.96,7.51),cv::Point2f(21.17,7.96),cv::Point2f(21.66,7.95),cv::Point2f(21.93,7.51),cv::Point2f(21.67,7.07),cv::Point2f(21.16,7.07)};
    our_tercentral_down_region= {cv::Point2f(8.84,8.07),cv::Point2f(9.06,8.30),cv::Point2f(9.53,8.30),cv::Point2f(9.53,6.73),cv::Point2f(9.06,6.73),cv::Point2f(8.84,6.96)};
    enemy_tercentral_down_region = {cv::Point2f(18.48,8.31),cv::Point2f(18.97,8.30),cv::Point2f(19.20,8.07),cv::Point2f(19.20,6.95),cv::Point2f(18.97,6.73),cv::Point2f(18.48,6.73)};
    our_tercentral_up_region = {cv::Point2f(10.35,8.24),cv::Point2f(10.35,6.70),cv::Point2f(11.00,6.70),cv::Point2f(11.23,6.93),cv::Point2f(11.20,8.24)};
    enemy_tercentral_up_region = {cv::Point2f(17.70,8.30),cv::Point2f(17.70,6.77),cv::Point2f(16.85,6.77),cv::Point2f(16.85,8.10),cv::Point2f(17.04,8.30)};
    our_terhigh_down_region = {cv::Point2f(7.82,2.50),cv::Point2f(8.06,2.73),cv::Point2f(9.18,2.73),cv::Point2f(9.43,2.50),cv::Point2f(9.43,2.02),cv::Point2f(7.82,2.02)};
    enemy_terhigh_down_region = {cv::Point2f(18.59,13.01),cv::Point2f(20.20,13.01),cv::Point2f(20.20,12.53),cv::Point2f(19.97,12.30),cv::Point2f(18.83,12.30),cv::Point2f(18.59,12.53)};
    our_terhigh_up_region = {cv::Point2f(7.82,1.82),cv::Point2f(9.43,1.82),cv::Point2f(9.43,1.47),cv::Point2f(9.19,1.23),cv::Point2f(8.05,1.23),cv::Point2f(7.82,1.47)};
    enemy_terhigh_up_region = {cv::Point2f(18.59,13.20),cv::Point2f(18.59,13.56),cv::Point2f(18.83,13.80),cv::Point2f(19.97,13.80),cv::Point2f(20.20,13.56),cv::Point2f(20.20,13.20)};

    mapInit(blueMap,our_supply_region);
    mapInit(blueMap,enemy_supply_region);
    mapInit(blueMap,our_exchange_region);
    mapInit(blueMap,enemy_exchange_region);
    mapInit(blueMap,our_base_region);
    mapInit(blueMap,enemy_base_region);
    mapInit(blueMap,our_energy_region);
    mapInit(blueMap,enemy_energy_region);
    mapInit(blueMap,our_fly_region);
    mapInit(blueMap,enemy_fly_region);
    mapInit(blueMap,our_fly_down_region);
    mapInit(blueMap,enemy_fly_down_region);
    mapInit(blueMap,our_high_region);
    mapInit(blueMap,enemy_high_region);
    mapInit(blueMap,our_central_high_region);
    mapInit(blueMap,enemy_central_high_region);
    mapInit(blueMap,our_out_station_region);
    mapInit(blueMap,enemy_out_station_region);
    mapInit(blueMap,our_basition_region);
    mapInit(blueMap,enemy_basition_region);
    mapInit(blueMap,our_tercentral_down_region);
    mapInit(blueMap,enemy_tercentral_down_region);
    mapInit(blueMap,our_tercentral_up_region);
    mapInit(blueMap,enemy_tercentral_up_region);
    mapInit(blueMap,our_terhigh_down_region);
    mapInit(blueMap,enemy_terhigh_down_region);
    mapInit(blueMap,our_terhigh_up_region);
    mapInit(blueMap,enemy_terhigh_up_region);
    
    mapInit(redMap,our_supply_region);
    mapInit(redMap,enemy_supply_region);
    mapInit(redMap,our_exchange_region);
    mapInit(redMap,enemy_exchange_region);
    mapInit(redMap,our_base_region);
    mapInit(redMap,enemy_base_region);
    mapInit(redMap,our_energy_region);
    mapInit(redMap,enemy_energy_region);
    mapInit(redMap,our_fly_region);
    mapInit(redMap,enemy_fly_region);
    mapInit(redMap,our_fly_down_region);
    mapInit(redMap,enemy_fly_down_region);
    mapInit(redMap,our_high_region);
    mapInit(redMap,enemy_high_region);
    mapInit(redMap,our_central_high_region);
    mapInit(redMap,enemy_central_high_region);
    mapInit(redMap,our_out_station_region);
    mapInit(redMap,enemy_out_station_region);
    mapInit(redMap,our_basition_region);
    mapInit(redMap,enemy_basition_region);
    mapInit(redMap,our_tercentral_down_region);
    mapInit(redMap,enemy_tercentral_down_region);
    mapInit(redMap,our_tercentral_up_region);
    mapInit(redMap,enemy_tercentral_up_region);
    mapInit(redMap,our_terhigh_down_region);
    mapInit(redMap,enemy_terhigh_down_region);
    mapInit(redMap,our_terhigh_up_region);
    mapInit(redMap,enemy_terhigh_up_region);
}

void smallMap::mapInit(cv::Mat map,std::vector<cv::Point2f> world_points)
{
    std::vector<cv::Point2i> points;
    for(int i = 0;i<world_points.size();i++)
    {
        world_points[i].x = int(world_points[i].x / 28.0f * map.cols);
        world_points[i].y = map.rows - int(world_points[i].y / 15.0f * map.rows);
    }
    for(int i =0;i<world_points.size();i++)
    {
        points.push_back(world_points[i]);
    }
    cv::polylines(map, points, true, cv::Scalar(0,0,255), 2);
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

    far_close_robots_adjust(far_robots,close_robots,robots);

    for(int i = 1;i<robots.size();i++)
    {
        //std::cout << "id:" << i << " x:" << far_robots[i].x << " y:" << far_robots[i].y << " confidence:" << far_robots[i].confidence << std::endl;
        int id;
        if(our_color == 0) //我们是红方
        {
            if(robots[i].confidence != 0.0)
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
            painter.drawEllipse(robots[i].x,robots[i].y,20 * this->scaleValue,20 * this->scaleValue);
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(robots[i].x + (20 * this->scaleValue) / 2,robots[i].y + (20 * this->scaleValue) / 2,QString::number(id));
            }
        }
        else if(our_color == 1) //我们是蓝方
        {
            if(robots[i].confidence != 0.0)
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
            painter.drawEllipse(robots[i].x,robots[i].y,20 * this->scaleValue,20 * this->scaleValue);
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(robots[i].x + (20 * this->scaleValue) / 2,robots[i].y + (20 * this->scaleValue) / 2,QString::number(id));
            }
        }
    }
    for(int i = 1;i<robots.size();i++)
    {
        robots[i].confidence = 0.0;
        robots[i].x = 0.0;
        robots[i].y = 0.0;
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
        robots.push_back(robot);
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

void smallMap::far_close_robots_adjust(std::vector<Robot> &far_robots,std::vector<Robot> &close_robots,std::vector<Robot> &robots)
{
    for(size_t i = 1;i<far_robots.size();i++)
    {
        if(far_robots[i].confidence != 0.0)
        {
            robots[i].confidence = far_robots[i].confidence;
            robots[i].x = far_robots[i].x;
            robots[i].y = far_robots[i].y;
        }
    }

    for(size_t i = 1;i<close_robots.size();i++)
    {
        if(close_robots[i].confidence != 0.0)
        {
                robots[i].confidence = close_robots[i].confidence;
                robots[i].x = close_robots[i].x;
                robots[i].y = close_robots[i].y;
            
        }
    }
}
//------------------------------------------------------------------------------------------------------------