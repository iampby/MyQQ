/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <label.h>
#include "combobox.h"
#include "subcombobox.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *widget;
    QFrame *frame;
    QWidget *bodyWidget;
    QStackedWidget *mainStackedWidget;
    QWidget *findPerson;
    QLineEdit *findPersonEdit;
    QCheckBox *liveCBox;
    ComboBox *whereCBox;
    ComboBox *hometownCBox;
    ComboBox *sexCBox;
    ComboBox *ageCBox;
    QPushButton *findBtn;
    QStackedWidget *findPersonStackedWidget;
    QWidget *initPage;
    QLabel *backgrounLabel;
    QWidget *findPage;
    QFrame *frame2;
    QLabel *arrowForReturnLabel;
    QLabel *separaterLabel;
    QLabel *findPersonInfLabel;
    Label *returnLabel;
    QStackedWidget *userListStackWindget;
    QWidget *page_1;
    QWidget *page_2;
    QLabel *label_2;
    QWidget *page_3;
    Label *sexLabel;
    Label *ageLabel;
    QLabel *lineLabel;
    QPushButton *friendBtn;
    QPushButton *townsmanBtn;
    QWidget *cityPop1;
    SubComboBox *whereSub1;
    SubComboBox *whereSub2;
    SubComboBox *whereSub4;
    SubComboBox *whereSub3;
    QWidget *cityPop2;
    SubComboBox *hometownSub3;
    SubComboBox *hometownSub2;
    SubComboBox *hometownSub4;
    SubComboBox *hometownSub1;
    QComboBox *comboBox;
    QWidget *findGroup_;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label;
    QStackedWidget *stackedWidget;
    QWidget *page1;
    QWidget *page2;
    QFrame *frame3;
    QLabel *arrowForReturnLabel2;
    QLabel *label5;
    QLabel *findGroupInfLabel2;
    QLabel *label6;
    QWidget *page;
    QFrame *frame1;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(846, 614);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(239, 242, 245, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        widget = new QWidget(MainWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(85, 255, 0, 245));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        widget->setPalette(palette1);
        widget->setStyleSheet(QString::fromUtf8(""));
        frame = new QFrame(widget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 826, 594));
        frame->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        bodyWidget = new QWidget(frame);
        bodyWidget->setObjectName(QString::fromUtf8("bodyWidget"));
        bodyWidget->setGeometry(QRect(0, 0, 826, 594));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush3(QColor(255, 255, 255, 245));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        bodyWidget->setPalette(palette2);
        bodyWidget->setStyleSheet(QString::fromUtf8(""));
        mainStackedWidget = new QStackedWidget(bodyWidget);
        mainStackedWidget->setObjectName(QString::fromUtf8("mainStackedWidget"));
        mainStackedWidget->setGeometry(QRect(0, 60, 826, 534));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        mainStackedWidget->setPalette(palette3);
        QFont font;
        font.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        mainStackedWidget->setFont(font);
        mainStackedWidget->setStyleSheet(QString::fromUtf8(""));
        findPerson = new QWidget();
        findPerson->setObjectName(QString::fromUtf8("findPerson"));
        findPersonEdit = new QLineEdit(findPerson);
        findPersonEdit->setObjectName(QString::fromUtf8("findPersonEdit"));
        findPersonEdit->setGeometry(QRect(20, 20, 478, 28));
        QFont font1;
        font1.setPointSize(10);
        findPersonEdit->setFont(font1);
        findPersonEdit->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border:1px solid rgb(0,155,219);\n"
"border-radius:5px\n"
"}\n"
""));
        liveCBox = new QCheckBox(findPerson);
        liveCBox->setObjectName(QString::fromUtf8("liveCBox"));
        liveCBox->setGeometry(QRect(518, 26, 58, 15));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        font2.setPointSize(12);
        liveCBox->setFont(font2);
        liveCBox->setCursor(QCursor(Qt::PointingHandCursor));
        liveCBox->setStyleSheet(QString::fromUtf8("QCheckBox:indicator:unchecked{\n"
"image:url(:/images/liveUnselected.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image:url(:/images/liveSelected.png);\n"
"}"));
        liveCBox->setChecked(false);
        liveCBox->setTristate(false);
        whereCBox = new ComboBox(findPerson);
        whereCBox->setObjectName(QString::fromUtf8("whereCBox"));
        whereCBox->setGeometry(QRect(20, 60, 156, 25));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        font3.setPointSize(10);
        whereCBox->setFont(font3);
        whereCBox->setAutoFillBackground(true);
        whereCBox->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
