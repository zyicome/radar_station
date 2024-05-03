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
    robots_init();
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

    connect(ui->blueMode,SIGNAL(clicked()),this,SLOT(blueMode()));
    connect(ui->redMode,SIGNAL(clicked()),this,SLOT(redMode()));

    connect(&this->qtnode,SIGNAL(updateFarImage()),this,SLOT(farImageUpdate()));
    //connect(&this->qtnode,SIGNAL(updateDepthImage()),this,SLOT(farDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateFarPoints()),this,SLOT(farPointsUpdate()));

    connect(&this->qtnode,SIGNAL(updateCloseImage()),this,SLOT(closeImageUpdate()));
    //connect(&this->qtnode,SIGNAL(updateCloseDepthImage()),this,SLOT(closeDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateClosePoints()),this,SLOT(closePointsUpdate()));

    connect(&this->qtnode,SIGNAL(updateGameState()),this,SLOT(gameStateUpdate()));

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
    ui->pnpMode->setChecked(false);
    ui->mapMode->setChecked(true);
    ui->pnpMode_2->setChecked(false);
    ui->mapMode_2->setChecked(true);
    ui->allWidget->setCurrentWidget(ui->mapWidget);
}

void radarStation::changeToPnpWidget()
{
    //qDebug() << "pnp";
    ui->pnpMode->setChecked(true);
    ui->mapMode->setChecked(false);
    ui->pnpMode_2->setChecked(true);
    ui->mapMode_2->setChecked(false);
    ui->allWidget->setCurrentWidget(ui->solvePnpWidget);
}

void radarStation::changeToMapWidget_2()
{
    //qDebug() << "map";
    ui->pnpMode->setChecked(false);
    ui->mapMode->setChecked(true);
    ui->pnpMode_2->setChecked(false);
    ui->mapMode_2->setChecked(true);
    ui->allWidget->setCurrentWidget(ui->mapWidget);
}

void radarStation::changeToPnpWidget_2()
{
    //qDebug() << "pnp";
    ui->pnpMode->setChecked(true);
    ui->mapMode->setChecked(false);
    ui->pnpMode_2->setChecked(true);
    ui->mapMode_2->setChecked(false);
    ui->allWidget->setCurrentWidget(ui->solvePnpWidget);
}

void radarStation::farImageUpdate()
{
    //far_qimage_mutex.lock();
    //mapMessageDisplay("farImageUpdate");
    if(ui->pnpMode->isChecked())
    {
        ui->solvePnpWidget->farimage = QPixmap::fromImage(qtnode.far_qimage);
        ui->solvePnpWidget->ui->farImg->update();
    }
    else if(ui->mapMode->isChecked())
    {
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
        ui->solvePnpWidget->closeimage = QPixmap::fromImage(qtnode.close_qimage);
        ui->solvePnpWidget->ui->closeImg->update();
    }
    else if(ui->mapMode->isChecked())
    {
        ui->closeImg->setPixmap(QPixmap::fromImage(qtnode.close_qimage));
        ui->closeImg->update();
    }
    //close_qimage_mutex.unlock();
}

/*void radarStation::farDepthImageUpdate()
{
    far_qimage_mutex.lock();
    //mapMessageDisplay("depthImageUpdate");
    if(ui->mapMode->isChecked())
    {
        ui->farDepth->setPixmap(QPixmap::fromImage(qtnode.fardepth_qimage));
        ui->farDepth->update();
    }
    far_qimage_mutex.unlock();
}*/

/*void radarStation::closeDepthImageUpdate()
{
    close_qimage_mutex.lock();
    //mapMessageDisplay("depthImageUpdate");
    if(ui->mapMode->isChecked())
    {
        ui->closeDepth->setPixmap(QPixmap::fromImage(qtnode.closedepth_qimage));
        ui->closeDepth->update();
    }
    close_qimage_mutex.unlock();
}*/

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
}

