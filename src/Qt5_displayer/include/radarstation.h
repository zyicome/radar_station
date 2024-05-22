#ifndef RADARSTATION_H
#define RADARSTATION_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QMutex>
#include "pnpwidget.h"
#include "qtnode.h"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "my_msgss/msg/points.hpp"
#include "smallmap.h"

#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class radarStation; }
QT_END_NAMESPACE

struct DecisionRobot
{
    int id;
    bool is_continue;
    bool is_far_continue;
    bool is_close_continue;
    bool is_120;
    float confidence;
    float x;
    float y;
    float radar_mark_progress;
    float hp;
};

class radarStation : public QMainWindow
{
    Q_OBJECT

public:
    radarStation(QWidget * parent = nullptr);
    ~radarStation();

    void mousePressEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void mapMessageDisplay(QString text);

    void robots_init();

    void robots_adjust(std::vector<Robot> &get_robots, bool is_far);

    void all_robots_adjust(bool is_far);

    void status_adjust(std::vector<DecisionRobot> &robots);

    void sendRobots(std::vector<DecisionRobot> &robots);

    void decision(std::vector<DecisionRobot> &robots);

    void init();
public Q_SLOTS:
    void changeToMapWidget();

    void changeToPnpWidget();

    void changeToMapWidget_2();

    void changeToPnpWidget_2();

    void farImageUpdate();

    void closeImageUpdate();

    //void farDepthImageUpdate();

    //void closeDepthImageUpdate();

    void publishPnpResult();

    void farPointsUpdate();

    void closePointsUpdate();

    void gameStateUpdate();

    void radarMarkUpdate();

    void radarInfoUpdate();

    void hpUpdate();

    void blueMode();

    void redMode();

    void saveVideo();
private:
    Ui::radarStation *ui;
    qtNode qtnode;
    mutable QMutex far_qimage_mutex;
    mutable QMutex close_qimage_mutex;

    std::vector<DecisionRobot> robots;

    int radar_info;
    bool was_first;
    bool was_second;
    
    QPoint pos;

};
#endif // RADARSTATION_H
