#ifndef MYQQLOGINSERVER_H
#define MYQQLOGINSERVER_H

#include <QObject>
#include<qtcpserver.h>
class MyQQLoginServer : public QTcpServer
{
    Q_OBJECT
public:
    MyQQLoginServer(QObject *parent = nullptr);
    ~MyQQLoginServer();

signals:

private slots:

private:
    void incomingConnection(qintptr socketDescriptor);
    qint64 count;

};

#endif // MYQQLOGINSERVER_H
