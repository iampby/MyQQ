#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "qmlimageprovider.h"
#include <QRgb>
#include <qbitmap.h>
#include<qdebug.h>

QmlImageProvider::QmlImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)

{

}

QmlImageProvider::~QmlImageProvider()
{
    qDebug()<<"~QmlImageProvider()";
}

QPixmap  QmlImageProvider::valueOf(const QString &id)
{
    return  images.value(id);
}

void QmlImageProvider::clear()
{
    images.clear();
    control.clear();
}

QPixmap QmlImageProvider::requestPixmap(const QString& id, QSize *size, const QSize &requestedSize)
{
QString tid=id;
    bool ng= tid.contains("NotGray", Qt::CaseInsensitive);
    if(ng)tid=tid.left(tid.length()-7);
    QPixmap pix=images.value(tid);
    if(pix.isNull()){
        pix=images.value(id);
        if(pix.isNull()){
             qDebug()<<"image is null:"<<id;
        }
        return pix;
    }
    if(!control.isEmpty()&&!ng){
        QString status=control.value(tid);
        if(status=="0")
            convertToGray(pix);
    }
    return pix;
}

void QmlImageProvider::convertToGray(QPixmap &pix)
{
    qDebug()<<"colorful image convert to gray image";
    QBitmap map= pix.mask();
    QImage img=pix.toImage();
    int h=img.height(),w=img.width();
    int gray=0;
    for (int i = 0; i <h; ++i) {
        QRgb*line=(QRgb*)img.scanLine(i);
        for (int j = 0; j < w; ++j) {
            QRgb rgb= line[j];
            gray = (qRed(rgb) * 19595 + qGreen(rgb) * 38469 + qBlue(rgb) * 7472) >> 16;  //灰度计算公式
            img.setPixelColor(j,i,QColor(gray,gray,gray));
        }
    }
    pix=QPixmap::fromImage(img);
    pix.setMask(map);//重设遮罩
}

