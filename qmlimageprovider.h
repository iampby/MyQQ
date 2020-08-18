#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H
//转换qimage to image
#include<qquickimageprovider.h>
class QmlImageProvider:public QQuickImageProvider
{
public:
    QmlImageProvider();
    ~QmlImageProvider();
  const QPixmap valueOf(const QString& id);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    void convertToGray(QPixmap&pix);//获取灰色图
    QHash<QString,QPixmap>images;
    QHash<QString,QString>control;//控制是否显示灰色图片
};

#endif // QMLIMAGEPROVIDER_H
