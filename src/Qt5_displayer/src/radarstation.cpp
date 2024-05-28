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
    QString img_path = QString::fromStdString(ament_index_cpp::get_package_share_directory("Qt5_displayer"));
    QString left_scroll_image_path = img_path + "/map/left.png";
    QString right_scroll_image_path = img_path + "/map/right.png";

    QImage left_scroll_image;
    QImage right_scroll_image;
    left_scroll_image.load(left_scroll_image_path);
    right_scroll_image.load(right_scroll_image_path);

    ui->leftScroll->setPixmap(QPixmap::fromImage(left_scroll_image));
    ui->rightScroll->setPixmap(QPixmap::fromImage(right_scroll_image));

    ui->map->mapx = ui->map->x();
    ui->map->mapy = ui->map->y();
    ui->map->mapcols = ui->map->width();
    ui->map->maprows = ui->map->height();
    ui->allWidget->setCurrentWidget(ui->mapWidget);

    QThread far_thread;
    QThread close_thread;
    //ui->farImg->moveToThread(&far_thread);
    //ui->closeImg->moveToThread(&close_thread);

    init();
    robots_init();

    was_first = false;
    was_second = false;
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

    connect(ui->isSaveVideo,SIGNAL(clicked()),this,SLOT(saveVideo()));

    connect(&this->qtnode,SIGNAL(updateFarImage()),this,SLOT(farImageUpdate()));
    //connect(&this->qtnode,SIGNAL(updateDepthImage()),this,SLOT(farDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateFarPoints()),this,SLOT(farPointsUpdate()));

    connect(&this->qtnode,SIGNAL(updateCloseImage()),this,SLOT(closeImageUpdate()));
    //connect(&this->qtnode,SIGNAL(updateCloseDepthImage()),this,SLOT(closeDepthImageUpdate()));
    connect(&this->qtnode,SIGNAL(updateClosePoints()),this,SLOT(closePointsUpdate()));

    connect(&this->qtnode,SIGNAL(updateGameState()),this,SLOT(gameStateUpdate()));
    connect(&this->qtnode,SIGNAL(updateRadarMark()),this,SLOT(radarMarkUpdate()));
    connect(&this->qtnode,SIGNAL(updateRadarInfo()),this,SLOT(radarInfoUpdate()));
    connect(&this->qtnode,SIGNAL(updateHp()),this,SLOT(hpUpdate()));

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
    far_qimage_mutex.lock();
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
    far_qimage_mutex.unlock();
}

void radarStation::closeImageUpdate()
{
    close_qimage_mutex.lock();
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
    close_qimage_mutex.unlock();
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
    if(ui->solvePnpWidget->pnp_img_id == 1)
    {
        pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(0,0));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(0,1));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(0,2));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(1,0));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(1,1));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(1,2));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(2,0));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(2,1));
    pnp_result.data.push_back(ui->solvePnpWidget->close_R.at<double>(2,2));
    pnp_result.data.push_back(ui->solvePnpWidget->close_T.at<double>(0,0));
    pnp_result.data.push_back(ui->solvePnpWidget->close_T.at<double>(1,0));
    pnp_result.data.push_back(ui->solvePnpWidget->close_T.at<double>(2,0));
    }
    else{
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
    }
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
            farpos.x = ui->map->far_robots[i].y / object_width * width;
            //farpos.y = height - (ui->map->far_robots[i].y / object_height * height);
            farpos.y = (ui->map->far_robots[i].x / object_height * height);
            farpos.x = farpos.x + ui->map->drawPos.x();
            farpos.y = farpos.y + ui->map->drawPos.y();
            ui->map->far_robots[i].x = farpos.x;
            ui->map->far_robots[i].y = farpos.y;
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
            closepos.x = ui->map->close_robots[i].y / object_width * width;
            //closepos.y = height - (ui->map->close_robots[i].y / object_height * height);
            closepos.y = (ui->map->close_robots[i].x / object_height * height);
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
    ui->gameRemainTime->setText(QString::number(game_state.stage_remain_time) + "s");
    if(game_state.stage_remain_time < 120 && radar_info <= 2 && was_first == false)
    {
        my_msgss::msg::Radarinfo radar_info_msg;
        radar_info_msg.radar_cmd = 1;
        qtnode.radar_info_pub_->publish(radar_info_msg);
        was_first = true;
        std::cout << "radar_info_msg:" << radar_info_msg.radar_cmd << std::endl;
    }
    if(game_state.stage_remain_time < 60 && radar_info == 2 && was_second == false)
    {
        my_msgss::msg::Radarinfo radar_info_msg;
        radar_info_msg.radar_cmd = 2;
        qtnode.radar_info_pub_->publish(radar_info_msg);
        was_second = true;
        std::cout << "radar_info_msg:" << radar_info_msg.radar_cmd << std::endl;
    }
}