" background: white;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        whereCBox->setEditable(true);
        whereCBox->setDuplicatesEnabled(false);
        hometownCBox = new ComboBox(findPerson);
        hometownCBox->setObjectName(QString::fromUtf8("hometownCBox"));
        hometownCBox->setGeometry(QRect(184, 60, 158, 25));
        hometownCBox->setFont(font);
        hometownCBox->setAutoFillBackground(true);
        hometownCBox->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        hometownCBox->setEditable(true);
        sexCBox = new ComboBox(findPerson);
        sexCBox->addItem(QString());
        sexCBox->addItem(QString());
        sexCBox->addItem(QString());
        sexCBox->setObjectName(QString::fromUtf8("sexCBox"));
        sexCBox->setGeometry(QRect(350, 60, 70, 25));
        sexCBox->setFont(font);
        sexCBox->setAutoFillBackground(false);
        sexCBox->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding:0 0 0 10;\n"
"  background: white;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin:15;\n"
"      subcontrol-position:right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  } \n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        sexCBox->setEditable(false);
        ageCBox = new ComboBox(findPerson);
        ageCBox->addItem(QString());
        ageCBox->addItem(QString());
        ageCBox->addItem(QString());
        ageCBox->addItem(QString());
        ageCBox->addItem(QString());
        ageCBox->addItem(QString());
        ageCBox->setObjectName(QString::fromUtf8("ageCBox"));
        ageCBox->setGeometry(QRect(428, 60, 70, 25));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Adobe \345\256\213\344\275\223 Std L"));
        font4.setPointSize(9);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setWeight(3);
        ageCBox->setFont(font4);
        ageCBox->setAutoFillBackground(false);
        ageCBox->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding:0  0 0 3;\n"
"  background: white;\n"
"	font: 25 9pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
"QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position:right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }\n"
""));
        ageCBox->setEditable(false);
        findBtn = new QPushButton(findPerson);
        findBtn->setObjectName(QString::fromUtf8("findBtn"));
        findBtn->setGeometry(QRect(596, 35, 117, 38));
        QFont font5;
        font5.setFamily(QString::fromUtf8("AcadEref"));
        font5.setPointSize(14);
        findBtn->setFont(font5);
        findBtn->setCursor(QCursor(Qt::PointingHandCursor));
        findBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color:white;\n"
"  background-color:rgb(0, 155,219);\n"
"border:0px;\n"
"border-radius:3px\n"
"}\n"
"QPushButton:hover:pressed {\n"
"      background-color:rgb(1, 108, 153)\n"
"  }\n"
"QPushButton:hover{\n"
"background-color:rgb(0, 139, 197)\n"
"}\n"
" "));
        findBtn->setFlat(false);
        findPersonStackedWidget = new QStackedWidget(findPerson);
        findPersonStackedWidget->setObjectName(QString::fromUtf8("findPersonStackedWidget"));
        findPersonStackedWidget->setGeometry(QRect(0, 128, 826, 406));
        findPersonStackedWidget->setFont(font);
        findPersonStackedWidget->setStyleSheet(QString::fromUtf8(""));
        initPage = new QWidget();
        initPage->setObjectName(QString::fromUtf8("initPage"));
        backgrounLabel = new QLabel(initPage);
        backgrounLabel->setObjectName(QString::fromUtf8("backgrounLabel"));
        backgrounLabel->setGeometry(QRect(0, 0, 826, 406));
        backgrounLabel->setAutoFillBackground(false);
        backgrounLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:0px;\n"
"background-image:url(:/images/findPersonInit.png);\n"
"}"));
        backgrounLabel->setFrameShadow(QFrame::Plain);
        backgrounLabel->setLineWidth(0);
        backgrounLabel->setWordWrap(false);
        findPersonStackedWidget->addWidget(initPage);
        findPage = new QWidget();
        findPage->setObjectName(QString::fromUtf8("findPage"));
        findPage->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(255, 255, 255);\n"
