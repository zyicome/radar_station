#include "radarstation.h"
#include "ui_radarstation.h"
#include "pnpwidget.h"
#include "ui_pnpwidget.h"
#include "smallmap.h"
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QRadioButton>

radarStation::radarStation(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::radarStation)
{
    ui->setupUi(this);
//    this->resize(1600,1400);
//    ui->centralwidget->resize(1600,1400);
    //ui->centralwidget->installEventFilter(this);
    //QString image_path = QFileDialog::getOpenFileName(this, "打开图片", ".", "Images (*.png *.xpm *.jpg)");
//    QString image_path = "C:/Users/zyb/Desktop/Robotmaster/pitures/map.png";
//    ui->map->setPixmap(QPixmap(image_path));
    ui->map->mapx = ui->map->x();
    ui->map->mapy = ui->map->y();
    ui->map->mapcols = ui->map->width();
    ui->map->maprows = ui->map->height();
    ui->allWidget->setCurrentWidget(ui->mapWidget);

    QThread far_thread;
    QThread close_thread;
    ui->farImg->moveToThread(&far_thread);
    ui->closeImg->moveToThread(&close_thread);

    init();
}

radarStation::~radarStation()
{
    delete ui;
}

void radarStation::init()
{
    connect(ui->pnpMode,SIGNAL(clicked()),this,SLOT(changeToPnpWidget()));
    connect(ui->mapMode,SIGNAL(clicked()),this,SLOT(changeToMapWidget()));

    connect(ui->pnpMode_2,SIGNAL(clicked()),this,SLOT(changeToPnpWidget_2()));
    connect(ui->mapMode_2,SIGNAL(clicked()),this,SLOT(changeToMapWidget_2()));

    connect(&this->qtnode,SIGNAL(updateFarImage()),this,SLOT(farImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateDepthImage()),this,SLOT(farDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateFarPoints()),this,SLOT(farPointsUpdate()));

    connect(&this->qtnode,SIGNAL(updateCloseImage()),this,SLOT(closeImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateCloseDepthImage()),this,SLOT(closeDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateClosePoints()),this,SLOT(closePointsUpdate()));

    connect(ui->solvePnpWidget,SIGNAL(pnpFinished()),this,SLOT(publishPnpResult()));
}

void radarStation::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "x:" << event->x() << "y:" << event->y();
    /*if(event->x() >= mapx && event->x() <= mapx + mapcols)
    {
        if(event->y() >= mapy && event->y() <= mapy + maprows)
        {
            QPainter painter(ui->map);
            painter.setPen(QPen(Qt::black,2));
            painter.drawEllipse(event->pos(),5,5);
            ui->map->update();
        }
    }*/
    QString text = "x:";
    text = text + QString::number(event->x());
    text = text + " y:" + QString::number(event->y());
    mapMessageDisplay(text);
}

void radarStation::paintEvent(QPaintEvent *event)
{
    QPainter t(this);
    QPen pen;
    pen.setWidth(5);
    t.setPen(pen);
    t.drawEllipse(pos,5,5);
    event->accept();
}

