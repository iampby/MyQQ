#include "funcc.h"
#include"registersocket.h"
#include"loginsocket.h"
#include"bigfilesocket.h"
#include"headimgwidget.h"
#include"friendmodel.h"
#include"addfriendgroupwidget.h"
#include<qthread.h>
#include<QDebug>
#include<qprocess.h>
#include<qsqlquery.h>
#include <qregularexpression.h>
#include <qdir.h>
#include <qhostaddress.h>
#include<QFileDialog>
#include<QXmlStreamReader>
#include <QQuickItem>
#include <qlabel.h>
#include <QBuffer>
#include<qmessagebox.h>
#include <qjsonarray.h>
//funcc��
FuncC::FuncC(QObject *parent):QObject(parent)
{
    m_win=Q_NULLPTR;registerSock=nullptr;loginSock=nullptr;timer=nullptr;updateTimer=nullptr;server=nullptr;
    addFGWidget=nullptr;
    processCount=0;ip="127.0.0.1";m_sourceIco="";
    loginPort=5567;updatePort=5568;registerPort=5566;
    online=false;
    m_myQQ=QString();m_passwd=QString();//��ʼ��myqq,passwd
    wh=new WeatherHandle;

    m_localCity="";
    m_localUrl="";
    for(int r=0;r<3;r++){
        for(int c=0;c<2;c++)
            _3dayWeaAndTem[r][c]="";
    }

    for(int r=0;r<3;r++){
        for(int c=0;c<2;c++)
            cityNameAboutWeather[r][c]="";
    }

    for(int i=0;i<50;i++){
        cityList[i][0]="";
        cityList[i][1]="";
    }
    cityCount=0;
    initAllCitys();
}

FuncC::~FuncC()
{
    qDebug()<<"~FuncC()";
    if(timer){
        qDebug()<<"network timer can be to delete";
        emit timer->stopMonitor();//�������������źŷ��� �������˳������¼�Ȧ��ɾ������
        delete timer,timer=nullptr;
    }
    if(updateTimer){
        qDebug()<<"updating timer can be to delete";
        emit updateTimer->stopTimer();//�������������źŷ��� �������˳������¼�Ȧ��ɾ������
        delete updateTimer,updateTimer=nullptr;
    }
    if(server){
        emit server->emitExit();//�˳��̣߳�ɾ��
    }
}

QQuickWindow *FuncC::win() const
{
    return  m_win;

}



void FuncC::setWin(QQuickWindow *arg)
{

    qDebug()<<"m_win is viaed";
    m_win=arg;
    if(!m_win){
        qDebug()<<"m_win is null";
        emit winChanged();
        return;
    }
    emit winChanged();
}

void FuncC::setLocalCity(const QString &arg)
{
    if(m_localCity==arg)
        return;
    m_localCity=arg;
    emit localCityChanged();
}

void FuncC::setLocalUrl(const QString &arg)
{
    if(arg==m_localUrl)
        return;
    m_localUrl=arg;
    emit localUrlChanged();
}

void FuncC::setMyQQ(const QString &arg)
{
    m_myQQ=arg;
    emit myQQChanged();
}

void FuncC::setPasswd(const QString &arg)
{
    m_passwd=arg;
    emit passwdChanged();
}



void FuncC::handleProcessStarted()
{
    processCount+=1;
    qDebug()<<"start";
}


void FuncC::registerFinished()
{
    if(registerSock->infoList.at(0)=="true"){
        QString temp=QString("%1").arg(registerSock->infoList.at(1));
        qDebug()<<"register temp"<<temp;
        emit registerResult(true,temp);
    }else
        emit registerResult(false,QString());
    registerSock->close();
}

void FuncC::registerConnectFailed()
{
    qDebug()<<"connecting failed!"<<endl;
    emit registerResult(false,QString());
    registerSock->close();
}



void FuncC::initLoginInfo()
{
    //�û��� name���ǳƣ� sex���Ա� signature������ǩ���� days����Ծ������ grade���ȼ�) status(״̬�� ���ڵ� ����
    QObject* obj=dynamic_cast<QObject*>(m_win);
    QMetaObject::invokeMethod(obj,"runGetUserInfo",Qt::DirectConnection,
                              Q_ARG(QVariant,QVariant::fromValue(userInfo)));
    QFile info("../user/"+m_myQQ+"/info.xml");
    qDebug()<<m_myQQ<<endl;
    if(!info.open(QFile::ReadOnly)){
        qDebug()<<"opened info.xml unsuccessfully";
        return;
    }
    QXmlStreamReader reader;
    reader.setDevice(&info);
    while(!reader.atEnd()){
        reader.readNextStartElement();
        if(reader.name().toString()=="set"){//��ʼ����ʾ������Ϣ
            initSetInfo(reader);
            // qDebug()<<"information of setting had been sended:"<<setInfo;
            QMetaObject::invokeMethod(obj,"runGetSetInfoFunction",Qt::DirectConnection,
                                      Q_ARG(QVariant,QVariant::fromValue(setInfo)));
        }else if(reader.name().toString()=="friendGroup"){//���ݺ�����Ϣ
            initFriendInfo(reader);
        }else if(reader.name().toString()=="groupChat"){//����Ⱥ��Ϣ
            initGroupChatInfo(reader);
        }
    }
    info.close();
    qDebug()<<"initLoginInfo() function running successfully.";
}

void FuncC::initSetInfo(QXmlStreamReader &reader)
{
    while(true){
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement:
            setInfo.insert(reader.name().toString(),reader.readElementText());
            break;
        case QXmlStreamReader::EndElement://����ĩβ����
            if(reader.name().toString()=="set")
                return;
        case QXmlStreamReader::Invalid://�����������Ĵ�������hasError()���
            if(reader.hasError()){
                qDebug()<<"initSetInfo() funciton has a error.";
                return ;
            }
        default:
            break;
        }
    }
}
//QMap<QString,QVector<QVector<QMap<QString,QString>>>>friendInfo
void FuncC::initFriendInfo(QXmlStreamReader &reader)
{
    QString groupName=QString();
    int pos=-1;
    while (true) {
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.name().toString()=="friend"){//��Ӻ�����Ϣ
                parseFriendInfo(reader,groupName,pos);
                groupName=QString();
            }else{
                //����������Qml
                ++pos;
                groupName=reader.name().toString();
                QRegExp reg("^[ ]*$");
                qint32 index= reg.indexIn(groupName);//ȫ�հ�ƥ�� �ͼ�һ���հ� ����xml�սڵ�
                if(index!=-1)groupName=groupName.left(groupName.length()-1);
                qDebug()<<"the group has been sended, named "<<groupName;
                emit getFriendGroup(groupName,reader.attributes().value("set").toString(),pos);
            }
        }else if(reader.isEndElement()){
            if(reader.name().toString()=="friendGroup")
                return;
            else{
                groupName=QString();
            }
        }else if(reader.hasError()){
            qDebug()<<"initFriendInfo() function has a error.";
            return;
        }
    }
}

void FuncC::initGroupChatInfo( QXmlStreamReader &reader)
{
    reader.readNextStartElement();
    QVector<QVector<QMap<QString,QString>>>v;
    //groupChatInfo.insert(reader.name().toString(),v);
}

void FuncC::parseFriendInfo(QXmlStreamReader &reader, QString &endString,int&pos)
{
    while(true){
        if(reader.name().toString()=="friend"&&reader.isStartElement()){
            QVariantMap myqqMap;//QvariantMap����ֱ�Ӵ���
            QString myqq= reader.attributes().value("myqq").toString();
            myqqMap.insert("myqq",myqq);
            while (true) {
                reader.readNext();
                if(reader.isStartElement()){
                    if(reader.name().toString()=="set"){
                        QString info=reader.attributes().value("info").toString();
                        QString status=reader.attributes().value("status").toString();
                        myqqMap.insert("Message-Settin",info);
                        myqqMap.insert("Status-Settin",status);
                    }else if(reader.name().toString()==QStringLiteral("����ǩ��")||reader.name().toString()==QStringLiteral("��ע")){
                        QString text=reader.attributes().value("isNull").toString();
                        if(text=="true") myqqMap.insert(reader.name().toString(),"");
                        else  myqqMap.insert(reader.name().toString(),reader.readElementText());
                    }else{
                        myqqMap.insert(reader.name().toString(),reader.readElementText());
                    }
                }else if(reader.isEndElement()){
                    if(reader.name().toString()=="friend"){
                        QObject* obj=dynamic_cast<QObject*>(m_win);
                        QMetaObject::invokeMethod(obj,"runGetFriendInfoFunction",Qt::DirectConnection,
                                                  Q_ARG(QVariant,QVariant::fromValue(myqqMap)),Q_ARG(int,pos));
                        break;
                    }
                }else if(reader.hasError()){
                    qDebug()<<"parseFriendInfo(const QXmlStreamReader &reader,"
                              " const QString &endString,int pos) function has a error:"<<reader.errorString();
                    return;
                }
            }
        }else if(reader.name().toString()==endString){//������Ӻ�����Ϣ
            break;
        }
        reader.readNext();
    }

}





QString FuncC::sourceIco() const
{
    return m_sourceIco;
}

QString FuncC::localCity() const
{
    return m_localCity;
}

QString FuncC::localUrl() const
{
    return m_localUrl;
}

QString FuncC::myQQ() const
{
    return m_myQQ;
}

QString FuncC::passwd() const
{
    return m_passwd;
}






//��ȡ����������
int FuncC::taskDirection() const
{
    QDesktopWidget*desk=QApplication::desktop();
    QRect availabelRc=desk->availableGeometry();
    QRect rc=desk->geometry();

    if(availabelRc.bottom()!=rc.bottom())
        return 0;
    else if(availabelRc.top()!=rc.top())
        return 1;
    else if(availabelRc.left()!=rc.left())
        return 3;
    else //(availabelRc.right()!=rc.right())
        return 2;

}

int FuncC::taskBarWidth() const
{
    QDesktopWidget*desk=QApplication::desktop();
    int diffHeight=desk->geometry().height()-desk->availableGeometry().height();
    int diffWidth=desk->geometry().width()-desk->availableGeometry().width();
    return diffHeight>diffWidth?diffHeight:diffWidth;
}



void FuncC::registerMyQQ(const QString &MyName, const QString &passwd)
{
    //��Ϊ�ں���ͨ��ʱ��֪��Ϊʲô�����ͷţ����Էŵ���ͷ
    if(registerSock){
        qDebug()<<"delete";
        delete registerSock;
        registerSock=nullptr;
    }
    registerSock=new RegisterSocket(MyName,passwd,this);
    connect(registerSock,SIGNAL(finished()),this,SLOT(registerFinished()));
    connect(registerSock,SIGNAL(errOccured()),this,SLOT(registerConnectFailed()));
    registerSock->connectToHost(ip,registerPort);
}

