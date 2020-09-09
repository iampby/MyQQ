#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "addfriendgroupwidget.h"
#include <qgraphicseffect.h>
#include<QFrame>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include<qdebug.h>
AddFriendGroupWidget::AddFriendGroupWidget(QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    isPressed=false;
    dragPoint=QPoint(0,0);
    this->setAttribute(Qt::WA_QuitOnClose,false);//关闭不退出
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(336,161);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置透明背景
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(200,200,200));
    //设置阴影圆角
    shadow->setBlurRadius(5);
    this ->setGraphicsEffect(shadow);
    QWidget*contWidget=new QWidget(this);
    contWidget->resize(326,151);
    contWidget->move(5,5);
    contWidget->setStyleSheet("QWidget{background-color:white;}");
    //子头
    QFrame* frameHeader=new QFrame(contWidget);
    frameHeader->resize(326,30);
    frameHeader->move(0,0);
    frameHeader->setStyleSheet("QFrame{\
                               image: url(:/images//mainInterface/newGroupTitle.png);\
} \
");
QPushButton* closeChildBtn=new QPushButton(frameHeader);
closeChildBtn->resize(28,30);
closeChildBtn->move(frameHeader->width()-28,0);
closeChildBtn->setStyleSheet("QPushButton{\
                             border:0;\
background-color: rgba(255, 255, 255, 0);\
}\
QPushButton:pressed {\
    border-image: url(:/images//mainInterface/addFriendClosePressed.png);\
}\
QPushButton:hover:!pressed {\
                       border-image: url(:/images//mainInterface/addFriendsCloseHovered.png);\
}");
QLabel*label=new QLabel(this);
label->resize(60,20);
label->move(30,50);
label->setText(("添加分组："));
QLineEdit*addLEdit=new QLineEdit(this);
addLEdit->resize(278,23);
addLEdit->move(30,70);
addLEdit->setProperty("lastLength",QVariant(0));
addLEdit->setStyleSheet("QLineEdit{\
                        border:1px solid lightgray;\
border-radius:5px;\
}\
QLineEdit:hover{\
border:1px solid rgb(0, 155, 219);\
    border-radius:5px;\
}");
//底部控件
QFrame* frameFooter=new QFrame(contWidget);
frameFooter->resize(326,35);
frameFooter->move(0,contWidget->height()-35);
frameFooter->setStyleSheet("QFrame{\
                           border:0px\
                           background-color:rab(229,239,247);\
} \
");
QPushButton* childBottomBtn1=new QPushButton(frameFooter);
QPushButton* childBottomBtn2=new QPushButton(frameFooter);
childBottomBtn1->resize(69,24);
childBottomBtn2->resize(69,24);
childBottomBtn1->setStyleSheet("QPushButton{\
                               border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
childBottomBtn2->setStyleSheet("QPushButton{\
                               border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
childBottomBtn1->move(frameFooter->width()-154,frameFooter->height()-28);
childBottomBtn2->move(frameFooter->width()-77,frameFooter->height()-28);
childBottomBtn1->setText(("确定"));
childBottomBtn2->setText(("取消"));
childBottomBtn1->setEnabled(false);
//关闭窗口
connect(closeChildBtn,&QPushButton::clicked,contWidget,[=]()mutable{
    this->deleteLater();
    emit setNull();//置零
});
connect(childBottomBtn2,&QPushButton::clicked,contWidget,[=](){
    closeChildBtn->click();
});
//添加新分组
connect(childBottomBtn1,&QPushButton::clicked,contWidget,[=](){
    emit emitGroup( addLEdit->text());
});
connect(addLEdit,&QLineEdit::textChanged,this,[=](){
    QString str=addLEdit->text();
    qint32 lastLength=addLEdit->property("lastLength").toInt();
    qint32 size=str.toUtf8().size();
    if(size>24){
        str=str.left(lastLength);
        addLEdit->setText(str);
    }
    lastLength=str.length();
    addLEdit->setProperty("lastLength",QVariant(lastLength));
    childBottomBtn1->setEnabled(true);
});

}

AddFriendGroupWidget::~AddFriendGroupWidget()
{
    qDebug()<<"~AddFriendGroupWidget()";

}

void AddFriendGroupWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){

        dragPoint=event->globalPos()-QPoint(this->x(),this->y());
        isPressed=true;
    }
    event->accept();
}

void AddFriendGroupWidget::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton)&&isPressed){
        this->move(event->globalPos()-dragPoint);
    }
    event->accept();
}

void AddFriendGroupWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        isPressed=false;
    event->accept();
}
