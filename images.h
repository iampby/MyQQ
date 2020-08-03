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
    //provider1 use
    Q_INVOKABLE void setMyQQ(QString arg);
    Q_INVOKABLE  QPixmap findPixmap(const QString&id);
    Q_INVOKABLE void setPixmap(const QString&id, const QString&pix);
    Q_INVOKABLE void readHistory();
    Q_INVOKABLE void removeHistory();
    void insert(QPixmap& pix);
    //provider2 use
    Q_INVOKABLE void setPixmap2(const QString&id, const QString&pix);
    //provider3 use
    Q_INVOKABLE qint32 insertPixmap3(const int&count, QVector<QString> files);//���� ͼƬ������ͷ�� ͼƬid��������
    Q_INVOKABLE bool setPixmap3(const QString& id, const QString&filename);//����ͼƬ��ͼƬ����


 Q_SIGNALS:
    void historyImageAdded(QString url,qint32 index);
signals:
    void readHistoryHeadImg(QString fileList);
public:
    QmlImageProvider*provider1;//��ʷͷ��
    QmlImageProvider*provider2;//����ͷ��
    QmlImageProvider*provider3;//�û���Ƭǽ
    QString myqq;
};

#endif // IMAGES_H