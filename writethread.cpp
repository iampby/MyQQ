#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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
#include<QSqlRecord>
#include<qthread.h>
#include<sstream>
WriteThread::WriteThread(qintptr socketDescriptor, qint64 count,  QObject *parent)
    :QObject(parent),socketDescriptor(socketDescriptor),count(count)
{
    size=0;FT=NoFile;
    tcpsocket=new QTcpSocket(this);//继承父类，线程与父类一致
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<("创建套接字失败！");
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
    foreach (QString str, dbPool) {
        {
        QSqlDatabase::database(str,false);
        }
        QSqlDatabase::removeDatabase(str);
    }
}

QSqlQuery WriteThread::openDB(bool &ok)
{
    //用精确到秒的时间和100个值范围的count计数来控制1秒内可以打开的数据库连接，即100个
    QString cn=QDateTime::currentDateTime().toString("yy-M-d h:m:s")+QString("%1").arg(count);
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",cn);
dbPool.append(cn);//入池
    QString connectString = (
                "DRIVER={sql server};"
                "SERVER=127.0.0.1;"
                "DATABASENAME:qtmanager;"
                "PORT:1433;"
                "UID=sa;"
                "PWD=123456x;");
    db.setDatabaseName(connectString);
    if(db.open())
        qDebug()<<("打开数据库成功！");
    else {
        qDebug()<<("打开数据库失败！");
        ok=false;
        return QSqlQuery();
    }
    qDebug()<<"login"<<db.connectionName();
    ok=true;
QSqlQuery query("use MyQQ",db);
    return query;
}

bool WriteThread::userExit()
{
    bool ok;
    QString status="0";
    QSqlQuery query=openDB(ok);
    if(!ok)return ok;
    query.prepare(" update userInfo set isConnected=? ,userState=?  WHERE myqq=? ");
    query.addBindValue(QVariant(false));
    query.addBindValue(QVariant(status));
    query.addBindValue(QVariant(myqq.toLongLong()));
    if(query.exec()){
        qDebug() <<"update user' status is successful";

        return exitStatusHandle(status);
    }

    return false;
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
    bool ok;
     QSqlQuery query=openDB(ok);
    if(!ok)return ok;//打开失败返回
    query.prepare(" update userInfo set personalizedSignature=?  where myqq=? ");
    QString sig=QString::fromUtf8(bytes);
    query.addBindValue(QVariant(sig.isEmpty()?"":sig));
    query.addBindValue(QVariant(myqq.toLongLong()));
    ok= query.exec();
    if(ok){
        qDebug()<<"query.exec() successfully"  ;
    }else  {
        qDebug()<<"query.exec() unsuccessfully";
        return ok;
    }

    return updateSignatureHandle(sig);
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
        for (qint8 var = list.length()-1; var >=0; --var) {
            QFile file(dir.filePath(QString("%1").arg(var)));
            quint8 temp=var+length;
            if(temp>8){
                qDebug()<<"warning: photo wall is acquired more planed data,the number is"<<myqq<<var;
                file.remove();
                return false;
            }
            file.rename(dir.filePath(QString("%1").arg(temp)));
        }
    bool ok=true;
    for (quint8 var = 0; var < length; ++var) {
        QPixmap& temp=pixVector[var];
        if(!temp.save(dir.absoluteFilePath(QString("%1").arg(var)),"png")){
            qDebug()<<"warning:photo wall is of failure to update a piamap";
            ok=false;
        }
    }
    return ok;
}

bool WriteThread::updateWall(int &pos)
{
    QDir dir("../userData/"+myqq+"/photoWall/");
    QFile file(dir.filePath(QString("%1").arg(pos)));
    if(!file.exists()){
        qDebug()<<"warning:can't find the file i want to  remove  ";
        return false;
    }
    if(!file.remove()){
        qDebug()<<"warning:removing  is of failure to the file";
        return false;
    }
    QStringList list=dir.entryList(QDir::Files,QDir::Name);//按名字排序
    int length=list.length();
    for(int i=0;i<length;++i){
        int name=list.at(i).toInt();
        if(name>pos){
            QFile tf(dir.filePath(QString("%1").arg(name)));
            //序号减1
            if(!tf.rename(dir.filePath(QString("%1").arg(--name)))){
                qDebug()<<"warning:renaming  is of failure to the file:"<<name+1;
                continue;
            }
        }
    }
    return true;
}

