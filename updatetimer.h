#ifndef UpdateTimer_H
#define UpdateTimer_H

#include <QObject>


//用于定时更新好友资料
class QTcpSocket;
class  QTimer;
class QThread;
class QEventLoop;
class UpdateTimer:public QObject
{
    Q_OBJECT
public:
    UpdateTimer(QObject *parent = nullptr);
    ~UpdateTimer();
    void setMyqq(const QString &arg);
    void stop();
    void start();
    void setTimerInterval(const int&);
    void startTcpScoket();
    void setIp(const QString&);
    void setPort(const quint16&);
signals:
    void stopTimer();
    void startTimer();
private slots:
    void writeInstruct();
private:
    QTimer*timer;
    QThread*t;
    QTcpSocket*tcpsocket;
    QString myqq;
    QString ip;
    quint16 port;
    QEventLoop* loop;
};

#endif // UpdateTimer_H
