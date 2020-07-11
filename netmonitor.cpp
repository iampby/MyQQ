#include "netmonitor.h"
#include <QLibrary>
#include <windows.h>
#include<Psapi.h>
#include<qdebug.h>
//����
typedef bool (*TestConnection)(int*flags,int reserved);
//��ȡ����仯����������仯�ź�
void GetInternetConnectState(qint32&online)
{
    QLibrary lib("Wininet");
    //�����ȷ������dll
    int b;
    if(lib.load())
    {
        bool isOnline=false;//�Ƿ�����
        int  flags;
        //��ȡdll���еĺ���InternetGetConnectedState������ַ`
        TestConnection  myConnectFun=(TestConnection)lib.resolve("InternetGetConnectedState");
        //�ж��Ƿ�����
        isOnline=myConnectFun(&flags,0);
        if(isOnline)
        {
            //���ߣ���������
            if ( flags & INTERNET_CONNECTION_MODEM ) //���ߣ���������
            {
                b=1;

            } else  if(flags & INTERNET_CONNECTION_LAN)  //���ߣ�ͨ��������
            {
                b=2;
            }
            else if(flags & INTERNET_CONNECTION_PROXY) //���ߣ�����
            {
                b=4;
            }
        }
        else
        {
            b = -1;//�����ж�

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
    //����������������ʵ���߳�ת��
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
    if(t->isRunning())t->quit();
}


