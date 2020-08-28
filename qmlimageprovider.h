#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H
//ת��qimage to image
#include<qquickimageprovider.h>
class QmlImageProvider:public QQuickImageProvider
{
public:
    QmlImageProvider();
    ~QmlImageProvider();
  QPixmap valueOf(const QString& id);
  void clear();
    QPixmap requestPixmap(const QString& id, QSize *size, const QSize &requestedSize);
    void convertToGray(QPixmap&pix);//��ȡ��ɫͼ
    QHash<QString,QPixmap>images;
    QHash<QString,QString>control;//�����Ƿ���ʾ��ɫͼƬ
};

#endif // QMLIMAGEPROVIDER_H
