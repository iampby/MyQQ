#ifndef LOGINTHREAD_H
#define LOGINTHREAD_H

#include<qobject.h>
#include<QTcpSocket>
#include<qeventloop.h>
#include<QQueue>
#include<qsqlquery.h>
class LoginThread:public QObject
{
    Q_OBJECT
public:
    LoginThread(qintptr socketDescriptor,qint64 count=100,QObject *parent = nullptr);
    ~ LoginThread();
protected:
    qint64 dateSubstraction(const QDate &substracted, const QDate &substraction) const;//时间差 返回年纪
    //写好友消息给对方
    void writeInfoToOpposite(QString ip, quint16 port, QJsonObject obj, QString content, QByteArray bytes=QByteArray(), bool b=true);
    void statusChanged(QString status, QString&myqq);//上线状态变化处理处理
    void setXmlStatus(QString &opposite, QString &me, QString &status);//改变myqq=me的好友opposite的 info.xml状态属性
    void offlineOtherHost(QString& ip,QString&host,QString& oldIP,quint16 &oldPort,QString&datetime);//下线另一个登陆点 发送ip 主机名 登录时间给下线方
   void onlineDispatchedIP(QJsonObject &writeObj, QString &number, const QString&ip, const quint16&port, QString &status);//查询在线状态时使用，为了避免对lambda对qsqldatabase的引用
signals:
    void error(QAbstractSocket::SocketError);
    void finished();
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
    QEventLoop loop;//控制写入是否继续

    QQueue<QString>dbPool;//数据库池
};

#endif // LOGINTHREAD_H
