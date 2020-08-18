#include "qmlimageprovider.h"
#include <QRgb>
#include<qdebug.h>

QmlImageProvider::QmlImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)

{

}

QmlImageProvider::~QmlImageProvider()
{
    qDebug()<<"~QmlImageProvider()";
}

const QPixmap  QmlImageProvider::valueOf(const QString &id)
{
    return  images.value(id);
}

QPixmap QmlImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pix=images.value(id);
    if(pix.isNull())qDebug()<<"image is null:"<<id;
if(!control.isEmpty()){
    QString status=control.value(id);
   if(status=="0")
      convertToGray(pix);
}
 return pix;
}

void QmlImageProvider::convertToGray(QPixmap &pix)
{
    qDebug()<<"colorful image convert to gray image";
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
}

