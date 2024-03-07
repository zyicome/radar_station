/********************************************************************************
** Form generated from reading UI file 'radarstation.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADARSTATION_H
#define UI_RADARSTATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <pnpwidget.h>
#include <smallmap.h>

QT_BEGIN_NAMESPACE

class Ui_radarStation
{
public:
    QWidget *centralwidget;
    QRadioButton *pnpMode;
    QRadioButton *mapMode;
    QStackedWidget *allWidget;
    QWidget *mapWidget;
    QLabel *closeImg;
    smallMap *map;
    QPlainTextEdit *mapMessage;
    QLabel *closeDepth;
    QLabel *farImg;
    QLabel *farDepth;
    pnpWidget *solvePnpWidget;

    void setupUi(QMainWindow *radarStation)
    {
        if (radarStation->objectName().isEmpty())
            radarStation->setObjectName(QString::fromUtf8("radarStation"));
        radarStation->resize(1600, 1400);
        centralwidget = new QWidget(radarStation);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setAutoFillBackground(false);
        pnpMode = new QRadioButton(centralwidget);
        pnpMode->setObjectName(QString::fromUtf8("pnpMode"));
        pnpMode->setGeometry(QRect(430, 20, 101, 51));
        pnpMode->setChecked(false);
        mapMode = new QRadioButton(centralwidget);
        mapMode->setObjectName(QString::fromUtf8("mapMode"));
        mapMode->setGeometry(QRect(290, 20, 141, 51));
        mapMode->setCheckable(true);
        mapMode->setChecked(true);
        mapMode->setAutoRepeat(false);
        allWidget = new QStackedWidget(centralwidget);
        allWidget->setObjectName(QString::fromUtf8("allWidget"));
        allWidget->setGeometry(QRect(0, 0, 1600, 1400));
        mapWidget = new QWidget();
        mapWidget->setObjectName(QString::fromUtf8("mapWidget"));
        closeImg = new QLabel(mapWidget);
        closeImg->setObjectName(QString::fromUtf8("closeImg"));
        closeImg->setEnabled(true);
        closeImg->setGeometry(QRect(1150, 80, 391, 321));
        closeImg->setAutoFillBackground(true);
        closeImg->setPixmap(QPixmap(QString::fromUtf8("C:/Users/zyb/Pictures/Saved Pictures/mylearnlow1.jpg")));
        closeImg->setScaledContents(true);
        map = new smallMap(mapWidget);
        map->setObjectName(QString::fromUtf8("map"));
        map->setGeometry(QRect(0, 80, 1041, 611));
        map->setAutoFillBackground(true);
        map->setPixmap(QPixmap(QString::fromUtf8(":/map/C:/Users/zyb/Desktop/Robotmaster/pitures/map.png")));
        map->setScaledContents(true);
        map->setWordWrap(false);
        mapMessage = new QPlainTextEdit(mapWidget);
        mapMessage->setObjectName(QString::fromUtf8("mapMessage"));
        mapMessage->setGeometry(QRect(0, 750, 1041, 401));
        mapMessage->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mapMessage->setReadOnly(true);
        mapMessage->setOverwriteMode(false);
        closeDepth = new QLabel(mapWidget);
        closeDepth->setObjectName(QString::fromUtf8("closeDepth"));
        closeDepth->setGeometry(QRect(1150, 400, 391, 321));
        closeDepth->setAutoFillBackground(true);
        closeDepth->setPixmap(QPixmap(QString::fromUtf8(":/map/C:/Users/zyb/Desktop/Robotmaster/pitures/map.png")));
        closeDepth->setScaledContents(true);
        farImg = new QLabel(mapWidget);
        farImg->setObjectName(QString::fromUtf8("farImg"));
        farImg->setGeometry(QRect(1150, 720, 391, 291));
        farImg->setAutoFillBackground(true);
        farImg->setPixmap(QPixmap(QString::fromUtf8(":/map/C:/Users/zyb/Desktop/Robotmaster/pitures/map.png")));
        farImg->setScaledContents(true);
        farDepth = new QLabel(mapWidget);
        farDepth->setObjectName(QString::fromUtf8("farDepth"));
        farDepth->setGeometry(QRect(1150, 1020, 391, 291));
        farDepth->setAutoFillBackground(true);
        farDepth->setScaledContents(true);
        allWidget->addWidget(mapWidget);
        closeDepth->raise();
        mapMessage->raise();
        map->raise();
        closeImg->raise();
        farImg->raise();
        farDepth->raise();
        solvePnpWidget = new pnpWidget();
        solvePnpWidget->setObjectName(QString::fromUtf8("solvePnpWidget"));
        allWidget->addWidget(solvePnpWidget);
        radarStation->setCentralWidget(centralwidget);
        allWidget->raise();
        pnpMode->raise();
        mapMode->raise();

        retranslateUi(radarStation);

        allWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(radarStation);
    } // setupUi

    void retranslateUi(QMainWindow *radarStation)
    {
        radarStation->setWindowTitle(QCoreApplication::translate("radarStation", "radarStation", nullptr));
        pnpMode->setText(QCoreApplication::translate("radarStation", "PnP\346\250\241\345\274\217", nullptr));
        mapMode->setText(QCoreApplication::translate("radarStation", "\345\260\217\345\234\260\345\233\276\346\250\241\345\274\217", nullptr));
        closeImg->setText(QString());
        map->setText(QString());
        closeDepth->setText(QString());
        farImg->setText(QString());
        farDepth->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class radarStation: public Ui_radarStation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADARSTATION_H
