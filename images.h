#ifndef IMAGES_H
#define IMAGES_H

#include <QObject>
#include"qmlimageprovider.h"
class Images: public QObject
{
    Q_OBJECT
public:
    Images(QObject *parent = nullptr);
    Q_INVOKABLE  QPixmap findPixmap(const QString&id);
   Q_INVOKABLE void setPixmap(const QString&id, const QString&pix);
signals:
    void imagesChanged(QString id);
public:
    QmlImageProvider*provider;
};

#endif // IMAGES_H
