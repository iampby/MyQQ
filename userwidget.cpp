#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "userwidget.h"
#include"label.h"
#include <qbitmap.h>
#include <qpainter.h>
#include<qpushbutton.h>
#include <qstyleoption.h>
#include<qdebug.h>
#include <QEvent>

UserWidget::UserWidget(QWidget *parent) : QWidget(parent)
{
    resize(190,80);
    primitivePixmap=nullptr;
    m_index=QModelIndex();
    pixLabel=new Label(this);
    pixLabel->resize(60,60);
    pixLabel->move(0,10);
    pixLabel->setStyleSheet(QString("QLabel{\
                                    border:0px;\
                            border-radius:%1px;\
}\
").arg(pixLabel->width()/2));
nameBtn=new QPushButton(this);
nameBtn->resize(116,20);
nameBtn->move(74,10);
nameBtn->setStyleSheet("QPushButton{\
                       color:black;\
background-color: transparent;\
border:0px;\
font: 10pt \"Adobe 仿宋 Std L\";\
text-align:left;\
}\
QPushButton:hover{\
color:rgb(62, 156, 234);\
}");
ageAndCountryLabel=new QLabel(this);
ageAndCountryLabel->resize(116,20);
ageAndCountryLabel->move(74,30);
ageAndCountryLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
ageAndCountryLabel->setStyleSheet("QLabel{\
                                  border:0px;\
color:gray;\
background-color:transparent;\
font: 10pt \"Adobe 仿宋 Std R\";\
}");
addBtn=new QPushButton(this);
addBtn->resize(52,20);
addBtn->move(74,50);
addBtn->setText(("＋好友"));
addBtn->setStyleSheet("QPushButton{\
                      color:white;\
background-color: #8fc0e7;\
border-radius:3px;\
font: 10pt \"Adobe 仿宋 Std R\";\
}\
QPushButton:hover{\
    background-color:rgb(62, 156, 234);\
}");
pixLabel->installEventFilter(this);
connect(pixLabel,&Label::clicked,this,[=](){ emit imgClicked();});//点击信号
connect(nameBtn,&QPushButton::clicked,this,[=](){emit nameClicked();});
connect(addBtn,&QPushButton::clicked,this,[=](){emit addButtonClicked();});
}

UserWidget::~UserWidget()
{
    qDebug()<<"~UserWidget()";
    if(primitivePixmap){
        delete primitivePixmap,primitivePixmap=nullptr;//释放内存
    }
}


void UserWidget::setPixmap(const QPixmap &img)
{
    pixLabel->setPixmap(img);
}

void UserWidget::setPrimitivePixmap(QPixmap *img)
{
    *img=img->scaled(pixLabel->size()-QSize(2,2));
    img=pixmapToRound(img);
    primitivePixmap=img;
    setPixmap(*primitivePixmap);
}

void UserWidget::setName(const QString &name)
{
    QFontMetrics f=nameBtn->fontMetrics();
    QString str=name;
    if(f.boundingRect(name).width()>=nameBtn->width()){
        str=f.elidedText(name,Qt::ElideRight,nameBtn->width()-f.boundingRect("...").width());
        str.replace(str.length()-3,3,"...");
    }
    nameBtn->setText(str);
}

void UserWidget::setAgeAndCountry(const QString&ageAndCountry)
{
    QFontMetrics f=ageAndCountryLabel->fontMetrics();
    QString str=ageAndCountry;
    str=f.elidedText(ageAndCountry,Qt::ElideRight,nameBtn->width()-f.boundingRect("...").width());
    ageAndCountryLabel->setText(str);
}

void UserWidget::setAgeAndCountry( QPixmap &img)
{
    QPixmap mask(img.size());
    QPainter p;
    p.begin(&mask);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    p.fillRect(0,0,img.width(),img.height(),QColor(255,255,255));
    p.setBrush(QColor(0,0,0));
    p.drawRoundedRect(0,0,img.width(),img.height(),img.width()/2,img.height()/2);
    img.setMask(mask);
   p.end();
    ageAndCountryLabel->setPixmap(img);
}

const QModelIndex UserWidget::index() const
{
    return m_index;
}

void UserWidget::setIndex(const QModelIndex &index)
{
    m_index=index;
}

QPixmap *UserWidget::pixmapToRound(QPixmap*img) const
{
    QBitmap mask(QSize( img->width(),img->height()));
    QPainter painter;
    painter.begin(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(QRect(0, 0, img->width(), img->height()), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(QRect(0, 0, img->width(), img->height()), 100, 100,Qt::RelativeSize);
   img->setMask(mask);
   painter.end();
    return img;
}

QPixmap &UserWidget::drawRoundOnPixmap(QPixmap &img) const
{
    QPainter p;
    p.begin(&img);
    QPen pen=p.pen();
    p.setPen(QPen(QColor(62, 156, 234),2));
    p.drawRoundedRect(QRect(0,0,img.width(),img.height()),100,100,Qt::RelativeSize);
    p.setPen(pen);
    p.end();
    return img;
}

bool UserWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==pixLabel){
        if(event->type()==QEvent::Enter){
            QPixmap img;//先声明后赋值，即不使用构造函数
            img=*primitivePixmap;
            pixLabel->setPixmap(drawRoundOnPixmap(img));
        }else if(event->type()==QEvent::Leave){
            setPixmap(*primitivePixmap);
        }
    }
    return QWidget::eventFilter(watched,event);
}



