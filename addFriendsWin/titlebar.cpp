#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "titlebar.h"
#include<QMainWindow>
#include<qlayout.h>
#include<qbitmap.h>
#include<qlabel.h>
#include<qdebug.h>
#include<QPushButton>
#include<qapplication.h>
titleBar::titleBar(QWidget *parent) : QWidget(parent),dragPoint(QPoint()),isInsidePressed(false)
{
resize(826,30);
setAutoFillBackground(true);
QPalette p=this->palette();
p.setColor(QPalette::Window,QColor(0,155,219));
this->setPalette(p);
//标题栏左边布局
QLabel*winIconLabel=new QLabel(this);
winIconLabel->resize(20,30);
winIconLabel->setStyleSheet("QLabel{\
                            image:url(:/images/winIco.png);\
                            background-color:transparent;\
}");
QLabel* winTitle=new QLabel(this);
winTitle->resize(40,30);
QPalette winTitlePalette=winTitle->palette();
winTitle->setStyleSheet("QLabel{\
                        color:rgb(255,255,255);\
                        background-color:transparent;\
}");
winTitle->setPalette(winTitlePalette);
winTitle->setAutoFillBackground(true);
winTitle->setText(("查找"));
winIconLabel->move(5,0);
winTitle->move(15+winIconLabel->width(),0);
//标题栏右边布局
minBtn=new QPushButton(this);
minBtn->resize(30,30);
minBtn->setStyleSheet("QPushButton{border-image:url(:/images/minimum.png);}"
                      "QPushButton:hover{background-color:rgb(38,169,224);}");
minBtn->move(766,0);
minBtn->setToolTip(("最小化"));

closeBtn=new QPushButton(this);
closeBtn->resize(30,30);
closeBtn->setStyleSheet("QPushButton{border-image:url(:/images/close.png);}"
                        "QPushButton:hover{background-color:rgb(212,64,39);}");
closeBtn->move(796,0);
closeBtn->setToolTip(("关 闭"));
connect(minBtn,SIGNAL(clicked()),this,SLOT(forwardSiganlForMinBtn()));
connect(closeBtn,SIGNAL(clicked()),this,SLOT(forwardSiganlForCloseBtn()));
}

void titleBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
           QMainWindow*rootWin=nullptr;
           for (QWidget*w : qApp->topLevelWidgets()) {
               if(w->objectName()=="MainWindow")
                   rootWin=(QMainWindow*)w;
           }
           if(rootWin){
        dragPoint=event->globalPos()-QPoint(rootWin->x(),rootWin->y());
        isInsidePressed=true;
           }
    }
  event->accept();
}

void titleBar::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton)&&isInsidePressed){
        QMainWindow*rootWin=nullptr;
        for (QWidget*w : qApp->topLevelWidgets()) {
            if(w->objectName()=="MainWindow")
                rootWin=(QMainWindow*)w;
        }
        if(rootWin){
        rootWin->move(event->globalPos()-dragPoint);
        }
    }
 event->accept();
}

void titleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
   isInsidePressed=false;
   event->accept();
}

void titleBar::forwardSiganlForMinBtn()
{
    emit minBtnClicked();
}

void titleBar::forwardSiganlForCloseBtn()
{
    emit closeBtnClicked();
}
