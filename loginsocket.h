#ifndef LOGINSOCKET_H
#define LOGINSOCKET_H

#include <QJsonObject>
#include <QObject>
#include<QTcpSocket>
#include<qeventloop.h>

class LoginSocket : public QTcpSocket
{
    Q_OBJECT
public:
    LoginSocket(const QString &myqq, const QString &passwd, QObject *parent = nullptr);
    ~LoginSocket();
    quint16 serverPort;
    QString myqq;
    QString passwd;
    QJsonObject infoObj;//储存着昵称、性别、个性签名等信息
    int result;
    int flags;
    QByteArray img;
    QByteArray infoXml;
private slots:
    void readD();
    void  toWrite();
    void err(QAbstractSocket::SocketError);

signals:
    void finished(int result);
    void readyWrite();
private:
    qint64 size;
    QEventLoop loop;
};

#endif // LOGINSOCKET_H
