#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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
    size=0;r_type=NoType;number=QString();isReceiving=false;grade=-1;ads="0";
    //父类对象管理子类对象的内存
    t=new QThread(this);
    timer=new QTimer(this);
    tcpsocket=new QTcpSocket(this);
    loop=new QEventLoop(this);
    moveToThread(t);
    t->start();
    connect(timer,&QTimer::timeout,this,&UpdateTimer::immediateGet);
    connect(tcpsocket,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    connect(tcpsocket,&QTcpSocket::connected,this,&UpdateTimer::writeInstruct);
    connect(tcpsocket,&QTcpSocket::readyRead,this,&UpdateTimer::readD);
    //连接失败
    connect(tcpsocket,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,[=](QAbstractSocket::SocketError code){
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
        isReceiving=false;//重置
    });
    //断开连接
    connect(tcpsocket,&QTcpSocket::disconnected,this,[=](){
        isReceiving=false;//重置
    });
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
    //立即更新
    connect(this,&UpdateTimer::update,this,&UpdateTimer::immediateGet);
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
    if(t->isRunning()){
        t->requestInterruption();
        t->exit();
        t->quit();
        t->wait();
    }
    isReceiving=false;
    qDebug()<<"update timer is stoped";
}

void UpdateTimer::start()
{
    timer->start();//注意定时器开启和结束线程要一致
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

void UpdateTimer::immediateGet()
{
    if(isReceiving){//如果上一次的没完就直接返回
        return;
    }
    isReceiving=true;
    size=0;
    number="";
    startTcpScoket();
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

void UpdateTimer::handleStatus(const QByteArray &bytes)
{
    qDebug()<<"in the proccess of  status data";
    QJsonDocument json=QJsonDocument::fromJson(bytes);
    if(json.isObject()){
        QJsonObject obj=json.object();
        if(obj.isEmpty()){
            qDebug()<<"status object is empty";
            return;
        }
        statusMap=obj.toVariantMap();
    }
}

void UpdateTimer::handleFGrade(const QByteArray &bytes)
{
    QJsonDocument json=QJsonDocument::fromJson(bytes);
    QJsonObject obj=json.object();
    if(obj.isEmpty()){
        qDebug()<<"warning:got a friends'grade data is empty";
        return;
    }
    fgrade=obj.toVariantMap();
}

void UpdateTimer::writeInstruct()
{
    if(t->isInterruptionRequested()){
        t->exit(0);
        t->quit();
        t->wait();
        return;
    }
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
    //检查中断操作操作
    if(t->isInterruptionRequested()){
        t->exit(0);
        t->quit();
        t->wait();
        return;
    }
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
                //更新个性签名和名字
            }else if(content=="signatureAndName"){
                qDebug()<<"signatureAndName:";
                size=obj.value("size").toInt();
                if(size==0)qDebug()<<"warning:toInt() is 0";
                r_type=SignatureAndName;
                continue;
                //更新好友状态
            }else if(content=="status"){
                qDebug()<<"status:";
                size=obj.value("size").toInt();
                if(size==0)qDebug()<<"warning:toInt() is 0";
                r_type=Status;
                continue;
            } else if(content=="mygrade"){
                r_type=NoType;
                bool isNeedUpdate=obj.value("isNeedUpdate").toBool();
                 ads=obj.value("activeDays").toString();
                grade=-1;
                if(isNeedUpdate){
                   grade=obj.value("grade").toInt(-1);
                    if(grade==-1){
                        qDebug()<<"the grade is not normal";
                        continue;
                    }
                }

            } else if(content=="friendsgrade"){
               r_type=FGrade;
                size=obj.value("size").toInt(-1);
                if(size==-1){
                    qDebug()<<"got a file size is equal to negative";
                   r_type=  NoType;
                   continue;
                }
            }
            else if(content=="end"){
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
        case Status:
            handleStatus(data);
            size=0;
            number="";
            r_type=NoType;
            break;
        case FGrade:
            handleFGrade(data);
            size=0;
            number="";
            r_type=NoType;
            break;
        default:
            break;
        }
    }
}
