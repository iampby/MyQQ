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
    void setMyQQ(QString &mq);
   signals:
    void emitExit();//�׽����߳��˳�
    void findPort(quint16 port);
    void emitFverify(QByteArray data);
    void emitGetFriend(QByteArray data,QPixmap pix);
    void emitOffline(QString ip,QString host,QString datetime);//����֪ͨ
    void emitFMessage(QString html,QString number,QString time);//������Ϣ
 public slots:
    void slotStarted();
private slots:
    void incomingConnection(qintptr socketDescriptor);

public:
    quint16 port;
QString myqq;
};

#endif // NATIVESERVER_H
