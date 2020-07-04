#ifndef IMAGES_H
#define IMAGES_H

#include <QObject>
#include"qmlimageprovider.h"
class Images: public QObject
{
    Q_OBJECT
public:
    Images(QObject *parent = nullptr);
~Images();
    Q_INVOKABLE void setMyQQ(QString arg);
    Q_INVOKABLE  QPixmap findPixmap(const QString&id);
    Q_INVOKABLE void setPixmap(const QString&id, const QString&pix);
    Q_INVOKABLE void readHistory();
    Q_INVOKABLE void removeHistory();
    void insert(QPixmap& pix);
 Q_SIGNALS:
    void historyImageAdded(QString url);
signals:
    void readHistoryHeadImg(QString fileList);
public:
    QmlImageProvider*provider;
     QString myqq;
};

#endif // IMAGES_H
