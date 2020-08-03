#ifndef NATIVESERVER_H
#define NATIVESERVER_H
//本地服务器 用来与远程通信
#include <QObject>
#include<QTcpServer>
#include <qpixmap.h>
class NativeServer:public QTcpServer
{
    Q_OBJECT
public:
    NativeServer(QObject *parent = nullptr);
    ~NativeServer();
   signals:
    void emitExit();
    void findPort(quint16 port);
    void emitFverify(QByteArray data);
    void emitGetFriend(QByteArray data,QPixmap pix);
 public slots:
    void slotStarted();
private slots:
    void incomingConnection(qintptr socketDescriptor);
public:
    quint16 port;

};

#endif // NATIVESERVER_H
