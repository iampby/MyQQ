#ifndef NATIVESOCKET_H
#define NATIVESOCKET_H

#include <QObject>
#include<qtcpsocket.h>
#include<QJsonDocument>
#include <qpixmap.h>

class NativeSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum DataType{
        NoAnything,
        VerifyForFriend,
        VerifyJson,
        VerifyImg,
    };
    NativeSocket(QObject *parent = nullptr);
    ~NativeSocket();
protected:
    void getFVerify(QByteArray &data);
protected slots:
    void readD();
signals:
    void finished();
    void emitFVerify(QByteArray data);
    void emitGetFriend(QByteArray data,QPixmap pix);
    void emitOffline(QString ip,QString host,QString datetime);//强制下线信号
protected:
    qint64 size;
    DataType IT;
    QPixmap pix;
    QByteArray bytes;
};

#endif // NATIVESOCKET_H
