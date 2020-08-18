#include "nativesocket.h"
#include<qjsonobject.h>
#include <qdatastream.h>

NativeSocket::NativeSocket(QObject *parent)
    :QTcpSocket(parent)
{
    size=0;
    IT=NoAnything;
    connect(this,&NativeSocket::readyRead,this,&NativeSocket::readD);
}

NativeSocket::~NativeSocket()
{
    if(this->isOpen())this->close();
    qDebug()<< "~NativeSocket()";
}

void NativeSocket::getFVerify(QByteArray&data)
{
   qDebug()<<"get data for verify";
   if(data.isEmpty()){
       qDebug()<<"get a empty data";
       return;
   }
   emit emitFVerify(data);
   emit finished();
}

void NativeSocket::readD()
{
    while (bytesAvailable()>0&&bytesAvailable()>=size){
        if(size==0){
            QByteArray data=this->read(1);
            QDataStream stream(&data,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            size=l;
            if(l<=0){
                qDebug()<<"size is less than or equal to zero";
            }
            qDebug()<<"l="<<l;
            continue;
        }
        QByteArray data=this->read(size);
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(doc.isObject()){
            QJsonObject obj=doc.object();
            qDebug()<<obj;
            QString content=obj.value("content").toString();
            //验证消息
            if(content=="friend-verify"){
                size=obj.value("size").toInt();
                if(size==0){
                    qDebug()<<"a error:data size is not corrected";
                    emit finished();
                    return;
                }
                IT=VerifyForFriend;
                continue;
                //添加好友
            }else if(content=="addFriend"){
                QString t=obj.value("type").toString();
                if(t=="json"){
                    IT=VerifyJson;
                }else{
                    IT=VerifyImg;
                }
                size=obj.value("size").toInt();
                continue;
                //强制下线
            }else if(content=="offline"){
                QString ip=obj.value("ip").toString();
                QString host=obj.value("host").toString();
                QString dt=obj.value("datetime").toString();
                emit emitOffline(ip,host,dt);
                emit finished();
            }
        }
        switch (IT) {
        case VerifyForFriend:
            this->getFVerify(data);
            size=0;
            IT=NoAnything;
            break;
        case VerifyJson:
            bytes=data;
            size=0;
            IT=NoAnything;
            break;
        case VerifyImg:
            pix.loadFromData(data);
            this->emitGetFriend(bytes,pix);
            emit finished();
            size=0;
            IT=NoAnything;
            break;
        default:
            break;
        }
    }
}