"}"));
        frame2 = new QFrame(findPage);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setGeometry(QRect(0, 0, 826, 30));
        QPalette palette4;
        QBrush brush4(QColor(233, 237, 238, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        frame2->setPalette(palette4);
        frame2->setStyleSheet(QString::fromUtf8("  QFrame {\n"
" background-color:#e9edee;\n"
"  }"));
        frame2->setFrameShape(QFrame::StyledPanel);
        frame2->setFrameShadow(QFrame::Raised);
        arrowForReturnLabel = new QLabel(frame2);
        arrowForReturnLabel->setObjectName(QString::fromUtf8("arrowForReturnLabel"));
        arrowForReturnLabel->setGeometry(QRect(15, 0, 20, 30));
        arrowForReturnLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"image:url(:/images/arrowForReturn.png)\n"
"}"));
        separaterLabel = new QLabel(frame2);
        separaterLabel->setObjectName(QString::fromUtf8("separaterLabel"));
        separaterLabel->setGeometry(QRect(85, 2, 1, 26));
        separaterLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:1px solid lightgray\n"
"}"));
        findPersonInfLabel = new QLabel(frame2);
        findPersonInfLabel->setObjectName(QString::fromUtf8("findPersonInfLabel"));
        findPersonInfLabel->setGeometry(QRect(97, 0, 700, 30));
        findPersonInfLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: gray;\n"
"}"));
        returnLabel = new Label(frame2);
        returnLabel->setObjectName(QString::fromUtf8("returnLabel"));
        returnLabel->setGeometry(QRect(35, 0, 40, 30));
        QPalette palette5;
        QBrush brush5(QColor(0, 155, 219, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        QBrush brush6(QColor(120, 120, 120, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        returnLabel->setPalette(palette5);
        QFont font6;
        font6.setPointSize(10);
        font6.setBold(true);
        font6.setWeight(75);
        returnLabel->setFont(font6);
        userListStackWindget = new QStackedWidget(findPage);
        userListStackWindget->setObjectName(QString::fromUtf8("userListStackWindget"));
        userListStackWindget->setGeometry(QRect(0, 30, 826, 376));
        userListStackWindget->setStyleSheet(QString::fromUtf8(""));
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        userListStackWindget->addWidget(page_1);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        label_2 = new QLabel(page_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(353, 139, 131, 81));
        QFont font7;
        font7.setPointSize(12);
        label_2->setFont(font7);
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: rgb(0, 0, 255);\n"
"	background-color: transparent;\n"
"}"));
        userListStackWindget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color:transparent;\n"
"	background-image:url(:/images/findPersonPage3.png);\n"
"}"));
        userListStackWindget->addWidget(page_3);
        findPersonStackedWidget->addWidget(findPage);
        sexLabel = new Label(findPerson);
        sexLabel->setObjectName(QString::fromUtf8("sexLabel"));
        sexLabel->setGeometry(QRect(352, 62, 55, 22));
        sexLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:0px;\n"
"border-radius:5px;\n"
"color:gray;\n"
"padding:0 0 0 10;\n"
"	background-color: rgb(255, 255, 255);\n"
"}"));
        sexLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ageLabel = new Label(findPerson);
        ageLabel->setObjectName(QString::fromUtf8("ageLabel"));
        ageLabel->setGeometry(QRect(430, 62, 55, 22));
        ageLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:0px;\n"
"border-radius:3px;\n"
"color:gray;\n"
"padding:0 0 0 10;\n"
"background-color: rgb(255, 255, 255);\n"
"}"));
        ageLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineLabel = new QLabel(findPerson);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setGeometry(QRect(0, 126, 826, 2));
        lineLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:1px solid lightgray\n"
"}"));
        friendBtn = new QPushButton(findPerson);
        friendBtn->setObjectName(QString::fromUtf8("friendBtn"));
        friendBtn->setGeometry(QRect(720, 43, 55, 22));
        QPalette palette6;
        QBrush brush7(QColor(128, 128, 128, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush7);
        QBrush brush8(QColor(128, 128, 128, 128));
        brush8.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Active, QPalette::PlaceholderText, brush8);
#endif
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush7);
        QBrush brush9(QColor(128, 128, 128, 128));
        brush9.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush9);
#endif
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush7);
        QBrush brush10(QColor(128, 128, 128, 128));
        brush10.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush10);
#endif
        friendBtn->setPalette(palette6);
        friendBtn->setCursor(QCursor(Qt::PointingHandCursor));
        friendBtn->setAutoFillBackground(false);
        friendBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color:gray;\n"
