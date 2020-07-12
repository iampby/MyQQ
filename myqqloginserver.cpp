#include "myqqloginserver.h"
#include"loginthread.h"

#include <qthread.h>
MyQQLoginServer::MyQQLoginServer(QObject *parent)
    :QTcpServer(parent)
{
    count=100;
}

MyQQLoginServer::~MyQQLoginServer()
{
    qDebug()<<"~MyQQLoginServer()";
}

void MyQQLoginServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"viaed incomingC"<<endl;
    QThread*thread=new QThread();
    //控制一秒内可以打开的数据库连接数
    if(count>=200)
        count=100;
    LoginThread*loginHandle=new LoginThread(socketDescriptor,count++);
    loginHandle->moveToThread(thread);//移动到子线程，通过信号槽机制把线程从点到线
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,loginHandle,&LoginThread::deleteLater);
    connect(loginHandle,&LoginThread::finished,loginHandle,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"login thread exit";
    });
    thread->start();
    emit loginHandle->startTimer();
}
