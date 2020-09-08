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
    connect(this,&NativeSocket::bytesWritten,&loop,&QEventLoop::quit,Qt::DirectConnection);//ע���¼�Ȧ�˳� ����ֱ��������ȷ�������˳�
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
    //ʧ��
    if(!ok){
        obj.insert("result",QJsonValue("false"));
        json.setObject(obj);
        quint8 l=json.toJson().size();
        stream<<l;
        this->write(size+json.toJson());
        loop.exec();
        emit finished();//����
    }else{
        obj.insert("result",QJsonValue("true"));
        json.setObject(obj);
        quint8 l=json.toJson().size();
        stream<<l;
        this->write(size+json.toJson());
        loop.exec();
        QString html=QString();
        QString time=QString();//timeΪʱ��;;��Ϣ�б���ʾ
        //����������Ϣ��¼
        if(!parseXml(xml,html,time)){
            qDebug()<<"message saveing is of failure";
            return;
        };
        emit emitFMessage(html,number,time);//������Ϣ��qml
    }
}

bool NativeSocket::parseXml(QByteArray &bytes, QString& html,QString&time)
{
    //�����ݿ�
    qDebug()<<"byte"<<bytes.size();
    QString tid;
    std::thread::id id=std::this_thread::get_id();
    std::stringstream sin;

    sin << id;
    tid=QString::fromStdString(sin.str());

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",tid+number);
    QDir dir(QString("../user/%1/friends/%2/chat/").arg(myqq).arg(number));
     dir.mkpath("./temp");//ͼƬ·�� ����������·��
    db.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(myqq).arg(number).arg(number));//���ݿ���_�Է�����
    db.setHostName("MyQQ");
    db.setUserName("sa");
    db.setPassword("@123456x");
    if(db.open())
        qDebug()<<("��sqliite���ݿ�ɹ���");
    else {
        qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db.databaseName());
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
        //ע��autoincrementֻ��Integer�ҹ�
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
                            db.close();//�ǵùر� ��ɾ���ļ�
                           return false;
                       }
    }
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.qualifiedName()=="��Ϣ"){
                QString infoTip=QString();//��Ϣ�б���ʾ
                time=reader.attributes().value("datetime").toString();
                qDebug()<<"message content finding ,sending time"<<time;
                //���ݴ���
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
                        //��ӽ�html
                        html+=text;
                        infoTip+=text;
                       }else if(type=="pixmap"){//��16����תΪԭ���ݱ���
                           QByteArray pix=QByteArray::fromHex(text.toUtf8());//ת��Ϊԭ����
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
                        //����ͼƬ url��ӽ�html
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
                        infoTip+="[ͼƬ]";
                       }
                    }else if(reader.isEndElement()){
                        if(reader.qualifiedName()=="��Ϣ")
                            break;
                    }
                }
                time+=";;"+infoTip;//����timeΪʱ��;;��Ϣ�б���ʾ
                break;
            }
        }
    }
    ok= query.exec(QString(" select count(*) from _%1info ").arg(number));
    if(!ok){
       qDebug()<<"query.exec(QString(\" select count(*) from _%1info \").arg(myqq)) is of failure";
        db.close();//�ǵùر� ��ɾ���ļ�
       return false;
    }
    try{
    query.next();
    int count=query.value(0).toInt();
    if(count<=0)throw "table is empty";
    ok= query.exec(QString(" update _%1info set adhesion=%2 ").arg(number).arg(false));
    if(!ok){
        qDebug()<<" update _%1info set adhesion=%2  failure";
         db.close();//�ǵùر� ��ɾ���ļ�
        return false;
    }
    }catch(_exception&e){
        qDebug()<<"a exception:"<<"type is "<<e.type<<"name is"<<e.name;
         db.close();//�ǵùر� ��ɾ���ļ�
        return false;
    }catch(...){
        qDebug()<<"a unknow excpetion,query's lat error is "<<query.lastError().text();
         db.close();//�ǵùر� ��ɾ���ļ�
        return false;
    }
 db.close();//�ǵùر� ��ɾ���ļ�
    return true;
}

void NativeSocket::readD()
{
    //����ж�

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
            //��֤��Ϣ
            if(content=="friend-verify"){
                size=obj.value("size").toInt();
                if(size==0){
                    qDebug()<<"a error:data size is not correct";
                    emit finished();
                    return;
                }
                IT=VerifyForFriend;
                continue;
                //��Ӻ���
            }else if(content=="addFriend"){
                QString t=obj.value("type").toString();
                if(t=="json"){
                    IT=VerifyJson;
                }else{
                    IT=VerifyImg;
                }
                size=obj.value("size").toInt();
                continue;
                //ǿ������
            }else if(content=="offline"){
                QString ip=obj.value("ip").toString();
                QString host=obj.value("host").toString();
                QString dt=obj.value("datetime").toString();
                emit emitOffline(ip,host,dt);
                emit finished();
                return;
                //�����Ϣ
            }else if(content=="fmessage"){
                number=obj.value("number").toString();//���շ� myself����
                ok=true;//����Ϊok
                size=obj.value("size").toInt(-1);
                if(size==-1){
                    qDebug()<<"message size is negative";
                    ok=false;
                    getFMessage(bytes);
                    size=0;
                    return;
                }
                //���벻һ��
                if(number!=myqq){
                    ok=false;
                    qDebug()<<"number is not corrected";
                }
                number=obj.value("myqq").toString();//���ͷ�����
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
            //ԭ���¼�Ȧ��ֱ�ӵ��� ��Ϊֱ������ȷ�����ϵ���
            bytes.append(data);//�����ݽ���ʱ��ò�Ҫ�ý��ܵ���ʱ���� ��֪��Ϊɶ�Ͽ���ִ�к���
            getFMessage(bytes);

          //  disconnectFromHost();//���õ�Ϊɶ һ�µȴ� ���ﲻ�ܹر� һ�رվ���Ϊ����ʧ�� �ͷ��ͷ���� ���ҹر���Ҳ��������ִ��
            size=0;
            IT=NoAnything;

            break;
        default:
            break;
        }
    }
}
