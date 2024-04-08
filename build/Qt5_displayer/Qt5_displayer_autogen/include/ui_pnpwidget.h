/********************************************************************************
** Form generated from reading UI file 'pnpwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PNPWIDGET_H
#define UI_PNPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pnpWidget
{
public:
    QLabel *farImg;
    QPushButton *beginPoint;
    QPushButton *restartPoint;
    QPushButton *beginPnp;
    QPlainTextEdit *message;
    QLabel *closeImg;

    void setupUi(QWidget *pnpWidget)
    {
        if (pnpWidget->objectName().isEmpty())
            pnpWidget->setObjectName(QString::fromUtf8("pnpWidget"));
        pnpWidget->resize(1600, 1400);
        farImg = new QLabel(pnpWidget);
        farImg->setObjectName(QString::fromUtf8("farImg"));
        farImg->setGeometry(QRect(90, 90, 640, 480));
        farImg->setFocusPolicy(Qt::NoFocus);
        farImg->setToolTipDuration(-1);
        farImg->setAutoFillBackground(true);
        farImg->setPixmap(QPixmap(QString::fromUtf8(":/map/C:/Users/zyb/Desktop/Robotmaster/pitures/map.png")));
        farImg->setScaledContents(true);
        farImg->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        beginPoint = new QPushButton(pnpWidget);
        beginPoint->setObjectName(QString::fromUtf8("beginPoint"));
        beginPoint->setGeometry(QRect(740, 80, 121, 31));
        restartPoint = new QPushButton(pnpWidget);
        restartPoint->setObjectName(QString::fromUtf8("restartPoint"));
        restartPoint->setGeometry(QRect(740, 120, 121, 31));
        beginPnp = new QPushButton(pnpWidget);
        beginPnp->setObjectName(QString::fromUtf8("beginPnp"));
        beginPnp->setGeometry(QRect(740, 160, 121, 31));
        message = new QPlainTextEdit(pnpWidget);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(90, 580, 1421, 391));
        message->setReadOnly(true);
        closeImg = new QLabel(pnpWidget);
        closeImg->setObjectName(QString::fromUtf8("closeImg"));
        closeImg->setGeometry(QRect(870, 90, 640, 480));
        closeImg->setAutoFillBackground(true);
        closeImg->setPixmap(QPixmap(QString::fromUtf8(":/map/C:/Users/zyb/Desktop/Robotmaster/pitures/map.png")));
        closeImg->setScaledContents(true);
        beginPoint->raise();
        restartPoint->raise();
        beginPnp->raise();
        farImg->raise();
        message->raise();
        closeImg->raise();

        retranslateUi(pnpWidget);

        QMetaObject::connectSlotsByName(pnpWidget);
    } // setupUi

    void retranslateUi(QWidget *pnpWidget)
    {
        pnpWidget->setWindowTitle(QCoreApplication::translate("pnpWidget", "Form", nullptr));
        farImg->setText(QString());
        beginPoint->setText(QCoreApplication::translate("pnpWidget", "\345\274\200\345\247\213\346\240\207\347\202\271", nullptr));
        restartPoint->setText(QCoreApplication::translate("pnpWidget", "\351\207\215\346\226\260\346\240\207\347\202\271", nullptr));
        beginPnp->setText(QCoreApplication::translate("pnpWidget", "\345\274\200\345\247\213pnp\347\273\223\347\256\227", nullptr));
        closeImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class pnpWidget: public Ui_pnpWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PNPWIDGET_H