void radarStation::radarMarkUpdate()
{
    int armor_number = 0;
    my_msgss::msg::Radarmark radar_mark = qtnode.radar_mark_msg;
    if(ui->map->our_color == 0)
    {
        for(int i =1;i<7;i++)
        {
            armor_number = i;
            switch(armor_number)
            {
                case 1:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_hero_progress;
                    ui->markProgress_1->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 2:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_engineer_progress;
                    ui->markProgress_2->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 3:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_3_progress;
                    ui->markProgress_3->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 4:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_4_progress;
                    ui->markProgress_4->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 5:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_5_progress;
                    ui->markProgress_5->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 6:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_sentry_progress;
                    ui->markProgress_6->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
            }
        }
    }
    else
    {
        for(int i =7;i<robots.size();i++)
        {
            armor_number = i;
            switch(armor_number)
            {
                case 7:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_hero_progress;
                    ui->markProgress_1->setText(QString::number(robots[armor_number].radar_mark_progress,'f',2));
                    break;
                case 8:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_engineer_progress;
                    ui->markProgress_2->setText(QString::number(robots[armor_number].radar_mark_progress,'f',2));
                    break;
                case 9:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_3_progress;
                    ui->markProgress_3->setText(QString::number(robots[armor_number].radar_mark_progress,'f',2));
                    break;
                case 10:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_4_progress;
                    ui->markProgress_4->setText(QString::number(robots[armor_number].radar_mark_progress,'f',2));
                    break;
                case 11:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_standard_5_progress;
                    ui->markProgress_5->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
                case 12:
                    robots[armor_number].radar_mark_progress = radar_mark.mark_sentry_progress;
                    ui->markProgress_6->setText(QString::number(robots[armor_number].radar_mark_progress));
                    break;
            }
        }
    }
}

void radarStation::radarInfoUpdate()
{
    my_msgss::msg::Radarinfo radar_info_msg = qtnode.radar_info_msg;
    radar_info = radar_info_msg.radar_info;
    ui->radarInfo->setText(QString::number(radar_info_msg.radar_info) + "次");
}

void radarStation::hpUpdate()
{
    my_msgss::msg::Hp hp_msg = qtnode.hp_msg;
    robots[1].hp = hp_msg.blue_1_hp;
    robots[2].hp = hp_msg.blue_2_hp;
    robots[3].hp = hp_msg.blue_3_hp;
    robots[4].hp = hp_msg.blue_4_hp;
    robots[5].hp = hp_msg.blue_5_hp;
    robots[6].hp = hp_msg.blue_7_hp;
    robots[7].hp = hp_msg.red_1_hp;
    robots[8].hp = hp_msg.red_2_hp;
    robots[9].hp = hp_msg.red_3_hp;
    robots[10].hp = hp_msg.red_4_hp;
    robots[11].hp = hp_msg.red_5_hp;
    robots[12].hp = hp_msg.red_7_hp;
    if(ui->map->our_color == 0) //red
    {
        ui->ourOutPostHp->setText(QString::number(hp_msg.red_outpose_hp));
        ui->enemyOutPostHp->setText(QString::number(hp_msg.blue_outpose_hp));
    }
    else
    {
        ui->ourOutPostHp->setText(QString::number(hp_msg.blue_outpose_hp));
        ui->enemyOutPostHp->setText(QString::number(hp_msg.red_outpose_hp));
    }
}

void radarStation::blueMode()
{
    ui->map->our_color = 1;
    ui->map->image = QPixmap::fromImage(ui->map->blueImage);
    std_msgs::msg::Int8 color;
    color.data = 1;
    qtnode.color_pub_->publish(color);
    ui->map->update();
}

