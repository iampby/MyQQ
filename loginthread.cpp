#include "loginthread.h"
#include"global.h"
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
#include <qtimer.h>
#include<qthread.h>
#include<qdir.h>
//��Ϊ�׽��ַ������ݿ�����ճ��һ��ģ�����Ҫ�ȷ��͸����ݴ�С
//������ڷ���ͷ�ֽڼӸ�int��Ǵ�С��������int8���json���ݣ���Ϊ�������ݵ���С��λ�����ֽ��ˣ���֤���յ���ͬʱjson�ֽ�С��256
//��Ȼuint32 ����õ���Ϊһ�����������λ��Ϊһ��Ϊ32λ����
//�����ݵĴ�С����json��Ǵ�С
//��ʼ������д�˸�json��ȥ������json��������С�����ͺܿ죬������ճ�������Է�������߾���json����,�Ͳ�����
LoginThread::LoginThread(qintptr socketDescriptor, qint64 count, QObject *parent)
    :QObject(parent),socketDescriptor(socketDescriptor),count(count)
{
    tcpsocket=new QTcpSocket(this);//tcpsocket ����̳и��� ����������ʹ���߳�
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<QStringLiteral("�����׽���ʧ�ܣ�");
        emit error(tcpsocket->error());
        ;
        return;
    }
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows����
    connect(tcpsocket,&QTcpSocket::readyRead,this,&LoginThread::readD);
    connect(this,&LoginThread::startTimer,this,&LoginThread::timer);
    connect(tcpsocket,&QTcpSocket::disconnected,this,&LoginThread::disconnected);
    connect(tcpsocket,&QTcpSocket::bytesWritten,this,[=](qint64 bytes){
        emit loopStop();
        qDebug()<<"bytesWritten:"<<bytes<<"bytes";
    });
}

