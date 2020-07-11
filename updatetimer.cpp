#include "UpdateTimer.h"
#include<qdebug.h>
#include<qtimer.h>
#include<qthread.h>
#include<qtcpsocket.h>
#include<qeventloop.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
UpdateTimer::UpdateTimer(QObject *parent)
    :QObject(parent)
{
    //父类对象管理子类对象的内存
    t=new QThread(this);
    timer=new QTimer(this);
    tcpsocket=new QTcpSocket(this);
    loop=new QEventLoop(this);
    moveToThread(t);
    t->start();
    connect(timer,&QTimer::timeout,timer,[=](){
        startTcpScoket();
    });
    connect(tcpsocket,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    connect(tcpsocket,&QTcpSocket::connected,this,&UpdateTimer::writeInstruct);
    //直接调用子线程槽
    connect(this,&UpdateTimer::stopTimer,this,[=](){
        qDebug()<<"stopTimer";
        stop();
    },Qt::BlockingQueuedConnection);
    connect(this,&UpdateTimer::startTimer,this,[=](){
        qDebug()<<":startTimer";
        start();
    },Qt::BlockingQueuedConnection);
}

UpdateTimer::~UpdateTimer()
{
    qDebug()<<"~UpdateTimer()";
}

void UpdateTimer::setMyqq(const QString &arg)
{
    myqq=arg;
}

void UpdateTimer::stop()
{
    timer->stop();
    if(t->isRunning())t->quit();
}

void UpdateTimer::start()
{
    timer->start();
    if(!t->isRunning())t->start();
    qDebug()<<"timer->start()";
}

void UpdateTimer::setTimerInterval(const int &t)
{
    timer->setInterval(t);
}

void UpdateTimer::startTcpScoket()
{
    tcpsocket->connectToHost(ip,port);
}

void UpdateTimer::setIp(const QString &arg)
{
    ip=arg;
}

void UpdateTimer::setPort(const quint16 &arg)
{
    port=arg;
}

void UpdateTimer::writeInstruct()
{
    QJsonDocument json;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue(6));
    obj.insert("content",QJsonValue("updateUser"));
    obj.insert("myqq",QJsonValue(myqq));
    json.setObject(obj);
    tcpsocket->write(json.toBinaryData());
    qDebug()<<"pre loop->exec()";
    loop->exec();
    qDebug()<<"aft loop->exec()";
    tcpsocket->disconnect();
}