void radarStation::farPointsUpdate()
{
    float object_width = 28;
    float object_height = 15;

    my_msgss::msg::Points far_qpoints = qtnode.far_world_qpoints;
    ui->map->get_robots(ui->map->far_robots,far_qpoints);
    ui->map->allrobots_adjust(ui->map->far_robots);
    robots_adjust(ui->map->far_robots,true);
    all_robots_adjust(true);
    status_adjust(robots);
    decision(robots);
    sendRobots(robots);

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
    float object_width = 28;
    float object_height = 15;

    my_msgss::msg::Points close_qpoints = qtnode.close_world_qpoints;
    ui->map->get_robots(ui->map->close_robots,close_qpoints);
    ui->map->allrobots_adjust(ui->map->close_robots);
    robots_adjust(ui->map->close_robots,false);
    all_robots_adjust(false);
    status_adjust(robots);
    decision(robots);
    sendRobots(robots);

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
            ui->map->close_robots[i].x = closepos.x;
            ui->map->close_robots[i].y = closepos.y;
            QString close_pointText = QString::number(closepos.id) + "号机器人相对于初始小地图的坐标为 x:" 
                                + QString::number((closepos.x - ui->map->drawPos.x()) / ui->map->scaleValue) +" y:"
                                + QString::number((closepos.y - ui->map->drawPos.y()) / ui->map->scaleValue);
            mapMessageDisplay(close_pointText);
        }
    }
    ui->map->update();
}

void radarStation::gameStateUpdate()
{
    my_msgss::msg::Gamestate game_state = qtnode.game_state_msg;
    if(game_state.game_progress == 0)
    {
        ui->gameProgress->setText("比赛状态：比赛未开始");
    }
    else if(game_state.game_progress == 1)
    {
        ui->gameProgress->setText("比赛状态：比赛准备中");
    }
    else if(game_state.game_progress == 2)
    {
        ui->gameProgress->setText("比赛状态：自检进行中");
    }
    else if(game_state.game_progress == 3)
    {
        ui->gameProgress->setText("比赛状态：5s倒计时中");
    }
    else if(game_state.game_progress == 4)
    {
        ui->gameProgress->setText("比赛状态：比赛进行中");
    }
    else if(game_state.game_progress == 5)
    {
        ui->gameProgress->setText("比赛状态：比赛结束");
    }
}

void radarStation::blueMode()
{
    ui->map->our_color = 1;
    ui->map->image = QPixmap::fromImage(ui->map->blueImage);
    ui->map->update();
}

void radarStation::redMode()
{
    ui->map->our_color = 0;
    ui->map->image = QPixmap::fromImage(ui->map->redImage);
    ui->map->update();
}

void radarStation::robots_init()
{
    DecisionRobot robot;
    robot.id = -1;
    robot.confidence = 0.0;
    robot.is_continue = false;
    robot.is_far_continue = false;
    robot.is_close_continue = false;
    robot.x = 0.0;
    robot.y = 0.0;
    for(int i =0;i<13;i++)
    {
        robot.id++;
        robots.push_back(robot);
    }
}

