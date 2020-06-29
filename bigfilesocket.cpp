#include "bigfilesocket.h"
#include<qdebug.h>
#include<QJsonDocument>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include<qjsonobject.h>
#include <qimage.h>
#include<qdir.h>
BigFileSocket::BigFileSocket(QObject *parent)
    :QTcpSocket(parent),instruct(QJsonDocument())
{
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this ,SLOT(err(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(readyRead()),this ,SLOT(readD()));
    connect(this,SIGNAL(connected()),this ,SLOT(writeD()));
    connect(this,SIGNAL(start()),this ,SLOT(post()));
    connect(this,SIGNAL(result(int)),this ,SLOT(resultSlot(int)));

    //初始化temp，用来保存分批到来的数据
    temp=QByteArray();
    size=0;
}

void BigFileSocket::setInstruct(const QString &arg)
{
    QStringList l=arg.split(" ",QString::SkipEmptyParts);
    QJsonObject o;
    o.insert("instruct",QJsonValue(l.at(0)));
    o.insert("content",QJsonValue(l.at(1)));
    o.insert("myqq",QJsonValue(l.at(2)));
    myqq=l.at(2);
    //建立目录
    QDir mk;
    QString dir;
    qDebug()<<"has writen headImg";
    dir="../user/"+myqq+"/friends";
    mk.mkpath(dir);
    if(!instruct.isEmpty())
        instruct=QJsonDocument();
    instruct.setObject(o);
}

void BigFileSocket::setIp(const QString &arg)
{
    ip=arg;
}

void BigFileSocket::setPort(const quint16 &arg)
{
    port=arg;
}


bool BigFileSocket::writeImg(const QByteArray &content, const QString &filepath, const char *format)
{

    QImage img;
    qDebug()<<"content";
    if(!img.loadFromData(content,format))
        return false;
    qDebug()<<"content1";
    if(!img.save(filepath,nullptr,0))
        return false;
    return true;
}

void BigFileSocket::readD()
{
    while (bytesAvailable()>=size&&bytesAvailable()>0) {
        if(size==0){
            QByteArray data=this->read(1);
            QDataStream stream(&data,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            size=l;
            if(l<=0){
                qDebug()<<"size is less than or equal to zero";
                continue;
            }
        }
        QByteArray data=this->read(size);
        qDebug()<<data.data();
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(doc.isObject()){
            QJsonObject obj=doc.object();
            qDebug()<<obj;
            if(obj.value("instruct").toString()=="20"){
                qDebug()<<"obj"<<obj.value("result").toString();
                if(obj.value("result").toString()=="writing"){ //写头像
                    m_write=HeadImg;
                    size=0;
                    qDebug()<<"writing"<<m_write<<size;
                    continue;
                }else if(obj.value("result").toString()=="true"){ //写头像成功
                    m_write=NoFile;//初始化标记枚举
                    size=0;
                    emit result(0);
                    return;
                }else if(obj.value("result").toString()=="false"){ //写头像失败
                    m_write=NoFile;
                    emit result(-1);
                    return;
                }
            }else if(m_write==HeadImg){
                imgName=obj.value("name").toString();
                qDebug()<<"img "<<imgName;
                size=obj.value("size").toVariant().toLongLong();
                if(size<=0){
                    qDebug()<<"size is less than or equal to zero";
                    emit result(-1);
                }
                continue;
            }
        }
        switch (m_write) {
        case HeadImg://传IMG
             img.insert(imgName,data);
            size=0,imgName="";
            break;
        default:
            break;
        }
    }
}

void BigFileSocket::writeD()
{
    this->write(instruct.toBinaryData());
    this->waitForBytesWritten();
}

void BigFileSocket::err(QAbstractSocket::SocketError code)
{
    switch (code) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<"The connection was refused by the peer (or timed out).";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<"The remote host closed the connection.";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"The host address was not found.";
        break;
    case QAbstractSocket::SocketAccessError:
        qDebug()<<"The socket operation failed because the application lacked the required privileges.";
        break;
    case QAbstractSocket::NetworkError:
        qDebug()<<"network error.";
        break;
    case    QAbstractSocket::UnknownSocketError:
        qDebug()<<"An unidentified error occurred.";
        break;
    }
    emit result(-1);
}

void BigFileSocket::post()
{
    this->connectToHost(ip,port);
}

void BigFileSocket::resultSlot(int code)
{

    if(code==-1){
        emit finished(-1);
        return;
    }
    foreach(QString key,img.keys()){
        QByteArray data=img.value(key);
        qDebug()<<data;
        if(!data.isEmpty()){
            qDebug()<<key;
            writeImg(data,"../user/"+myqq+"/friends/"+key+".png","PNG");
        }
    }
    emit finished(code);
}
