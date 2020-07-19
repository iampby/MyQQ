#include "writethread.h"
#include"global.h"
#include <qsqlquery.h>
#include <qdir.h>
#include <qjsondocument.h>
#include<qjsonobject.h>
#include <qpixmap.h>
#include <qtimer.h>
#include<QXmlStreamReader>
#include <qjsonarray.h>
#include <qdatetime.h>
#include<QDomDocument>
#include<qbuffer.h>
WriteThread::WriteThread(qintptr socketDescriptor, qint64 count,  QObject *parent)
    :QObject(parent),socketDescriptor(socketDescriptor),count(count)
{
    size=0;FT=NoFile;
    tcpsocket=new QTcpSocket(this);//继承父类，线程与父类一致
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<QStringLiteral("创建套接字失败！");
        emit error(tcpsocket->error());
        return;
    }
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows必设
    connect(tcpsocket,&QTcpSocket::readyRead,this,&WriteThread::readD);
    connect(this,&WriteThread::startTimer,this,&WriteThread::timer);
    connect(tcpsocket,&QTcpSocket::disconnected,this,&WriteThread::disconnected);

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
        qDebug()<<"adjustHistoryImg:folder is empty or null"<<dir.path();
        return false;
    }
    QStringList temp;temp<<"*.png";
    QString newName,name;qint8 two;
    QFileInfoList info=dir.entryInfoList(temp,QDir::Files,QDir::Name);
    for (int i = info.length()-1; i >=0;--i) {
        name=info.at(i).fileName();
        two=name.left(2).toShort();
        QFile file(dir.filePath(name));
        if(two>=18)file.remove();
        ++two;
        if(two<10)newName="0"+QString("%1.png").arg(two);
        else newName=QString("%1.png").arg(two);
        bool ok=file.rename(dir.filePath(newName));
        if(!ok)qDebug()<<"warning: alter file is of fuilure, named "<<name;
    }
    bool ok= head.save(dir.filePath("01.png"),"png");
    if(!ok)return ok;
    dir.cdUp();

    QFile infoxml(dir.path()+"/info.xml");
    if(!infoxml.open(QIODevice::ReadOnly)){
        qDebug()<<dir.path()+"/info.xml"<<":opened unsuceessful";
    }
    qDebug()<<"readed "<<infoxml.fileName();
    QXmlStreamReader reader;
    reader.setDevice(&infoxml);
    QVector<QString>friends;
    while (!reader.atEnd()) {
        reader.readNext();
        qDebug()<<"name="<<reader.name().toString();
        if(reader.name().toString()=="friend"&&reader.isStartElement()){
            QString mq=reader.attributes().value("myqq").toString();
            qDebug()<<"found a friend,it's MyQQ is"<<mq;
            friends.append(mq);
        }
    }
    qDebug()<<"number of friends is equal to"<<friends.length();
    //循环遍历全局文件对象，查找是否有标记文件，标记文件保存着那些好友头像发生了变化
    for (QString&v :friends) {
        qDebug()<<"marked MyQQ is "<<v<<",this represent to need updating image of head while next to accesses marked floder";
        QDir tagDir("../userData/"+v+"/friendsInfo");
        if(!tagDir.exists()){
            tagDir.mkpath("./");//创建当前目录
        }
        QString tagName="markedHeadImg.json";
        if(historyImgFiles.contains(v)){
            QMutex* muter=historyImgMuter.value(v);//引用
            muter->lock();
            //如果没有容纳 则自动更新线程前一步调用 需跳转到新建更新标记文件处
            if(!historyImgFiles.contains(v)){
                qDebug()<<"notice:marked markedHeadImg.json file just now  was deleted from other thread";
                muter->unlock();
                historyImgMuter.remove(v);
                delete muter,muter=nullptr;//删除指针
                goto label;
            }
            QFile* markedFile=historyImgFiles.value(v);
            if(markedFile->open(QIODevice::ReadWrite)){
                QByteArray markData=markedFile->readAll();
                QJsonDocument jsonDoc=QJsonDocument::fromJson(markData);
                if(!jsonDoc.isObject()){
                    qDebug()<<"error:file "<<markedFile->fileName()<<" is not a object";
                }
                QJsonObject obj=jsonDoc.object();
                QJsonArray arr=obj.value("myqq").toArray();
                QJsonObject tempObj;
                tempObj.insert("number",QJsonValue(myqq));
                arr.append(QJsonValue(tempObj));
                obj.insert("myqq",QJsonValue(arr));
                jsonDoc=QJsonDocument();
                jsonDoc.setObject(obj);
                markedFile->write(jsonDoc.toJson());
                markedFile->close();
            }else
                qDebug()<<"json write is of failure,named  "<<markedFile->fileName();
            muter->unlock();
        }else {
            label:
            QFile* markedFile=new QFile(tagDir.filePath(tagName));
            if(markedFile->open(QIODevice::ReadWrite)){
                QJsonDocument jsonDoc;
                QJsonObject obj;
                QJsonArray arr;
                QJsonObject tempObj;
                tempObj.insert("number",QJsonValue(myqq));
                arr.append(QJsonValue(tempObj));
                obj.insert("myqq",QJsonValue(arr));
                jsonDoc;
                jsonDoc.setObject(obj);
                markedFile->write(jsonDoc.toJson());
                markedFile->close();
                historyImgFiles.insert(v,markedFile);
                QMutex* muter=new QMutex();
                historyImgMuter.insert(v,muter);
            }else{
                qDebug()<<"json write is of failure,named  "<<markedFile->fileName();
                markedFile->deleteLater();
            }
        }
    }
    return true;
}

