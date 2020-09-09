#ifndef FINDBTN_H
#define FINDBTN_H

#include <QObject>
#include<QPushButton>
#include<qpen.h>
class FindBtn : public QPushButton
{
    Q_OBJECT
public:
    FindBtn(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent*event);
     QPoint points[3];
};

#endif // FINDBTN_H
