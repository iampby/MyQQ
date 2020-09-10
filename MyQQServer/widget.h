#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class MyQQRegisterServer;
class MyQQLoginServer;
class  WriteServer;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton1_clicked();

    void on_pushButton2_clicked();

    void on_pushButton3_clicked();
private:

    //对应server ip及端口

    quint16 registerPort;
    quint16 loginPort;
    quint16 writePort;
    MyQQRegisterServer* regServer;
    MyQQLoginServer*logServer;
    WriteServer*writeServer;
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