bool WriteThread::updateSignature(QByteArray &bytes)
{
    //用精确到秒的时间和100个值范围的count计数来控制1秒内可以打开的数据库连接，即100个
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",QDateTime::currentDateTime().toString("yy-M-d h:m:s")+QString("%1").arg(count));

    QString connectString = QStringLiteral(
                "DRIVER={sql server};"
                "SERVER=127.0.0.1;"
                "DATABASENAME:qtmanager;"
                "PORT:1433;"
                "UID=sa;"
                "PWD=123456x;");
    db.setDatabaseName(connectString);
    if(db.open())
        qDebug()<<QStringLiteral("打开数据库成功！");
    else {
        qDebug()<<QStringLiteral("打开数据库失败！");
        return false;
    }
    qDebug()<<"login"<<db.connectionName()<<db.databaseName();
    QSqlQuery query(" use myqq ",db);
    query.prepare(" update userInfo set personalizedSignature=?  where myqq=? ");
    QString sig=QString::fromUtf8(bytes);
    query.addBindValue(QVariant(sig.isEmpty()?"":sig));
    query.addBindValue(QVariant(myqq));
    bool ok= query.exec();
    if(ok){
        qDebug()<<"query.exec() successfully"  ;
    }else  {
        qDebug()<<"query.exec() unsuccessfully";
        return ok;
    }
    QFile infoxml("../userData/"+myqq+"/info.xml");
    if(!infoxml.open(QIODevice::ReadWrite)){
        qDebug()<<myqq<<":opened info.xml  unsuceessful";
    }
    qDebug()<<"readed "<<infoxml.fileName();
    QDomDocument doc;
    doc.setContent(&infoxml);
    infoxml.close();
    QDomElement dele=doc.documentElement();
dele=dele.firstChildElement("friendGroup");
QDomNodeList listGroup=dele.childNodes();
QVector<QString>friends;//保存好友号码
for (int var = 0; var < listGroup.size(); ++var) {
    QDomElement ele=listGroup.at(var).toElement();
    qDebug()<<ele.tagName();
    if(ele.hasChildNodes()){
        QDomElement friendEle=ele.firstChild().toElement();
        while(!friendEle.isNull()){
            QString mq=friendEle.attribute("myqq");
            if(!mq.isEmpty()){
                qDebug()<<"a number is found:"<<mq;
                friends.append(mq);
                if(mq==myqq){
                   QDomElement myself=friendEle.firstChildElement(QStringLiteral("个性签名"));
                   if(!myself.isNull()){
                       qDebug()<<"find myself "<<mq<<" signature:"<<myself.text();
                     QDomElement newele=doc.createElement(QStringLiteral("个性签名"));
                     QDomText t=doc.createTextNode(QString::fromUtf8(bytes));//保存签名
                     newele.appendChild(t);
                     friendEle.replaceChild(newele,myself);//替换
                   }
                }
            }
            friendEle=friendEle.nextSiblingElement();
        }
    }
}
infoxml.open(QIODevice::WriteOnly);//只写覆盖
QTextStream stream(&infoxml);
stream.setCodec("utf-8");
doc.save(stream,4,QDomNode::EncodingFromTextStream);
infoxml.close();

    qDebug()<<"number of friends is equal to"<<friends.length();
    //循环遍历全局文件对象，查找是否有标记文件，标记文件保存着那些好友头像发生了变化的号码
    for (QString&v :friends) {
        QDir tagDir("../userData/"+v+"/friendsInfo");
        qDebug()<<"marked MyQQ is "<<v<<",this represent to need updating user' infomation of signature   while next to accesses marked floder";
        if(!tagDir.exists()){
            tagDir.mkpath("./");//创建当前目录
        }
        QString tagName="markedSignatureAndName.json";
        if(sigFiles.contains(v)){
            QMutex*muter=sigMuter[v];
            muter->lock();
            if(!sigFiles.contains(v)){
                qDebug()<<"notice:marked markedSignatureAndName.json file  just now  was deleted from other thread";
                muter->unlock();
               sigMuter.remove(v);
                delete muter,muter=nullptr;//删除指针
                goto label;//如果文件已经移除，重新更新文件
            }
            QFile* markedFile=sigFiles.value(v);
            if(markedFile->open(QIODevice::ReadWrite)){
                QByteArray markData=markedFile->readAll();
                QJsonDocument json=QJsonDocument::fromJson(markData);
                QJsonObject obj=json.object();
                QJsonArray arr=obj.value("signature").toArray();
                QJsonObject temp;
                temp.insert("signature",QJsonValue(QString::fromUtf8(bytes)));
                temp.insert("number",QJsonValue(myqq));
                arr.append(temp);
                json=QJsonDocument();
                obj.insert("signature",QJsonValue(arr));
                json.setObject(obj);
                markedFile->write(json.toJson());
                markedFile->close();
            }else
                qDebug()<<"json write is of failure,named  "<<markedFile->fileName();
            muter->unlock();
        }else {
            label:
            QFile* markedFile=new QFile(tagDir.filePath(tagName));
            if(markedFile->open(QIODevice::ReadWrite)){
                QJsonDocument json;
                QJsonObject obj;
                QJsonArray arr;
                QJsonObject temp;
                temp.insert("signature",QJsonValue(QString::fromUtf8(bytes)));
                temp.insert("number",QJsonValue(myqq));
                arr.append(temp);
                obj.insert("signature",QJsonValue(arr));
                json.setObject(obj);
                markedFile->write(json.toJson());
                markedFile->close();
                sigFiles.insert(v,markedFile);
                QMutex* muter=new QMutex();
                sigMuter.insert(v,muter);
            }else{
                qDebug()<<"json write is of failure,named  "<<markedFile->fileName();
             delete markedFile,markedFile=nullptr;
            }
        }

    }
    return ok;
}
//保存资料封面
bool WriteThread::updateCover(QByteArray &bytes)
{
    QPixmap pix;
   if(! pix.loadFromData(bytes)){
        qDebug()<<"warning:cover is not loaded,the number is equal to "<<myqq;
        return false;
    }
   if(!pix.save("../userData/"+myqq+"/cover","png")){
       qDebug()<<"warning:cover is not saved suffix is png,the number is equal to "<<myqq;
       return false;
   }
   return true;
}

