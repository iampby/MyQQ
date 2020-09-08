#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include"tcpsocket.h"
#include <QDir>
#include<qimage.h>
QString TcpSocket::m_myqq=QString();
QString TcpSocket::m_beginId=QString();

TcpSocket::TcpSocket(QObject *parent)//构造函数初始化
    :QTcpSocket(parent),instruct(QJsonObject()),cityData(QString()),flags(qint32()),
      size(qint64()),jsonDoc(QJsonDocument()),fileName(QString()),fileData(QByteArray())
{
 this->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows必设
    connect(this,&TcpSocket::connected,this,&TcpSocket::writeD);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readD);
}

bool TcpSocket::writeImg(const QByteArray &content, const QString &filepath, const char *format)
{
QImage img;
QString str=filepath.left(filepath.lastIndexOf("/"));
QDir dir;
dir.mkpath(str);
if(!img.loadFromData(content,format))
    return false;
    qDebug()<<" a image was constructed successfully";
if(!img.save(filepath,nullptr,0))
    return false;
    qDebug()<<"saved IMG";
return true;
}

void TcpSocket::setMyqq(const QString &arg)
{
    m_myqq=arg;
}

void TcpSocket::setBeginId(const QString &arg)
{
    m_beginId=arg;
}

QString TcpSocket::myqq()
{
return m_myqq;
}

QString TcpSocket::beginId()
{
return m_beginId;
}

void TcpSocket::setInstruct(const QJsonObject &arg)
{
instruct=arg;
}

QJsonObject &TcpSocket::getInstruct()
{
return instruct;
}

void TcpSocket::readD()
{
    while (this->bytesAvailable()>=size&&this->bytesAvailable()>0) {
        if(size==0){
            QByteArray data=this->read(1);
            QDataStream stream(&data,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
           stream>>l;
           if(l<=0){
               return;
           }
           size=l;
        }
        QByteArray data=this->read(size);
        QJsonDocument json=QJsonDocument::fromJson(data);
        if(json.isObject()){
            QJsonObject obj=json.object();
            QString content=obj.value("content").toString();
            QString in=obj.value("instruct").toString();
           //废弃 使用了本地sqlite
            if(content=="city-data"){
                if(in=="30"){
                    size=0;
                    if(obj.value("result")=="true"){
                        flags=0;
                        emit getCityModel();
                        return;
                    }else{
                        flags=0;
                        emit failed();
                        return;
                    }
                }else if(in=="31"){
                    bool ok=false;
                    size=obj.value("size").toVariant().toLongLong(&ok);
                    if(!ok)
                        emit failed();
                    flags=11;
                  continue;
                }
            }else if(content=="find-person"){
                if(in=="30"){
                    size=0;
                    if(obj.value("result")=="true"){
                        flags=0;
                        emit finished();
                        return;
                    }else{
                        flags=0;
                        emit failed();
                        return;
                    }
                }else if(in=="31"){
                    bool ok=false;
                    size=obj.value("size").toVariant().toLongLong(&ok);
                    if(!ok)
                        emit failed();
                    m_beginId=obj.value("begin-id").toString();
                    flags=21;
                    continue;
                }else if(in=="32"){
                    fileName="";
                    fileData=QByteArray();
                    bool ok=false;
                    size=obj.value("size").toVariant().toLongLong(&ok);
                    if(!ok)
                        emit failed();
                    fileName="./images/"+obj.value("imgname").toString();
                    flags=22;
                   continue;
                }
            }
        }
        switch (flags) {
        case 11:
            cityData.append(data);
            size=0;
            break;
        case 21:
           jsonDoc=json;
           size=0;
            break;
        case 22:
            fileData.append(data);
            if(!writeImg(fileData,fileName,"png"))
                qDebug()<<"saving the image had failed on "<<fileName;
           size=0;
            break;
        default:
            break;
        }
    }
}

void TcpSocket::writeD()
{
QJsonDocument temp;
temp.setObject(instruct);
this->write(temp.toBinaryData());
qDebug()<<"has write";
}
