#ifndef RIGISTERTHREAD_H
#define RIGISTERTHREAD_H

#include <QObject>
#include<qthread.h>
#include<QTcpSocket>
#include<qtimer.h>
class RegisterThread:public QThread
{
    Q_OBJECT
public:
    RegisterThread(qintptr socketDescriptor,QObject *parent = nullptr);
    void  run();
signals:
    void error(QTcpSocket::SocketError error);
    void hasRunning(QTcpSocket*sock,QThread*t);

private:
   qintptr socketDescriptor;

   bool registerMyQQ(const QString& name,const QString&passwd);
};

#endif // RIGISTERTHREAD_H
