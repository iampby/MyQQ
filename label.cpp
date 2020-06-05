#include "label.h"
#include<QMouseEvent>
Label::Label(QWidget *parent)
    :QLabel(parent),isLeftButtonPressed(false)
{

}

void Label::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
       {
        isLeftButtonPressed=true;
    }
    e->ignore();
}

void Label::mouseReleaseEvent(QMouseEvent *e)
{
    if((e->button()==Qt::LeftButton)&isLeftButtonPressed)
    {
      isLeftButtonPressed=false;
      emit clicked();
    }
    e->ignore();
}
