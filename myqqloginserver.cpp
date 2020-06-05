#include "myqqloginserver.h"
#include"loginthread.h"

#include <qthread.h>
MyQQLoginServer::MyQQLoginServer(QObject *parent)
    :QTcpServer(parent)
{
count=100;
}

void MyQQLoginServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"viaed incomingC"<<endl;
    QThread*thread=new QThread();
    if(count>200)
        count=100;
    LoginThread*loginHandle=new LoginThread(socketDescriptor,count++);
    loginHandle->moveToThread(thread);//�ƶ������̣߳�ͨ���źŻ��ư��̴߳ӵ㵽��
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,loginHandle,&LoginThread::deleteLater);
    connect(loginHandle,&LoginThread::finished,thread,[=](){
        thread->exit();
    });
    thread->start();
}
