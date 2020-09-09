#ifndef SENDSOCKET_H
#define SENDSOCKET_H

#include <QObject>
#include<qtcpsocket.h>
#include<qthread.h>
#include<qeventloop.h>
class SendSocket : public QTcpSocket
{
    Q_OBJECT
public:
    SendSocket(QObject *parent = nullptr);
    ~SendSocket();
    void setIP(QString&ip);
    void setIP(const QString& ip);
    void setPort(quint16&port);
    void setPort(const quint16&port);
    void setMessage(QByteArray xml);
   void setTimeout(int ms);
   void setNumber(QString&sender,QString& receiver);
   void post();//����
signals:
   void finished(int code,QString descp);

public slots:
private slots:
    void writeMessage();
    void occuredErr(SocketError code);
    void readResult();
private:
    QString m_ip;
    quint16 m_port;
    QByteArray m_xml;
    int m_timeout;
    QString m_myqq;//���ͷ�����
    QString m_number;//���ܷ�����
    qint64 size;
    bool hasRead;//false����Է�û�з�Ӧ

    QEventLoop loop;
};

#endif // SENDSOCKET_H
