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
    void getFocus();//Widget获得焦点 发送到qml关闭历史头像选中标签
    void updateRemoteHeadImg(const QPixmap&);
public slots:
    void openFile(const QString&filename);
    void okClicked(Images *images);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void transColor(QImage&img, const QHash<QPoint, QRgb> &colors);

private:
    HeadImgView*view;
    QPushButton*zoomout;//缩小
    QSlider*slider;
    QPushButton*zoomin;//放大
    QPushButton*cw;//顺时针
    QPushButton*acw;//逆时针
    QImage imgIn;//放大图片
    QImage imgOut;//缩小图片
    QImage imgCw;//顺时针图片
    QImage imgAcw;//逆时针图片
    QHash<QPoint,QRgb> rgbIn[2];//放大图片图像信息
    QHash<QPoint,QRgb> rgbOut[2];//缩小图片图像信息
    QHash<QPoint,QRgb> rgbCw[3];//顺时针图片图像信息
    QHash<QPoint,QRgb> rgbAcw[3];//逆时针图片图像信息
    QPixmap imgHead;//头像
    bool isread;
};

#endif // HEADIMGWIDGET_H