"}\n"
"QPushButton:hover:pressed {\n"
"      background-color:rgba(255, 255, 255,0)\n"
"  }\n"
"QPushButton:hover{\n"
"      background-color:rgba(255, 255, 255,0)\n"
"}"));
        friendBtn->setFlat(true);
        townsmanBtn = new QPushButton(findPerson);
        townsmanBtn->setObjectName(QString::fromUtf8("townsmanBtn"));
        townsmanBtn->setGeometry(QRect(775, 43, 55, 22));
        townsmanBtn->setCursor(QCursor(Qt::PointingHandCursor));
        townsmanBtn->setAutoFillBackground(false);
        townsmanBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color:gray;\n"
"}\n"
"QPushButton:hover:pressed {\n"
"      background-color:rgba(255, 255, 255,0)\n"
"  }\n"
"QPushButton:hover{\n"
"      background-color:rgba(255, 255, 255,0)\n"
"}"));
        townsmanBtn->setFlat(true);
        cityPop1 = new QWidget(findPerson);
        cityPop1->setObjectName(QString::fromUtf8("cityPop1"));
        cityPop1->setEnabled(true);
        cityPop1->setGeometry(QRect(30, 101, 324, 68));
        cityPop1->setStyleSheet(QString::fromUtf8("QWidget {\n"
"      border: 1px solid  rgb(0, 199, 219);\n"
"      border-radius: 5px;\n"
"      background:white;\n"
"  }"));
        whereSub1 = new SubComboBox(cityPop1);
        whereSub1->setObjectName(QString::fromUtf8("whereSub1"));
        whereSub1->setGeometry(QRect(8, 5, 150, 28));
        whereSub1->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        whereSub1->setEditable(false);
        whereSub2 = new SubComboBox(cityPop1);
        whereSub2->setObjectName(QString::fromUtf8("whereSub2"));
        whereSub2->setEnabled(true);
        whereSub2->setGeometry(QRect(168, 5, 150, 28));
        whereSub2->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        whereSub4 = new SubComboBox(cityPop1);
        whereSub4->setObjectName(QString::fromUtf8("whereSub4"));
        whereSub4->setEnabled(true);
        whereSub4->setGeometry(QRect(169, 37, 150, 28));
        whereSub4->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        whereSub3 = new SubComboBox(cityPop1);
        whereSub3->setObjectName(QString::fromUtf8("whereSub3"));
        whereSub3->setEnabled(true);
        whereSub3->setGeometry(QRect(9, 37, 150, 28));
        whereSub3->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        cityPop2 = new QWidget(findPerson);
        cityPop2->setObjectName(QString::fromUtf8("cityPop2"));
        cityPop2->setGeometry(QRect(183, 101, 324, 68));
        cityPop2->setStyleSheet(QString::fromUtf8("QWidget {\n"
"      border: 1px solid  rgb(0, 199, 219);\n"
"      border-radius: 5px;\n"
"      background:white;\n"
"  }"));
        hometownSub3 = new SubComboBox(cityPop2);
        hometownSub3->setObjectName(QString::fromUtf8("hometownSub3"));
        hometownSub3->setEnabled(true);
        hometownSub3->setGeometry(QRect(9, 37, 150, 28));
        hometownSub3->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        hometownSub2 = new SubComboBox(cityPop2);
        hometownSub2->setObjectName(QString::fromUtf8("hometownSub2"));
        hometownSub2->setEnabled(true);
        hometownSub2->setGeometry(QRect(168, 5, 150, 28));
        hometownSub2->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        hometownSub4 = new SubComboBox(cityPop2);
        hometownSub4->setObjectName(QString::fromUtf8("hometownSub4"));
        hometownSub4->setEnabled(true);
        hometownSub4->setGeometry(QRect(169, 37, 150, 28));
        hometownSub4->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        hometownSub1 = new SubComboBox(cityPop2);
        hometownSub1->setObjectName(QString::fromUtf8("hometownSub1"));
        hometownSub1->setEnabled(true);
        hometownSub1->setGeometry(QRect(8, 5, 150, 28));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Button, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush);
        hometownSub1->setPalette(palette7);
        hometownSub1->setStyleSheet(QString::fromUtf8(" QComboBox {\n"
"      border:1px solid rgb(0, 155, 219);\n"
"      border-radius: 5px;\n"
"padding: 0 0 0 5;\n"
"  background: white;\n"
"  }\n"
"  QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"      top: 1px;\n"
"      left: 1px;\n"
"  }\n"
"QComboBox::down-arrow {\n"
"image:url(:/images/down-arrow.png)\n"
"}\n"
" QComboBox::drop-down {\n"
"      subcontrol-origin: 15;\n"
"      subcontrol-position: right;\n"
"      width:15px;\n"
"color:red;\n"
"border-bottom-right-radius: 5px;\n"
"  }\n"
" QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"image:url(:/images/up-arrow.png)\n"
"  }"));
        comboBox = new QComboBox(findPerson);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(640, 90, 69, 22));
        comboBox->setEditable(true);
        mainStackedWidget->addWidget(findPerson);
        findPersonEdit->raise();
        liveCBox->raise();
        whereCBox->raise();
        hometownCBox->raise();
        sexCBox->raise();
        ageCBox->raise();
        findBtn->raise();
        findPersonStackedWidget->raise();
        ageLabel->raise();
        lineLabel->raise();
        friendBtn->raise();
        townsmanBtn->raise();
        cityPop1->raise();
        cityPop2->raise();
        comboBox->raise();
        sexLabel->raise();
        findGroup_ = new QWidget();
        findGroup_->setObjectName(QString::fromUtf8("findGroup_"));
        lineEdit = new QLineEdit(findGroup_);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(130, 40, 490, 34));
        lineEdit->setFont(font2);
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border:1px solid rgb(0,155,219);\n"
"border-radius:1px\n"
"}"));
        pushButton = new QPushButton(findGroup_);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(610, 40, 60, 34));
        pushButton->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color:rgb(0, 155,219);\n"
