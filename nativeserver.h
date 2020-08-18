#ifndef NATIVESERVER_H
#define NATIVESERVER_H
//���ط����� ������Զ��ͨ��
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
    void emitOffline(QString ip,QString host,QString datetime);//����֪ͨ
 public slots:
    void slotStarted();
private slots:
    void incomingConnection(qintptr socketDescriptor);
public:
    quint16 port;

};

#endif // NATIVESERVER_H
