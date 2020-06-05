#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include<QTcpSocket>
#include<QDataStream>
#include<qjsonobject.h>
#include <QJsonDocument>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QObject *parent = nullptr);
    static bool writeImg(const QByteArray &content, const QString &filepath, const char *format);
    static void setMyqq(const QString&);
    static void setBeginId(const QString&);
    static QString myqq();
    static QString beginId();
    void  setInstruct(const QJsonObject &);
    QJsonObject&  getInstruct();
    QString cityData;
    QJsonDocument jsonDoc;
signals:
    void getCityModel();
    void failed();
    void finished();
private slots:
    void readD();
    void writeD();
private:
    QJsonObject instruct;
    QString fileName;
    QByteArray fileData;
    qint64 size;
    qint32 flags;
    static QString m_myqq;//我的号码
    static QString m_beginId;//找好友时开始的ID值
};

#endif // TCPSOCKET_H
