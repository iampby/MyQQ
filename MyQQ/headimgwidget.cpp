#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "headimgwidget.h"
#include"images.h"
#include <QEvent>
#include<QHBoxLayout>
#include <QSlider>
#include<qdebug.h>
#include <qpushbutton.h>
#include<QApplication>
#include<qlabel.h>
#include<qdir.h>
//#pragma comment(lib,"qtquickcontrols2plugind.dll")//����qml�����

//qpoint�ľ�̬qhash���ط��� ����qhash
static uint qHash(const QPoint &key, uint seed)
{
    return pow(qHash(key.x(),seed),2)*qHash(key.y(),seed);//ע���ֹ��ͻ���������ͼƬ��ɫ�����룬��һ��ʱ��ϣ�������ͻ����
}


HeadImgWidget::HeadImgWidget(QWidget *parent, const bool &b) : QWidget(parent)
{
    qDebug()<<"init headimgwidget";
    //��ʼ��ͼƬ
    imgOut.load(":/images/QWidget/headImgSubtraction.png","png");
    imgIn.load(":/images/QWidget/headImgAddition.png","png");
    imgCw.load(":/images/QWidget/headImgCw.png","png");
    imgAcw.load(":/images/QWidget/headImgAcw.png","png");
    //��ʼ���Ŵ�ͼƬ��Ϣ�����ڸı�ͼƬ��ɫ
    for (int i = 0; i < imgOut.height(); ++i) {
        QRgb*line=(QRgb*)imgOut.scanLine(i);

        for (int j = 0; j < imgOut.width(); ++j) {
            QPoint pos(j,i);
            QRgb rgb=line[j];
            if(qAlpha(rgb)!=0){
                rgbOut[0].insert(pos,rgb);//ԭɫ
                rgbOut[1].insert(pos,qRgba(97,207,248,255));//hoverˮ��ɫ
            }else{
                rgbOut[0].insert(pos,rgb);//ԭɫ͸��
                rgbOut[1].insert(pos,rgb);//hover͸��
            }
        }
    }

    for (int i = 0; i < imgIn.height(); ++i) {
        QRgb*line=(QRgb*)imgIn.scanLine(i);
        for (int j = 0; j < imgIn.width(); ++j) {
            QPoint pos(j,i);
            QRgb rgb=line[j];
            if(qAlpha(rgb)!=0){
                rgbIn[0].insert(pos,rgb);//ԭɫ
                rgbIn[1].insert(pos,qRgba(97,207,248,255));//hoverˮ��ɫ
            }else{
                rgbIn[0].insert(pos,rgb);//ԭɫ͸��
                rgbIn[1].insert(pos,rgb);//hover͸��
            }
        }
    }
    for (int i = 0; i < imgCw.height(); ++i) {
        QRgb*line=(QRgb*)imgCw.scanLine(i);
        for (int j = 0; j < imgCw.width(); ++j) {
            QPoint pos(j,i);
            QRgb rgb=line[j];
            if(qAlpha(rgb)!=0){
                rgbCw[0].insert(pos,rgb);//ԭɫ
                rgbCw[1].insert(pos,qRgba(142,146,161,255));//hover��ɫ
                rgbCw[2].insert(pos,qRgba(97,207,248,255));//pressˮ��ɫ
            }else{
                rgbCw[0].insert(pos,rgb);//ԭɫ͸��
                rgbCw[1].insert(pos,rgb);//hover͸��
                rgbCw[2].insert(pos,rgb);//press͸��
            }
        }
    }
    for (int i = 0; i < imgAcw.height(); ++i) {
        QRgb*line=(QRgb*)imgAcw.scanLine(i);
        for (int j = 0; j < imgAcw.width(); ++j) {
            QPoint pos(j,i);
            QRgb rgb=line[j];
            if(qAlpha(rgb)!=0){
                rgbAcw[0].insert(pos,rgb);//ԭɫ
                rgbAcw[1].insert(pos,qRgba(142,146,161,255));//hover��ɫ
                rgbAcw[2].insert(pos,qRgba(97,207,248,255));//pressˮ��ɫ
            }else{
                rgbAcw[0].insert(pos,rgb);//ԭɫ͸��
                rgbAcw[1].insert(pos,rgb);//hover͸��
                rgbAcw[2].insert(pos,rgb);//press͸��
            }
        }
    }
    QPalette p=this->palette();
    this->setAutoFillBackground(true);
    p.setColor(QPalette::Window,QColor::fromRgba(qRgb(255,255,255)));
    this->setPalette(p);
    setFixedSize(350,390);
    //��ͼ
    view=new HeadImgView(this,b);
qDebug()<<"Opened mask or not?"<<b;
    zoomout=new QPushButton(this);
    zoomout->setFixedSize(18,16);
    zoomout->setFlat(true);
    zoomout->setIcon(QIcon(QPixmap::fromImage(imgOut)));
    //������palette����,���
    zoomout->move(0,362);
    QString str="QPushButton{\
            background-color:rgba(255, 255,255);;\
    border.width:0px;\
border: none; /* no border */\
}\
";
zoomout->setStyleSheet(str);


slider=new QSlider(this);
slider->setOrientation(Qt::Horizontal);
slider->resize(230,10);
slider->setRange(1,300);
slider->setTickInterval(10);
slider->setValue(5);
slider->setStyleSheet("QSlider::groove:horizontal {\
                      background: lightgray;\
height:4;\
margin: 2px 0;\
}\
QSlider::handle:horizontal {\
                    background: rgba(97,207,248,255);\
width:10;\
margin: -3px -3px;\
border-radius: 5px;\
}\
");
slider->move( zoomout->x()+ zoomout->width()+6, zoomout->y()+zoomout->height()/2-4);

zoomin=new QPushButton(this);
zoomin->setFixedSize(18,16);
zoomin->setIcon(QIcon(QPixmap::fromImage(imgIn)));
zoomin->move(slider->x()+slider->width()+6,zoomout->y());
zoomin->setStyleSheet(str);
cw=new QPushButton(this);
cw->setFixedSize(16,16);
cw->setIcon(QIcon(QPixmap::fromImage(imgCw)));
cw->move(350-cw->width(),zoomin->y());
cw->setStyleSheet(str);
acw=new QPushButton(this);
acw->setFixedSize(16,16);
acw->setIcon(QIcon(QPixmap::fromImage(imgAcw)));
acw->move(cw->x()-10-acw->width(),cw->y());
acw->setStyleSheet(str);
//���������¼���widget������hover clicked
zoomin->installEventFilter(this);
zoomout->installEventFilter(this);
cw->installEventFilter(this);
acw->installEventFilter(this);


view->setSlider(slider);
slider->setEnabled(false);
connect(zoomout,&QPushButton::clicked,[=](){emit getFocus();});//���͵�qml�ر���ʷͷ���ǩ
connect(zoomout,&QPushButton::clicked,view,&HeadImgView::zoomOutClicked);
connect(zoomin,&QPushButton::clicked,[=](){emit getFocus();});//���͵�qml�ر���ʷͷ���ǩ
connect(zoomin,&QPushButton::clicked,view,&HeadImgView::zoomInClicked);
connect(cw,&QPushButton::clicked,[=](){emit getFocus();});//���͵�qml�ر���ʷͷ���ǩ
connect(cw,&QPushButton::clicked,view,&HeadImgView::cwClicked);
connect(acw,&QPushButton::clicked,[=](){emit getFocus();});//���͵�qml�ر���ʷͷ���ǩ
connect(acw,&QPushButton::clicked,view,&HeadImgView::acwClicked);
connect(slider,&QSlider::sliderPressed,[=](){emit getFocus();});//���͵�qml�ر���ʷͷ���ǩ
connect(view,&HeadImgView::getFocus,this,[=](){emit getFocus();});//
}




