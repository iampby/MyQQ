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
    qint64 dateSubstraction(const QDate &substracted, const QDate &substraction) const;//ʱ��� �������
    //д������Ϣ���Է�
 void writeInfoToOpposite(QString ip, quint16 port, QJsonObject obj, QString content, QByteArray bytes=QByteArray(), bool b=true);
void statusChanged(QString&status,QString&myqq);//����״̬�仯������
void setXmlStatus(QString &opposite, QString &me, QString &status);//�ı�myqq=me�ĺ���opposite�� info.xml״̬����
void offlineOtherHost(QString& ip,QString&host,QString& oldIP,quint16 &oldPort,QString&datetime);//������һ����½�� ����ip ������ ��¼ʱ������߷�
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
    bool noDis;//��trueʱ ����Զ�̶Ͽ�����ʱ���˳��߳�
};

#endif // LOGINTHREAD_H
