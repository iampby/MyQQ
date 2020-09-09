#ifndef TITLEBAR_H
#define TITLEBAR_H
//自定义标题栏
#include <QWidget>
#include<QMouseEvent>
class QPushButton;
class titleBar : public QWidget
{
    Q_OBJECT
public:
    explicit titleBar(QWidget *parent = nullptr);

void  mousePressEvent(QMouseEvent *event);
void mouseMoveEvent(QMouseEvent *event);
void mouseReleaseEvent(QMouseEvent*event);
signals:
void closeBtnClicked();
void minBtnClicked();
protected slots:
void forwardSiganlForMinBtn();
void forwardSiganlForCloseBtn();
public:

private:
QPoint dragPoint;
QPushButton*minBtn;
QPushButton*closeBtn;
bool isInsidePressed;
};

#endif // TITLEBAR_H
