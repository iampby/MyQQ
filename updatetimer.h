#ifndef UpdateTimer_H
#define UpdateTimer_H

#include <QObject>
#include<qmap.h>

//用于定时更新好友资料
class QTcpSocket;
class  QTimer;
class QThread;
class QEventLoop;
class UpdateTimer:public QObject
{
    Q_OBJECT
public:
    enum ReceiveType{
      NoType,
       SignatureAndName,
        HeadImage,
    };
    UpdateTimer(QObject *parent = nullptr);
    ~UpdateTimer();
    void setMyqq(const QString &arg);
    void stop();
    void start();
    void setTimerInterval(const int&);
    void startTcpScoket();
    void setIp(const QString&);
    void setPort(const quint16&);
private:
    void splitSignatureAndName(QByteArray&data);
    void makePixmap(const QByteArray&);
signals:
    void stopTimer();
    void startTimer();
    void emitResult(const bool &ok);
private slots:
    void writeInstruct();
    void readD();
public:
    QMap<QString,QPixmap>historyMap;//记录好友头像
    QMap<QString,QString>sigMap;//记录好友个性签名
    QMap<QString,QString>nameMap;//记录好友昵称
private:
    QTimer*timer;
    QThread*t;
    QTcpSocket*tcpsocket;
    QString myqq;
    QString ip;
    quint16 port;
    QEventLoop* loop;

    qint64 size;
    QString number;//好友MyQQ
    ReceiveType r_type;//接受的数据标识
};

#endif // UpdateTimer_H
