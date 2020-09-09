#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "movedwidget.h"

MovedWidget::MovedWidget(QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
isPressed=false;
dragPoint=QPoint(0,0);
}

void MovedWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){

        dragPoint=event->globalPos()-QPoint(this->x(),this->y());
        isPressed=true;
    }
    event->accept();
}

void MovedWidget::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton)&&isPressed){
        this->move(event->globalPos()-dragPoint);
    }
    event->accept();
}

void MovedWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        isPressed=false;
    event->accept();
}