void FuncC::setPort(const quint16 &port)
{
    this->registerPort=port;
}

void FuncC::setIp(const QString &address)
{
    ip=address;
}

void FuncC::initImageSet(Images *arg)
{
    images=arg;
}



bool FuncC::saveStringToTxt(const QString &str,const QString& title,const QString&dir)
{
    QString r=QFileDialog::getSaveFileName(nullptr,title,dir,"Text (*.txt);;All Files (*.*)");
    if(!r.isEmpty()){
        QFile file(r);
        if(file.open(QIODevice::WriteOnly)){
            if(file.write(str.toUtf8())!=-1){
                return true;
            }
        }
    }
    return false;
}

void FuncC::login(const QString &myqq,const QString &passwd)
{
    //ע�ⷲ�Ƕ�ע��qml���Ը�ֵ,��������qml�����︳ֵʱ�����뷢���ź�����qml�����ı�
    setMyQQ(myqq);
    setPasswd(passwd);
    //�û��� name���ǳƣ� sex���Ա� signature������ǩ���� days����Ծ������ grade���ȼ�) status(״̬�� ���ڵ� ����
    loginSock=new LoginSocket(myqq,passwd,this);
    connect(loginSock,&LoginSocket::finished,[=](int result){
        qDebug()<<"result:"<<result;
        if(result==0){
            userInfo=loginSock->infoObj.toVariantMap();
            userInfo.remove("instruct");
            userInfo.remove("result");
            qint64 ad=userInfo.value("days").toLongLong();//ת��Ϊ��ȫ���ݣ�jsû��64λ����
            userInfo.insert("days",QVariant(QString("%1").arg(ad)));
            QDir d;
            d.mkpath("../user/"+myqq);
            if(writeFile(loginSock->infoXml,"../user/"+myqq+"/info.xml")){
                qDebug()<<"writeFile() retturn true";

                d.mkpath("../user/"+myqq+"/historyHeadImg");
                if(writeImg(loginSock->img,"../user/"+myqq+"/historyHeadImg/01.png","png")){
                    qDebug()<<"writeImg() return true";
                    userInfo.insert("headUrl",d.absoluteFilePath("../user/"+myqq+"/historyHeadImg/01.png"));
                    emit loginResult(result);
                    loginSock->close();
                    if(loginSock->state()!=QAbstractSocket::UnconnectedState)
                        loginSock->waitForDisconnected();
                    // initLoginInfo();
                    /*QFile r("../user/"+myqq+"/info.xml");
                if(!r.remove())
                    qDebug()<<"removed info.xml unsuccessfully.";*/
                    loginSock->deleteLater();//ɾ��ָ��
                    //���߳� ��·״̬��ʱ�ж�
                    online=true;
                    timer=new NetMonitor();
                    timer->setTimerInterval(2000);//2s���
                    connect(timer,&NetMonitor::getNetStatus,timer,[=](qint32&status){
                        //������
                        if(online&&status==-1){
                            qDebug()<<"disconnected network";
                            online=false;
                            //������
                        }else if(!online&&status!=-1){
                            qDebug()<<"connected network";
                            online=true;
                        }
                    });
                    emit timer->startMonitor();
                    //���߳� ��ʱ���½�����Ϣ
                    updateTimer=new UpdateTimer();
                    updateTimer->setMyqq(myqq);
                    updateTimer->setIp(ip);
                    updateTimer->setPort(loginPort);
                    updateTimer->setTimerInterval(100000);//����ӻ�ȡһ�θ���
                    emit updateTimer->startTimer();
                    connect(updateTimer,&UpdateTimer::emitResult,this,&FuncC::updateHandle);

                    return;
                }
            }
            emit loginResult(3);
        }
        emit loginResult(result);
        loginSock->close();
        if(loginSock->state()!=QAbstractSocket::UnconnectedState)
            loginSock->waitForDisconnected();
        loginSock->deleteLater();//ɾ��ָ��
        return;
    });
    connect(loginSock,&LoginSocket::connected,loginSock,[=](){
        this-> newServer();
        connect(server,&NativeServer::findPort,loginSock,[=](quint16 port){
            loginSock->serverPort=port;
            emit loginSock->readyWrite();
        });
    });

    loginSock->connectToHost(ip,loginPort);

}

void FuncC::newServer()
{
    if(server)qDebug()<<"a wild pointer was found,name server";
    server=new NativeServer();
    QThread*thread=new QThread(this);
    server->moveToThread(thread);
    connect(thread,&QThread::started,server,&NativeServer::slotStarted);
    connect(thread,&QThread::finished,server,[=]()mutable{
        server->deleteLater();
        server=nullptr;
    });
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(server,&NativeServer::emitExit,server,[=](){
        qDebug()<<"server thread will be exited";
        thread->exit(0);
        thread->quit();
        server->close();
    });
    //��֤��Ϣ
    connect(server,&NativeServer::emitFverify,this,&FuncC::getFVerify);
    connect(server,&NativeServer::emitGetFriend,this,&FuncC::getFriend);
    thread->start();
}

void FuncC::realseServer()
{
    if(server){
        qDebug()<<"server could be deleted";
        emit server->emitExit();
    }
}
//���½�����Ϣ
void FuncC::updateHandle(const bool &ok)
{
    qDebug()<<"updateHandle(bool &ok)"<<ok;
    if(ok){
        //������º���ͷ��
        if(!updateTimer->historyMap.isEmpty()){
            qDebug()<<"updated a friends image of head ";
            QMap<QString,QPixmap>::const_iterator i=updateTimer->historyMap.cbegin();
            QMap<QString,QPixmap>::const_iterator end=updateTimer->historyMap.cend();
            while(i!=end){
                QString number=i.key();
                QPixmap&newpix= images->provider2->images[number];
                newpix= i.value();//����ֵ
                newpix.save("./x.png");
                qDebug()<<endl<<endl;
                emit updateFriendsModel("image://friends/"+number+"1",FriendModel::ImgPathRole,number);//����һ������ͷ��
                ++i;
            }
            updateTimer->historyMap.clear();
        }
        //����ǩ��
        if(!updateTimer->sigMap.isEmpty()){
            qDebug()<<"updated  friends signature ";
            QMap<QString,QString>::const_iterator i=updateTimer->sigMap.cbegin();
            QMap<QString,QString>::const_iterator end=updateTimer->sigMap.cend();
            while(i!=end){
                QString number=i.key();
                QString sig=i.value();
                emit updateFriendsModel(sig,FriendModel::SignatureRole,number);//����һ������ͷ��
                ++i;
            }
            updateTimer->sigMap.clear();
        }
        //�����ǳ�
        if(!updateTimer->nameMap.isEmpty()){
            qDebug()<<"updated  friends name ";
            QMap<QString,QString>::const_iterator i=updateTimer->nameMap.cbegin();
            QMap<QString,QString>::const_iterator end=updateTimer->nameMap.cend();
            while(i!=end){
                QString number=i.key();
                QString name=i.value();
                emit updateFriendsModel(name,FriendModel::NameRole,number);//����һ�������ǳ�
                ++i;
            }
            updateTimer->nameMap.clear();
        }
    }else{
        if(!updateTimer->historyMap.isEmpty())updateTimer->historyMap.clear();
        if(!updateTimer->sigMap.isEmpty())updateTimer->sigMap.clear();
        if(!updateTimer->nameMap.isEmpty())updateTimer->nameMap.clear();
    }
}

void FuncC::getFVerify(QByteArray data)
{
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonObject obj=doc.object();
    QVariantMap map=obj.toVariantMap();
    if(map.isEmpty()){
        qDebug()<<"a error:final verify data is empty";
        return;
    }
    emit emitFVeify(map);
}

