#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "registersocket.h"

RegisterSocket::RegisterSocket(const QString&name,const QString&passwd,QObject *parent)
    :QTcpSocket (parent),name(name),passwd(passwd)
{
    infoList=QStringList();
    connect(this,SIGNAL(readyRead()),this,SLOT(readD()));
    connect(this,SIGNAL(connected()),this,SLOT(writeD()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(err(QAbstractSocket::SocketError)));
}


void RegisterSocket::readD()
{
    qDebug()<<"client read?";
    QByteArray data;
    data=this->readAll();
    QString temp=QString::fromUtf8(data.data());
    infoList=temp.split(" ");
    emit finished();
}

void RegisterSocket::writeD()
{
    qDebug()<<"client write?";

    if(this->write(QString("0 "+name+" "+passwd).toUtf8().data())==-1){
    this->deleteLater();
    }
}

void RegisterSocket::err(QAbstractSocket::SocketError code)
{
    emit errOccured();
    switch (code) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<("连接被拒接或超时！")<<endl;
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<("远程主机已关闭！")<<endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<("主机没有发现！")<<endl;
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<("未知错误！")<<endl;
        break;
    default:
        qDebug()<<("出现了错误！")<<endl;
    }
}


