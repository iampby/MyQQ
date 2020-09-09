#ifndef HEADIMGVIEW_H
#define HEADIMGVIEW_H
//Ƕ��qml�� �޸�ͼ����ͼ �ɼ�����
#include <QObject>
#include<QGraphicsView>
#include<QBitmap>
class QSlider;
class HeadImgView:public QGraphicsView
{
    Q_OBJECT
protected:
    //��ͼ��λö��
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

    void   drawBackground(QPainter *painter, const QRectF &rect);//������������
    void wheelEvent(QWheelEvent *event);//����ͼƬ
    void mouseMoveEvent(QMouseEvent *event);//����ͼƬλ��
    void mousePressEvent(QMouseEvent *event);//��¼����λ��
    const QPixmap getGrabPixmap();//�����ؼ�
protected:
    void valueChanged(int);
signals:
void getFocus();
public slots:
    void zoomOutClicked();
    void zoomInClicked();
    void cwClicked();//��ͼ˳ʱ����ת90 ͬʱ��¼��ͼ��λ
    void acwClicked();
protected:
    ViewDirection direct;
private:
    QPixmap img;
    QSlider*slider;
    QPixmap imgBackup;
    QPoint posBegin;//����ʼ����ʱ������ڵ�λ��
    QPoint posPreScene;//����ʼ����ʱscene���ڵ�λ��
    bool control;//����slider����С|���ʱ�����϶�������
    QPoint rangPos;
    bool hasMask;//Ĭ�Ͽ�������
};

#endif // HEADIMGVIEW_H