LoginThread::~LoginThread()
{
    qDebug()<<"~LoginThread()";
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

void LoginThread::timer()
{
    //���̵߳��ö�ʱ��
    QTimer::singleShot(60000,Qt::CoarseTimer,this,[=](){
        qDebug()<<"timer 60s exit";
        emit finished();
        qDebug()<<"thread had exited";
    });
}




void LoginThread::readD()
{

    //�þ�ȷ�����ʱ���100��ֵ��Χ��count����������1���ڿ��Դ򿪵����ݿ����ӣ���100��
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
        qDebug()<<QStringLiteral("�����ݿ�ɹ���");
    else {
        qDebug()<<QStringLiteral("�����ݿ�ʧ�ܣ�");
        emit finished();//����ͬʱ�����ݿ�����������ԭ�����
        return;
    }
    qDebug()<<"login"<<db.connectionName()<<db.databaseName();
    QSqlQuery query(" use myqq ",db);
    QJsonDocument readJson=QJsonDocument::fromBinaryData(tcpsocket->readAll());
    QJsonDocument writeJson;//ע��ÿʹ��һ��json�ĵ���������ʹjson�ĵ�����ͬ�� json�ĵ���������
    QJsonObject writeObj;
    qint64 byteSize;//д��io�豸�ֽڵĴ�С
    QEventLoop loop;//����д���Ƿ����
    query.setForwardOnly(true);//����ֻ���α�
    connect(this,&LoginThread::loopStop,&loop,&QEventLoop::quit);//һ��д����ɽ����¼�ѭ��
    if(!readJson.isEmpty()){
        if(readJson.isObject()){
            QJsonObject obj=readJson.object();
            if(!obj.isEmpty()){
                qDebug()<<"isObject"<<endl;
                QJsonValue in=obj.value("instruct");
                if(in.toString()=="1"){//��¼���
                    qDebug()<<QStringLiteral("��¼��Ϣ��֤")<<endl;
                    QString myqq=obj.value("myqq").toString();
                    QString passwd=obj.value("password").toString();
                    QString hostname=obj.value("hostname").toString();
                    QString ip=obj.value("ip").toString();
                    if(!(myqq.isEmpty()|passwd.isEmpty()|hostname.isEmpty()|ip.isEmpty())){
                        qDebug()<<QStringLiteral("not empty")<<myqq.toLongLong()<<endl;
                        query.prepare(" select myqq,passwd,signLog from userInfo where myqq=? ");
                        query.bindValue(0,myqq.toLongLong());
                        if(query.exec()){
                            qDebug()<<QStringLiteral("execution is successful")<<endl;
                            query.next();
                            if(query.isValid()){
                                qDebug()<<QStringLiteral("isvalid");
                                QString temp=query.value(1).toString();
                                QString logRec=query.value(2).toString();
                                if(temp!=passwd){//�������
                                    qDebug()<<QStringLiteral("passwd err")<<endl;
                                    writeObj.insert("instruct",QJsonValue("10"));
                                    writeObj.insert("result",QJsonValue(2));
                                    writeJson.setObject(writeObj);
                                    quint8  s=writeJson.toJson().size();
                                    QByteArray size;//д������ͷ
                                    QDataStream stream(&size,QIODevice::WriteOnly);
                                    stream.setVersion(QDataStream::Qt_4_0);
                                    stream<<s;
                                    tcpsocket->write(size+writeJson.toJson());
                                    tcpsocket->waitForBytesWritten(2000);
                                    return;
                                }
                                logRec= handleLogRec(logRec,hostname,ip);
                                query.prepare(" update userinfo set signLog=?,isConnected=? where myqq=? ");

                                query.bindValue(0,QVariant(logRec));
                                query.bindValue(1,QVariant(bool(1)));
                                query.bindValue(2,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    qDebug()<<QStringLiteral("���Ӹ��³ɹ���");
                                }

                                //��ȡ������Ϣ����name,personalizedSignature,activeDays
                                query.prepare("  select myqqName,sex,headImgPath,personalizedSignature,activeDays ,myqqGrade,userState,homeTownCountry,homeTownProvince,"
                                              "homeTownCity,homeTown,locateInCountry,locateInProvince,locateInCity,locateInTown from userInfo where myqq=? ");
                                query.bindValue(0,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    query.next();
                                    if(query.isValid()){
                                        qDebug()<<QStringLiteral("��ȡMyQQ(")+myqq+QStringLiteral(")������Ϣ�ɹ�");
                                        QString name=query.value(0).toString();//�ǳ�
                                        QString sex=query.value(1).toString();//�Ա�
                                        QString headPath=query.value(2).toString();//ͷ��·��
                                        QString signature=query.value(3).toString();//����ǩ��
                                        qint64 days=query.value(4).toLongLong();//��Ծ����
                                        quint8 grade=query.value(5).toInt();//�ȼ�
                                        QString status=query.value(6).toString();//״̬
                                        QString hometown=query.value(7).toString()+","+query.value(8).toString()+","+query.value(9).toString()+","+query.value(10).toString();
                                        QString where=query.value(11).toString()+","+query.value(12).toString()+","+query.value(13).toString()+","+query.value(14).toString();
                                        //��ͷ�� png
                                        qDebug()<<headPath;
                                        QFile  headImg(headPath);
                                        if(!headImg.exists()){
                                            qDebug()<<"a error is occured beacuse headImg file doesn't exisit and myqq is "+myqq;
                                        }
                                        qDebug()<<"files"<<headImg.size();
                                        if(headImg.open(QIODevice::ReadOnly)){

                                            //�����δ��������Ϣ������Client
                                            // query.prepare(" select savePath from info_t where typeId=1 and id= ?  and Unsend=1 ");
                                            QByteArray img;
                                            byteSize=headImg.size();
                                            img.resize(byteSize);
                                            //QByteArray ����д��ʱ���ƶ��������ã���˱����趨��С
                                            qint64 imgSize=headImg.read(img.data(),byteSize);
                                            headImg.close();
                                            if(imgSize!=-1){
                                                qDebug()<<"read headImg is successful";
                                                QJsonObject imgobj;
                                                imgobj.insert("instruct",QJsonValue("11"));//ͷ����ָ��
                                                imgobj.insert("size",QJsonValue(imgSize));
                                                QJsonDocument imgjson(imgobj);
                                                quint8 s=imgjson.toJson().size();
                                                QByteArray size;//����ͷ��С
                                                QDataStream stream(&size,QIODevice::WriteOnly);
                                                stream.setVersion(QDataStream::Qt_4_0);
                                                stream<<s;
                                                tcpsocket->write(size+imgjson.toJson());
                                                loop.exec();//�ȴ�д��
                                                tcpsocket->write(img);
                                                loop.exec();
                                                //��ȡ��Ϣ�ļ� ����� Ⱥ
                                                QDir infoDir;
                                                infoDir.setPath(headPath);
                                                infoDir.cdUp();
                                                infoDir.cdUp();
                                                QFile infoFile(infoDir.absoluteFilePath("info.xml"));
                                                if(infoFile.open(QIODevice::ReadOnly)){
                                                    qDebug()<<"successfully opened info.xml file ";
                                                    byteSize=infoFile.size();
                                                    QJsonDocument infojson;
                                                    QJsonObject infoobj;
                                                    qDebug()<<"infoFile size:"<<byteSize;
                                                    infoobj.insert("instruct",QJsonValue("12"));
                                                    infoobj.insert("size",QJsonValue(byteSize));
                                                    infojson.setObject(infoobj);
                                                    s=infojson.toJson().size();
                                                    QByteArray size2;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                                                    QDataStream stream2(&size2,QIODevice::WriteOnly);
                                                    stream2.setVersion(QDataStream::Qt_4_0);
                                                    stream2<<s;
                                                    tcpsocket->write(size2+infojson.toJson());
                                                    loop.exec();
                                                    QByteArray info;
                                                    info.resize(byteSize);
                                                    qint64 infoSize=infoFile.read(info.data(),byteSize);
                                                    infoFile.close();
                                                    if(infoSize!=-1){
                                                        qDebug()<<"read info.xml successfully";
                                                        tcpsocket->write(info.data(),infoSize);
                                                        loop.exec();
                                                        //��ɵ�¼
                                                        qDebug()<<QStringLiteral("finished");
                                                        writeObj.insert("instruct",QJsonValue("10"));
                                                        writeObj.insert("result",QJsonValue(0));
                                                        writeObj.insert("name",QJsonValue(name));
                                                        writeObj.insert("sex",QJsonValue(sex));
                                                        writeObj.insert("signature",QJsonValue(signature));
                                                        writeObj.insert("days",QJsonValue(days));
                                                        writeObj.insert("grade",QJsonValue(grade));
                                                        writeObj.insert("status",QJsonValue(status));
                                                        writeObj.insert(QStringLiteral("���ڵ�"),QJsonValue(where));
                                                        writeObj.insert(QStringLiteral("����"),QJsonValue(hometown));
                                                        writeJson.setObject(writeObj);
                                                        s=writeJson.toJson().size();
                                                        QByteArray size3;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                                                        QDataStream stream3(&size3,QIODevice::WriteOnly);
                                                        stream3.setVersion(QDataStream::Qt_4_0);
                                                        stream3<<s;
                                                        tcpsocket->write(size3+writeJson.toJson());
                                                        loop.exec();
                                                        return;
                                                    }
                                                }
                                            }
                                        }

                                    }
                                }
                            }else{//MyQQ�������
                                qDebug()<<QStringLiteral("account err")<<endl;
                                writeObj.insert("instruct",QJsonValue("10"));
                                writeObj.insert("result",QJsonValue(1));
                                writeJson.setObject(writeObj);
                                quint8 s=writeJson.toJson().size();
                                QByteArray size;
                                QDataStream stream(&size,QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_4_0);
                                stream<<s;
                                tcpsocket->write(size+writeJson.toJson());
                                tcpsocket->waitForBytesWritten(2000);
                                return;
                            }
                        }
                    }
                    //ʧ��ִ��
                    qDebug()<<QStringLiteral("failed exec")<<endl;
                    writeObj.insert("instruct",QJsonValue("1"));
                    writeObj.insert("result",QJsonValue(3));
                    writeJson.setObject(writeObj);
                    quint8 s=writeJson.toJson().size();
                    QByteArray size;
                    QDataStream stream(&size,QIODevice::WriteOnly);
                    stream.setVersion(QDataStream::Qt_4_0);
                    stream<<s;
                    tcpsocket->write(size+writeJson.toJson());
                    loop.exec();
                    return;
                }else if(in.toString()=="2"){//��ͷ��
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
                                }else if(reader.name().toString()==QStringLiteral("ͷ��·��")){
                                    if(!temp.isEmpty()){
                                        list.insert(temp,reader.readElementText());
                                        temp=QString();
                                    }
                                }
                            }

                            foreach (QString v, list.keys()) {
                                QFile img(list.value(v));
                                if(img.open(QIODevice::ReadOnly)){
                                    byteSize=img.size();
                                    QByteArray imgData;
                                    imgData.resize(byteSize);
                                    if(img.read(imgData.data(),byteSize)==-1){
                                        qDebug()<<"read "<<writeObj.value("name").toString()+".png had failed";
                                    }
                                    //��ͼƬ ͼƬ�� myqq+1
                                    QJsonDocument imgJson;
                                    QJsonObject imgObj;
                                    imgObj.insert("instruct",QJsonValue("20"));
                                    imgObj.insert("result",QJsonValue("writing"));
                                    imgObj.insert("name",QJsonValue(v+"1"+".png"));
                                    imgObj.insert("size",QJsonValue(byteSize));
                                    imgJson.setObject(imgObj);
                                    quint8 s=imgJson.toJson().size();
                                    QByteArray size;
                                    QDataStream stream(&size,QIODevice::WriteOnly);
                                    stream.setVersion(QDataStream::Qt_4_0);
                                    stream<<s;
                                    tcpsocket->write(size+imgJson.toJson());
                                    loop.exec();
                                    qDebug()<<v<<list.value(v);
                                    tcpsocket->write(imgData);
                                    loop.exec();
                                    img.close();
                                }else {
                                    writeObj.insert("result",QJsonValue("false"));
                                    writeObj.insert("instruct",QJsonValue("20"));
                                    writeJson.setObject(writeObj);
                                    quint8 s1=writeJson.toJson().size();
                                    QByteArray size1;
                                    QDataStream stream1(&size1,QIODevice::WriteOnly);
                                    stream1.setVersion(QDataStream::Qt_4_0);
                                    stream1<<s1;
                                    tcpsocket->write(size1+writeJson.toJson());
                                    loop.exec();
                                    return;
                                }
                            }
                            writeObj.insert("instruct",QJsonValue("20"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            quint8 s2=writeJson.toJson().size();
                            QByteArray size2;
                            QDataStream stream2(&size2,QIODevice::WriteOnly);
                            stream2.setVersion(QDataStream::Qt_4_0);
                            stream2<<s2;
                            tcpsocket->write(size2+writeJson.toJson());
                            loop.exec();
                            info.close();//�ر��ļ�
                            return;
                        }
                    }
                    writeObj.insert("result",QJsonValue("false"));
                    writeObj.insert("instruct",QJsonValue("20"));
                    writeJson.setObject(writeObj);
                    quint8 s=writeJson.toJson().size();
                    QByteArray size;
                    QDataStream stream(&size,QIODevice::WriteOnly);
                    stream.setVersion(QDataStream::Qt_4_0);
                    stream<<s;
                    tcpsocket->write(size+writeJson.toJson());
                    loop.exec();
                    return;
                    //������ӽ���򿪴���
                }else if(in.toString()=="3"){
                    QString value=obj.value("content").toString();
                    if(value=="city-data"){//����������
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
                                    //д������ָ�� 31 ��ʼ,��Ϊ�׽��ַ������ݿ�����ճ��һ��ģ�����Ҫ�ȷ��͸����ݴ�С
                                    //������ڷ���ͷ�ֽڼӸ�int32��Ǵ�С����Ϊ�������ݵ���С��λ�����ֽ���,�����¼����һ��ʱ32λ��4�ֽڱ�֤���յ�
                                    //������д��json��ȥ������json��������С������ճ�������Է�������߾���json����
                                    cityObj.insert("instruct",QJsonValue("31"));
                                    cityObj.insert("content",QJsonValue("city-data"));
                                    //�ֽڸ����������ַ�����
                                    cityObj.insert("size",QJsonValue(data.toUtf8().size()));
                                    cityJson.setObject(cityObj);
                                    quint8 s=cityJson.toJson().size();//�������json����<256 bytes
                                    QByteArray size;
                                    QDataStream stream(&size,QIODevice::WriteOnly);
                                    stream.setVersion(QDataStream::Qt_4_0);
                                    stream<<s;
                                    tcpsocket->write(size+cityJson.toJson());
                                    loop.exec();
                                    qDebug()<<tcpsocket->write(data.toUtf8())<<"bytes";
                                    loop.exec();
                                    qDebug()<<"writed geography-data end successfully";
                                    //д������ָ�� 30 ����
                                    writeObj.insert("instruct",QJsonValue("30"));
                                    writeObj.insert("content",QJsonValue("city-data"));
                                    writeObj.insert("result",QJsonValue("true"));
                                    writeJson.setObject(writeObj);
                                    s=writeJson.toJson().size();
                                    QByteArray size1;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                                    QDataStream stream1(&size1,QIODevice::WriteOnly);
                                    stream1.setVersion(QDataStream::Qt_4_0);
                                    stream1<<s;
                                    tcpsocket->write(size1+writeJson.toJson());
                                    loop.exec();
                                    return;
                                }
                            }
                        }
                        //д������ָ�� 30 ����
                        writeObj.insert("instruct",QJsonValue("30"));
                        writeObj.insert("content",QJsonValue("city-data"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        quint8 s=writeJson.toJson().size();
                        QByteArray size;
                        QDataStream stream(&size,QIODevice::WriteOnly);
                        stream.setVersion(QDataStream::Qt_4_0);
                        stream<<s;
                        tcpsocket->write(size+writeJson.toJson());
                        loop.exec();
                        return;
                        //���Һ���
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
                            if(sex==QStringLiteral("����"))
                                sex="";
                            int age=obj.value("age").toInt();
                            QDate beginDate,endDate;
                            switch (age) {
                            default://0��-1
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
                            QVector<QJsonObject> list;//�û��б�
                            query.setForwardOnly(false);//ȡ��ֻ���α�
                            while(swit){//ѭ����ü�¼��������ԣ�����Ҫ��20��
                                query.prepare(" exec findFriends_pro :begin_id,:containName,:sex,:begin_date,"
                                              ":end_date,:isConnected,:location1,:location2,:location3,:location4,"
                                              ":hometown1,:hometown2,:hometown3,:hometown4 ");
                                query.addBindValue(QVariant(beginId));
                                query.addBindValue(QVariant(containName));
                                query.addBindValue(QVariant(sex));
                                query.addBindValue(QVariant(beginDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(endDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(online));
                                query.addBindValue(QVariant(list1.at(0)==QStringLiteral("����")?"":list1.at(0)));
                                query.addBindValue(QVariant(list1.size()>1?list1.at(1):""));
                                query.addBindValue(QVariant(list1.size()>2?list1.at(2):""));
                                query.addBindValue(QVariant(list1.size()>3?list1.at(3):""));
                                query.addBindValue(QVariant(list2.at(0)==QStringLiteral("����")?"":list2.at(0)));
                                query.addBindValue(QVariant(list2.size()>1?list2.at(1):""));
                                query.addBindValue(QVariant(list2.size()>2?list2.at(2):""));
                                query.addBindValue(QVariant(list2.size()>3?list2.at(3):""));
                                /*
���ݿ��в��ִ���
proc findFriends_pro
--���� @begin_id���������Сֵ ,@containName���ǳư������ı� @sex���Ա�  @begin_date��@end_date�����շ�Χ   ���ڵؼ�����
@begin_id bigint,@containName varchar(24),@sex varchar(2),@begin_date date,@end_date datetime,@isConnected bit,
@location1 varchar(20),@location2 varchar(20),@location3 varchar(20),@location4 varchar(20),@hometown1 varchar(20),
@hometown2 varchar(20),@hometown3 varchar(20),@hometown4 varchar(20)
select top 20 myqqName,myqq ,sex,birthday,headImgPath, --�ǳ� ���� �Ա� ͷ�� ���ڵ�
table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,table1_4.name as location4--���ڵ�
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
                                    query.first();//�ص���һ����¼
                                    if(count<20){//�����ٻ�ȡ�����¼��
                                        while (query.isValid()) {
                                            bool ok=true;
                                            for (QVariant v : friendList) {
                                                //���˺���
                                                if(query.value("myqq").toLongLong()==v.toLongLong()){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            if(ok){//��ӷǺ�����Ϣ
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
                                        break;//���ٻ�ȡ
                                    }else if(count>=20){
                                        while (query.isValid()) {
                                            bool ok=true;
                                            for (QVariant v : friendList) {
                                                //���˺���
                                                if(query.value("myqq").toLongLong()==v.toLongLong()){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            if(ok){//��ӷǺ�����Ϣ
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
                                            break;// ��ȡ20��¼������ѭ��
                                        }
                                    }
                                }

                            }
                            QJsonDocument doc;
                            QJsonArray array;
                            for(int i=0;i<list.length()&&i<20;i++){//��ȡ����20����¼
                                QJsonObject obj=list.at(i);
                                array.append(obj);
                            }
                            doc.setArray(array);
                            QJsonDocument userJson;//��ͨ����ֵ����json�Ͳ�������
                            QJsonObject userObj;
                            userObj.insert("instruct",QJsonValue("31"));
                            userObj.insert("content",QJsonValue("find-person"));
                            userObj.insert("size",QJsonValue(doc.toJson().size()));
                            userObj.insert("begin-id",QJsonValue(QString("%1").arg(beginId)));//תΪ�ַ��� ��Ϊqjsonvalue��֧��int64
                            userJson.setObject(userObj);
                            quint8 s=userJson.toJson().size();//�������json����<256 bytes
                            QByteArray size;
                            QDataStream stream(&size,QIODevice::WriteOnly);
                            stream.setVersion(QDataStream::Qt_4_0);
                            stream<<s;
                            tcpsocket->write(size+userJson.toJson());
                            loop.exec();
                            tcpsocket->write(doc.toJson());//д�����
                            loop.exec();
                            //��ͼƬ myqq.png
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
                                    //д������ָ�� 32 ������ͼ��
                                    QJsonDocument imgJson;//
                                    QJsonObject imgObj;
                                    imgObj.insert("instruct",QJsonValue("32"));
                                    imgObj.insert("content",QJsonValue("find-person"));
                                    imgObj.insert("imgname",QJsonValue(number+".png"));
                                    imgObj.insert("size",QJsonValue(byteSize));
                                    imgJson.setObject(imgObj);
                                    s=imgJson.toJson().size();
                                    QByteArray size1;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                                    QDataStream stream1(&size1,QIODevice::WriteOnly);
                                    stream1.setVersion(QDataStream::Qt_4_0);
                                    stream1<<s;
                                    tcpsocket->write(size1+imgJson.toJson());
                                    loop.exec();
                                    tcpsocket->write(imgData);
                                    loop.exec();
                                    img.close();//�ر��ļ�
                                }else qDebug()<<"opened image had failed";
                            }
                            //д������ָ�� 30 ����
                            writeObj.insert("instruct",QJsonValue("30"));
                            writeObj.insert("content",QJsonValue("find-person"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            QByteArray size1;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                            QDataStream stream1(&size1,QIODevice::WriteOnly);
                            stream1.setVersion(QDataStream::Qt_4_0);
                            stream1<<s;
                            tcpsocket->write(size1+writeJson.toJson());
                            loop.exec();
                            return;
                        }
                        //д������ָ�� 30 ����
                        writeObj.insert("instruct",QJsonValue("30"));
                        writeObj.insert("content",QJsonValue("find-person"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        quint8 s=writeJson.toJson().size();
                        QByteArray size;
                        QDataStream stream(&size,QIODevice::WriteOnly);
                        stream.setVersion(QDataStream::Qt_4_0);
                        stream<<s;
                        tcpsocket->write(size+writeJson.toJson());
                        loop.exec();
                        return;
                        //���Һ���
                    }
                    //��ʷͷ���ȡ  4 historyHeadImg "+myqq
                }else if(in.toString()=="4"){
                    QString value=obj.value("content").toString();
                    //��ʷͷ���ȡ  4 historyHeadImg "+myqq
                    if(value=="historyHeadImg"){
                        QString myqq=obj.value("myqq").toString();//myqq����
                        QDir dir("../userData/"+myqq+"/historyHeadImg/");//��λ��ͷ���ļ���
                        if(!dir.exists()){
                            qDebug()<<"file is not exists";
                            goto label;//��ת��β
                        }
                        QStringList temp;
                        temp<<"*.png";
                        QStringList filelist=dir.entryList(temp,QDir::Files);
                        if(filelist.isEmpty()){
                            qDebug()<<"it's is empty for dir of history-image";
                            goto label;//��ת��β
                        }

                        qDebug()<<filelist.length()<<"filelist";
                        for (QString& v : filelist) {
                            qDebug()<<"found a file,named: "<<v;
                            if(v=="01.png"){
                                qDebug()<<"found myqq file";
                                continue;
                            }
                            QFile ofile(dir.filePath(v));
                            qDebug()<<"a file that was not successfully opened, named "<<dir.filePath(v);
                            if(!ofile.open(QIODevice::ReadOnly)){
                                qDebug()<<"a file that was not successfully opened, named "<<dir.filePath(v);
                                goto label;//��ת��β
                            }
                            byteSize=ofile.size();
                            QByteArray filedata;filedata.resize(byteSize);
                            if(ofile.read(filedata.data(),byteSize)==-1){
                                qDebug()<<"readed file is of failure" ;
                                break;
                            };
                            //ÿ��Ҫ���
                            writeObj.insert("name",QJsonValue(v));
                            writeObj.insert("size",QJsonValue(byteSize));
                            writeObj.insert("instruct",QJsonValue("40"));
                            writeObj.insert("result",QJsonValue("writing"));
                            writeJson.setObject(writeObj);
                            QByteArray sizedata;
                            quint8 l=writeJson.toJson().size();
                            QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l;
                            tcpsocket->write(sizedata+writeJson.toJson());
                            loop.exec();
                            tcpsocket->write(filedata);
                            loop.exec();
                        }
                        writeObj=QJsonObject();
                        writeJson=QJsonDocument();
                        writeObj.insert("instruct",QJsonValue("40"));
                        writeObj.insert("result",QJsonValue("true"));
                        writeJson.setObject(writeObj);
                        QByteArray sizedata;
                        quint8 l2=writeJson.toJson().size();
                        QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                        sizeStr.setVersion(QDataStream::Qt_4_0);
                        sizeStr<<l2;
                        tcpsocket->write(sizedata+ writeJson.toJson());
                        loop.exec();
                        return;
                    }

                    //�����⣬��ת����ʧ�����
                    //��β��� ��ǩ
label:
                    qDebug()<<"failed";
                    writeObj.insert("instruct",QJsonValue("40"));
                    writeObj.insert("result",QJsonValue("false"));
                    writeJson.setObject(writeObj);
                    tcpsocket->write(writeJson.toJson());
                    loop.exec();
                    //���޸����ݻ�user
                }else if(in.toString()=="6"){
                    QString value=obj.value("content").toString();
                    if(value=="updateUser"){
                        bool ok=true;//Ĭ�ϳɹ�
                        QString myqq=obj.value("myqq").toString();
                        QDir dir("../userData/"+myqq+"/friendsInfo");
                        if(dir.exists()){
                            if(historyImgFiles.contains(myqq)){
                                QMutex*muter=historyImgMuter.value(myqq);
                                muter->lock();
                                QFile* headimg=historyImgFiles.value(myqq);
                                if(headimg->exists()){
                                    qDebug()<<headimg->fileName()<<" is found";
                                    if(headimg->open(QIODevice::ReadOnly)){
                                        qDebug()<<"opened the file";
                                        QJsonDocument json=QJsonDocument::fromJson(headimg->readAll());
                                        if(!json.isObject()){
                                            qDebug()<<" json is not object";
                                            ok=false;
                                        }
                                        QJsonObject obj=json.object();
                                        QJsonArray arr=obj.value("myqq").toArray();
                                        if(arr.isEmpty())ok=false;
                                        else{
                                            qint64 length= arr.size();
                                            for (int var = 0; var < length; ++var) {
                                                QJsonObject temp= arr.at(var).toObject();
                                                if(temp.isEmpty()){
                                                    qDebug()<<"warning:occured a empty obj in markedHeadImg.json";
                                                    continue;
                                                }
                                                QString mq=temp.value("number").toString();
                                                QFile tempFile("../userData/"+mq+"/historyHeadImg/01.png");
                                                if(!tempFile.open(QIODevice::ReadOnly)){
                                                    qDebug()<<"warning:opened a file unsuccessful,named "<<tempFile.fileName();
                                                    continue;
                                                }else
                                                    qDebug()<<"friend's MyQQ is "<<mq;
                                                QByteArray data=tempFile.readAll();
                                                tempFile.close();
                                                QJsonDocument tempDoc;
                                                QJsonObject tempObj;
                                                tempObj.insert("content",QJsonValue("headImg"));
                                                tempObj.insert("size",QJsonValue(data.size()));
                                                tempObj.insert("number",QJsonValue(mq));
                                                tempDoc.setObject(tempObj);
                                                quint8 size;QByteArray sizeData;
                                                size=tempDoc.toJson().size();
                                                QDataStream stream(&sizeData,QIODevice::WriteOnly);
                                                stream.setVersion(QDataStream::Qt_4_0);
                                                stream<<size;
                                                tcpsocket->write(sizeData+tempDoc.toJson());
                                                loop.exec();
                                                qDebug()<<size;
                                                tcpsocket->write(data);
                                                loop.exec();
                                            }
                                        }
                                    }else ok=false;
                                    headimg->remove();//ɾ������ļ�
                                }
                                headimg->deleteLater();//�ļ��ӳ�ɾ��
                                historyImgFiles.remove(myqq);//ɾ�������ļ�����
                                muter->unlock();//���� ������� ����һ�����±���ļ��߳�ִ����� ��������������ɾ��������һ���߳�ɾ��
                                emit delayedHeadImgDeletion(myqq);//�ӳ�ɾ��������
                            }
                            //����ǩ�����ǳ�
                            if(sigFiles.contains(myqq)){
                                QMutex*muter=sigMuter.value(myqq);//��ֵ����
                                muter->lock();
                                QFile* sigFile=sigFiles.value(myqq);
                                if(sigFile->exists()){
                                    qDebug()<<sigFile->fileName()<<" is found";
                                    if(!sigFile->open(QIODevice::ReadOnly)){
                                        ok=false;
                                        qDebug()<<"warning:opened the file is of failure,named "<<sigFile->fileName();
                                    }else{
                                        QByteArray data=sigFile->readAll();
                                        QJsonDocument tempDoc;
                                        QJsonObject tempObj;
                                        tempObj.insert("content",QJsonValue("signatureAndName"));
                                        tempObj.insert("size",QJsonValue(data.size()));
                                        tempDoc.setObject(tempObj);
                                        quint8 size;QByteArray sizeData;
                                        size=tempDoc.toJson().size();
                                        QDataStream stream(&sizeData,QIODevice::WriteOnly);
                                        stream.setVersion(QDataStream::Qt_4_0);
                                        stream<<size;
                                        tcpsocket->write(sizeData+tempDoc.toJson());
                                        loop.exec();
                                        tcpsocket->write(data);
                                        loop.exec();
                                    }
                                    if(sigFile->remove())qDebug()<<"result of  sigFile->remove():true";
                                }
                                sigFile->deleteLater();
                                sigFiles.remove(myqq);//ɾ�������ļ�����
                                muter->unlock();//���� ������� ����һ�����±���ļ��߳�ִ����� ��������������ɾ��������һ���߳�ɾ��
                                emit delayedSigAndNameDeletion(myqq);
                            }
                        }else ok=false;
                        qDebug()<<"result is "<<ok;
                        if(ok){
                            //�ɹ�����
                            writeObj.insert("content",QJsonValue("end"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            QByteArray sizedata;
                            quint8 l=writeJson.toJson().size();
                            QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l;
                            tcpsocket->write(sizedata+ writeJson.toJson());
                            loop.exec();
                            tcpsocket->disconnectFromHost();
                        }else{
                            writeObj.insert("content",QJsonValue("end"));
                            writeObj.insert("result",QJsonValue("false"));
                            writeJson.setObject(writeObj);
                            QByteArray sizedata;
                            quint8 l=writeJson.toJson().size();
                            QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l;
                            tcpsocket->write(sizedata+ writeJson.toJson());
                            loop.exec();
                            tcpsocket->disconnectFromHost();
                        }
                    }else{
                        writeObj.insert("content",QJsonValue("end"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        QByteArray sizedata;
                        quint8 l=writeJson.toJson().size();
                        QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                        sizeStr.setVersion(QDataStream::Qt_4_0);
                        sizeStr<<l;
                        tcpsocket->write(sizedata+ writeJson.toJson());
                        loop.exec();
                        tcpsocket->disconnectFromHost();
                    }
                    return;
                }else if(in.toString()=="8"){
                    QString value=obj.value("content").toString();
                    QString myqq=obj.value("myqq").toString();
                    qDebug()<<"value=="<<value;
                    //��myself��������
                    if(value=="getPersonalData"){
                        bool ok=true;
                        if(query.exec(" exec myselfInformation "+myqq)){

                            qDebug()<<"query is of success for personal information";
                            query.next();
                            if(query.isValid()){
                                writeObj.insert("birthday",query.value("birthday").toString());
                                writeObj.insert("registerDateTime",query.value("registerDateTime").toString());
                                writeObj.insert("bloodGroup",query.value("bloodGroup").toString());
                                writeObj.insert("education",query.value("education").toString());
                                writeObj.insert("profession",query.value("profession").toString());
                                writeObj.insert("corporation",query.value("corporation").toString());
                                writeObj.insert("location1",query.value("location1").toString());
                                writeObj.insert("location2",query.value("location2").toString());
                                writeObj.insert("location3",query.value("location3").toString());
                                writeObj.insert("location4",query.value("location4").toString());
                                writeObj.insert("home1",query.value("home1").toString());
                                writeObj.insert("home2",query.value("home2").toString());
                                writeObj.insert("home3",query.value("home3").toString());
                                writeObj.insert("home4",query.value("home4").toString());
                                writeObj.insert("personalStatement",query.value("personalStatement").toString());
                                writeObj.insert("phone",query.value("phone").toString());
                                QString wallPath= query.value("photoWallPath").toString();
                                writeJson.setObject(writeObj);//���json�ı�����
                                QJsonDocument doc;
                                QJsonObject temp;
                                temp.insert("instruct",QJsonValue("80"));
                                temp.insert("result",QJsonValue("writingJson"));
                                temp.insert("size",QJsonValue(writeJson.toJson().size()));
                                doc.setObject(temp);
                                QByteArray sizeData;
                                quint8 l=doc.toJson().size();
                                QDataStream sizeStr(&sizeData,QIODevice::WriteOnly);
                                sizeStr.setVersion(QDataStream::Qt_4_0);
                                sizeStr<<l;
                                tcpsocket->write(sizeData+doc.toJson());
                                loop.exec();
                                tcpsocket->write(writeJson.toJson());
                                loop.exec();
                                //��Ƭǽ
                                QDir wall(wallPath);
                                if(wall.exists()){
                                    if(!wall.isEmpty()){
                                       QStringList list=wall.entryList(QStringList("*"),QDir::Files,QDir::Name|QDir::Reversed);
                                       qint32 length=list.length();
                                       for (int var = 0; var <length; ++var) {
                                           QString fname=list.at(var);
                                           QFile tempF(wall.filePath(fname));
                                           if(tempF.open(QIODevice::ReadOnly)){
                                               qDebug()<<"a file is opened,named "<<fname;
                                               QByteArray fdata=tempF.readAll();
                                               if(fdata.isEmpty()){
                                                   qDebug()<<"a photowall file was readed unsuccessfully";
                                                   continue;
                                               }
                                               QJsonDocument ftempDoc;
                                               QJsonObject ftempObj;
                                               ftempObj.insert("instruct",QJsonValue("80"));
                                               ftempObj.insert("result",QJsonValue("writingPhotoWall"));
                                               ftempObj.insert("name",QJsonValue(fname));
                                               ftempObj.insert("size",QJsonValue(fdata.size()));
                                               ftempDoc.setObject(ftempObj);
                                               QByteArray tempData;
                                               quint8 tl=ftempDoc.toJson().size();
                                               QDataStream tempStr(&tempData,QIODevice::WriteOnly);
                                               tempStr.setVersion(QDataStream::Qt_4_0);
                                               tempStr<<tl;
                                               tcpsocket->write(tempData+ftempDoc.toJson());
                                               loop.exec();
                                               tcpsocket->write(fdata);
                                               loop.exec();
                                           }
                                       }
                                    }else{
                                       qDebug()<<"photo wall is empty";
                                    }
                                }else{
                                    wall.mkpath("./");//������ǰĿ¼
                                }
                            }else ok=false;
                        }else{
                            ok=false;
                            qDebug()<<"query is of failure for personal information";
                        }
                        QFile cover("../userData/"+myqq+"/cover");
                        if(cover.exists()){
                            qDebug()<<"cover-image is found";
                            if( cover.open(QIODevice::ReadOnly)){
                                QByteArray coverData;
                                coverData=  cover.readAll();
                                if(!coverData.isEmpty()){
                                    QJsonDocument jsonCover;
                                    QJsonObject objCover;
                                    objCover.insert("instruct",QJsonValue("80"));
                                    objCover.insert("result",QJsonValue("writingCover"));
                                    objCover.insert("name",QJsonValue("cover"));
                                    objCover.insert("size",QJsonValue(coverData.size()));
                                    jsonCover.setObject(objCover);
                                    QByteArray sizeData;
                                    quint8 l=jsonCover.toJson().size();
                                    QDataStream sizeStr(&sizeData,QIODevice::WriteOnly);
                                    sizeStr.setVersion(QDataStream::Qt_4_0);
                                    sizeStr<<l;
                                    tcpsocket->write(sizeData+jsonCover.toJson());
                                    loop.exec();
                                    tcpsocket->write(coverData);
                                    loop.exec();
                                }else{
                                    qDebug()<<"warning: readed cover-image file is of failure ";
                                    ok=false;
                                }
                            }else{
                                qDebug()<<"warning:data of cover-image is not got";
                                ok=false;
                            }
                        }
                        //�ɹ���ʧ�ܴ���
                        QJsonDocument json;
                        QJsonObject jsonObj;
                        if(ok){
                            jsonObj.insert("instruct",QJsonValue("80"));
                            jsonObj.insert("result",QJsonValue("true"));
                            json.setObject(jsonObj);
                            QByteArray sizeData;
                            quint8 l=json.toJson().size();
                            QDataStream sizeStr(&sizeData,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l;
                            tcpsocket->write(sizeData+json.toJson());
                            loop.exec();
                        }else{
                            jsonObj.insert("instruct",QJsonValue("80"));
                            jsonObj.insert("result",QJsonValue("false"));
                            json.setObject(jsonObj);
                            QByteArray sizeData;
                            quint8 l=json.toJson().size();
                            QDataStream sizeStr(&sizeData,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l;
                            tcpsocket->write(sizeData+json.toJson());
                            loop.exec();
                        }
                    }else{
                        //ʧ��
                        writeObj.insert("content",QJsonValue("end"));
                        writeObj.insert("result",QJsonValue("false"));
                        writeJson.setObject(writeObj);
                        QByteArray sizedata;
                        quint8 l=writeJson.toJson().size();
                        QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                        sizeStr.setVersion(QDataStream::Qt_4_0);
                        sizeStr<<l;
                        tcpsocket->write(sizedata+ writeJson.toJson());
                        loop.exec();
                        tcpsocket->disconnectFromHost();
                    }
                    return;
                }
            }
        }
    }
}
//�ͻ��˶Ͽ����Ӿ�ɾ���̼߳�����
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