bool WriteThread::updateUserInfo(QByteArray &bytes)
{
    qDebug()<<"updateUserInfo(QByteArray &bytes) called";
    QJsonDocument doc=QJsonDocument::fromBinaryData(bytes);
    if(doc.isObject()){
        QJsonObject obj=doc.object();

        QString name=obj.value("name").toString();
        if(name.isEmpty()){
            qDebug()<<"name is empty";
            return false;
        }
        QString  birthday=obj.value("birthday").toString();
        if(birthday.length()<8){
            qDebug()<<"the format is not corrected for birthday that is "<<birthday<<birthday.size();
            return false  ;
        }
        QString blooGroup=obj.value("blooGroup").toString();
        if(blooGroup.isNull())blooGroup="";//空值替换
        QString sex=obj.value("sex").toString();
        if(sex.isNull())sex="";
        QString personalStatement=obj.value("personalStatement").toString();
        if(personalStatement.isNull())personalStatement="";
        QString profession=obj.value("profession").toString();
        if(profession.isNull())profession="";
        QString corporation=obj.value("corporation").toString();
        if(corporation.isNull())corporation="";
        QString where1=obj.value("where1").toString();
        if(where1.isNull())where1="";
        QString where2=obj.value("where2").toString();
        if(where2.isNull())where2="";
        QString where3=obj.value("where3").toString();
        if(where3.isNull())where3="";
        QString where4=obj.value("where4").toString();
        if(where4.isNull())where4="";
        QString home1=obj.value("home1").toString();
        if(home1.isNull())home1="";
        QString home2=obj.value("home2").toString();
        if(home2.isNull())home2="";
        QString home3=obj.value("home3").toString();
        if(home3.isNull())home3="";
        QString home4=obj.value("home4").toString();
        if(home4.isNull())home4="";
        QString phone=obj.value("phone").toString();
        if(phone.isEmpty())phone="0";
        QString education=obj.value("education").toString();
        if(education.isNull())education="";
        bool ok;
         QSqlQuery query=openDB(ok);
        if(!ok){

            return ok;
        }
        query.prepare(" exec updateUserInformation ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,? ");
        query.addBindValue(QVariant(name));
        query.addBindValue(QVariant(sex));
        query.addBindValue(QVariant(birthday));
        query.addBindValue(QVariant(blooGroup));
        query.addBindValue(QVariant(home1));
        query.addBindValue(QVariant(home2));
        query.addBindValue(QVariant(home3));
        query.addBindValue(QVariant(home4));
        query.addBindValue(QVariant(where1));
        query.addBindValue(QVariant(where2));
        query.addBindValue(QVariant(where3));
        query.addBindValue(QVariant(where4));
        query.addBindValue(QVariant(profession));
        query.addBindValue(QVariant(corporation));
        query.addBindValue(QVariant(personalStatement));
        query.addBindValue(QVariant(phone.toLongLong()));
        query.addBindValue(QVariant(education));
        query.addBindValue(QVariant(myqq.toLongLong()));
        ok= query.exec();
        if(ok){
            qDebug()<<"query.exec() successfully"  ;
            updateNameHandle(name);//标记昵称变换
        }else  {
            qDebug()<<"query.exec() unsuccessfully";
        }

        return ok;
    }else{
        qDebug()<<"it's not object";
        return false;
    }
}

bool WriteThread::addFGroup(QByteArray &bytes)
{
    QJsonDocument json=QJsonDocument::fromBinaryData(bytes);
    QJsonObject obj=json.object();
    if(obj.isEmpty()){
        qDebug()<<"warning:a object is empty while added a friends group";
        return false;
    }
    QString name=obj.value("groupName").toString();
    QFile infoxml("../userData/"+myqq+"/info.xml");
    if(!infoxml.open(QIODevice::ReadOnly)){
        qDebug()<<infoxml.fileName()<<" is  opened successfully";
        return false;
    }
    QDomDocument doc;
    doc.setContent(&infoxml);
    infoxml.close();
    QDomElement dele=doc.documentElement();
    dele=dele.firstChildElement("friendGroup");
    qint32 count= dele.attribute("count").toInt();
    dele.setAttribute("count",++count);
    //找插入位置
    qint32 index=obj.value("index").toInt(-1);
    if(index==-1){
        QDomElement addEle=doc.createElement(("好友组")+QString("%1").arg(count-1));
        addEle.setAttribute("name",name);
        addEle.setAttribute("set","none");

        QDomText addText=doc.createTextNode("none");
        addEle.appendChild(addText);
        QDomNode last=dele.lastChild();
        last.toElement().setTagName(("好友组")+QString("%1").arg(count));//重设标签名
        dele.insertBefore(addEle,last);
    }else{
        //先插入
        QDomElement addEle=doc.createElement(("好友组")+QString("%1").arg(index+1));
        addEle.setAttribute("name",name);
        addEle.setAttribute("set","none");
        QDomNodeList list= dele.childNodes();
        QDomNode node=list.item(index);
        if(node.isNull()){
            qDebug()<<"node is null";
            return false;
        }
        if(dele.insertBefore(addEle,node).isNull()){
            qDebug()<<"insertBefore(addEle,node).isNull()";
            return false;
        }
        //遍历修改名
        count-=1;//遍历排除最后一个
        for(int i=index;i<count;++i){
            QDomNode tnode=list.item(i);
            tnode.toElement().setTagName(("好友组")+QString("%1").arg(i+2));//组名进1
        }
    }
    //文件写入
    if(!infoxml.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qDebug()<<infoxml.fileName()<<" is  opened to sace successfully";
        return false;
    }
    QTextStream stream(&infoxml);
    doc.save(stream,4);
    infoxml.close();
    return true;
}

