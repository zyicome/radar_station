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
    QStackedWidget *allWidget;
    QWidget *mapWidget;
    QLabel *closeImg;
    smallMap *map;
    QPlainTextEdit *mapMessage;
    QLabel *closeDepth;
    QLabel *farImg;
    QLabel *farDepth;
    QLabel *robot_1;
    QLabel *robot_5;
    QLabel *hp_1;
    QLabel *hp_5;
    QLabel *robot_2;
    QLabel *robot_6;
    QLabel *hp_6;
    QLabel *hp_2;
    QLabel *robot_3;
    QLabel *robot_7;
    QLabel *hp_7;
    QLabel *hp_3;
    QLabel *robot_4;
    QLabel *robot_9;
    QLabel *hp_8;
    QLabel *hp_4;
    QRadioButton *mapMode;
    QRadioButton *pnpMode;
    pnpWidget *solvePnpWidget;
    QRadioButton *mapMode_2;
    QRadioButton *pnpMode_2;

    void setupUi(QMainWindow *radarStation)
    {
        if (radarStation->objectName().isEmpty())
            radarStation->setObjectName(QString::fromUtf8("radarStation"));
        radarStation->resize(1600, 1400);
        centralwidget = new QWidget(radarStation);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setAutoFillBackground(false);
        allWidget = new QStackedWidget(centralwidget);
        allWidget->setObjectName(QString::fromUtf8("allWidget"));
        allWidget->setGeometry(QRect(0, 0, 1600, 1400));
        mapWidget = new QWidget();
        mapWidget->setObjectName(QString::fromUtf8("mapWidget"));
        closeImg = new QLabel(mapWidget);
        closeImg->setObjectName(QString::fromUtf8("closeImg"));
        closeImg->setEnabled(true);
        closeImg->setGeometry(QRect(1070, 40, 391, 321));
        closeImg->setAutoFillBackground(true);
        closeImg->setPixmap(QPixmap(QString::fromUtf8("../map/map.png")));
        closeImg->setScaledContents(true);
        map = new smallMap(mapWidget);
        map->setObjectName(QString::fromUtf8("map"));
        map->setGeometry(QRect(-10, 40, 1041, 611));
        map->setAutoFillBackground(true);
        map->setPixmap(QPixmap(QString::fromUtf8("../map/map.png")));
        map->setScaledContents(true);
        map->setWordWrap(false);
        mapMessage = new QPlainTextEdit(mapWidget);
        mapMessage->setObjectName(QString::fromUtf8("mapMessage"));
        mapMessage->setGeometry(QRect(0, 660, 661, 201));
        mapMessage->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mapMessage->setReadOnly(true);
        mapMessage->setOverwriteMode(false);
        closeDepth = new QLabel(mapWidget);
        closeDepth->setObjectName(QString::fromUtf8("closeDepth"));
        closeDepth->setGeometry(QRect(1070, 360, 391, 321));
        closeDepth->setAutoFillBackground(true);
        closeDepth->setPixmap(QPixmap(QString::fromUtf8("../map/map.png")));
        closeDepth->setScaledContents(true);
        farImg = new QLabel(mapWidget);
        farImg->setObjectName(QString::fromUtf8("farImg"));
        farImg->setGeometry(QRect(1070, 690, 391, 321));
        farImg->setAutoFillBackground(true);
        farImg->setPixmap(QPixmap(QString::fromUtf8("../map/map.png")));
        farImg->setScaledContents(true);
        farDepth = new QLabel(mapWidget);
        farDepth->setObjectName(QString::fromUtf8("farDepth"));
        farDepth->setGeometry(QRect(670, 690, 391, 321));
        farDepth->setAutoFillBackground(true);
        farDepth->setPixmap(QPixmap(QString::fromUtf8("../map/map.png")));
        farDepth->setScaledContents(true);
        robot_1 = new QLabel(mapWidget);
        robot_1->setObjectName(QString::fromUtf8("robot_1"));
        robot_1->setGeometry(QRect(20, 860, 61, 61));
        robot_5 = new QLabel(mapWidget);
        robot_5->setObjectName(QString::fromUtf8("robot_5"));
        robot_5->setGeometry(QRect(20, 940, 61, 61));
        hp_1 = new QLabel(mapWidget);
        hp_1->setObjectName(QString::fromUtf8("hp_1"));
        hp_1->setGeometry(QRect(80, 880, 67, 31));
        hp_5 = new QLabel(mapWidget);
        hp_5->setObjectName(QString::fromUtf8("hp_5"));
        hp_5->setGeometry(QRect(80, 960, 67, 31));
        robot_2 = new QLabel(mapWidget);
        robot_2->setObjectName(QString::fromUtf8("robot_2"));
        robot_2->setGeometry(QRect(160, 860, 61, 61));
        robot_6 = new QLabel(mapWidget);
        robot_6->setObjectName(QString::fromUtf8("robot_6"));
        robot_6->setGeometry(QRect(160, 940, 61, 61));
        hp_6 = new QLabel(mapWidget);
        hp_6->setObjectName(QString::fromUtf8("hp_6"));
        hp_6->setGeometry(QRect(230, 960, 67, 31));
        hp_2 = new QLabel(mapWidget);
        hp_2->setObjectName(QString::fromUtf8("hp_2"));
        hp_2->setGeometry(QRect(230, 880, 67, 31));
        robot_3 = new QLabel(mapWidget);
        robot_3->setObjectName(QString::fromUtf8("robot_3"));
        robot_3->setGeometry(QRect(310, 860, 61, 61));
        robot_7 = new QLabel(mapWidget);
        robot_7->setObjectName(QString::fromUtf8("robot_7"));
        robot_7->setGeometry(QRect(310, 940, 61, 61));
        hp_7 = new QLabel(mapWidget);
        hp_7->setObjectName(QString::fromUtf8("hp_7"));
        hp_7->setGeometry(QRect(380, 960, 67, 31));
        hp_3 = new QLabel(mapWidget);
        hp_3->setObjectName(QString::fromUtf8("hp_3"));
        hp_3->setGeometry(QRect(380, 880, 67, 31));
        robot_4 = new QLabel(mapWidget);
        robot_4->setObjectName(QString::fromUtf8("robot_4"));
        robot_4->setGeometry(QRect(460, 860, 61, 61));
        robot_9 = new QLabel(mapWidget);
        robot_9->setObjectName(QString::fromUtf8("robot_9"));
        robot_9->setGeometry(QRect(460, 940, 61, 61));
        hp_8 = new QLabel(mapWidget);
        hp_8->setObjectName(QString::fromUtf8("hp_8"));
        hp_8->setGeometry(QRect(530, 960, 67, 31));
        hp_4 = new QLabel(mapWidget);
        hp_4->setObjectName(QString::fromUtf8("hp_4"));
        hp_4->setGeometry(QRect(530, 880, 67, 31));
        mapMode = new QRadioButton(mapWidget);
        mapMode->setObjectName(QString::fromUtf8("mapMode"));
        mapMode->setGeometry(QRect(340, 0, 112, 41));
        pnpMode = new QRadioButton(mapWidget);
        pnpMode->setObjectName(QString::fromUtf8("pnpMode"));
        pnpMode->setGeometry(QRect(460, 0, 112, 41));
        allWidget->addWidget(mapWidget);
        closeDepth->raise();
        mapMessage->raise();
        map->raise();
        closeImg->raise();
        farImg->raise();
        farDepth->raise();
        robot_1->raise();
        robot_5->raise();
        hp_1->raise();
        hp_5->raise();
        robot_2->raise();
        robot_6->raise();
        hp_6->raise();
        hp_2->raise();
        robot_3->raise();
        robot_7->raise();
        hp_7->raise();
        hp_3->raise();
        robot_4->raise();
        robot_9->raise();
        hp_8->raise();
        hp_4->raise();
        mapMode->raise();
        pnpMode->raise();
        solvePnpWidget = new pnpWidget();
        solvePnpWidget->setObjectName(QString::fromUtf8("solvePnpWidget"));
        mapMode_2 = new QRadioButton(solvePnpWidget);
        mapMode_2->setObjectName(QString::fromUtf8("mapMode_2"));
        mapMode_2->setGeometry(QRect(260, 10, 112, 41));
        pnpMode_2 = new QRadioButton(solvePnpWidget);
        pnpMode_2->setObjectName(QString::fromUtf8("pnpMode_2"));
        pnpMode_2->setGeometry(QRect(390, 10, 112, 41));
        allWidget->addWidget(solvePnpWidget);
        radarStation->setCentralWidget(centralwidget);

        retranslateUi(radarStation);

        allWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(radarStation);
    } // setupUi

    void retranslateUi(QMainWindow *radarStation)
    {
        radarStation->setWindowTitle(QCoreApplication::translate("radarStation", "radarStation", nullptr));
        closeImg->setText(QString());
        map->setText(QString());
        closeDepth->setText(QString());
        farImg->setText(QString());
        farDepth->setText(QString());
        robot_1->setText(QCoreApplication::translate("radarStation", "1\345\217\267", nullptr));
        robot_5->setText(QCoreApplication::translate("radarStation", "\344\272\224\345\217\267", nullptr));
        hp_1->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        hp_5->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        robot_2->setText(QCoreApplication::translate("radarStation", "2\345\217\267", nullptr));
        robot_6->setText(QCoreApplication::translate("radarStation", "\346\227\240\344\272\272\346\234\272", nullptr));
        hp_6->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        hp_2->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        robot_3->setText(QCoreApplication::translate("radarStation", "3\345\217\267", nullptr));
        robot_7->setText(QCoreApplication::translate("radarStation", "\345\223\250\345\205\265", nullptr));
        hp_7->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        hp_3->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        robot_4->setText(QCoreApplication::translate("radarStation", "\345\233\233\345\217\267", nullptr));
        robot_9->setText(QCoreApplication::translate("radarStation", "\351\243\236\351\225\226", nullptr));
        hp_8->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        hp_4->setText(QCoreApplication::translate("radarStation", "TextLabel", nullptr));
        mapMode->setText(QCoreApplication::translate("radarStation", "\345\260\217\345\234\260\345\233\276\346\250\241\345\274\217", nullptr));
        pnpMode->setText(QCoreApplication::translate("radarStation", "pnp\346\250\241\345\274\217", nullptr));
        mapMode_2->setText(QCoreApplication::translate("radarStation", "\345\260\217\345\234\260\345\233\276\346\250\241\345\274\217", nullptr));
        pnpMode_2->setText(QCoreApplication::translate("radarStation", "pnp\346\250\241\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class radarStation: public Ui_radarStation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADARSTATION_H
