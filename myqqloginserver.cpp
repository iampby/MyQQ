#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "myqqloginserver.h"
#include"global.h"
#include"loginthread.h"

#include <qthread.h>
#include <qtimer.h>
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
    QThread*thread=new QThread(this);
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
     //删除historyImgMuter锁集合中的一个锁
    connect(loginHandle,&LoginThread::delayedHeadImgDeletion,this,[=](const QString number){
        //延迟5s删除
        QTimer::singleShot(5000,this,[=](){
            qDebug()<<"delayed deletion";
            if(!historyImgFiles.contains(number)){
                QMutex*muter=historyImgMuter.value(number);
                historyImgMuter.remove(number);//如果没有更新标记文件，正常删除锁
                delete muter,muter=nullptr;
            }
        });
    });
    //删除锁集合中的一个锁
    connect(loginHandle,&LoginThread::delayedSigAndNameDeletion,this,[=](const QString number){
        //延迟5s删除
        QTimer::singleShot(5000,this,[=](){
             qDebug()<<"delayed deletion";
            if(!sigFiles.contains(number)){
                 QMutex*muter=sigMuter.value(number);
                sigMuter.remove(number);//如果没有更新标记文件，正常删除锁
                   delete muter,muter=nullptr;
            }
        });
    });
    thread->start();
    emit loginHandle->startTimer();
}
