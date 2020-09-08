#ifndef MOVEDWIDGET_H
#define MOVEDWIDGET_H

#include <QObject>
#include <QWidget>
#include<QMouseEvent>
class MovedWidget:public QWidget
{
    Q_OBJECT
public:
    MovedWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void mousePressEvent(QMouseEvent*event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*event);
protected:
    bool isPressed;//�Ƿ���
    QPoint dragPoint;//�϶���
};

#endif // MOVEDWIDGET_H