bool WriteThread::updateWall(QByteArray &bytes)
{
    QDataStream stream(&bytes,QIODevice::ReadOnly);
    quint8 length;
    stream>>length;
    if(length>9)return false;
    QVector<quint32>sizeVctor;
    QVector<QPixmap>pixVector;
    for (quint8 var = 0; var < length; ++var) {
        quint32 temp;
        stream>>temp;
        qDebug()<<"size"<<var<<"="<<temp;
        sizeVctor.append(temp);
    }
   QBuffer buff(&bytes);
   if(!buff.open(QIODevice::ReadOnly)){
       qDebug()<<" data of photo-wall acquisition failed";
       return false;
   }
   qint64 pos=1+length*4;
   if(bytes.size()<pos)return false;
   buff.seek(pos);//移动到数据段
   //获取图片数据
  for(quint8 i=0;i<length;i++){
      QByteArray temp;
      temp.resize(sizeVctor.at(i));
      temp=buff.read(sizeVctor.at(i));
      QPixmap pix;
      pix.loadFromData(temp);
     if(pix.isNull()){
         qDebug()<<" data is null for a pixmap of photo wall ";
         continue;
     }
     pixVector.append(pix);
  }
  length=pixVector.size();
   QDir dir("../userData/"+myqq+"/photoWall");
   if(!dir.exists()){
       dir.mkpath("./");//创建当前目录
   }
   QStringList list;
   list =dir.entryList(QStringList("*"),QDir::Files);
   if(list.length()>0)
   for (quint8 var = list.length()-1; var >=0; --var) {
       QFile file(dir.relativeFilePath(QString("%1").arg(var)));
       quint8 temp=var+length;
       if(temp>8){
           qDebug()<<"warning: photo wall is acquired more planed data,the number is"<<myqq<<var;
           file.remove();
          return false;
       }
       file.rename(QString("%1").arg(temp));
   }
   bool ok=true;
   for (quint8 var = 0; var < length; ++var) {
      QPixmap& temp=pixVector[var];
      qDebug()<<temp.size();
      if(!temp.save(dir.absoluteFilePath(QString("%1").arg(var)),"png")){
          qDebug()<<"warning:photo wall is of failure to update a piamap";
          ok=false;
      }

   }
   return ok;
}

