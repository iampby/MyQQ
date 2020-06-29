#ifndef REGISTERSOCKET_H
#define REGISTERSOCKET_H

#include <QObject>
#include<QTcpSocket>

class RegisterSocket : public QTcpSocket
{
    Q_OBJECT
signals:
    void finished();
    void errOccured();
public:
    RegisterSocket(const QString&name,const QString&passwd,QObject *parent = nullptr);
    QStringList infoList;
private slots:
    void readD();
    void writeD();
    void err(QAbstractSocket::SocketError);
private:
    QString name;
    QString passwd;
};

#endif // REGISTERSOCKET_H
