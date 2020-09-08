#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "bigfilesocket.h"
#include<qdebug.h>
#include<QJsonDocument>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include<qjsonobject.h>
#include <qimage.h>
#include<qdir.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
BigFileSocket::BigFileSocket(QObject *parent)
    :QTcpSocket(parent),instruct(QJsonDocument())
{
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this ,SLOT(err(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(readyRead()),this ,SLOT(readD()));
    connect(this,SIGNAL(connected()),this ,SLOT(writeD()));
    connect(this,SIGNAL(start()),this ,SLOT(post()));
    connect(this,SIGNAL(result(int,QString,QString)),this ,SLOT(resultSlot(int,QString,QString)));
    connect(this,&QTcpSocket::bytesWritten,this,[=](qint64 bytes){
        qDebug()<<bytes<<" bytes has been readed";
        loop.quit();
        qDebug()<<" loop.quit()";
    });
    //��ʼ��temp�����������������������
    carrier=QByteArray();
    size=0;
    timeout=40000;//Ĭ��40s
}

BigFileSocket::~BigFileSocket()
{
    if(this->isOpen()){
        this->close();
    }
    qDebug()<<"~BigFileSocket()";
}

void BigFileSocket::setInstruct(const QString &obj)
{

    QStringList l=obj.split(" ",QString::SkipEmptyParts);
    QJsonObject o;
    try{
        if(l.length()>4)throw -1;
        else if(l.length()<=2)throw -2;
        o.insert("instruct",QJsonValue(l.at(0)));
        o.insert("content",QJsonValue(l.at(1)));
        o.insert("myqq",QJsonValue(l.at(2)));
    }catch(int &except){
        qDebug()<<"this list is overflow,code:"<<except;
    }catch(...){
        qDebug()<<"unkown exception in:32row:bigfilesocket";
    }
    myqq=l.at(2);
    if(l.length()==4){
        o.insert("headerSize",QJsonValue(true));
    }
    //����Ŀ¼
    QDir mk;
    QString dir;
    qDebug()<<"has writen headImg";
    dir="../user/"+myqq+"/friends";
    mk.mkpath(dir);
    if(!instruct.isEmpty())
        instruct=QJsonDocument();
    instruct.setObject(o);
}

void BigFileSocket::setInstruct(QJsonObject &obj)
{
    instruct.setObject(obj);
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
    QDir dir(filepath);
    dir.mkpath("../");
    if(!img.loadFromData(content,format)){
        qDebug()<<"loaded data is of failure";
        return false;
    }
    if(!img.save(filepath,format,0)){
        qDebug()<<"saved image  is of failure";
        return false;
    }
    return true;
}
//����Ŀ¼·��:dstr+number/chat
bool BigFileSocket::writeDB(const QByteArray &content, const QString &dstr,QString&name)
{
    QString number=name.mid(1,name.length()-4);
    QDir dir(dstr+number+"/chat");
    if(!dir.exists()){
        dir.mkpath("./");
    }else{
        QFile file(dir.filePath(name));
        //���������Ϣ���ݿ� ���½������ݿ�Ȼ��������ݵ��Ѵ������ݿ�
        if(file.exists()){
            QFile tfile(dir.filePath("temp.db"));
            if(tfile.open(QIODevice::WriteOnly)){
                tfile.write(content);
                tfile.close();//�����ļ�
                //�����ݿ�
                bool ok;
                QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",myqq);
                db.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(myqq,number,number));//���Ѵ������ݿ�
                db.setHostName("MyQQ");
                db.setUserName("sa");
                db.setPassword("@123456x");
                if(db.open())
                    qDebug()<<("��sqliite���ݿ�ɹ���");
                else {
                    qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db.databaseName());
                    return false;
                }
                QSqlQuery query(db);//�������ݿ�Ĳ�ѯ����
                QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",myqq+number);
                db1.setDatabaseName(QString("../user/%1/friends/%2/chat/temp.db").arg(myqq,number));//���½��յ����ݿ�
                db1.setHostName("MyQQ");
                db1.setUserName("sa");
                db1.setPassword("@123456x");
                if(db1.open())
                    qDebug()<<("��sqliite���ݿ�ɹ���");
                else {
                    qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db1.databaseName());
                    return false;
                }
                QSqlQuery query1(db1);//δ���յ���Ϣ���ݿ��ѯ����
                ok=   query1.exec(QString(" select* from _%1info").arg(number));
                if(!ok){
                    qDebug()<<"query is of failure to arriving information of database";
                    return false;
                }
                query1.next();
                /** ��ṹ
                                      id integer not null primary key  autoincrement,\
                                      data BLOD not null,
                                      datetime datetime not null,
                                      type text not null,
                                      adhesion bit default 0,
                                       number text not null
                                      */
                QString info=QString();//���ڱ������¼�¼
                bool last=false;//������һ���ǲ���adhesion=false
                QString time;//��¼ʱ��
                while (query1.isValid()) {
                    //��ȡ����
                    QByteArray data=query1.value("data").toByteArray();//blod��Ӧ��������Ϊbytearray
                    time=query1.value("datetime").toString();
                    QString type=query1.value("type").toString();
                    bool adhesion=query1.value("adhesion").toBool();
                    QString sendNmber=query1.value("number").toString();
                    if(adhesion){
                        //��һ���������yig��Ϣ ��Ϣ����
                        if(last){
                            info="";
                            last=!last;
                        }
                        if(type=="pixmap"){
                            info.append("[ͼ��]");
                        }else if(type=="text"){
                            info.append(data);
                        }
                    }else{
                        last=true;
                        if(type=="pixmap"){
                            info.append("[ͼ��]");
                        }else if(type=="text"){
                            info.append(data);
                        }
                    }
                    //�������ݵ��������ݿ�
                    query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(number));
                    query.addBindValue(QVariant(data));
                    query.addBindValue(QVariant(time));
                    query.addBindValue(QVariant(type));
                    query.addBindValue(QVariant(adhesion));
                    query.addBindValue(QVariant(sendNmber));
                    query1.next();//λ��ǰ��1
                }
                info=info.replace("<br />","");//ȥ�����л���
                info.append(";;"+time);//�����;;���ʱ��
                infoList.insert(number,info);//������Ϣ�б�
                 db1.close();
                db.close();//�ǵùر� ��ɾ���ļ�
            }else{
                qDebug()<<"opened a database failure";
                return false;
            }
            return true;
        }
    }
    //��������ڱ������ݿ⣬��ֱ�ӱ���
    QFile tfile(dir.filePath(QString("_%1.db").arg(number)));
    if(tfile.open(QIODevice::WriteOnly)){
        tfile.write(content);
        tfile.close();
        QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",myqq+number);
        db1.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(myqq,number,number));//���½��յ����ݿ�
        db1.setHostName("MyQQ");
        db1.setUserName("sa");
        db1.setPassword("@123456x");
        if(db1.open())
            qDebug()<<("��sqliite���ݿ�ɹ���");
        else {
            qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db1.databaseName());
            return false;
        }
        QSqlQuery query1(db1);//δ���յ���Ϣ���ݿ��ѯ����
        if(query1.exec(QString(" select * from  _%1info ").arg(number))){
            query1.next();
            try{
                QString info=QString();//���ڱ������¼�¼
                bool last=false;//������һ���ǲ���adhesion=false
                QString time=QString();
                while (query1.isValid()) {
                    //��ȡ����
                    QByteArray data=query1.value("data").toByteArray();//blod��Ӧ��������Ϊbytearray
                     time=query1.value("datetime").toString();
                    QString type=query1.value("type").toString();
                    bool adhesion=query1.value("adhesion").toBool();
                    QString sendNmber=query1.value("number").toString();
                    if(adhesion){
                        //��һ���������ĩβƬ����Ϣ ��Ϣ����
                        if(last){
                            info="";
                            last=!last;
                        }
                        if(type=="pixmap"){
                            info.append("[ͼ��]");
                        }else if(type=="text"){
                            info.append(data);
                        }
                    }else{
                        last=true;
                        if(type=="pixmap"){
                            info.append("[ͼ��]");
                        }else if(type=="text"){
                            info.append(data);
                        }
                    }

                    query1.next();//λ��ǰ��1
                }
                info=info.replace("<br />","");//ȥ�����л���
                info.append(";;"+time);
                infoList.insert(number,info);//������Ϣ�б�
            }catch(_exception&e){
                qDebug()<<"type:"<<e.type<<",name:"<<e.name;
            }
        }else{
            qDebug()<<"a friends message query failure";
             db1.close();//�ǵùر� ��ɾ���ļ�
            return false;
        }
 db1.close();//�ǵùر� ��ɾ���ļ�
    }else{
        qDebug()<<"a file not opening when the friend message was stored";
        return false;
    }
    return true;
}

