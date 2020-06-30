#include "images.h"
#include<qdebug.h>
#include<qapplication.h>
Images::Images(QObject *parent ):QObject(parent)
{
    provider=new QmlImageProvider;
    //connect(qApp,&QApplication::destroyed,this,[=](){disconnect(qApp,SIGNAL(destroyed(QObject*)));delete this;});
}



QPixmap Images::findPixmap(const QString &id)
{

    return provider->valueOf(id);
}
void Images::setPixmap(const QString &id, const QString &pix)
{
    QPixmap pixmap;
    pixmap.load(pix,"png");
    if(pixmap.isNull()){
        qDebug()<<"pixmap is null";
        return;
    }
    qDebug()<<id<<endl;
    provider->images.insert(id,pixmap);
    emit imagesChanged(id);
}
