#include "loginthread.h"
#include<qtcpsocket.h>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include <qsqlquery.h>
#include <qdir.h>
#include <qdatetime.h>
#include<QXmlStreamReader>
#include<qeventloop.h>
#include <qdatastream.h>
#include <QJsonArray>
//因为套接字发送数据可能是粘在一起的，所以要先发送个数据大小
//最好是在发送头字节加个int标记大小，这里用int8标记json数据，因为发送数据的最小单位就是字节了，保证接收到，同时json字节小于256
//大数据的大小就用json标记大小
//开始不懂，写了个json过去，不过json数据量很小，发送很快，几乎不粘包，所以服务器这边就用json处理,就不改了
LoginThread::LoginThread(qintptr socketDescriptor, qint64 count, QObject *parent)
    :QObject(parent),socketDescriptor(socketDescriptor),count(count)
{
    tcpsocket=new QTcpSocket(this);
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<QStringLiteral("创建套接字失败！");
        emit error(tcpsocket->error());
        ;
        return;
    }
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows必设
    connect(tcpsocket,&QTcpSocket::readyRead,this,&LoginThread::readD);
    connect(tcpsocket,&QTcpSocket::disconnected,this,&LoginThread::disconnected);
    connect(tcpsocket,&QTcpSocket::bytesWritten,this,[=](qint64 bytes){
        emit loopStop();
        qDebug()<<bytes<<" bytes has been readed";
    });
}

LoginThread::~LoginThread()
{
    if(tcpsocket){
        tcpsocket->close();
        if(tcpsocket->state()!=QAbstractSocket::UnconnectedState)
            tcpsocket->waitForDisconnected();
        delete tcpsocket;
        tcpsocket=nullptr;
    }

}

qint64 LoginThread::dateSubstraction(const QDate &substracted, const QDate &substraction) const
{
    QDate date1,date2;
    if(substracted<substraction){
        date1=substraction;date2=substracted;
    }else{
        date2=substraction;date1=substracted;
    }
    qint64 age=0;
    qint64 age1,age2,month1,month2,day1,day2;
    age1=date1.year();age2=date2.year();
    month1=date1.month();month2=date2.month();
    day1=date1.day();day2=date2.day();
    age=age1-age2;
    if(month1<month2){
        age-=1;
    }else if(month1==month2){
        if(day1<day2)
            age-=1;
    }
    return age;
}




