#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "headimgview.h"
#include<QGraphicsScene>
#include <QWheelEvent>
#include<qdebug.h>
#include<qmatrix.h>
#include<qpaintengine.h>
#include<qslider.h>

HeadImgView::HeadImgView(QWidget *parent,const bool&b):QGraphicsView(parent),hasMask(b)
{
    setFixedSize(350,350);
    control=true;
    direct=Top;//��ʼ��Ϊ����
    if(hasMask)
    setStyleSheet("QFrame{border-width:1px solid rgba(155,155,155);}");//���������򻭱���
    else  setFrameShape(QFrame::NoFrame);//�ر�fram����
    QGraphicsScene*scene=new QGraphicsScene(this);
    setScene(scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setCacheMode(CacheBackground);
    //ȫ�����£�������������
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
    //���쵽���δ�С��������С�Ŀ�����չ��������ͬʱ�����ݺ��
    imgBackup=image;
    QMatrix m;
    qreal s=0;
    qint32 width=imgBackup.width(),height=imgBackup.height();
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
    rangPos=QPoint(-(img.width()-348.0)/2,-(img.height()-348.0)/2);
    scene()->setSceneRect( rangPos.x(), rangPos.y(),0,0);//ˢ��
    scene()->setSceneRect( rangPos.x(), rangPos.y(),img.width(),img.height());//���Ķ���
    qDebug()<<"start draw"<<scene()->sceneRect().topLeft()<<img.size();
    switch (direct) {
    case Left:
        rotate(90);
        direct=Top;
        break;
    case Bottom:
        rotate(180);
        direct=Top;
        break;
    case Right:
        rotate(-90);
        direct=Top;
        break;
    default:
        break;
    }
    viewport()->update();//ֻ�е����Ӿ�ڸ��²��ܸ���scene �����������ñ�����ʱ���ử����
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
    //�ػ���������
    painter->drawPixmap(sceneRect().left(),sceneRect().top(),img);//�ڳ��������λ�ÿ�ʼ��ͼƬ
   //������
    if(hasMask){
        QRegion r1(rect.x(),rect.y(),350,350,QRegion::Ellipse);
        QRegion r2(rect.x(),rect.y(),350,350);
        QRegion r= r2.subtracted(r1);
        QPainterPath path;
        path.addRegion(r);
        painter->setPen( QColor(177,177,177,0));
        painter->setBrush(QColor(177,177,177,155));
        painter->drawPath(path);
    }
}

void HeadImgView::wheelEvent(QWheelEvent *event)
{
    emit getFocus();
    if(!slider->isEnabled())return;
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        slider->setValue(slider->value()+numPixels.y());
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        slider->setValue(slider->value()+numSteps.y()*slider->tickInterval());
    }
    event->accept();//������ͼ����
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
        //���賡������Ȼ������ʾ
        QPoint temp=(posPreScene+offset);
        if(temp.x()<rangPos.x()*2)temp.rx()=rangPos.x()*2;
        else if(temp.x()>0)temp.rx()=0;
        if(temp.y()<rangPos.y()*2)temp.ry()=rangPos.y()*2;
        else if(temp.y()>0)temp.ry()=0;
        scene()->setSceneRect(QRectF((temp),QSizeF(img.width(),img.height())));
        qDebug()<<"move"<<sceneRect().topLeft()<<mapToScene(viewport()->pos());
        viewport()->update();
    }
    event->accept();
}
void HeadImgView::mousePressEvent(QMouseEvent *event)
{
    emit getFocus();
    if(event->button()==Qt::LeftButton){
        posBegin=event->pos();
        posPreScene=sceneRect().topLeft().toPoint();
        qDebug()<<"position of starting to press where equal to "<<posBegin;
    }
}

const QPixmap HeadImgView::getGrabPixmap()
{
    QPixmap newPix;
    QPoint newPos=sceneRect().topLeft().toPoint();
    QPoint movePos=rangPos-newPos;
    QPoint pos= movePos-rangPos;//�ƶ�-���=��ͼƬ���

    qDebug()<<"current image is location "<<pos<<" in old image";
    newPix=img.copy(QRect(pos,QSize(348,348)));
    if(newPix.isNull()){qDebug()<<"newpix is null";}
    return newPix;
}
//����ϵ��=ʣ��ϵ��*����ĵ�ǰ�ٷֱ�ֵ+��Сϵ��
void HeadImgView::valueChanged(int value)
{
    qDebug()<<"value:"<<value;
    qint16 length=(imgBackup.width()>imgBackup.height()?imgBackup.height():imgBackup.width());

    qreal scaleBegin=348.0/length;
    qreal scaleReamin=1-scaleBegin;
    qreal scale=scaleBegin+scaleReamin*(value-5)/290;
    QMatrix m;
    m.scale(scale,scale);
    qint32 prew=img.width();
    qint32 preh=img.height();
    img=imgBackup.transformed(m);
    qint32 noww=img.width();
    qint32 nowh=img.height();
    qDebug()<<"value changed"<<scale;
    rangPos=QPoint(-(noww-348.0)/2,-(nowh-348.0)/2);
    scene()->setSceneRect(sceneRect().x()-(noww-prew)/2.0,sceneRect().y()-(nowh-preh)/2.0,img.width(),img.height());
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