void radarStation::redMode()
{
    ui->map->our_color = 0;
    ui->map->image = QPixmap::fromImage(ui->map->redImage);
    std_msgs::msg::Int8 color;
    color.data = 0;
    qtnode.color_pub_->publish(color);
    ui->map->update();
}

void radarStation::saveVideo()
{
    std_msgs::msg::Int8 is_save;
    is_save.data = 1;
    qtnode.is_save_video_pub_->publish(is_save);
    ui->saveVideoStatus->setText("正在录制");
}

void radarStation::robots_init()
{
    DecisionRobot robot;
    robot.id = -1;
    robot.confidence = 0.0;
    robot.is_continue = false;
    robot.is_far_continue = false;
    robot.is_close_continue = false;
    robot.is_120 =false;
    robot.x = 0.0;
    robot.y = 0.0;
    robot.radar_mark_progress = 0;
    robot.hp = 0;
    for(int i =0;i<13;i++)
    {
        robot.id++;
        robots.push_back(robot);
    }
}

void radarStation::robots_adjust(std::vector<Robot> &get_robots, bool is_far)
{
    float object_width = 28;
    float object_height = 14;
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
            x = get_robots[i].x;
            //cout << "width:" << width << " height:" << height << endl;
            //cout << "get_robots_x:" << get_robots[i].x << " get_robots_y:" << get_robots[i].y << endl;
            y =  get_robots[i].y;
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


void radarStation::all_robots_adjust(bool is_far)
{
    for(int i = 0;i<robots.size();i++)
    {
        /*std::cout << "robots_id:" << robots[i].id << " robots_confidence:" << robots[i].confidence << std::endl;
        std::cout << "robots_x:" << robots[i].x << " robots_y:" << robots[i].y << std::endl;
        std::cout << "robots_is_continue:" << robots[i].is_continue << std::endl;
        std::cout << "robots_is_far_continue:" << robots[i].is_far_continue << std::endl;
        std::cout << "robots_is_close_continue:" << robots[i].is_close_continue << std::endl;*/
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
                    ui->baseMessage_1->setText(QString::number(confidence));
                    ui->location_1->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 2: 
                    ui->baseMessage_2->setText(QString::number(confidence));
                    ui->location_2->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 3:
                    ui->baseMessage_3->setText(QString::number(confidence));
                    ui->location_3->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 4:
                    ui->baseMessage_4->setText(QString::number(confidence));
                    ui->location_4->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 5:
                    ui->baseMessage_5->setText(QString::number(confidence));
                    ui->location_5->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 6:
                    ui->baseMessage_6->setText(QString::number(confidence));
                    ui->location_6->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
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
                    ui->baseMessage_1->setText(QString::number(confidence));
                    ui->location_1->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 2: 
                    ui->baseMessage_2->setText(QString::number(confidence));
                    ui->location_2->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 3:
                    ui->baseMessage_3->setText(QString::number(confidence));
                    ui->location_3->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 4:
                    ui->baseMessage_4->setText(QString::number(confidence));
                    ui->location_4->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 5:
                    ui->baseMessage_5->setText(QString::number(confidence));
                    ui->location_5->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
                case 6:
                    ui->baseMessage_6->setText(QString::number(confidence));
                    ui->location_6->setText("x:" + QString::number(robots[i].x,'f',2) + "m y:" + QString::number(robots[i].y,'f',2) + "m");
                    break;
            }
            }
        }
    }
}

