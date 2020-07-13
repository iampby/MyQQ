#include "loginsocket.h"
#include<qbytearray.h>
#include<qdatastream.h>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include<QHostInfo>
LoginSocket::LoginSocket(const QString &myqq, const QString &passwd, QObject *parent)
    :QTcpSocket(parent),myqq(myqq),passwd(passwd),result(int(3)),flags(0),size(qint64())
{
    connect(this, SIGNAL(readyRead()),this,SLOT(readD()));
    connect(this, SIGNAL(connected()),this,SLOT(toWrite()));
    connect(this, SIGNAL(bytesWritten(qint64)),&loop,SLOT(quit()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(err(QAbstractSocket::SocketError)));
}

void LoginSocket::readD()
{
    while (bytesAvailable()>=size&&size>=0) {
        if(size==0){
            QByteArray data=this->read(1);
            QDataStream stream(&data,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            size=l;
            if(l<=0){
                qDebug()<<"size is less than zero,equal to "<<size;
               return;
            }
        }
        QByteArray data=this->read(size);
        QJsonDocument readJson=QJsonDocument::fromJson(data);
        if(readJson.isObject()){
            qDebug()<<readJson.toJson();
            QJsonObject obj=readJson.object();
            if(obj.value("instruct").toString()=="10"){
                flags=0;
                size=0;
            }else if(obj.value("instruct").toString()=="11"){//准备接受头像指令
                flags=1;
                size=obj.value("size").toVariant().toLongLong();
                if(size<=0){
                    emit  finished(3);//失败
                return;
                }
                continue;
            }else if(obj.value("instruct").toString()=="12"){//准备接受info.xml数据
                flags=2;
                size=obj.value("size").toVariant().toLongLong();
                if(size<=0){
                    emit  finished(3);//失败
                return;
                }
                continue;
            }
        }
        if(flags==0){
            int r=readJson.object().value("result").toInt(-1);
            qDebug()<<r;
            if(r==-1)
                emit finished(3);
            else{
                result=r;
                if(result==0)
                    infoObj=readJson.object();
                emit finished(result);
            }
            return;
        }else if(flags==1){//读取头像
            img=data;//头像
            flags=0,size=0;
        }else if(flags==2){//读取xml文件
            infoXml=data;
            flags=0,size=0;
        }
    }
}

void LoginSocket::toWrite()
{
    const QString& hostname=QHostInfo::localHostName();
    QHostInfo info=QHostInfo::fromName(hostname);

    QList<QHostAddress> addresses=info.addresses();
    if(addresses.isEmpty()){
        emit finished(result);
        return;
    }
    QHostAddress addr=addresses.first();
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("1"));
    obj.insert("myqq",QJsonValue(myqq));
    obj.insert("password",QJsonValue(passwd));
    obj.insert("hostname",QJsonValue(hostname));
    obj.insert("ip",QJsonValue(addr.toString()));
    QJsonDocument docJson(obj);
    this->write(docJson.toBinaryData());
    loop.exec();
}

void LoginSocket::err(QAbstractSocket::SocketError code)
{
    switch (code) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<QStringLiteral("连接被拒接或超时！")<<endl;
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<QStringLiteral("远程主机已关闭！")<<endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<QStringLiteral("主机没有发现！")<<endl;
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<QStringLiteral("未知错误！")<<endl;
        break;
    default:
        qDebug()<<QStringLiteral("出现了错误！")<<endl;
    }
    emit finished(result);
}

