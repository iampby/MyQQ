#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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
    //控制一秒内可以打开的数据库连接数 200
    if(count>=400)
        count=200;
    WriteThread*writeFileHandle=new WriteThread(socketDescriptor,count++);
    writeFileHandle->moveToThread(thread);//移动到子线程，通过信号槽机制把线程从点到线
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,writeFileHandle,&WriteThread::deleteLater);
    connect(writeFileHandle,&WriteThread::finished,writeFileHandle,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"exited writeFileHandle";
    });

    thread->start();
//注意 信号在死循环时不能执行
  emit  writeFileHandle->startTimer();//子线程开启计时
}

