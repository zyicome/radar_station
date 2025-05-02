#include "radarstation.h"
#include <QApplication>
#include <iostream>
#include <QtCore/qmetatype.h>
Q_DECLARE_METATYPE(cv::Mat)

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat");  // 注册 cv::Mat 类型
    radarStation w;
    w.show();
    return a.exec();
}