void BigFileSocket::setTimeout(const qint64 &t)
{
    timeout=t;
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
            }
            continue;
        }
        QByteArray data=this->read(size);
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(doc.isObject()){
            QJsonObject obj=doc.object();
            qDebug()<<obj;
            //����ͷ��
            QString in=obj.value("instruct").toString();
            if(in=="20"){
                QString content=obj.value("result").toString();
                if(content=="writing"){ //д����ͷ��
                    m_write=Img;
                    imgName=obj.value("name").toString();
                    qDebug()<<"image name: "<<imgName;
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(content=="true"){ //дͷ��ɹ�
                    m_write=NoFile;//��ʼ�����ö��
                    size=0;
                    emit result(0,"../user/"+myqq+"/friends/","png");
                    return;
                }else if(content=="false"){ //дͷ��ʧ��
                    m_write=NoFile;
                    emit result(-1);
                    return;
                }else if(content=="writeDB"){
                    imgName=obj.value("dbName").toString();
                    size=obj.value("size").toVariant().toLongLong();
                    if(imgName.isEmpty()){
                        qDebug()<<"information database name is empty";
                        emit result(-1);
                    }
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    m_write=DB;
                    continue;
                    //��֤��Ϣ
                }else if(content=="writeVerify"){
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"verify message data size is less than zero";
                        emit result(-1);
                    }
                    m_write=SingleJson;
                    continue;
                }
                //������ʷͷ��
            }else if(in=="40"){
                if(obj.value("result").toString()=="writing"){ //дͷ��
                    m_write=Img;
                    qDebug()<<"writing"<<m_write<<size;
                    imgName=obj.value("name").toString();
                    qDebug()<<"img "<<imgName;
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(obj.value("result").toString()=="true"){ //дͷ��ɹ�
                    m_write=NoFile;//��ʼ�����ö��
                    size=0;
                    emit result(0,"../user/"+myqq+"/historyHeadImg/","png");
                    return;
                }else if(obj.value("result").toString()=="false"){ //дͷ��ʧ��
                    m_write=NoFile;
                    emit result(-1);
                    return;
                }
                //�������ϴ���
            }else if(in=="80"){
                qDebug()<<"obj"<<obj.value("result").toString();
                if(obj.value("result").toString()=="writingJson"){ //д��������
                    m_write=SingleJson;//json��Ϣ
                    qDebug()<<"writing"<<m_write<<size;
                    qDebug()<<"img "<<imgName;
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(obj.value("result").toString()=="writingPhotoWall"){ //��Ƭǽ
                    m_write=Img;//ͼƬ
                    qDebug()<<"writing"<<m_write<<size;
                    imgName=obj.value("name").toString();
                    qDebug()<<"img "<<imgName;
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(obj.value("result").toString()=="writingCover"){ //����
                    m_write=Img;
                    qDebug()<<"writing"<<m_write<<size;
                    imgName=obj.value("name").toString();
                    qDebug()<<"img "<<imgName;
                    size=obj.value("size").toVariant().toLongLong();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(obj.value("result").toString()=="true"){ //�ɹ�
                    m_write=NoFile;//��ʼ�����ö��
                    size=0;
                    emit finished(0);
                    return;
                }else if(obj.value("result").toString()=="false"){ //ʧ��
                    m_write=NoFile;
                    size=0;
                    emit finished(-1);
                    return;
                }
                //��ȡ��֤��Ϣ
            }else if(in=="130"){
                QString content=obj.value("content").toString();
                if(content=="verifyJson"){
                    m_write=SingleJson;
                    qDebug()<<"writing"<<m_write<<size;
                    size=obj.value("size").toInt();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    continue;
                }else if(content=="verifyImg"){
                    m_write=Img;//��ʼ�����ö��
                    imgName=obj.value("imgName").toString();
                    size=obj.value("size").toInt();
                    if(size<=0){
                        qDebug()<<"size is less than or equal to zero";
                        emit result(-1);
                    }
                    return;
                }else if(content=="end"){
                    m_write=NoFile;
                    size=0;
                    QString r=obj.value("result").toString();
                    if(r=="true"){
                        emit result(0,"../user/"+myqq+"/friends/verifyFriends/","png");
                    }
                    else
                        emit result(-1);
                    return;
                }
                //���߼��
            }else if(in=="170"){
                carrier=data;
                emit finished();
                return;
            }
        }
        switch (m_write) {
        case Img://��IMG
            img.insert(imgName,data);
            size=0,imgName="";
            break;
        case SingleJson:
            carrier.append(data);
            size=0;
            break;
        case DB:
            img.insert(imgName,data);//�������ݿ�����
            size=0,imgName="";
            break;
        default:
            break;
        }
    }
}

void BigFileSocket::writeD()
{
    if(instruct.object().value("headerSize").toBool()){
        qDebug()<<"length "<<4;
        QByteArray length;
        QDataStream stream(&length,QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_4_0);
        quint8 l=instruct.toBinaryData().size();
        stream<<l;
        this->write(length+instruct.toBinaryData());
    }else
        this->write(instruct.toBinaryData());
    qDebug()<<"started event loop";
    loop.exec();
    qDebug()<<"exited event loop";
    emit writtenInstruction();
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
    QTimer::singleShot(timeout,this,[=](){
        qDebug()<<"QTimer::singleShot "<<timeout<<"ms";
        emit finished(-1);
    });
}

void BigFileSocket::resultSlot(int code, const QString& folder, const QString& type)
{

    if(code==-1){
        emit finished(-1);
        return;
    }
    qDebug()<<"result slot viaed";
    foreach(QString key,img.keys()){
        QByteArray data=img.value(key);
        if(!data.isEmpty()){
            try{
                QString prefix=key.right(key.length()-key.lastIndexOf(".")-1);//�ļ���׺/\.(.*)/
                qDebug()<<"file prefix is "<<prefix;
                if(prefix=="png"||key.lastIndexOf(".")==-1){//ͷ�񱣴� ���û�к�׺��ΪĬ��ΪͼƬ
                    if(!writeImg(data,folder+key,type.toUtf8().data()))throw false;
                }else if(prefix=="db"){//�������ݿ��ļ�
                    if(!writeDB(data,folder,key))throw false;
                }
            }catch(bool&b){
                qDebug()<<"saved file result:"<<b;
            }catch(...){
                qDebug()<<"curred a unkown exception";
                return;
            }
        }
    }
    emit finished(code);
}