bool WriteThread::dmrFGroup(QByteArray &bytes)
{
    QJsonDocument json=QJsonDocument::fromBinaryData(bytes);
    if(json.isObject()){
        QJsonObject obj=json.object();
        if(obj.isEmpty()){
            qDebug()<<"object is empty, deleted or moved friend group unsuccessfully " ;
            return false;
        }
        QString type=obj.value("type").toString();
        //打开文件 初始化 dom树
        QFile file("../userData/"+myqq+"/info.xml");
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"opened the file is of failure" ;
            return false;
        }
        QDomDocument domDoc;
        domDoc.setContent(&file);
        file.close();
        //获取节点表
        QDomElement  fgele=domDoc.documentElement();
        fgele=fgele.firstChildElement("friendGroup");
        QDomNodeList list=fgele.childNodes();
        if(type=="move"){
            qDebug()<<"friend group moving";
            qint32 index1=obj.value("index1").toInt();
            qint32 index2=obj.value("index2").toInt();
            if(index1>index2){
                qint32 mid=index2;
                index2=index1;
                index1=mid;
            }

            QDomNode node1=list.item(index1);
            QDomNode node2=list.item(index2);
            QString name1=node1.toElement().tagName();
            QString name2=node2.toElement().tagName();
            if(fgele.replaceChild(node2,node1).isNull()){
                qDebug()<<"replaceChild(node2,node1).isNull()";
                return false;
            }
            node2.toElement().setTagName(name1);
            list=fgele.childNodes();
            QDomNode node3=list.item(index2-1);//重构list
            if(fgele.insertAfter(node1,node3).isNull()){
                qDebug()<<"insertAfter(node1,node3).isNull()";
                return false;
            }
            node1.toElement().setTagName(name2);
            //重命名
        }else if(type=="rename"){
            qDebug()<<"friend group rename";
            qint32 index=obj.value("index").toInt();
            QString name=obj.value("newName").toString();
            QDomNode node=list.item(index);
            node.toElement().setAttribute("name",name);
        }else if(type=="delete"){
            qint32 index=obj.value("index").toInt();
            qint32 count=fgele.attribute("count").toInt();
            if(index<2||index==count-1){
                qDebug()<<"attempt to delete a  group  with no nonsupport the action ";
                return false;
            }
            QDomNode node=list.item(index);
            QDomNode oldNode=fgele.removeChild(node);
            if(oldNode.isNull()){
                qDebug()<<  "removeChild(node).isNull()";
                return false;
            }
            fgele.setAttribute("count",--count);
            list=fgele.childNodes();
            for (int var = index; var < count;++var) {
                node=list.item(var);
                qDebug()<<node.toElement().tagName()<<var;
                node.toElement().setTagName(("好友组")+QString("%1").arg(var+1));
            }
            //好友移动到第二组好友组
            node=list.item(1);
            if(oldNode.firstChild().nodeValue()!="none"){
                list=oldNode.childNodes();
                qint32 length=list.size();
                QDomNode f=node.firstChild();
                if(f.nodeValue()=="none"){
                    node.replaceChild(list.item(0),f);
                }else{
                    node.appendChild(list.item(0));
                }
                for (int var = 1; var < length; ++var) {
                    node.appendChild(list.item(var));
                }
            }
        }
        //保存文件
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
            qDebug()<<"opened the file the second time is of failure" ;
            return false;
        }
        QTextStream stream(&file);
        domDoc.save(stream,4);
        file.close();
        return true;
    }
    return false;
}

