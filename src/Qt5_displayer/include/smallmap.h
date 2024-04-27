#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <QLabel>
#include "my_msgss/msg/points.hpp"

struct mapPos
{
    int id;
    float x;
    float y;
};

struct Robot
{
    int id;
    bool is_continue;
    float confidence;
    float x;
    float y;
};

class smallMap : public QLabel
{
    Q_OBJECT
public:
    smallMap(QWidget * parent);
    ~smallMap();

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void changeScaleValue(QPoint event, int angle);

public:
    QPointF pos;
    QVector<mapPos> farMapPoints;
    QVector<mapPos> closeMapPoints;
    QPixmap image;
    QString redimage_path;
    QString blueimage_path;
    int mapx;
    int mapy;
    int mapcols;
    int maprows;
    QPointF mousePos;
    QPointF drawPos;
    QRect rectPixmap;
    bool isMousePress;
    float scaleValue;

    float SCALL_MAX_VALUE;
    float SCALL_MIN_VALUE;

public:
    //------------------------------------------
    bool our_color; //0:red 1:blue

    std::vector<Robot> far_robots;
    std::vector<Robot> close_robots;

    void robots_init();
    void get_robots(std::vector<Robot> &robots, const my_msgss::msg::Points &input);
    void allrobots_adjust(std::vector<Robot> &robots);
    //------------------------------------------
};

#endif // SMALLMAP_H