void FuncC::getFriend(QByteArray data, QPixmap pix)
{
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QVariantMap map=doc.object().toVariantMap();
    if(map.isEmpty()){
        qDebug()<<"map is empty";
        return;
    }
    QString number=map.value("number").toString();
    if(number.isEmpty()){
        qDebug()<<"data is not correct because number is empty";
        return;
    }
    pix.save("../user/"+m_myQQ+"/friends"+"/"+number+"1.png","png");
    images->provider2->images.insert(number+"1",pix);
    emit emitFriend(map);

}
bool FuncC::writeFile(const QByteArray &content, const QString &filepath)
{
    QFile file(filepath);

    bool ok=file.open(QFile::WriteOnly);

    if(ok){
        if(file.write(content)==-1){
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    return false;
}

bool FuncC::writeImg(const QByteArray &content, const QString &filepath, const char *format)
{
    QImage img;
    if(!img.loadFromData(content,format))
        return false;
    if(!img.save(filepath,nullptr,0))
        return false;
    return true;
}



void FuncC::startClock()
{
    //  timer->start();
}

void FuncC::stopClock()
{
    //timer->stop();
}


void FuncC::setMyCursor(const int& direct, QWindow *w) const
{
    switch (direct) {
    case 1:
        w->setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case 2:
        w->setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case 3:
        w->setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case 4:
        w->setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case 5:
        w->setCursor(QCursor(Qt::ArrowCursor));
        break;
    case 6:
        w->setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case 7:
        w->setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case 8:
        w->setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case 9:
        w->setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    }

}

void FuncC::openTempMesWin() const
{

    QDialog*tipWin=new QDialog();
    tipWin->setFixedSize(510,415);
    tipWin->setWindowIcon(QIcon(":/images/mainInterface/qqWhite.png"));
    tipWin->setWindowTitle("MyQQ");
    tipWin->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);//������֮��
    QLabel*lab=new QLabel(tipWin);
    lab->move(180,215);
    lab->setText(QStringLiteral("��ϲ�㣡�Ѿ������°汾"));
    tipWin->setToolTip(QStringLiteral("��ϲ�㣡�Ѿ������°汾"));
    tipWin->setAttribute(Qt::WA_QuitOnClose,false);//�ر�Ĭ�϶��㴰�ڹر��˳���Ϊ
    tipWin->setAttribute(Qt::WA_DeleteOnClose,true);//�����ɾ
    tipWin->show();
}

void FuncC::addHeadWidget(QWindow *w,const int&x,const int&y,QPixmap pixmap,const QString&myqq,const bool isgot)const
{
    HeadImgWidget*widget=new HeadImgWidget;
    widget->setWindowFlag(Qt::FramelessWindowHint,true);//����ȥ��������������parent�����Զ�ȥ������
    widget->setAttribute(Qt::WA_QuitOnClose,false);
    widget->winId();
    QWindow*temp=widget->windowHandle();
    if(temp){
        temp->setParent(w);
        QDir dir("../user/"+myqq+"/historyHeadImg/");
        if(!dir.exists()||!isgot){
            qDebug()<< dir.mkpath("")<<"create dir result";
            getHistoryHeadImg(myqq);
            connect(this,&FuncC::emitReadHistory,widget,[=](){
                qDebug()<<"received emitReadHistroy";
                QMetaObject::invokeMethod(w,"midFunc",Qt::QueuedConnection) ;
            });//��ȡͷ��
        }

        widget->setHeadImg(pixmap);//��ӵ�ǰͷ��
        temp->setGeometry(x,y,widget->width(),widget->height());
        connect(this,&FuncC::emitHeadImgOpenFile,widget,&HeadImgWidget::openFile);//���ļ�
        connect(this,&FuncC::emitHeadImgOKClicked,widget,[=](Images*images){
            widget->okClicked(images,myqq);
        });//ok����
        //ˢ�º���ģ��idֵ
        connect(widget,&HeadImgWidget::updateMyself,this,[=](const QString&number){
            qDebug()<<"emit emitUpdateFriendsModel(id)";
            emit updateFriendsModel("image://friends/"+number+"1",FriendModel::ImgPathRole,number);
        });
        //����Զ��ͷ��
        connect(widget,&HeadImgWidget::updateRemoteHeadImg,this,[=](const QPixmap&pix){
            qDebug()<<"updateRemoteHeadImg signal had sent";
            disconnect(widget,SIGNAL(updateRemoteHeadImg(QPixmap)));//�ȶϿ����ӣ����ͨ��ֻ����һ��

            QString instructDescription="4 historyHeadImg "+myqq+" writedHeaderSize";
            BigFileSocket*updateImgSock=new BigFileSocket();//���̲߳����и���
            updateImgSock->setInstruct(instructDescription);
            updateImgSock->setIp(ip);
            updateImgSock->setPort(updatePort);
            updateImgSock->setTimeout(20000);//20s��ʱ
            QThread*thread=new QThread();
            updateImgSock->moveToThread(thread);
            thread->start();
            emit updateImgSock->start();//ת�Ƶ����߳�ȥpost host
            connect(thread,&QThread::finished,updateImgSock,&BigFileSocket::deleteLater);
            connect(updateImgSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
            //ɾ���߳�
            connect(this,&FuncC::emitHeadImgOKClicked,thread,[=](){
                qDebug()<<"closed window causes a thread to exit";
                thread->exit(0);
                thread->quit();
            });
            connect(thread,&QThread::finished,thread,&QThread::deleteLater);
            //��ȡ�ļ������β����
            connect(updateImgSock,&BigFileSocket::writtenInstruction, updateImgSock,[=](){
                qDebug()<<"writtenInstruction";
                QBuffer buffer;
                buffer.open(QIODevice::WriteOnly);
                pix.save(&buffer,"png");
                QByteArray pixArray;
                pixArray.append(buffer.data());
                buffer.close();
                if(pixArray.isEmpty()){
                    qDebug()<<"warning:data of cover-image is empty";
                    thread->exit(0);
                    thread->quit();
                    return;
                }
                updateImgSock->write(pixArray);
                updateImgSock->loop.exec();
                thread->exit(0);
                thread->quit();
                qDebug()<<"updating headimg thread had exited";
            });

        });

        //�ر���ʷͷ���ǩ
        connect(widget,&HeadImgWidget::getFocus,[=](){
            qDebug()<<"QMetaObject::invokeMethod(w,\"alterSelectedIndex\",Qt::QueuedConnection)";
            QMetaObject::invokeMethod(w,"alterSelectedIndex",Qt::QueuedConnection) ;
        });
        //���ѡ�е�ͷ��
        connect(this,&FuncC::emitSelectedImg,widget,[=]( QPixmap pix){
            qDebug()<<"handle selected img ";
            widget->setHeadImg(pix);
        });
        connect(this,&FuncC::emitCloseHead,widget,[=](){
            widget->deleteLater();//�ӳ�ɾ��
        });
        widget->setHeadImg(pixmap);
        temp->show();
    }else{
        qDebug()<<"widget is null";
        widget->deleteLater();
    }
}

void FuncC::openFile( QString filename)
{
    qDebug()<<"emitopenfile";

    emit emitHeadImgOpenFile(filename.replace("file:///","",Qt::CaseInsensitive));
}

void FuncC::closeWidget()
{
    emit emitCloseHead();//�ر��ź�
}




void FuncC::getHistoryHeadImg(const QString&myqq)const
{
    QString instructDescription="4 historyHeadImg "+myqq;
    BigFileSocket*historyImgSock=new BigFileSocket();//���̲߳����и���
    historyImgSock->setInstruct(instructDescription);
    historyImgSock->setIp(ip);
    historyImgSock->setPort(loginPort);
    QThread*thread=new QThread();
    historyImgSock->moveToThread(thread);
    thread->start();
    emit historyImgSock->start();//ת�Ƶ����߳�ȥpost host
    connect(thread,&QThread::finished,historyImgSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //��ȡ�ļ������β����
    connect( historyImgSock,&BigFileSocket::finished, historyImgSock,[=](int code){
        qDebug()<<"code"<<code;
        thread->exit(0);
        thread->quit();
        emit emitReadHistory(code);
    });
}

void FuncC::updateSignature(QString signature, const QString &in)
{
    qDebug()<<"updating remote signature";
    BigFileSocket*updateSigSock=new BigFileSocket();//���̲߳����и���
    updateSigSock->setInstruct(in);
    updateSigSock->setIp(ip);
    updateSigSock->setPort(updatePort);
    updateSigSock->setTimeout(10000);//10s��ʱ
    QThread*thread=new QThread();
    updateSigSock->moveToThread(thread);
    thread->start();
    emit updateSigSock->start();//ת�Ƶ����߳�ȥpost host
    connect(thread,&QThread::finished,updateSigSock,&BigFileSocket::deleteLater);
    connect(updateSigSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //��ȡ�ļ������β����
    connect(updateSigSock,&BigFileSocket::writtenInstruction, updateSigSock,[=](){

        if(!signature.isEmpty()){
            updateSigSock->write(signature.toUtf8());
            updateSigSock->loop.exec();
        }
        thread->exit(0);
        thread->quit();
        qDebug()<<"updating signature thread had exited";
    });
}

void FuncC::updateCover(QString qmlFilePath)
{
    QString instructDescription="7 updateCover "+m_myQQ+" writedHeaderSize";//���·���
    BigFileSocket*updateCoverSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    updateCoverSock->setInstruct(instructDescription);
    updateCoverSock->setIp(ip);
    updateCoverSock->setPort(updatePort);
    updateCoverSock->setTimeout(20000);//20s��ʱ
    QThread*thread=new QThread();
    updateCoverSock->moveToThread(thread);
    thread->start();
    emit updateCoverSock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,updateCoverSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateCoverSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(updateCoverSock,&BigFileSocket::writtenInstruction, updateCoverSock,[=](){
        qDebug()<<"writtenInstruction"<<qmlFilePath;
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QPixmap pix;
        pix.load(qmlFilePath);
        if(pix.isNull()){
            qDebug()<<"warning:cover-image is null";
        }
        pix.save(&buffer,"png");
        QByteArray pixArray;
        pixArray.append(buffer.data());
        qDebug()<<pix.size();
        buffer.close();//�ر�io�豸
        if(pixArray.isEmpty()){
            qDebug()<<"warning:data of cover-image is empty";
            thread->exit(0);
            thread->quit();
            return;
        }
        updateCoverSock->write(pixArray);
        updateCoverSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<"updating conver thread had exited";
    });
}

void FuncC::deleteNetAndUpdateTimer()
{
    if(timer){
        qDebug()<<"network timer can be to delete";
        emit timer->stopMonitor();//�������������źŷ��� �������˳������¼�Ȧ��ɾ������
        delete timer,timer=nullptr;
    }
    if(updateTimer){
        qDebug()<<"updating timer can be to delete";
        emit updateTimer->stopTimer();//�������������źŷ��� �������˳������¼�Ȧ��ɾ������
        delete updateTimer,updateTimer=nullptr;
    }
}
//���ͷ��������浽qml���ķ������
void FuncC::addCoverWidget(QWindow *w, const int &x, const int &y, QString filePath) const
{
    filePath.replace("file:///","");//qml·������
    qDebug()<<filePath;
    QPixmap pix;
    pix.load(filePath);
    if(pix.isNull()){
        qDebug()<<"warning:cover-image is null";
    }
    HeadImgWidget*widget=new HeadImgWidget(nullptr,false);//�ر�����
    widget->setWindowFlag(Qt::FramelessWindowHint,true);//����ȥ��������������parent�����Զ�ȥ������
    widget->setAttribute(Qt::WA_QuitOnClose,false);
    widget->winId();
    QWindow*temp=widget->windowHandle();
    //����ɹ�
    if(temp){
        temp->setParent(w);//Ƕ��
        temp->setGeometry(x,y,temp->width(),temp->height());//��λ
        connect(this,&FuncC::emitCloseCover,widget,&HeadImgWidget::deleteLater);//ɾ��widget��Դ
        connect(this,&FuncC::emitCoverOKClicked,widget,&HeadImgWidget::okCoverClicked);// ����һ�������ļ�����
        widget->show();//����show
        //ע�⣺��֪��ԭ���¸��ķ��棨Ҳ���������֣���ͼ���������Ǵ�100���ң�������ͨ���۲�����ľ���ԭ��֪��
        //ԭ�� ������show������ͼƬ
        widget->openFile(filePath);//����ͼƬ
    }else{
        qDebug()<<"widget is null";
        widget->deleteLater();
    }
}

void FuncC::closeCoverWidget()
{
    emit emitCloseCover();//�ر��ź�
}
//��ȡԶ�̸�������
void FuncC::getIndividualData()
{
    qDebug()<<"getIndividualData()";
    QString instructDescription="8 getPersonalData "+m_myQQ;//���·���
    BigFileSocket*getPersonalDataSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    getPersonalDataSock->setInstruct(instructDescription);
    getPersonalDataSock->setIp(ip);
    getPersonalDataSock->setPort(loginPort);
    getPersonalDataSock->setTimeout(50000);//��ʱ50s
    QThread*thread=new QThread();
    getPersonalDataSock->moveToThread(thread);
    thread->start();
    emit getPersonalDataSock->start();//ת�Ƶ����߳�ȥpost host
    connect(getPersonalDataSock,&BigFileSocket::finished,getPersonalDataSock,[=](){
        qDebug()<<"personal data is got or timeout";
        QVariantMap jsonObj=(QJsonDocument::fromJson(getPersonalDataSock->carrier)).object().toVariantMap();
        if(!jsonObj.isEmpty()){
            qDebug()<<"the most personal  json-data would be dispatched ";
            emit emitPersonalJsonInfo(jsonObj);
        }else{
            qDebug()<<"the most personal  json-data is not got ";
        }
        QDir dir;
        dir.mkpath("../user/"+m_myQQ+"/photowall");
        if(!getPersonalDataSock->img.isEmpty()){
            qDebug()<<"the user's cover and photowall would be dispatched ";
            QMap<QString,QByteArray>::const_iterator i=getPersonalDataSock->img.cbegin();
            QMap<QString,QByteArray>::const_iterator end=getPersonalDataSock->img.cend();
            QVector<QString>names;
            QString cover;
            while (i!=end) {
                QString name=i.key();
                qDebug()<<"a file name is "<<name<<i.value().size();
                QPixmap pix;
                pix.loadFromData(i.value());
                //ע�Ᵽ��ʱָ����ʽ
                if(name!="cover"){
                    if(!pix.save("../user/"+m_myQQ+"/photowall/"+name,"png"))
                        qDebug()<<"warning:file is not save,named "<<name;
                    else {
                        names.append(name);
                    }
                }else{
                    if(!pix.save("../user/"+m_myQQ+"/"+name,"png")){
                        qDebug()<<"warning:file is not save,named "<<name;
                    }else
                        cover=name;
                }
                ++i;
            }
            emit emitPersonalCoverAndPhoto(names,cover);
        }else{
            qDebug()<<"the user's cover and photowall would be not set";
        }
        //��ʱ��������ɽ����߳�
        thread->exit(0);
        thread->quit();
        getPersonalDataSock->deleteLater();
        thread->deleteLater();
    });
}
//�����û�Զ����Ƭǽ
void FuncC::updatePhotoWall(quint8 length)
{
    QString instructDescription="9 updatePhotoWall "+m_myQQ+" writedHeaderSize";//���·���
    BigFileSocket*updateWallSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    updateWallSock->setInstruct(instructDescription);
    updateWallSock->setIp(ip);
    updateWallSock->setPort(updatePort);
    updateWallSock->setTimeout(20000);//20s��ʱ
    QThread*thread=new QThread();
    updateWallSock->moveToThread(thread);
    thread->start();
    emit updateWallSock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,updateWallSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateWallSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(updateWallSock,&BigFileSocket::writtenInstruction, updateWallSock,[=]()mutable{
        qDebug()<<"writtenInstruction";
        //���ڷ���˵�updateportֻ��һ������json����һ�����ݡ��������ݵĹ���,�����ͼƬ����ȫ���������
        //���ݽṹ length(quint8) �������ݵ����ݳ���(quint32) ��������
        QByteArray totalData;//ȫ������
        QByteArray lengthData;
        QDataStream stream(&lengthData,QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_4_0);
        stream<<length;
        totalData.append(lengthData);//��¼����
        qDebug()<<"length="<<length;
        QVector<quint32>sizeVector;//��¼�������ݴ�С
        QVector<QByteArray>dataVector;//��¼ͼƬ����
        for (quint8 var = 0; var < length; ++var) {
            QPixmap temp=images->provider3->images.value(QString("%1").arg(var));//��ȡ������ӵ�����
            if(temp.isNull())qDebug()<<"provider3 not found a pixmap";
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);

            if(!temp.save(&buffer,"png")){
                qDebug()<<"warning:a pixmap is not dispatched because readed data is of failure";
                buffer.close();//�ر�io�豸
                continue;
            }
            QByteArray  pixdata=buffer.data();
            if(pixdata.isEmpty())qDebug()<<"warning:a empty pixmap is dispatched";
            sizeVector.append(pixdata.size());
            dataVector.append(pixdata);
            buffer.close();//�ر�io�豸
        }
        length=sizeVector.size();
        for (quint8 var = 0; var < length; ++var) {
            QByteArray data;
            QDataStream temp(&data,QIODevice::WriteOnly);
            temp.setVersion(QDataStream::Qt_4_0);
            temp<<sizeVector.at(var);
            qDebug()<<"data size="<<data.data()<<sizeVector.at(var);
            totalData.append(data);//��¼����
        }
        length=dataVector.size();
        //��������
        for (quint8 var = 0; var < length; ++var) {
            totalData.append(dataVector[var]);
            qDebug()<<"size?"<<dataVector[var].size();
        }
        updateWallSock->write(totalData);
        if(!totalData.isEmpty())
            updateWallSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<"updating photo wall thread had exited";
    });
}

void FuncC::inintCityData(QQuickWindow*w)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","city_data");
    db.setDatabaseName("../city.db");
    db.setHostName("MyQQ");
    db.setUserName("sa");
    db.setPassword("@123456x");
    if(db.open())
        qDebug()<<QStringLiteral("�����ݿ�ɹ���");
    else {
        qDebug()<<QStringLiteral("�����ݿ�ʧ�ܣ�");
        return;
    }
    QSqlQuery query2(db);
    qDebug()<<"open:"<<db.connectionName()<<db.databaseName();
    QVariantList countryList,provinceList,cityList,countyList;//˳�򴢴�id name fid
    if(!query2.exec(" begin transaction ")){
        qDebug()<<"warning:begin transaction is of failure";
    }
    if(!query2.exec(" select*from country ")){
        qDebug()<<"warning:city.db is not found";
    }
    query2.next();
    while (query2.isValid()) {
        qint32 id=query2.value("id").toLongLong();
        QString name=query2.value("name").toString();
        if(name.isEmpty()){
            query2.next();
            continue;
        }
        countryList.append(id);
        countryList.append(name);
        query2.next();
    }
    if(!query2.exec(" select*from province ")){
        qDebug()<<"warning:province table is not found";
    }
    query2.next();
    while (query2.isValid()) {
        qint32 id=query2.value("id").toLongLong();
        QString name=query2.value("name").toString();
        qint32 fid=query2.value("belongCountryId").toLongLong();
        if(name.isEmpty()){
            query2.next();
            continue;
        }
        provinceList.append(id);
        provinceList.append(name);
        provinceList.append(fid);
        query2.next();
    }
    if(!query2.exec(" select*from city ")){
        qDebug()<<"warning:city table is not found";
    }
    query2.next();
    while (query2.isValid()) {
        qint32 id=query2.value("id").toLongLong();
        QString name=query2.value("name").toString();
        qint32 fid=query2.value("belongProvinceId").toLongLong();
        if(name.isEmpty()){
            query2.next();
            continue;
        }
        cityList.append(id);
        cityList.append(name);
        cityList.append(fid);
        query2.next();
    }
    if(!query2.exec(" select*from county ")){
        qDebug()<<"warning:county table is not found";
    }
    query2.next();
    while (query2.isValid()) {
        qint32 id=query2.value("id").toLongLong();
        QString name=query2.value("name").toString();
        qint32 fid=query2.value("belongCityId").toLongLong();
        if(name.isEmpty()){
            query2.next();
            continue;
        }
        countyList.append(id);
        countyList.append(name);
        countyList.append(fid);
        query2.next();
    }
    if(!query2.exec(" end transaction ")){
        qDebug()<<"warning:end transaction is of failure";
    }
    QMetaObject::invokeMethod(w," addCountryData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(countryList)));
    QMetaObject::invokeMethod(w," addProvinceData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(provinceList)));
    QMetaObject::invokeMethod(w," addCityData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(cityList)));
    QMetaObject::invokeMethod(w," addCountyData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(countyList)));
}

void FuncC::updateUserInformation(QVariantMap info)
{
    qDebug()<<"updateUserInformation(QVariantMap info)  called";
    QString instructDescription="10 updateUserInformation "+m_myQQ+" writedHeaderSize";//���·���
    BigFileSocket*updateUserInfoSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    updateUserInfoSock->setInstruct(instructDescription);
    updateUserInfoSock->setIp(ip);
    updateUserInfoSock->setPort(updatePort);
    updateUserInfoSock->setTimeout(30000);//30s��ʱ
    QThread*thread=new QThread();
    updateUserInfoSock->moveToThread(thread);
    thread->start();
    emit updateUserInfoSock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,updateUserInfoSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateUserInfoSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(updateUserInfoSock,&BigFileSocket::writtenInstruction, updateUserInfoSock,[=](){
        qDebug()<<"writtenInstruction finished";
        QJsonDocument json(QJsonObject::fromVariantMap(info));
        QByteArray data=json.toBinaryData();

        if(data.isEmpty()){
            thread->exit(0);
            thread->quit();
            qDebug()<<"updating failed for remote user information and lived thread had exited";
            return;
        }
        updateUserInfoSock->write(data);
        updateUserInfoSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<"the thread had exited to update remote user information";
    });
}

void FuncC::addRemoteFriendGroup(QJsonDocument&doc)
{
    QString instructDescription="11 addFGroup "+m_myQQ+" writedHeaderSize";//��ӷ���
    BigFileSocket*addFGroupSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    addFGroupSock->setInstruct(instructDescription);
    addFGroupSock->setIp(ip);
    addFGroupSock->setPort(updatePort);
    addFGroupSock->setTimeout(10000);//10s��ʱ
    QThread*thread=new QThread();
    addFGroupSock->moveToThread(thread);
    thread->start();
    emit  addFGroupSock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,addFGroupSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(addFGroupSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(addFGroupSock,&BigFileSocket::writtenInstruction,addFGroupSock,[=](){
        QByteArray temp=doc.toBinaryData();
        if(temp.isEmpty()){
            qDebug()<<"added a remote group is empty";
            thread->exit(0);
            thread->quit();
            return;
        }
        addFGroupSock->write(temp);
        addFGroupSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<" thread of added a remote group had exited";
    });
}

void FuncC::exitMyQQ(QQuickWindow*w)
{
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("-1"));
    obj.insert("myqq",QJsonValue(m_myQQ));
    doc.setObject(obj);
    QTcpSocket*exitSock=new QTcpSocket(this);
    QEventLoop* loop=new QEventLoop(exitSock);
    connect(exitSock,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);//���д��
    connect(exitSock,&QTcpSocket::connected,exitSock,[=](){
        qDebug()<<" ready to write exit status to remote host";
        QByteArray data=doc.toBinaryData();
        if(data.isEmpty()){
            qDebug()<<"json data is empty while exiting progarm";
        }else{
            QByteArray size;
            QDataStream stem(&size,QIODevice::WriteOnly);
            quint8 l=data.size();
            stem.setVersion(QDataStream::Qt_4_0);
            stem<<l;
            exitSock->write(size+data);
            QTimer::singleShot(1500,exitSock,[=](){
                qDebug()<<"json data was blocked 2s while exiting program,then gave up process";
                loop->quit();
            });
            qDebug()<<"a loop exec";
            loop->exec();
            qDebug()<<"a loop exit";
        }
        exitSock->close();
        exitSock->deleteLater();
        if(w){
            QMetaObject::invokeMethod((QObject*)w,"aboutToQuit",Qt::DirectConnection);//�˳�����
        }
    });
    connect(exitSock,QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),exitSock,[=](QAbstractSocket::SocketError error){
        qDebug()<<"error:a error occured while readying to write exit status to remote host\n\rerror description:";
        switch (error) {
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
        exitSock->deleteLater();
        if(w)
            QMetaObject::invokeMethod((QObject*)w,"aboutToQuit",Qt::DirectConnection);
    });
    exitSock->connectToHost(ip,updatePort);
    exitSock->waitForConnected(1500);//�ȴ�1.5�� ����3sɾ���׽���
}

void FuncC::getVerifyArray(const QString &myqq,QQuickWindow *qmlWin)
{
    qDebug()<<"getVerifyArray";
    QString instructDescription="13  getVerifyArray "+myqq;//��ȡԶ����֤��Ϣ�б�
    BigFileSocket*getVerifyArrSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    getVerifyArrSock->setInstruct(instructDescription);
    getVerifyArrSock->setIp(ip);
    getVerifyArrSock->setPort(loginPort);
    getVerifyArrSock->setTimeout(15000);//��ʱ15s
    QThread*thread=new QThread(this);
    getVerifyArrSock->moveToThread(thread);
    thread->start();
    emit getVerifyArrSock->start();//ת�Ƶ����߳�ȥpost host
    connect(getVerifyArrSock,&BigFileSocket::finished,getVerifyArrSock,[=](){
        qDebug()<<"verify data is got or timeout";
        QJsonDocument json=QJsonDocument::fromJson(getVerifyArrSock->carrier);
        QVariantMap map=json.object().toVariantMap();
        qDebug()<<map;
        QMetaObject::invokeMethod(qmlWin,"getFVerify",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(map)));
        //��ʱ��������ɽ����߳�
        thread->exit(0);
        thread->quit();
        getVerifyArrSock->deleteLater();
        thread->deleteLater();
    });

}

void FuncC::openAddFGroupWidget(QQuickWindow*w,QQuickWindow *qqMainWin)
{
    if(nullptr!=addFGWidget){
        qDebug()<<"addFGWidget is not null";
        qint32 offx,offy;
        offx=w->x()+ (w->width()-addFGWidget->width())/2;
        offy=w->y()+ (w->height()-addFGWidget->height())/2;
        addFGWidget->move(offx,offy);
        addFGWidget->show();
        addFGWidget->raise();
        addFGWidget->activateWindow();
        return;
    }

    addFGWidget =new AddFriendGroupWidget();
    addFGWidget->show();
    addFGWidget->raise();
    addFGWidget->activateWindow();
    qint32 offx,offy;
    offx=w->x()+ (w->width()-addFGWidget->width())/2;
    offy=w->y()+ (w->height()-addFGWidget->height())/2;
    addFGWidget->move(offx,offy);
    connect(this,&FuncC::emitCloseAddFGroup,addFGWidget,[=]()mutable{
        delete addFGWidget,addFGWidget=nullptr;//��������ɾ��
    });
    connect(addFGWidget,&AddFriendGroupWidget::emitGroup,this,[=](QString name){
        QMetaObject::invokeMethod((QObject*)qqMainWin,"addFGroup",Qt::DirectConnection,Q_ARG(QString,name));
        QMetaObject::invokeMethod((QObject*)w,"addFGroup",Qt::DirectConnection,Q_ARG(QString,name));
        qDebug()<<"updating a remote group";
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("instruct",QJsonValue("add group"));//ָ�� ����Ҫ���·���
        obj.insert("groupName",QJsonValue(name));
        doc.setObject(obj);
        addRemoteFriendGroup(doc);
    });
    //deletelater������
    connect(addFGWidget,&AddFriendGroupWidget::setNull,addFGWidget,[=]()mutable{
        addFGWidget=nullptr;
    });
}

void FuncC::handleFVerify(QVariantMap obj)
{
    obj.insert("instruct",QJsonValue("14"));
    obj.insert("content",QJsonValue("FVerify"));
    BigFileSocket*fVerifySock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    QJsonObject instruct=QJsonObject::fromVariantMap(obj);
    fVerifySock->setInstruct(instruct);
    fVerifySock->setIp(ip);
    fVerifySock->setPort(loginPort);
    fVerifySock->setTimeout(20000);//20s��ʱ
    QThread*thread=new QThread();
    fVerifySock->moveToThread(thread);
    thread->start();
    emit fVerifySock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,fVerifySock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(fVerifySock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(fVerifySock,&BigFileSocket::writtenInstruction, fVerifySock,[=](){
        thread->exit(0);
        thread->quit();
       qDebug()<<"handling friend verify thread had exited";
    });
}

void FuncC::updateFGroup(QVariantMap obj)
{
    QString instructDescription="15 "+obj.take("content").toString()+" "+m_myQQ+" writedHeaderSize";//��ӷ���
    BigFileSocket*updateFGroupSock=new BigFileSocket();//���̶߳�����ò�Ҫ�и���
    updateFGroupSock->setInstruct(instructDescription);
    updateFGroupSock->setIp(ip);
    updateFGroupSock->setPort(updatePort);
    updateFGroupSock->setTimeout(10000);//10s��ʱ
    QThread*thread=new QThread();
    updateFGroupSock->moveToThread(thread);
    thread->start();
    emit  updateFGroupSock->start();//ת�Ƶ����߳�ȥpost host
    //ɾ���׽���
    connect(thread,&QThread::finished,updateFGroupSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateFGroupSock,&BigFileSocket::finished,thread,&QThread::quit);//��ʱ����
    //��ȡ�ļ������β����
    connect(updateFGroupSock,&BigFileSocket::writtenInstruction,updateFGroupSock,[=](){
       QJsonObject jobj=QJsonObject::fromVariantMap(obj);
       QJsonDocument json(jobj);
       QByteArray data=json.toBinaryData();
       if(data.isEmpty()){
        qDebug()<<"a error:data of dispatching is empty ";
                  thread->exit(0);
                  thread->quit();
                  return;
       }
        updateFGroupSock->write(data);
        updateFGroupSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<" thread had exited to move or delete remote group";
    });
}


void FuncC::startAddFriendsProcess(QQuickWindow*arg,QMap<QString, QVariant>obj,QList<QVariant>arr)
{

    qDebug()<<"add friends win process started->";
    QObject*parent=dynamic_cast<QObject*>(arg);

    QProcess *myProcess = new QProcess(parent);
    connect(myProcess,&QProcess::readyReadStandardOutput,myProcess,[=](){
        QByteArray data=myProcess->readAllStandardOutput();
        QJsonParseError err;
        data=QString::fromLocal8Bit(data).toUtf8();
        QJsonDocument tempDoc=QJsonDocument::fromJson(data,&err);
        if(err.error!=QJsonParseError::NoError){
            qDebug()<<"error description:"<<err.errorString();
            qDebug()<<"a error:parse json from  myprocess is of failure,then closing myprocess";
            myProcess->kill();
            myProcess->waitForFinished();
            return;
        }
        if(tempDoc.isObject()){
            QJsonObject obj=tempDoc.object();
            qDebug()<<"get a json from myprocess:"<<obj;
            QString instruct=obj.value("instruct").toString();
            if(instruct=="add group"){
                QString group=obj.value("groupName").toString();
                QMetaObject::invokeMethod(arg,"addFGroup",Qt::DirectConnection,Q_ARG(QString,group));
                addRemoteFriendGroup(tempDoc);
            }
        }
    });

    connect(myProcess,&QProcess::readyReadStandardError,myProcess,[=](){
        QByteArray data=myProcess->readAllStandardError();;
        qDebug()<<"stderr:"<<data;
    });

    connect(myProcess,SIGNAL(started()),this,SLOT(handleProcessStarted()));
    //qqmainwin�رվ���ֹ����
    connect(this,&FuncC::emitCloseMyProcess,myProcess,[=](){
        qDebug()<<"kill  myprocess";
        myProcess->kill();
        myProcess->waitForFinished();
    });
    //finished�źŴ�����
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),myProcess,
            [=](int exitCode, QProcess::ExitStatus exitStatus)mutable{
        qDebug()<<"deletion myprocess";
        processCount-=1;
        addSMm.detach();
        if(myProcess)
            delete myProcess,myProcess=nullptr;
    });
    if(addFriendsProcessCount()==0){
        addSMm.setKey("addFriendgroups"+m_myQQ);//��Ӻ����ã�һ�������飬�͸����ڴ� keyΨһ��ʶ ����Ψһ
        qDebug()<<"myprocess key is "<<addSMm.key();
        addSMm.create(1024);
        if(!addSMm.isAttached()){
            qDebug()<<"warning:a unexpected action addSMm is not attached";
            return;
        }
        QJsonDocument sdoc(QJsonArray::fromVariantList(arr));
        QByteArray sdata=sdoc.toJson();
        qDebug()<<sdata.data();
        memccpy(addSMm.data(),sdata.data(),'\0',1024);//���ݲ�����1024�ֽ� ���15������
        QJsonDocument doc(QJsonObject::fromVariantMap(obj));
        QStringList list;
        list<<doc.toJson();
        myProcess->start( QStringLiteral("../MyQQExternal/addFriendsWin"),list);
    }
}

unsigned short FuncC::addFriendsProcessCount() const
{
    return processCount;
}

void FuncC::analysisWh(QString totalGeoAddr)
{
    //��Ϊ��ַ���У�ֻ��ǿ��ֵ
    totalGeoAddr=QStringLiteral("�й� �����г�����");
    int s1=0,s2=0,s3=0;
    QString cityN="";
    for(int i=0;i<totalGeoAddr.size();i++){
        QString s=totalGeoAddr.data()[i];
        if(s==QStringLiteral("ʡ")||s==QStringLiteral("��")||s==QStringLiteral("��")||
                s==QStringLiteral("��")||s==QStringLiteral("��"))
            if(s1==0){
                s1=i;
            }
            else if(s2==0)
                s2=i;
            else s3=i;
    }
    if(s1!=0){
        cityN="";
        for(int i=s1-1;i>0;i--){
            QString temp=cityN;
            cityN= totalGeoAddr.data()[i];
            QString inverse=cityN+temp;
            if(!allCitys.value(inverse).isEmpty()){
                m_localCity=inverse;
                goto label;
            }
        }
        if(s2!=0){
            cityN="";
            for(int i=s1+1;i<s2;i++){
                cityN+=totalGeoAddr.data()[i];
                if(!allCitys.value(cityN).isEmpty()){
                    m_localCity=cityN;
                    goto label;
                }
            }

            if(s3!=0){
                cityN="";
                for(int i=s2+1;i<s3;i++){
                    cityN+=totalGeoAddr.data()[i];
                    if(!allCitys.value(cityN).isEmpty()){
                        m_localCity=cityN;
                        break;
                    }
                }

            }
        }
    }
label:
    m_localUrl=allCitys.value(m_localCity);
    qDebug()<<m_localCity<<m_localUrl;
    emit finished();
}

void FuncC::crawWeatherUrl(const QString &url)
{
    QNetworkAccessManager*manager=new QNetworkAccessManager();
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");


    QNetworkReply*reply=manager->get(request);

    connect(manager,&QNetworkAccessManager::finished,this,[=](){
        QByteArray data;
        data.resize(1024);
        data=reply->readLine(1024);
        int count=0;
        while (!data.isEmpty()) {
            //qDebug()<<QString::fromUtf8(data.data());
            QRegularExpression tar("^(?:<p title=\".+\" class=\".+\">)(.+)(?:</p>)$");
            QRegularExpression firtar("(?:<input type=\"hidden\" id=\"hidden_title\" value"
                                      "=\".+  )([0-9]+)(?:/)([0-9]+)(?:.C\" />)$");
            QRegularExpressionMatch tarItr;
            QRegularExpressionMatch firtarItr;
            tarItr = tar.match(QString::fromUtf8(data.data()));
            firtarItr = firtar.match(QString::fromUtf8(data.data()));
            if(tarItr.hasMatch()){
                _3dayWeaAndTem[count][0]=tarItr.captured(1);
                qDebug()<<QStringLiteral("wea:")<<_3dayWeaAndTem[count][0];
                data=reply->readLine(1024);
                data=reply->readLine(1024);
                QRegularExpression temRe("^(?:<span>)(.+)(?:</span>)(.+)(?:<i>)(.+)(?:</i>)$");
                QRegularExpressionMatch temItr;
                temItr = temRe.match(QString::fromUtf8(data.data()));
                if(temItr.hasMatch()){
                    _3dayWeaAndTem[count][1]=temItr.captured(1)+temItr.captured(2)+temItr.captured(3);
                    qDebug()<<QStringLiteral("�¶�:")<<_3dayWeaAndTem[count][1];
                }
                ++count;
                if(!(count<3)){
                    emit crawWeatherUrlFinished();
                    return ;
                }
            }else if(firtarItr.hasMatch()){
                _3dayWeaAndTem[0][1]=firtarItr.captured(1)+QStringLiteral("��")+"/"+firtarItr.captured(2)+QStringLiteral("��");
                qDebug()<<QStringLiteral("����:")<<_3dayWeaAndTem[0][1];
            }
            data=reply->readLine(1024);
        }
        emit crawWeatherUrlFinished();
    });

}

QString FuncC::_3daysdata(const int& r,const int& c)
{
    return  _3dayWeaAndTem[r][c];
}


void FuncC::initWh()
{
    connect(wh,&WeatherHandle::analysisIPGeoAddr,this,&FuncC::analysisWh);
    wh->getIPGeoAdress();

}

void FuncC::mkDir(const QString &dirString)
{
    QDir*dir=new QDir;

    if(dir->exists(dirString))
        qDebug()<<QStringLiteral("�����ļ���'%1'").arg(dirString);
    else{
        bool ok=dir->mkpath(dirString);
        if(!ok)
            qDebug()<<QStringLiteral("����ʧ��");
        else
            qDebug()<<QStringLiteral("�����ɹ�");
    }
}

void FuncC::readWeatherFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.exists()){
        qDebug()<<QStringLiteral("�������ļ�:'%1'").arg(fileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<<QStringLiteral("�����ļ�ʧ��");
        }
        file.close();
        return;
    }
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<QStringLiteral("���ļ�ʧ��");
        return;
    }
    qDebug()<<QStringLiteral("���ļ��ɹ�");
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_0);
    in>>cityNameAboutWeather[0][0]>>cityNameAboutWeather[0][1]>>cityNameAboutWeather[1][0]>>cityNameAboutWeather[1][1]
            >>cityNameAboutWeather[2][0]>>cityNameAboutWeather[2][1];
    file.close();
}

