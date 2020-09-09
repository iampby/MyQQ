#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "loginthread.h"
#include"global.h"
#include<qtcpsocket.h>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include <qdir.h>
#include <qdatetime.h>
#include<QXmlStreamReader>
#include <qdatastream.h>
#include <QJsonArray>
#include <qtimer.h>
#include<qthread.h>
#include<qdir.h>
#include <qpixmap.h>
#include <QDomDocument>
#include<qsqlerror.h>
#include<sstream>
//��Ϊ�׽��ַ������ݿ�����ճ��һ��ģ�����Ҫ�ȷ��͸����ݴ�С
//������ڷ���ͷ�ֽڼӸ�int��Ǵ�С��������int8���json���ݣ���Ϊ�������ݵ���С��λ�����ֽ��ˣ���֤���յ���ͬʱjson�ֽ�С��256
//��Ȼuint32 ����õ���Ϊһ�����������λ��Ϊһ��Ϊ32λ����
//�����ݵĴ�С����json��Ǵ�С
//��ʼ������д�˸�json��ȥ������json��������С�����ͺܿ죬������ճ�������Է�������߾���json����,�Ͳ�����
LoginThread::LoginThread(qintptr socketDescriptor, qint64 count, QObject *parent)
    :QObject(parent),socketDescriptor(socketDescriptor),count(count),noDis(false)
{
    tcpsocket=new QTcpSocket(this);//tcpsocket ����̳и��� ����������ʹ���߳�
    loop.setParent(this);//�ƶ�������
    if(!tcpsocket->setSocketDescriptor(this->socketDescriptor)) {
        qDebug()<<("�����׽���ʧ�ܣ�");
        emit error(tcpsocket->error());
        return;
    }
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption,true);//windows����
    connect(tcpsocket,&QTcpSocket::readyRead,this,&LoginThread::readD);
    connect(this,&LoginThread::startTimer,this,&LoginThread::timer);
    connect(tcpsocket,&QTcpSocket::disconnected,this,&LoginThread::disconnected);
    connect(tcpsocket,&QTcpSocket::bytesWritten,&loop,&QEventLoop::quit);//һ��д����ɽ����¼�ѭ��
}

LoginThread::~LoginThread()
{
    qDebug()<<"~LoginThread()";
    foreach (QString str, dbPool) {
        {
        QSqlDatabase db= QSqlDatabase::database(str,false);
        if(db.isOpen())db.close();
        }
       if(QSqlDatabase::contains(str))
        QSqlDatabase::removeDatabase(str);
    }
    dbPool.clear();
     qDebug()<<" dbPool.clear()";
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

void LoginThread::writeInfoToOpposite(QString ip, quint16 port, QJsonObject obj, QString content, QByteArray bytes, bool b)
{
    QTcpSocket*verSock=new QTcpSocket(this);
    QEventLoop*loop=new QEventLoop(verSock);
    //�¼�Ȧ
    connect(verSock,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    //����
    connect(verSock,&QTcpSocket::connected,verSock,[=](){
        qDebug()<<"versock is conneted";
        QJsonDocument doc(obj);
        QByteArray data=doc.toJson();
        if(data.isEmpty())qDebug()<<"verify data is changed to blank";
        else{
            QJsonDocument tempDoc;
            QJsonObject tempObj;
            tempObj.insert("content",QJsonValue(content));
            tempObj.insert("size",QJsonValue(data.size()));
            tempObj.insert("type",QJsonValue("json"));
            tempDoc.setObject(tempObj);
            QByteArray size;
            QDataStream s(&size,QIODevice::WriteOnly);
            s.setVersion(QDataStream::Qt_4_0);
            quint8 l=tempDoc.toJson().size();
            s<<l;
            verSock->write(size+tempDoc.toJson());
            loop->exec();
            verSock->write(data);
            loop->exec();
            if(!bytes.isEmpty()){
                QJsonDocument tempDoc1;
                QJsonObject tempObj1;
                tempObj1.insert("content",QJsonValue(content));
                tempObj1.insert("size",QJsonValue(bytes.size()));
                tempObj1.insert("type",QJsonValue("bytes"));
                tempDoc1.setObject(tempObj1);
                QByteArray size1;
                QDataStream s1(&size1,QIODevice::WriteOnly);
                s1.setVersion(QDataStream::Qt_4_0);
                quint8 l1=tempDoc1.toJson().size();
                s1<<l1;
                verSock->write(size1+tempDoc1.toJson());
                loop->exec();

                verSock->write(bytes);
                loop->exec();
            }
        }
        verSock->close();
        verSock->deleteLater();
        loop->deleteLater();
        if(b)//�����Ҫ�˳����˳�
            emit finished();
    });
    connect(verSock,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),verSock,[=](QAbstractSocket::SocketError code){
        switch (code) {
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<("���ӱ��ܽӻ�ʱ��")<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            qDebug()<<("Զ�������ѹرգ�")<<endl;
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<("����û�з��֣�")<<endl;
            break;
        case QAbstractSocket::UnknownSocketError:
            qDebug()<<("δ֪����")<<endl;
            break;
        default:
            qDebug()<<("�����˴���")<<endl;
        }
        verSock->deleteLater();
        loop->deleteLater();
        emit finished();
    });
    //��ʱ
    QTimer::singleShot(10000,verSock,[=](){
        qDebug()<<"versock conection timeout";
        verSock->deleteLater();
        loop->deleteLater();
        emit finished();
    });
    verSock->connectToHost(ip,port);
}

//����״̬��β����
void LoginThread::statusChanged(QString status,QString&myqq)
{
    QFile file("../userData/"+myqq+"/info.xml");
    if(file.open(QIODevice::ReadOnly)){
        qDebug()<<"successfully opened info.xml file ";
        //��¼����״̬�ı���Ϣ
        QString tid;
        std::thread::id id=std::this_thread::get_id();
        std::stringstream sin;
        sin << id;
        tid=QString::fromStdString(sin.str());
        qDebug()<<"threadID : "<<tid;
        QDomDocument infoDoc;
        infoDoc.setContent(&file);
        file.close();//�ر�
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
                        //�ı��Լ���״̬
                        if(mq==myqq){
                            ele=ele.firstChildElement(("״̬"));
                            ele.firstChild().setNodeValue(status);//�޸��ı��ڵ�
                            //����״̬�ı���Ϣ
                        }else {
                            QDir tdir("../userData/"+mq+"/friendsInfo/chat");
                            if(!tdir.exists()){
                                if(!tdir.mkpath("./")){
                                    qDebug()<<"dir creation is of failure";
                                    continue;
                                }
                            }
                            bool ok=true;
                            QString cn1=tid+mq+QString("%1").arg( QDateTime::currentDateTime().toTime_t());
                            QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",cn1);
                            dbPool.append(cn1);
                            db1.setDatabaseName(QString("../userData/%1/friendsInfo/chat/_%2.db").arg(mq).arg(myqq));
                            db1.setHostName("MyQQ");
                            db1.setUserName("sa");
                            db1.setPassword("@123456x");
                            if(db1.open())
                                qDebug()<<("��sqliite���ݿ�ɹ�����������")<<db1.connectionName();
                            else {
                                qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db1.databaseName());
                                return;
                            }
                            QSqlQuery query1(db1);
                            ok=  query1.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1status' ").arg(myqq));
                            if(!ok){
                                qDebug()<<"count of table query is of failure ";
                                return;
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
                                ok= query1.exec(QString(" update _%1status  set status='%2' ").arg(myqq,status));
                                if(!ok){
                                    qDebug()<<"update _%1status  set status='%2': failure";
                                    continue;
                                }
                            }
                            //�ı����xml״̬��¼
                            setXmlStatus(myqq,mq,status);
                        }
                    }
                }
            }
        }
        //�����ļ�
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
            qDebug()<<"warning:opended  the xml file to alter status  unsuccessfully ";
            return ;
        }
        QTextStream stream(&file);
        infoDoc.save(stream,4);
        file.close();
    }
}

void LoginThread::setXmlStatus(QString &opposite, QString &me, QString &status)
{
    QFile file("../userData/"+me+"/info.xml");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"warning:opended the opposite xml file to alter the status  unsuccessfully ";
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
                    //�ı�xml״̬��¼
                    if(mq==opposite){
                        ele=ele.firstChildElement("״̬");;
                        ele.firstChild().setNodeValue(status);
                        i=length;
                        break;//����ѭ��
                    }
                }
            }
        }
    }
    //�����ļ�
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qDebug()<<"warning:opended for second time the opposite xml file to alter the status   unsuccessfully ";
        return ;
    }
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
}

void LoginThread::offlineOtherHost(QString &ip, QString &host, QString &oldIP, quint16 &oldPort, QString &datetime)
{
    qDebug()<<"last ip="<<oldIP<<",port="<<oldPort;
    QTcpSocket*offlineSock=new QTcpSocket(this);
    QEventLoop*loop=new QEventLoop(offlineSock);
    //�¼�Ȧ
    connect(offlineSock,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    //����
    connect(offlineSock,&QTcpSocket::connected,offlineSock,[=](){

        QJsonDocument tempDoc;
        QJsonObject tempObj;
        tempObj.insert("content",QJsonValue("offline"));
        tempObj.insert("ip",QJsonValue(ip));
        tempObj.insert("host",QJsonValue(host));
        tempObj.insert("datetime",QJsonValue(datetime));
        tempDoc.setObject(tempObj);
        QByteArray size;
        QDataStream s(&size,QIODevice::WriteOnly);
        s.setVersion(QDataStream::Qt_4_0);
        quint8 l=tempDoc.toJson().size();
        s<<l;
        offlineSock->write(size+tempDoc.toJson());
        loop->exec();
        offlineSock->close();
        offlineSock->deleteLater();
        loop->deleteLater();
    });
    connect(offlineSock,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),offlineSock,[=](QAbstractSocket::SocketError code){
        switch (code) {
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<("���ӱ��ܽӻ�ʱ��")<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            qDebug()<<("Զ�������ѹرգ�")<<endl;
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<("����û�з��֣�")<<endl;
            break;
        case QAbstractSocket::UnknownSocketError:
            qDebug()<<("δ֪����")<<endl;
            break;
        default:
            qDebug()<<("�����˴���")<<endl;
        }
        offlineSock->deleteLater();
        loop->deleteLater();
        emit finished();//ʧ�ܾ�ֱ���˳� ��¼ʧ��
    });
    //��ʱ
    QTimer::singleShot(10000,offlineSock,[=](){
        qDebug()<<"versock conection timeout";
        offlineSock->deleteLater();
        loop->deleteLater();
        emit finished();
    });
    offlineSock->connectToHost(oldIP,oldPort);
}

