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
    QThread*cthread=new QThread();//��Ҫ����thisΪ��������Ϊemitexitʱ��ֱ�ӵ���this->deletelater�����߳���ʧȥ���ƶ������˳�
    nsock->moveToThread(cthread);
    //��ת��֤��Ϣ
    connect(nsock,&NativeSocket::emitFVerify,this,[=](QByteArray data){
        qDebug()<<"the relay station of the data packet of friends verify has passed";
        emit emitFverify(data);
    });
    //��ת��Ӻ�����Ϣ
    connect(nsock,&NativeSocket::emitGetFriend,this,[=](QByteArray data,QPixmap pix){
        qDebug()<<"the relay station of the data packet of added friend information  has passed";
        emit emitGetFriend(data,pix);
    });
    //����֪ͨ�м�վ
    connect(nsock,&NativeSocket::emitOffline,this,[=](QString ip,QString host,QString datetime){
        qDebug()<<"the relay station of the data packet of offline has passed";
        emit emitOffline(ip,host,datetime);
    });
    //��ת����html��Ϣ
    connect(nsock,&NativeSocket::emitFMessage,this,[=](QString html,QString number,QString time){
        qDebug()<<"the relay station of the data packet of message has passed";
        emit emitFMessage(html,number,time);
    });
    //ɾ��
    connect(cthread,&QThread::finished,cthread,&QThread::deleteLater);
    connect(cthread,&QThread::finished,nsock,&NativeSocket::deleteLater);
    //�˳��̺߳���ָ��
    auto d=[=](){
        qDebug()<<"a thread exit";
        //����һ���˳�����
        cthread->requestInterruption();
        cthread->exit(0);
        cthread->quit();
        cthread->wait();
 qDebug()<<"end wait";
    };
    connect(nsock,&NativeSocket::finished,nsock,d);
    connect(this,&NativeServer::emitExit,cthread,d);//��ִ�вۺ���
    cthread->start();
}


