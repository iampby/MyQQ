#ifndef WRITESERVER_H
#define WRITESERVER_H

#include <QObject>
#include<qtcpserver.h>

//更新信息
class WriteServer : public QTcpServer
{
    Q_OBJECT
public:
    WriteServer(QObject *parent = nullptr);
    ~WriteServer();
private:
    void incomingConnection(qintptr socketDescriptor);
    qint64 count;
};

#endif // WRITESERVER_H

