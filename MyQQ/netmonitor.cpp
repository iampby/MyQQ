#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "netmonitor.h"
#include <QLibrary>
#include <windows.h>
#include<Psapi.h>
#include<qdebug.h>
//函数
typedef bool (*TestConnection)(int*flags,int reserved);
//获取网络变化并发送网络变化信号
void GetInternetConnectState(qint32&online)
{
    QLibrary lib("Wininet");
    //如果正确加载了dll
    int b;
    if(lib.load())
    {
        bool isOnline=false;//是否在线
        int  flags;
        //获取dll库中的函数InternetGetConnectedState函数地址`
        TestConnection  myConnectFun=(TestConnection)lib.resolve("InternetGetConnectedState");
        //判断是否连网
        isOnline=myConnectFun(&flags,0);
        if(isOnline)
        {
            //在线：拨号上网
            if ( flags & INTERNET_CONNECTION_MODEM ) //在线：拨号上网
            {
                b=1;

            } else  if(flags & INTERNET_CONNECTION_LAN)  //在线：通过局域网
            {
                b=2;
            }
            else if(flags & INTERNET_CONNECTION_PROXY) //在线：代理
            {
                b=4;
            }
        }
        else
        {
            b = -1;//网络中断

        }
    }
    online=b;
}
NetMonitor::NetMonitor(QObject *parent)
    :QObject(parent)
{
    t=new QThread(this);
    timer=new QTimer(this);
    moveToThread(t);
    t->start();

    connect(timer,&QTimer::timeout,timer,[=](){
        //  qDebug()<<"network timer had been triggered";
        qint32 status;
        GetInternetConnectState(status);
        emit getNetStatus(status);
    });
    //利用阻塞队列连接实现线程转移
    connect(this,&NetMonitor::stopMonitor,this,[=](){
        qDebug()<<"stopMonitor";
        stop();
    },Qt::BlockingQueuedConnection);
    connect(this,&NetMonitor::startMonitor,this,[=](){
        qDebug()<<"startMonitor";
        start();
    },Qt::BlockingQueuedConnection);
}

NetMonitor::~NetMonitor()
{
    qDebug()<<"~NetMonitor()";

}

void NetMonitor::setTimerInterval(const int &arg)
{
    timer->setInterval(arg);
}

void NetMonitor::start()
{
    timer->start();
    if(!t->isRunning())t->start();
    qDebug()<<"timer->start()";
}

void NetMonitor::stop()
{
    timer->stop();
    if(t->isRunning()){
        t->requestInterruption();
        t->exit(0);
        t->quit();
        t->wait();
    }
      qDebug()<<"net monitor timer is stoped";
}


