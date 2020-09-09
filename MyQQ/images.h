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
    Q_INVOKABLE void removeHistory();//移除历史头像
    Q_INVOKABLE void clearAllData();
    void insert(QPixmap& pix);
    //provider2 use
    Q_INVOKABLE void setPixmap2(const QString&id, const QString&pix, const QString &status);//好友图片设置
    Q_INVOKABLE void flushPixmap2(QString id,QString status);
    Q_INVOKABLE  void roundPixmap(QPixmap&);//圆图 用于托盘显示圆形头像闪烁

    //provider3 use
    Q_INVOKABLE qint32 insertPixmap3(const int&count, QVector<QString> files);//插入 图片集到开头， 图片id重新排列
    Q_INVOKABLE bool setPixmap3(const QString& id, const QString&filename);//添加图片到图片集合
Q_INVOKABLE bool removePixmap3(int pos);//删除一个图片
 Q_SIGNALS:
    void historyImageAdded(QString url,qint32 index);
signals:
    void readHistoryHeadImg(QString fileList);
public:
    QmlImageProvider*provider1;//历史头像
    QmlImageProvider*provider2;//好友头像
    QmlImageProvider*provider3;//用户照片墙
    QString myqq;
};

#endif // IMAGES_H
