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
    qint64 dateSubstraction(const QDate &substracted, const QDate &substraction) const;//ʱ��� �������
    //д������Ϣ���Է�
    void writeInfoToOpposite(QString ip, quint16 port, QJsonObject obj, QString content, QByteArray bytes=QByteArray(), bool b=true);
    void statusChanged(QString status, QString&myqq);//����״̬�仯������
    void setXmlStatus(QString &opposite, QString &me, QString &status);//�ı�myqq=me�ĺ���opposite�� info.xml״̬����
    void offlineOtherHost(QString& ip,QString&host,QString& oldIP,quint16 &oldPort,QString&datetime);//������һ����½�� ����ip ������ ��¼ʱ������߷�
   void onlineDispatchedIP(QJsonObject &writeObj, QString &number, const QString&ip, const quint16&port, QString &status);//��ѯ����״̬ʱʹ�ã�Ϊ�˱����lambda��qsqldatabase������
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
    bool noDis;//��trueʱ ����Զ�̶Ͽ�����ʱ���˳��߳�
    QEventLoop loop;//����д���Ƿ����

    QQueue<QString>dbPool;//���ݿ��
};

#endif // LOGINTHREAD_H
