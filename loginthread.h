#ifndef LOGINTHREAD_H
#define LOGINTHREAD_H

#include<qobject.h>
#include<QTcpSocket>
class LoginThread:public QObject
{
    Q_OBJECT
public:
    LoginThread(qintptr socketDescriptor,qint64 count=100,QObject *parent = nullptr);
   ~ LoginThread();
    qint64 dateSubstraction(const QDate &substracted, const QDate &substraction) const;//时间差 返回年纪
    //写好友消息给对方
 void writeInfoToOpposite(QString ip, quint16 port, QJsonObject obj, QString content, QByteArray bytes=QByteArray(), bool b=true);
void statusChanged(QString&status,QString&myqq);//上线状态变化处理处理
void setXmlStatus(QString &opposite, QString &me, QString &status);//改变myqq=me的好友opposite的 info.xml状态属性
void offlineOtherHost(QString& ip,QString&host,QString& oldIP,quint16 &oldPort,QString&datetime);//下线另一个登陆点 发送ip 主机名 登录时间给下线方
signals:
    void error(QAbstractSocket::SocketError);
    void finished();
    void loopStop();
    void startTimer();
    void  delayedHeadImgDeletion(const QString);
    void  delayedSigAndNameDeletion(const QString);
public slots:
void timer();
private slots:
      void readD();
      void disconnected();
private:
    qintptr socketDescriptor;
    QTcpSocket*tcpsocket;
    QString handleLogRec(QString&, QString& host, QString&ip, QString &port)const;
    qint64 count;
    bool noDis;//当true时 控制远程断开连接时不退出线程
};

#endif // LOGINTHREAD_H
