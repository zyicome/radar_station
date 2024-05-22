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
    our_supply_region = {cv::Point2f(1.6,12.85),cv::Point2f(2.90,12.85),cv::Point2f(2.90,14.65),cv::Point2f(1.60,14.85)};
    enemy_supply_region = {cv::Point2f(26.40,2.15),cv::Point2f(25.10,2.15),cv::Point2f(25.10,0.35),cv::Point2f(25.10,0.35)};
    our_exchange_region = {cv::Point2f(1.60,10.80),cv::Point2f(2.90,10.80),cv::Point2f(2.90,12.55),cv::Point2f(1.60,12.55)};
    enemy_exchange_region = {cv::Point2f(26.40,4.20),cv::Point2f(25.10,4.20),cv::Point2f(25.10,2.45),cv::Point2f(26.40,2.45)};
    our_base_region = {cv::Point2f(0.45,6.95),cv::Point2f(1.55,5.45),cv::Point2f(3.55,6.60),cv::Point2f(3.55,7.25),cv::Point2f(3.55,7.65),cv::Point2f(3.55,8.25),cv::Point2f(1.55,9.40),cv::Point2f(0.45,8.80)};
    enemy_base_region = {cv::Point2f(27.55,7.05),cv::Point2f(26.35,6.55),cv::Point2f(26.35,6.55),cv::Point2f(25.45,7.35),cv::Point2f(24.45,7.35),
                        cv::Point2f(24.45,7.75),cv::Point2f(25.45,7.75),cv::Point2f(26.45,8.65),cv::Point2f(27.05,8.00)};
    our_R3_region = {cv::Point2f(3.15,3.45),cv::Point2f(3.15,1.05),cv::Point2f(7.10,1.05),cv::Point2f(5.40,3.45)};
    enemy_R3_region = {cv::Point2f(22.60,11.55),cv::Point2f(20.90,13.95),cv::Point2f(24.85,13.95),cv::Point2f(24.85,11.55)};
    our_R2_region = {cv::Point2f(10.90,3.10),cv::Point2f(11.80,3.70),cv::Point2f(10.15,6.10),cv::Point2f(9.00,5.70)};
    enemy_R2_region = {cv::Point2f(17.85,8.90),cv::Point2f(19.00,9.30),cv::Point2f(17.10,11.55),cv::Point2f(16.20,11.30)};
    our_R4_region = {cv::Point2f(5.00,13.15),cv::Point2f(5.00,10.80),cv::Point2f(7.00,10.80),cv::Point2f(7.90,12.20),cv::Point2f(6.00,12.20),cv::Point2f(6.00,13.15)};
    enemy_R4_region = {cv::Point2f(22.00,1.85),cv::Point2f(22.00,2.80),cv::Point2f(20.10,2.80),cv::Point2f(21.00,4.20),cv::Point2f(23.00,4.20),cv::Point2f(23.00,1.85)};
    our_energy_region = {cv::Point2f(9.05,12.50),cv::Point2f(9.05,13.55),cv::Point2f(8.00,13.55),cv::Point2f(8.00,12.50)};
    enemy_energy_region = {cv::Point2f(20.00,2.50),cv::Point2f(20.00,1.45),cv::Point2f(18.95,1.45),cv::Point2f(18.95,2.50)};
    our_fly_region = {cv::Point2f(7.00,13.90),cv::Point2f(8.15,13.90),cv::Point2f(8.15,14.75),cv::Point2f(7.00,14.75)};
    enemy_fly_region = {cv::Point2f(21.00,0.25),cv::Point2f(19.85,0.25),cv::Point2f(19.85,1.10),cv::Point2f(21.00,1.10)};
    our_fly_down_region = {cv::Point2f(12.15,0.15),cv::Point2f(13.10,0.15),cv::Point2f(13.10,1.00),cv::Point2f(12.15,1.00)};
    enemy_fly_down_region = {cv::Point2f(15.85,14.00),cv::Point2f(14.90,14.00),cv::Point2f(14.90,14.85),cv::Point2f(15.85,14.85)};
    our_outpost_region = {cv::Point2f(12.15,12.65),cv::Point2f(11.75,11.70),cv::Point2f(10.65,11.70),cv::Point2f(10.15,12.65),cv::Point2f(10.65,13.55),cv::Point2f(11.75,13.55)};
    enemy_outpost_region = {cv::Point2f(16.25,1.45),cv::Point2f(17.35,1.45),cv::Point2f(17.85,2.35),cv::Point2f(17.35,3.30),cv::Point2f(16.25,3.30),cv::Point2f(15.75,2.35)};
    our_central_island_region = {cv::Point2f(12.70,7.30),cv::Point2f(12.70,8.25),cv::Point2f(13.15,8.65),cv::Point2f(14.05,8.65)};
    enemy_central_island_region = {cv::Point2f(13.95,6.35),cv::Point2f(14.85,6.35),cv::Point2f(15.30,6.75),cv::Point2f(15.30,7.70)};

    mapInit(blueMap,our_supply_region);
    mapInit(blueMap,enemy_supply_region);
    mapInit(blueMap,our_exchange_region);
    mapInit(blueMap,enemy_exchange_region);
    mapInit(blueMap,our_base_region);
    mapInit(blueMap,enemy_base_region);
    mapInit(blueMap,our_R3_region);
    mapInit(blueMap,enemy_R3_region);
    mapInit(blueMap,our_R2_region);
    mapInit(blueMap,enemy_R2_region);
    mapInit(blueMap,our_R4_region);
    mapInit(blueMap,enemy_R4_region);
    mapInit(blueMap,our_energy_region);
    mapInit(blueMap,enemy_energy_region);
    mapInit(blueMap,our_fly_region);
    mapInit(blueMap,enemy_fly_region);
    mapInit(blueMap,our_fly_down_region);
    mapInit(blueMap,enemy_fly_down_region);
    mapInit(blueMap,our_outpost_region);
    mapInit(blueMap,enemy_outpost_region);
    mapInit(blueMap,our_central_island_region);
    mapInit(blueMap,enemy_central_island_region);

    mapInit(redMap,our_supply_region);
    mapInit(redMap,enemy_supply_region);
    mapInit(redMap,our_exchange_region);
    mapInit(redMap,enemy_exchange_region);
    mapInit(redMap,our_base_region);
    mapInit(redMap,enemy_base_region);
    mapInit(redMap,our_R3_region);
    mapInit(redMap,enemy_R3_region);
    mapInit(redMap,our_R2_region);
    mapInit(redMap,enemy_R2_region);
    mapInit(redMap,our_R4_region);
    mapInit(redMap,enemy_R4_region);
    mapInit(redMap,our_energy_region);
    mapInit(redMap,enemy_energy_region);
    mapInit(redMap,our_fly_region);
    mapInit(redMap,enemy_fly_region);
    mapInit(redMap,our_fly_down_region);
    mapInit(redMap,enemy_fly_down_region);
    mapInit(redMap,our_outpost_region);
    mapInit(redMap,enemy_outpost_region);
    mapInit(redMap,our_central_island_region);
    mapInit(redMap,enemy_central_island_region);
}

void smallMap::mapInit(cv::Mat map,std::vector<cv::Point2f> world_points)
{
    std::vector<cv::Point2i> points;
    for(int i = 0;i<world_points.size();i++)
    {
        world_points[i].x = int(world_points[i].x / 28 * map.cols);
        world_points[i].y = int(world_points[i].y / 15 * map.rows);
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
        std::cout << "id:" << i << " x:" << far_robots[i].x << " y:" << far_robots[i].y << " confidence:" << far_robots[i].confidence << std::endl;
        int id;
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
    for(int i = 1;i<far_robots.size();i++)
    {
        if(far_robots[i].confidence != 0.0)
        {
            robots[i].confidence = far_robots[i].confidence;
            robots[i].x = far_robots[i].x;
            robots[i].y = far_robots[i].y;
        }
    }

    for(int i = 1;i<close_robots.size();i++)
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