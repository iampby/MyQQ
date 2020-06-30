#include "headimgview.h"
#include<QGraphicsScene>
#include <QWheelEvent>
#include<qdebug.h>
#include<qmatrix.h>
#include<qpaintengine.h>
#include<qslider.h>

HeadImgView::HeadImgView(QWidget *parent):QGraphicsView(parent)
{
    setFixedSize(350,350);
    control=true;
    direct=Top;//初始化为向上
    //setFrameShape(QFrame::NoFrame);
    setStyleSheet("QFrame{border-width:1px solid rgba(155,155,155);}");
    QGraphicsScene*scene=new QGraphicsScene(this);
    setScene(scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCacheMode(CacheBackground);
    //全部更新，消除背景残留
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // setWindowState(Qt::WindowMaximized);
}

void HeadImgView::setImage(QPixmap &image)
{
    if(image.isNull()){
        qDebug()<<img.width()<<img.height()<<"image is null";
        return;
    }
    img=image;
    //拉伸到矩形大小，尽可能小的可以扩展到矩形外同时保持纵横比
    imgBackup=image;
    QMatrix m;
    qreal s=0;
    qint16 width=imgBackup.width(),height=imgBackup.height();
    if(width>height){
        s=348.0/height;
    }else{
        s=348.0/width;
    }
    if(width<=348||height<=348)
        slider->setEnabled(false);
    else{
        slider->setValue(5);
        slider->setEnabled(true);
    }
    m.scale(s,s);
    img= img.transformed(m,Qt::SmoothTransformation);
    scene()->setSceneRect(-(img.width()-348.0)/2,-(img.height()-348.0)/2,img.width(),img.height());//中心对齐
    qDebug()<<"start draw"<<viewport()->pos();;
    viewport()->update();//只有调用视距口更新才能更新scene 这里主动调用避免有时不会画背景
    // drawBackground( this->paintEngine()->painter(),QRectF(mapToScene(0,0),QSizeF(348.0,348.0)));
}

void HeadImgView::setSlider(QSlider *s)
{
    if(s)
        slider=s;
    connect(slider,&QSlider::valueChanged,[=](int value){

        if(value<=5){
            control=false;
            slider->setValue(5);
        }
        else if(value>=295){
            control=false;
            slider->setValue(295);
        }else control=true;
        if(control)
            valueChanged(slider->value());
    });
}

void HeadImgView::drawBackground(QPainter *painter, const QRectF &rect)
{
    //重画场景背景
    qDebug()<<"rect:"<<rect<<sceneRect().topLeft()<<viewport()->rect();
    QRegion r1(rect.x(),rect.y(),350,350,QRegion::Ellipse);
    QRegion r2(rect.x(),rect.y(),350,350);
    QRegion r= r2.subtracted(r1);
    QPainterPath path;
    path.addRegion(r);
    painter->drawPixmap(sceneRect().left(),sceneRect().top(),img);//在场景的起点位置开始画图片
    painter->setPen( QColor(177,177,177,0));
    painter->setBrush(QColor(177,177,177,155));
    painter->drawPath(path);
}

void HeadImgView::wheelEvent(QWheelEvent *event)
{
    if(!slider->isEnabled())return;
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        slider->setValue(slider->value()+numPixels.y());
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        slider->setValue(slider->value()+numSteps.y()*slider->tickInterval());
    }
    event->accept();//不让视图滚动
}

void HeadImgView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton){
        QPoint offset=event->pos()-posBegin;
        switch (direct) {
        case Top:
            break;
        case Bottom:
            offset =-offset;
            break;
        case Left:
            offset =QPoint(-offset.y(),-offset.x());
            break;
        case Right:
            offset =QPoint(offset.y(),offset.x());
            break;
        default:
            qDebug()<<"excepted a error";
            return;
        }
        //重设场景区域，然后重显示
        scene()->setSceneRect(QRectF((posPreScene+offset),QSizeF(img.width(),img.height())));
        qDebug()<<"move"<<sceneRect().topLeft();
        viewport()->update();
    }
    event->accept();
}
void HeadImgView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        posBegin=event->pos();
        posPreScene=sceneRect().topLeft().toPoint();
        qDebug()<<"position of starting to press where equal to "<<posBegin;
    }
}
//拉伸系数=剩余系数*滑块的当前百分比值+最小系数
void HeadImgView::valueChanged(int value)
{
    qDebug()<<"value:"<<value;
    qint16 length=(imgBackup.width()>imgBackup.height()?imgBackup.height():imgBackup.width());

    qreal scaleBegin=348.0/length;
    qreal scaleReamin=1-scaleBegin;
    qreal scale=scaleBegin+scaleReamin*(value-5)/290;
    QMatrix m;
    m.scale(scale,scale);
    img=imgBackup.transformed(m);
    qDebug()<<"value changed"<<scale;
    scene()->setSceneRect(-img.width()/2,-img.height()/2,img.width(),img.height());
    viewport()->update();
}

void HeadImgView::zoomOutClicked()
{
    qDebug()<<"zoomout clicked";
    if(!slider->isEnabled())return;
    slider->setValue(slider->value()-slider->tickInterval());
}

void HeadImgView::zoomInClicked()
{
    if(!slider->isEnabled())return;
    qDebug()<<"zoomit clicked";
    slider->setValue(slider->value()+slider->tickInterval());
}

void HeadImgView::cwClicked()
{
    rotate(90);
    switch (direct) {
    case Top:
        direct=Right;
        break;
    case Bottom:
       direct=Left;
        break;
    case Left:
        direct=Top;
        break;
    case Right:
        direct=Bottom;
        break;
    default:
        qDebug()<<"excepted a error";
        return;
    }
}

void HeadImgView::acwClicked()
{
    rotate(-90);
    switch (direct) {
    case Top:
        direct=Left;
        break;
    case Bottom:
       direct=Right;
        break;
    case Left:
        direct=Bottom;
        break;
    case Right:
          direct=Top;
        break;
    default:
        qDebug()<<"excepted a error";
        return;
    }
}
