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
    //����һ���ڿ��Դ򿪵����ݿ�������
    if(count>=200)
        count=100;
    LoginThread*loginHandle=new LoginThread(socketDescriptor,count++);
    loginHandle->moveToThread(thread);//�ƶ������̣߳�ͨ���źŲۻ��ư��̴߳ӵ㵽��
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