HeadImgWidget::~HeadImgWidget()
{
    qDebug()<<"delete view";
}


void HeadImgWidget::setHeadImg( QPixmap &head)
{
    qDebug()<<"image of head is loaded";
    view->setImage(head);
}



void HeadImgWidget::openFile(const QString &filename)
{
    qDebug()<<"opened a file,named: "<<filename;
    QPixmap img;
    img.load(filename);
    if(img.isNull()){
        qDebug()<<"warning: loaded image of QWidget is null";
    }
    view->setImage(img);
}

void HeadImgWidget::okClicked(Images*images,const QString&myqq)
{
    QPixmap newHeadImg= view->getGrabPixmap();
    qDebug()<<"new image has been  got";
    emit updateRemoteHeadImg(newHeadImg);
    images->insert(newHeadImg);

    QString id=myqq+"1";
    if(images->provider2->images.contains(id)){
       images->provider2->images[id]=newHeadImg ;
        qDebug()<<"images->provider2->images[id]=newHeadImg";
        updateMyself(myqq);
    }
}

void HeadImgWidget::okCoverClicked(const QString &myqq)
{
    QPixmap newHeadImg= view->getGrabPixmap();
    qDebug()<<"new cover-image has been  got";
    QDir dir("../user/"+myqq);
    if(!dir.exists())dir.mkpath("./");
    newHeadImg.save(dir.absoluteFilePath("cover"),"png");
}




