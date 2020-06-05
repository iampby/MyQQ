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
        qDebug()<<QStringLiteral("�����׽��ֳ���!");
        return;
    }
    tcpsock->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows����
    emit hasRunning(tcpsock,this);
    connect(this,&RegisterThread::finished,tcpsock,&QTcpSocket::deleteLater);
    exec();
}