"border:1px solid rgb(0, 155,219);\n"
"border-radius:1px;\n"
"image:url(:/images/searchImg.png)\n"
"}\n"
"QPushButton:hover:pressed {\n"
"      background-color:rgb(1, 108, 153)\n"
"  }\n"
"QPushButton:hover{\n"
"background-color:rgb(0, 139, 197)\n"
"}"));
        label = new QLabel(findGroup_);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 102, 826, 1));
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:1px solid rgb(192, 192, 192)\n"
"}"));
        stackedWidget = new QStackedWidget(findGroup_);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 103, 826, 431));
        stackedWidget->setAutoFillBackground(false);
        page1 = new QWidget();
        page1->setObjectName(QString::fromUtf8("page1"));
        stackedWidget->addWidget(page1);
        page2 = new QWidget();
        page2->setObjectName(QString::fromUtf8("page2"));
        frame3 = new QFrame(page2);
        frame3->setObjectName(QString::fromUtf8("frame3"));
        frame3->setGeometry(QRect(0, 0, 826, 30));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::Button, brush);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush);
        frame3->setPalette(palette8);
        frame3->setAutoFillBackground(false);
        frame3->setStyleSheet(QString::fromUtf8(""));
        frame3->setFrameShape(QFrame::StyledPanel);
        frame3->setFrameShadow(QFrame::Raised);
        arrowForReturnLabel2 = new QLabel(frame3);
        arrowForReturnLabel2->setObjectName(QString::fromUtf8("arrowForReturnLabel2"));
        arrowForReturnLabel2->setGeometry(QRect(15, 0, 20, 30));
        arrowForReturnLabel2->setCursor(QCursor(Qt::PointingHandCursor));
        arrowForReturnLabel2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"image:url(:/images/arrowForReturn.png)\n"
"}"));
        label5 = new QLabel(frame3);
        label5->setObjectName(QString::fromUtf8("label5"));
        label5->setGeometry(QRect(72, 2, 1, 26));
        label5->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border:1px solid lightgray\n"
"}"));
        findGroupInfLabel2 = new QLabel(frame3);
        findGroupInfLabel2->setObjectName(QString::fromUtf8("findGroupInfLabel2"));
        findGroupInfLabel2->setGeometry(QRect(83, 0, 371, 30));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Button, brush);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush);
        findGroupInfLabel2->setPalette(palette9);
        label6 = new QLabel(frame3);
        label6->setObjectName(QString::fromUtf8("label6"));
        label6->setGeometry(QRect(35, 0, 30, 30));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label6->setPalette(palette10);
        label6->setFont(font6);
        label6->setCursor(QCursor(Qt::PointingHandCursor));
        stackedWidget->addWidget(page2);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);
        mainStackedWidget->addWidget(findGroup_);
        frame1 = new QFrame(bodyWidget);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setGeometry(QRect(0, 30, 826, 30));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::Button, brush5);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette11.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette11.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        frame1->setPalette(palette11);
        QFont font8;
        font8.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        font8.setPointSize(14);
        frame1->setFont(font8);
        frame1->setAutoFillBackground(false);
        frame1->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color:#009bdb;\n"
