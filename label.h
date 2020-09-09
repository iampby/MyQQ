#ifndef LABEL_H
#define LABEL_H

#include <QObject>
#include<qlabel.h>
class Label : public QLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent= nullptr);
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);

signals:
    void clicked();
private:
   bool isLeftButtonPressed;
};

#endif // LABEL_H
