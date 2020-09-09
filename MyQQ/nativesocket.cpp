#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "nativesocket.h"
#include<qjsonobject.h>
#include <qdatastream.h>
#include<QXmlStreamReader>
#include<sstream>
#include<thread>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include<qdir.h>
#include<qthread.h>
#include<QSqlError>
NativeSocket::NativeSocket(QObject *parent)
    :QTcpSocket(parent)
{
    size=0;number=QString();ok=true;
    IT=NoAnything;
    connect(this,&NativeSocket::readyRead,this,&NativeSocket::readD);
    connect(this,&NativeSocket::bytesWritten,&loop,&QEventLoop::quit,Qt::DirectConnection);//注意事件圈退出 必须直接连接以确保马上退出
}

NativeSocket::~NativeSocket()
{
    if(this->isOpen()){
        this->close();
    }
    qDebug()<< "~NativeSocket()";
}

void NativeSocket::setMyQQ(QString &mq)
{
    myqq=mq;
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

void NativeSocket::getFMessage(QByteArray& xml)
{
    QJsonDocument json;
    QJsonObject obj;
    QByteArray size;
    QDataStream stream(&size,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    //失败
    if(!ok){
        obj.insert("result",QJsonValue("false"));
        json.setObject(obj);
        quint8 l=json.toJson().size();
        stream<<l;
        this->write(size+json.toJson());
        loop.exec();
        emit finished();//结束
    }else{
        obj.insert("result",QJsonValue("true"));
        json.setObject(obj);
        quint8 l=json.toJson().size();
        stream<<l;
        this->write(size+json.toJson());
        loop.exec();
        QString html=QString();
        QString time=QString();//time为时间;;消息列表提示
        //解析保存消息记录
        if(!parseXml(xml,html,time)){
            qDebug()<<"message saveing is of failure";
            return;
        };
        emit emitFMessage(html,number,time);//发送消息到qml
    }
}

bool NativeSocket::parseXml(QByteArray &bytes, QString& html,QString&time)
{
    //打开数据库
    qDebug()<<"byte"<<bytes.size();
    QString tid;
    std::thread::id id=std::this_thread::get_id();
    std::stringstream sin;

    sin << id;
    tid=QString::fromStdString(sin.str());

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",tid+number);
    QDir dir(QString("../user/%1/friends/%2/chat/").arg(myqq).arg(number));
     dir.mkpath("./temp");//图片路径 必须先制造路径
    db.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(myqq).arg(number).arg(number));//数据库名_对方号码
    db.setHostName("MyQQ");
    db.setUserName("sa");
    db.setPassword("@123456x");
    if(db.open())
        qDebug()<<("打开sqliite数据库成功！");
    else {
        qDebug()<<("打开sqliite数据库失败！名字:")<<(db.databaseName());
        return false;
    }
    QSqlQuery query(db);
   bool ok=  query.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1info' ").arg(number));
    if(!ok){
        qDebug()<<"count of table query is of failure ";
        return false;
    }
    query.next();
    if(query.value(0).toInt()==0){
        //注意autoincrement只与Integer挂钩
        ok= query.exec(QString( " create table _%1info (\
                                id integer not null primary key  autoincrement,\
                                data BLOD not null,\
                                datetime datetime not null,\
                                type text not null,\
                                adhesion bit default 0,\
number text not null\
                                )").arg(number));
                       if(!ok){
                           qDebug()<<"create a table is of failure";
                            db.close();//记得关闭 好删除文件
                           return false;
                       }
    }
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.qualifiedName()=="消息"){
                QString infoTip=QString();//消息列表提示
                time=reader.attributes().value("datetime").toString();
                qDebug()<<"message content finding ,sending time"<<time;
                //内容处理
                while(!reader.atEnd()){
                    reader.readNext();
                    if(reader.isStartElement()){
                        QString name=reader.qualifiedName().toString();
                        qDebug()<<"information qualified name:"<<name;
                        QString type=reader.attributes().value("type").toString();
                        QString text=reader.readElementText();
                       if(type=="text"){
                           query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(number));
                         query.addBindValue(QVariant(text));
                         query.addBindValue(QVariant(time));
                         query.addBindValue(QVariant(type));
                         query.addBindValue(QVariant(true));
                         query.addBindValue(QVariant(number));
                         ok=query.exec();
                        if(!ok){
                            qDebug()<<"type:text;query.exec(\" insert into _%1info(data,datetime,type,adhesion) values(?,?,?,?) \")=false";
                            continue;
                        }
                        //添加进html
                        html+=text;
                        infoTip+=text;
                       }else if(type=="pixmap"){//把16进制转为原数据保存
                           QByteArray pix=QByteArray::fromHex(text.toUtf8());//转化为原数据
                         query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(number));
                         query.addBindValue(QVariant(pix));
                         query.addBindValue(QVariant(time));
                         query.addBindValue(QVariant(type));
                         query.addBindValue(QVariant(true));
                         query.addBindValue(QVariant(number));
                         ok=query.exec();
                        if(!ok){
                            qDebug()<<"type:pixmap; query.exec(\" insert into _%1info(data,datetime,type,adhesion) values(?,?,?,?) \")=false";
                            continue;
                        }
                        //保存图片 url添加进html
                        QPixmap img;
                        img.loadFromData(pix);
                        if(img.isNull()){
                            qDebug()<<"message image saving failure";
                            continue;
                        }
                        QString nid=QString("%1").arg(time.toUInt())+tid;
                        nid=dir.absoluteFilePath("./temp/"+nid);
                        if(!img.save(nid,"png")){
                            qDebug()<<"message image saving failure";
                            continue;
                        }
                        int width=img.width(), height=img.height();
                        if(width>height){
                            height=height*1.0/width*290;
                            width=290;
                        }else{
                            height=width*1.0/ height*290;
                            height=290;
                        }
                        html+= "<img src=\"file:///"+nid+"\" width=\""+QString::number(width)+
                                "\" height=\""+QString::number(height)+"\" />";
                        infoTip+="[图片]";
                       }
                    }else if(reader.isEndElement()){
                        if(reader.qualifiedName()=="消息")
                            break;
                    }
                }
                time+=";;"+infoTip;//最终time为时间;;消息列表提示
                break;
            }
        }
    }
    ok= query.exec(QString(" select count(*) from _%1info ").arg(number));
    if(!ok){
       qDebug()<<"query.exec(QString(\" select count(*) from _%1info \").arg(myqq)) is of failure";
        db.close();//记得关闭 好删除文件
       return false;
    }
    try{
    query.next();
    int count=query.value(0).toInt();
    if(count<=0)throw "table is empty";
    ok= query.exec(QString(" update _%1info set adhesion=%2 ").arg(number).arg(false));
    if(!ok){
        qDebug()<<" update _%1info set adhesion=%2  failure";
         db.close();//记得关闭 好删除文件
        return false;
    }
    }catch(_exception&e){
        qDebug()<<"a exception:"<<"type is "<<e.type<<"name is"<<e.name;
         db.close();//记得关闭 好删除文件
        return false;
    }catch(...){
        qDebug()<<"a unknow excpetion,query's lat error is "<<query.lastError().text();
         db.close();//记得关闭 好删除文件
        return false;
    }
 db.close();//记得关闭 好删除文件
    return true;
}