bool HeadImgWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==zoomout){
        if(event->type()==QEvent::HoverEnter){
            transColor(imgOut,rgbOut[1]);//����ˮ��ɫ
            zoomout->setIcon(QIcon(QPixmap::fromImage(imgOut)));
        }else if(event->type()==QEvent::HoverLeave) {
            transColor(imgOut,rgbOut[0]);//����ԭɫ
            zoomout->setIcon(QIcon(QPixmap::fromImage(imgOut)));
        }
        return false;//������͸�¼�
    }else if(watched==zoomin){
        if(event->type()==QEvent::HoverEnter){
            transColor(imgIn,rgbIn[1]);
            zoomin->setIcon(QIcon(QPixmap::fromImage(imgIn)));//����ˮ��ɫ
        }else if(event->type()==QEvent::HoverLeave) {
            transColor(imgIn,rgbIn[0]);
            zoomin->setIcon(QIcon(QPixmap::fromImage(imgIn)));//����ԭɫ
        }
        return false;//������͸�¼�
    }else if(watched==cw){
        if(event->type()==QEvent::HoverEnter){
            transColor(imgCw,rgbCw[1]);
            cw->setIcon(QIcon(QPixmap::fromImage(imgCw)));//���û�ɫ
        }else if(event->type()==QEvent::HoverLeave) {
            transColor(imgCw,rgbCw[0]);
            cw->setIcon(QIcon(QPixmap::fromImage(imgCw)));//����ԭɫ
        }else if(event->type()==QEvent::MouseButtonPress){
            transColor(imgCw,rgbCw[2]);
            cw->setIcon(QIcon(QPixmap::fromImage(imgCw)));//����ˮ��ɫ
        }else if(event->type()==QEvent::MouseButtonPress){//�����»ָ�hover��ɫ
            transColor(imgCw,rgbCw[1]);
            cw->setIcon(QIcon(QPixmap::fromImage(imgCw)));//���û�ɫ
        }
        return false;//������͸�¼�
    }else if(watched==acw){
        if(event->type()==QEvent::HoverEnter){
            transColor(imgAcw,rgbAcw[1]);
            acw->setIcon(QIcon(QPixmap::fromImage(imgAcw)));//���û�ɫ
        }else if(event->type()==QEvent::HoverLeave) {
            transColor(imgAcw,rgbAcw[0]);
            acw->setIcon(QIcon(QPixmap::fromImage(imgAcw)));//����ԭɫ
        }else if(event->type()==QEvent::MouseButtonPress){
            transColor(imgAcw,rgbAcw[2]);
            acw->setIcon(QIcon(QPixmap::fromImage(imgAcw)));//����ˮ��ɫ
        }else if(event->type()==QEvent::MouseButtonPress){//�����»ָ�hover��ɫ
            transColor(imgAcw,rgbAcw[1]);
            acw->setIcon(QIcon(QPixmap::fromImage(imgAcw)));//���û�ɫ
        }
        return false;//������͸�¼�
    }
    return QWidget::eventFilter(watched,event);
}
//������ͼƬ�����ظ�ֵ
void HeadImgWidget::transColor(QImage &img, const QHash<QPoint, QRgb> &colors)
{
    QHash<QPoint, QRgb >::const_iterator i =colors.constBegin();
    while (i != colors.constEnd()) {
        img.setPixelColor(i.key().x(),i.key().y(),QColor(i.value()));
        ++i;
    }
}


