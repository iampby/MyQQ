#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "myqqregisterserver.h"
#include"registerthread.h"
#include<qsqlquery.h>
#include<qbytearray.h>
#include<qdatastream.h>
#include<qdebug.h>
#include <qdir.h>
#include <qimage.h>
#include <QDomText>
#include <qdatetime.h>
#include<qmutex.h>
#include <qwidget.h>
MyQQRegisterServer::MyQQRegisterServer(QObject *parent)
    :QTcpServer (parent)
{
    count=0;
    ("");
}

MyQQRegisterServer::~MyQQRegisterServer()
{
    qDebug()<<"~MyQQRegisterServer()";
}


bool MyQQRegisterServer::registerMyQQ(const QString& name, const QString&passwd, qint64 &myqq,QString&cn)
{
    qDebug()<<"register executing thread is "<<this->thread()->currentThread();
    qDebug()<<name<<passwd;
    QMutex m;
    m.lock();//给count上锁
    if(count>=100)//理论上1秒内可以打开的数据库数 100
        count=0;
    cn=QDateTime::currentDateTime().toMSecsSinceEpoch()+QString("%1").arg(count++);
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",cn);
    m.unlock();//释放count
    db.setDatabaseName("qtmanager");
    db.setHostName("127.0.0.1");
    db.setUserName("sa");
    db.setPassword("123456x");
    if(db.open())
        qDebug()<<("打开数据库成功！");
    else {
        qDebug()<<("打开数据库失败！");
    }
    QSqlQuery query("use myqq",db);
    query.setForwardOnly(true);
    bool sucessful=false;
    query.prepare("declare @myqq bigint,@sucessful bit=0 "
                  "exec registerMyQQ_pro :name,:passwd, @myqq  output,@sucessful output "
                  "select @myqq,@sucessful");
    query.bindValue(":name",name);
    query.bindValue(":passwd",passwd);
    if(query.exec()){
        if(query.next()){
            myqq=query.value(0).toLongLong();
            sucessful=query.value(1).toBool();
            if(sucessful){
                qDebug()<<("注册成功！qq:")<<myqq<<endl;
                if(myqq!=0){
                    QDir dir;
                    dir.mkpath(QString("../userData/%1/historyHeadImg").arg(myqq));
                    QString headPath=dir.relativeFilePath(QString("../userData/%1/historyHeadImg").arg(myqq)+"/01.png");
                    QImage img(":/img/init.png");
                    if(img.save(headPath,nullptr,0)){
                        qDebug()<<"IMG is saved！";
                        query.prepare(" update userInfo set headimgpath=?,photoWallPath=? where myqq=? ");
                        query.bindValue(0,QVariant(headPath));
                        query.bindValue(1,QVariant(QString("../userData/%1/photoWall").arg(myqq)));
                        query.bindValue(2,QVariant(myqq));
                        if(query.exec()){
                            qDebug()<<"updating the headImgPath of myqq sucessfully,myqq equal to "+QString("%1").arg( myqq);
                            dir.mkpath(QString("../userData/%1/friendsInfo").arg(myqq));
                            dir.mkpath(QString("../userData/%1/groupsInfo").arg(myqq));
                            QFile infoFile(dir.relativeFilePath(QString("../userData/%1/info.xml").arg(myqq)));
                            if(infoFile.open(QIODevice::WriteOnly)){
                                qDebug()<<"creating info.xml file sucessfully";
                                QDomDocument doc;
                                QDomProcessingInstruction header=doc.createProcessingInstruction("xml","version="
                                                                                                       "\"1.0\" encoding=\"UTF-8\"");
                                doc.appendChild(header);
                                QDomElement root=doc.createElement("myqq");
                                root.setAttribute("number",myqq);
                                doc.appendChild(root);
                                QDomElement set=doc.createElement("set");
                                QStringList showList;//显示信息配置
                                showList<<("头像")<<("名称")<<("列表")
                                       <<("联系人")<<("黑名单");
                                for(int i=0;i<showList.count();i++){
                                    QDomElement showEle=doc.createElement(showList.at(i));
                                    QDomText showT= doc.createTextNode("0");
                                    showEle.appendChild(showT);
                                    set.appendChild(showEle);
                                }
                                QDomElement passwdEle=doc.createElement(("独立密码"));
                                QDomText passwdT= doc.createTextNode("none");
                                passwdEle.appendChild(passwdT);
                                set.appendChild(passwdEle);
                                root.appendChild(set);

                                QDomElement friends=doc.createElement("friendGroup");
                                QStringList friendGroup;
                                friendGroup<<("我的设备")<<("我的好友")
                                          <<("朋友")<<("家人")<<("同学")
                                         <<("黑名单");
                                friends.setAttribute("count",friendGroup.length());//标记数
                                for(int i=0;i<friendGroup.count();i++){
                                    QDomElement tempEle=doc.createElement(("好友组")+QString("%1").arg(i+1));
                                    QDomText value=doc.createTextNode("none");
                                    tempEle.setAttribute("name",friendGroup.at(i));
                                    tempEle.setAttribute("set","none");
                                    if(friendGroup.at(i)==("我的好友")){
                                        QDomElement meEle=doc.createElement("friend");
                                        meEle.setAttribute("myqq",QString("%1").arg(myqq));
                                        QDomElement nameEle=doc.createElement(("昵称"));
                                        QDomText nameT=doc.createTextNode(name);
                                        nameEle.appendChild(nameT);
                                        QDomElement signatureEle=doc.createElement(("个性签名"));
                                        signatureEle.setAttribute("isNull","true");
                                        QDomText signatureT=doc.createTextNode("none");
                                        signatureEle.appendChild(signatureT);
                                        QDomElement headPathEle=doc.createElement(("头像路径"));
                                        QDomText headPathT=doc.createTextNode(headPath);
                                        headPathEle.appendChild(headPathT);
                                        QDomElement tagEle=doc.createElement(("备注"));
                                        tagEle.setAttribute("isNull","true");
                                        QDomText tagT=doc.createTextNode("none");//默认为none
                                        tagEle.appendChild(tagT);
                                        QDomElement gradeEle=doc.createElement(("等级"));
                                        QDomText gradeT=doc.createTextNode("0");
                                        gradeEle.appendChild(gradeT);
                                        QDomElement statusEle=doc.createElement(("状态"));
                                        QDomText statusT=doc.createTextNode("0");
                                        statusEle.appendChild(statusT);
                                        QDomElement setEle=doc.createElement(("set"));
                                        setEle.setAttribute("isNull","true");
                                        setEle.setAttribute("info","0");
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
                                    }else
                                        tempEle.appendChild(value);
                                    friends.appendChild(tempEle);
                                }
                                QDomElement groups=doc.createElement("groupChat");
                                QDomElement groupChatEle=doc.createElement(("我的群聊"));
                                groupChatEle.setAttribute("isNull","true");
                                QDomText groupChatT=doc.createTextNode("none");
                                groups.appendChild(groupChatEle);
                                groupChatEle.appendChild(groupChatT);
                                root.appendChild(friends);
                                root.appendChild(groups);
                                QTextStream s(&infoFile);
                                doc.save(s,4);
                                infoFile.close();

                                db.close();//记得关闭 好删除文件

                                return sucessful;
                            }
                        }
                    }
                }
            }else{
                qDebug()<<("注册失败！")<<endl;
            }
        }
    }else
        qDebug()<<("sql 语句执行失败！");

    db.close();//记得关闭 好删除文件
    return sucessful;
}