void FuncC::writeWeatherFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<QStringLiteral("���ļ�ʧ��");
        return;
    }
    qDebug()<<QStringLiteral("���ļ��ɹ�");
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_0);
    for(int i=0;i<3;i++){
        qDebug()<<"city:"<<cityNameAboutWeather[i][0];
        out<<cityNameAboutWeather[i][0]<<cityNameAboutWeather[i][1];
    }
    file.close();
}





void FuncC::connectGetFile(const QString &instructDescription)
{
    BigFileSocket*sendFileSock=new BigFileSocket();//���̲߳����и���
    sendFileSock->setInstruct(instructDescription);
    sendFileSock->setIp(ip);
    sendFileSock->setPort(loginPort);
    QThread*thread=new QThread();
    sendFileSock->moveToThread(thread);
    thread->start();
    emit sendFileSock->start();//ת�Ƶ����߳�ȥpost host
    connect(thread,&QThread::finished,sendFileSock,&BigFileSocket::deleteLater);
    //ɾ���߳�
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //��ȡ�ļ������β����
    connect(sendFileSock,&BigFileSocket::finished,[=](int code){
        qDebug()<<"code"<<code;
        thread->exit(0);
        thread->quit();
        emit getFileFinished(code);
    });
}

QString FuncC::getCityData(const int &r, const int &c)const
{
    return cityNameAboutWeather[r][c];
}

