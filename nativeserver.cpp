#include "nativeserver.h"
#include"nativesocket.h"
#include <qthread.h>
NativeServer::NativeServer(QObject *parent)
    :QTcpServer(parent)
{
    port=-1;

}

NativeServer::~NativeServer()
{
    qDebug()<<"~NativeServer()";
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
  if(!nsock->setSocketDescriptor(socketDescriptor)){
      qDebug()<<"a error:native server create a socket is of failure,socketDescriptor="<<socketDescriptor;
      nsock->deleteLater();
      nsock=nullptr;
      return;
  }
   QThread*cthread=new QThread(this);
   nsock->moveToThread(cthread);
   //��ת��֤��Ϣ
   connect(nsock,&NativeSocket::emitFVerify,this,[=](QByteArray data){
       qDebug()<<"the relay station of the data packet has passed";
       emit emitFverify(data);
   });
   //��ת������Ϣ
   connect(nsock,&NativeSocket::emitGetFriend,this,[=](QByteArray data,QPixmap pix){
       qDebug()<<"the relay station of the data packet has passed";
       emit emitGetFriend(data,pix);
   });
   connect(cthread,&QThread::finished,cthread,&QThread::deleteLater);
   connect(cthread,&QThread::finished,nsock,&NativeSocket::deleteLater);
  //�˳��̺߳���ָ��
   auto d=[=](){
       qDebug()<<"a thread exit";
       cthread->exit(0);
       cthread->quit();
   };
   connect(nsock,&NativeSocket::finished,nsock,d);
   connect(this,&NativeServer::emitExit,cthread,d);
   cthread->start();
}
