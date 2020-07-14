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
    explicit HeadImgWidget(QWidget *parent = nullptr,const bool&b=true);
    ~HeadImgWidget();
    void setHeadImg(QPixmap &head);
signals:
    void getFocus();//Widget��ý��� ���͵�qml�ر���ʷͷ��ѡ�б�ǩ
    void updateRemoteHeadImg(const QPixmap&);
    void updateMyself(const QString &number);
public slots:
    void openFile(const QString&filename);
    void okClicked(Images *images, const QString &myqq);//����ͷ�����ר��
    void okCoverClicked(const QString&myqq);//���ķ������ר��
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
