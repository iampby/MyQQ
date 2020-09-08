#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "funcc.h"
#include"registersocket.h"
#include"loginsocket.h"
#include"bigfilesocket.h"
#include"headimgwidget.h"
#include"friendmodel.h"
#include"addfriendgroupwidget.h"
#include"screenwidget.h"
#include<qthread.h>
#include<QDebug>
#include<qprocess.h>
#include<qsqlquery.h>
#include <qregularexpression.h>
#include <qdir.h>
#include <qhostaddress.h>
#include<QFileDialog>
#include<QXmlStreamReader>
#include<QXmlStreamWriter>
#include<qtextcodec.h>
#include <QQuickItem>
#include <qlabel.h>
#include <QBuffer>
#include<qlocalserver.h>
#include<qlocalsocket.h>
#include<qmessagebox.h>
#include <qjsonarray.h>
#include<qmath.h>
#include<thread>
#include<sstream>
#include<qsqlerror.h>
#include <qqmlfile.h>
//funcc类
FuncC::FuncC(QObject *parent):QObject(parent)
{
    m_win=Q_NULLPTR;registerSock=nullptr;loginSock=nullptr;timer=nullptr;updateTimer=nullptr;server=nullptr;
    addFGWidget=nullptr;
    processCount=0;ip="127.0.0.1";m_sourceIco="";
    loginPort=5567;updatePort=5568;registerPort=5566;
    online=false;
    m_myQQ=QString();m_passwd=QString();//初始化myqq,passwd
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
        emit timer->stopMonitor();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
        delete timer,timer=nullptr;
    }
    if(updateTimer){
        qDebug()<<"updating timer can be to delete";
        emit updateTimer->stopTimer();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
        delete updateTimer,updateTimer=nullptr;
    }
    if(server){
        emit server->emitExit();//退出线程，删除
    }
    ScreenWidget::deletionInstance();
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
    //用户的 name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    QObject* obj=dynamic_cast<QObject*>(m_win);
    QMetaObject::invokeMethod(obj,"runGetUserInfo",Qt::DirectConnection,
                              Q_ARG(QVariant,QVariant::fromValue(userInfo)));
    QFile info("../user/"+m_myQQ+"/info.xml");
    if(!info.open(QFile::ReadOnly)){
        qDebug()<<"opened info.xml unsuccessfully";
        return;
    }
    QXmlStreamReader reader;
    reader.setDevice(&info);
    while(!reader.atEnd()){
        reader.readNextStartElement();
        if(reader.name().toString()=="set"){//初始化显示设置信息
            initSetInfo(reader);
            // qDebug()<<"information of setting had been sended:"<<setInfo;
            QMetaObject::invokeMethod(obj,"runGetSetInfoFunction",Qt::DirectConnection,
                                      Q_ARG(QVariant,QVariant::fromValue(setInfo)));
        }else if(reader.name().toString()=="friendGroup"){//传递好友信息
            initFriendInfo(reader);
        }else if(reader.name().toString()=="groupChat"){//传递群信息
            initGroupChatInfo(reader);
        }
    }
    info.close();
    info.remove();
    //定时2秒后发送信号处理先前的验证消息对象和信息列表 让qml界面有空隙先生成界面
    QTimer::singleShot(1500,this,[=](){
        emit    emitHandleVerifyAndInfoList();
    });
    qDebug()<<"initLoginInfo() function running successfully.";
}

void FuncC::initSetInfo(QXmlStreamReader &reader)
{
    while(true){
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement:
            setInfo.insert(reader.name().toString(),reader.readElementText());
            break;
        case QXmlStreamReader::EndElement://到达末尾返回
            if(reader.name().toString()=="set")
                return;
        case QXmlStreamReader::Invalid://这个并不是真的错误，需用hasError()检测
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
            if(reader.name().toString()=="friend"){//添加好友信息
                parseFriendInfo(reader,groupName,pos);
                groupName=QString();
            }else{
                //发送组名到Qml
                ++pos;
                groupName=reader.name().toString();
                QString name=reader.attributes().value("name").toString();;
                qDebug()<<"the group has been sended, named "<<name;;
                emit getFriendGroup(name,reader.attributes().value("set").toString(),pos);
            }
        }else if(reader.isEndElement()){
            if(reader.name().toString()=="friendGroup")
                return;
            else{
                groupName=QString();
            }
        }else if(reader.hasError()){
            qDebug()<<"initFriendInfo() function has a error:"<<reader.errorString();
            return;
        }else if(reader.isEndDocument())return;
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
        QString name=reader.attributes().value("name").toString();
        if(reader.name().toString()=="friend"&&reader.isStartElement()){
            QVariantMap myqqMap;//QvariantMap可以直接传参
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
                    }else if(reader.name().toString()==("个性签名")||reader.name().toString()==("备注")){
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
                }else if(reader.isEndDocument())return;
            }
        }else if(reader.name()==endString){//结束添加好友信息
            break;
        }else if(reader.isEndDocument())return;
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






//获取任务栏方向
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
    //因为在后面通信时不知道为什么不能释放，所以放到开头
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
    //注意凡是对注册qml属性赋值,但不是在qml引擎里赋值时，必须发出信号提醒qml作出改变
    setMyQQ(myqq);
    setPasswd(passwd);
    //用户的 name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    loginSock=new LoginSocket(myqq,passwd,this);
    connect(loginSock,&LoginSocket::finished,[=](int result){
        qDebug()<<"result:"<<result;
        //成功
        if(result==0){
            userInfo=loginSock->infoObj.toVariantMap();
            userInfo.remove("instruct");
            userInfo.remove("result");
            qint64 ad=userInfo.value("days").toLongLong();//转换为安全数据，js没有64位整数
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
                    if(loginSock->isOpen())
                        loginSock->close();

                    // initLoginInfo();
                    /*QFile r("../user/"+myqq+"/info.xml");
                if(!r.remove())
                    qDebug()<<"removed info.xml unsuccessfully.";*/
                    loginSock->deleteLater();//删除指针
                    //子线程 网路状态定时判断
                    online=true;
                    timer=new NetMonitor();
                    timer->setTimerInterval(2000);//2s监测
                    connect(timer,&NetMonitor::getNetStatus,timer,[=](qint32&status){
                        //断网了
                        if(online&&status==-1){
                            qDebug()<<"disconnected network";
                            online=false;
                            //有网了
                        }else if(!online&&status!=-1){
                            qDebug()<<"connected network";
                            online=true;
                        }
                    });
                    //程序终止 直接删除
                    connect(qApp,&QApplication::aboutToQuit,timer,[=](){
                        if(timer){
                            qDebug()<<"network timer can be to delete";
                            timer->thread()->requestInterruption();//中断
                            emit timer->stopMonitor();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
                            delete timer,timer=nullptr;
                        }
                    });
                    emit timer->startMonitor();
                    //子线程 定时更新界面信息
                    updateTimer=new UpdateTimer();
                    updateTimer->setMyqq(myqq);
                    updateTimer->setIp(ip);
                    updateTimer->setPort(loginPort);
                    updateTimer->setTimerInterval(100000);//五分钟获取一次更新
                    emit updateTimer->startTimer();
                    connect(updateTimer,&UpdateTimer::emitResult,this,&FuncC::updateHandle);
                    //程序终止 直接删除
                    connect(qApp,&QApplication::aboutToQuit,updateTimer,[=](){
                        qDebug()<<"updating timer can be to delete";
                        if(updateTimer){
                            updateTimer->thread()->requestInterruption();//中断
                            emit updateTimer->stopTimer();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
                            delete updateTimer,updateTimer=nullptr;
                        }
                    });
                    return;
                }
            }
            emit loginResult(3);
        }
        emit loginResult(result);
        if(loginSock->isOpen())
            loginSock->close();
        loginSock->deleteLater();//删除指针
        //清理本地服务器
        if(server){
            emit server->emitExit();//发送信号清理所有server线程对象
        }
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
    server->setMyQQ(m_myQQ);
    QThread*thread=new QThread(this);//线程父对象最好不要和线程活跃在一个线程 因为父对象毁灭时删除子对象，而线程可能还没退出
    server->moveToThread(thread);
    connect(thread,&QThread::started,server,&NativeServer::slotStarted);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,server,[=]()mutable{
        server->deleteLater();
        server=nullptr;//sever已赋值地址值，可以置为0 防止野指针
    });
    connect(server,&NativeServer::emitExit,server,[=](){
        qDebug()<<"server thread will be exited";
        thread->requestInterruption();
        thread->exit(0);
        thread->quit();
        thread->wait();
    });
    //验证消息
    connect(server,&NativeServer::emitFverify,this,&FuncC::getFVerify);
    //添加好友消息
    connect(server,&NativeServer::emitGetFriend,this,&FuncC::getFriend,Qt::DirectConnection);//直连为子线程运行
    connect(server,&NativeServer::emitOffline,this,&FuncC::offline,Qt::DirectConnection);
    connect(server,&NativeServer::emitFMessage,this,&FuncC::GetFMessage,Qt::DirectConnection);
    thread->start();
}

