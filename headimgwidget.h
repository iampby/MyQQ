#ifndef HEADIMGWIDGET_H
#define HEADIMGWIDGET_H

#include <QObject>
#include <QWidget>
#include"headimgview.h"
#include<qhash.h>
class QPushButton;
class QSlider;
class QLabel;
class Images;
class HeadImgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadImgWidget(QWidget *parent = nullptr);
    void setHeadImg(QPixmap&head);
    ~HeadImgWidget();
signals:
    void getFocus();//Widget��ý��� ���͵�qml�ر���ʷͷ��ѡ�б�ǩ
    void updateRemoteHeadImg(const QPixmap&);
public slots:
    void openFile(const QString&filename);
    void okClicked(Images *images);
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
    bool isread;
};

#endif // HEADIMGWIDGET_H
