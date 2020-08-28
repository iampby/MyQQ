#ifndef NATIVESOCKET_H
#define NATIVESOCKET_H

#include <QObject>
#include<qtcpsocket.h>
#include<QJsonDocument>
#include <qpixmap.h>
#include<qeventloop.h>
class NativeSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum DataType{
        NoAnything,
        VerifyForFriend,
        VerifyJson,
        VerifyImg,
        FMessage//������Ϣ
    };
    NativeSocket(QObject *parent = nullptr);
    ~NativeSocket();
    void setMyQQ(QString&mq);
protected:
    void getFVerify(QByteArray &data);
    void getFMessage(QByteArray &xml);
    bool parseXml(QByteArray&, QString &html, QString &time);
protected slots:
    void readD();
signals:
    void finished();
    void emitFVerify(QByteArray data);
    void emitGetFriend(QByteArray data,QPixmap pix);
    void emitOffline(QString ip,QString host,QString datetime);//ǿ�������ź�
    void emitFMessage(QString html,QString number,QString time);
protected:
    qint64 size;
    DataType IT;
    QPixmap pix;
    QByteArray bytes;
    QString number;//���ͷ�����|�Լ�����
    QString myqq;//myself����

    bool ok;//������Ϣ�ã��ж������Ƿ���ܳɹ�
    QEventLoop loop;
};

#endif // NATIVESOCKET_H