void radarStation::mapMessageDisplay(QString text)
{
     QPlainTextEdit *messageEdit = ui->mapMessage;
     if(!messageEdit->isReadOnly())
     {
         messageEdit->setReadOnly(true);
     }
     //messageEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
     if(messageEdit->toPlainText().size() > 1024 * 4)
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

void radarStation::changeToMapWidget()
{
    //qDebug() << "map";
    ui->allWidget->setCurrentWidget(ui->mapWidget);
}

void radarStation::changeToPnpWidget()
{
    //qDebug() << "pnp";
    ui->allWidget->setCurrentWidget(ui->solvePnpWidget);
}

void radarStation::changeToMapWidget_2()
{
    //qDebug() << "map";
    ui->allWidget->setCurrentWidget(ui->mapWidget);
}

void radarStation::changeToPnpWidget_2()
{
    //qDebug() << "pnp";
    ui->allWidget->setCurrentWidget(ui->solvePnpWidget);
}

void radarStation::farImageUpdate()
{
    //far_qimage_mutex.lock();
    //mapMessageDisplay("farImageUpdate");
    if(ui->pnpMode->isChecked())
    {
        //cout << "pnpMode" << endl;
        ui->solvePnpWidget->farimage = QPixmap::fromImage(qtnode.far_qimage);
        ui->solvePnpWidget->ui->farImg->update();
    }
    else if(ui->mapMode->isChecked())
    {
        //cout << "mapMode" << endl;
        ui->farImg->setPixmap(QPixmap::fromImage(qtnode.far_qimage));
        ui->farImg->update();
    }
    //far_qimage_mutex.unlock();
}

void radarStation::closeImageUpdate()
{
    //close_qimage_mutex.lock();
    //mapMessageDisplay("farImageUpdate");
    if(ui->pnpMode->isChecked())
    {
        //cout << "pnpMode" << endl;
        ui->solvePnpWidget->closeimage = QPixmap::fromImage(qtnode.close_qimage);
        ui->solvePnpWidget->ui->closeImg->update();
    }
    else if(ui->mapMode->isChecked())
    {
        //cout << "mapMode" << endl;
        ui->closeImg->setPixmap(QPixmap::fromImage(qtnode.close_qimage));
        ui->closeImg->update();
    }
    //close_qimage_mutex.unlock();
}

void radarStation::farDepthImageUpdate()
{
    far_qimage_mutex.lock();
    //mapMessageDisplay("depthImageUpdate");
    if(ui->mapMode->isChecked())
    {
        //cout << "绘画深度图" << endl;
        ui->farDepth->setPixmap(QPixmap::fromImage(qtnode.fardepth_qimage));
        ui->farDepth->update();
    }
    far_qimage_mutex.unlock();
}

void radarStation::closeDepthImageUpdate()
{
    close_qimage_mutex.lock();
    //mapMessageDisplay("depthImageUpdate");
    if(ui->mapMode->isChecked())
    {
        //cout << "绘画深度图" << endl;
        ui->closeDepth->setPixmap(QPixmap::fromImage(qtnode.closedepth_qimage));
        ui->closeDepth->update();
    }
    close_qimage_mutex.unlock();
}

void radarStation::publishPnpResult()
{
    std_msgs::msg::Float32MultiArray pnp_result;
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(0,0));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(0,1));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(0,2));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(1,0));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(1,1));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(1,2));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(2,0));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(2,1));
    pnp_result.data.push_back(ui->solvePnpWidget->far_R.at<double>(2,2));
    pnp_result.data.push_back(ui->solvePnpWidget->far_T.at<double>(0,0));
    pnp_result.data.push_back(ui->solvePnpWidget->far_T.at<double>(1,0));
    pnp_result.data.push_back(ui->solvePnpWidget->far_T.at<double>(2,0));
    pnp_result.data.push_back(ui->solvePnpWidget->pnp_img_id);
    qtnode.pnp_pub_->publish(pnp_result);
    cout << "发送pnp结果" << endl;
}

void radarStation::farPointsUpdate()
{
    float object_width = 11.5;
    float object_height = 8;

    my_msgss::msg::Points far_qpoints = qtnode.far_world_qpoints;
    ui->map->get_robots(ui->map->far_robots,far_qpoints);
    ui->map->allrobots_adjust(ui->map->far_robots);

    float width = ui->map->width() * ui->map->scaleValue;
    float height = ui->map->height() * ui->map->scaleValue;
    mapPos farpos;
    for(int i = 1; i < ui->map->far_robots.size();i++)
    {
        if(ui->map->far_robots[i].confidence > 0.0)
        {
            farpos.id = ui->map->far_robots[i].id;
            farpos.x = ui->map->far_robots[i].x / object_width * width;
            farpos.y = height - (ui->map->far_robots[i].y / object_height * height);
            farpos.x = farpos.x + ui->map->drawPos.x();
            farpos.y = farpos.y + ui->map->drawPos.y();
            ui->map->farMapPoints.push_back(farpos);
            QString far_pointText = QString::number(farpos.id) + "号机器人相对于初始小地图的坐标为 x:" 
                                + QString::number((farpos.x - ui->map->drawPos.x()) / ui->map->scaleValue) +" y:"
                                + QString::number((farpos.y - ui->map->drawPos.y()) / ui->map->scaleValue);
            mapMessageDisplay(far_pointText);
        }
    }
    ui->map->update();
}

void radarStation::closePointsUpdate()
{
    float object_width = 11.5;
    float object_height = 8;

    my_msgss::msg::Points close_qpoints = qtnode.close_world_qpoints;
    ui->map->get_robots(ui->map->close_robots,close_qpoints);
    ui->map->allrobots_adjust(ui->map->close_robots);

    float width = ui->map->width() * ui->map->scaleValue;
    float height = ui->map->height() * ui->map->scaleValue;
    mapPos closepos;
    for(int i = 1; i < ui->map->close_robots.size();i++)
    {
        if(ui->map->close_robots[i].confidence > 0.0)
        {
            closepos.id = ui->map->close_robots[i].id;
            closepos.x = ui->map->close_robots[i].x / object_width * width;
            closepos.y = height - (ui->map->close_robots[i].y / object_height * height);
            closepos.x = closepos.x + ui->map->drawPos.x();
            closepos.y = closepos.y + ui->map->drawPos.y();
            ui->map->closeMapPoints.push_back(closepos);
            QString close_pointText = QString::number(closepos.id) + "号机器人相对于初始小地图的坐标为 x:" 
                                + QString::number((closepos.x - ui->map->drawPos.x()) / ui->map->scaleValue) +" y:"
                                + QString::number((closepos.y - ui->map->drawPos.y()) / ui->map->scaleValue);
            mapMessageDisplay(close_pointText);
        }
    }
    ui->map->update();
}