bool WriteThread::dmrFriend(QByteArray &bytes)
{
    QJsonDocument json=QJsonDocument::fromBinaryData(bytes);
    if(json.isObject()){
        QJsonObject obj=json.object();
        if(obj.isEmpty()){
            qDebug()<<"object is empty, deleted or moved  friend ,alter friend's tag  unsuccessfully " ;
            return false;
        }
        QString type=obj.value("type").toString();
        //打开文件 初始化 dom树
        QFile file("../userData/"+myqq+"/info.xml");
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"opened the file is of failure" ;
            return false;
        }
        QDomDocument domDoc;
        domDoc.setContent(&file);
        file.close();
        //获取节点表
        QDomElement  fgele=domDoc.documentElement();
        fgele=fgele.firstChildElement("friendGroup");
        QDomNodeList list=fgele.childNodes();
        //移动好友
        if(type=="move"){
            qint32 before=obj.value("before").toInt(-1);
            qint32 after=obj.value("after").toInt(-1);
            QString number=obj.value("number").toString();
            if(before==-1||after==-1||number<"10001"){
                qDebug()<<"warning:moving friend with number "<<number<<" form "<<before<<" to "<<after<<" group";
                return false;
            }
            qDebug()<<"moving friend with number="<<number<<" form "<<before<<" to "<<after<<" group";

            QDomNode bnode=list.item(before);
            QDomNode anode=list.item(after);
            if(bnode.isNull()||anode.isNull()){
                qDebug()<<"fuond a node is null";
                return false;
            }
            QDomNodeList listb=bnode.childNodes();
            if(listb.at(0).firstChild().nodeValue()=="none"){
                qDebug()<<"warning:node value is not found friends";
                return false;
            }
            int length=listb.size();
            for(int i=0;i<length;++i){
                QDomNode temp=listb.item(i);
                QString str=temp.toElement().attribute("myqq");
                if(str==number){
                    qDebug()<<"found the friend with number "<<number;
                    if(length==1){
                        QDomText none=domDoc.createTextNode("none");
                        temp=bnode.replaceChild(none,temp);
                    }else
                        temp= bnode.removeChild(temp);
                    if(temp.isNull()){
                        qDebug()<<"bnode.removeChild(temp).isNull()";
                        return false;
                    }
                    QDomNode tfnode=anode.firstChild();
                    if(tfnode.nodeValue()=="none"){
                        temp= anode.replaceChild(temp,tfnode);
                    }else{
                        temp= anode.appendChild(temp);
                    }
                    if(temp.isNull()){
                        qDebug()<<" anode.append a Child and return a null reference node";
                        return false;
                    }
                    break;
                }
            }
            //删除
        }else if(type=="delete"){
            qDebug()<<"friend deletion";
            qint32 index=obj.value("index").toInt();
            QString number=obj.value("number").toString();
            if(index<0||index>=list.size()){
                qDebug()<<"deleting friend index is more than range of array ";
                return false;
            }
            QDomNode node=list.item(index);
            if(!node.hasChildNodes()){
                qDebug()<<"deleting friend data is of exception";
                return false;
            }
            list=node.childNodes();
            qint32 length=list.size();
            for (int var = 0; var < length; ++var) {
                QDomNode temp=list.item(var);
                if(temp.toElement().attribute("myqq")==number){
                    if(length==1){
                        QDomText  text= domDoc.createTextNode("none");
                        if(node.replaceChild(text,temp).isNull()){
                            qDebug()<<"node.replaceChild(text,temp).isNull()";
                            return false;
                        }
                    }else{
                        if((node.removeChild(temp).isNull())){
                            qDebug()<<"node.removeChild(temp).isNull()";
                            return false;
                        }
                    }
                    QFile tf("../userData/"+number+"/info.xml");
                    if(!tf.exists()){
                        qDebug()<<"friend "<<number<<" file info.xml is not existis)";
                        return false;
                    }
                    if(!tf.open(QIODevice::ReadOnly)){
                        qDebug()<<"opend info.xml is not successful";
                        return false;
                    }
                    QDomDocument tdoc;
                    tdoc.setContent(&tf);
                    tf.close();
                    fgele=tdoc.documentElement();
                    fgele=fgele.firstChildElement("friendGroup");
                    list=fgele.childNodes();//组集
                    length=list.size();
                    for(int i=0;i<length;++i){
                        QDomElement ele=list.item(i).toElement();
                        if(ele.hasChildNodes()){
                            QDomElement friendEle=ele.firstChild().toElement();//单个好友元素
                            while(!friendEle.isNull()){
                                QString mq=friendEle.attribute("myqq");
                                qDebug()<<"mq="<<mq<<myqq;
                                if(!mq.isEmpty()){
                                    //删除主动方节点
                                    if(mq==myqq){
                                        qDebug()<<"found the number "<<mq<<" has been deleted";
                                        if(ele.childNodes().length()==1){
                                            QDomText  text=tdoc.createTextNode("none");
                                            if( ele.replaceChild(text,friendEle).isNull()){
                                                qDebug()<<"deleting opposite list node is of failure";
                                                return false;
                                            }
                                        }else{
                                            if(ele.removeChild(friendEle).isNull()){
                                                qDebug()<<"deleting opposite list node is of failure";
                                                return false;
                                            }
                                        }
                                        i=length;//跳出2个循环
                                        break;
                                    }
                                }
                                friendEle=friendEle.nextSiblingElement();
                            }
                        }
                    }
                    //保存删除好友已修改的info.xml文件
                    if(!tf.open(QIODevice::WriteOnly|QIODevice::Truncate)){
                        qDebug()<<"opend info.xml is not successful";
                        return false;
                    }
                    QTextStream stream(&tf);
                    tdoc.save(stream,4);
                    tf.close();
                    var=length;//跳出循环
                    break;
                }
            }

        }else if(type=="alias"){
            QString number=obj.value("number").toString();
            QString tag=obj.value("tag").toString();
            int index=obj.value("index").toInt(-1);
            qint32 count=fgele.attribute("count").toInt();
            if(index==-1||index>=count){
                qDebug()<<"warning:index is -1";
                return false;
            }
            QDomNode node=list.item(index);
            list=node.childNodes();
            int length=list.size();

            for (int var = 0; var <length;++var) {
                node=list.item(var);
                if(node.toElement().attribute("myqq")==number){
                    qDebug()<<"found target number "<<number;
                    QDomElement tele=  node.firstChildElement(("备注"));
                    if(tele.isNull()){
                        qDebug()<<"found a null node element";
                        return false;
                    }
                    if(tele.attribute("isNull")=="true"){
                        if(!tag.isEmpty()){
                            node=tele.firstChild();
                            QDomText ttext=domDoc.createTextNode(tag);
                            if(tele.replaceChild(ttext,node).isNull()){
                                qDebug()<<"tele.replaceChild(ttext,node).isNull()";
                                return false;
                            }
                            tele.setAttribute("isNull","false");//标记为非空备注
                        }
                    }else{
                        if(tag.isEmpty()){
                            tele.setAttribute("isNull","true");
                            tag="none";
                        }
                        node=tele.firstChild();
                        QDomText ttext=domDoc.createTextNode(tag);
                        if(tele.replaceChild(ttext,node).isNull()){
                            qDebug()<<"tele.replaceChild(ttext,node).isNull()";
                            return false;
                        }
                    }
                    break;
                }
            }

        }
        //保存文件
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
            qDebug()<<"opened the file the second time is of failure" ;
            return false;
        }
        QTextStream stream(&file);
        domDoc.save(stream,4);
        file.close();
        return true;
    }
    return false;
}
//保存好友消息 上线即传
bool WriteThread::fMessageSave(QByteArray &bytes)
{
    QString number=fileName;//接收方号码
    if(bytes.isEmpty()){
        qDebug()<<"warning:message is empty";
        return false;
    }
    //打开数据库
    QDir dir(QString("../userData/%1/friendsInfo/chat").arg(number));
    if(!dir.mkpath("./")){
        qDebug()<<"dir mkplath is of failure";
        return false;
    }
    QString tid;
    std::thread::id id=std::this_thread::get_id();
    std::stringstream sin;
    sin << id;
    tid=QString::fromStdString(sin.str());
    QString cn=tid+myqq+QString("%1").arg(QDateTime::currentDateTime().toTime_t());
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",cn);
    dbPool.append(cn);//入池
    db.setDatabaseName(dir.absoluteFilePath( QString("_%1.db").arg(myqq)));
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
    bool ok=  query.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1info' ").arg(myqq));
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
                                )").arg(myqq));
                       if(!ok){
                           qDebug()<<"create a table is of failure";

                           return false;
                       }
    }
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.qualifiedName()=="消息"){
                QString time=reader.attributes().value("datetime").toString();
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
                            query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(myqq));
                            query.addBindValue(QVariant(text));
                            query.addBindValue(QVariant(time));
                            query.addBindValue(QVariant(type));
                            query.addBindValue(QVariant(true));
                            query.addBindValue(QVariant(myqq.toLongLong()));
                            ok=query.exec();
                            if(!ok){
                                qDebug()<<"type:text;query.exec(\" insert into _%1info(data,datetime,type,adhesion) values(?,?,?,?) \")=false";
                                continue;
                            }
                        }else if(type=="pixmap"){//把16进制转为原数据保存
                            QByteArray pix=QByteArray::fromHex(text.toUtf8());//转化为原数据
                            query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(myqq));
                            query.addBindValue(QVariant(pix));
                            query.addBindValue(QVariant(time));
                            query.addBindValue(QVariant(type));
                            query.addBindValue(QVariant(true));
                            query.addBindValue(QVariant(myqq.toLongLong()));
                            ok=query.exec();
                            if(!ok){
                                qDebug()<<"type:pixmap; query.exec(\" insert into _%1info(data,datetime,type,adhesion) values(?,?,?,?) \")=false";
                                continue;
                            }
                        }
                    }else if(reader.isEndElement()){
                        if(reader.qualifiedName()=="消息")
                            break;
                    }
                }
                break;
            }
        }
    }
    ok= query.exec(QString(" select count(*) from _%1info ").arg(myqq));
    if(!ok){
        qDebug()<<"query.exec(QString(\" select count(*) from _%1info \").arg(myqq)) is of failure";

        return false;
    }
    try{
        query.next();
        int count=query.value(0).toInt();
        if(count<=0)throw "table is empty";
        ok= query.exec(QString(" update _%1info set adhesion=%2 where id=%3 ").arg(myqq).arg(false).arg(count));
        if(!ok){
            qDebug()<<" update _%1info set adhesion=%2  failure";

            return false;
        }
    }catch(_exception&e){
        qDebug()<<"a exception:"<<"type is "<<e.type<<"name is"<<e.name;

        return false;
    }catch(...){
        qDebug()<<"a unknow excpetion";

        return false;
    }

    return true;
}

