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
    void setMyQQ(QString &mq);
   signals:
    void emitExit();//套接字线程退出
    void findPort(quint16 port);
    void emitFverify(QByteArray data);
    void emitGetFriend(QByteArray data,QPixmap pix);
    void emitOffline(QString ip,QString host,QString datetime);//下线通知
    void emitFMessage(QString html,QString number,QString time);//好友消息
 public slots:
    void slotStarted();
private slots:
    void incomingConnection(qintptr socketDescriptor);

public:
    quint16 port;
QString myqq;
};

#endif // NATIVESERVER_H