void FuncC::realseServer()
{
    if(server){
        qDebug()<<"server could be deleted";
        emit server->emitExit();
    }
}
//更新界面信息
void FuncC::updateHandle(const bool &ok)
{
    qDebug()<<"updateHandle(bool &ok)"<<ok;
    if(ok){
        //处理更新好友头像
        if(!updateTimer->historyMap.isEmpty()){
            qDebug()<<"updated a friends image of head ";
            QMap<QString,QPixmap>::const_iterator i=updateTimer->historyMap.cbegin();
            QMap<QString,QPixmap>::const_iterator end=updateTimer->historyMap.cend();
            while(i!=end){
                QString number=i.key();
                QPixmap&newpix= images->provider2->images[number];
                newpix= i.value();//赋新值
                newpix.save("./x.png");
                emit updateFriendsModel("image://friends/"+number+"1",FriendModel::ImgPathRole,number);//更新一个好友头像
                ++i;
            }
            updateTimer->historyMap.clear();
        }
        //更新签名
        if(!updateTimer->sigMap.isEmpty()){
            qDebug()<<"updated  friends signature ";
            QMap<QString,QString>::const_iterator i=updateTimer->sigMap.cbegin();
            QMap<QString,QString>::const_iterator end=updateTimer->sigMap.cend();
            while(i!=end){
                QString number=i.key();
                QString sig=i.value();
                emit updateFriendsModel(sig,FriendModel::SignatureRole,number);//更新一个好友头像
                ++i;
            }
            updateTimer->sigMap.clear();
        }
        //更新昵称
        if(!updateTimer->nameMap.isEmpty()){
            qDebug()<<"updated  friends name ";
            QMap<QString,QString>::const_iterator i=updateTimer->nameMap.cbegin();
            QMap<QString,QString>::const_iterator end=updateTimer->nameMap.cend();
            while(i!=end){
                QString number=i.key();
                QString name=i.value();
                emit updateFriendsModel(name,FriendModel::NameRole,number);//更新一个好友昵称
                ++i;
            }
            updateTimer->nameMap.clear();
        }
        //更新状态
        if(updateTimer->statusMap.size()>0){
            qDebug()<<"updated  friends status";
            QVariantMap::const_iterator i=updateTimer->statusMap.cbegin();
            QVariantMap::const_iterator end=updateTimer->statusMap.cend();
            while(i!=end){
                QString number=i.key();
                QString name=i.value().toString();
                emit updateFriendsModel(name,FriendModel::StatusRole,number);//更新一个好友昵称
                ++i;
            }
            updateTimer->statusMap.clear();
        }
        //更新好友等级
        if(!updateTimer->fgrade.isEmpty()){
            QVariantMap::const_iterator i=updateTimer->fgrade.cbegin();
            QVariantMap::const_iterator end=updateTimer->fgrade.cend();
            while(i!=end){
                QString number=i.key();
                QString grade=QString("%1").arg(i.value().toInt());
                emit updateFriendsModel(grade,FriendModel::GradeRole,number);//更新一个好友等级
                ++i;
            }
        }
        //更新我的活跃度和等级
        emit updateMyGrade(updateTimer->grade,updateTimer->ads);
        //重置等级
        updateTimer->grade=-1;updateTimer->ads="0";
        //由于正常刷新灰度图不能正常刷新 发送信号换掉整个模型
        emit updateTotalFModel();
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
    QDir dir("../user/"+m_myQQ+"/friends"+"/"+number+"1.png");
    if(!dir.exists()){
        dir.mkpath("./");
    }
    pix.save("../user/"+m_myQQ+"/friends"+"/"+number+"1.png","png");
    images->provider2->images.insert(number+"1",pix);
    emit emitFriend(map);

}

void FuncC::offline(QString ip, QString host,QString datetime)
{
    QVariantMap map;
    map.insert("ip",ip);
    map.insert("host",host);
    datetime.replace("/","-");
    map.insert("datetime",datetime);
    emit emitOffline(map);
}

void FuncC::GetFMessage(QString html,QString number,QString time)
{
    if(html.isEmpty()){
        qDebug()<<"lost html content on the midway";
        return;
    }
    emit friendMessage(number, html,time);//发送消息信号
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

void FuncC::update()
{
    qDebug()<<"start upating";
    if(updateTimer){
        emit  updateTimer->update();//立即获取一次
    }
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
    tipWin->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);//主窗口之上
    QLabel*lab=new QLabel(tipWin);
    lab->move(180,215);
    lab->setText(("恭喜你！已经是最新版本"));
    tipWin->setToolTip(("恭喜你！已经是最新版本"));
    tipWin->setAttribute(Qt::WA_QuitOnClose,false);//关闭默认顶层窗口关闭退出行为
    tipWin->setAttribute(Qt::WA_DeleteOnClose,true);//用完就删
    tipWin->show();
}

void FuncC::addHeadWidget(QWindow *w,const int&x,const int&y,QPixmap pixmap,const QString&myqq,const bool isgot)
{
    HeadImgWidget*widget=new HeadImgWidget;
    widget->setWindowFlag(Qt::FramelessWindowHint,true);//必须去除标题栏，设置parent不会自动去除标题
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
            });//读取头像
        }

        widget->setHeadImg(pixmap);//添加当前头像
        temp->setGeometry(x,y,widget->width(),widget->height());
        connect(this,&FuncC::emitHeadImgOpenFile,widget,&HeadImgWidget::openFile);//打开文件
        connect(this,&FuncC::emitHeadImgOKClicked,widget,[=](Images*images){
            widget->okClicked(images,myqq);
        });//ok处理
        //刷新好友模型id值
        connect(widget,&HeadImgWidget::updateMyself,this,[=](const QString&number){
            qDebug()<<"emit updateFriendsModel(id)";
            emit updateFriendsModel("image://friends/"+number+"1",FriendModel::ImgPathRole,number);
            //由于正常刷新灰度图不能正常刷新  这里为了方便配合刷新灰度图方法 发送信号换掉整个模型来刷新图片
            emit updateTotalFModel();
        });
        //更新远程头像
        connect(widget,&HeadImgWidget::updateRemoteHeadImg,this,[=](const QPixmap&pix){
            qDebug()<<"updateRemoteHeadImg signal had sent";
            disconnect(widget,SIGNAL(updateRemoteHeadImg(QPixmap)));//先断开连接，这个通信只调用一次

            QString instructDescription="4 historyHeadImg "+myqq+" writedHeaderSize";
            BigFileSocket*updateImgSock=new BigFileSocket();//子线程不能有父类
            updateImgSock->setInstruct(instructDescription);
            updateImgSock->setIp(ip);
            updateImgSock->setPort(updatePort);
            updateImgSock->setTimeout(20000);//20s超时
            QThread*thread=new QThread();
            updateImgSock->moveToThread(thread);
            thread->start();
            emit updateImgSock->start();//转移到新线程去post host
            connect(thread,&QThread::finished,updateImgSock,&BigFileSocket::deleteLater);
            connect(updateImgSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
            //删除线程
            connect(this,&FuncC::emitHeadImgOKClicked,thread,[=](){
                qDebug()<<"closed window causes a thread to exit";
                thread->exit(0);
                thread->quit();
            });
            connect(thread,&QThread::finished,thread,&QThread::deleteLater);
            //获取文件结果收尾处理
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

        //关闭历史头像标签
        connect(widget,&HeadImgWidget::getFocus,[=](){
            qDebug()<<"QMetaObject::invokeMethod(w,\"alterSelectedIndex\",Qt::QueuedConnection)";
            QMetaObject::invokeMethod(w,"alterSelectedIndex",Qt::QueuedConnection) ;
        });
        //添加选中的头像
        connect(this,&FuncC::emitSelectedImg,widget,[=]( QPixmap pix){
            qDebug()<<"handle selected img ";
            widget->setHeadImg(pix);
        });
        connect(this,&FuncC::emitCloseHead,widget,[=](){
            widget->deleteLater();//延迟删除
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
    emit emitCloseHead();//关闭信号
}




void FuncC::getHistoryHeadImg(const QString&myqq)const
{
    QString instructDescription="4 historyHeadImg "+myqq;
    BigFileSocket*historyImgSock=new BigFileSocket();//子线程不能有父类
    historyImgSock->setInstruct(instructDescription);
    historyImgSock->setIp(ip);
    historyImgSock->setPort(loginPort);
    QThread*thread=new QThread();
    historyImgSock->moveToThread(thread);
    thread->start();
    emit historyImgSock->start();//转移到新线程去post host
    connect(thread,&QThread::finished,historyImgSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //获取文件结果收尾处理
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
    BigFileSocket*updateSigSock=new BigFileSocket();//子线程不能有父类
    updateSigSock->setInstruct(in);
    updateSigSock->setIp(ip);
    updateSigSock->setPort(updatePort);
    updateSigSock->setTimeout(10000);//10s超时
    QThread*thread=new QThread();
    updateSigSock->moveToThread(thread);
    thread->start();
    emit updateSigSock->start();//转移到新线程去post host
    connect(thread,&QThread::finished,updateSigSock,&BigFileSocket::deleteLater);
    connect(updateSigSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //获取文件结果收尾处理
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
    QString instructDescription="7 updateCover "+m_myQQ+" writedHeaderSize";//更新封面
    BigFileSocket*updateCoverSock=new BigFileSocket();//子线程对象最好不要有父类
    updateCoverSock->setInstruct(instructDescription);
    updateCoverSock->setIp(ip);
    updateCoverSock->setPort(updatePort);
    updateCoverSock->setTimeout(20000);//20s超时
    QThread*thread=new QThread();
    updateCoverSock->moveToThread(thread);
    thread->start();
    emit updateCoverSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,updateCoverSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateCoverSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
    connect(updateCoverSock,&BigFileSocket::writtenInstruction, updateCoverSock,[=](){
        qDebug()<<"writtenInstruction"<<qmlFilePath;
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QPixmap pix;
        pix.load(QQmlFile::urlToLocalFileOrQrc(qmlFilePath));//qml路径转为本地路径
        if(pix.isNull()){
            qDebug()<<"warning:cover-image is null";
        }
        pix.save(&buffer,"png");
        QByteArray pixArray;
        pixArray.append(buffer.data());
        qDebug()<<pix.size();
        buffer.close();//关闭io设备
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
        emit timer->stopMonitor();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
        delete timer,timer=nullptr;
    }
    if(updateTimer){
        qDebug()<<"updating timer can be to delete";
        emit updateTimer->stopTimer();//阻塞队列连接信号发出 必须先退出进程事件圈在删除对象
        delete updateTimer,updateTimer=nullptr;
    }
}
//添加头像操作界面到qml更改封面界面
void FuncC::addCoverWidget(QWindow *w, const int &x, const int &y, QString filePath) const
{
    filePath.replace("file:///","");//qml路径修正
    qDebug()<<filePath;
    QPixmap pix;
    pix.load(filePath);
    if(pix.isNull()){
        qDebug()<<"warning:cover-image is null";
    }
    HeadImgWidget*widget=new HeadImgWidget(nullptr,false);//关闭遮罩
    widget->setWindowFlag(Qt::FramelessWindowHint,true);//必须去除标题栏，设置parent不会自动去除标题
    widget->setAttribute(Qt::WA_QuitOnClose,false);
    widget->winId();
    QWindow*temp=widget->windowHandle();
    //如果成功
    if(temp){
        temp->setParent(w);//嵌入
        temp->setGeometry(x,y,temp->width(),temp->height());//定位
        connect(this,&FuncC::emitCloseCover,widget,&HeadImgWidget::deleteLater);//删除widget资源
        connect(this,&FuncC::emitCoverOKClicked,widget,&HeadImgWidget::okCoverClicked);// 创建一个本地文件过渡

        widget->show();//重新show
        //注意：不知名原因导致更改封面（也就是无遮罩）的图像坐标总是大100左右
        //解决 必须先show再设置图片
        widget->openFile(filePath);//加载图片
    }else{
        qDebug()<<"widget is null";
        widget->deleteLater();
    }
}

void FuncC::closeCoverWidget()
{
    emit emitCloseCover();//关闭信号
}
//获取远程个人资料
void FuncC::getIndividualData(QString content,QString number,QQuickWindow*qmlWin)
{
    qDebug()<<"getIndividualData()";
    QString instructDescription="8 "+content+" "+number;//更新封面
    BigFileSocket*getPersonalDataSock=new BigFileSocket();//子线程对象最好不要有父类
    getPersonalDataSock->setInstruct(instructDescription);
    getPersonalDataSock->setIp(ip);
    getPersonalDataSock->setPort(loginPort);
    getPersonalDataSock->setTimeout(40000);//超时40s
    QThread*thread=new QThread();//子线程父对象最好在父线程
    getPersonalDataSock->moveToThread(thread);
    thread->start();
    emit getPersonalDataSock->start();//转移到新线程去post host
    connect(getPersonalDataSock,&BigFileSocket::finished,getPersonalDataSock,[=](){
        qDebug()<<"personal data is got or timeout";
        QVariantMap jsonObj=(QJsonDocument::fromJson(getPersonalDataSock->carrier)).object().toVariantMap();
        if(!jsonObj.isEmpty()){
            qDebug()<<"the most personal  json-data would be dispatched ";
            //添加d大部分信息到qml
            QMetaObject::invokeMethod((QObject*)qmlWin,"emitPersonalJsonInfo",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(jsonObj)));
        }else{
            qDebug()<<"the most personal  json-data is not got ";
        }
        QDir dir;
        dir.mkpath("../user/"+m_myQQ+"/photowall/"+number);
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
            //注意保存时指定格式
            if(name!="cover"){
                if(!pix.save("../user/"+m_myQQ+"/photowall/"+number+"/"+name,"png"))
                    qDebug()<<"warning:file is not save,named "<<name;
                else {
                    names.append(name);
                }
            }else{
                if(!pix.save("../user/"+m_myQQ+"/photowall/"+number+"/"+name,"png")){
                    qDebug()<<"warning:file is not save,named "<<name;
                }else
                    cover=name;
            }
            ++i;
        }
        //添加封面和照片到qml
        QMetaObject::invokeMethod((QObject*)qmlWin,"emitPersonalCoverAndPhoto",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(names)),
                                  Q_ARG(QString,cover));

        //超时放弃或完成结束线程
        thread->exit(0);
        thread->quit();
        getPersonalDataSock->deleteLater();
        thread->deleteLater();
    });
}
//更新用户远程照片墙
void FuncC::updatePhotoWall(quint8 length)
{
    QString instructDescription="9 updatePhotoWall "+m_myQQ+" writedHeaderSize";//更新封面
    BigFileSocket*updateWallSock=new BigFileSocket();//子线程对象最好不要有父类
    updateWallSock->setInstruct(instructDescription);
    updateWallSock->setIp(ip);
    updateWallSock->setPort(updatePort);
    updateWallSock->setTimeout(20000);//20s超时
    QThread*thread=new QThread();
    updateWallSock->moveToThread(thread);
    thread->start();
    emit updateWallSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,updateWallSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateWallSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
    connect(updateWallSock,&BigFileSocket::writtenInstruction, updateWallSock,[=]()mutable{
        qDebug()<<"writtenInstruction";
        //由于服务端的updateport只有一个解析json接受一个数据、处理数据的功能,这里把图片数据全部打包发送
        //数据结构 length(quint8) 各个数据的数据长度(quint32) 各个数据
        QByteArray totalData;//全部数据
        QByteArray lengthData;
        QDataStream stream(&lengthData,QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_4_0);
        stream<<length;
        totalData.append(lengthData);//记录长度
        qDebug()<<"length="<<length;
        QVector<quint32>sizeVector;//记录各个数据大小
        QVector<QByteArray>dataVector;//记录图片数据
        for (quint8 var = 0; var < length; ++var) {
            QPixmap temp=images->provider3->images.value(QString("%1").arg(var));//获取最新添加的数据
            if(temp.isNull())qDebug()<<"provider3 not found a pixmap";
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);

            if(!temp.save(&buffer,"png")){
                qDebug()<<"warning:a pixmap is not dispatched because readed data is of failure";
                buffer.close();//关闭io设备
                continue;
            }
            QByteArray  pixdata=buffer.data();
            if(pixdata.isEmpty())qDebug()<<"warning:a empty pixmap is dispatched";
            sizeVector.append(pixdata.size());
            dataVector.append(pixdata);
            buffer.close();//关闭io设备
        }
        length=sizeVector.size();
        for (quint8 var = 0; var < length; ++var) {
            QByteArray data;
            QDataStream temp(&data,QIODevice::WriteOnly);
            temp.setVersion(QDataStream::Qt_4_0);
            temp<<sizeVector.at(var);
            qDebug()<<"data size="<<data.data()<<sizeVector.at(var);
            totalData.append(data);//记录长度
        }
        length=dataVector.size();
        //保存数据
        for (quint8 var = 0; var < length; ++var) {
            totalData.append(dataVector[var]);
        }
        updateWallSock->write(totalData);
        if(!totalData.isEmpty())
            updateWallSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<"updating photo wall thread had exited";
    });
}

void FuncC::removePhotoFromRemoteWall(int pos)
{
    QJsonObject obj;
    obj.insert("myqq",QJsonValue(m_myQQ));
    obj.insert("pos",QJsonValue(pos));
    obj.insert("instruct",QJsonValue("9"));
    obj.insert("content",QJsonValue("removePhotoWall"));
    obj.insert("headerSize",QJsonValue(true));

    BigFileSocket*updateWallSock=new BigFileSocket();//子线程对象最好不要有父类
    updateWallSock->setInstruct(obj);
    updateWallSock->setIp(ip);
    updateWallSock->setPort(updatePort);
    updateWallSock->setTimeout(5000);//5s超时
    QThread*thread=new QThread();
    updateWallSock->moveToThread(thread);
    thread->start();
    emit updateWallSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,updateWallSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateWallSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
    connect(updateWallSock,&BigFileSocket::writtenInstruction, updateWallSock,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"updating photo wall thread had exited";
    });
}

void FuncC::inintCityData(QQuickWindow*w)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE", "city_data");
    db.setDatabaseName("../city.db");
    db.setHostName("MyQQ");
    db.setUserName("sa");
    db.setPassword("@123456x");
    if(db.open())
        qDebug()<<("打开数据库成功！");
    else {
        qDebug()<<("打开数据库失败！");
        return;
    }
    QSqlQuery query2(db);
    qDebug()<<"open:"<<db.connectionName()<<db.databaseName();
    QVariantList countryList,provinceList,cityList,countyList;//顺序储存id name fid
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
    db.close();//记得关闭 好删除文件
    QMetaObject::invokeMethod(w," addCountryData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(countryList)));
    QMetaObject::invokeMethod(w," addProvinceData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(provinceList)));
    QMetaObject::invokeMethod(w," addCityData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(cityList)));
    QMetaObject::invokeMethod(w," addCountyData",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(countyList)));
}

void FuncC::updateUserInformation(QVariantMap info)
{
    qDebug()<<"updateUserInformation(QVariantMap info)  called";
    QString instructDescription="10 updateUserInformation "+m_myQQ+" writedHeaderSize";//更新封面
    BigFileSocket*updateUserInfoSock=new BigFileSocket();//子线程对象最好不要有父类
    updateUserInfoSock->setInstruct(instructDescription);
    updateUserInfoSock->setIp(ip);
    updateUserInfoSock->setPort(updatePort);
    updateUserInfoSock->setTimeout(30000);//30s超时
    QThread*thread=new QThread();
    updateUserInfoSock->moveToThread(thread);
    thread->start();
    emit updateUserInfoSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,updateUserInfoSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateUserInfoSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
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
    QString instructDescription="11 addFGroup "+m_myQQ+"  writedHeaderSize";//添加分组
    BigFileSocket*addFGroupSock=new BigFileSocket();//子线程对象最好不要有父类
    addFGroupSock->setInstruct(instructDescription);
    addFGroupSock->setIp(ip);
    addFGroupSock->setPort(updatePort);
    addFGroupSock->setTimeout(10000);//10s超时
    QThread*thread=new QThread();
    addFGroupSock->moveToThread(thread);
    thread->start();
    emit  addFGroupSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,addFGroupSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(addFGroupSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
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

void FuncC::addRemoteFriendGroup(QVariantMap obj)
{
    QJsonDocument doc(QJsonObject::fromVariantMap(obj));
    if(doc.isEmpty()){
        qDebug()<<"added a remote group is empty";
        return;
    }
    addRemoteFriendGroup(doc);
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
    connect(exitSock,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);//完成写入
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
            QMetaObject::invokeMethod((QObject*)w,"aboutToQuit",Qt::DirectConnection);//退出程序
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
    QTimer::singleShot(3000,exitSock,[=]()mutable{
        qDebug()<<"timeout 3s,now socket will be deleted";
        exitSock->deleteLater();
        if(w)
            QMetaObject::invokeMethod((QObject*)w,"aboutToQuit",Qt::DirectConnection);
    }) ;//等待1.5秒 至多3s删除套接字
}

void FuncC::getVerifyArray(const QString &myqq,QQuickWindow *qmlWin)
{
    qDebug()<<"getVerifyArray";
    QString instructDescription="13  getVerifyArray "+myqq;//获取远程验证消息列表
    BigFileSocket*getVerifyArrSock=new BigFileSocket();//子线程对象最好不要有父类
    getVerifyArrSock->setInstruct(instructDescription);
    getVerifyArrSock->setIp(ip);
    getVerifyArrSock->setPort(loginPort);
    getVerifyArrSock->setTimeout(15000);//超时15s
    QThread*thread=new QThread(this);
    getVerifyArrSock->moveToThread(thread);
    thread->start();
    emit getVerifyArrSock->start();//转移到新线程去post host
    connect(getVerifyArrSock,&BigFileSocket::finished,getVerifyArrSock,[=](){
        qDebug()<<"verify data is got or timeout";
        QJsonDocument json=QJsonDocument::fromJson(getVerifyArrSock->carrier);
        QVariantMap map=json.object().toVariantMap();
        qDebug()<<map;
        QMetaObject::invokeMethod(qmlWin,"getFVerify",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(map)));
        //超时放弃或完成结束线程
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
        delete addFGWidget,addFGWidget=nullptr;//必须立即删除
    });
    connect(addFGWidget,&AddFriendGroupWidget::emitGroup,this,[=](QString name){
        QMetaObject::invokeMethod((QObject*)qqMainWin,"addFGroup",Qt::DirectConnection,Q_ARG(QString,name));
        QMetaObject::invokeMethod((QObject*)w,"addFGroup",Qt::DirectConnection,Q_ARG(QString,name));
        qDebug()<<"updating a remote group";
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("groupName",QJsonValue(name));
        obj.insert("index",QJsonValue(-1));//末尾前一项添加
        doc.setObject(obj);
        addRemoteFriendGroup(doc);
    });
    //deletelater后置零
    connect(addFGWidget,&AddFriendGroupWidget::setNull,addFGWidget,[=]()mutable{
        addFGWidget=nullptr;
    });
}

void FuncC::handleFVerify(QVariantMap obj)
{
    obj.insert("instruct",QJsonValue("14"));
    obj.insert("content",QJsonValue("FVerify"));
    BigFileSocket*fVerifySock=new BigFileSocket();//子线程对象最好不要有父类
    QJsonObject instruct=QJsonObject::fromVariantMap(obj);
    fVerifySock->setInstruct(instruct);
    fVerifySock->setIp(ip);
    fVerifySock->setPort(loginPort);
    fVerifySock->setTimeout(20000);//20s超时
    QThread*thread=new QThread();
    fVerifySock->moveToThread(thread);
    thread->start();
    emit fVerifySock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,fVerifySock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(fVerifySock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃

    //获取文件结果收尾处理
    connect(fVerifySock,&BigFileSocket::writtenInstruction, fVerifySock,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"handling friend verify thread had exited";
    });

}

void FuncC::updateFGroup(QVariantMap obj)
{
    QString instructDescription="15 "+obj.take("content").toString()+" "+m_myQQ+" writedHeaderSize";//添加分组
    BigFileSocket*updateFGroupSock=new BigFileSocket();//子线程对象最好不要有父类
    updateFGroupSock->setInstruct(instructDescription);
    updateFGroupSock->setIp(ip);
    updateFGroupSock->setPort(updatePort);
    updateFGroupSock->setTimeout(10000);//10s超时
    QThread*thread=new QThread();
    updateFGroupSock->moveToThread(thread);
    thread->start();
    emit  updateFGroupSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,updateFGroupSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(updateFGroupSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
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
        qDebug()<<" thread had exited to move or delete,rename remote group";
    });
}

void FuncC::moveFriend(const int &before, const int &after, const QString &number)
{
    QJsonObject obj;
    obj.insert("type",QJsonValue("move"));
    obj.insert("before",QJsonValue(before));
    obj.insert("after",QJsonValue(after));
    obj.insert("number",QJsonValue(number));
    dmrFriend(obj);
}

void FuncC::deleteFriend(const int&index,const QString &number)
{
    QJsonObject obj;
    obj.insert("type",QJsonValue("delete"));
    obj.insert("index",QJsonValue(index));
    obj.insert("number",QJsonValue(number));
    dmrFriend(obj);
}

void FuncC::alterFTag(QVariantMap map)
{
    QJsonObject obj=QJsonObject::fromVariantMap(map);
    dmrFriend(obj);
}

void FuncC::dmrFriend(QJsonObject &obj)
{
    QString instructDescription="16 dmrFriend "+m_myQQ+" writedHeaderSize";//添加分组
    BigFileSocket*dmrFriendSock=new BigFileSocket();//子线程对象最好不要有父类
    dmrFriendSock->setInstruct(instructDescription);
    dmrFriendSock->setIp(ip);
    dmrFriendSock->setPort(updatePort);
    dmrFriendSock->setTimeout(10000);//10s超时
    QThread*thread=new QThread();
    dmrFriendSock->moveToThread(thread);
    thread->start();
    emit  dmrFriendSock->start();//转移到新线程去post host
    //删除套接字
    connect(thread,&QThread::finished,dmrFriendSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(dmrFriendSock,&BigFileSocket::finished,thread,&QThread::quit);//超时放弃
    //获取文件结果收尾处理
    connect(dmrFriendSock,&BigFileSocket::writtenInstruction,dmrFriendSock,[=](){
        QJsonDocument json(obj);
        QByteArray data=json.toBinaryData();
        if(data.isEmpty()){
            qDebug()<<"a error:data of dispatching is empty ";
            thread->exit(0);
            thread->quit();
            return;
        }
        dmrFriendSock->write(data);
        dmrFriendSock->loop.exec();
        thread->exit(0);
        thread->quit();
        qDebug()<<" thread had exited to move or delete,rename remote group";
    });
}

void FuncC::screenShot()
{
    ScreenWidget*temp=  ScreenWidget::Instance();
    temp->showFullScreen();
    connect(temp,&ScreenWidget::finished,this,[=](){
        QPixmap pix=temp->getPixmap();//数据获取完成
        if(!pix.isNull()){
            emit getScreenPximap(pix);
        }
        disconnect(temp,&ScreenWidget::finished,0,0);//只连一次
    });
}

void FuncC::removeDir(QString path)
{
    QDir dir(path);
    if(dir.exists()){
        if(!dir.removeRecursively()){
            qDebug()<<"it's of failure for removing";
            return;
        }
    }
}

void FuncC::getFIP(QString number)
{
    qDebug()<<"getFIP";
    QJsonObject inObj;
    inObj.insert("instruct",QJsonValue("17"));
    inObj.insert("content",QJsonValue("getFIP"));
    inObj.insert("myqq",QJsonValue(m_myQQ));
    inObj.insert("number",QJsonValue(number.left(number.length()-1)));
    BigFileSocket*getFIPSock=new BigFileSocket();//子线程对象最好不要有父类
    getFIPSock->setInstruct(inObj);

    getFIPSock->setIp(ip);
    getFIPSock->setPort(loginPort);
    getFIPSock->setTimeout(10000);//超时10s
    QThread*thread=new QThread(this);
    getFIPSock->moveToThread(thread);
    //注意：移动线程的对象必须在创建线程外删除 或等待线程事件结束后删除 比如quit线程后
    connect(thread,&QThread::finished,getFIPSock,[=](){
        getFIPSock->moveToThread(qApp->thread());//移动到gui线程
        getFIPSock->deleteLater();//利用槽函数安全删除线程
        thread->deleteLater();
    });//删除线程
    thread->start();
    emit getFIPSock->start();//转移到新线程去post host
    connect(getFIPSock,&BigFileSocket::finished,getFIPSock,[=](){
        qDebug()<<"test opposite status  finish or timeout"<<getFIPSock->carrier;
        QJsonParseError  err;
        QJsonDocument tempDoc=QJsonDocument::fromJson(getFIPSock->carrier,&err);
        if(err.error!=QJsonParseError::NoError){
            qDebug()<<"error:"<<err.errorString();
        }
        if(tempDoc.isObject()){
            QJsonObject info=tempDoc.object();
            QString  status=info.value("status").toString();
            if(info.value("result").toString()=="true"){
                if(info.value("online").toBool()){
                    qDebug()<<"friend is online";
                    QString ip=info.value("ip").toString();
                    QString port=QString("%1").arg(info.value("port").toVariant().toUInt());
                    emit getAddress(ip,port,status);
                    //超时放弃或完成结束线程
                    thread->exit(0);
                    thread->quit();
                    thread->wait();
                    return;
                }
            }
        }else{
            qDebug()<<"data is not correct";
        }
        emit getAddress();
        //超时放弃或完成结束线程
        thread->exit(0);
        thread->quit();
        thread->wait();
    });
}

void FuncC::sendFMessage(QString ip, QString port, QString number,QString html,QQuickWindow*win)
{
    qDebug()<<"start sending message to"<<number;
    QString text=QString();
    QByteArray sendData=parseFHtml(html,text);
    if(sendData.isEmpty()){
        qDebug()<<"warning:send a message is empty";
        QMetaObject::invokeMethod((QObject*)win,"sendResult",Qt::DirectConnection,Q_ARG(bool,false),Q_ARG(QString,text));
        QMetaObject::invokeMethod((QObject*)win,"getInfoListPlainText",Qt::DirectConnection,Q_ARG(QString,QString(sendData)));
        return;
    }

    //发送解析的纯文本到qml win
    QMetaObject::invokeMethod((QObject*)win,"getInfoListPlainText",Qt::DirectConnection,Q_ARG(QString,text));
    SendSocket*sendSock=new SendSocket();
    sendSock->setIP(ip);
    sendSock->setPort((quint16)port.toUInt());
    sendSock->setMessage(sendData);
    sendSock->setTimeout(20000);//超时20s
    sendSock->setNumber(m_myQQ,number);//  我方号码 用于查找对象 ,对方号码 用于接收判断号码是否正确
    QThread*thread=new QThread(this);
    sendSock->moveToThread(thread);

    connect(win,&QQuickWindow::destroyed,sendSock,[=](){
        thread->exit(0);
        thread->quit();
        thread->wait();
    });//关闭则退出线程
    //退出线程则删除对象 注意先把对象在子线程删除或移除才能删除子线程对象，这是qthread的关键点
    connect(thread,&QThread::finished,sendSock,[=](){
        sendSock->moveToThread(qApp->thread());//移动到gui线程
        sendSock->deleteLater();
        thread->deleteLater();
    });
    //开始post
    connect(thread,&QThread::started,sendSock,[=](){
        sendSock->post();
    });
    connect(sendSock,&SendSocket::finished,sendSock,[=](int code,QString descp){
        qDebug()<<code<<descp;
        //失败重连服务器
        if(code<0){
            QJsonObject obj;
            obj.insert("content",QJsonValue("sendFMessage"));
            obj.insert("instruct",QJsonValue("18"));
            obj.insert("myqq",QJsonValue(m_myQQ));
            obj.insert("number",QJsonValue(number));
            obj.insert("headerSize",QJsonValue(true));

            BigFileSocket*twoSendSock=new BigFileSocket();//子线程对象最好不要有父类
            twoSendSock->setInstruct(obj);
            twoSendSock->setIp(this->ip);
            twoSendSock->setPort(updatePort);
            twoSendSock->setTimeout(15000);//超时15s

            QThread*twoThread=new QThread(sendSock);
            twoSendSock->moveToThread(twoThread);
            twoThread->start();
            emit twoSendSock->start();//转移到新线程去post host
            connect(twoThread,&QThread::finished,twoSendSock,[=](){
                twoSendSock->moveToThread(qApp->thread());
                twoThread->deleteLater();
                twoSendSock->deleteLater();
                thread->exit(0);
                thread->quit();
                thread->wait();
            });
            connect(twoSendSock,&BigFileSocket::writtenInstruction,twoSendSock,[=](){
                qDebug()<<"message sending to remote host,data size:"<<sendData.size();
                twoSendSock->write(sendData);
                twoSendSock->loop.exec();
                //删除资源
                /**发送数据时不能用这个关闭，这个死循环会导致数据不能写入
          if(twoSendSock->isOpen()){
              twoSendSock->close();
               while(true){
                   if(twoSendSock->state()==QAbstractSocket::UnconnectedState)break;
                   qDebug()<<"true";
               };//等待关闭
          }
          */
                //注意gui对象如何在子线程通信 千万不要把gui代码运行在子线程
                QMetaObject::invokeMethod((QObject*)win,"sendResult",Qt::BlockingQueuedConnection,Q_ARG(bool,true),Q_ARG(QString,QString(sendData)));
                twoThread->exit(0);
                twoThread->quit();
                twoThread->wait();
            });
            //发送失败
            connect(twoSendSock,&BigFileSocket::finished,twoSendSock,[=](){
                qDebug()<<"message sending failure";


                //注意gui对象如何在子线程通信 千万不要把gui代码运行在子线程
                QMetaObject::invokeMethod((QObject*)win,"sendResult",Qt::BlockingQueuedConnection,Q_ARG(bool,false),Q_ARG(QString,QString(sendData)));
                twoThread->exit(0);
                twoThread->quit();
                twoThread->wait();

            });
        }else{//成功
            //注意gui对象如何在子线程通信 千万不要把gui代码运行在子线程
            QMetaObject::invokeMethod((QObject*)win,"sendResult",Qt::BlockingQueuedConnection,Q_ARG(bool,true),Q_ARG(QString,QString(sendData)));
            thread->exit(0);
            thread->quit();
            thread->wait();
        }
    });
    thread->start();
}

QByteArray FuncC::parseFHtml(QString &html,QString&ptext)
{

    QByteArray sendData;
    QXmlStreamWriter writer(&sendData);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeStartElement("消息");
    writer.writeAttribute("datetime",QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    QRegExp reg("(?:<p.*)(>.*</p>)");
    reg.setMinimal(true);//非贪婪模式
    qint32 pos=reg.indexIn(html);
    if(pos==-1){
        qDebug()<<"document content is not correct to html";
        return sendData;
    }
    QString content=reg.cap(1);//>内容</p>
    QRegularExpression ex("(>)(.*)(<img src=\\\"file:///(.*)\\\".*/)|((.*)</p>)");
    ex.setPatternOptions(QRegularExpression::InvertedGreedinessOption);//非贪婪匹配
    QRegularExpressionMatchIterator iteror=  ex.globalMatch(content);
    pos=0;
    //无效写入全部
    if(!iteror.isValid()){
        QString text=content.mid(1,content.length()-5);
        writer.writeStartElement(QString("消息%1").arg(++pos));
        writer.writeAttribute("type","text");
        writer.writeCharacters(text);
        writer.writeEndElement();
        //纯文本获取
        ptext=text;
    }else
        while(iteror.hasNext()){
            QRegularExpressionMatch item=  iteror.next();
            QString text=item.captured(2);
            QString name=item.captured(4);
            //如果是结尾则匹配的是第三项 但包含着> 不知道为啥匹配的包含第一项
            if(text.isEmpty()&&name.isEmpty()){
                text=item.captured(6);
                text=text.right(text.length()-1);
                //一条文本写入
                writer.writeStartElement(QString("消息%1").arg(++pos));
                writer.writeAttribute("type","text");
                writer.writeCharacters(text);
                writer.writeEndElement();
                //纯文本获取
                ptext.append(text);
                break;//结束获取
            }else if(!name.isEmpty()){
                if(!text.isEmpty()){
                    writer.writeStartElement(QString("消息%1").arg(++pos));
                    writer.writeAttribute("type","text");
                    writer.writeCharacters(text);//图片数据为16进制
                    writer.writeEndElement();
                    //纯文本获取
                    ptext.append(text);
                }
                QPixmap pix;
                pix.load(name);
                if(pix.isNull()){
                    qDebug()<<" a pixmap is null  for saving as infromation";
                    continue;
                }
                QBuffer buf;
                if(!buf.open(QIODevice::ReadWrite)){
                    qDebug()<<"opened a buffer  is of failure for saving as  infromation";
                    continue;
                }
                pix.save(&buf,"png");
                QByteArray data=buf.data();
                writer.writeStartElement(QString("消息%1").arg(++pos));
                writer.writeAttribute("type","pixmap");
                writer.writeCharacters(QString(data.toHex()));//图片数据为16进制
                writer.writeEndElement();
                //纯文本获取
                ptext.append("[图片]");
            }
        }

    writer.writeEndElement();
    writer.writeEndDocument();
    return sendData;
}

void FuncC::loadFChatLog(QQuickWindow *qmlWin, QString number)
{
    QThread*thread=new QThread();
    thread->moveToThread(thread);//线程自己移动到开辟线程
    //子线程点
    connect(thread,&QThread::started,thread,[=](){
        QFile dbFile(QString("../user/%1/friends/%2/chat/_%3.db").arg(m_myQQ,number,number));
        if(dbFile.exists()){
            std::thread::id tid=std::this_thread::get_id();
            std::stringstream sin;
            sin << tid;
            QString tstr =QString::fromStdString(sin.str());
            //打开数据库
            bool ok;
            QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",tstr+m_myQQ+number);
            QDir dir(QString("../user/%1/friends/%2/chat/").arg(m_myQQ,number));
            dir.mkpath("./temp");//制造temp目录
            db.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(m_myQQ,number,number));//打开已存在数据库
            db.setHostName("MyQQ");
            db.setUserName("sa");
            db.setPassword("@123456x");
            if(db.open()){
                qDebug()<<("打开sqliite数据库成功！");
                QSqlQuery query(db);//存在数据库的查询对象
                ok=  query.exec(QString(" select data,type,datetime,adhesion,number from _%1info  order by id desc limit 5000 ").arg(number));//查询最新的5000行记录

                if(ok){
                    query.next();
                    int sum= 0;
                    QString html=QString();
                    QString mq=QString();
                    QString time=QString();
                    int count=0;//记录消息数，让它最多获取500条消息
                    while (query.isValid()) {
                        if(count>500)break;//大于500不在获取
                        QByteArray data=query.value("data").toByteArray();//BLOD
                        QString type=query.value("type").toString();
                        bool adhesion=query.value("adhesion").toBool();
                        qDebug()<<"adhesion"<<adhesion;
                        if(!adhesion){//一条消息处理
                            if(!html.isEmpty()){//有消息则发送
                                //注意gui只能在gui线程的含义 即gui涉及ui的执行代码只能在gui线程，而gui对象及其他操作是可以在子线程执行的
                                //消息接受 注意线程在gui线程 不能为子线程 因为有qml ui 发送参数 h't'm'l内容 发送时间 发送方号码
                                // qmlwin第一个对象是qobject，直接强转即可，无需动态转换
                                //gui操作比需在gui线程 注意线程
                                QMetaObject::invokeMethod((QObject*)qmlWin,"loadLog",Qt::DirectConnection,Q_ARG(QString,html),Q_ARG(QString,time),Q_ARG(QString,mq));
                                html="";
                                ++count;//计数加1
                            }
                            mq=query.value("number").toString();//现在消息的时间
                            time=query.value("datetime").toString();
                            if(type=="text"){
                                html=QString(data)+html;//插入文本
                                qDebug()<<"text1:"<<html;
                            }else if(type=="pixmap"){
                                QPixmap pix;
                                pix.loadFromData(data);
                                QString name=dir.absoluteFilePath("temp/"+QString("%1").arg(QDateTime::currentDateTime().toTime_t())+QString("%1").arg(rand()));
                                ok=  pix.save(name,"png");
                                if(!ok){
                                    qDebug()<<"saved a pixmap is of failure while loading a pixcture chat log！1"<<name;
                                }else{
                                    int width=pix.width(),height=pix.height();
                                    if(width>height){
                                        height=height*1.0/width*290;
                                        width=290;
                                    }else{
                                        width=width*1.0/height*290;
                                        height=290;
                                    }
                                    QString name="<img src=\"file:///"+name+"\" width=\""+QString::number(width)+
                                            "\" height=\""+QString::number(height)+"\" />";
                                    html=name+html;//插图图片
                                    qDebug()<<"pixmap1:"<<html;
                                }
                            }

                        }else{//是片段信息
                            mq=query.value("number").toString();
                            //现在消息的时间
                            time=query.value("datetime").toString();
                            if(type=="text"){
                                html=QString(data)+html;//插入文本
                                qDebug()<<"text2:"<<html;
                            }else if(type=="pixmap"){
                                QPixmap pix;
                                pix.loadFromData(data);
                                QString name=dir.absoluteFilePath("temp/"+QString("%1").arg(QDateTime::currentDateTime().toTime_t())+QString("%1").arg(rand()));
                                ok=  pix.save(name,"png");
                                if(!ok){
                                    qDebug()<<"saved a pixmap is of failure while loading a pixcture chat log！2"<<name;
                                }else{
                                    int width=pix.width(),height=pix.height();
                                    if(width>height){
                                        height=height*1.0/width*290;
                                        width=290;
                                    }else{
                                        width=width*1.0/height*290;
                                        height=290;
                                    }
                                    name="<img src=\"file:///"+name+"\" width=\""+QString::number(width)+
                                            "\" height=\""+QString::number(height)+"\" />";
                                    html=name+html;//插图图片
                                    qDebug()<<"pixmap2:"<<html;
                                }
                            }
                        }
                        query.next();//前进一步
                        ++sum;
                    }
                    //判断是否保留最后一个html
                    if(sum<5000){//小于说明最后一项是完整的消息 发送
                        //注意gui只能在gui线程的含义 即gui涉及ui的执行代码只能在gui线程，而gui对象及其他操作是可以在子线程执行的
                        //消息接受 注意线程在gui线程 不能为子线程 因为有qml ui 发送参数 h't'm'l内容 发送时间 发送方号码
                        // qmlwin第一个对象是qobject，直接强转即可，无需动态转换
                        //gui操作比需在gui线程 注意线程
                        QMetaObject::invokeMethod((QObject*)qmlWin,"loadLog",Qt::DirectConnection,Q_ARG(QString,html),Q_ARG(QString,time),Q_ARG(QString,mq));
                    }
                    db.close();//记得关闭 好删除文件
                    //退出线程
                    thread->exit(0);
                    thread->quit();
                    thread->wait();
                    return;//成功不继续运行
                }else{
                    qDebug()<<"select count(*),data,type,datetime,adhesion,number from _%1info  order by id desc limit 5000 sentence excuting failure:"<<endl<<query.lastError();
                }
                db.close();//记得关闭 好删除文件
            }else{
                qDebug()<<("打开sqliite数据库失败！");
            }
        }else{
            qDebug()<<"not find local database file";
        }
        //退出线程

        thread->exit(0);
        thread->quit();
        thread->wait();
    });
    connect(thread,&QThread::destroyed,this,[=](){
        qDebug()<<"~QThread()";
    });
    connect(thread,&QThread::finished,thread,[=]()mutable{
        thread->moveToThread(this->thread());//移动到gui线程
        thread->deleteLater();//利用槽函数实现线程转移，安全毁灭线程
    });
    connect(qApp,&QApplication::aboutToQuit,thread,[=](){
        thread->exit(0);
        thread->quit();
        thread->wait();
    },Qt::DirectConnection);//改直接连接 强制转线程为gui线程
    thread->start();//开启线程
}

void FuncC::saveSentFLog(QString number,QString bytes)
{
    if(bytes.isEmpty()){
        qDebug()<<"warning:message is empty";
        return;
    }
    QByteArray sendData=bytes.toUtf8();
    //打开数据库
    QDir dir(QString("../user/%1/friends/%2/chat/").arg(m_myQQ).arg(number));
    if(!dir.mkpath("./")){
        qDebug()<<"dir mkpath is of failure";
        return ;
    }
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE",QString("%1").arg(rand())+number);
    db.setDatabaseName(QString("../user/%1/friends/%2/chat/_%3.db").arg(m_myQQ).arg(number,number));
    db.setHostName("MyQQ");
    db.setUserName("sa");
    db.setPassword("@123456x");
    if(db.open())
        qDebug()<<("打开sqliite数据库成功！");
    else {
        qDebug()<<("打开sqliite数据库失败！名字:")<<(db.databaseName());
        return ;
    }
    QSqlQuery query(db);
    bool ok=  query.exec(QString(" SELECT count(*) FROM sqlite_master WHERE type='table' AND name ='_%1info' ").arg(number));
    if(!ok){
        qDebug()<<"count of table query is of failure ";
        db.close();//记得关闭 好删除文件
        return ;
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
                                )").arg(number));
                       if(!ok){
                           qDebug()<<"create a table is of failure";
                           db.close();//记得关闭 好删除文件
                           return ;
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
                            query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(number));
                            query.addBindValue(QVariant(text));
                            query.addBindValue(QVariant(time));
                            query.addBindValue(QVariant(type));
                            query.addBindValue(QVariant(true));
                            query.addBindValue(QVariant(m_myQQ));
                            ok=query.exec();
                            if(!ok){
                                qDebug()<<"type:text;query.exec(\" insert into _%1info(data,datetime,type,adhesion) values(?,?,?,?) \")=false";
                                continue;
                            }
                        }else if(type=="pixmap"){//把16进制转为原数据保存
                            QByteArray pix=QByteArray::fromHex(text.toUtf8());//转化为原数据
                            query.prepare(QString(" insert into _%1info(data,datetime,type,adhesion,number) values(?,?,?,?,?) ").arg(number));
                            query.addBindValue(QVariant(pix));
                            query.addBindValue(QVariant(time));
                            query.addBindValue(QVariant(type));
                            query.addBindValue(QVariant(true));
                            query.addBindValue(QVariant(m_myQQ));
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
    ok= query.exec(QString(" select count(*) from _%1info ").arg(number));
    if(!ok){
        qDebug()<<"query.exec(QString(\" select count(*) from _%1info \").arg(myqq)) is of failure";
        db.close();//记得关闭 好删除文件
        return ;
    }
    try{
        query.next();
        int count=query.value(0).toInt();
        if(count<=0)throw "table is empty";
        ok= query.exec(QString(" update _%1info set adhesion=%2 where id=%3 ").arg(number).arg(false).arg(count));
        if(!ok){
            qDebug()<<" update _%1info set adhesion=%2  failure";
            db.close();//记得关闭 好删除文件
            return ;
        }
    }catch(_exception&e){
        qDebug()<<"a exception:"<<"type is "<<e.type<<"name is"<<e.name;
        db.close();//记得关闭 好删除文件
        return;
    }catch(...){
        qDebug()<<"a unknow excpetion";
        db.close();//记得关闭 好删除文件
        return;
    }
    db.close();//记得关闭 好删除文件
    return;
}

QString FuncC::saveSentFLog(QString bytes)
{
    qDebug()<<"start saveing native log for chating with myself ";
    QString text=QString();
    QByteArray sendData=parseFHtml(bytes,text);
    if(sendData.isEmpty()){
        qDebug()<<"warning:send a message is empty";
        return QString();
    }
    saveSentFLog(m_myQQ,sendData);
    return text;
}




void FuncC::startAddFriendsProcess(QQuickWindow*arg,QMap<QString, QVariant>obj,QList<QVariant>arr)
{

    qDebug()<<"add friends win process started->";
    QObject*parent=dynamic_cast<QObject*>(arg);
 if(addFriendsProcessCount()==0){//控制只有一个进程
    QProcess *myProcess = new QProcess(parent);
    connect(myProcess,&QProcess::readyReadStandardOutput,myProcess,[=](){
        QByteArray data=myProcess->readAllStandardOutput();
        QJsonParseError err;
        data=QString::fromLocal8Bit(data).toUtf8();
        QJsonDocument tempDoc=QJsonDocument::fromJson(data,&err);
        if(err.error!=QJsonParseError::NoError){
            qDebug()<<"error description:"<<err.errorString()<<endl<<data.data();
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
            }else if(instruct=="open fwin"){
                try{
                    qDebug()<<"打开资料界面";
                    QVariantMap map;
                    QString number=obj.value("number").toString();
                    map.insert("number",QVariant(number));
                    QString pix=obj.value("pixmap").toString();
                    images->setPixmap2(number+"1",pix,"1");
                    map.insert("headImg","image://friends/"+number+"1");
                    map.insert("name",obj.value("name").toString());
                    map.insert("signature",obj.value("signature").toString());
                    QMetaObject::invokeMethod(arg,"openFWin",Qt::DirectConnection,Q_ARG(QVariant,QVariant::fromValue(map)));
                }catch(QString&estr){
                    qDebug()<<"warning:"<<estr;
                }
            }
        }else{
            qDebug()<<"process output:"<<data;
        }
    });

    connect(myProcess,&QProcess::readyReadStandardError,myProcess,[=](){
        QByteArray data=myProcess->readAllStandardError();;
        qDebug()<<"stderr:"<<data;
    });

    connect(myProcess,SIGNAL(started()),this,SLOT(handleProcessStarted()));
    //qqmainwin关闭就终止进程
    connect(this,&FuncC::emitCloseMyProcess,myProcess,[=](){
        qDebug()<<"kill  myprocess";
        myProcess->kill();
        myProcess->waitForFinished();
    });
    //finished信号处理函数
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),myProcess,
            [=](int exitCode, QProcess::ExitStatus exitStatus)mutable{
        qDebug()<<"deletion myprocess";
        processCount-=1;
        addSMm.detach();
        if(myProcess)
            delete myProcess,myProcess=nullptr;
    });

        addSMm.setKey("addFriendgroups"+m_myQQ);//添加好友用，一旦更新组，就更新内存 key唯一标识 进程唯一
        qDebug()<<"myprocess key is "<<addSMm.key();
        addSMm.create(1024);
        memset(addSMm.data(),0,1024);//初始化数据
        if(!addSMm.isAttached()){
            qDebug()<<"warning:a unexpected action addSMm is not attached";
            return;
        }
        QJsonDocument sdoc(QJsonArray::fromVariantList(arr));
        QByteArray sdata=sdoc.toJson();
        qDebug()<<sdata.data();
        memccpy(addSMm.data(),sdata.data(),'\0',1024);//数据不超过1024字节 最多15个分组
        //由于windows上不能利用进程输入通道i和子线程通信，所以
        //建立本地的qlocalserver用来监听子进程
        QLocalServer*localServer=new QLocalServer(myProcess);
       localServer->listen("localserver"+m_myQQ);
       //创建qlocalsocket通信
       connect(localServer,&QLocalServer::newConnection,localServer,[=](){
           QLocalSocket *localSocket=localServer->nextPendingConnection();
           //打开添加窗口动作
           connect(this,&FuncC::writeStandartInputToProcess,localSocket,[=](QString number){
               QJsonDocument doc;
               QJsonObject obj;
               qDebug()<<"writeStandartInputToProcess:";
               obj.insert("instruct",QJsonValue("add friend"));
               obj.insert("number",QJsonValue(number));
               doc.setObject(obj);
            localSocket->write(doc.toJson());//写入本地管道或者域套接字
            qDebug()<<doc.toJson().data();
           });
           //检测删除指定号码项
           connect(this,&FuncC::emitDeletionExternalItemOf,localSocket,[=](QString number){
               QJsonDocument doc;
               QJsonObject obj;
               qDebug()<<"emitDeletionExternalItemOf:";
               obj.insert("instruct",QJsonValue("delete item"));
               obj.insert("number",QJsonValue(number));
               doc.setObject(obj);
            localSocket->write(doc.toJson());//写入本地管道或者域套接字
            qDebug()<<doc.toJson().data();
           });
       });

       connect(myProcess,&QProcess::destroyed,myProcess,[=](){
       localServer->close();
       QLocalServer::removeServer("localserver"+m_myQQ);
      //删除辅助进程的头像文件
       QDir dir("../head_images");
       if(dir.exists())dir.removeRecursively();
       });//随进程毁灭而毁灭
        QJsonDocument doc(QJsonObject::fromVariantMap(obj));
        QStringList list;
        list<<doc.toJson();
        //等管道打开后开启进程
        myProcess->start( ("../MyQQExternal/addFriendsWin"),list);
    }
}

unsigned short FuncC::addFriendsProcessCount() const
{
    return processCount;
}

void FuncC::updateAuxiliaryProcessFGoup(QList<QVariant>arr)
{
    if(addSMm.isAttached()){
        QJsonDocument sdoc(QJsonArray::fromVariantList(arr));
        QByteArray sdata=sdoc.toJson();
        if(sdata.isEmpty()){
            qDebug()<<"the friend group data is empty and cannot be updated in the auxiliary process";
            return;
        }
        memset(addSMm.data(),0,1024);//清空数据
        memccpy(addSMm.data(),sdata.data(),'\0',1024);//数据不超过1024字节 最多15个分组
    }
}



void FuncC::analysisWh(QString totalGeoAddr)
{
    //因为网址不行，只能强赋值
    totalGeoAddr=("中国 北京市朝阳区");
    int s1=0,s2=0,s3=0;
    QString cityN="";
    for(int i=0;i<totalGeoAddr.size();i++){
        QString s=totalGeoAddr.data()[i];
        if(s==("省")||s==("市")||s==("区")||
                s==("县")||s==("州"))
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
                qDebug()<<("wea:")<<_3dayWeaAndTem[count][0];
                data=reply->readLine(1024);
                data=reply->readLine(1024);
                QRegularExpression temRe("^(?:<span>)(.+)(?:</span>)(.+)(?:<i>)(.+)(?:</i>)$");
                QRegularExpressionMatch temItr;
                temItr = temRe.match(QString::fromUtf8(data.data()));
                if(temItr.hasMatch()){
                    _3dayWeaAndTem[count][1]=temItr.captured(1)+temItr.captured(2)+temItr.captured(3);
                    qDebug()<<("温度:")<<_3dayWeaAndTem[count][1];
                }
                ++count;
                if(!(count<3)){
                    emit crawWeatherUrlFinished();
                    return ;
                }
            }else if(firtarItr.hasMatch()){
                _3dayWeaAndTem[0][1]=firtarItr.captured(1)+("℃")+"/"+firtarItr.captured(2)+("℃");
                qDebug()<<("天气:")<<_3dayWeaAndTem[0][1];
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
        qDebug()<<QString("存在文件夹'%1'").arg(dirString);
    else{
        bool ok=dir->mkpath(dirString);
        if(!ok)
            qDebug()<<("创建失败");
        else
            qDebug()<<("创建成功");
    }
}

void FuncC::readWeatherFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.exists()){
        qDebug()<<QString("不存在文件:'%1'").arg(fileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<<("创建文件失败");
        }
        file.close();
        return;
    }
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<("打开文件失败");
        return;
    }
    qDebug()<<("打开文件成功");
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
        qDebug()<<("打开文件失败");
        return;
    }
    qDebug()<<("打开文件成功");
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
    BigFileSocket*sendFileSock=new BigFileSocket();//子线程不能有父类
    sendFileSock->setInstruct(instructDescription);
    sendFileSock->setIp(ip);
    sendFileSock->setPort(loginPort);
    sendFileSock->setTimeout(30000);
    QThread*thread=new QThread();
    sendFileSock->moveToThread(thread);
    thread->start();
    emit sendFileSock->start();//转移到新线程去post host
    connect(thread,&QThread::finished,sendFileSock,&BigFileSocket::deleteLater);
    //删除线程
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    //获取文件结果收尾处理
    connect(sendFileSock,&BigFileSocket::finished,[=](int code){
        emit getFileFinished(code);//先发送完成信号更新头像及界面
        if(code==-1){
            qDebug()<<"login code is equal to -1";
        }else{
            QJsonDocument tdoc=QJsonDocument::fromJson(sendFileSock->carrier);
            QVariantMap map=QVariantMap();
            if(tdoc.isObject()){
                map=tdoc.object().toVariantMap();
            }else{
                qDebug()<<"verify message is empty or not corrected";
            }
            emit getReceiveFMessage(sendFileSock->infoList,map);
        }
        thread->exit(0);
        thread->quit();
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
    m_win=nullptr;//重置为0
    emit sourceIcoChanged();
}

void FuncC::setSourceIco(QQuickWindow *win, QString key)
{
    QIcon* ico=new QIcon();//数据载体
    QThread *thread=new QThread();
    thread->moveToThread(thread);
    connect(thread,&QThread::finished,this,[=]()mutable{
        thread->deleteLater();
    });
    connect(thread,&QThread::finished,this,[=]()mutable{
        win->setIcon(*ico);
        delete ico,ico=nullptr;
    });
    //子线程处理数据
    connect(thread,&QThread::started,thread,[=](){
        QPixmap pix=images->provider2->images.value(key);
        if(pix.isNull()){
            qDebug()<<"not found the pixmap:"<<key;
            return;
        }
        QImage img=pix.toImage();
        img= img.convertToFormat(QImage::Format_ARGB32_Premultiplied);//格式转换32位
        int h=img.height(),w=img.width();
        qreal r=w/2.0;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                qreal ti=i-r,tj=j-r;
                if(ti*ti+tj*tj>r*r){
                    img.setPixelColor(j,i,QColor(0,0,0,0));
                }
            }
        }
        pix =QPixmap::fromImage(img);
        ico->addPixmap(pix);

        if(ico->isNull()){
            qDebug()<<"it's of failure for converting pixmap to ico";
            return;
        }

        thread->exit(0);
        thread->quit();
    });
    thread->start();
}


