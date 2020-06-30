#ifndef HEADIMGWIDGET_H
#define HEADIMGWIDGET_H

#include <QObject>
#include <QWidget>
#include"headimgview.h"
#include<qhash.h>
class QPushButton;
class QSlider;
class QLabel;
class HeadImgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadImgWidget(QWidget *parent = nullptr);
    void setHeadImg(QPixmap&head);
signals:
public slots:
void openFile(const QString&filename);
void okClicked();
protected:
bool eventFilter(QObject *watched, QEvent *event);
void transColor(QImage&img, const QHash<QPoint, QRgb> &colors);
private:
    HeadImgView*view;
    QPushButton*zoomout;//��С
    QSlider*slider;
    QPushButton*zoomin;//�Ŵ�
    QPushButton*cw;//˳ʱ��
    QPushButton*acw;//��ʱ��
    QImage imgIn;//�Ŵ�ͼƬ
    QImage imgOut;//��СͼƬ
    QImage imgCw;//˳ʱ��ͼƬ
    QImage imgAcw;//��ʱ��ͼƬ
  QHash<QPoint,QRgb> rgbIn[2];//�Ŵ�ͼƬͼ����Ϣ
  QHash<QPoint,QRgb> rgbOut[2];//��СͼƬͼ����Ϣ
  QHash<QPoint,QRgb> rgbCw[3];//˳ʱ��ͼƬͼ����Ϣ
   QHash<QPoint,QRgb> rgbAcw[3];//��ʱ��ͼƬͼ����Ϣ
   QPixmap imgHead;//ͷ��

};

#endif // HEADIMGWIDGET_H
