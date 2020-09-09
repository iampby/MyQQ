#ifndef ADDFRIENDGROUPWIDGET_H
#define ADDFRIENDGROUPWIDGET_H


#include <QObject>
#include <QWidget>
#include<QMouseEvent>
class AddFriendGroupWidget:public QWidget
{
    Q_OBJECT
public:
    AddFriendGroupWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AddFriendGroupWidget();
    void mousePressEvent(QMouseEvent*event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*event);
signals:
    void emitGroup(QString name);
    void setNull();
protected:
    bool isPressed;//是否按下
    QPoint dragPoint;//拖动点
};

#endif // ADDFRIENDGROUPWIDGET_H
