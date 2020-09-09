#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "findbtn.h"
#include<qpainter.h>
#include<QPaintEvent>
FindBtn::FindBtn(QWidget *parent):QPushButton(parent)
{
    setCheckable(true);
    resize(50,30);

    points[0]=QPoint(19,30);
    points[1]=QPoint(25,24);
    points[2]=QPoint(31,30);
}

void FindBtn::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    QPen wPen;
    QBrush wBrush;

    wPen.setColor(QColor(239,242,245,245));
    wBrush.setColor(QColor(239,242,245,245));
    wBrush.setStyle(Qt::SolidPattern);
    p.setPen(wPen);
    p.setBrush(wBrush);
    p.drawText(event->rect(),Qt::AlignHCenter|Qt::AlignTop,text());
    if(isChecked()){
        p.drawPolygon(points,3);
    }else {
        QPen bPen;
        QBrush bBrush;
        bBrush.setStyle(Qt::SolidPattern);
        bPen.setColor(QColor(0,155,219));
        bBrush.setColor(QColor(0,155,219));
        p.setBrush(bBrush);p.setPen(bPen);
        p.drawPolygon(points,3);
    }
    p.end();
update();
}