void FuncC::setCityData(const QString &v, const int &r, const int &c)
{
    cityNameAboutWeather[r][c]=v;
}


QString FuncC::getWeatherUrl(const QString &query) const
{
    for(int i=0;i<50;i++){
        if(cityList[i][0]==query)
            return cityList[i][1];
    }
    return QString();
}

void FuncC::findCityEvent(const QString &text)
{
    cityCount=0;
    QMap<QString, QString>::const_iterator i = allCitys.constBegin();
    while (i != allCitys.constEnd()) {
        if(i.key().contains(text)){
            if(cityCount<50){
                cityList[cityCount][0]=i.key();
                cityList[cityCount][1]=i.value();
                ++cityCount;
                qDebug()<<"find citys"<<"->"<<cityList[cityCount-1][0];
            }
        }
        ++i;
    }
}

QString FuncC::indexForCityList(int r,int c) const
{
    return  cityList[r][c];
}

void FuncC::clearForCityList()
{
    cityCount=0;
    for(int i=0;i<getArrayLenth(cityList);i++){
        for(int j=0;j<getArrayLenth(cityList[0]);j++)
            cityList[i][j]="";
    }
}

int FuncC::getCityCount() const
{
    return cityCount;
}





void FuncC::setSourceIco(const QString &arg)
{
    qDebug()<<"ico";
    if(!m_win)
    {
        qDebug()<<"ico:m_win is null";
        return;
    }
    if(arg==m_sourceIco)
        return;
    m_sourceIco=arg;
    QIcon tempIco(m_sourceIco);
    if(tempIco.isNull())
    {
        qDebug()<<"sourceIco is invalid";
        return;
    }
    m_win->setIcon(tempIco);
    m_win=nullptr;//����Ϊ0
    emit sourceIcoChanged();
}


