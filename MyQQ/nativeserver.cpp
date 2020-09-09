#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "nativeserver.h"
#include"nativesocket.h"
#include <qthread.h>
#include<qtimer.h>
NativeServer::NativeServer(QObject *parent)
    :QTcpServer(parent)
{
    port=-1;

}

NativeServer::~NativeServer()
{
    qDebug()<<"~NativeServer()";

}

void NativeServer::setMyQQ(QString& mq)
{
    myqq=mq;
}

void NativeServer::slotStarted()
{
    quint16 p=5565;
    while (true) {
        if(this->listen(QHostAddress::Any,++p)){
            qDebug()<<"a useful port "<<p<<" is found";
            port=p;
            emit findPort(port);
            break;
        }
    }
}

void NativeServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"incomingConnection(qintptr socketDescriptor)";
    NativeSocket*nsock=new NativeSocket();
    nsock->setMyQQ(myqq);
    if(!nsock->setSocketDescriptor(socketDescriptor)){
        qDebug()<<"a error:native server create a socket is of failure,socketDescriptor="<<socketDescriptor;
        nsock->deleteLater();
        nsock=nullptr;
        return;
    }
    QThread*cthread=new QThread();//不要设置this为父对象，因为emitexit时会直接调用this->deletelater导致线程先失去控制而不能退出
    nsock->moveToThread(cthread);
    //中转验证消息
    connect(nsock,&NativeSocket::emitFVerify,this,[=](QByteArray data){
        qDebug()<<"the relay station of the data packet of friends verify has passed";
        emit emitFverify(data);
    });
    //中转添加好友消息
    connect(nsock,&NativeSocket::emitGetFriend,this,[=](QByteArray data,QPixmap pix){
        qDebug()<<"the relay station of the data packet of added friend information  has passed";
        emit emitGetFriend(data,pix);
    });
    //下线通知中间站
    connect(nsock,&NativeSocket::emitOffline,this,[=](QString ip,QString host,QString datetime){
        qDebug()<<"the relay station of the data packet of offline has passed";
        emit emitOffline(ip,host,datetime);
    });
    //中转好友html消息
    connect(nsock,&NativeSocket::emitFMessage,this,[=](QString html,QString number,QString time){
        qDebug()<<"the relay station of the data packet of message has passed";
        emit emitFMessage(html,number,time);
    });
    //删除
    connect(cthread,&QThread::finished,cthread,&QThread::deleteLater);
    connect(cthread,&QThread::finished,nsock,&NativeSocket::deleteLater);
    //退出线程函数指针
    auto d=[=](){
        qDebug()<<"a thread exit";
        //给它一套退出操作
        cthread->requestInterruption();
        cthread->exit(0);
        cthread->quit();
        cthread->wait();
 qDebug()<<"end wait";
    };
    connect(nsock,&NativeSocket::finished,nsock,d);
    connect(this,&NativeServer::emitExit,cthread,d);//先执行槽函数
    cthread->start();
}