void MyQQRegisterServer::read(QTcpSocket *tcpsock,QThread*t)
{
    //通过信号槽机制，利用tcpsock把处理过程转移到子线程
    connect(tcpsock,&QTcpSocket::readyRead,tcpsock,[=](){
        QString str=QString::fromUtf8(tcpsock->readAll().data());
        QStringList strList=str.split(" ");
        if(strList.length()<3){
            const char* error("-1");
            tcpsock->write(error,sizeof (error));
            tcpsock->disconnectFromHost();
            if(tcpsock->state()!=QAbstractSocket::UnconnectedState) tcpsock->waitForDisconnected();
            t->exit();
            return;
        }else if(strList.at(0)=="0"){//注册
            //整理字符
            QString _1=QString(),_2=QString();
            for(int i=1;i<strList.length()-1;i++){
                _1+=strList.at(i)+" ";
            }
            _1.remove(_1.length()-1,1);//获取昵称
            _2=strList.at(strList.length()-1);//获取passwd
            qint64 myqq=qint64();
            QString cn=QString();
            if(registerMyQQ(_1,_2,myqq,cn)){

                QByteArray rigSuc;
                QDataStream out(&rigSuc,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);
                QString temp,myqqStr;
                myqqStr.setNum(myqq);
                temp=QString("true "+myqqStr);
                out.writeRawData(temp.toUtf8(),temp.length());
                qDebug()<<"rigsuc"<<rigSuc.data();
                tcpsock->write(rigSuc);
                tcpsock->disconnectFromHost();
                if(tcpsock->state()!=QAbstractSocket::UnconnectedState)tcpsock->waitForDisconnected();
            }
            if(QSqlDatabase::contains(cn)){
                {
                    QSqlDatabase::database(cn,false);//作用域内关闭
                }
                QSqlDatabase::removeDatabase(cn);//作用域外移除连接 防止重复连接
            }
        }
        t->exit();
    });
}


void MyQQRegisterServer::incomingConnection(qintptr socketDescriptor)
{
    RegisterThread*thread=new RegisterThread(socketDescriptor,nullptr);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::DirectConnection);
    //这里槽函数启动的是gui线程，可以用Qt::DirectConnection主动设置为子线程
    connect(thread,SIGNAL(hasRunning(QTcpSocket*,QThread*)),this,SLOT(read(QTcpSocket*,QThread*)));
    thread->start();
}
