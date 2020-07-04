#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H
//×ª»»qimage to image
#include<qquickimageprovider.h>
class QmlImageProvider:public QQuickImageProvider
{
public:
    QmlImageProvider();
  const  QPixmap &valueOf(const QString& id);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    QHash<QString,QPixmap>images;
};

#endif // QMLIMAGEPROVIDER_H
