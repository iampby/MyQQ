#include "writethread.h"
#include <qsqlquery.h>
#include <qdir.h>
#include <qjsondocument.h>
#include<qjsonobject.h>
#include <qpixmap.h>
#include <qtimer.h>

WriteThread::WriteThread(qintptr socketDescriptor,  QObject *parent)
:QObject(parent),socketDescriptor(socketDescriptor)
{
    size=0;FT=NoFile;
    tcpsocket=new QTcpSocket(this);
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<QStringLiteral("创建套接字失败！");
        emit error(tcpsocket->error());
        return;
    }
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows必设
    connect(tcpsocket,&QTcpSocket::readyRead,this,&WriteThread::readD);
    connect(tcpsocket,&QTcpSocket::disconnected,this,&WriteThread::disconnected);
    connect(tcpsocket,&QTcpSocket::bytesWritten,this,[=](qint64 bytes){
        emit loopStop();
        qDebug()<<bytes<<" bytes has been readed";
    });
    QTimer::singleShot(200,Qt::CoarseTimer,this,[=](){
        qDebug()<<"timer 6000ms exit";
        emit finished();
        qDebug()<<"thread had exited";
    });
}

WriteThread::~WriteThread()
{
    qDebug()<<"~WriteThread()";
}

bool WriteThread::adjustHistoryImg(QByteArray &bytes, const QString &filePath, const QString &fileType)
{
    QPixmap head;
    if(!head.loadFromData(bytes,"png"))return false;
    QDir dir(filePath.left(filePath.lastIndexOf("/")));
    if(!dir.exists()||dir.isEmpty()){
        qDebug()<<"adjustHistoryImg:folder is empty or null";
        return false;
    }
    QStringList temp;temp<<"*.png";
    QFileInfoList info=dir.entryInfoList(temp,QDir::Files,QDir::Name);
    for (int i = info.length()-1; i >=0;--i) {
        QString name=info.at(i).fileName();
        qint8 two=name.left(2).toShort();
        QFile file(dir.filePath(name));
        if(two>=18)file.remove();
        ++two;
        bool ok=file.rename(dir.filePath(QString("%1.png").arg(two)));
        if(!ok)qDebug()<<"warning: alter file is of fuilure, named "<<name;
    }
    return head.save(dir.filePath("01.png"),"png");

}



void WriteThread::readD()
{
    while (tcpsocket->bytesAvailable()>size&&size>=0) {
        if(size==0){
            QByteArray header=tcpsocket->read(1);
            QDataStream stream(&header,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            if(l<=0){
                qDebug()<<"l="<<l;
                continue;
            }
            size=l;
        }
        if(size!=1){
            QByteArray data=tcpsocket->read(size);
            QJsonDocument jsondoc=QJsonDocument::fromBinaryData(data);
            if(jsondoc.isObject()){
                QJsonObject obj=jsondoc.object();
                QString in=obj.value("instruct").toString();
                if(in=="4"){
                    QString content=obj.value("content").toString();
                    if(content=="historyHeadImg"){
                       myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            tcpsocket->disconnectFromHost();
                            return;
                        }
                        fileName="../userData/"+myqq+"/historyHeadImg/01.png";
                        FT=Png;
                        size=1;
                        continue;
                    }
                }
            }
        }else{
            QByteArray data=tcpsocket->readAll();
            switch (FT) {
            case Png:
                png.append(data);
                break;
            default:
                return;
            }
        }
    }
}

void WriteThread::disconnected()
{
    qDebug()<<"  Write QTcpSocket disconnected";
    switch (FT) {
    case Png:
        if(adjustHistoryImg(png,fileName,"png")){
            qDebug()<<"added history image successfully";
        }
        break;
    default:
        break;
    }
    emit finished();
}