"}"));
        frame1->setFrameShape(QFrame::StyledPanel);
        frame1->setFrameShadow(QFrame::Raised);
        frame1->setLineWidth(0);
        MainWindow->setCentralWidget(widget);

        retranslateUi(MainWindow);

        mainStackedWidget->setCurrentIndex(0);
        sexCBox->setCurrentIndex(-1);
        ageCBox->setCurrentIndex(-1);
        findBtn->setDefault(true);
        findPersonStackedWidget->setCurrentIndex(0);
        userListStackWindget->setCurrentIndex(0);
        whereSub1->setCurrentIndex(-1);
        hometownSub4->setCurrentIndex(-1);
        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        findPersonEdit->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245MyQQ\345\217\267\347\240\201/\346\230\265\347\247\260/\345\205\263\351\224\256\350\257\215", nullptr));
        liveCBox->setText(QApplication::translate("MainWindow", "\345\234\250 \347\272\277", nullptr));
        whereCBox->setCurrentText(QApplication::translate("MainWindow", "\346\211\200\345\234\250\345\234\260\357\274\232\344\270\255\345\233\275", nullptr));
        hometownCBox->setCurrentText(QApplication::translate("MainWindow", "\346\225\205\344\271\241\357\274\232\344\270\255\345\233\275", nullptr));
        sexCBox->setItemText(0, QApplication::translate("MainWindow", "\344\270\215\351\231\220", nullptr));
        sexCBox->setItemText(1, QApplication::translate("MainWindow", "\347\224\267", nullptr));
        sexCBox->setItemText(2, QApplication::translate("MainWindow", "\345\245\263", nullptr));

        sexCBox->setCurrentText(QString());
        ageCBox->setItemText(0, QApplication::translate("MainWindow", "\344\270\215\351\231\220", nullptr));
        ageCBox->setItemText(1, QApplication::translate("MainWindow", "18\345\262\201\344\273\245\344\270\213", nullptr));
        ageCBox->setItemText(2, QApplication::translate("MainWindow", "18~22\345\262\201", nullptr));
        ageCBox->setItemText(3, QApplication::translate("MainWindow", "23~26\345\262\201", nullptr));
        ageCBox->setItemText(4, QApplication::translate("MainWindow", "27~35\345\262\201", nullptr));
        ageCBox->setItemText(5, QApplication::translate("MainWindow", "35\345\262\201\344\273\245\344\270\212", nullptr));

        ageCBox->setCurrentText(QString());
        findBtn->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276", nullptr));
        backgrounLabel->setText(QString());
        arrowForReturnLabel->setText(QString());
        separaterLabel->setText(QString());
        findPersonInfLabel->setText(QString());
        returnLabel->setText(QApplication::translate("MainWindow", " \350\277\224\345\233\236", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275\344\270\255\357\274\214\350\257\267\347\250\215\345\200\231...", nullptr));
        sexLabel->setText(QApplication::translate("MainWindow", "\346\200\247\345\210\253", nullptr));
        ageLabel->setText(QApplication::translate("MainWindow", "\345\271\264\351\276\204", nullptr));
        lineLabel->setText(QString());
        friendBtn->setText(QApplication::translate("MainWindow", "\345\220\214\345\237\216\344\272\244\345\217\213", nullptr));
        townsmanBtn->setText(QApplication::translate("MainWindow", "\345\220\214\345\237\216\350\200\201\344\271\241", nullptr));
        whereSub1->setCurrentText(QString());
        lineEdit->setPlaceholderText(QApplication::translate("MainWindow", "\346\211\276\347\276\244", nullptr));
        pushButton->setText(QString());
        label->setText(QString());
        arrowForReturnLabel2->setText(QString());
        label5->setText(QString());
        findGroupInfLabel2->setText(QString());
        label6->setText(QApplication::translate("MainWindow", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
