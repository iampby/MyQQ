#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "combobox.h"
#include<QMouseEvent>
#include<qdebug.h>
#include<qlineedit.h>

//class combobox
ComboBox::ComboBox(QWidget *parent):QComboBox(parent),isLeftButtonPressed(false),m_text(QString())
{
setInsertPolicy(QComboBox::NoInsert);
}

void ComboBox::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
       {
        isLeftButtonPressed=true;
    }
    e->accept();
}

void ComboBox::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<"release";
    if((e->button()==Qt::LeftButton)&isLeftButtonPressed)
    {
      isLeftButtonPressed=false;
      emit clicked();
    }
    e->accept();
}



void ComboBox::hidePopup()
{
    QComboBox::hidePopup();
    emit emitHidePopup();
}

void ComboBox::showPopup()
{
    QComboBox::showPopup();
}

void ComboBox::setText(const QString &arg)
{
    m_text=arg;
    setCurrentText(m_text);
    lineEdit()->setCursorPosition(0);
}

QString ComboBox::text()const
{
    return m_text;
}






