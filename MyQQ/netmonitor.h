#ifndef NETMONITOR_H
#define NETMONITOR_H
// Flags for InternetGetConnectedState
#define INTERNET_CONNECTION_MODEM           1
#define INTERNET_CONNECTION_LAN             2
#define INTERNET_CONNECTION_PROXY           4
#define INTERNET_CONNECTION_MODEM_BUSY      8
#include <QObject>
#include<QTimer>
#include<QThread>

class NetMonitor:public QObject
{
    Q_OBJECT
public:
    NetMonitor(QObject *parent = nullptr);
   ~ NetMonitor();
    void setTimerInterval(const int&);
    void start();
void stop();
signals:
    void getNetStatus(qint32 &);
    void stopMonitor();
    void startMonitor();
private:
    QTimer* timer;
    QThread* t;
};

#endif // NETMONITOR_H
