#include "writeserver.h"
#include"writethread.h"
#include<qthread.h>
#include<qtimer.h>

WriteServer::WriteServer(QObject *parent)
 :QTcpServer(parent)
{
    count=200;
}

WriteServer::~WriteServer()
{
     qDebug()<<"~WriteServer()";
}

void WriteServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"viaed incoming Connection:writefile"<<endl;
    QThread*thread=new QThread();
    //����һ���ڿ��Դ򿪵����ݿ������� 200
    if(count>=400)
        count=200;
    WriteThread*writeFileHandle=new WriteThread(socketDescriptor,count++);
    writeFileHandle->moveToThread(thread);//�ƶ������̣߳�ͨ���źŲۻ��ư��̴߳ӵ㵽��
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,writeFileHandle,&WriteThread::deleteLater);
    connect(writeFileHandle,&WriteThread::finished,writeFileHandle,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"exited writeFileHandle";
    });

    thread->start();
  emit  writeFileHandle->startTimer();//���߳̿�����ʱ
}