void FuncC::initAllCitys()
{

    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101010100.shtml");
    allCitys.insert(QStringLiteral("ʯ��ׯ"),"http://www.weather.com.cn/weather/101090101.shtml");
    allCitys.insert(QStringLiteral("����"),	"http://www.weather.com.cn/weather/101090201.shtml");
    allCitys.insert(QStringLiteral("�żҿ�"),"http://www.weather.com.cn/weather/101090301.shtml");
    allCitys.insert(QStringLiteral("�е�"),	"http://www.weather.com.cn/weather/101090401.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101090501.shtml");
    allCitys.insert(QStringLiteral("�ȷ�"),"http://www.weather.com.cn/weather/101090601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101090701.shtml");
    allCitys.insert(QStringLiteral("��ˮ"),"http://www.weather.com.cn/weather/101090801.shtml");
    allCitys.insert(QStringLiteral("��̨"),"http://www.weather.com.cn/weather/101090901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101091001.shtml");
    allCitys.insert(QStringLiteral("�ػʵ�"),"http://www.weather.com.cn/weather/101091101.shtml");
    allCitys.insert(QStringLiteral("�۰�����"),"http://www.weather.com.cn/weather/101091201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101040100.shtml");
    allCitys.insert(QStringLiteral("���ͺ���"),"http://www.weather.com.cn/weather/101080101.shtml");
    allCitys.insert(QStringLiteral("��ͷ"),"http://www.weather.com.cn/weather/101080201.shtml");
    allCitys.insert(QStringLiteral("�ں�"),"http://www.weather.com.cn/weather/101080301.shtml");
    allCitys.insert(QStringLiteral("�����첼"),"http://www.weather.com.cn/weather/101080401.shtml");
    allCitys.insert(QStringLiteral("ͨ��"),"http://www.weather.com.cn/weather/101080501.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101080601.shtml");
    allCitys.insert(QStringLiteral("������˹"),"http://www.weather.com.cn/weather/101080701.shtml");
    allCitys.insert(QStringLiteral("�����׶�"),"http://www.weather.com.cn/weather/101080801.shtml");
    allCitys.insert(QStringLiteral("���ֹ���"),"http://www.weather.com.cn/weather/101080901.shtml");
    allCitys.insert(QStringLiteral("���ױ���"),"http://www.weather.com.cn/weather/101081001.shtml");
    allCitys.insert(QStringLiteral("�˰���"),"http://www.weather.com.cn/weather/101081101.shtml");
    allCitys.insert(QStringLiteral("��������"),"http://www.weather.com.cn/weather/101081201.shtml");
    allCitys.insert(QStringLiteral("�Ϻ�"),"http://www.weather.com.cn/weather/101020100.shtml");
    allCitys.insert(QStringLiteral("�人"),"http://www.weather.com.cn/weather/101200101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101200201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101200301.shtml");
    allCitys.insert(QStringLiteral("Т��"),"http://www.weather.com.cn/weather/101200401.shtml");
    allCitys.insert(QStringLiteral("�Ƹ�"),"http://www.weather.com.cn/weather/101200501.shtml");
    allCitys.insert(QStringLiteral("��ʯ"),"http://www.weather.com.cn/weather/101200601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101200701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101200801.shtml");
    allCitys.insert(QStringLiteral("�˲�"),"http://www.weather.com.cn/weather/101200901.shtml");
    allCitys.insert(QStringLiteral("��ʩ"),"http://www.weather.com.cn/weather/101201001.shtml");
    allCitys.insert(QStringLiteral("ʮ��"),"http://www.weather.com.cn/weather/101201101.shtml");
    allCitys.insert(QStringLiteral("��ũ��"),"http://www.weather.com.cn/weather/101201201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101201301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101201401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101201501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101201601.shtml");
    allCitys.insert(QStringLiteral("Ǳ��"),"http://www.weather.com.cn/weather/101201701.shtml");
    allCitys.insert(QStringLiteral("̫ԭ"),"http://www.weather.com.cn/weather/101100101.shtml");
    allCitys.insert(QStringLiteral("��ͬ"),"http://www.weather.com.cn/weather/101100201.shtml");
    allCitys.insert(QStringLiteral("��Ȫ"),"http://www.weather.com.cn/weather/101100301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101100401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101100501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101100601.shtml");
    allCitys.insert(QStringLiteral("�ٷ�"),"http://www.weather.com.cn/weather/101100701.shtml");
    allCitys.insert(QStringLiteral("�˳�"),"http://www.weather.com.cn/weather/101100801.shtml");
    allCitys.insert(QStringLiteral("˷��"),"http://www.weather.com.cn/weather/101100901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101101001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101101100.shtml");
    allCitys.insert(QStringLiteral("��ɳ"),"http://www.weather.com.cn/weather/101250101.shtml");
    allCitys.insert(QStringLiteral("��̶"),"http://www.weather.com.cn/weather/101250201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250700.shtml");
    allCitys.insert(QStringLiteral("¦��"),"http://www.weather.com.cn/weather/101250801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101250901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101251001.shtml");
    allCitys.insert(QStringLiteral("�żҽ�"),"http://www.weather.com.cn/weather/101251101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101251201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101251401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101251501.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101030100.shtml");
    allCitys.insert(QStringLiteral("֣��"),"http://www.weather.com.cn/weather/101180101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180301.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101180401.shtml");
    allCitys.insert(QStringLiteral("ƽ��ɽ"),"http://www.weather.com.cn/weather/101180501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101180901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101181001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101181101.shtml");
    allCitys.insert(QStringLiteral("�ױ�"),"http://www.weather.com.cn/weather/101181201.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101181301.shtml");
    allCitys.insert(QStringLiteral("�ܿ�"),"http://www.weather.com.cn/weather/101181401.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101181501.shtml");
    allCitys.insert(QStringLiteral("פ���"),"http://www.weather.com.cn/weather/101181601.shtml");
    allCitys.insert(QStringLiteral("����Ͽ"),"http://www.weather.com.cn/weather/101181701.shtml");
    allCitys.insert(QStringLiteral("��Դ"),"http://www.weather.com.cn/weather/101181801.shtml");
    allCitys.insert(QStringLiteral("�Ϸ�"),"http://www.weather.com.cn/weather/101220101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220201.shtml");
    allCitys.insert(QStringLiteral("�ߺ�"),"http://www.weather.com.cn/weather/101220301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220401.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101220501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101220901.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101221001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101221101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101221201.shtml");
    allCitys.insert(QStringLiteral("ͭ��"),"http://www.weather.com.cn/weather/101221301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101221401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101221501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101221701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101260101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101260201.shtml");
    allCitys.insert(QStringLiteral("��˳"),"http://www.weather.com.cn/weather/101260301.shtml");
    allCitys.insert(QStringLiteral("ǭ��"),"http://www.weather.com.cn/weather/101260401.shtml");
    allCitys.insert(QStringLiteral("ǭ����"),"http://www.weather.com.cn/weather/101260501.shtml");
    allCitys.insert(QStringLiteral("ͭ��"),"http://www.weather.com.cn/weather/101260601.shtml");
    allCitys.insert(QStringLiteral("�Ͻ�"),"http://www.weather.com.cn/weather/101260701.shtml");
    allCitys.insert(QStringLiteral("����ˮ"),"http://www.weather.com.cn/weather/101260801.shtml");
    allCitys.insert(QStringLiteral("ǭ����"),"http://www.weather.com.cn/weather/101260901.shtml");
    allCitys.insert(QStringLiteral("�Ͼ�"),"http://www.weather.com.cn/weather/101190101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101190201.shtml");
    allCitys.insert(QStringLiteral(" ��"),"http://www.weather.com.cn/weather/101190301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101190401.shtml");
    allCitys.insert(QStringLiteral("��ͨ"),"http://www.weather.com.cn/weather/101190501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101190601.shtml");
    allCitys.insert(QStringLiteral("�γ�"),"http://www.weather.com.cn/weather/101190701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101190801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101190901.shtml");
    allCitys.insert(QStringLiteral("���Ƹ�"),"http://www.weather.com.cn/weather/101191001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101191101.shtml");
    allCitys.insert(QStringLiteral("̩��"),"http://www.weather.com.cn/weather/101191201.shtml");
    allCitys.insert(QStringLiteral("��Ǩ"),"http://www.weather.com.cn/weather/101191301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110200.shtml");
    allCitys.insert(QStringLiteral("�Ӱ�"),"http://www.weather.com.cn/weather/101110300.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110401.shtml");
    allCitys.insert(QStringLiteral("μ��"),"http://www.weather.com.cn/weather/101110501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101110901.shtml");
    allCitys.insert(QStringLiteral("ͭ��"),"http://www.weather.com.cn/weather/101111001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101111101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101150801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101120101.shtml");
    allCitys.insert(QStringLiteral("�ൺ"),"http://www.weather.com.cn/weather/101120201.shtml");
    allCitys.insert(QStringLiteral("�Ͳ�"),"http://www.weather.com.cn/weather/101120301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101120401.shtml");
    allCitys.insert(QStringLiteral("��̨"),"http://www.weather.com.cn/weather/101120501.shtml");
    allCitys.insert(QStringLiteral("Ϋ��"),"http://www.weather.com.cn/weather/101120601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101120701.shtml");
    allCitys.insert(QStringLiteral("̩��"),"http://www.weather.com.cn/weather/101120801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101120901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101121001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101121101.shtml");
    allCitys.insert(QStringLiteral("��Ӫ"),"http://www.weather.com.cn/weather/101121201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101121301.shtml");
    allCitys.insert(QStringLiteral("��ׯ"),"http://www.weather.com.cn/weather/101121401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101121501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101121601.shtml");
    allCitys.insert(QStringLiteral("�ĳ�"),"http://www.weather.com.cn/weather/101121701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210501.shtml");
    allCitys.insert(QStringLiteral("̨��"),"http://www.weather.com.cn/weather/101210601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101210701.shtml");
    allCitys.insert(QStringLiteral("��ˮ"),"http://www.weather.com.cn/weather/101210801.shtml");
    allCitys.insert(QStringLiteral("��"),"http://www.weather.com.cn/weather/101210901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101211001.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101211101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101170101.shtml");
    allCitys.insert(QStringLiteral("ʯ��ɽ"),"http://www.weather.com.cn/weather/101170201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101170301.shtml");
    allCitys.insert(QStringLiteral("��ԭ"),"http://www.weather.com.cn/weather/101170401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101170501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101140101.shtml");
    allCitys.insert(QStringLiteral("�տ���"),"http://www.weather.com.cn/weather/101140201.shtml");
    allCitys.insert(QStringLiteral("ɽ��"),"http://www.weather.com.cn/weather/101140301.shtml");
    allCitys.insert(QStringLiteral("��֥"),"http://www.weather.com.cn/weather/101140401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101140501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101140601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101140701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101290101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101290201.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101290301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101290401.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101290501.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101290601.shtml");
    allCitys.insert(QStringLiteral("��Ϫ"),"http://www.weather.com.cn/weather/101290701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101290801.shtml");
    allCitys.insert(QStringLiteral("�ն�"),"http://www.weather.com.cn/weather/101290901.shtml");
    allCitys.insert(QStringLiteral("��ͨ"),"http://www.weather.com.cn/weather/101291001.shtml");
    allCitys.insert(QStringLiteral("�ٲ�"),"http://www.weather.com.cn/weather/101291101.shtml");
    allCitys.insert(QStringLiteral("ŭ��"),"http://www.weather.com.cn/weather/101291201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101291301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101291401.shtml");
    allCitys.insert(QStringLiteral("�º�"),"http://www.weather.com.cn/weather/101291501.shtml");
    allCitys.insert(QStringLiteral("��˫����"),"http://www.weather.com.cn/weather/101291601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230401.shtml");
    allCitys.insert(QStringLiteral("Ȫ��"),"http://www.weather.com.cn/weather/101230501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101230801.shtml");
    allCitys.insert(QStringLiteral("��ƽ"),"http://www.weather.com.cn/weather/101230901.shtml");
    allCitys.insert(QStringLiteral("���㵺"),"http://www.weather.com.cn/weather/101231001.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101320101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101160101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101160201.shtml");
    allCitys.insert(QStringLiteral("ƽ��"),"http://www.weather.com.cn/weather/101160301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101160401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101160501.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101160601.shtml");
    allCitys.insert(QStringLiteral("��Ҵ"),"http://www.weather.com.cn/weather/101160701.shtml");
    allCitys.insert(QStringLiteral("��Ȫ"),"http://www.weather.com.cn/weather/101160801.shtml");
    allCitys.insert(QStringLiteral("��ˮ"),"http://www.weather.com.cn/weather/101160901.shtml");
    allCitys.insert(QStringLiteral("¤��"),"http://www.weather.com.cn/weather/101161001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101161101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101161201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101161301.shtml");
    allCitys.insert(QStringLiteral("������"),"http://www.weather.com.cn/weather/101161401.shtml");
    allCitys.insert(QStringLiteral("̨��"),"http://www.weather.com.cn/weather/101340101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101340201.shtml");
    allCitys.insert(QStringLiteral("̨��"),"http://www.weather.com.cn/weather/101340401.shtml");
    allCitys.insert(QStringLiteral("��³ľ��"),"http://www.weather.com.cn/weather/101130101.shtml");
    allCitys.insert(QStringLiteral("��������"),"http://www.weather.com.cn/weather/101130201.shtml");
    allCitys.insert(QStringLiteral("ʯ����"),"http://www.weather.com.cn/weather/101130301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101130401.shtml");
    allCitys.insert(QStringLiteral("��³��"),"http://www.weather.com.cn/weather/101130501.shtml");
    allCitys.insert(QStringLiteral("��������"),"http://www.weather.com.cn/weather/101130601.shtml");
    allCitys.insert(QStringLiteral("������"),"http://www.weather.com.cn/weather/101130701.shtml");
    allCitys.insert(QStringLiteral("������"),"http://www.weather.com.cn/weather/101130801.shtml");
    allCitys.insert(QStringLiteral("��ʲ"),"http://www.weather.com.cn/weather/101130901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101131001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101131101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101131201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101131301.shtml");
    allCitys.insert(QStringLiteral("����̩"),"http://www.weather.com.cn/weather/101131401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101131501.shtml");
    allCitys.insert(QStringLiteral("��������"),"http://www.weather.com.cn/weather/101131601.shtml");
    allCitys.insert(QStringLiteral("ͼľ���"),"http://www.weather.com.cn/weather/101131701.shtml");
    allCitys.insert(QStringLiteral("�����"),"http://www.weather.com.cn/weather/101131801.shtml");
    allCitys.insert(QStringLiteral("���Ź�"),"http://www.weather.com.cn/weather/101131901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101132101.shtml");
    allCitys.insert(QStringLiteral("˫��"),"http://www.weather.com.cn/weather/101132201.shtml");
    allCitys.insert(QStringLiteral("�ɿ˴���"),"http://www.weather.com.cn/weather/101132301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300701.shtml");
    allCitys.insert(QStringLiteral("���"),"http://www.weather.com.cn/weather/101300801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101300901.shtml");
    allCitys.insert(QStringLiteral("��ɫ"),"http://www.weather.com.cn/weather/101301001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101301101.shtml");
    allCitys.insert(QStringLiteral("�ӳ�"),"http://www.weather.com.cn/weather/101301201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101301301.shtml");
    allCitys.insert(QStringLiteral("���Ǹ�"),"http://www.weather.com.cn/weather/101301401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101330101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101280101.shtml");
    allCitys.insert(QStringLiteral("�ع�"),"http://www.weather.com.cn/weather/101280201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101280301.shtml");
    allCitys.insert(QStringLiteral("÷��"),"http://www.weather.com.cn/weather/101280401.shtml");
    allCitys.insert(QStringLiteral("��ͷ"),"http://www.weather.com.cn/weather/101280501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101280601.shtml");
    allCitys.insert(QStringLiteral("�麣"),"http://www.weather.com.cn/weather/101280701.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101280800.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101280901.shtml");
    allCitys.insert(QStringLiteral("տ��"),"http://www.weather.com.cn/weather/101281001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101281101.shtml");
    allCitys.insert(QStringLiteral("��Դ"),"http://www.weather.com.cn/weather/101281201.shtml");
    allCitys.insert(QStringLiteral("��Զ"),"http://www.weather.com.cn/weather/101281301.shtml");
    allCitys.insert(QStringLiteral("�Ƹ�"),"http://www.weather.com.cn/weather/101281401.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101281501.shtml");
    allCitys.insert(QStringLiteral("��ݸ"),"http://www.weather.com.cn/weather/101281601.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101281701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101281801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101281901.shtml");
    allCitys.insert(QStringLiteral("ï��"),"http://www.weather.com.cn/weather/101282001.shtml");
    allCitys.insert(QStringLiteral("��β"),"http://www.weather.com.cn/weather/101282101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310202.shtml");
    allCitys.insert(QStringLiteral("�ٸ�"),"http://www.weather.com.cn/weather/101310203.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310204.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310205.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310206.shtml");
    allCitys.insert(QStringLiteral("��ɳ"),"http://www.weather.com.cn/weather/101310207.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310208.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310209.shtml");
    allCitys.insert(QStringLiteral("�Ͳ�"),"http://www.weather.com.cn/weather/101310210.shtml");
    allCitys.insert(QStringLiteral("��"),"http://www.weather.com.cn/weather/101310211.shtml");
    allCitys.insert(QStringLiteral("�Ĳ�"),"http://www.weather.com.cn/weather/101310212.shtml");
    allCitys.insert(QStringLiteral("��ͤ"),"http://www.weather.com.cn/weather/101310214.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101310215.shtml");
    allCitys.insert(QStringLiteral("��ˮ"),"http://www.weather.com.cn/weather/101310216.shtml");
    allCitys.insert(QStringLiteral("�ֶ�"),"http://www.weather.com.cn/weather/101310221.shtml");
    allCitys.insert(QStringLiteral("��ָɽ"),"http://www.weather.com.cn/weather/101310222.shtml");
    allCitys.insert(QStringLiteral("��ɳ"),"http://www.weather.com.cn/weather/101310302.shtml");
    allCitys.insert(QStringLiteral("��ɳ"),"http://www.weather.com.cn/weather/101310303.shtml");
    allCitys.insert(QStringLiteral("��ɳ"),"http://www.weather.com.cn/weather/101310304.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101070101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101070201.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101070301.shtml");
    allCitys.insert(QStringLiteral("��˳"),"http://www.weather.com.cn/weather/101070401.shtml");
    allCitys.insert(QStringLiteral("��Ϫ"),"http://www.weather.com.cn/weather/101070501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101070601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101070701.shtml");
    allCitys.insert(QStringLiteral("Ӫ��"),"http://www.weather.com.cn/weather/101070801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101070901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101071001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101071101.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101071201.shtml");
    allCitys.insert(QStringLiteral("�̽�"),"http://www.weather.com.cn/weather/101071301.shtml");
    allCitys.insert(QStringLiteral("��«��"),"http://www.weather.com.cn/weather/101071401.shtml");
    allCitys.insert(QStringLiteral("�ɶ�"),"http://www.weather.com.cn/weather/101270101.shtml");
    allCitys.insert(QStringLiteral("��֦��"),"http://www.weather.com.cn/weather/101270201.shtml");
    allCitys.insert(QStringLiteral("�Թ�"),"http://www.weather.com.cn/weather/101270301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101270401.shtml");
    allCitys.insert(QStringLiteral("�ϳ�"),"http://www.weather.com.cn/weather/101270501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101270601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101270701.shtml");
    allCitys.insert(QStringLiteral("�㰲"),"http://www.weather.com.cn/weather/101270801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101270901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101271001.shtml");
    allCitys.insert(QStringLiteral("�˱�"),"http://www.weather.com.cn/weather/101271101.shtml");
    allCitys.insert(QStringLiteral("�ڽ�"),"http://www.weather.com.cn/weather/101271201.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101271301.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101271401.shtml");
    allCitys.insert(QStringLiteral("üɽ"),"http://www.weather.com.cn/weather/101271501.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101271601.shtml");
    allCitys.insert(QStringLiteral("�Ű�"),"http://www.weather.com.cn/weather/101271701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101271801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101271901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101272001.shtml");
    allCitys.insert(QStringLiteral("��Ԫ"),"http://www.weather.com.cn/weather/101272101.shtml");
    allCitys.insert(QStringLiteral("������"),"http://www.weather.com.cn/weather/101050101.shtml");
    allCitys.insert(QStringLiteral("�������"),"http://www.weather.com.cn/weather/101050201.shtml");
    allCitys.insert(QStringLiteral("ĵ����"),"http://www.weather.com.cn/weather/101050301.shtml");
    allCitys.insert(QStringLiteral("��ľ˹"),"http://www.weather.com.cn/weather/101050401.shtml");
    allCitys.insert(QStringLiteral("�绯"),"http://www.weather.com.cn/weather/101050501.shtml");
    allCitys.insert(QStringLiteral("�ں�"),"http://www.weather.com.cn/weather/101050601.shtml");
    allCitys.insert(QStringLiteral("���˰���"),"http://www.weather.com.cn/weather/101050701.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101050801.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101050901.shtml");
    allCitys.insert(QStringLiteral("��̨��"),"http://www.weather.com.cn/weather/101051001.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101051101.shtml");
    allCitys.insert(QStringLiteral("�׸�"),"http://www.weather.com.cn/weather/101051201.shtml");
    allCitys.insert(QStringLiteral("˫Ѽɽ"),"http://www.weather.com.cn/weather/101051301.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101060101.shtml");
    allCitys.insert(QStringLiteral(" ����"),"http://www.weather.com.cn/weather/101060201.shtml");
    allCitys.insert(QStringLiteral("�ӱ�"),"http://www.weather.com.cn/weather/101060301.shtml");
    allCitys.insert(QStringLiteral("��ƽ"),"http://www.weather.com.cn/weather/101060401.shtml");
    allCitys.insert(QStringLiteral("ͨ��"),"http://www.weather.com.cn/weather/101060501.shtml");
    allCitys.insert(QStringLiteral("�׳�"),"http://www.weather.com.cn/weather/101060601.shtml");
    allCitys.insert(QStringLiteral("��Դ"),"http://www.weather.com.cn/weather/101060701.shtml");
    allCitys.insert(QStringLiteral("��ԭ"),"http://www.weather.com.cn/weather/101060801.shtml");
    allCitys.insert(QStringLiteral("��ɽ"),"http://www.weather.com.cn/weather/101060901.shtml");
    allCitys.insert(QStringLiteral("�ϲ�"),"http://www.weather.com.cn/weather/101240101.shtml");
    allCitys.insert(QStringLiteral("�Ž�"),"http://www.weather.com.cn/weather/101240201.shtml");
    allCitys.insert(QStringLiteral("���� "),"http://www.weather.com.cn/weather/101240301.shtml");
    allCitys.insert(QStringLiteral("���� "),"http://www.weather.com.cn/weather/101240401.shtml");
    allCitys.insert(QStringLiteral("�˴�"),"http://www.weather.com.cn/weather/101240501.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101240601.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101240701.shtml");
    allCitys.insert(QStringLiteral("������"),"http://www.weather.com.cn/weather/101240801.shtml");
    allCitys.insert(QStringLiteral("Ƽ��"),"http://www.weather.com.cn/weather/101240901.shtml");
    allCitys.insert(QStringLiteral("����"),"http://www.weather.com.cn/weather/101241001.shtml");
    allCitys.insert(QStringLiteral("ӥ̶"),"http://www.weather.com.cn/weather/101241101.shtml");
}
