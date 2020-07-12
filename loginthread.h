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
    qint64 dateSubstraction(const QDate &substracted, const QDate &substraction) const;
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
    QString handleLogRec(QString&,QString& host,QString&ip)const;
    qint64 count;
};

#endif // LOGINTHREAD_H