void LoginThread::readD()
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
        ;
        return;
    }
    qDebug()<<"login"<<db.connectionName()<<db.databaseName();
    QSqlQuery query(" use myqq ",db);
    QJsonDocument readJson=QJsonDocument::fromBinaryData(tcpsocket->readAll());
    QJsonDocument writeJson;//注意每使用一次json文档变量，即使json文档不相同， json文档都会膨胀
    QJsonObject writeObj;
    qint64 byteSize;//写入io设备字节的大小
    QEventLoop loop;//控制写入是否继续
    query.setForwardOnly(true);//设置只进游标
    connect(this,&LoginThread::loopStop,&loop,&QEventLoop::quit);//一旦写入完成结束事件循环
    if(!readJson.isEmpty()){
        qDebug()<<"readText->"<<readJson.toJson().data();
        if(readJson.isObject()){
            QJsonObject obj=readJson.object();
            if(!obj.isEmpty()){
                qDebug()<<"isObject"<<endl;
                QJsonValue in=obj.value("instruct");
                if(in.toString()=="1"){//登录检测
                    qDebug()<<QStringLiteral("登录信息验证")<<endl;
                    QString myqq=obj.value("myqq").toString();
                    QString passwd=obj.value("password").toString();
                    QString hostname=obj.value("hostname").toString();
                    QString ip=obj.value("ip").toString();
                    if(!(myqq.isEmpty()|passwd.isEmpty()|hostname.isEmpty()|ip.isEmpty())){
                        qDebug()<<QStringLiteral("not empty")<<myqq.toLongLong()<<endl;
                        query.prepare(" select myqq,passwd, signLog from userInfo where myqq=? ");
                        query.bindValue(0,myqq.toLongLong());
                        if(query.exec()){
                            qDebug()<<QStringLiteral("execution is successful")<<endl;
                            query.next();
                            if(query.isValid()){
                                qDebug()<<QStringLiteral("isvalid");
                                QString temp=query.value(1).toString();
                                QString logRec=query.value(2).toString();
                                if(temp!=passwd){//密码错误
                                    qDebug()<<QStringLiteral("passwd err")<<endl;
                                    writeObj.insert("instruct",QJsonValue("10"));
                                    writeObj.insert("result",QJsonValue(2));
                                    writeJson.setObject(writeObj);
                                    tcpsocket->write(writeJson.toBinaryData());
                                    tcpsocket->waitForBytesWritten(2000);
                                    return;
                                }
                                logRec= handleLogRec(logRec,hostname,ip);
                                query.prepare(" update userinfo set signLog=?,isConnected=? where myqq=? ");

                                query.bindValue(0,QVariant(logRec));
                                query.bindValue(1,QVariant(bool(1)));
                                query.bindValue(2,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    qDebug()<<QStringLiteral("连接更新成功！");
                                }

                                //获取基本信息，如name,personalizedSignature,activeDays
                                query.prepare("  select myqqName,headImgPath ,personalizedSignature,activeDays ,myqqGrade from userInfo where myqq=? ");
                                query.bindValue(0,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    query.next();
                                    if(query.isValid()){
                                        qDebug()<<QStringLiteral("读取MyQQ(")+myqq+QStringLiteral(")基本信息成功");
                                        QString name=query.value(0).toString();
                                        QString headPath=query.value(1).toString();
                                        QString signature=query.value(2).toString();
                                        qint64 days=query.value(3).toLongLong();
                                        qint8 grade=query.value(4).toInt();
                                        //传头像 png
                                        QFile  headImg(headPath);
                                        if(!headImg.exists()){
                                            qDebug()<<"a error is occured beacuse headImg file doesn't exisit and myqq is "+myqq;
                                        }
                                        qDebug()<<"files"<<headImg.size();
                                        if(headImg.open(QIODevice::ReadOnly)){

                                            //如果有未读好友信息，发送Client
                                            // query.prepare(" select savePath from info_t where typeId=1 and id= ?  and Unsend=1 ");
                                            QByteArray img;
                                            byteSize=headImg.size();
                                            img.resize(byteSize);
                                            //QByteArray 会在写入时复制而不是引用，如此必须设定大小
                                            qint64 imgSize=headImg.read(img.data(),byteSize);

                                            headImg.close();
                                            if(imgSize!=-1){
                                                qDebug()<<"read headImg is successful";
                                                QJsonObject imgobj;
                                                imgobj.insert("instruct",QJsonValue("11"));//头像传输指令
                                                QJsonDocument imgjson(imgobj);
                                                tcpsocket->write(imgjson.toBinaryData());
                                                loop.exec();
                                                tcpsocket->write(img);
                                                loop.exec();
                                                //读取信息文件 如好友 群
                                                QDir infoDir;
                                                infoDir.setPath(headPath);
                                                infoDir.cdUp();
                                                infoDir.cdUp();
                                                QFile infoFile(infoDir.absoluteFilePath("info.xml"));
                                                if(infoFile.open(QIODevice::ReadOnly)){
                                                    qDebug()<<"successfully opened info.xml file ";
                                                    QJsonDocument infojson;
                                                    QJsonObject infoobj;
                                                    byteSize=infoFile.size();
                                                    qDebug()<<"infoFile size:"<<byteSize;
                                                    infoobj.insert("instruct",QJsonValue("12"));
                                                    infojson.setObject(infoobj);
                                                    tcpsocket->write(infojson.toBinaryData());
                                                    loop.exec();
                                                    QByteArray info;
                                                    info.resize(byteSize);
                                                    qint64 infoSize=infoFile.read(info.data(),byteSize);
                                                    infoFile.close();
                                                    if(infoSize!=-1){
                                                        qDebug()<<"read info.xml successfully";
                                                        tcpsocket->write(info);
                                                        loop.exec();
                                                        //完成登录
                                                        qDebug()<<QStringLiteral("finished");
                                                        writeObj.insert("instruct",QJsonValue("10"));
                                                        writeObj.insert("result",QJsonValue(0));
                                                        writeObj.insert("name",QJsonValue(name));
                                                        writeObj.insert("signature",QJsonValue(signature));
                                                        writeObj.insert("days",QJsonValue(days));
                                                        writeObj.insert("grade",QJsonValue(grade));
                                                        writeJson.setObject(writeObj);
                                                        tcpsocket->write(writeJson.toBinaryData());
                                                        loop.exec();
                                                        return;
                                                    }
                                                }
                                            }
                                        }

                                    }
                                }
                            }else{//MyQQ号码错误
                                qDebug()<<QStringLiteral("account err")<<endl;
                                writeObj.insert("instruct",QJsonValue("10"));
                                writeObj.insert("result",QJsonValue(1));
                                writeJson.setObject(writeObj);
                                tcpsocket->write(writeJson.toBinaryData());
                                tcpsocket->waitForBytesWritten(2000);
                                return;
                            }
                        }
                    }
                    //失败执行
                    qDebug()<<QStringLiteral("failed exec")<<endl;
                    writeObj.insert("instruct",QJsonValue("1"));
                    writeObj.insert("result",QJsonValue(3));
                    writeJson.setObject(writeObj);
                    tcpsocket->write(writeJson.toJson());
                    tcpsocket->waitForBytesWritten(2000);
                    return;
                }else if(in.toString()=="2"){//传头像
                    QString myqq=obj.value("myqq").toString();
                    QString content=obj.value("content").toString();
                    qDebug()<<myqq<<content;
                    if(content=="headImg"){
                        QMap<QString,QString>list;
                        QFile info("../userData/"+myqq+"/info.xml");
                        if(info.open(QIODevice::ReadOnly)){
                            QXmlStreamReader reader;
                            reader.setDevice(&info);
                            QString temp=QString();
                            while (!reader.atEnd()) {
                                reader.readNextStartElement();
                                if(reader.name().toString()=="friend"){
                                    temp=reader.attributes().value("myqq").toString();
                                }else if(reader.name().toString()==QStringLiteral("头像路径")){
                                    if(!temp.isEmpty()){
                                        list.insert(temp,reader.readElementText());
                                        temp=QString();
                                    }
                                }
                            }
                            //传图片 图片名 myqq+1
                            QJsonDocument imgJson;//不通过赋值复用json就不会膨胀
                            QJsonObject imgObj;
                            imgObj.insert("instruct",QJsonValue("20"));
                            imgObj.insert("result",QJsonValue("writing"));
                            imgJson.setObject(imgObj);
                            tcpsocket->write(imgJson.toBinaryData());
                            loop.exec();
                            foreach (QString v, list.keys()) {
                                QJsonDocument tempJson;
                                QJsonObject tempObj;
                                tempObj.insert("name",QJsonValue(v+"1"));
                                tempJson.setObject(tempObj);
                                tcpsocket->write(tempJson.toBinaryData());
                                loop.exec();
                                qDebug()<<v<<list.value(v);
                                QFile img(list.value(v));
                                if(img.open(QIODevice::ReadOnly)){
                                    byteSize=img.size();
                                    QByteArray imgData;
                                    imgData.resize(byteSize);
                                    if(img.read(imgData.data(),byteSize)==-1){
                                        qDebug()<<"read "<<writeObj.value("name").toString()+".png had failed";
                                    }
                                    tcpsocket->write(imgData);
                                    loop.exec();
                                    img.close();
                                }else {
                                    writeObj.insert("result",QJsonValue("false"));
                                    writeObj.insert("instruct",QJsonValue("20"));
                                    writeJson.setObject(writeObj);
                                    tcpsocket->write(writeJson.toBinaryData());
                                    loop.exec();
                                    return;
                                }
                            }
                            writeObj.insert("instruct",QJsonValue("20"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            tcpsocket->write(writeJson.toBinaryData());
                            loop.exec();
                            qDebug()<<writeObj;
                            info.close();//关闭文件
                            return;
                        }
                    }
                    writeObj.insert("result",QJsonValue("false"));
                    writeObj.insert("instruct",QJsonValue("20"));
                    writeJson.setObject(writeObj);
                    tcpsocket->write(writeJson.toBinaryData());
                    loop.exec();
                    return;
                    //好友添加界面打开，传城市数据
                }else if(in.toString()=="3"){
                    QString value=obj.value("content").toString();
                    if(value=="city-data"){//传城市数据
                        if(query.exec("select id,name from country")){
                            QString data;
                            query.next();
                            while(query.isValid()) {
                                QString id=query.value(0).toString();
                                QString name=query.value(1).toString();
                                data+=";"+id+" "+name;
                                query.next();
                            }
                            data+=";;";

                            if(query.exec( " select id,name,belongCountryId from province ")){
                                query.next();
                                while (query.isValid()) {
                                    QString idp=query.value(0).toString();
                                    QString namep=query.value(1).toString();
                                    QString idf=query.value(2).toString();
                                    data+=";"+idp+" "+namep+" "+idf;
                                    query.next();
                                }
                                data+=";;";
                                if(query.exec( " select id,name,belongProvinceId from city ")){
                                    query.next();
                                    while (query.isValid()) {
                                        QString idp=query.value(0).toString();
                                        QString namep=query.value(1).toString();
                                        QString idf=query.value(2).toString();
                                        data+=";"+idp+" "+namep+" "+idf;
                                        query.next();
                                    }
                                    data+=";;";
                                }
                                if(query.exec( " select id,name,belongCityId from county ")){
                                    query.next();
                                    while (query.isValid()) {
                                        QString idp=query.value(0).toString();
                                        QString namep=query.value(1).toString();
                                        QString idf=query.value(2).toString();
                                        data+=";"+idp+" "+namep+" "+idf;
                                        query.next();
                                    }
                                    QJsonDocument cityJson;
                                    QJsonObject cityObj;
                                    //写入数据指令 31 开始,因为套接字发送数据可能是粘在一起的，所以要先发送个数据大小
                                    //最好是在发送头字节加个int32标记大小，因为发送数据的最小单位就是字节了,而当下计算机一般时32位，4字节保证接收到
                                    //而不是写个json过去，不过json数据量很小，很难粘包，所以服务器这边就用json处理
                                    cityObj.insert("instruct",QJsonValue("31"));
                                    cityObj.insert("content",QJsonValue("city-data"));
                                    //字节个数而不是字符个数
                                    cityObj.insert("size",QJsonValue(data.toUtf8().size()));
                                    cityJson.setObject(cityObj);
                                    qint8 s=cityJson.toJson().size();//仅仅标记json数据<256 bytes
                                    QByteArray size;
                                    QDataStream stream(&size,QIODevice::WriteOnly);
                                    stream.setVersion(QDataStream::Qt_4_0);
                                    stream<<s;
                                    tcpsocket->write(size+cityJson.toJson());
                                    loop.exec();
                                    qDebug()<<tcpsocket->write(data.toUtf8())<<"bytes";
                                    loop.exec();
                                    qDebug()<<"writed geography-data end successfully";
                                    //写入数据指令 30 结束
                                    writeObj.insert("instruct",QJsonValue("30"));
                                    writeObj.insert("content",QJsonValue("city-data"));
                                    writeObj.insert("result",QJsonValue("true"));
                                    writeJson.setObject(writeObj);
                                    s=writeJson.toJson().size();
                                    QByteArray size1;//重写一个size头 不能复用size，每写入一次都会膨胀
                                    QDataStream stream1(&size1,QIODevice::WriteOnly);
                                    stream1.setVersion(QDataStream::Qt_4_0);
                                    stream1<<s;
                                    tcpsocket->write(size1+writeJson.toJson());
                                    loop.exec();
                                    return;
                                }
                            }
                        }
                        //写入数据指令 30 结束
                        writeObj.insert("instruct",QJsonValue("30"));
                        writeObj.insert("content",QJsonValue("city-data"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        qint8 s=writeJson.toJson().size();
                        QByteArray size;
                        QDataStream stream(&size,QIODevice::WriteOnly);
                        stream.setVersion(QDataStream::Qt_4_0);
                        stream<<s;
                        tcpsocket->write(size+writeJson.toJson());
                        loop.exec();
                        return;
                        //查找好友
                    }else if(value=="find-person"){
                        qint64 myqq=obj.value("myqq").toString().toLongLong();
                        QFile friendInfo("../userData/"+QString("%1").arg(myqq)+"/info.xml");
                        if(friendInfo.open(QIODevice::ReadOnly)){
                            QVariantList friendList;
                            QXmlStreamReader reader;
                            reader.setDevice(&friendInfo);
                            while (!reader.atEnd()) {
                                reader.readNextStartElement();
                                if(reader.name().toString()=="friend"){
                                    friendList.append(reader.attributes().value("myqq").toLongLong());
                                }
                            }
                            friendInfo.close();
                            qint64 beginId=obj.value("begin-id").toString().toLongLong();
                            QString containName=obj.value("name").toString();
                            QString  location=obj.value("location").toString();
                            QString  hometown=obj.value("hometown").toString();
                            QString sex=obj.value("sex").toString();
                            if(sex==QStringLiteral("不限"))
                                sex="";
                            int age=obj.value("age").toInt();
                            QDate beginDate,endDate;
                            switch (age) {
                            default://0或-1
                                beginDate=QDate(1,1,1);
                                endDate=QDate(9999,1,1);
                                break;
                            case 1:
                                beginDate=QDate::currentDate().addYears(-23);
                                endDate=QDate::currentDate().addYears(-18);
                                break;
                            case 2:
                                beginDate=QDate::currentDate().addYears(-27);
                                endDate=QDate::currentDate().addYears(-23);
                                break;
                            case 3:
                                beginDate=QDate::currentDate().addYears(-36);
                                endDate=QDate::currentDate().addYears(-27);
                                break;
                            case 4:
                                beginDate=QDate(1,1,1);
                                endDate=QDate::currentDate().addYears(-36);
                                break;
                            }
                            bool online=obj.value("online").toBool();
                            QStringList list1=location.split(",",QString::SkipEmptyParts);
                            QStringList list2=hometown.split(",",QString::SkipEmptyParts);
                            bool swit=true;
                            QVector<QJsonObject> list;//用户列表
                            query.setForwardOnly(false);//取消只进游标
                            while(swit){//循环获得记录，如果可以，必须要足20个
                                query.prepare(" exec findFriends_pro :begin_id,:containName,:sex,:begin_date,"
                                              ":end_date,:isConnected,:location1,:location2,:location3,:location4,"
                                              ":hometown1,:hometown2,:hometown3,:hometown4 ");
                                query.addBindValue(QVariant(beginId));
                                query.addBindValue(QVariant(containName));
                                query.addBindValue(QVariant(sex));
                                query.addBindValue(QVariant(beginDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(endDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(online));
                                query.addBindValue(QVariant(list1.at(0)==QStringLiteral("不限")?"":list1.at(0)));
                                query.addBindValue(QVariant(list1.size()>1?list1.at(1):""));
                                query.addBindValue(QVariant(list1.size()>2?list1.at(2):""));
                                query.addBindValue(QVariant(list1.size()>3?list1.at(3):""));
                                query.addBindValue(QVariant(list2.at(0)==QStringLiteral("不限")?"":list2.at(0)));
                                query.addBindValue(QVariant(list2.size()>1?list2.at(1):""));
                                query.addBindValue(QVariant(list2.size()>2?list2.at(2):""));
                                query.addBindValue(QVariant(list2.size()>3?list2.at(3):""));
                                /*
数据库中部分代码
proc findFriends_pro
--参数 @begin_id：号码的最小值 ,@containName：昵称包含的文本 @sex：性别  @begin_date和@end_date：生日范围   所在地及故乡
@begin_id bigint,@containName varchar(24),@sex varchar(2),@begin_date date,@end_date datetime,@isConnected bit,
@location1 varchar(20),@location2 varchar(20),@location3 varchar(20),@location4 varchar(20),@hometown1 varchar(20),
@hometown2 varchar(20),@hometown3 varchar(20),@hometown4 varchar(20)
select top 20 myqqName,myqq ,sex,birthday,headImgPath, --昵称 号码 性别 头像 所在地
table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,table1_4.name as location4--所在地
*/
                                if(query.exec()){
                                    qDebug()<<"exec successfully";
                                    query.last();
                                    qint32 count=query.at();
                                    if(count>=0)
                                        count+=1;
                                    else break;
                                    beginId=query.value("myqq").toLongLong()+1;
                                    qDebug()<<count<<"<-count begin-id="<<beginId;
                                    query.first();//回到第一个记录
                                    if(count<20){//不能再获取更多记录了
                                        while (query.isValid()) {
                                            bool ok=true;
                                            for (QVariant v : friendList) {
                                                //过滤好友
                                                if(query.value("myqq").toLongLong()==v.toLongLong()){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            if(ok){//添加非好友信息
                                                QJsonObject temp;
                                                temp.insert("myqq",QJsonValue(query.value("myqq").toLongLong()));
                                                temp.insert("myqqName",QJsonValue(query.value("myqqName").toString()));
                                                temp.insert("sex",QJsonValue(query.value("sex").toString()));
                                                qint64 age=dateSubstraction(QDate::currentDate(),query.value("birthday").toDate());
                                                temp.insert("age",QJsonValue(QString("%1").arg(age)));
                                                temp.insert("imgpath",QJsonValue(query.value("headImgPath").toString()));
                                                temp.insert("location1",QJsonValue(query.value("location1").toString()));
                                                temp.insert("location2",QJsonValue(query.value("location2").toString()));
                                                temp.insert("location3",QJsonValue(query.value("location3").toString()));
                                                temp.insert("location4",QJsonValue(query.value("location4").toString()));
                                                list.append(temp);
                                            }
                                            query.next();
                                        }
                                        break;//不再获取
                                    }else if(count>=20){
                                        while (query.isValid()) {
                                            bool ok=true;
                                            for (QVariant v : friendList) {
                                                //过滤好友
                                                if(query.value("myqq").toLongLong()==v.toLongLong()){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            if(ok){//添加非好友信息
                                                QJsonObject temp;

                                                temp.insert("myqq",QJsonValue(query.value("myqq").toLongLong()));
                                                temp.insert("myqqName",QJsonValue(query.value("myqqName").toString()));
                                                temp.insert("sex",QJsonValue(query.value("sex").toString()));
                                                qint64 age=dateSubstraction(QDate::currentDate(),query.value("birthday").toDate());
                                                temp.insert("age",QJsonValue(QString("%1").arg(age)));
                                                temp.insert("imgpath",QJsonValue(query.value("headImgPath").toString()));
                                                temp.insert("location1",QJsonValue(query.value("location1").toString()));
                                                temp.insert("location2",QJsonValue(query.value("location2").toString()));
                                                temp.insert("location3",QJsonValue(query.value("location3").toString()));
                                                temp.insert("location4",QJsonValue(query.value("location4").toString()));
                                                list.append(temp);
                                            }
                                            query.next();
                                        }
                                        if(list.size()>=20){
                                            break;// 获取20记录，结束循环
                                        }
                                    }
                                }

                            }
                            QJsonDocument doc;
                            QJsonArray array;
                            for(int i=0;i<list.length()&&i<20;i++){//获取至多20个记录
                                QJsonObject obj=list.at(i);
                                array.append(obj);
                            }
                            doc.setArray(array);
                            QJsonDocument userJson;//不通过赋值复用json就不会膨胀
                            QJsonObject userObj;
                            userObj.insert("instruct",QJsonValue("31"));
                            userObj.insert("content",QJsonValue("find-person"));
                            userObj.insert("size",QJsonValue(doc.toJson().size()));
                            userObj.insert("begin-id",QJsonValue(QString("%1").arg(beginId)));//转为字符串 因为qjsonvalue不支持int64
                            userJson.setObject(userObj);
                            qint8 s=userJson.toJson().size();//仅仅标记json数据<256 bytes
                            QByteArray size;
                            QDataStream stream(&size,QIODevice::WriteOnly);
                            stream.setVersion(QDataStream::Qt_4_0);
                            stream<<s;
                            tcpsocket->write(size+userJson.toJson());
                            loop.exec();
                            tcpsocket->write(doc.toJson());//写入好友
                            loop.exec();
                            //传图片 myqq.png
                            for (QJsonValue v : array) {
                                QJsonObject obj=v.toObject();
                                QString path=obj.value("imgpath").toString();
                                QString number=QString("%1").arg(obj.value("myqq").toVariant().toLongLong());
                                qDebug()<<number;
                                QString fullpath="../userData/"+number+"/historyHeadImg/01.png";
                                QFile img(fullpath);
                                if(img.open(QIODevice::ReadOnly)){
                                    byteSize=img.size();
                                    QByteArray imgData;
                                    imgData.resize(byteSize);
                                    if(img.read(imgData.data(),byteSize)==-1){
                                        qDebug()<<"read "<<fullpath+".png had failed";
                                    }
                                    //写入数据指令 32 传好友图像
                                    QJsonDocument imgJson;//
                                    QJsonObject imgObj;
                                    imgObj.insert("instruct",QJsonValue("32"));
                                    imgObj.insert("content",QJsonValue("find-person"));
                                    imgObj.insert("imgname",QJsonValue(number+".png"));
                                    imgObj.insert("size",QJsonValue(byteSize));
                                    imgJson.setObject(imgObj);
                                    s=imgJson.toJson().size();
                                    QByteArray size1;//重写一个size头 不能复用size，每写入一次都会膨胀
                                    QDataStream stream1(&size1,QIODevice::WriteOnly);
                                    stream1.setVersion(QDataStream::Qt_4_0);
                                    stream1<<s;
                                    tcpsocket->write(size1+imgJson.toJson());
                                    loop.exec();
                                    tcpsocket->write(imgData);
                                    loop.exec();
                                    img.close();//关闭文件
                                }else qDebug()<<"opened image had failed";
                            }
                            //写入数据指令 30 结束
                            writeObj.insert("instruct",QJsonValue("30"));
                            writeObj.insert("content",QJsonValue("find-person"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            QByteArray size1;//重写一个size头 不能复用size，每写入一次都会膨胀
                            QDataStream stream1(&size1,QIODevice::WriteOnly);
                            stream1.setVersion(QDataStream::Qt_4_0);
                            stream1<<s;
                            tcpsocket->write(size1+writeJson.toJson());
                            loop.exec();
                            return;
                        }
                        //写入数据指令 30 结束
                        writeObj.insert("instruct",QJsonValue("30"));
                        writeObj.insert("content",QJsonValue("find-person"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        qint8 s=writeJson.toJson().size();
                        QByteArray size;
                        QDataStream stream(&size,QIODevice::WriteOnly);
                        stream.setVersion(QDataStream::Qt_4_0);
                        stream<<s;
                        tcpsocket->write(size+writeJson.toJson());
                        loop.exec();
                        return;
                        //查找好友
                    }
                }
            }
        }
    }
}
//客户端断开连接就删除线程及对象
void LoginThread::disconnected()
{
    qDebug()<<"disconnected";
    emit finished();
}

QString LoginThread::handleLogRec(QString &rec,QString& host,QString&ip) const
{
    QStringList list=rec.split(";;",QString::SkipEmptyParts);
    int count=list.count(),hostRow=-1;
    for(int i=0;i<count;i++){
        if(list.at(i).contains(host)){
            hostRow=i;
        }
    }
    QDateTime time=QDateTime::currentDateTime();
    QString newRec=host+"  "+ip+" "+time.toString("yyyy/M/d hh:mm:ss PM");
    QString result;
    if(hostRow!=-1){
        for(int i=hostRow;i>0;){
            list.replace(i,list.at(--i));
        }
        list.replace(0,newRec);
    }else{
        if(count>7){
            for(int i=count-1;i>0;){
                list.replace(i,list.at(--i));
            }
            list.replace(0,newRec);
        }else
            list.insert(list.begin(),newRec);
    }
    result=list.join(";;");
    return result;
}


