#include "writeserver.h"
#include"writethread.h"
#include<qthread.h>
#include<qtimer.h>
WriteServer::WriteServer(QObject *parent)
 :QTcpServer(parent)
{
}

WriteServer::~WriteServer()
{
     qDebug()<<"~WriteServer()";
}

void WriteServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"viaed incoming Connection:writefile"<<endl;
    QThread*thread=new QThread();

    WriteThread*writeFileHandle=new WriteThread(socketDescriptor);
    writeFileHandle->moveToThread(thread);//�ƶ������̣߳�ͨ���źŲۻ��ư��̴߳ӵ㵽��
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,writeFileHandle,&WriteThread::deleteLater);
    connect(writeFileHandle,&WriteThread::finished,writeFileHandle,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"exited writeFileHandle";
    });

    thread->start();
}
