#ifndef HEADIMGVIEW_H
#define HEADIMGVIEW_H
//嵌入qml的 修改图像视图 可加遮罩
#include <QObject>
#include<QGraphicsView>
#include<QBitmap>
class QSlider;
class HeadImgView:public QGraphicsView
{
    Q_OBJECT
protected:
    //视图方位枚举
    enum ViewDirection{
        Top,
        Bottom,
        Left,
        Right
    };
public:
    HeadImgView(QWidget *parent = nullptr, const bool &b=true);
    void setImage(QPixmap &image);
    void setSlider(QSlider*);

    void   drawBackground(QPainter *painter, const QRectF &rect);//画背景及遮罩
    void wheelEvent(QWheelEvent *event);//拉伸图片
    void mouseMoveEvent(QMouseEvent *event);//修正图片位置
    void mousePressEvent(QMouseEvent *event);//记录按下位置
    const QPixmap getGrabPixmap();//截屏控件
protected:
    void valueChanged(int);
signals:
void getFocus();
public slots:
    void zoomOutClicked();
    void zoomInClicked();
    void cwClicked();//视图顺时针旋转90 同时记录视图方位
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
    bool hasMask;//默认开启遮罩
};

#endif // HEADIMGVIEW_H
