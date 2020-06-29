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
        qDebug()<<QStringLiteral("���ӱ��ܽӻ�ʱ��")<<endl;
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<QStringLiteral("Զ�������ѹرգ�")<<endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<QStringLiteral("����û�з��֣�")<<endl;
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<QStringLiteral("δ֪����")<<endl;
        break;
    default:
        qDebug()<<QStringLiteral("�����˴���")<<endl;
    }
}


