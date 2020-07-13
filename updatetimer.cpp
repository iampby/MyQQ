#include "UpdateTimer.h"
#include<qdebug.h>
#include<qtimer.h>
#include<qthread.h>
#include<qtcpsocket.h>
#include<qeventloop.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qdatastream.h>
#include <qpixmap.h>
#include <qjsonarray.h>
UpdateTimer::UpdateTimer(QObject *parent)
    :QObject(parent)
{
    size=0;r_type=NoType;number=QString();
    //父类对象管理子类对象的内存
    t=new QThread(this);
    timer=new QTimer(this);
    tcpsocket=new QTcpSocket(this);
    loop=new QEventLoop(this);
    moveToThread(t);
    t->start();
    connect(timer,&QTimer::timeout,timer,[=](){
        size=0;
        number="";
        startTcpScoket();
    });
    connect(tcpsocket,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    connect(tcpsocket,&QTcpSocket::connected,this,&UpdateTimer::writeInstruct);
    connect(tcpsocket,&QTcpSocket::readyRead,this,&UpdateTimer::readD);
    connect(tcpsocket,&QTcpSocket::disconnected,[=](){qDebug()<<"disconnection";});
    connect(this,&UpdateTimer::emitResult,this,[=](){
        tcpsocket->disconnectFromHost();
        qDebug()<<"tcpsocket->disconnectFromHost()";
    });
    //直接调用子线程槽
    connect(this,&UpdateTimer::stopTimer,this,[=](){
        qDebug()<<"stopTimer";
        stop();
    },Qt::BlockingQueuedConnection);
    connect(this,&UpdateTimer::startTimer,this,[=](){
        qDebug()<<":startTimer";
        start();
    },Qt::BlockingQueuedConnection);
}

UpdateTimer::~UpdateTimer()
{
    qDebug()<<"~UpdateTimer()";
}

void UpdateTimer::setMyqq(const QString &arg)
{
    myqq=arg;
}

void UpdateTimer::stop()
{
    timer->stop();
    if(t->isRunning())t->quit();
}

void UpdateTimer::start()
{
    timer->start();
    if(!t->isRunning())t->start();
    qDebug()<<"timer->start()";
}

void UpdateTimer::setTimerInterval(const int &t)
{
    timer->setInterval(t);
}

void UpdateTimer::startTcpScoket()
{
    tcpsocket->connectToHost(ip,port);
}

void UpdateTimer::setIp(const QString &arg)
{
    ip=arg;
}

void UpdateTimer::setPort(const quint16 &arg)
{
    port=arg;
}

void UpdateTimer::splitSignatureAndName(QByteArray &data)
{
    QJsonDocument json=QJsonDocument::fromJson(data);
    if(!json.isObject()){
        qDebug()<<"json is not object";
        return;
    }
    QJsonObject obj=json.object();
    QJsonArray arr1=obj.value("signature").toArray(QJsonArray());
    QJsonArray arr2=obj.value("name").toArray(QJsonArray());
    if(!arr1.isEmpty()){
        qDebug()<<"signature is got";
        for (int var = 0; var < arr1.size(); ++var) {
            QJsonObject temp=arr1.at(var).toObject(QJsonObject());
            QString num=temp.value("number").toString(),v=temp.value("signature").toString();
            if(num.isEmpty()){
                qDebug()<<"warning:a signature is null";
                continue;
            }
            sigMap.insert(num,v);
        }
    }
    if(!arr2.isEmpty()){
        qDebug()<<"name is got";
        for (int var = 0; var < arr2.size(); ++var) {
            QJsonObject temp=arr2.at(var).toObject(QJsonObject());
            QString num=temp.value("number").toString(),v=temp.value("name").toString();
            if(num.isEmpty()){
                qDebug()<<"warning:a name is null";
                continue;
            }
            sigMap.insert(num,v);
        }
    }
    qDebug()<<"signature and name is arranged";
}

void UpdateTimer::makePixmap(const QByteArray &data)
{
    QPixmap headPix;
    if(!number.isEmpty()){
        if(headPix.loadFromData(data,"png")){
            qDebug()<<"image of head is loaded";
            historyMap.insert(number,headPix);
        }
        else qDebug()<<"warning:image of head is not loaded";
    }
    else qDebug()<<"warning:number is empty";
}

void UpdateTimer::writeInstruct()
{
    qDebug()<<"UpdateTimer:writeInstruct()";
    QJsonDocument json;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("6"));
    obj.insert("content",QJsonValue("updateUser"));
    obj.insert("myqq",QJsonValue(myqq));
    json.setObject(obj);
    tcpsocket->write(json.toBinaryData());
    qDebug()<<"pre loop->exec()";
    loop->exec();
    qDebug()<<"aft loop->exec()";
}

void UpdateTimer::readD()
{
    while (tcpsocket->bytesAvailable()>0&&tcpsocket->bytesAvailable()>=size) {
        if(size==0){
            QByteArray data=tcpsocket->read(1);
            QDataStream stream(&data,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            qDebug()<<"l="<<l;
            size=l;
            if(l<=0){
                qDebug()<<"size is less than or equal to zero";
            }
            continue;
        }
        QByteArray data=tcpsocket->read(size);
        qDebug()<<"dispatched size is equal to "<<size;
        QJsonDocument jsonDoc=QJsonDocument::fromJson(data);
        if(jsonDoc.isObject()){
            qDebug()<<"object:"<<jsonDoc.toJson();
            QJsonObject obj=jsonDoc.object();
            QString content=obj.value("content").toString();
            //更新头像
            if(content=="headImg"){
                qDebug()<<"headImg:";
                size=obj.value("size").toInt();
                if(size==0)qDebug()<<"warning:toInt() is 0";
                number=obj.value("number").toString();
                r_type=HeadImage;
                continue;
            }else if(content=="signatureAndName"){
                qDebug()<<"signatureAndName:";
                size=obj.value("size").toInt();
                if(size==0)qDebug()<<"warning:toInt() is 0";
                r_type=SignatureAndName;
                continue;
            }else if(content=="end"){
                QString ok=obj.value("result").toString();
                if(ok=="true"){
                    qDebug()<<"finishing updating";
                    emit emitResult(true);
                }else{
                    qDebug()<<"unfinished updating";
                    emit emitResult(false);
                }
            }

        }
        switch (r_type) {
        //接收头像
        case HeadImage:
            makePixmap(data);
            size=0;
            number="";
            r_type=NoType;
            break;
            //个性签名处理和昵称
        case SignatureAndName:
            splitSignatureAndName(data);
            size=0;
            number="";
            r_type=NoType;
            break;
        default:
            break;
        }
    }
}