bool WriteThread::updateSignatureHandle(QString &sig)
{
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
                        QDomElement myself=friendEle.firstChildElement(("个性签名"));
                        if(!myself.isNull()){
                            qDebug()<<"find myself "<<mq<<" signature:"<<myself.text();
                            QDomElement newele=doc.createElement(("个性签名"));
                            QString value=sig;
                            QDomText text;
                            if(value.isEmpty()){
                                text=doc.createTextNode("none");
                                newele.setAttribute("isNull","true");
                            }else{
                                text=doc.createTextNode(value);//保存签名
                                newele.setAttribute("isNull","false");
                            }
                            newele.appendChild(text);
                            friendEle.replaceChild(newele,myself);//替换
                        }
                    }else{
                        updateXMLSignature(myqq,mq,sig);
                    }
                }
                friendEle=friendEle.nextSiblingElement();
            }
        }
    }
    infoxml.open(QIODevice::WriteOnly|QIODevice::Truncate);//只写覆盖
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
                temp.insert("signature",QJsonValue(sig));
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
                temp.insert("signature",QJsonValue(sig));
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
    return true;
}

void WriteThread::updateNameHandle(QString &name)
{
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
                        QDomElement myself=friendEle.firstChildElement(("昵称"));
                        if(!myself.isNull()){
                            qDebug()<<"find myself "<<mq<<" signature:"<<myself.text();
                            QDomElement newele=doc.createElement(("昵称"));
                            QDomText text=doc.createTextNode(name);//保存签名
                            newele.appendChild(text);
                            friendEle.replaceChild(newele,myself);//替换
                        }
                    }else{
                        updateXMLName(myqq,mq,name);//更新
                    }
                }
                friendEle=friendEle.nextSiblingElement();
            }
        }
    }
    infoxml.open(QIODevice::WriteOnly|QIODevice::Truncate);//只写覆盖
    QTextStream stream(&infoxml);
    stream.setCodec("utf-8");
    doc.save(stream,4,QDomNode::EncodingFromTextStream);
    infoxml.close();

    qDebug()<<"number of friends is equal to"<<friends.length();
    //循环遍历全局文件对象，查找是否有标记文件，标记文件保存着那些好友头像发生了变化的号码
    for (QString&v :friends) {
        QDir tagDir("../userData/"+v+"/friendsInfo");
        qDebug()<<"marked MyQQ is "<<v<<",this represent to need updating user' infomation of name   while next to accesses marked floder";
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
                QJsonArray arr=obj.value("name").toArray();
                QJsonObject temp;
                temp.insert("name",QJsonValue(name));
                temp.insert("number",QJsonValue(myqq));
                arr.append(temp);
                json=QJsonDocument();
                obj.insert("name",QJsonValue(arr));
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
                temp.insert("name",QJsonValue(name));
                temp.insert("number",QJsonValue(myqq));
                arr.append(temp);
                obj.insert("name",QJsonValue(arr));
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
}

