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
    QStringLiteral("");
}

MyQQRegisterServer::~MyQQRegisterServer()
{
    qDebug()<<"~MyQQRegisterServer()";
}


bool MyQQRegisterServer::registerMyQQ(const QString& name, const QString&passwd, qint64 &myqq)
{
    qDebug()<<name<<passwd;
    QMutex m;
   m.lock();//��count����
    if(count>=100)//������1���ڿ��Դ򿪵����ݿ��� 100
        count=0;
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC",QDateTime::currentDateTime().toMSecsSinceEpoch()+QString("%1").arg(count++));
    m.unlock();//�ͷ�count
    db.setDatabaseName("qtmanager");
    db.setHostName("127.0.0.1");
    db.setUserName("sa");
    db.setPassword("123456x");
    if(db.open())
        qDebug()<<QStringLiteral("�����ݿ�ɹ���");
    else {
        qDebug()<<QStringLiteral("�����ݿ�ʧ�ܣ�");
    }
    QSqlQuery query("use myqq",db);
    query.setForwardOnly(true);
    bool sucessful=false;
    query.prepare("declare @myqq bigint,@sucessful bit=0 "
                  "exec registerMyQQ_pro :name,:passwd, @myqq  output,@sucessful output "
                  "select @myqq,@sucessful ");
    query.bindValue(":name",name);
    query.bindValue(":passwd",passwd);
    if(query.exec()){
            if(query.next()){
                myqq=query.value(0).toLongLong();
                sucessful=query.value(1).toBool();
                if(sucessful){
                    qDebug()<<QStringLiteral("ע��ɹ���qq:")<<myqq<<endl;
                    if(myqq!=0){
                        QDir dir;
                        dir.mkpath(QString("../userData/%1/historyHeadImg").arg(myqq));
                        QString headPath=dir.relativeFilePath(QString("../userData/%1/historyHeadImg").arg(myqq)+"/01.png");
                        QImage img(":/img/init.png");
                        if(img.save(headPath,nullptr,0)){
                            qDebug()<<"IMG is saved��";
                            query.prepare(" update userInfo set headimgpath=? where myqq=? ");
                            query.bindValue(0,QVariant(headPath));
                            query.bindValue(1,QVariant(myqq));
                            if(query.exec()){
                                qDebug()<<"updating the headImgPath of myqq sucessfully,myqq equal to "+QString("%1").arg( myqq);
                                dir.mkpath(QString("../userData/%1/friendsInfo").arg(myqq));
                                dir.mkpath(QString("../userData/%1/groupsInfo").arg(myqq));
                                QFile infoFile(dir.relativeFilePath(QString("../userData/%1/info.xml").arg(myqq)));
                                if(infoFile.open(QIODevice::WriteOnly)){
                                    qDebug()<<"creating info.xml file sucessfully";
                                    QDomDocument doc;
                                    QDomElement root=doc.createElement("myqq");
                                    root.setAttribute("number",myqq);
                                    doc.appendChild(root);
                                    QDomElement set=doc.createElement("set");
                                    QStringList showList;//��ʾ��Ϣ����
                                    showList<<QStringLiteral("ͷ��")<<QStringLiteral("����")<<QStringLiteral("�б�")
                                           <<QStringLiteral("��ϵ��")<<QStringLiteral("������");
                                    for(int i=0;i<showList.count();i++){
                                        QDomElement showEle=doc.createElement(showList.at(i));
                                        QDomText showT= doc.createTextNode("0");
                                        showEle.appendChild(showT);
                                        set.appendChild(showEle);
                                    }
                                    QDomElement passwdEle=doc.createElement(QStringLiteral("��������"));
                                    QDomText passwdT= doc.createTextNode("");
                                    passwdEle.appendChild(passwdT);
                                    set.appendChild(passwdEle);
                                    root.appendChild(set);

                                    QDomElement friends=doc.createElement("friendGroup");
                                    QStringList friendGroup;
                                    friendGroup<<QStringLiteral("�ҵ��豸")<<QStringLiteral("�ҵĺ���")
                                              <<QStringLiteral("����")<<QStringLiteral("����")<<QStringLiteral("ͬѧ")
                                             <<QStringLiteral("������");
                                    for(int i=0;i<friendGroup.count();i++){
                                        QDomElement tempEle=doc.createElement(friendGroup.at(i));
                                        QDomText value=doc.createTextNode("");
                                        tempEle.setAttribute("set","none");
                                        if(friendGroup.at(i)==QStringLiteral("�ҵĺ���")){
                                            QDomElement meEle=doc.createElement("friend");
                                            meEle.setAttribute("myqq",QString("%1").arg(myqq));
                                            QDomElement nameEle=doc.createElement(QStringLiteral("�ǳ�"));
                                            QDomText nameT=doc.createTextNode(name);
                                            nameEle.appendChild(nameT);
                                            QDomElement signatureEle=doc.createElement(QStringLiteral("����ǩ��"));
                                            QDomText signatureT=doc.createTextNode("");
                                            signatureEle.appendChild(signatureT);
                                            QDomElement headPathEle=doc.createElement(QStringLiteral("ͷ��·��"));
                                            QDomText headPathT=doc.createTextNode(headPath);
                                            headPathEle.appendChild(headPathT);
                                            QDomElement tagEle=doc.createElement(QStringLiteral("��ע"));
                                            QDomText tagT=doc.createTextNode(name);//Ĭ��Ϊ�ǳ�
                                            tagEle.appendChild(tagT);
                                            QDomElement gradeEle=doc.createElement(QStringLiteral("�ȼ�"));
                                            QDomText gradeT=doc.createTextNode("0");
                                            gradeEle.appendChild(gradeT);
                                            QDomElement statusEle=doc.createElement(QStringLiteral("״̬"));
                                            QDomText statusT=doc.createTextNode("0");
                                            statusEle.appendChild(statusT);
                                            QDomElement setEle=doc.createElement(QStringLiteral("set"));
                                            setEle.setAttribute("info","0");
                                            setEle.setAttribute("status","0");
                                            QDomText setT=doc.createTextNode("");
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
                                        friends.appendChild(tempEle);
                                        tempEle.appendChild(value);
                                    }
                                    QDomElement groups=doc.createElement("groupChat");
                                    QDomElement groupChatEle=doc.createElement(QStringLiteral("�ҵ�Ⱥ��"));
                                    QDomText groupChatT=doc.createTextNode("");
                                    groups.appendChild(groupChatEle);
                                    groupChatEle.appendChild(groupChatT);
                                    root.appendChild(friends);
                                    root.appendChild(groups);
                                    QTextStream s(&infoFile);
                                    s.setCodec("utf-8");
                                    doc.save(s,4,QDomNode::EncodingFromTextStream);
                                    infoFile.close();
                                    return sucessful;
                                }
                            }
                        }
                    }
                }else{
                    qDebug()<<QStringLiteral("ע��ʧ�ܣ�")<<endl;
                }
            }
    }else
        qDebug()<<QStringLiteral("sql ���ִ��ʧ�ܣ�");
    return sucessful;
}

void MyQQRegisterServer::read(QTcpSocket *tcpsock,QThread*t)
{
    //ͨ���źŲۻ��ƣ�����tcpsock�Ѵ������ת�Ƶ����߳�
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
        }else if(strList.at(0)=="0"){//ע��
            //�����ַ�
            QString _1=QString(),_2=QString();
            for(int i=1;i<strList.length()-1;i++){
                _1+=strList.at(i)+" ";
            }
            _1.remove(_1.length()-1,1);//��ȡ�ǳ�
            _2=strList.at(strList.length()-1);//��ȡpasswd
            qint64 myqq=qint64();
            if(registerMyQQ(_1,_2,myqq)){

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
        }
        t->exit();
    });
}


void MyQQRegisterServer::incomingConnection(qintptr socketDescriptor)
{
    RegisterThread*thread=new RegisterThread(socketDescriptor,nullptr);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::DirectConnection);
    //����ۺ�����������gui�̣߳�������Qt::DirectConnection��������Ϊ���߳�
    connect(thread,SIGNAL(hasRunning(QTcpSocket*,QThread*)),this,SLOT(read(QTcpSocket*,QThread*)));
    thread->start();
}
