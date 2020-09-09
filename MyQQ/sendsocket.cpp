#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "sendsocket.h"

#include <qdatastream.h>
#include <qtimer.h>
#include<qjsonobject.h>
#include<qjsondocument.h>

SendSocket::SendSocket(QObject *parent)
    :QTcpSocket(parent)
{
    //初始化数据
    m_ip=QString(),m_port=0,m_timeout=3000,m_xml=QByteArray(),size=0,m_myqq=QString();hasRead=false;
    connect(this,&SendSocket::connected,this,&SendSocket::writeMessage);
    connect(this,QOverload<QAbstractSocket::SocketError>::of(&SendSocket::error),this,&SendSocket::occuredErr);
    connect(this,&SendSocket::bytesWritten,&loop,[=](qint64 bytes){
        qDebug()<<"bytes writen size:"<<bytes;
        loop.quit();
    },Qt::DirectConnection);
    connect(this,&SendSocket::readyRead,this,&SendSocket::readResult);
}

SendSocket::~SendSocket()
{
    qDebug()<<"~SendSocket()";
}

void SendSocket::setIP(QString &ip)
{
    m_ip=ip;
}

void SendSocket::setIP(const QString &ip)
{
    m_ip=ip;
}

void SendSocket::setPort(quint16 &port)
{
    m_port=port;
}

void SendSocket::setPort(const quint16 &port)
{
    m_port=port;
}



void SendSocket::setMessage(QByteArray xml)
{
    m_xml=xml;
}

void SendSocket::setTimeout(int ms)
{
    m_timeout=ms;
}

void SendSocket::setNumber(QString &sender, QString &receiver)
{
    m_myqq=sender;
    m_number=receiver;
}

void SendSocket::post()
{
    if(m_ip.isEmpty()||m_port<=1000){
     emit finished(-5, QString("ip or port of opposite host is empty may be cause by opposite offline "));
        return;
    }
    this->connectToHost(m_ip,m_port);

}


void SendSocket::writeMessage()
{
    qDebug()<<"writen message";

    QTimer::singleShot(m_timeout,this,[=](){
        hasRead=true;
        QString temp=QString("conection is timeout %1 ms").arg(m_timeout);
        emit finished(-2,temp);
    });

    QJsonObject obj;
    obj.insert("content",QJsonValue("fmessage"));
    obj.insert("size",QJsonValue(m_xml.size()));
    obj.insert("myqq",QJsonValue(m_myqq));
    obj.insert("number",QJsonValue(m_number));
    QJsonDocument json(obj);
    QByteArray size;
    QDataStream stream(&size,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    quint8 l=json.toJson().size();
    stream<<l;
    this->write(size+json.toJson());
    this->write(m_xml);

    //不能二次z阻塞？
    //不知道为啥 用全局变量+断开连接就立即执行函数了，感觉阻塞了，但没有数据到来
    // loop.exec();
   // loop.exec();
    //原因：对方事件圈非直接调用 改为直接连接确保马上调用


    QTimer::singleShot(15000,this,[=](){
        if(!hasRead)
       emit finished(-4,"opposite don't react");
    });

}

void SendSocket::occuredErr(QAbstractSocket::SocketError code)
{
    hasRead=true;
    QString des;
    switch (code) {
    case QAbstractSocket::ConnectionRefusedError:
        des="The connection was refused by the peer (or timed out).";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        des="The remote host closed the connection.";
        break;
    case QAbstractSocket::HostNotFoundError:
        des="The host address was not found.";
        break;
    case QAbstractSocket::SocketAccessError:
        des="The socket operation failed because the application lacked the required privileges.";
        break;
    case QAbstractSocket::NetworkError:
        des="network error.";
        break;
    case    QAbstractSocket::UnknownSocketError:
        des="An unidentified error occurred.";
        break;
    default:
        des="a unknow error";
    }
    emit finished(-1,des);
}

void SendSocket::readResult()
{
    while (bytesAvailable()>0&&bytesAvailable()>=size) {
        if(size==0){
            QByteArray length=this->read(1);
            QDataStream stream(&length,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            if(l<=0){
                emit finished(-3,"l is less than zero");
                return;
            }
            size=l;
        }
        QByteArray data=this->read(size);
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(doc.isObject()){
             qDebug()<<"read";
            hasRead=true;
            QJsonObject obj=doc.object();
            QString result=obj.value("result").toString();
            if(result=="false"){
                emit finished(-3,"message sending failure  may be cause by data loss");
                size=0;
                return;
           //连接到服务器
            }else {
                emit finished(0,"message sent successfully");
                size=0;
                return;
            }
        }
    }
}
