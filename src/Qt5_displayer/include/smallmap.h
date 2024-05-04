#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <QLabel>
#include "my_msgss/msg/points.hpp"

#include "opencv2/opencv.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

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

    void gainRegionInit();

    void mapInit(cv::Mat map,std::vector<cv::Point2f> world_points);

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
    QImage redImage;
    QImage blueImage;
    //QString redimage_path;
    //QString blueimage_path;
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
    //All gain region
    //------------------------------------------
    cv::Mat blueMap;
    cv::Mat redMap;

    std::vector<cv::Point2f> our_supply_region;
    std::vector<cv::Point2f> enemy_supply_region;
    std::vector<cv::Point2f> our_exchange_region;
    std::vector<cv::Point2f> enemy_exchange_region;
    std::vector<cv::Point2f> our_base_region;
    std::vector<cv::Point2f> enemy_base_region;
    std::vector<cv::Point2f> our_R3_region;
    std::vector<cv::Point2f> enemy_R3_region;
    std::vector<cv::Point2f> our_R2_region;
    std::vector<cv::Point2f> enemy_R2_region;
    std::vector<cv::Point2f> our_R4_region;
    std::vector<cv::Point2f> enemy_R4_region;
    std::vector<cv::Point2f> our_energy_region;
    std::vector<cv::Point2f> enemy_energy_region;
    std::vector<cv::Point2f> our_fly_region;
    std::vector<cv::Point2f> enemy_fly_region;
    std::vector<cv::Point2f> our_fly_down_region;
    std::vector<cv::Point2f> enemy_fly_down_region;
    std::vector<cv::Point2f> our_outpost_region;
    std::vector<cv::Point2f> enemy_outpost_region;
    std::vector<cv::Point2f> our_central_island_region;
    std::vector<cv::Point2f> enemy_central_island_region;
    //------------------------------------------
};

#endif // SMALLMAP_H
