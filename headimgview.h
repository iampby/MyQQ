#ifndef HEADIMGVIEW_H
#define HEADIMGVIEW_H

#include <QObject>
#include<QGraphicsView>
#include<QBitmap>
class QSlider;
class HeadImgView:public QGraphicsView
{
    Q_OBJECT
protected:
    enum ViewDirection{
        Top,
        Bottom,
        Left,
        Right
    };
public:
    HeadImgView(QWidget *parent = nullptr);
    void setImage(QPixmap &image);
    void setSlider(QSlider*);

    void   drawBackground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);//拉伸图片
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    const QPixmap getGrabPixmap();
protected:
    void valueChanged(int);
signals:
void getFocus();
public slots:
    void zoomOutClicked();
    void zoomInClicked();
    void cwClicked();
    void acwClicked();
protected:
    ViewDirection direct;
private:
    QPixmap img;
    QSlider*slider;
    QPixmap imgBackup;
    QPoint posBegin;//当开始按下时鼠标所在的位置
    QPoint posPreScene;//当开始按下时scene所在的位置
    bool control;//控制slider在最小|最大时继续拖动不反响
    QPoint rangPos;
};

#endif // HEADIMGVIEW_H