void radarStation::sendRobots(std::vector<DecisionRobot> &robots)
{
    my_msgss::msg::Points send_robots;
    bool if_send =false;
    DecisionRobot best_robot;
    best_robot.confidence = 0.0;
    if(ui->map->our_color == 0) // 我们是红方
    {
        for(int i =1;i<7;i++)
        {
            if(robots[i].x !=0.0 && robots[i].y != 0.0)
            {
                if(robots[i].radar_mark_progress >= 120)
                {
                    robots[i].is_120 = true;
                }
                else if(robots[i].radar_mark_progress <= 60)
                {
                    robots[i].is_120 =false;
                }
                if(robots[i].confidence < 0.3)
                {
                    continue;
                }
                if(robots[i].radar_mark_progress >115 && robots[i].is_120 == true)
                {
                    continue;
                }
                if(robots[i].radar_mark_progress <= 115 && robots[i].radar_mark_progress >= 100 && robots[i].is_120 ==true)
                {
                    best_robot = robots[i];
                    if_send = true;
                    continue;
                }
                if(robots[i].id == 1)
                {
                    best_robot = robots[i];
                    best_robot.confidence = 1.0;
                    if_send = true;
                    continue;
                }
                else if(robots[i].id == 6)
                {
                    best_robot = robots[i];
                    best_robot.confidence = 0.95;
                    if_send = true;
                    continue;
                }
                if(robots[i].confidence > best_robot.confidence)
                {
                    best_robot = robots[i];
                    if_send = true;
                    continue;
                }
            }
        }
    }
    else if(ui->map->our_color == 1) // 我们是蓝方
    {
        for(int i =7;i<robots.size();i++)
        {
            if(robots[i].x !=0.0 && robots[i].y != 0.0)
            {
                if(robots[i].radar_mark_progress >= 120)
                {
                    robots[i].is_120 = true;
                }
                else if(robots[i].radar_mark_progress <= 60)
                {
                    robots[i].is_120 =false;
                }
                if(robots[i].confidence < 0.3)
                {
                    continue;
                }
                if(robots[i].radar_mark_progress >115 && robots[i].is_120 == true)
                {
                    continue;
                }
                if(robots[i].radar_mark_progress <= 115 && robots[i].radar_mark_progress >= 100 && robots[i].is_120 ==true)
                {
                    best_robot = robots[i];
                    best_robot.id -=6;
                    if_send = true;
                    continue;
                }
                if(robots[i].id == 1)
                {
                    best_robot = robots[i];
                    best_robot.id -=6;
                    best_robot.confidence = 1.0;
                    if_send = true;
                    continue;
                }
                else if(robots[i].id == 6)
                {
                    best_robot = robots[i];
                    best_robot.id -=6;
                    best_robot.confidence = 0.95;
                    if_send = true;
                    continue;
                }
                if(robots[i].confidence > best_robot.confidence)
                {
                    best_robot = robots[i];
                    best_robot.id -=6; 
                    if_send = true;
                    continue;
                }
            }
        }
    }
    if(if_send)
    {
        my_msgss::msg::Point send_robot;
        send_robot.id = best_robot.id;
        send_robot.x = best_robot.x;
        send_robot.y = best_robot.y;
        send_robot.confidence = best_robot.confidence;
        send_robots.data.push_back(send_robot);
        qtnode.points_pub_->publish(send_robots);
    }
    /*for(int i = 0;i<robots.size();i++)
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
    }*/
}

void radarStation::decision(std::vector<DecisionRobot> &robots)
{
    int armor_number = 0;
    int number_begin = 0;
    int number_end = 0;
    if(ui->map->our_color) // 我们是红方，只需要检索蓝方
    {
        number_begin = 1;
        number_end = 7;
    }
    else
    {
        number_begin = 7;
        number_end = 13;
    }

    QString decisionText;

    for(int i = number_begin;i<number_end;i++)
    {
        armor_number = i;
        if(robots[armor_number].confidence != 0.0)
        {
            cv::Point2f robot_point = cv::Point(robots[armor_number].x,robots[armor_number].y);
            if(pointPolygonTest(ui->map->our_R3_region,robot_point,false) == 1)
            {
                decisionText = "注意注意！！!  " + QString::number(armor_number) + "号机器人在我方R3区域内";
            }
            else if(pointPolygonTest(ui->map->our_R2_region,robot_point,false) == 1)
            {
                decisionText = "注意注意！！!  " + QString::number(armor_number) + "号机器人在我方R2区域内";
            }
            else if(pointPolygonTest(ui->map->our_R4_region,robot_point,false) == 1)
            {
                decisionText = "注意注意！！!  " + QString::number(armor_number) + "号机器人在我方R4区域内";
            }
            else if(pointPolygonTest(ui->map->our_energy_region,robot_point,false) == 1)
            {
                decisionText = "注意注意！！!  " + QString::number(armor_number) + "号机器人在我方能量机关区域内";
            }
            else if(pointPolygonTest(ui->map->enemy_fly_down_region,robot_point,false) == 1)
            {
                decisionText = "注意注意！！!  " + QString::number(armor_number) + "号机器人飞坡!!!!!!";
            }
        }
    }
}