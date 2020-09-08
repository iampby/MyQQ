#ifndef MYQQTCPSERVER_H
#define MYQQTCPSERVER_H

#include <QObject>
#include<QTcpServer>

class MyQQRegisterServer:public QTcpServer
{
    Q_OBJECT
public:
    MyQQRegisterServer(QObject *parent = nullptr);
   ~ MyQQRegisterServer();

private slots:
void read(QTcpSocket*tcpsock,QThread*t);
private:
    void incomingConnection(qintptr socketDescriptor);
    bool registerMyQQ(const QString& name, const QString&passwd, qint64&myqq, QString &cn);
    qint64 count;//计数,用于数据库连接命名
};

#endif // MYQQTCPSERVER_H
