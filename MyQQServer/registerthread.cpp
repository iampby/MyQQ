#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "registerthread.h"
#include<qdatastream.h>
#include<qbytearray.h>
#include<qiodevice.h>
#include<qsqlquery.h>
#include<QApplication>
#include<qsqlrecord.h>
#include<qdebug.h>

RegisterThread::RegisterThread(qintptr socketDescriptor, QObject *parent)
    :QThread (parent),socketDescriptor(socketDescriptor)
{

}

void RegisterThread::run()
{
    QTcpSocket* tcpsock=new QTcpSocket();
    if(!tcpsock->setSocketDescriptor(socketDescriptor)){
        emit error(tcpsock->error());
        qDebug()<<("设置套接字出错!");
        return;
    }
    tcpsock->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows必设
    emit hasRunning(tcpsock,this);
    connect(this,&RegisterThread::finished,tcpsock,&QTcpSocket::deleteLater);
    //子线程调用定时器
    QTimer::singleShot(30000,Qt::CoarseTimer,tcpsock,[=](){
        qDebug()<<"timer 30s exit";
        this->exit(1);
        this->quit();
        qDebug()<<"thread had exited";
    });
    exec();
}