void LoginThread::onlineDispatchedIP(QJsonObject&writeObj,  QString &number, const QString&ip, const quint16&port,  QString&status)
{
    QTcpSocket*testSock=new QTcpSocket(this);
    //����
    connect(testSock,&QTcpSocket::connected,testSock,[=]()mutable{
        qDebug()<<"connection to opposite";
        QJsonDocument writeJson;
        writeObj.insert("result",QJsonValue("true"));
        writeObj.insert("online",QJsonValue(true));
        writeObj.insert("status",QJsonValue(status));
        writeJson.setObject(writeObj);
        QByteArray size;
        QDataStream s(&size,QIODevice::WriteOnly);
        s.setVersion(QDataStream::Qt_4_0);
        quint8 l=writeJson.toJson().size();
        s<<l;
        tcpsocket->write(size+writeJson.toJson());
        loop.exec();
        emit finished();
    });
    //�����ϸ���״̬
    connect(testSock,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),[=]()mutable{
        QJsonDocument writeJson;
        qDebug()<< "not connection to opposite";
        writeObj.insert("result",QJsonValue("true"));
        writeObj.insert("online",QJsonValue(false));
        writeObj.insert("status",QJsonValue("0"));
        writeJson.setObject(writeObj);
        QByteArray size;
        QDataStream s(&size,QIODevice::WriteOnly);
        s.setVersion(QDataStream::Qt_4_0);
        quint8 l=writeJson.toJson().size();
        s<<l;
        tcpsocket->write(size+writeJson.toJson());
        loop.exec();
        if(status!="0"){
            QString cn=QString("%1").arg( QDateTime::currentDateTime().toTime_t())+QString("%1").arg(count);
            QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",cn);
         dbPool.append(cn);
            QString connectString = (
                        "DRIVER={sql server};"
                        "SERVER=127.0.0.1;"
                        "DATABASENAME:qtmanager;"
                        "PORT:1433;"
                        "UID=sa;"
                        "PWD=123456x;");
            db.setDatabaseName(connectString);
            qDebug()<<"curtime"<<QTime::currentTime();
            if(db.open())
                qDebug()<<("�����ݿ�ɹ���");
            else {
                qDebug()<<("�����ݿ�ʧ�ܣ�")<<db.lastError();
                emit finished();//����ͬʱ�����ݿ�����������ԭ�����
                return;
            }

            qDebug()<<"login"<<db.connectionName();
            QSqlQuery query(" use myqq ",db);
           bool ok=  query.exec(QString(" update userInfo set isConnected=0,userStatus='0'  where myqq=%1 ").arg(number.toLongLong()));
            if(!ok){
                qDebug()<<"the query is of failure for updating status";

            }else{
                statusChanged(QString("0"),number);
            }
        }
        emit finished();
    });
    testSock->connectToHost(ip,port);
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
    qDebug()<<"login executing thread is "<<this->thread()->currentThread();
    //�þ�ȷ�����ʱ���100��ֵ��Χ��count����������1���ڿ��Դ򿪵����ݿ����ӣ���100��
    QString cn=QString("%1").arg( QDateTime::currentDateTime().toTime_t())+QString("%1").arg(count);
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",cn);
 dbPool.append(cn);
    QString connectString = (
                "DRIVER={sql server};"
                "SERVER=127.0.0.1;"
                "DATABASENAME:qtmanager;"
                "PORT:1433;"
                "UID=sa;"
                "PWD=123456x;");
    db.setDatabaseName(connectString);
    qDebug()<<"curtime"<<QTime::currentTime();
    if(db.open())
        qDebug()<<("�����ݿ�ɹ���");
    else {
        qDebug()<<("�����ݿ�ʧ�ܣ�")<<db.lastError();
        emit finished();//����ͬʱ�����ݿ�����������ԭ�����
        return;
    }

    qDebug()<<"login"<<db.connectionName();
    QSqlQuery query(" use myqq ",db);

    QJsonDocument readJson=QJsonDocument::fromBinaryData(tcpsocket->readAll());
    QJsonDocument writeJson;//ע��ÿʹ��һ��json�ĵ���������ʹjson�ĵ�����ͬ�� json�ĵ���������
    QJsonObject writeObj;
    qint64 byteSize;//д��io�豸�ֽڵĴ�С

    if(!readJson.isEmpty()){
        if(readJson.isObject()){
            QJsonObject obj=readJson.object();
            if(!obj.isEmpty()){
                qDebug()<<"isObject"<<endl;
                QJsonValue in=obj.value("instruct");
                if(in.toString()=="1"){//��¼���
                    qDebug()<<("��¼��Ϣ��֤")<<endl;
                    QString myqq=obj.value("myqq").toString();
                    QString passwd=obj.value("password").toString();
                    QString hostname=obj.value("hostname").toString();
                    QString ip=obj.value("ip").toString();
                    QString port=obj.value("serverPort").toString();
                    if(!(myqq.isEmpty()|passwd.isEmpty()|hostname.isEmpty()|ip.isEmpty())){
                        qDebug()<<("not empty")<<myqq.toLongLong()<<endl;
                        query.prepare(" select myqq,passwd,signLog,isConnected from userInfo where myqq=? ");
                        query.bindValue(0,myqq.toLongLong());
                        if(query.exec()){
                            qDebug()<<("execution is successful")<<endl;
                            query.next();
                            if(query.isValid()){
                                qDebug()<<("isvalid");
                                QString temp=query.value(1).toString();
                                QString logRec=query.value(2).toString();
                                bool isCon=query.value(3).toBool();
                                if(temp!=passwd){//�������
                                    qDebug()<<("passwd err")<<endl;
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
                                //�Ѿ�������Ͽ�֮ǰ������

                                if(isCon){
                                    //��ȡ�ϴε�¼��¼
                                    qDebug()<<"other host will be offline ";
                                    QStringList list=logRec.split(";;",QString::SkipEmptyParts);
                                    QString oldIP;
                                    try{
                                        if(list.size()<1)throw "index out of range";
                                        oldIP =list.at(0);
                                    }catch(_exception& e){
                                        qDebug()<<"error:type="<<e.type<<",name="<<e.name;
                                    }catch(...){
                                        qDebug()<<"a unkown fatal error";
                                    }
                                    list=oldIP.split(" ",QString::SkipEmptyParts);
                                    try{
                                        if(list.size()<5)throw QString("index out of range");
                                    }catch(QString &code){
                                        qDebug()<<code;
                                        //ʧ��ִ��
                                        writeObj=QJsonObject();
                                        writeJson=QJsonDocument();
                                        writeObj.insert("instruct",QJsonValue("10"));
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
                                    }

                                    oldIP=list.at(1);//ip
                                    QString temp=list.at(2);//port
                                    quint16 oldPort=(quint16)temp.toUInt();
                                    temp=list.at(3)+" "+list.at(4);//���ڼ�ʱ��
                                    qDebug()<<"now ip="<<ip<<",port="<<port;
                                    offlineOtherHost(ip,hostname,oldIP,oldPort,temp);
                                }
                                logRec= handleLogRec(logRec,hostname,ip,port);
                                query.prepare(" update userinfo set signLog=?,isConnected=?,userState=? where myqq=? ");

                                query.bindValue(0,QVariant(logRec));
                                query.bindValue(1,QVariant(bool(1)));
                                query.bindValue(2,QVariant("1"));
                                query.bindValue(3,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    qDebug()<<("���Ӹ��³ɹ���");
                                }

                                //��ȡ������Ϣ����name,personalizedSignature,activeDays
                                query.prepare("  select myqqName,sex,headImgPath,personalizedSignature,activeDays ,myqqGrade,userState,homeTownCountry,homeTownProvince,"
                                              "homeTownCity,homeTown,locateInCountry,locateInProvince,locateInCity,locateInTown from userInfo with(rowlock,holdlock) where myqq=? ");
                                query.bindValue(0,QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    query.next();
                                    if(query.isValid()){
                                        qDebug()<<("��ȡMyQQ(")+myqq+(")������Ϣ�ɹ�");
                                        QString name=query.value(0).toString();//�ǳ�
                                        QString sex=query.value(1).toString();//�Ա�
                                        QString headPath="../userData/"+myqq+"/historyHeadImg/01.png";//query.value(2).toString();//ͷ��·��
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


                                            QByteArray img;
                                            byteSize=headImg.size();
                                            img.resize(byteSize);
                                            //QByteArray �����趨��С
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
                                                    if(byteSize==0){
                                                        qDebug()<<"warning:bytesize is nulll";
                                                    }
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
                                                        writeObj.insert("instruct",QJsonValue("10"));
                                                        writeObj.insert("result",QJsonValue(0));
                                                        writeObj.insert("name",QJsonValue(name));
                                                        writeObj.insert("sex",QJsonValue(sex));
                                                        writeObj.insert("signature",QJsonValue(signature));
                                                        writeObj.insert("days",QJsonValue(days));
                                                        writeObj.insert("grade",QJsonValue(grade));
                                                        writeObj.insert("status",QJsonValue(status));
                                                        writeObj.insert(("���ڵ�"),QJsonValue(where));
                                                        writeObj.insert(("����"),QJsonValue(hometown));
                                                        writeJson.setObject(writeObj);
                                                        s=writeJson.toJson().size();
                                                        QByteArray size3;//��дһ��sizeͷ ���ܸ���size������ÿд��һ�ζ�������
                                                        QDataStream stream3(&size3,QIODevice::WriteOnly);
                                                        stream3.setVersion(QDataStream::Qt_4_0);
                                                        stream3<<s;
                                                        tcpsocket->write(size3+writeJson.toJson());
                                                        loop.exec();
                                                        //����״̬��β����
                                                        statusChanged(status,myqq);
                                                        return;
                                                    }
                                                }
                                            }
                                        }

                                    }
                                }
                            }else{//MyQQ�������
                                qDebug()<<("account err")<<endl;
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
                    qDebug()<<("failed exec")<<endl;
                    writeObj.insert("instruct",QJsonValue("10"));
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
                }else if(in.toString()=="2"){//��ͷ�����Ϣ��¼
                    QString myqq=obj.value("myqq").toString();
                    QString content=obj.value("content").toString();
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
                                }else if(reader.name().toString()==("ͷ��·��")){
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
                            info.close();//�ر��ļ�
                            //��sqlite:��Ϣ��¼
                            //������status����
                            QString tid;//�߳�id
                            std::thread::id id=std::this_thread::get_id();
                            std::stringstream sin;
                            sin << id;
                            tid=QString::fromStdString(sin.str());
                            qDebug()<<"threadID : "<<tid;
                            QDir dir(QString("../userData/%1/friendsInfo/chat").arg(myqq));

                            if(dir.exists()){
                                QStringList list=dir.entryList(QStringList("*.db"),QDir::Files);
                                qint32 length=list.length();
                                bool ok;
                                for(qint32 i=0;i<length;++i){
                                    QString name=list.at(i);
                                    qDebug()<<"finding database name:"<<name;
                                    QString cn1=tid+myqq+QString("%1").arg(i)+QString("%1").arg(QDateTime::currentDateTime().toTime_t());
                                     dbPool.append(cn1);
                                    QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",cn1);
                                    db1.setDatabaseName(dir.filePath(name));
                                    db1.setHostName("MyQQ");
                                    db1.setUserName("sa");
                                    db1.setPassword("@123456x");
                                    if(db1.open())
                                        qDebug()<<("��sqliite���ݿ�ɹ����������ǣ�")<<db1.connectionName();
                                    else {
                                        qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db1.databaseName());
                                        continue;
                                    }
                                    QSqlQuery query1(db1);
                                    ok=  query1.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1status' ").arg(name.left(name.length()-3)));
                                    if(!ok){
                                        qDebug()<<"count of table query is of failure ";
                                        return;
                                    }
                                    query1.next();
                                    if(query1.value(0).toInt()!=0){
                                        ok= query1.exec(QString( " drop table _%1status ").arg(name.left(name.length()-3)));
                                        if(!ok){
                                            qDebug()<<"warning:drop  a table about status is of failure";
                                        }
                                    }
                                    QFile file(dir.filePath(name));
                                    if(!file.open(QIODevice::ReadOnly)){
                                        qDebug()<<"opened file is of failure";
                                        return;
                                    }
                                    QByteArray data=file.readAll();
                                    QJsonDocument dbdoc;
                                    QJsonObject dbobj;
                                    dbobj.insert("instruct",QJsonValue("20"));
                                    dbobj.insert("dbName",QJsonValue(name));
                                    dbobj.insert("size",QJsonValue(data.size()));
                                    dbobj.insert("result",QJsonValue("writeDB"));
                                    dbdoc.setObject(dbobj);
                                    quint8 l=dbdoc.toJson().size();
                                    QByteArray size;
                                    QDataStream stream(&size,QIODevice::WriteOnly);
                                    stream.setVersion(QDataStream::Qt_4_0);
                                    stream<<l;
                                    tcpsocket->write(size+dbdoc.toJson());
                                    loop.exec();
                                    tcpsocket->write(data);
                                    loop.exec();
                                    //
                                    query1.clear();
                                    db1.close();//��ɾ���ļ�
                                    file.close();
                                    qDebug()<<"delete result"<< file.fileName()<<file.remove();//��������Ϣ��¼
                                }
                            }
                            //�����µ���֤��Ϣ
                            QDir vdir("../userData/"+myqq+"/friendsInfo/verifyFriends");
                            if(vdir.exists()){
                                QFileInfoList  list=vdir.entryInfoList(QStringList("*.json"),QDir::Files,QDir::Time);//ʱ���С����
                                try{
                                    if(list.length()>0){
                                        QString filename=list.at(0).absoluteFilePath();
                                        QFile vfile(filename);
                                        if(vfile.exists()){
                                            if(vfile.open(QIODevice::ReadOnly)){
                                                QJsonDocument tdoc=QJsonDocument::fromJson(vfile.readAll());
                                                QJsonObject tobj=tdoc.object();
                                                if(tobj.isEmpty())throw "the friend verify infromation is empty";
                                                QJsonObject sobj;
                                                sobj.insert("name",QJsonValue(tobj.value("name").toString()));
                                                sobj.insert("time",QJsonValue(tobj.value("time").toString()));
                                                sobj.insert("number",QJsonValue(tobj.value("number").toString()));
                                                tdoc=QJsonDocument();
                                                tdoc.setObject(sobj);
                                                QByteArray sdata=tdoc.toJson();
                                                if(sdata.isEmpty())throw "sending friend verify data is empty";
                                                //����
                                                writeJson=QJsonDocument();
                                                writeObj=QJsonObject();
                                                writeObj.insert("instruct",QJsonValue("20"));
                                                writeObj.insert("result",QJsonValue("writeVerify"));
                                                writeObj.insert("size",QJsonValue(sdata.size()));
                                                writeJson.setObject(writeObj);
                                                quint8 l=writeJson.toJson().size();
                                                QByteArray size;
                                                QDataStream stream(&size,QIODevice::WriteOnly);
                                                stream.setVersion(QDataStream::Qt_4_0);
                                                stream<<l;
                                                tcpsocket->write(size+writeJson.toJson());
                                                loop.exec();
                                                tcpsocket->write(sdata);
                                                loop.exec();
                                                vfile.close();
                                                // vfile.remove();//��������Ϣ��¼
                                            }else{
                                                qDebug()<<"a friend adding verify file is of failure for opening";
                                                throw "opened file failure";
                                            }
                                        }
                                    }
                                }catch(_exception&e){
                                    qDebug()<<"exception:type "<<e.type<<",name "<<e.name;
                                }
                            }
                            //����
                            writeJson=QJsonDocument();
                            writeObj=QJsonObject();
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
                    //����
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
                            QString containName=obj.value("name").toString();
                            qint64 beginId=obj.value("begin-id").toString().toLongLong();
                            //�����ȫΪ���ֻ��һ������Ϊ0���ȡ�����б�����ȡָ������
                            if(!containName.data()->isDigit()||containName.at(0)=="0"){
                                friendInfo.close();
                                QString  location=obj.value("location").toString();
                                QString  hometown=obj.value("hometown").toString();
                                QString sex=obj.value("sex").toString();
                                if(sex==("����"))
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

                                QVector<QJsonObject> list;//�û��б�
                                query.prepare(" exec findFriends_pro :begin_id,:containName,:sex,:begin_date,"
                                              ":end_date,:isConnected,:location1,:location2,:location3,:location4,"
                                              ":hometown1,:hometown2,:hometown3,:hometown4,:isNumber ");
                                query.addBindValue(QVariant(beginId));
                                query.addBindValue(QVariant(containName));
                                query.addBindValue(QVariant(sex));
                                query.addBindValue(QVariant(beginDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(endDate.toString("yyyy-MM-dd")));
                                query.addBindValue(QVariant(online));
                                query.addBindValue(QVariant(list1.at(0)==("����")?"":list1.at(0)));
                                query.addBindValue(QVariant(list1.size()>1?list1.at(1):""));
                                query.addBindValue(QVariant(list1.size()>2?list1.at(2):""));
                                query.addBindValue(QVariant(list1.size()>3?list1.at(3):""));
                                query.addBindValue(QVariant(list2.at(0)==("����")?"":list2.at(0)));
                                query.addBindValue(QVariant(list2.size()>1?list2.at(1):""));
                                query.addBindValue(QVariant(list2.size()>2?list2.at(2):""));
                                query.addBindValue(QVariant(list2.size()>3?list2.at(3):""));
                                query.addBindValue(QVariant(false));
                                qint32 count=0;
                                qint32 sum=0;
                                while(true){//ѭ����ü�¼��������ԣ�����Ҫ��20��
                                    /*
���ݿ��в��ִ���
proc findFriends_pro
--���� @begin_id���������Сֵ ,@containName���ǳư������ı� @sex���Ա�  @begin_date��@end_date�����շ�Χ   ���ڵؼ�����
@begin_id bigint,@containName varchar(24),@sex varchar(2),@begin_date date,@end_date datetime,@isConnected bit,
@location1 varchar(20),@location2 varchar(20),@location3 varchar(20),@location4 varchar(20),@hometown1 varchar(20),
@hometown2 varchar(20),@hometown3 varchar(20),@hometown4 varchar(20),@isNumber
select top 20 myqqName,myqq ,sex,birthday,headImgPath,personalizedSignature, --�ǳ� ���� �Ա� ͷ�� ���ڵ�
table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,table1_4.name as location4--���ڵ�
*/
                                    if(query.exec()){
                                        qDebug()<<"exec successfully";
                                        query.next();
                                        if(!query.isValid()){
                                            break;//���ܳ����������⵼����ѭ����ǿ�ƽ���ѭ��
                                        }
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
                                                qint64 tempId=query.value("myqq").toLongLong();
                                                if(tempId>beginId)beginId=tempId;//��ȡ���myqq
                                                temp.insert("myqq",QJsonValue(tempId));
                                                temp.insert("myqqName",QJsonValue(query.value("myqqName").toString()));
                                                temp.insert("sex",QJsonValue(query.value("sex").toString()));
                                                qint64 age=dateSubstraction(query.value("birthday").toDate(),QDate::currentDate());
                                                temp.insert("age",QJsonValue(QString("%1").arg(age)));
                                                temp.insert("imgpath",QJsonValue(query.value("headImgPath").toString()));
                                                temp.insert("signature",QJsonValue(query.value("personalizedSignature").toString()));
                                                temp.insert("location1",QJsonValue(query.value("location1").toString()));
                                                temp.insert("location2",QJsonValue(query.value("location2").toString()));
                                                temp.insert("location3",QJsonValue(query.value("location3").toString()));
                                                temp.insert("location4",QJsonValue(query.value("location4").toString()));
                                                qDebug()<<temp;
                                                list.append(temp);
                                                ++count;
                                            }
                                            ++sum;
                                            query.next();
                                        }
                                        if(sum<20)
                                            break;//�����ٻ�ȡ
                                        else{
                                            //������ȡ
                                            if(count<20){
                                                query.prepare(" exec findFriends_pro :begin_id,:containName,:sex,:begin_date,"
                                                              ":end_date,:isConnected,:location1,:location2,:location3,:location4,"
                                                              ":hometown1,:hometown2,:hometown3,:hometown4,:isNumber ");
                                                query.addBindValue((QVariant(beginId)));//���°�
                                                query.addBindValue(QVariant(beginId));
                                                query.addBindValue(QVariant(containName));
                                                query.addBindValue(QVariant(sex));
                                                query.addBindValue(QVariant(beginDate.toString("yyyy-MM-dd")));
                                                query.addBindValue(QVariant(endDate.toString("yyyy-MM-dd")));
                                                query.addBindValue(QVariant(online));
                                                query.addBindValue(QVariant(list1.at(0)==("����")?"":list1.at(0)));
                                                query.addBindValue(QVariant(list1.size()>1?list1.at(1):""));
                                                query.addBindValue(QVariant(list1.size()>2?list1.at(2):""));
                                                query.addBindValue(QVariant(list1.size()>3?list1.at(3):""));
                                                query.addBindValue(QVariant(list2.at(0)==("����")?"":list2.at(0)));
                                                query.addBindValue(QVariant(list2.size()>1?list2.at(1):""));
                                                query.addBindValue(QVariant(list2.size()>2?list2.at(2):""));
                                                query.addBindValue(QVariant(list2.size()>3?list2.at(3):""));
                                                query.addBindValue(QVariant(false));
                                            }else
                                                break;//����Ҫ�ٻ�ȡ
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
                                tcpsocket->write(doc.toJson());//д�������б�
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

                            }else{
                                bool contain=false;
                                for (QVariant mq : friendList) {
                                    if(mq.toString()==containName){
                                        contain=true;
                                    }
                                }
                                if(!contain){
                                    query.prepare(" exec findFriends_pro :begin_id,:containName ");
                                    query.addBindValue(QVariant((qint64)10001));
                                    query.addBindValue(QVariant(containName));
                                    if(query.exec()){
                                        query.next();
                                        QJsonObject temp;
                                        temp.insert("myqq",QJsonValue(query.value("myqq").toLongLong()));
                                        temp.insert("myqqName",QJsonValue(query.value("myqqName").toString()));
                                        temp.insert("sex",QJsonValue(query.value("sex").toString()));
                                        qint64 age=dateSubstraction(query.value("birthday").toDate(),QDate::currentDate());
                                        temp.insert("age",QJsonValue(QString("%1").arg(age)));
                                        temp.insert("imgpath",QJsonValue(query.value("headImgPath").toString()));
                                        temp.insert("signature",QJsonValue(query.value("personalizedSignature").toString()));
                                        temp.insert("location1",QJsonValue(query.value("location1").toString()));
                                        temp.insert("location2",QJsonValue(query.value("location2").toString()));
                                        temp.insert("location3",QJsonValue(query.value("location3").toString()));
                                        temp.insert("location4",QJsonValue(query.value("location4").toString()));
                                        QJsonDocument doc;
                                        QJsonArray array;
                                        array.append(QJsonValue(temp));
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
                                        tcpsocket->write(doc.toJson());//д�������б�
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

                                    }else{
                                        qDebug()<<"q query sentence excuting is of failure";
                                    }
                                }else{
                                    qDebug()<<"the number of serching is a friend number";
                                }
                            }
                            //д������ָ�� 30 ����
                            writeObj.insert("instruct",QJsonValue("30"));
                            writeObj.insert("content",QJsonValue("find-person"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            QByteArray size1;//��дһ��sizeͷ ���ܸ���size��ÿд��һ�ζ�������
                            quint8 s=writeJson.toJson().size();
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
                            //����������״̬�仯
                            dir.setPath(QString("../userData/%1/friendsInfo/chat").arg(myqq));
                            if(dir.exists()){
                                QStringList list=dir.entryList(QStringList("*.db"),QDir::Files);
                                QJsonObject statusObj;//״̬�������� number:status
                                qDebug()<<"database size is "<<list.size();
                                for (QString name :list ) {
                                    QString number=name.mid(1,name.length()-4);//_(.*).db ����
                                    QString cn1=number+QString("%1%2").arg(QDateTime::currentDateTime().toTime_t()).arg(count);
                                    QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",cn1);
                                     dbPool.append(cn1);
                                    db1.setDatabaseName(QString("../userData/%1/friendsInfo/chat/_%2.db").arg(myqq).arg(number));
                                    db1.setHostName("MyQQ");
                                    db1.setUserName("sa");
                                    db1.setPassword("@123456x");
                                    if(db1.open())
                                        qDebug()<<("��sqliite���ݿ�ɹ�����������")<<db1.connectionName();
                                    else {
                                        qDebug()<<("��sqliite���ݿ�ʧ�ܣ�����:")<<(db1.databaseName());
                                        continue;//����
                                    }
                                    QSqlQuery query1(db1);
                                    ok=  query1.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1status' ").arg(number));
                                    if(!ok){
                                        qDebug()<<"count of table query is of failure ";
                                        continue;
                                    }
                                    query1.next();
                                    if(query1.value(0).toInt()!=0){
                                        ok= query1.exec(QString(" select status from _%1status ").arg(number));
                                        if(!ok){
                                            qDebug()<<"select status from _%1status failure";
                                            continue;
                                        }
                                        query1.next();
                                        QString status=query1.value("status").toString();
                                        statusObj.insert(number,status);//����״̬����
                                        //ɾ��״̬��
                                        ok=query1.exec(QString(" drop table _%1status ").arg(number));
                                        if(!ok){
                                            qDebug()<<"warning: drop table _%1status sentence executing failure";
                                        }
                                    }

                                }
                                //����״̬����
                                if(!statusObj.isEmpty()){
                                    QJsonDocument tdoc(statusObj);
                                    QByteArray sdata=tdoc.toJson();
                                    writeJson=QJsonDocument();
                                    writeObj=QJsonObject();
                                    writeObj.insert("content",QJsonValue("status"));
                                    writeObj.insert("size",QJsonValue(sdata.size()));
                                    writeJson.setObject(writeObj);
                                    QByteArray sizedata;
                                    quint8 l=writeJson.toJson().size();
                                    QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                                    sizeStr.setVersion(QDataStream::Qt_4_0);
                                    sizeStr<<l;
                                    tcpsocket->write(sizedata+ writeJson.toJson());
                                    loop.exec();
                                    tcpsocket->write(sdata);
                                    loop.exec();
                                }
                            }

                        }else ok=false;
                        qDebug()<<"result is "<<ok;
                        //��������ʱ�� ����Ծ�� ����ȼ��仯��д�����·���ļ� ��ȫ��������
                        if(query.exec(QString(" select liveTime,activeDays,myqqGrade from userInfo where myqq=%1 ").arg(myqq.toLongLong()))){
                           query.next();//��ȡ��¼
                            QString time;//��������ʱ��
                            QString lTime=query.value("liveTime").toString();
                            qint64 activeDays=query.value("activeDays").toLongLong();
                            int grade=query.value("myqqGrade").toInt();
                            QStringList list=  lTime.split(";;",QString::SkipEmptyParts);
                            if(list.size()>=2){
                                QDate date=QDate::fromString(list.at(0),Qt::ISODate);
                                if(date!=QDate::currentDate()){
                                    time="0";//����Ϊ0
                                }else{
                                    time=list.at(1);
                                }
                            }else{
                                time="0";
                            }
                            int l =time.toInt();
                            bool isNeesUpdate=false;
                            if(l<120){
                                l+=5;
                                //����2Сʱ ��Ծ����1
                                if(l>=120){
                                    activeDays+=1;
                                    int temp=(grade+5)*(grade+1);
                                    if(activeDays>=temp){
                                        grade+=1;
                                        activeDays=activeDays-temp;
                                        isNeesUpdate=true;
                                    }
                                }
                            }else{
                                l+=5;
                            }
                            if(isNeesUpdate){
                                query.prepare(" update userInfo set liveTime=?,activeDays=?,myqqGrade=? where myqq=? ");
                                query.addBindValue(QVariant(QDate::currentDate().toString("yyyy-MM-dd")+QString(";;%1").arg(l)));
                                query.addBindValue(QVariant(activeDays));
                                query.addBindValue(QVariant(grade));
                                query.addBindValue(QVariant(myqq.toLongLong()));
                                if(query.exec()){
                                    //д���������json�ļ�
                                    QFile friendInfo("../userData/"+QString("%1").arg(myqq)+"/info.xml");
                                    QStringList friendList;
                                    if(friendInfo.open(QIODevice::ReadOnly)){
                                        QXmlStreamReader reader;
                                        reader.setDevice(&friendInfo);
                                        friendInfo.close();
                                        while (!reader.atEnd()) {
                                            reader.readNextStartElement();
                                            if(reader.name().toString()=="friend"){
                                                friendList.append(reader.attributes().value("myqq").toString());
                                            }
                                        }
                                    }
                                    for (QString v : friendList) {
                                        QDir dir("../userData/"+v+"/friendsInfo");
                                        if(!dir.exists())dir.mkpath("./");
                                        QMutex*muter;//������
                                        QSharedPointer<QMutex>smuter;//����ָ����� ������ ���ü�����һ ������ȫɾ����
                                        if(gradeMuter.contains(v)){
                                           smuter =gradeMuter.value(v);//��ȡ���Ƶ�����ָ�����
                                            muter=smuter.data();
                                            muter->lock();
                                        }else{
                                            muter=new QMutex();
                                            QSharedPointer<QMutex>tmuter(muter,[=](QMutex*m){
                                                delete m,m=nullptr;
                                            });//��������ָ�뼰ɾ������
                                            gradeMuter.insert(v,tmuter);
                                            smuter=gradeMuter.value(v);//����map��ȡһ������������ָ����� ���ü�����1
                                            muter->lock();
                                        }
                                        QFile gtf(dir.filePath("grade.json"));
                                        if(gtf.open(QIODevice::ReadWrite)){
                                            QJsonDocument gtdoc=QJsonDocument::fromJson(gtf.readAll());
                                            QJsonObject gtobj=gtdoc.object();
                                            gtobj.insert(myqq,QJsonValue(grade));//���롢���µȼ�
                                            gtdoc=QJsonDocument();
                                            gtdoc.setObject(gtobj);
                                            gtf.write(gtdoc.toJson());//д���ļ�
                                            gtf.close();//�ر�
                                        }else{
                                            qDebug()<<"opened a file failure";
                                        }
                                        muter->unlock();//����
                                    }
                                }else{
                                    qDebug()<<"update the grade is of failure";
                                }
                            }else{//��������ʱ��ͻ�Ծ��
                                query.prepare(" update userInfo set liveTime=?,activeDays=? where myqq=? ");
                                query.addBindValue(QVariant(QDate::currentDate().toString("yyyy-MM-dd")+QString(";;%1").arg(l)));
                                query.addBindValue(QVariant(activeDays));
                                query.addBindValue(QVariant(myqq.toLongLong()));
                                if(!query.exec()){
                                    qDebug()<<"sentence 'update userInfo set liveTime=?,activeDays=? where myqq=?' executing failure";
                                }
                            }
                            writeJson=QJsonDocument();
                            writeObj=QJsonObject();
                            writeObj.insert("content",QJsonValue("mygrade"));
                            writeObj.insert("isNeedUpdate",QJsonValue(isNeesUpdate));
                            if(isNeesUpdate){
                            writeObj.insert("grade",QJsonValue(grade));
                            }
                              writeObj.insert("activeDays",QJsonValue(QString("%1").arg(activeDays)));//תΪ��ȫ����
                            writeJson.setObject(writeObj);
                            QByteArray sizedata;
                            quint8 lt=writeJson.toJson().size();
                            QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<lt;
                            tcpsocket->write(sizedata+ writeJson.toJson());
                            loop.exec();
                        }else{
                            qDebug()<<"querying the grade is of failure";
                        }

                        //������ѵȼ��Ƿ�仯
                        QFile gradeFile("../userData/"+myqq+"/friendsInfo/grade.json");
                        if(gradeFile.exists()){
                            qDebug()<<"it's need updating for friends grade";
                            if(gradeMuter.contains(myqq)){
                                QSharedPointer<QMutex>&smuter=gradeMuter[myqq];//���ö���
                                QMutex*muter=smuter.data();
                                muter->lock();
                            if(gradeFile.open(QIODevice::ReadOnly)){
                                QJsonDocument gjson=QJsonDocument::fromJson(gradeFile.readAll());
                                QJsonObject gobj=gjson.object();
                                if(gobj.isEmpty()){
                                    qDebug()<<"warning:the grade.json file is empty";
                                }else{
                                    //���� ��ȡ����
                                    QJsonObject fobj;//��������
                                   QJsonObject::const_iterator pos=gobj.constBegin();
                                   QJsonObject::const_iterator end=gobj.constEnd();
                                   while (pos!=end) {
                                     fobj.insert(pos.key(),pos.value());
                                     ++pos;
                                   }
                                   QJsonDocument fjson(fobj);
                                   writeJson=QJsonDocument();
                                   writeObj=QJsonObject();
                                   writeObj.insert("content",QJsonValue("friendsgrade"));
                                   writeObj.insert("size",QJsonValue(fjson.toJson().size()));
                                   writeJson.setObject(writeObj);
                                   QByteArray sizedata;
                                   quint8 lt=writeJson.toJson().size();
                                   QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                                   sizeStr.setVersion(QDataStream::Qt_4_0);
                                   sizeStr<<lt;
                                   tcpsocket->write(sizedata+ writeJson.toJson());
                                   loop.exec();
                                   tcpsocket->write(fjson.toJson());
                                   loop.exec();
                                }
                                gradeFile.close();
                            }
                            gradeFile.remove();//��ɾ���ļ��ٽ���
                            gradeMuter.remove(myqq);//�Ƴ�
                            muter->unlock();//����
                            smuter.clear();//������� ����һ��ɾ�������ָ�����
                            }else{//�п����Ǹոն��ļ�ɾ������д�������д���ļ� ����������ȡ
                                if(gradeFile.open(QIODevice::ReadOnly)){
                                    QJsonDocument gjson=QJsonDocument::fromJson(gradeFile.readAll());
                                    QJsonObject gobj=gjson.object();
                                    if(gobj.isEmpty()){
                                        qDebug()<<"warning:the grade.json file is empty";
                                    }else{
                                        //���� ��ȡ����
                                        QJsonObject fobj;//��������
                                       QJsonObject::const_iterator pos=gobj.constBegin();
                                       QJsonObject::const_iterator end=gobj.constEnd();
                                       while (pos!=end) {
                                         fobj.insert(pos.key(),pos.value());
                                         ++pos;
                                       }
                                       QJsonDocument fjson(fobj);
                                       writeJson=QJsonDocument();
                                       writeObj=QJsonObject();
                                       writeObj.insert("content",QJsonValue("friendsgrade"));
                                       writeObj.insert("size",QJsonValue(fjson.toJson().size()));
                                       writeJson.setObject(writeObj);
                                       QByteArray sizedata;
                                       quint8 lt=writeJson.toJson().size();
                                       QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                                       sizeStr.setVersion(QDataStream::Qt_4_0);
                                       sizeStr<<lt;
                                       tcpsocket->write(sizedata+ writeJson.toJson());
                                       loop.exec();
                                       tcpsocket->write(fjson.toJson());
                                       loop.exec();
                                    }
                                    gradeFile.close();
                                    gradeFile.remove();
                                }
                                }
                        }
                        writeJson=QJsonDocument();//����
                        writeObj=QJsonObject();
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
                    //�������Ϸ���
                }else if(in.toString()=="8"){
                    QString value=obj.value("content").toString();
                    QString myqq=obj.value("myqq").toString();
                    qDebug()<<"value=="<<value;
                    //��myself��������
                    if(value=="getPersonalData"||value=="getFriendInformation"){
                        bool ok=true;
                        if(query.exec("exec myselfInformation "+myqq)){

                            qDebug()<<"query is of success for personal information";
                            query.next();
                            if(query.isValid()){
                                if(value=="getFriendInformation"){
                                    writeObj.insert("signature",query.value("personalizedSignature").toString());
                                    writeObj.insert("name",query.value("myqqName").toString());
                                    writeObj.insert("sex",query.value("sex").toString());
                                    writeObj.insert("myqqGrade",query.value("myqqGrade").toString());
                                }
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
                                        QStringList list=wall.entryList(QStringList("*"),QDir::Files,QDir::Name);
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
                    //���������֤��Ϣ
                }else if(in.toString()=="12"){
                    noDis=true;//�Ͽ����˳�
                    QString myqq=obj.value("myqq").toString();
                    QString content=obj.value("content").toString();
                    if(content=="verifyInfo"){
                        qDebug()<<"handling the verify information";
                        QString number=obj.value("number").toString();//�Է�myqq
                        QString tag=obj.value("tag").toString();
                        QString groupName=obj.value("groupName").toString();
                        QString verify=obj.value("verify").toString();
                        obj.remove("instruct");
                        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
                        obj.insert("time",QJsonValue(time));
                        obj.insert("hasProcessed",QJsonValue(false));
                        //д���¼
                        QDir dir("../userData/"+number+"/friendsInfo/verifyFriends");
                        if(!dir.exists()){
                            qDebug()<<"dir:"<<dir.dirName()<<"is not exists";
                            dir.mkpath("./");
                        }
                        QFile file(dir.filePath(myqq+".json"));
                        if(!file.open(QIODevice::WriteOnly)){
                            qDebug()<<dir.filePath(myqq+".json")<<" was opened unsuccessfully";
                            emit finished();

                            return;
                        }
                        QJsonDocument doc;
                        doc.setObject(obj);
                        file.write(doc.toJson());
                        file.close();
                        query.prepare(" select myqqName,signLog,isConnected from userInfo with(rowlock,holdlock) where myqq=? ");
                        query.addBindValue(number);
                        if(query.exec()){
                            qDebug()<<"verify select query is sucessful";
                            query.next();
                            bool isConet=query.value("isConnected").toBool();
                            QStringList signList=query.value("signlog").toString().split(";;",QString::SkipEmptyParts);
                            //��������
                            if(isConet&&signList.length()>0){
                                qDebug()<<"opposite party is online ";
                                QString ip =signList.at(0);
                                signList=ip.split(" ",QString::SkipEmptyParts);
                                ip=signList.at(1);//ip
                                QString temp=signList.at(2);//port
                                quint16 port=(quint16)temp.toUInt();
                                qDebug()<<"opposite ip and port:"<<ip<<" "<<port;
                                QString name=query.value("myqqName").toString();
                                QJsonObject tempObj;
                                tempObj.insert("name",QJsonValue(name));
                                tempObj.insert("number",QJsonValue(number));

                                tempObj.insert("time",QJsonValue(time));
                                writeInfoToOpposite(ip,port,tempObj,"friend-verify");

                                return;
                            }
                        }
                        emit finished();
                    }
                }
                //13  getVerifyArray ����֤��Ϣ
                else if(in.toString()=="13"){
                    QString myqq=obj.value("myqq").toString();
                    QString content=obj.value("content").toString();
                    if(content=="getVerifyArray"){
                        QDir dir("../userData/"+myqq+"/friendsInfo/verifyFriends");
                        if(dir.exists()){
                            QStringList list=dir.entryList(QStringList("*.json"),QDir::Files);
                            qint32 length=list.length();
                            for(qint32 i=0;i<length;++i)
                            {
                                QString fileName=dir.filePath(list.at(i));
                                QFile file(fileName);
                                if(file.open(QIODevice::ReadOnly)){
                                    qDebug()<<"opened a file,named "<<fileName;
                                    QByteArray data=file.readAll();
                                    QJsonDocument temp=QJsonDocument::fromJson(data);
                                    QJsonObject tempObj=temp.object();
                                    if(tempObj.isEmpty()){
                                        qDebug()<<"a error:a empty json object";
                                        continue;
                                    }
                                    QString number=tempObj.value("myqq").toString();
                                    if(number.isEmpty()){
                                        qDebug()<<"a error:a verify number is empty";
                                        continue;
                                    }
                                    query.prepare(" select myqqName,sex,birthday ,city.name  from userInfo with(rowlock),\
                                                  city with(rowlock) where myqq=? and city.id=userInfo.locateInCity ");
                                            query.addBindValue(QVariant(number));
                                            if(!query.exec()){
                                        qDebug()<<"a error:sql query is of failure";
                                        continue;
                                    }
                                    query.next();
                                    QString name=query.value("myqqName").toString();
                                    QString sex=query.value("sex").toString();
                                    QString bir=query.value("birthday").toString();
                                    QString city=query.value("name").toString();
                                    qint64 age=dateSubstraction(QDate::fromString(bir,Qt::ISODate),QDate::currentDate());
                                    QJsonObject cobj;
                                    cobj.insert("name",QJsonValue(name));
                                    cobj.insert("sex",QJsonValue(sex));
                                    cobj.insert("age",QJsonValue(age));
                                    cobj.insert("city",QJsonValue(city));
                                    cobj.insert("verify",QJsonValue(tempObj.value("verify").toString()));
                                    cobj.insert("time",QJsonValue(tempObj.value("time").toString()));
                                    writeObj.insert(number,QJsonValue(cobj));
                                }
                            }
                            writeJson.setObject(writeObj);
                            QByteArray data=writeJson.toJson();
                            QJsonDocument temp;
                            QJsonObject tempObj;
                            tempObj.insert("instruct",QJsonValue("130"));
                            tempObj.insert("content",QJsonValue("verifyJson"));
                            tempObj.insert("size",QJsonValue(data.size()));
                            temp.setObject(tempObj);
                            quint8 l=temp.toJson().size();
                            QByteArray size;
                            QDataStream s(&size,QIODevice::WriteOnly);
                            s<<l;
                            tcpsocket->write(size+temp.toJson());
                            loop.exec();
                            tcpsocket->write(data);
                            loop.exec();
                            list=writeObj.keys();
                            length=list.length();
                            for(qint32 i=0;i<length;++i)
                            {
                                QString number=list.at(i);
                                QFile file("../userData/"+number+"/historyHeadImg/01.png");
                                if(!file.open(QIODevice::ReadOnly)){
                                    qDebug()<<"warning:a file not opening";
                                    continue;
                                }
                                data=file.readAll();
                                if(data.isEmpty()){
                                    qDebug()<<"warning:a file  data is empty";
                                    continue;
                                }
                                writeJson=QJsonDocument();
                                writeObj=QJsonObject();
                                writeObj.insert("instruct",QJsonValue("130"));
                                writeObj.insert("content",QJsonValue("verifyImg"));
                                writeObj.insert("imgName",QJsonValue(number));
                                writeObj.insert("size",QJsonValue(data.size()));
                                writeJson.setObject(writeObj);
                                quint8 l1=writeJson.toJson().size();
                                QByteArray size1;
                                QDataStream s1(&size1,QIODevice::WriteOnly);
                                s1<<l1;
                                tcpsocket->write(size1+writeJson.toJson());
                                loop.exec();
                                tcpsocket->write(data);
                                loop.exec();
                            }
                            //����
                            writeJson=QJsonDocument();
                            writeObj=QJsonObject();
                            writeObj.insert("instruct",QJsonValue("130"));
                            writeObj.insert("content",QJsonValue("end"));
                            writeObj.insert("result",QJsonValue("true"));
                            writeJson.setObject(writeObj);
                            QByteArray sizedata;
                            quint8 l1=writeJson.toJson().size();
                            QDataStream sizeStr(&sizedata,QIODevice::WriteOnly);
                            sizeStr.setVersion(QDataStream::Qt_4_0);
                            sizeStr<<l1;
                            tcpsocket->write(sizedata+ writeJson.toJson());
                            loop.exec();
                            tcpsocket->disconnectFromHost();
                        }else{
                            //û��
                            writeObj.insert("instruct",QJsonValue("130"));
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
                    }
                    //��Ӻ���
                }else if(in.toString()=="14"){
                    noDis=true;
                    QString myqq=obj.value("myqq").toString();
                    QString content=obj.value("content").toString();
                    if(content=="FVerify"){
                        qDebug()<<"FVerify";
                        QString number=obj.value("number").toString();
                        QString handle=obj.value("handle").toString();
                        QDir dir("../userData/"+myqq+"/friendsInfo/verifyFriends");
                        if(dir.exists()){
                            QFile file(dir.filePath(number+".json"));
                            if(file.exists()){
                                if(handle=="add"){
                                    qDebug()<<"add";
                                    if(!file.open(QIODevice::ReadOnly)){
                                        qDebug()<<"file is of failure to open";
                                    }
                                    QByteArray data=file.readAll();
                                    QJsonDocument tdoc=QJsonDocument::fromJson(data);
                                    QJsonObject tobj=tdoc.object();
                                    query.prepare(" select myqqName, personalizedSignature,signLog,myqqGrade,userState,isConnected from userInfo with(rowlock,holdlock) where myqq=? ");
                                    query.addBindValue(QVariant(number));
                                    if(query.exec()){
                                        qDebug()<<"query is successful";
                                        query.next();
                                        QString name1=query.value("myqqName").toString();
                                        QString sig1=query.value("personalizedSignature").toString();
                                        QString grade1=query.value("myqqGrade").toString();
                                        QString status1=query.value("userState").toString();
                                        bool isCon=query.value("isConnected").toBool();
                                        QString log1=query.value("signLog").toString();
                                        query.prepare("  select myqqName,personalizedSignature,signlog,myqqGrade,userState from userInfo with(rowlock,holdlock) where myqq=? ");
                                        query.addBindValue(QVariant(myqq.toLongLong()));
                                        if(!query.exec()){
                                            qDebug()<<"a query is of failure";
                                        }
                                        query.next();
                                        QString name2=query.value("myqqName").toString();
                                        QString sig2=query.value("personalizedSignature").toString();
                                        QString grade2=query.value("myqqGrade").toString();
                                        QString status2=query.value("userState").toString();
                                        QString log2=query.value("signLog").toString();

                                        QString ogn=tobj.value("groupName").toString();//�Է��������
                                        QString gn=obj.value("groupName").toString();//�������
                                        qDebug()<<"my added group name:"<<gn<<";opposite part:"<<ogn;
                                        //�޸ĶԷ����ļ�
                                        QFile opFile("../userData/"+number+"/info.xml");
                                        if(opFile.open(QIODevice::ReadWrite)){
                                            qDebug()<<opFile.fileName();
                                            QDomDocument doc;
                                            doc.setContent(&opFile);
                                            opFile.close();
                                            QDomElement dele=doc.documentElement();
                                            dele=dele.firstChildElement("friendGroup");
                                            QDomNodeList list=  dele.childNodes();
                                            qint32 length=list.length();
                                            bool isFound=false;
                                            for(qint32 i=0;i<length;++i){
                                                QDomElement tempEle=list.item(i).toElement();
                                                if(ogn==tempEle.attribute("name")){
                                                    qDebug()<<"find the friends group:"<<ogn;
                                                    isFound=true;
                                                    if(tempEle.text()=="none")
                                                        tempEle.removeChild(tempEle.firstChild());
                                                    QDomElement meEle=doc.createElement("friend");
                                                    meEle.setAttribute("myqq",QString("%1").arg(myqq));
                                                    QDomElement nameEle=doc.createElement(("�ǳ�"));
                                                    QDomText nameT=doc.createTextNode(name2);
                                                    nameEle.appendChild(nameT);
                                                    QDomElement signatureEle=doc.createElement(("����ǩ��"));
                                                    if(sig2.isEmpty()){
                                                        signatureEle.setAttribute("isNull","true");
                                                        QDomText signatureT=doc.createTextNode("none");
                                                        signatureEle.appendChild(signatureT);
                                                    }else{
                                                        signatureEle.setAttribute("isNull","false");
                                                        QDomText signatureT=doc.createTextNode(sig2);
                                                        signatureEle.appendChild(signatureT);
                                                    }
                                                    QDomElement headPathEle=doc.createElement(("ͷ��·��"));
                                                    QDomText headPathT=doc.createTextNode("../userData/"+myqq+"/historyHeadImg/01.png");
                                                    headPathEle.appendChild(headPathT);
                                                    QDomElement tagEle=doc.createElement(("��ע"));
                                                    if(tobj.value("tag").toString().isEmpty()){
                                                        tagEle.setAttribute("isNull","true");
                                                        QDomText tagT=doc.createTextNode("none");//Ĭ��Ϊnone
                                                        tagEle.appendChild(tagT);
                                                    }else{
                                                        tagEle.setAttribute("isNull","false");
                                                        QDomText tagT=doc.createTextNode(tobj.value("tag").toString());//Ĭ��Ϊnone
                                                        tagEle.appendChild(tagT);
                                                    }
                                                    QDomElement gradeEle=doc.createElement(("�ȼ�"));
                                                    QDomText gradeT=doc.createTextNode(grade2);
                                                    gradeEle.appendChild(gradeT);
                                                    QDomElement statusEle=doc.createElement(("״̬"));
                                                    QDomText statusT=doc.createTextNode(status2);
                                                    statusEle.appendChild(statusT);
                                                    QDomElement setEle=doc.createElement(("set"));
                                                    setEle.setAttribute("isNull","true");
                                                    setEle.setAttribute("infoet","0");
                                                    setEle.setAttribute("status","0");
                                                    QDomText setT=doc.createTextNode("none");
                                                    setEle.appendChild(setT);
                                                    meEle.appendChild(nameEle);
                                                    meEle.appendChild(signatureEle);
                                                    meEle.appendChild(headPathEle);
                                                    meEle.appendChild(tagEle);
                                                    meEle.appendChild(gradeEle);
                                                    meEle.appendChild(statusEle);
                                                    meEle.appendChild(setEle);

                                                    tempEle.appendChild(meEle);
                                                    break;
                                                }
                                            }
                                            if(!isFound){
                                                qDebug()<<"warning:not found the friend groups it may be caused by "
                                                          "the auxiliary process not updating the friend groups data in time when adding friends "
                                                          "or rejected the friend request after long time result in friend group deletion";
                                                QDomElement tempEle=list.item(1).toElement();
                                                qDebug()<<"the friend will be added to  default group "<<tempEle.attribute("name");
                                                if(tempEle.text()=="none")
                                                    tempEle.removeChild(tempEle.firstChild());
                                                QDomElement meEle=doc.createElement("friend");
                                                meEle.setAttribute("myqq",QString("%1").arg(myqq));
                                                QDomElement nameEle=doc.createElement(("�ǳ�"));
                                                QDomText nameT=doc.createTextNode(name2);
                                                nameEle.appendChild(nameT);
                                                QDomElement signatureEle=doc.createElement(("����ǩ��"));
                                                if(sig2.isEmpty()){
                                                    signatureEle.setAttribute("isNull","true");
                                                    QDomText signatureT=doc.createTextNode("none");
                                                    signatureEle.appendChild(signatureT);
                                                }else{
                                                    signatureEle.setAttribute("isNull","false");
                                                    QDomText signatureT=doc.createTextNode(sig2);
                                                    signatureEle.appendChild(signatureT);
                                                }
                                                QDomElement headPathEle=doc.createElement(("ͷ��·��"));
                                                QDomText headPathT=doc.createTextNode("../userData/"+myqq+"/historyHeadImg/01.png");
                                                headPathEle.appendChild(headPathT);
                                                QDomElement tagEle=doc.createElement(("��ע"));
                                                if(tobj.value("tag").toString().isEmpty()){
                                                    tagEle.setAttribute("isNull","true");
                                                    QDomText tagT=doc.createTextNode("none");//Ĭ��Ϊnone
                                                    tagEle.appendChild(tagT);
                                                }else{
                                                    tagEle.setAttribute("isNull","false");
                                                    QDomText tagT=doc.createTextNode(tobj.value("tag").toString());//Ĭ��Ϊnone
                                                    tagEle.appendChild(tagT);
                                                }
                                                QDomElement gradeEle=doc.createElement(("�ȼ�"));
                                                QDomText gradeT=doc.createTextNode(grade2);
                                                gradeEle.appendChild(gradeT);
                                                QDomElement statusEle=doc.createElement(("״̬"));
                                                QDomText statusT=doc.createTextNode(status2);
                                                statusEle.appendChild(statusT);
                                                QDomElement setEle=doc.createElement(("set"));
                                                setEle.setAttribute("isNull","true");
                                                setEle.setAttribute("infoet","0");
                                                setEle.setAttribute("status","0");
                                                QDomText setT=doc.createTextNode("none");
                                                setEle.appendChild(setT);
                                                meEle.appendChild(nameEle);
                                                meEle.appendChild(signatureEle);
                                                meEle.appendChild(headPathEle);
                                                meEle.appendChild(tagEle);
                                                meEle.appendChild(gradeEle);
                                                meEle.appendChild(statusEle);
                                                meEle.appendChild(setEle);

                                                tempEle.appendChild(meEle);
                                            }
                                            if(!opFile.open(QIODevice::WriteOnly)){
                                                qDebug()<<opFile.fileName()<<" is  opened to save unsuccessfully";
                                            }else{
                                                QTextStream stream(&opFile);
                                                stream.setCodec("utf-8");
                                                doc.save(stream,4,QDomNode::EncodingFromTextStream);
                                                opFile.close();
                                            }
                                        }
                                        QFile tempFile("../userData/"+myqq+"/historyHeadImg/01.png");
                                        if(tempFile.open(QIODevice::ReadOnly)){
                                            QByteArray pixData=tempFile.readAll();
                                            if(pixData.isEmpty())qDebug()<<"opposite part image of head is empty";
                                            //�޸�myself�ļ�
                                            QFile meFile("../userData/"+myqq+"/info.xml");
                                            if(meFile.open(QIODevice::ReadWrite)){
                                                qDebug()<<meFile.fileName();
                                                QDomDocument doc1;
                                                doc1.setContent(&meFile);
                                                meFile.close();
                                                QDomElement dele1=doc1.documentElement();
                                                dele1=dele1.firstChildElement("friendGroup");
                                                QDomNodeList list1=  dele1.childNodes();
                                                qint32 length=list1.length();
                                                bool isFound=false;
                                                for(qint32 i=0;i<length;++i){
                                                    QDomElement tempEle=list1.item(i).toElement();
                                                    if(gn==tempEle.attribute("name")){
                                                        isFound=true;
                                                        if(tempEle.text()=="none")
                                                            tempEle.removeChild(tempEle.firstChild());
                                                        qDebug()<<"find the friends group:"<<gn;
                                                        QDomElement meEle=doc1.createElement("friend");
                                                        meEle.setAttribute("myqq",QString("%1").arg(number));
                                                        QDomElement nameEle=doc1.createElement(("�ǳ�"));
                                                        QDomText nameT=doc1.createTextNode(name1);
                                                        nameEle.appendChild(nameT);
                                                        QDomElement signatureEle=doc1.createElement(("����ǩ��"));
                                                        if(sig1.isEmpty()){
                                                            signatureEle.setAttribute("isNull","true");
                                                            QDomText signatureT=doc1.createTextNode("none");
                                                            signatureEle.appendChild(signatureT);
                                                        }else{
                                                            signatureEle.setAttribute("isNull","false");
                                                            QDomText signatureT=doc1.createTextNode(sig1);
                                                            signatureEle.appendChild(signatureT);
                                                        }
                                                        QDomElement headPathEle=doc1.createElement(("ͷ��·��"));
                                                        QDomText headPathT=doc1.createTextNode("../userData/"+number+"/historyHeadImg/01.png");
                                                        headPathEle.appendChild(headPathT);
                                                        QDomElement tagEle=doc1.createElement(("��ע"));
                                                        if(obj.value("tag").toString().isEmpty()){
                                                            tagEle.setAttribute("isNull","true");
                                                            QDomText tagT=doc1.createTextNode("none");//Ĭ��Ϊnone
                                                            tagEle.appendChild(tagT);
                                                        }else{
                                                            tagEle.setAttribute("isNull","false");
                                                            QDomText tagT=doc1.createTextNode(obj.value("tag").toString());//Ĭ��Ϊnone
                                                            tagEle.appendChild(tagT);
                                                        }
                                                        QDomElement gradeEle=doc1.createElement(("�ȼ�"));
                                                        QDomText gradeT=doc1.createTextNode(grade1);
                                                        gradeEle.appendChild(gradeT);
                                                        QDomElement statusEle=doc1.createElement(("״̬"));
                                                        QDomText statusT=doc1.createTextNode(status1);
                                                        statusEle.appendChild(statusT);
                                                        QDomElement setEle=doc1.createElement(("set"));
                                                        setEle.setAttribute("isNull","true");
                                                        setEle.setAttribute("info","0");
                                                        setEle.setAttribute("status","0");
                                                        QDomText setT=doc1.createTextNode("none");
                                                        setEle.appendChild(setT);
                                                        meEle.appendChild(nameEle);
                                                        meEle.appendChild(signatureEle);
                                                        meEle.appendChild(headPathEle);
                                                        meEle.appendChild(tagEle);
                                                        meEle.appendChild(gradeEle);
                                                        meEle.appendChild(statusEle);
                                                        meEle.appendChild(setEle);
                                                        tempEle.appendChild(meEle);
                                                        break;
                                                    }
                                                }
                                                if(!isFound){
                                                    qDebug()<<"warning:not found the friend groups it may be caused by "
                                                              "the auxiliary process not updating the friend groups data in time when adding friends "
                                                              "or rejected the friend request after long time result in friend group deletion";
                                                    QDomElement tempEle=list1.item(1).toElement();
                                                    qDebug()<<"the friend will be added to  default group "<<tempEle.attribute("name");
                                                    if(tempEle.text()=="none")
                                                        tempEle.removeChild(tempEle.firstChild());
                                                    qDebug()<<"find the friends group:"<<gn;
                                                    QDomElement meEle=doc1.createElement("friend");
                                                    meEle.setAttribute("myqq",QString("%1").arg(number));
                                                    QDomElement nameEle=doc1.createElement(("�ǳ�"));
                                                    QDomText nameT=doc1.createTextNode(name1);
                                                    nameEle.appendChild(nameT);
                                                    QDomElement signatureEle=doc1.createElement(("����ǩ��"));
                                                    if(sig1.isEmpty()){
                                                        signatureEle.setAttribute("isNull","true");
                                                        QDomText signatureT=doc1.createTextNode("none");
                                                        signatureEle.appendChild(signatureT);
                                                    }else{
                                                        signatureEle.setAttribute("isNull","false");
                                                        QDomText signatureT=doc1.createTextNode(sig1);
                                                        signatureEle.appendChild(signatureT);
                                                    }
                                                    QDomElement headPathEle=doc1.createElement(("ͷ��·��"));
                                                    QDomText headPathT=doc1.createTextNode("../userData/"+number+"/historyHeadImg/01.png");
                                                    headPathEle.appendChild(headPathT);
                                                    QDomElement tagEle=doc1.createElement(("��ע"));
                                                    if(obj.value("tag").toString().isEmpty()){
                                                        tagEle.setAttribute("isNull","true");
                                                        QDomText tagT=doc1.createTextNode("none");//Ĭ��Ϊnone
                                                        tagEle.appendChild(tagT);
                                                    }else{
                                                        tagEle.setAttribute("isNull","false");
                                                        QDomText tagT=doc1.createTextNode(obj.value("tag").toString());//Ĭ��Ϊnone
                                                        tagEle.appendChild(tagT);
                                                    }
                                                    QDomElement gradeEle=doc1.createElement(("�ȼ�"));
                                                    QDomText gradeT=doc1.createTextNode(grade1);
                                                    gradeEle.appendChild(gradeT);
                                                    QDomElement statusEle=doc1.createElement(("״̬"));
                                                    QDomText statusT=doc1.createTextNode(status1);
                                                    statusEle.appendChild(statusT);
                                                    QDomElement setEle=doc1.createElement(("set"));
                                                    setEle.setAttribute("isNull","true");
                                                    setEle.setAttribute("info","0");
                                                    setEle.setAttribute("status","0");
                                                    QDomText setT=doc1.createTextNode("none");
                                                    setEle.appendChild(setT);
                                                    meEle.appendChild(nameEle);
                                                    meEle.appendChild(signatureEle);
                                                    meEle.appendChild(headPathEle);
                                                    meEle.appendChild(tagEle);
                                                    meEle.appendChild(gradeEle);
                                                    meEle.appendChild(statusEle);
                                                    meEle.appendChild(setEle);
                                                    tempEle.appendChild(meEle);
                                                }
                                                if(!meFile.open(QIODevice::WriteOnly)){
                                                    qDebug()<<meFile.fileName()<<" is  opened to save unsuccessfully";
                                                }else{
                                                    QTextStream stream(&meFile);
                                                    stream.setCodec("utf-8");
                                                    doc1.save(stream,4,QDomNode::EncodingFromTextStream);
                                                    meFile.close();
                                                }
                                                QStringList signList1=log1.split(";;",QString::SkipEmptyParts);
                                                //��������
                                                if(isCon&&signList1.length()>0){
                                                    qDebug()<<"opposite party is online ";
                                                    QString ip =signList1.at(0);
                                                    signList1=ip.split(" ",QString::SkipEmptyParts);
                                                    if(signList1.size()>2){//�ų��������
                                                    ip=signList1.at(1);//ip
                                                    QString temp=signList1.at(2);//port
                                                    quint16 port=(quint16)temp.toUInt();
                                                    qDebug()<<"opposite ip and port:"<<ip<<" "<<port<<number;
                                                    QJsonObject tempObj;
                                                    tempObj.insert("name",QJsonValue(name2));
                                                    tempObj.insert("number",QJsonValue(myqq));
                                                    tempObj.insert("signature",QJsonValue(sig2));
                                                    tempObj.insert("grade",QJsonValue(grade2));
                                                    tempObj.insert("imgPath",QJsonValue("../userData/"+myqq+"/historyHeadImg/01.png"));
                                                    tempObj.insert("status",QJsonValue(status2));
                                                    tempObj.insert("infoSet",QJsonValue("0"));
                                                    tempObj.insert("statusSet",QJsonValue("0"));
                                                    tempObj.insert("verify",QJsonValue(tobj.value("verify").toString()));
                                                    tempObj.insert("tag",QJsonValue(tobj.value("tag").toString()));
                                                    tempObj.insert("groupName",QJsonValue(ogn));
                                                    tempObj.insert("time",QJsonValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                                                    //д��Է���ַ�˿�
                                                    writeInfoToOpposite(ip,port,tempObj,"addFriend",pixData,false);
}
                                                }else{
                                                    qDebug()<<"opposite is not online";
                                                    //��¼���������Ϣ
                                                    //����Ŀ¼
                                                    QDir dir1("../userData/"+number+"/friendsInfo/chat");
                                                    if(!dir1.exists())dir1.mkpath("./");
                                                    QString cn1=QString("%1").arg(QDateTime::currentDateTime().toTime_t())+db.connectionName();
                                                    QSqlDatabase db1=QSqlDatabase::addDatabase("QSQLITE",cn1);
                                                     dbPool.append(cn1);
                                                    db1.setDatabaseName(QString("../userData/%1/friendsInfo/chat/_%2.db").arg(number).arg(myqq));
                                                    db1.setHostName("MyQQ");
                                                    db1.setUserName("sa");
                                                    db1.setPassword("@123456x");
                                                    if(db1.open())
                                                        qDebug()<<("��sqliite���ݿ�ɹ����������ǣ�")<<db1.connectionName();
                                                    else {
                                                        qDebug()<<("��sqliite���ݿ�ʧ�ܣ�");

                                                        return;
                                                    }
                                                    bool ok;
                                                    QSqlQuery query1(db1);
                                                    ok=  query1.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1info' ").arg(myqq));
                                                    if(!ok){
                                                        qDebug()<<"count of table query is of failure ";

                                                        return;
                                                    }
                                                    query1.next();
                                                    if(query1.value(0).toInt()==0){
                                                        ok= query1.exec(QString( " create table _%1info (\
                                                                                 id integer  not null primary key  autoincrement,\
                                                                                 data BLOD not null,\
                                                                                 datetime datetime not null,\
                                                                                 type text not null,\
                                                                                 adhesion bit default 0,\
                                                                                 number text not null\
                                                                                 )").arg(myqq) );
                                                                        if(!ok){
                                                                            qDebug()<<"table creation is of failure ,name ";
                                                                          return;
                                                                        }
                                                    }
                                                    QString  cur=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//ʱ��
                                                    QByteArray idata=QString("�����Ѿ��Ǻ�������һ��������ɣ�").toUtf8().toHex();//����
                                                    query1.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(myqq));
                                                    query1.addBindValue(QVariant(idata));
                                                    query1.addBindValue(QVariant(cur));
                                                    query1.addBindValue(QVariant("text"));
                                                    query1.addBindValue(QVariant(false));
                                                    query1.addBindValue(QVariant(myqq.toLongLong()));
                                                    ok=query1.exec();
                                                    if(!ok)qDebug()<<"added information is of failure";

                                                }
                                                //�����Ƿ����߶����Լ���� �ظ��Լ�
                                                QStringList msignList=log2.split(";;",QString::SkipEmptyParts);
                                              if(msignList.size()>0){
                                                QString mip =msignList.at(0);
                                                msignList=mip.split(" ",QString::SkipEmptyParts);
                                                if(msignList.size()>2){//�ų��������
                                                mip=msignList.at(1);//ip
                                                QString temp=msignList.at(2);//port
                                                quint16 mport=(quint16)temp.toUInt();
                                                qDebug()<<"my ip and port:"<<mip<<" "<<mport;
                                                QJsonObject tempObj1;
                                                tempObj1.insert("name",QJsonValue(name1));
                                                tempObj1.insert("number",QJsonValue(number));
                                                tempObj1.insert("signature",QJsonValue(sig1));
                                                tempObj1.insert("grade",QJsonValue(query.value("myqqGrade").toString()));
                                                tempObj1.insert("imgPath",QJsonValue("../userData/"+number+"/historyHeadImg/01.png"));
                                                tempObj1.insert("status",QJsonValue(status1));
                                                tempObj1.insert("infoSet",QJsonValue("0"));
                                                tempObj1.insert("statusSet",QJsonValue("0"));
                                                tempObj1.insert("tag",QJsonValue(obj.value("tag").toString()));
                                                tempObj1.insert("groupName",QJsonValue(gn));
                                                tempObj1.insert("verify",QJsonValue(tobj.value("verify").toString()));
                                                //д����շ�ͼ��
                                                QFile file("../userData/"+number+"/historyHeadImg/01.png");
                                                if(!file.open(QIODevice::ReadOnly)){
                                                    qDebug()<<"a file is of failure to open";
                                                }
                                                QByteArray mImg=file.readAll();
                                                if(mImg.isEmpty())qDebug()<<"a error:data of reading file is empty";
                                                writeInfoToOpposite(mip,mport,tempObj1,"addFriend",mImg);
                                                }
                                              }else{
                                                  qDebug()<<"log list is unexpected:"<<msignList;

                                              }
                                            }
                                        }
                                    }
                                }
                                file.close();
                                qDebug()<<"file:"<<file.fileName()<<",remove result:"<< file.remove();
                            }
                        }
                    }
                    //��ȡ����ip �˿�
                }else if(in.toString()=="17"){
                    bool ok;
                    QString myqq=obj.value("myqq").toString();
                    QString number=obj.value("number").toString();
                    QString content=obj.value("content").toString();
                    if(!number.isEmpty()&&!myqq.isEmpty()){

                        if(content=="getFIP"){
                            qDebug()<<"getFIP";
                            ok=  query.exec(QString(" select signLog,isConnected,userState from userInfo where myqq=%1 ").arg(number));
                            //ʧ��ȫ��goto ��ʧ�ܴ���
                            if(!ok){
                                qDebug()<<"it is of failure for execing a sql query";
                                goto notGetIp;
                            }else{
                                query.next();
                                QString log=query.value("signLog").toString();
                                bool live=query.value("isConnected").toBool();
                                QString status=query.value("userState").toString();
                                QString ip;quint16 port;
                                QStringList list=log.split(";;",QString::SkipEmptyParts);
                                try{
                                    if(list.size()<1)throw "index out of range";
                                    ip =list.at(0);
                                }catch(_exception& e){
                                    qDebug()<<"error:type="<<e.type<<",name="<<e.name;
                                    goto notGetIp;
                                }catch(...){
                                    qDebug()<<"a unkown fatal error";
                                    goto notGetIp;
                                }
                                list=ip.split(" ",QString::SkipEmptyParts);
                                ip=list.at(1);//ip
                                QString temp=list.at(2);//port
                                port=(quint16)temp.toUInt();
                                writeObj.insert("instruct",QJsonValue("170"));
                                writeObj.insert("ip",QJsonValue(ip));
                                writeObj.insert("port",QJsonValue(port));
                                //���� ��ȡudp��Ϣ ��ip��Ϣ
                                if(live){
                                    //�ú���������lambda�����ݿ��������ã��Ӷ����������ݿ����ɾ���ж��������е�ͬ�����ӵ��±���,ע�ⲻҪ�����ݿ������и��� �ᵼ�¶���������ͬ������ ������ѯ����
                                   onlineDispatchedIP(writeObj,number,ip,port,status);
                                    //������
                                }else{
                                    qDebug()<<" opposite is offline";
                                    writeObj.insert("result",QJsonValue("true"));
                                    writeObj.insert("online",QJsonValue(false));
                                    writeObj.insert("status",QJsonValue(status));
                                    writeJson.setObject(writeObj);
                                    QByteArray size;
                                    QDataStream s(&size,QIODevice::WriteOnly);
                                    s.setVersion(QDataStream::Qt_4_0);
                                    quint8 l=writeJson.toJson().size();
                                    s<<l;
                                    tcpsocket->write(size+writeJson.toJson());
                                    loop.exec();
                                    emit finished();
                                }
                            }

                            return;
                        }
                    }
                    //ʧ��ִ�� ��ǩ
notGetIp:
                    qDebug()<<("failed exec")<<endl;
                    writeObj=QJsonObject();
                    writeJson=QJsonDocument();
                    writeObj.insert("instruct",QJsonValue("170"));
                    writeObj.insert("result",QJsonValue("false"));
                    writeJson.setObject(writeObj);
                    quint8 s=writeJson.toJson().size();
                    QByteArray size;
                    QDataStream stream(&size,QIODevice::WriteOnly);
                    stream.setVersion(QDataStream::Qt_4_0);
                    stream<<s;
                    tcpsocket->write(size+writeJson.toJson());
                    loop.exec();
                    emit finished();

                    return;
                }
            }
        }
    }

}
//�ͻ��˶Ͽ����Ӿ�ɾ���̼߳�����
void LoginThread::disconnected()
{
    qDebug()<<"disconnected,noDis="<<noDis;
    if(!noDis)
        emit finished();
}

QString LoginThread::handleLogRec(QString &rec,QString& host,QString&ip,QString&port) const
{
    QStringList list=rec.split(";;",QString::SkipEmptyParts);
    int count=list.count(),hostRow=-1;
    for(int i=0;i<count;i++){
        if(list.at(i).contains(host)){
            hostRow=i;
        }
    }
    QDateTime time=QDateTime::currentDateTime();
    QString newRec=host+"  "+ip+"  "+port+"  "+time.toString("yyyy/MM/dd hh:mm:ss PM");
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