void WriteThread::updateXMLSignature(QString &number, QString &me,QString&sig)
{
    QFile infoxml("../userData/"+me+"/info.xml");
    if(!infoxml.open(QIODevice::ReadWrite)){
        qDebug()<<me<<":opened info.xml  unsuceessful";
    }
    qDebug()<<"readed "<<infoxml.fileName();
    QDomDocument doc;
    doc.setContent(&infoxml);
    infoxml.close();
    QDomElement dele=doc.documentElement();
    dele=dele.firstChildElement("friendGroup");
    QDomNodeList listGroup=dele.childNodes();
    int length=listGroup.size();
    for (int var = 0; var <length; ++var) {
        QDomElement ele=listGroup.at(var).toElement();
        if(ele.hasChildNodes()){
            QDomElement friendEle=ele.firstChild().toElement();
            while(!friendEle.isNull()){
                QString mq=friendEle.attribute("myqq");
                if(!mq.isEmpty()){
                    qDebug()<<"a number is found:"<<mq;
                    if(mq==myqq){
                        QDomElement myself=friendEle.firstChildElement(("个性签名"));
                        if(!myself.isNull()){
                            qDebug()<<"find myself "<<mq<<" signature:"<<myself.text();
                            QDomElement newele=doc.createElement(("个性签名"));
                            QDomText text;
                            if(sig.isEmpty()){
                                text=doc.createTextNode("none");
                                newele.setAttribute("isNull","true");
                            }else{
                                text=doc.createTextNode(sig);//保存签名
                                newele.setAttribute("isNull","false");
                            }
                            newele.appendChild(text);
                            friendEle.replaceChild(newele,myself);//替换
                        }
                        var =length;//跳出循环
                        break;
                    }
                }
                friendEle=friendEle.nextSiblingElement();
            }
        }
    }
    infoxml.open(QIODevice::WriteOnly|QIODevice::Truncate);//只写覆盖
    QTextStream stream(&infoxml);
    stream.setCodec("utf-8");
    doc.save(stream,4,QDomNode::EncodingFromTextStream);
    infoxml.close();
}

void WriteThread::updateXMLName(QString &number, QString &me, QString &name)
{
    QFile infoxml("../userData/"+me+"/info.xml");
    if(!infoxml.open(QIODevice::ReadWrite)){
        qDebug()<<me<<":opened info.xml  unsuceessful";
    }
    qDebug()<<"readed "<<infoxml.fileName();
    QDomDocument doc;
    doc.setContent(&infoxml);
    infoxml.close();
    QDomElement dele=doc.documentElement();
    dele=dele.firstChildElement("friendGroup");
    QDomNodeList listGroup=dele.childNodes();
    qint32 length=listGroup.size();
    for (int var = 0; var < length; ++var) {
        QDomElement ele=listGroup.at(var).toElement();
        qDebug()<<ele.tagName();
        if(ele.hasChildNodes()){
            QDomElement friendEle=ele.firstChild().toElement();
            while(!friendEle.isNull()){
                QString mq=friendEle.attribute("myqq");
                if(!mq.isEmpty()){
                    qDebug()<<"a number is found:"<<mq;
                    if(mq==myqq){
                        QDomElement myself=friendEle.firstChildElement(("昵称"));
                        if(!myself.isNull()){
                            qDebug()<<"find myself "<<mq<<" signature:"<<myself.text();
                            QDomElement newele=doc.createElement(("昵称"));
                            QDomText text=doc.createTextNode(name);//保存签名
                            newele.appendChild(text);
                            friendEle.replaceChild(newele,myself);//替换
                        }
                        var=length;//跳出循环
                        break;
                    }
                }
                friendEle=friendEle.nextSiblingElement();
            }
        }
    }
    infoxml.open(QIODevice::WriteOnly|QIODevice::Truncate);//只写覆盖
    QTextStream stream(&infoxml);
    stream.setCodec("utf-8");
    doc.save(stream,4,QDomNode::EncodingFromTextStream);
    infoxml.close();
}

