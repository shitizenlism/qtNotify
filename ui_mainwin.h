/********************************************************************************
** Form generated from reading UI file 'mainwin.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWin
{
public:
    QWidget *m_iCentralWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QWidget *widget;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *tableA_videoID;
    QLabel *lable_2;
    QLabel *label_3;
    QLineEdit *tableA_obsID;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *datasvr_host;
    QLabel *label_7;
    QLineEdit *datasvr_port;
    QLabel *label_8;
    QToolButton *Btn_SwitchToTableA;
    QToolButton *Btn_SwitchToTableB;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *tableB_videoID;
    QSpinBox *tableA_tableID;
    QLabel *label_11;
    QLineEdit *tableB_obsID;
    QSpinBox *tableB_tableID;

    void setupUi(QMainWindow *CMainWin)
    {
        if (CMainWin->objectName().isEmpty())
            CMainWin->setObjectName(QStringLiteral("CMainWin"));
        CMainWin->resize(1000, 400);
        CMainWin->setMinimumSize(QSize(1000, 400));
        CMainWin->setMaximumSize(QSize(1000, 400));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        CMainWin->setFont(font);
        m_iCentralWidget = new QWidget(CMainWin);
        m_iCentralWidget->setObjectName(QStringLiteral("m_iCentralWidget"));
        m_iCentralWidget->setMaximumSize(QSize(1920, 1080));
        verticalLayout = new QVBoxLayout(m_iCentralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QWidget(m_iCentralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 1221, 391));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setWeight(50);
        groupBox->setFont(font1);
        groupBox->setAlignment(Qt::AlignCenter);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 81, 40));
        label->setMinimumSize(QSize(60, 40));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        tableA_videoID = new QLineEdit(groupBox);
        tableA_videoID->setObjectName(QStringLiteral("tableA_videoID"));
        tableA_videoID->setGeometry(QRect(120, 60, 151, 32));
        tableA_videoID->setMinimumSize(QSize(120, 32));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setWeight(50);
        tableA_videoID->setFont(font3);
        lable_2 = new QLabel(groupBox);
        lable_2->setObjectName(QStringLiteral("lable_2"));
        lable_2->setGeometry(QRect(40, 61, 71, 41));
        lable_2->setMinimumSize(QSize(60, 32));
        lable_2->setFont(font3);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 100, 80, 32));
        label_3->setMinimumSize(QSize(80, 32));
        label_3->setFont(font3);
        tableA_obsID = new QLineEdit(groupBox);
        tableA_obsID->setObjectName(QStringLiteral("tableA_obsID"));
        tableA_obsID->setGeometry(QRect(120, 100, 121, 32));
        tableA_obsID->setMinimumSize(QSize(120, 32));
        tableA_obsID->setFont(font3);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 140, 80, 32));
        label_4->setMinimumSize(QSize(80, 32));
        label_4->setFont(font3);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(340, 30, 101, 40));
        label_5->setMinimumSize(QSize(60, 40));
        label_5->setFont(font2);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(370, 70, 81, 32));
        label_6->setMinimumSize(QSize(60, 32));
        label_6->setFont(font3);
        datasvr_host = new QLineEdit(groupBox);
        datasvr_host->setObjectName(QStringLiteral("datasvr_host"));
        datasvr_host->setGeometry(QRect(460, 70, 161, 32));
        datasvr_host->setMinimumSize(QSize(120, 32));
        datasvr_host->setFont(font3);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(380, 110, 61, 32));
        label_7->setMinimumSize(QSize(60, 32));
        label_7->setFont(font3);
        datasvr_port = new QLineEdit(groupBox);
        datasvr_port->setObjectName(QStringLiteral("datasvr_port"));
        datasvr_port->setGeometry(QRect(460, 110, 120, 32));
        datasvr_port->setMinimumSize(QSize(120, 32));
        datasvr_port->setFont(font3);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(730, 150, 81, 32));
        label_8->setMinimumSize(QSize(60, 32));
        label_8->setFont(font3);
        Btn_SwitchToTableA = new QToolButton(groupBox);
        Btn_SwitchToTableA->setObjectName(QStringLiteral("Btn_SwitchToTableA"));
        Btn_SwitchToTableA->setEnabled(true);
        Btn_SwitchToTableA->setGeometry(QRect(30, 250, 260, 64));
        Btn_SwitchToTableA->setMinimumSize(QSize(260, 64));
        Btn_SwitchToTableA->setMaximumSize(QSize(200, 64));
        QPalette palette;
        QBrush brush(QColor(0, 0, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(0, 120, 215, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush1);
        QBrush brush2(QColor(240, 240, 240, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        QBrush brush3(QColor(0, 0, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush3);
        Btn_SwitchToTableA->setPalette(palette);
        Btn_SwitchToTableA->setFont(font);
        Btn_SwitchToTableB = new QToolButton(groupBox);
        Btn_SwitchToTableB->setObjectName(QStringLiteral("Btn_SwitchToTableB"));
        Btn_SwitchToTableB->setGeometry(QRect(690, 250, 260, 64));
        Btn_SwitchToTableB->setMinimumSize(QSize(260, 64));
        Btn_SwitchToTableB->setMaximumSize(QSize(200, 64));
        Btn_SwitchToTableB->setFont(font);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(710, 30, 81, 40));
        label_9->setMinimumSize(QSize(60, 40));
        label_9->setFont(font2);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(730, 70, 71, 32));
        label_10->setMinimumSize(QSize(60, 32));
        label_10->setFont(font3);
        tableB_videoID = new QLineEdit(groupBox);
        tableB_videoID->setObjectName(QStringLiteral("tableB_videoID"));
        tableB_videoID->setGeometry(QRect(800, 70, 151, 32));
        tableB_videoID->setMinimumSize(QSize(120, 32));
        tableB_videoID->setFont(font3);
        tableA_tableID = new QSpinBox(groupBox);
        tableA_tableID->setObjectName(QStringLiteral("tableA_tableID"));
        tableA_tableID->setGeometry(QRect(120, 140, 61, 31));
        tableA_tableID->setFont(font3);
        tableA_tableID->setMinimum(1);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(730, 110, 61, 32));
        label_11->setMinimumSize(QSize(60, 32));
        label_11->setFont(font3);
        tableB_obsID = new QLineEdit(groupBox);
        tableB_obsID->setObjectName(QStringLiteral("tableB_obsID"));
        tableB_obsID->setGeometry(QRect(800, 110, 120, 32));
        tableB_obsID->setMinimumSize(QSize(120, 32));
        tableB_obsID->setFont(font3);
        tableB_tableID = new QSpinBox(groupBox);
        tableB_tableID->setObjectName(QStringLiteral("tableB_tableID"));
        tableB_tableID->setGeometry(QRect(800, 150, 61, 31));
        tableB_tableID->setFont(font3);
        tableB_tableID->setMinimum(1);
        tableB_tableID->setValue(2);

        gridLayout->addWidget(widget, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        CMainWin->setCentralWidget(m_iCentralWidget);

        retranslateUi(CMainWin);

        QMetaObject::connectSlotsByName(CMainWin);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWin)
    {
        CMainWin->setWindowTitle(QApplication::translate("CMainWin", "data server notifyer", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("CMainWin", "Table Config and Click to Notify", Q_NULLPTR));
        label->setText(QApplication::translate("CMainWin", "TableA", Q_NULLPTR));
        tableA_videoID->setText(QApplication::translate("CMainWin", "BG01", Q_NULLPTR));
        lable_2->setText(QApplication::translate("CMainWin", "videoID:", Q_NULLPTR));
        label_3->setText(QApplication::translate("CMainWin", "obsID:", Q_NULLPTR));
        tableA_obsID->setText(QApplication::translate("CMainWin", "bgtz-a", Q_NULLPTR));
        label_4->setText(QApplication::translate("CMainWin", "tableID:", Q_NULLPTR));
        label_5->setText(QApplication::translate("CMainWin", "Data Server", Q_NULLPTR));
        label_6->setText(QApplication::translate("CMainWin", "host or IP:", Q_NULLPTR));
        datasvr_host->setText(QApplication::translate("CMainWin", "127.0.0.1", Q_NULLPTR));
        label_7->setText(QApplication::translate("CMainWin", "port:", Q_NULLPTR));
        datasvr_port->setText(QApplication::translate("CMainWin", "17010", Q_NULLPTR));
        label_8->setText(QApplication::translate("CMainWin", "tableID:", Q_NULLPTR));
        Btn_SwitchToTableA->setText(QApplication::translate("CMainWin", "Switch To TableA", Q_NULLPTR));
        Btn_SwitchToTableB->setText(QApplication::translate("CMainWin", "Switch To TableB", Q_NULLPTR));
        label_9->setText(QApplication::translate("CMainWin", "TableB", Q_NULLPTR));
        label_10->setText(QApplication::translate("CMainWin", "videoID:", Q_NULLPTR));
        tableB_videoID->setText(QApplication::translate("CMainWin", "BG02", Q_NULLPTR));
        label_11->setText(QApplication::translate("CMainWin", "obsID:", Q_NULLPTR));
        tableB_obsID->setText(QApplication::translate("CMainWin", "bgtz-b", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CMainWin: public Ui_CMainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
