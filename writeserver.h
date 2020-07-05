#ifndef WRITESERVER_H
#define WRITESERVER_H

#include <QObject>
#include<qtcpserver.h>
class WriteServer : public QTcpServer
{
    Q_OBJECT
public:
    WriteServer(QObject *parent = nullptr);
    ~WriteServer();
private:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // WRITESERVER_H