//在线状态收尾处理
bool WriteThread::exitStatusHandle(QString&status)
{
    QFile file("../userData/"+myqq+"/info.xml");
    if(file.open(QIODevice::ReadOnly)){
        qDebug()<<"successfully opened info.xml file ";
        //记录离线状态改变信息
        QString tid;
        std::thread::id id=std::this_thread::get_id();
        std::stringstream sin;
        sin << id;
        tid=QString::fromStdString(sin.str());
        qDebug()<<"threadID : "<<tid;
        QDomDocument infoDoc;
        infoDoc.setContent(&file);
        file.close();//关闭
        QDomElement root=infoDoc.documentElement();
        root=root.firstChildElement("friendGroup");
        QDomNodeList list=root.childNodes();
        qint32 length=list.length();
        for(qint32 i=0;i<length;++i){
            QDomNode node=list.item(i);
            if(node.hasChildNodes()){
                if(node.firstChild().toElement().text()!="none"){
                    QDomNodeList list1=node.childNodes();
                    qint32 length1=list1.length();
                    for(qint32 j=0;j<length1;++j){
                        QDomElement ele=list1.item(j).toElement();
                        QString mq=ele.attribute("myqq");
                        if(mq.isEmpty())continue;
                        //改变自己的状态
                        if(mq==myqq){
                            ele=ele.firstChildElement(("状态"));
                            QString oldStatus=ele.text();
                            if(oldStatus==status)return true;//状态相同还回
                            ele.firstChild().setNodeValue(status);//修改文本节点
                            //保存状态改变信息
                        }else {
                            QDir tdir("../userData/"+mq+"/friendsInfo/chat");
                            if(!tdir.exists()){
                                if(!tdir.mkpath("./")){
                                    qDebug()<<"dir creation is of failure";
                                    continue;
                                }
                            }
                            bool ok=true;
                            QString cn1=tid+mq+QString("%1").arg(QDateTime::currentDateTime().toTime_t());
                            QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",cn1);
                            dbPool.append(cn1);//入池
                            db1.setDatabaseName(QString("../userData/%1/friendsInfo/chat/_%2.db").arg(mq).arg(myqq));
                            db1.setHostName("MyQQ");
                            db1.setUserName("sa");
                            db1.setPassword("@123456x");
                            if(db1.open())
                                qDebug()<<("打开sqliite数据库成功！");
                            else {
                                qDebug()<<("打开sqliite数据库失败！名字:")<<(db1.databaseName());
                                return false;
                            }
                            QSqlQuery query1(db1);
                            ok=  query1.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1status' ").arg(myqq));
                            if(!ok){
                                qDebug()<<"count of table query is of failure ";

                                return false;
                            }
                            query1.next();
                            if(query1.value(0).toInt()==0){
                                ok= query1.exec(QString( " create table _%1status (\
                                                         status varchar(2)\
                                                         )").arg(myqq) );
                                                if(!ok){
                                                    qDebug()<<"created a table is is of failure ";
                                                    continue;
                                                }
                            }
                            ok= query1.exec(QString(" select count(*) from _%1status ").arg(myqq));
                            if(!ok){
                                qDebug()<<" select count(*),status from _%1status: failure";
                                continue;
                            }

                            query1.next();
                            if(query1.value(0).toInt()==0){
                                query1.prepare(QString(" insert into _%1status values(?) ").arg(myqq));
                                query1.addBindValue(QVariant(status));
                                ok= query1.exec();
                                if(!ok){
                                    qDebug()<<"insert into _%1status values('%2'): failure";
                                    continue;
                                }
                            }else{
                                //转义保证非一次性取代时保证符号不发生歧义
                                ok= query1.exec(QString(" update _%1status  set status=\'%2\' ").arg(myqq).arg(status));
                                if(!ok){
                                    qDebug()<<"update _%1status  set status='%2': failure";
                                    continue;
                                }
                            }

                            setXmlStatus(myqq,mq,status);//修改对应文件状态
                        }
                    }
                }
            }
        }
        //保存文件
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
            qDebug()<<"warning:opended for second time the  xml file to alter the status  unsuccessfully ";
            return false ;
        }
        QTextStream stream(&file);
        infoDoc.save(stream,4);
        file.close();
        return true;
    }
    return false;
}
void WriteThread::setXmlStatus(QString &opposite, QString &me, QString &status)
{
    QFile file("../userData/"+me+"/info.xml");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"warning:alter the status in xml file unsuccessfully ";
        return;
    }
    QDomDocument doc;
    doc.setContent(&file);
    file.close();
    QDomElement root=doc.documentElement();
    root=root.firstChildElement("friendGroup");
    QDomNodeList list=root.childNodes();
    qint32 length=list.length();
    for(qint32 i=0;i<length;++i){
        QDomNode node=list.item(i);
        if(node.hasChildNodes()){
            if(node.firstChild().toElement().text()!="none"){
                QDomNodeList list1=node.childNodes();
                qint32 length1=list1.length();
                for(qint32 j=0;j<length1;++j){
                    QDomElement ele=list1.item(j).toElement();
                    QString mq=ele.attribute("myqq");
                    //改变xml状态记录
                    if(mq==opposite){
                        ele=ele.firstChildElement(("状态"));;
                        ele.firstChild().setNodeValue(status);
                        j=length1;
                        i=length;
                        break;//跳出循环
                    }
                }
            }
        }
    }
    //保存文件
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qDebug()<<"warning:opended for second time the opposite xml file to alter the status   unsuccessfully ";
        return ;
    }
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
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
    while (tcpsocket->bytesAvailable()>0&&size>=0) {
        if(size==0){
            QByteArray header=tcpsocket->read(1);
            QDataStream stream(&header,QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_0);
            quint8 l;
            stream>>l;
            if(l<=0){
                qDebug()<<"note:l="<<l;
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
                //退出
                if(in=="-1"){
                    myqq=obj.value("myqq").toString();
                    if(myqq.isEmpty()){
                        qDebug()<<"warning:myqq.isEmpty()";
                        tcpsocket->disconnectFromHost();
                        return;//退出线程
                    }
                    FT=ExitChangeStatus;
                    size=1;
                    continue;
                }
                //4 更新历史头像
                else if(in=="4"){
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
                    myqq=obj.value("myqq").toString();
                    if(myqq.isEmpty()){
                        qDebug()<<"warning:myqq.isEmpty()";
                        tcpsocket->disconnectFromHost();
                        return;//退出线程
                    }
                    QString content=obj.value("content").toString();
                    if(content=="updatePhotoWall"){
                        FT=PhotoWall;
                        size=1;
                        continue;
                        //删除照片
                    }else if(content=="removePhotoWall"){
                        int pos=obj.value("pos").toInt(-1);
                        if(pos==-1){
                            qDebug()<<"went to remove a not existis item ";
                            emit finished();
                            return;
                        }
                        if(updateWall(pos)){
                            qDebug()<<"remove a photo from walls successfully";
                        }
                        emit finished();
                        return;
                    }
                    //更新用户修改的信息
                }else if(in=="10"){
                    QString content=obj.value("content").toString();
                    if(content=="updateUserInformation"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=UserInformation;
                        size=1;
                        continue;
                    }
                    //添加好友分组的信息
                }else if(in=="11"){
                    QString content=obj.value("content").toString();
                    if(content=="addFGroup"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=AddFGroup;
                        size=1;
                        continue;
                    }
                    //移动或删除 rename好友分组
                }else if(in=="15"){
                    QString content=obj.value("content").toString();
                    if(content=="dmrFGroup"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=DMRFGroup;
                        size=1;
                        continue;
                    }
                    //对好友 删除 移动 修改备注 操作
                }else if(in=="16"){
                    QString content=obj.value("content").toString();
                    if(content=="dmrFriend"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=DMRFiend;
                        size=1;
                        continue;
                    }
                }else if(in=="18"){
                    //18  sendFMessage
                    QString content=obj.value("content").toString();
                    if(content=="sendFMessage"){
                        myqq=obj.value("myqq").toString();
                        if(myqq.isEmpty()){
                            qDebug()<<"warning:myqq.isEmpty()";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        fileName=obj.value("number").toString();
                        if( fileName.isEmpty()){
                            qDebug()<<"warning:opposite is empty";
                            tcpsocket->disconnectFromHost();
                            return;//退出线程
                        }
                        FT=FMessageXml;
                        size=1;
                        continue;
                    }
                }
            }
        }else{
            QByteArray data=tcpsocket->readAll();
            switch (FT) {
            case HistoryHeadImage:
            case Signature:
            case CoverImage:
            case PhotoWall:
            case UserInformation:
            case AddFGroup:
            case DMRFGroup:
            case DMRFiend:
                bytes.append(data);
                break;
            case FMessageXml:
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
        //更新用户资料
    case UserInformation:
        if(updateUserInfo(bytes)){
            qDebug()<<"updated user information successfully";
        }
        break;
        //添加用户好友分组
    case AddFGroup:
        if(addFGroup(bytes)){
            qDebug()<<"added a friends group successfully";
        }
        break;
        //退出
    case ExitChangeStatus:
        if(userExit()){
            qDebug()<<" user's status updating successfully";
        }
        break;
        //移动或删除好友分组
    case DMRFGroup:
        if(dmrFGroup(bytes)){
            qDebug()<<" updating the friend groups successfully";
        }
        break;
    case DMRFiend:
        if(dmrFriend(bytes)){
            qDebug()<<" updating the friend successfully";
        }
        break;
        //好友消息保存
    case FMessageXml:
        if(fMessageSave(bytes)){
            qDebug()<<" saved friend message successfully";
        }
        break;
    default:
        break;
    }
    qDebug()<<"emit finished()";
    emit finished();
}