void FuncC::initAllCitys()
{

    allCitys.insert(("北京"),"http://www.weather.com.cn/weather/101010100.shtml");
    allCitys.insert(("石家庄"),"http://www.weather.com.cn/weather/101090101.shtml");
    allCitys.insert(("保定"),	"http://www.weather.com.cn/weather/101090201.shtml");
    allCitys.insert(("张家口"),"http://www.weather.com.cn/weather/101090301.shtml");
    allCitys.insert(("承德"),	"http://www.weather.com.cn/weather/101090401.shtml");
    allCitys.insert(("唐山"),"http://www.weather.com.cn/weather/101090501.shtml");
    allCitys.insert(("廊坊"),"http://www.weather.com.cn/weather/101090601.shtml");
    allCitys.insert(("沧州"),"http://www.weather.com.cn/weather/101090701.shtml");
    allCitys.insert(("衡水"),"http://www.weather.com.cn/weather/101090801.shtml");
    allCitys.insert(("邢台"),"http://www.weather.com.cn/weather/101090901.shtml");
    allCitys.insert(("邯郸"),"http://www.weather.com.cn/weather/101091001.shtml");
    allCitys.insert(("秦皇岛"),"http://www.weather.com.cn/weather/101091101.shtml");
    allCitys.insert(("雄安新区"),"http://www.weather.com.cn/weather/101091201.shtml");
    allCitys.insert(("重庆"),"http://www.weather.com.cn/weather/101040100.shtml");
    allCitys.insert(("呼和浩特"),"http://www.weather.com.cn/weather/101080101.shtml");
    allCitys.insert(("包头"),"http://www.weather.com.cn/weather/101080201.shtml");
    allCitys.insert(("乌海"),"http://www.weather.com.cn/weather/101080301.shtml");
    allCitys.insert(("乌兰察布"),"http://www.weather.com.cn/weather/101080401.shtml");
    allCitys.insert(("通辽"),"http://www.weather.com.cn/weather/101080501.shtml");
    allCitys.insert(("赤峰"),"http://www.weather.com.cn/weather/101080601.shtml");
    allCitys.insert(("鄂尔多斯"),"http://www.weather.com.cn/weather/101080701.shtml");
    allCitys.insert(("巴彦淖尔"),"http://www.weather.com.cn/weather/101080801.shtml");
    allCitys.insert(("锡林郭勒"),"http://www.weather.com.cn/weather/101080901.shtml");
    allCitys.insert(("呼伦贝尔"),"http://www.weather.com.cn/weather/101081001.shtml");
    allCitys.insert(("兴安盟"),"http://www.weather.com.cn/weather/101081101.shtml");
    allCitys.insert(("阿拉善盟"),"http://www.weather.com.cn/weather/101081201.shtml");
    allCitys.insert(("上海"),"http://www.weather.com.cn/weather/101020100.shtml");
    allCitys.insert(("武汉"),"http://www.weather.com.cn/weather/101200101.shtml");
    allCitys.insert(("襄阳"),"http://www.weather.com.cn/weather/101200201.shtml");
    allCitys.insert(("鄂州"),"http://www.weather.com.cn/weather/101200301.shtml");
    allCitys.insert(("孝感"),"http://www.weather.com.cn/weather/101200401.shtml");
    allCitys.insert(("黄冈"),"http://www.weather.com.cn/weather/101200501.shtml");
    allCitys.insert(("黄石"),"http://www.weather.com.cn/weather/101200601.shtml");
    allCitys.insert(("咸宁"),"http://www.weather.com.cn/weather/101200701.shtml");
    allCitys.insert(("荆州"),"http://www.weather.com.cn/weather/101200801.shtml");
    allCitys.insert(("宜昌"),"http://www.weather.com.cn/weather/101200901.shtml");
    allCitys.insert(("恩施"),"http://www.weather.com.cn/weather/101201001.shtml");
    allCitys.insert(("十堰"),"http://www.weather.com.cn/weather/101201101.shtml");
    allCitys.insert(("神农架"),"http://www.weather.com.cn/weather/101201201.shtml");
    allCitys.insert(("随州"),"http://www.weather.com.cn/weather/101201301.shtml");
    allCitys.insert(("荆门"),"http://www.weather.com.cn/weather/101201401.shtml");
    allCitys.insert(("天门"),"http://www.weather.com.cn/weather/101201501.shtml");
    allCitys.insert(("仙桃"),"http://www.weather.com.cn/weather/101201601.shtml");
    allCitys.insert(("潜江"),"http://www.weather.com.cn/weather/101201701.shtml");
    allCitys.insert(("太原"),"http://www.weather.com.cn/weather/101100101.shtml");
    allCitys.insert(("大同"),"http://www.weather.com.cn/weather/101100201.shtml");
    allCitys.insert(("阳泉"),"http://www.weather.com.cn/weather/101100301.shtml");
    allCitys.insert(("晋中"),"http://www.weather.com.cn/weather/101100401.shtml");
    allCitys.insert(("长治"),"http://www.weather.com.cn/weather/101100501.shtml");
    allCitys.insert(("晋城"),"http://www.weather.com.cn/weather/101100601.shtml");
    allCitys.insert(("临汾"),"http://www.weather.com.cn/weather/101100701.shtml");
    allCitys.insert(("运城"),"http://www.weather.com.cn/weather/101100801.shtml");
    allCitys.insert(("朔州"),"http://www.weather.com.cn/weather/101100901.shtml");
    allCitys.insert(("忻州"),"http://www.weather.com.cn/weather/101101001.shtml");
    allCitys.insert(("吕梁"),"http://www.weather.com.cn/weather/101101100.shtml");
    allCitys.insert(("长沙"),"http://www.weather.com.cn/weather/101250101.shtml");
    allCitys.insert(("湘潭"),"http://www.weather.com.cn/weather/101250201.shtml");
    allCitys.insert(("株洲"),"http://www.weather.com.cn/weather/101250301.shtml");
    allCitys.insert(("衡阳"),"http://www.weather.com.cn/weather/101250401.shtml");
    allCitys.insert(("郴州"),"http://www.weather.com.cn/weather/101250501.shtml");
    allCitys.insert(("常德"),"http://www.weather.com.cn/weather/101250601.shtml");
    allCitys.insert(("益阳"),"http://www.weather.com.cn/weather/101250700.shtml");
    allCitys.insert(("娄底"),"http://www.weather.com.cn/weather/101250801.shtml");
    allCitys.insert(("邵阳"),"http://www.weather.com.cn/weather/101250901.shtml");
    allCitys.insert(("岳阳"),"http://www.weather.com.cn/weather/101251001.shtml");
    allCitys.insert(("张家界"),"http://www.weather.com.cn/weather/101251101.shtml");
    allCitys.insert(("怀化"),"http://www.weather.com.cn/weather/101251201.shtml");
    allCitys.insert(("永州"),"http://www.weather.com.cn/weather/101251401.shtml");
    allCitys.insert(("湘西"),"http://www.weather.com.cn/weather/101251501.shtml");
    allCitys.insert(("天津"),"http://www.weather.com.cn/weather/101030100.shtml");
    allCitys.insert(("郑州"),"http://www.weather.com.cn/weather/101180101.shtml");
    allCitys.insert(("安阳"),"http://www.weather.com.cn/weather/101180201.shtml");
    allCitys.insert(("新乡"),"http://www.weather.com.cn/weather/101180301.shtml");
    allCitys.insert(("许昌"),"http://www.weather.com.cn/weather/101180401.shtml");
    allCitys.insert(("平顶山"),"http://www.weather.com.cn/weather/101180501.shtml");
    allCitys.insert(("信阳"),"http://www.weather.com.cn/weather/101180601.shtml");
    allCitys.insert(("南阳"),"http://www.weather.com.cn/weather/101180701.shtml");
    allCitys.insert(("开封"),"http://www.weather.com.cn/weather/101180801.shtml");
    allCitys.insert(("洛阳"),"http://www.weather.com.cn/weather/101180901.shtml");
    allCitys.insert(("商丘"),"http://www.weather.com.cn/weather/101181001.shtml");
    allCitys.insert(("焦作"),"http://www.weather.com.cn/weather/101181101.shtml");
    allCitys.insert(("鹤壁"),"http://www.weather.com.cn/weather/101181201.shtml");
    allCitys.insert(("濮阳"),"http://www.weather.com.cn/weather/101181301.shtml");
    allCitys.insert(("周口"),"http://www.weather.com.cn/weather/101181401.shtml");
    allCitys.insert(("漯河"),"http://www.weather.com.cn/weather/101181501.shtml");
    allCitys.insert(("驻马店"),"http://www.weather.com.cn/weather/101181601.shtml");
    allCitys.insert(("三门峡"),"http://www.weather.com.cn/weather/101181701.shtml");
    allCitys.insert(("济源"),"http://www.weather.com.cn/weather/101181801.shtml");
    allCitys.insert(("合肥"),"http://www.weather.com.cn/weather/101220101.shtml");
    allCitys.insert(("蚌埠"),"http://www.weather.com.cn/weather/101220201.shtml");
    allCitys.insert(("芜湖"),"http://www.weather.com.cn/weather/101220301.shtml");
    allCitys.insert(("淮南"),"http://www.weather.com.cn/weather/101220401.shtml");
    allCitys.insert(("马鞍山"),"http://www.weather.com.cn/weather/101220501.shtml");
    allCitys.insert(("安庆"),"http://www.weather.com.cn/weather/101220601.shtml");
    allCitys.insert(("宿州"),"http://www.weather.com.cn/weather/101220701.shtml");
    allCitys.insert(("阜阳"),"http://www.weather.com.cn/weather/101220801.shtml");
    allCitys.insert(("亳州"),"http://www.weather.com.cn/weather/101220901.shtml");
    allCitys.insert(("黄山"),"http://www.weather.com.cn/weather/101221001.shtml");
    allCitys.insert(("滁州"),"http://www.weather.com.cn/weather/101221101.shtml");
    allCitys.insert(("淮北"),"http://www.weather.com.cn/weather/101221201.shtml");
    allCitys.insert(("铜陵"),"http://www.weather.com.cn/weather/101221301.shtml");
    allCitys.insert(("宣城"),"http://www.weather.com.cn/weather/101221401.shtml");
    allCitys.insert(("六安"),"http://www.weather.com.cn/weather/101221501.shtml");
    allCitys.insert(("池州"),"http://www.weather.com.cn/weather/101221701.shtml");
    allCitys.insert(("贵阳"),"http://www.weather.com.cn/weather/101260101.shtml");
    allCitys.insert(("遵义"),"http://www.weather.com.cn/weather/101260201.shtml");
    allCitys.insert(("安顺"),"http://www.weather.com.cn/weather/101260301.shtml");
    allCitys.insert(("黔南"),"http://www.weather.com.cn/weather/101260401.shtml");
    allCitys.insert(("黔东南"),"http://www.weather.com.cn/weather/101260501.shtml");
    allCitys.insert(("铜仁"),"http://www.weather.com.cn/weather/101260601.shtml");
    allCitys.insert(("毕节"),"http://www.weather.com.cn/weather/101260701.shtml");
    allCitys.insert(("六盘水"),"http://www.weather.com.cn/weather/101260801.shtml");
    allCitys.insert(("黔西南"),"http://www.weather.com.cn/weather/101260901.shtml");
    allCitys.insert(("南京"),"http://www.weather.com.cn/weather/101190101.shtml");
    allCitys.insert(("无锡"),"http://www.weather.com.cn/weather/101190201.shtml");
    allCitys.insert((" 镇江"),"http://www.weather.com.cn/weather/101190301.shtml");
    allCitys.insert(("苏州"),"http://www.weather.com.cn/weather/101190401.shtml");
    allCitys.insert(("南通"),"http://www.weather.com.cn/weather/101190501.shtml");
    allCitys.insert(("扬州"),"http://www.weather.com.cn/weather/101190601.shtml");
    allCitys.insert(("盐城"),"http://www.weather.com.cn/weather/101190701.shtml");
    allCitys.insert(("徐州"),"http://www.weather.com.cn/weather/101190801.shtml");
    allCitys.insert(("淮安"),"http://www.weather.com.cn/weather/101190901.shtml");
    allCitys.insert(("连云港"),"http://www.weather.com.cn/weather/101191001.shtml");
    allCitys.insert(("常州"),"http://www.weather.com.cn/weather/101191101.shtml");
    allCitys.insert(("泰州"),"http://www.weather.com.cn/weather/101191201.shtml");
    allCitys.insert(("宿迁"),"http://www.weather.com.cn/weather/101191301.shtml");
    allCitys.insert(("西安"),"http://www.weather.com.cn/weather/101110101.shtml");
    allCitys.insert(("咸阳"),"http://www.weather.com.cn/weather/101110200.shtml");
    allCitys.insert(("延安"),"http://www.weather.com.cn/weather/101110300.shtml");
    allCitys.insert(("榆林"),"http://www.weather.com.cn/weather/101110401.shtml");
    allCitys.insert(("渭南"),"http://www.weather.com.cn/weather/101110501.shtml");
    allCitys.insert(("商洛"),"http://www.weather.com.cn/weather/101110601.shtml");
    allCitys.insert(("安康"),"http://www.weather.com.cn/weather/101110701.shtml");
    allCitys.insert(("汉中"),"http://www.weather.com.cn/weather/101110801.shtml");
    allCitys.insert(("宝鸡"),"http://www.weather.com.cn/weather/101110901.shtml");
    allCitys.insert(("铜川"),"http://www.weather.com.cn/weather/101111001.shtml");
    allCitys.insert(("杨凌"),"http://www.weather.com.cn/weather/101111101.shtml");
    allCitys.insert(("西宁"),"http://www.weather.com.cn/weather/101150101.shtml");
    allCitys.insert(("海东"),"http://www.weather.com.cn/weather/101150201.shtml");
    allCitys.insert(("黄南"),"http://www.weather.com.cn/weather/101150301.shtml");
    allCitys.insert(("海南"),"http://www.weather.com.cn/weather/101150401.shtml");
    allCitys.insert(("果洛"),"http://www.weather.com.cn/weather/101150501.shtml");
    allCitys.insert(("玉树"),"http://www.weather.com.cn/weather/101150601.shtml");
    allCitys.insert(("海西"),"http://www.weather.com.cn/weather/101150701.shtml");
    allCitys.insert(("海北"),"http://www.weather.com.cn/weather/101150801.shtml");
    allCitys.insert(("济南"),"http://www.weather.com.cn/weather/101120101.shtml");
    allCitys.insert(("青岛"),"http://www.weather.com.cn/weather/101120201.shtml");
    allCitys.insert(("淄博"),"http://www.weather.com.cn/weather/101120301.shtml");
    allCitys.insert(("德州"),"http://www.weather.com.cn/weather/101120401.shtml");
    allCitys.insert(("烟台"),"http://www.weather.com.cn/weather/101120501.shtml");
    allCitys.insert(("潍坊"),"http://www.weather.com.cn/weather/101120601.shtml");
    allCitys.insert(("济宁"),"http://www.weather.com.cn/weather/101120701.shtml");
    allCitys.insert(("泰安"),"http://www.weather.com.cn/weather/101120801.shtml");
    allCitys.insert(("临沂"),"http://www.weather.com.cn/weather/101120901.shtml");
    allCitys.insert(("菏泽"),"http://www.weather.com.cn/weather/101121001.shtml");
    allCitys.insert(("滨州"),"http://www.weather.com.cn/weather/101121101.shtml");
    allCitys.insert(("东营"),"http://www.weather.com.cn/weather/101121201.shtml");
    allCitys.insert(("威海"),"http://www.weather.com.cn/weather/101121301.shtml");
    allCitys.insert(("枣庄"),"http://www.weather.com.cn/weather/101121401.shtml");
    allCitys.insert(("日照"),"http://www.weather.com.cn/weather/101121501.shtml");
    allCitys.insert(("莱芜"),"http://www.weather.com.cn/weather/101121601.shtml");
    allCitys.insert(("聊城"),"http://www.weather.com.cn/weather/101121701.shtml");
    allCitys.insert(("杭州"),"http://www.weather.com.cn/weather/101210101.shtml");
    allCitys.insert(("湖州"),"http://www.weather.com.cn/weather/101210201.shtml");
    allCitys.insert(("嘉兴"),"http://www.weather.com.cn/weather/101210301.shtml");
    allCitys.insert(("宁波"),"http://www.weather.com.cn/weather/101210401.shtml");
    allCitys.insert(("绍兴"),"http://www.weather.com.cn/weather/101210501.shtml");
    allCitys.insert(("台州"),"http://www.weather.com.cn/weather/101210601.shtml");
    allCitys.insert(("温州"),"http://www.weather.com.cn/weather/101210701.shtml");
    allCitys.insert(("丽水"),"http://www.weather.com.cn/weather/101210801.shtml");
    allCitys.insert(("金华"),"http://www.weather.com.cn/weather/101210901.shtml");
    allCitys.insert(("衢州"),"http://www.weather.com.cn/weather/101211001.shtml");
    allCitys.insert(("舟山"),"http://www.weather.com.cn/weather/101211101.shtml");
    allCitys.insert(("银川"),"http://www.weather.com.cn/weather/101170101.shtml");
    allCitys.insert(("石嘴山"),"http://www.weather.com.cn/weather/101170201.shtml");
    allCitys.insert(("吴忠"),"http://www.weather.com.cn/weather/101170301.shtml");
    allCitys.insert(("固原"),"http://www.weather.com.cn/weather/101170401.shtml");
    allCitys.insert(("中卫"),"http://www.weather.com.cn/weather/101170501.shtml");
    allCitys.insert(("拉萨"),"http://www.weather.com.cn/weather/101140101.shtml");
    allCitys.insert(("日喀则"),"http://www.weather.com.cn/weather/101140201.shtml");
    allCitys.insert(("山南"),"http://www.weather.com.cn/weather/101140301.shtml");
    allCitys.insert(("林芝"),"http://www.weather.com.cn/weather/101140401.shtml");
    allCitys.insert(("昌都"),"http://www.weather.com.cn/weather/101140501.shtml");
    allCitys.insert(("那曲"),"http://www.weather.com.cn/weather/101140601.shtml");
    allCitys.insert(("阿里"),"http://www.weather.com.cn/weather/101140701.shtml");
    allCitys.insert(("昆明"),"http://www.weather.com.cn/weather/101290101.shtml");
    allCitys.insert(("大理"),"http://www.weather.com.cn/weather/101290201.shtml");
    allCitys.insert(("红河"),"http://www.weather.com.cn/weather/101290301.shtml");
    allCitys.insert(("曲靖"),"http://www.weather.com.cn/weather/101290401.shtml");
    allCitys.insert(("保山"),"http://www.weather.com.cn/weather/101290501.shtml");
    allCitys.insert(("文山"),"http://www.weather.com.cn/weather/101290601.shtml");
    allCitys.insert(("玉溪"),"http://www.weather.com.cn/weather/101290701.shtml");
    allCitys.insert(("楚雄"),"http://www.weather.com.cn/weather/101290801.shtml");
    allCitys.insert(("普洱"),"http://www.weather.com.cn/weather/101290901.shtml");
    allCitys.insert(("昭通"),"http://www.weather.com.cn/weather/101291001.shtml");
    allCitys.insert(("临沧"),"http://www.weather.com.cn/weather/101291101.shtml");
    allCitys.insert(("怒江"),"http://www.weather.com.cn/weather/101291201.shtml");
    allCitys.insert(("迪庆"),"http://www.weather.com.cn/weather/101291301.shtml");
    allCitys.insert(("丽江"),"http://www.weather.com.cn/weather/101291401.shtml");
    allCitys.insert(("德宏"),"http://www.weather.com.cn/weather/101291501.shtml");
    allCitys.insert(("西双版纳"),"http://www.weather.com.cn/weather/101291601.shtml");
    allCitys.insert(("福州"),"http://www.weather.com.cn/weather/101230101.shtml");
    allCitys.insert(("厦门"),"http://www.weather.com.cn/weather/101230201.shtml");
    allCitys.insert(("宁德"),"http://www.weather.com.cn/weather/101230301.shtml");
    allCitys.insert(("莆田"),"http://www.weather.com.cn/weather/101230401.shtml");
    allCitys.insert(("泉州"),"http://www.weather.com.cn/weather/101230501.shtml");
    allCitys.insert(("漳州"),"http://www.weather.com.cn/weather/101230601.shtml");
    allCitys.insert(("龙岩"),"http://www.weather.com.cn/weather/101230701.shtml");
    allCitys.insert(("三明"),"http://www.weather.com.cn/weather/101230801.shtml");
    allCitys.insert(("南平"),"http://www.weather.com.cn/weather/101230901.shtml");
    allCitys.insert(("钓鱼岛"),"http://www.weather.com.cn/weather/101231001.shtml");
    allCitys.insert(("香港"),"http://www.weather.com.cn/weather/101320101.shtml");
    allCitys.insert(("兰州"),"http://www.weather.com.cn/weather/101160101.shtml");
    allCitys.insert(("定西"),"http://www.weather.com.cn/weather/101160201.shtml");
    allCitys.insert(("平凉"),"http://www.weather.com.cn/weather/101160301.shtml");
    allCitys.insert(("庆阳"),"http://www.weather.com.cn/weather/101160401.shtml");
    allCitys.insert(("武威"),"http://www.weather.com.cn/weather/101160501.shtml");
    allCitys.insert(("金昌"),"http://www.weather.com.cn/weather/101160601.shtml");
    allCitys.insert(("张掖"),"http://www.weather.com.cn/weather/101160701.shtml");
    allCitys.insert(("酒泉"),"http://www.weather.com.cn/weather/101160801.shtml");
    allCitys.insert(("天水"),"http://www.weather.com.cn/weather/101160901.shtml");
    allCitys.insert(("陇南"),"http://www.weather.com.cn/weather/101161001.shtml");
    allCitys.insert(("临夏"),"http://www.weather.com.cn/weather/101161101.shtml");
    allCitys.insert(("甘南"),"http://www.weather.com.cn/weather/101161201.shtml");
    allCitys.insert(("白银"),"http://www.weather.com.cn/weather/101161301.shtml");
    allCitys.insert(("嘉峪关"),"http://www.weather.com.cn/weather/101161401.shtml");
    allCitys.insert(("台北"),"http://www.weather.com.cn/weather/101340101.shtml");
    allCitys.insert(("高雄"),"http://www.weather.com.cn/weather/101340201.shtml");
    allCitys.insert(("台中"),"http://www.weather.com.cn/weather/101340401.shtml");
    allCitys.insert(("乌鲁木齐"),"http://www.weather.com.cn/weather/101130101.shtml");
    allCitys.insert(("克拉玛依"),"http://www.weather.com.cn/weather/101130201.shtml");
    allCitys.insert(("石河子"),"http://www.weather.com.cn/weather/101130301.shtml");
    allCitys.insert(("昌吉"),"http://www.weather.com.cn/weather/101130401.shtml");
    allCitys.insert(("吐鲁番"),"http://www.weather.com.cn/weather/101130501.shtml");
    allCitys.insert(("巴音郭楞"),"http://www.weather.com.cn/weather/101130601.shtml");
    allCitys.insert(("阿拉尔"),"http://www.weather.com.cn/weather/101130701.shtml");
    allCitys.insert(("阿克苏"),"http://www.weather.com.cn/weather/101130801.shtml");
    allCitys.insert(("喀什"),"http://www.weather.com.cn/weather/101130901.shtml");
    allCitys.insert(("伊犁"),"http://www.weather.com.cn/weather/101131001.shtml");
    allCitys.insert(("塔城"),"http://www.weather.com.cn/weather/101131101.shtml");
    allCitys.insert(("哈密"),"http://www.weather.com.cn/weather/101131201.shtml");
    allCitys.insert(("和田"),"http://www.weather.com.cn/weather/101131301.shtml");
    allCitys.insert(("阿勒泰"),"http://www.weather.com.cn/weather/101131401.shtml");
    allCitys.insert(("克州"),"http://www.weather.com.cn/weather/101131501.shtml");
    allCitys.insert(("博尔塔拉"),"http://www.weather.com.cn/weather/101131601.shtml");
    allCitys.insert(("图木舒克"),"http://www.weather.com.cn/weather/101131701.shtml");
    allCitys.insert(("五家渠"),"http://www.weather.com.cn/weather/101131801.shtml");
    allCitys.insert(("铁门关"),"http://www.weather.com.cn/weather/101131901.shtml");
    allCitys.insert(("北屯"),"http://www.weather.com.cn/weather/101132101.shtml");
    allCitys.insert(("双河"),"http://www.weather.com.cn/weather/101132201.shtml");
    allCitys.insert(("可克达拉"),"http://www.weather.com.cn/weather/101132301.shtml");
    allCitys.insert(("南宁"),"http://www.weather.com.cn/weather/101300101.shtml");
    allCitys.insert(("崇左"),"http://www.weather.com.cn/weather/101300201.shtml");
    allCitys.insert(("柳州"),"http://www.weather.com.cn/weather/101300301.shtml");
    allCitys.insert(("来宾"),"http://www.weather.com.cn/weather/101300401.shtml");
    allCitys.insert(("桂林"),"http://www.weather.com.cn/weather/101300501.shtml");
    allCitys.insert(("梧州"),"http://www.weather.com.cn/weather/101300601.shtml");
    allCitys.insert(("贺州"),"http://www.weather.com.cn/weather/101300701.shtml");
    allCitys.insert(("贵港"),"http://www.weather.com.cn/weather/101300801.shtml");
    allCitys.insert(("玉林"),"http://www.weather.com.cn/weather/101300901.shtml");
    allCitys.insert(("百色"),"http://www.weather.com.cn/weather/101301001.shtml");
    allCitys.insert(("钦州"),"http://www.weather.com.cn/weather/101301101.shtml");
    allCitys.insert(("河池"),"http://www.weather.com.cn/weather/101301201.shtml");
    allCitys.insert(("北海"),"http://www.weather.com.cn/weather/101301301.shtml");
    allCitys.insert(("防城港"),"http://www.weather.com.cn/weather/101301401.shtml");
    allCitys.insert(("澳门"),"http://www.weather.com.cn/weather/101330101.shtml");
    allCitys.insert(("广州"),"http://www.weather.com.cn/weather/101280101.shtml");
    allCitys.insert(("韶关"),"http://www.weather.com.cn/weather/101280201.shtml");
    allCitys.insert(("惠州"),"http://www.weather.com.cn/weather/101280301.shtml");
    allCitys.insert(("梅州"),"http://www.weather.com.cn/weather/101280401.shtml");
    allCitys.insert(("汕头"),"http://www.weather.com.cn/weather/101280501.shtml");
    allCitys.insert(("深圳"),"http://www.weather.com.cn/weather/101280601.shtml");
    allCitys.insert(("珠海"),"http://www.weather.com.cn/weather/101280701.shtml");
    allCitys.insert(("佛山"),"http://www.weather.com.cn/weather/101280800.shtml");
    allCitys.insert(("肇庆"),"http://www.weather.com.cn/weather/101280901.shtml");
    allCitys.insert(("湛江"),"http://www.weather.com.cn/weather/101281001.shtml");
    allCitys.insert(("江门"),"http://www.weather.com.cn/weather/101281101.shtml");
    allCitys.insert(("河源"),"http://www.weather.com.cn/weather/101281201.shtml");
    allCitys.insert(("清远"),"http://www.weather.com.cn/weather/101281301.shtml");
    allCitys.insert(("云浮"),"http://www.weather.com.cn/weather/101281401.shtml");
    allCitys.insert(("潮州"),"http://www.weather.com.cn/weather/101281501.shtml");
    allCitys.insert(("东莞"),"http://www.weather.com.cn/weather/101281601.shtml");
    allCitys.insert(("中山"),"http://www.weather.com.cn/weather/101281701.shtml");
    allCitys.insert(("阳江"),"http://www.weather.com.cn/weather/101281801.shtml");
    allCitys.insert(("揭阳"),"http://www.weather.com.cn/weather/101281901.shtml");
    allCitys.insert(("茂名"),"http://www.weather.com.cn/weather/101282001.shtml");
    allCitys.insert(("汕尾"),"http://www.weather.com.cn/weather/101282101.shtml");
    allCitys.insert(("海口"),"http://www.weather.com.cn/weather/101310101.shtml");
    allCitys.insert(("三亚"),"http://www.weather.com.cn/weather/101310201.shtml");
    allCitys.insert(("东方"),"http://www.weather.com.cn/weather/101310202.shtml");
    allCitys.insert(("临高"),"http://www.weather.com.cn/weather/101310203.shtml");
    allCitys.insert(("澄迈"),"http://www.weather.com.cn/weather/101310204.shtml");
    allCitys.insert(("儋州"),"http://www.weather.com.cn/weather/101310205.shtml");
    allCitys.insert(("昌江"),"http://www.weather.com.cn/weather/101310206.shtml");
    allCitys.insert(("白沙"),"http://www.weather.com.cn/weather/101310207.shtml");
    allCitys.insert(("琼中"),"http://www.weather.com.cn/weather/101310208.shtml");
    allCitys.insert(("定安"),"http://www.weather.com.cn/weather/101310209.shtml");
    allCitys.insert(("屯昌"),"http://www.weather.com.cn/weather/101310210.shtml");
    allCitys.insert(("琼海"),"http://www.weather.com.cn/weather/101310211.shtml");
    allCitys.insert(("文昌"),"http://www.weather.com.cn/weather/101310212.shtml");
    allCitys.insert(("保亭"),"http://www.weather.com.cn/weather/101310214.shtml");
    allCitys.insert(("万宁"),"http://www.weather.com.cn/weather/101310215.shtml");
    allCitys.insert(("陵水"),"http://www.weather.com.cn/weather/101310216.shtml");
    allCitys.insert(("乐东"),"http://www.weather.com.cn/weather/101310221.shtml");
    allCitys.insert(("五指山"),"http://www.weather.com.cn/weather/101310222.shtml");
    allCitys.insert(("西沙"),"http://www.weather.com.cn/weather/101310302.shtml");
    allCitys.insert(("中沙"),"http://www.weather.com.cn/weather/101310303.shtml");
    allCitys.insert(("南沙"),"http://www.weather.com.cn/weather/101310304.shtml");
    allCitys.insert(("沈阳"),"http://www.weather.com.cn/weather/101070101.shtml");
    allCitys.insert(("大连"),"http://www.weather.com.cn/weather/101070201.shtml");
    allCitys.insert(("鞍山"),"http://www.weather.com.cn/weather/101070301.shtml");
    allCitys.insert(("抚顺"),"http://www.weather.com.cn/weather/101070401.shtml");
    allCitys.insert(("本溪"),"http://www.weather.com.cn/weather/101070501.shtml");
    allCitys.insert(("丹东"),"http://www.weather.com.cn/weather/101070601.shtml");
    allCitys.insert(("锦州"),"http://www.weather.com.cn/weather/101070701.shtml");
    allCitys.insert(("营口"),"http://www.weather.com.cn/weather/101070801.shtml");
    allCitys.insert(("阜新"),"http://www.weather.com.cn/weather/101070901.shtml");
    allCitys.insert(("辽阳"),"http://www.weather.com.cn/weather/101071001.shtml");
    allCitys.insert(("铁岭"),"http://www.weather.com.cn/weather/101071101.shtml");
    allCitys.insert(("朝阳"),"http://www.weather.com.cn/weather/101071201.shtml");
    allCitys.insert(("盘锦"),"http://www.weather.com.cn/weather/101071301.shtml");
    allCitys.insert(("葫芦岛"),"http://www.weather.com.cn/weather/101071401.shtml");
    allCitys.insert(("成都"),"http://www.weather.com.cn/weather/101270101.shtml");
    allCitys.insert(("攀枝花"),"http://www.weather.com.cn/weather/101270201.shtml");
    allCitys.insert(("自贡"),"http://www.weather.com.cn/weather/101270301.shtml");
    allCitys.insert(("绵阳"),"http://www.weather.com.cn/weather/101270401.shtml");
    allCitys.insert(("南充"),"http://www.weather.com.cn/weather/101270501.shtml");
    allCitys.insert(("达州"),"http://www.weather.com.cn/weather/101270601.shtml");
    allCitys.insert(("遂宁"),"http://www.weather.com.cn/weather/101270701.shtml");
    allCitys.insert(("广安"),"http://www.weather.com.cn/weather/101270801.shtml");
    allCitys.insert(("巴中"),"http://www.weather.com.cn/weather/101270901.shtml");
    allCitys.insert(("泸州"),"http://www.weather.com.cn/weather/101271001.shtml");
    allCitys.insert(("宜宾"),"http://www.weather.com.cn/weather/101271101.shtml");
    allCitys.insert(("内江"),"http://www.weather.com.cn/weather/101271201.shtml");
    allCitys.insert(("资阳"),"http://www.weather.com.cn/weather/101271301.shtml");
    allCitys.insert(("乐山"),"http://www.weather.com.cn/weather/101271401.shtml");
    allCitys.insert(("眉山"),"http://www.weather.com.cn/weather/101271501.shtml");
    allCitys.insert(("凉山"),"http://www.weather.com.cn/weather/101271601.shtml");
    allCitys.insert(("雅安"),"http://www.weather.com.cn/weather/101271701.shtml");
    allCitys.insert(("甘孜"),"http://www.weather.com.cn/weather/101271801.shtml");
    allCitys.insert(("阿坝"),"http://www.weather.com.cn/weather/101271901.shtml");
    allCitys.insert(("德阳"),"http://www.weather.com.cn/weather/101272001.shtml");
    allCitys.insert(("广元"),"http://www.weather.com.cn/weather/101272101.shtml");
    allCitys.insert(("哈尔滨"),"http://www.weather.com.cn/weather/101050101.shtml");
    allCitys.insert(("齐齐哈尔"),"http://www.weather.com.cn/weather/101050201.shtml");
    allCitys.insert(("牡丹江"),"http://www.weather.com.cn/weather/101050301.shtml");
    allCitys.insert(("佳木斯"),"http://www.weather.com.cn/weather/101050401.shtml");
    allCitys.insert(("绥化"),"http://www.weather.com.cn/weather/101050501.shtml");
    allCitys.insert(("黑河"),"http://www.weather.com.cn/weather/101050601.shtml");
    allCitys.insert(("大兴安岭"),"http://www.weather.com.cn/weather/101050701.shtml");
    allCitys.insert(("伊春"),"http://www.weather.com.cn/weather/101050801.shtml");
    allCitys.insert(("大庆"),"http://www.weather.com.cn/weather/101050901.shtml");
    allCitys.insert(("七台河"),"http://www.weather.com.cn/weather/101051001.shtml");
    allCitys.insert(("鸡西"),"http://www.weather.com.cn/weather/101051101.shtml");
    allCitys.insert(("鹤岗"),"http://www.weather.com.cn/weather/101051201.shtml");
    allCitys.insert(("双鸭山"),"http://www.weather.com.cn/weather/101051301.shtml");
    allCitys.insert(("长春"),"http://www.weather.com.cn/weather/101060101.shtml");
    allCitys.insert((" 吉林"),"http://www.weather.com.cn/weather/101060201.shtml");
    allCitys.insert(("延边"),"http://www.weather.com.cn/weather/101060301.shtml");
    allCitys.insert(("四平"),"http://www.weather.com.cn/weather/101060401.shtml");
    allCitys.insert(("通化"),"http://www.weather.com.cn/weather/101060501.shtml");
    allCitys.insert(("白城"),"http://www.weather.com.cn/weather/101060601.shtml");
    allCitys.insert(("辽源"),"http://www.weather.com.cn/weather/101060701.shtml");
    allCitys.insert(("松原"),"http://www.weather.com.cn/weather/101060801.shtml");
    allCitys.insert(("白山"),"http://www.weather.com.cn/weather/101060901.shtml");
    allCitys.insert(("南昌"),"http://www.weather.com.cn/weather/101240101.shtml");
    allCitys.insert(("九江"),"http://www.weather.com.cn/weather/101240201.shtml");
    allCitys.insert(("上饶 "),"http://www.weather.com.cn/weather/101240301.shtml");
    allCitys.insert(("抚州 "),"http://www.weather.com.cn/weather/101240401.shtml");
    allCitys.insert(("宜春"),"http://www.weather.com.cn/weather/101240501.shtml");
    allCitys.insert(("吉安"),"http://www.weather.com.cn/weather/101240601.shtml");
    allCitys.insert(("赣州"),"http://www.weather.com.cn/weather/101240701.shtml");
    allCitys.insert(("景德镇"),"http://www.weather.com.cn/weather/101240801.shtml");
    allCitys.insert(("萍乡"),"http://www.weather.com.cn/weather/101240901.shtml");
    allCitys.insert(("新余"),"http://www.weather.com.cn/weather/101241001.shtml");
    allCitys.insert(("鹰潭"),"http://www.weather.com.cn/weather/101241101.shtml");
}