void radarStation::robots_adjust(std::vector<Robot> &get_robots, bool is_far)
{
    float object_width = 28;
    float object_height = 15;
    float width = ui->map->width();
    float height = ui->map->height();
    int armor_number = 0;
    float confidence = 0.0;
    float x = 0.0;
    float y = 0.0;

    for(int i = 0;i<get_robots.size();i++)
    {
        if(get_robots[i].confidence != 0.0)
        {
            armor_number = get_robots[i].id;
            confidence = get_robots[i].confidence;
            //cout << "get_robots_armor_number:" << armor_number << " confidence:" << confidence << endl;
            x = get_robots[i].x / object_width * width;
            y = height - (get_robots[i].y / object_height * height);
            if(is_far)
            {
                if(robots[armor_number].confidence == 0.0)
                {
                    robots[armor_number].id = armor_number;
                    robots[armor_number].is_continue = true;
                    robots[armor_number].is_far_continue = true;
                    robots[armor_number].is_close_continue = false;
                    robots[armor_number].confidence = confidence;
                    robots[armor_number].x = x;
                    robots[armor_number].y = y;
                }
                else if(robots[armor_number].confidence != 0.0)
                {
                    if(confidence >= robots[armor_number].confidence)
                    {
                        robots[armor_number].id = armor_number;
                        robots[armor_number].is_continue = true;
                        robots[armor_number].is_far_continue = true;
                        robots[armor_number].is_close_continue = false;
                        robots[armor_number].confidence = confidence;
                        robots[armor_number].x = x;
                        robots[armor_number].y = y;
                    }
                    else
                    {
                        robots[armor_number].is_far_continue = false;
                    }
                }
            else
            {
                if(robots[armor_number].confidence == 0.0)
                {
                    robots[armor_number].id = armor_number;
                    robots[armor_number].is_continue = true;
                    robots[armor_number].is_far_continue = false;
                    robots[armor_number].is_close_continue = true;
                    robots[armor_number].confidence = confidence;
                    robots[armor_number].x = x;
                    robots[armor_number].y = y;
                }
                else if(robots[armor_number].confidence != 0.0)
                {
                    if(confidence >= robots[armor_number].confidence)
                    {
                        robots[armor_number].id = armor_number;
                        robots[armor_number].is_continue = true;
                        robots[armor_number].is_far_continue = false;
                        robots[armor_number].is_close_continue = true;
                        robots[armor_number].confidence = confidence;
                        robots[armor_number].x = x;
                        robots[armor_number].y = y;
                    }
                    else
                    {
                        robots[armor_number].is_close_continue = false;
                    }
                }
            }
        }
    }
    }
}

void radarStation::all_robots_adjust(bool is_far)
{
    for(int i = 0;i<robots.size();i++)
    {
        if(robots[i].is_continue == false)
        {
            if(robots[i].is_far_continue == false && robots[i].is_close_continue == false)
            {
                robots[i].confidence = 0.0;
                robots[i].x = 0.0;
                robots[i].y = 0.0;
            }
        }
        if(is_far)
        {
            robots[i].is_far_continue = false;
        }
        else
        {
            robots[i].is_close_continue = false;
        }
        if(robots[i].is_far_continue == false && robots[i].is_close_continue == false)
        {
            robots[i].is_continue = false;
        }
    }
}

void radarStation::status_adjust(std::vector<DecisionRobot> &robots)
{
    int armor_number = 0;
    float confidence = 0.0;
    if(ui->map->our_color == 0) // red
    {
        for(int i = 1;i<7;i++)
        {
            if(robots[i].confidence != 0.0)
            {
            armor_number = robots[i].id;
            confidence = robots[i].confidence;
            //std::cout << "armor_number:" << armor_number << " confidence:" << confidence << std::endl;
            switch(armor_number)
            {
                case 1:
                    ui->hp_1->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 2: 
                    ui->hp_2->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 3:
                    ui->hp_3->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 4:
                    ui->hp_4->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 5:
                    ui->hp_5->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 6:
                    ui->hp_6->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
            }
            }
        }
    }
    else
    {
        for(int i = 7;i<13;i++)
        {
            if(robots[i].confidence != 0.0)
            {
            armor_number = robots[i].id - 6;
            confidence = robots[i].confidence;
            switch(armor_number)
            {
                case 1:
                    ui->hp_1->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 2: 
                    ui->hp_2->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 3:
                    ui->hp_3->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 4:
                    ui->hp_4->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 5:
                    ui->hp_5->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
                case 6:
                    ui->hp_6->setText(QString::number(confidence) + "  x:" + QString::number(robots[i].x) + "m  y:" + QString::number(robots[i].y) + "m");
                    break;
            }
            }
        }
    }
}

void radarStation::sendRobots(std::vector<DecisionRobot> &robots)
{
    my_msgss::msg::Points send_robots;
    for(int i = 0;i<robots.size();i++)
    {
        if(robots[i].confidence != 0.0)
        {
            my_msgss::msg::Point robot;
            robot.id = robots[i].id;
            robot.confidence = robots[i].confidence;
            robot.x = robots[i].x;
            robot.y = robots[i].y;
            send_robots.data.push_back(robot);
        }
    }
    qtnode.points_pub_->publish(send_robots);
}

void radarStation::decision(std::vector<DecisionRobot> &robots)
{

}