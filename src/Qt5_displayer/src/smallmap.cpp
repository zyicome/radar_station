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
    this->setParent(parent);
    //connect(this,SIGNAL(clicked(bool)),this,SLOT(drawCircle(QMouthEvent &e)));
    QString image_path = "/home/mechax/zyb/radar_station/src/Qt5_displayer/map/test.png";
    image.load(image_path);
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
    //std::cout << "绘画地图" << std::endl;
    if(farMapPoints.size() != 0)
    {
        for(int i = 0;i < farMapPoints.size();i++)
        {
            if(farMapPoints[i].id <= 5)
            {
                farMapPoints[i].id = farMapPoints[i].id + 1;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            else if(farMapPoints[i].id > 5 && farMapPoints[i].id < 12)
            {
                farMapPoints[i].id = farMapPoints[i].id - 5;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            else if(farMapPoints[i].id == 12)
            {
                farMapPoints[i].id = 0;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            else if(farMapPoints[i].id == 13)
            {
                farMapPoints[i].id = 0;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            painter.drawEllipse(farMapPoints[i].x,farMapPoints[i].y,20 * this->scaleValue,20 * this->scaleValue);
            //std::cout << "绘画坐标点" << std::endl;
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(farMapPoints[i].x + (20 * this->scaleValue) / 2,farMapPoints[i].y + (20 * this->scaleValue) / 2,QString::number(farMapPoints[i].id));
        }
        farMapPoints.clear();
    }
    if(closeMapPoints.size() != 0)
    {
        for(int i = 0;i < closeMapPoints.size();i++)
        {
            if(closeMapPoints[i].id <= 5)
            {
                closeMapPoints[i].id = closeMapPoints[i].id + 1;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            else if(closeMapPoints[i].id > 5 && closeMapPoints[i].id < 12)
            {
                closeMapPoints[i].id = closeMapPoints[i].id - 5;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            else if(closeMapPoints[i].id == 12)
            {
                closeMapPoints[i].id = 0;
                painter.setPen(QPen(Qt::red,5));
                painter.setBrush(Qt::red);
            }
            else if(closeMapPoints[i].id == 13)
            {
                closeMapPoints[i].id = 0;
                painter.setPen(QPen(Qt::blue,5));
                painter.setBrush(Qt::blue);
            }
            painter.drawEllipse(closeMapPoints[i].x,closeMapPoints[i].y,20 * this->scaleValue,20 * this->scaleValue);
            //std::cout << "绘画坐标点" << std::endl;
            painter.setPen(QPen(Qt::black,5));
            painter.drawText(closeMapPoints[i].x + (20 * this->scaleValue) / 2,closeMapPoints[i].y + (20 * this->scaleValue) / 2,QString::number(closeMapPoints[i].id));
        }
        closeMapPoints.clear();
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