void WriteThread::timer()
{
    //子线程调用定时器
    QTimer::singleShot(50000,Qt::CoarseTimer,this,[=](){
        qDebug()<<"timer 50s exit";
        emit finished();
        qDebug()<<"thread had exited";
    });
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
                //4 更新历史头像
                if(in=="4"){
                    QString content=obj.value("content").toString();
                    if(content=="historyHeadImg"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        fileName="../userData/"+myqq+"/historyHeadImg/01.png";
                        FT=HistoryHeadImage;
                        size=1;
                        continue;
                    }
                    //更新个性签名
                }else if(in=="5"){
                    QString content=obj.value("content").toString();
                    if(content=="updateSianature"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            tcpsocket->disconnectFromHost();
                            return;
                        }
                        FT=Signature;
                        size=1;
                        continue;
                    }
                    //更新用户资料封面
                }else if(in=="7"){
                    QString content=obj.value("content").toString();
                    if(content=="updateCover"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=CoverImage;
                        size=1;
                        continue;
                    }
                    //9 updatePhotoWall
                }else if(in=="9"){
                    QString content=obj.value("content").toString();
                    if(content=="updatePhotoWall"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=PhotoWall;
                        size=1;
                        continue;
                    }
                }
            }
        }else{
            QByteArray data=tcpsocket->readAll();
            switch (FT) {
            case HistoryHeadImage:
                bytes.append(data);
                break;
            case Signature:
                bytes.append(data);
                break;
            case CoverImage:
                bytes.append(data);
                break;
            case PhotoWall:
                bytes.append(data);
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
    size=0;
    switch (FT) {
    //更新历史头像
    case HistoryHeadImage:
        if(adjustHistoryImg(bytes,fileName,"png")){
            qDebug()<<"added history image successfully";
        }
        break;
        //更新个性签名
    case Signature:
        if(updateSignature(bytes)){
            qDebug()<<"updated signature successfully";
        }
        break;
        //更新封面
    case CoverImage:
        if(updateCover(bytes)){
            qDebug()<<"updated cover-image successfully";
        }
        break;
        //更新照片墙
    case PhotoWall:
        if(updateWall(bytes)){
            qDebug()<<"updated photo wall successfully";
        }
        break;
    default:
        break;
    }
    qDebug()<<"emit finished()";
    emit finished();
}