void NativeSocket::readD()
{
    //检查中断

    if(this->thread()->isInterruptionRequested()){
        this->thread()->exit(0);
        this->thread()->quit();
        return;
    }

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
                    qDebug()<<"a error:data size is not correct";
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
                return;
                //获得消息
            }else if(content=="fmessage"){
                number=obj.value("number").toString();//接收方 myself号码
                ok=true;//重置为ok
                size=obj.value("size").toInt(-1);
                if(size==-1){
                    qDebug()<<"message size is negative";
                    ok=false;
                    getFMessage(bytes);
                    size=0;
                    return;
                }
                //号码不一致
                if(number!=myqq){
                    ok=false;
                    qDebug()<<"number is not corrected";
                }
                number=obj.value("myqq").toString();//发送方号码
                if(number.isEmpty()){
                    ok=false;
                    qDebug()<<"number is empty";
                }
                IT=FMessage;
                continue;
            }
        }
        switch (IT) {
        case VerifyForFriend:
            bytes=data;
            this->getFVerify(bytes);
             disconnectFromHost();
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
             disconnectFromHost();
            size=0;
            IT=NoAnything;
            break;
        case FMessage:
            //原因：事件圈非直接调用 改为直接连接确保马上调用
            bytes.append(data);//在数据接受时最好不要用接受的临时数据 不知道为啥断开才执行函数
            getFMessage(bytes);

          //  disconnectFromHost();//布置到为啥 一致等待 这里不能关闭 一关闭就认为发送失败 就发送服务端 而且关闭了也不会立即执行
            size=0;
            IT=NoAnything;

            break;
        default:
            break;
        }
    }
}
