#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"userview.h"
#include"tcpsocket.h"
#include<QColor>
#include <QListView>
#include<qdebug.h>
#include<qlayout.h>
#include <qmessagebox.h>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include <qjsonarray.h>
#include <QGraphicsDropShadowEffect>
#include <qlistwidget.h>
#include<qtimer.h>
#include<qsocketnotifier.h>
#include <qsqldatabase.h>
#include<qfileinfo.h>
#include <qsqlquery.h>
#include<iostream>
#include <qdir.h>
using namespace std ;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket=nullptr;
    qDebug()<<"addFriendsWin.exe would is execuated";
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置透明背景
    TcpSocket::setBeginId("0");
    where=("不限");
    hometown=("不限");
    sex=("");
    host="127.0.0.1",port=5567;
isSeach=true;//默认为是点击搜索标志

    ui->userListStackWindget->setCurrentIndex(0);
    userView=new UserView(ui->userListStackWindget->currentWidget());
    userModel=new FindUserModel(this);//自动释放
    userView->setModel(userModel);
    connect(userView,&UserView::continueGetList,this,&MainWindow::continueAddFriendsList);//不满20继续添加
    connect(userView,&UserView::addButtonClicked,this,QOverload<const QModelIndex>::of(&MainWindow::showAddFriend));//添加好友界面
    connect(userView,&UserView::imgClicked,this,&MainWindow::openPersonalData);//查看资料界面
    connect(userView,&UserView::nameClicked,this,&MainWindow::openPersonalData);//查看资料界面
    myqq="10001";
    hasSeachCBoxShow=false;
    isNeedDisabled1=false;
    isNeedDisabled2=false;
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("gray"));
    //设置阴影圆角
    shadow->setBlurRadius(5);
    ui->frame->setGraphicsEffect(shadow);

    setWindowTitle(("查找"));
    setWindowIcon(QIcon(":./MyQQ.ico"));
    titlebar=new titleBar(ui->bodyWidget);//标题栏
    connect(titlebar,SIGNAL(minBtnClicked()),this,SLOT(minBtnClicked()));
    connect(titlebar,SIGNAL(closeBtnClicked()),this,SLOT(closeBtnClicked()));

    findPersonBtn=new FindBtn(ui->frame1);
    findPersonBtn->setChecked(true);
    findPersonBtn->setText(("找人"));
    findPersonBtn->move(320,0);
    findGroupBtn=new FindBtn(ui->frame1);
    findGroupBtn->setChecked(false);
    findGroupBtn->setText(("找群"));
    findGroupBtn->move(434,0);
    ui->cityPop1->setVisible(false);//所在地城市控件集不可见
    ui->cityPop2->setVisible(false);//故乡城市控件集不可见
    ui->whereCBox->setText(("所在地：不限"));
    ui->hometownCBox->setText(("故乡：不限"));
    ui->whereCBox->lineEdit()->setPlaceholderText(("所在地"));
    ui->hometownCBox->lineEdit()->setPlaceholderText(("故乡"));
    QApplication::instance()->installEventFilter(this);//过滤所有事件
    connect(findGroupBtn,SIGNAL(clicked(bool)),this,SLOT(showGroupPage()));
    connect(findPersonBtn,SIGNAL(clicked(bool)),this,SLOT(showPersonPage()));
    connect(ui->sexLabel,SIGNAL(clicked()),this,SLOT(sexPopup()));
    connect(ui->sexCBox,SIGNAL(clicked()),this,SLOT(sexPopup()));
    connect(ui->sexCBox,SIGNAL(emitHidePopup()),this,SLOT(sexHidePopup()));
    connect(ui->ageCBox,SIGNAL(clicked()),this,SLOT(agePopup()));
    connect(ui->ageCBox,SIGNAL(emitHidePopup()),this,SLOT(ageHidePopup()));
    connect(ui->ageLabel,SIGNAL(clicked()),this,SLOT(agePopup()));
    connect(ui->whereCBox,SIGNAL(clicked()),this,SLOT(whereCBoxPopup()));
    connect(ui->whereCBox,SIGNAL(emitHidePopup()),this,SLOT(whereCBoxHidePopup()));
    connect(ui->hometownCBox,SIGNAL(clicked()),this,SLOT(hometownCBoxPopup()));
    connect(ui->hometownCBox,SIGNAL(emitHidePopup()),this,SLOT(hometownCBoxHidePopup()));

    connect(ui->returnLabel,SIGNAL(clicked()),this,SLOT(returnLabelClicked()));

    //城市数据加载
    initCityModel();
    TcpSocket::setMyqq(myqq);



}

MainWindow::~MainWindow()
{
    delete ui;

    for (AddFriendsWidget* w:listWidget) {
        w->close();//关闭
    }
    listWidget.clear();
    qDebug()<<"~MainWindow()";
}

void MainWindow::setMyQQInfo(const QJsonDocument &json)
{

    QJsonObject obj=json.object();
    if(obj.isEmpty()||!json.isObject()){
        qDebug()<<"adding friends process parameter  not passed about user's information. ";
        return;
    }
    myqq=obj.value("myqq").toString();
    sex=obj.value("sex").toString();
    where=obj.value(("所在地")).toString();
    hometown=obj.value(("故乡")).toString();
    if(where.isEmpty())where=("不限");
    if(hometown.isEmpty())hometown=("不限");
    //管道设置 用于主进程通信
    localSocket=new QLocalSocket(this);
    connect(localSocket,&QLocalSocket::readyRead,this,&MainWindow::readFromMainProcess);
    connect(localSocket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),this,[=](QLocalSocket::LocalSocketError err){
        cerr<<"local socket occured a error!error code is "<<err;
    });
    //连接本地server
    localSocket->connectToServer("localserver"+myqq,QIODevice::ReadOnly);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonRelease){
        //如果在这些控件点击则没有反应
        if(watched==ui->cityPop1||watched==ui->cityPop2||watched==ui->whereSub1||watched==ui->whereSub2||
                watched==ui->whereSub3||watched==ui->whereSub4||watched==ui->hometownSub1||watched==ui->hometownSub2||
                watched==ui->hometownSub3||watched==ui->hometownSub4){
            return true;
            //如果是在所在地控件内点击则发送点击信号
        }else if(watched==ui->whereCBox||watched==ui->whereCBox->lineEdit()){
            emit ui->whereCBox->clicked();
            return true;
            //如果是在故乡控件内点击则发送点击信号
        }else if(watched==ui->hometownCBox||watched==ui->hometownCBox->lineEdit()){
            emit ui->hometownCBox->clicked();
            return true;
            //如果是在位置编辑控件外点击则关闭citypop1 citypop2
        }else if(watched==ui->findPersonEdit||watched==ui->findBtn||watched==ui->findPerson||watched==ui->friendBtn
                 ||watched==ui->townsmanBtn||watched==ui->liveCBox||watched==ui->frame1||watched==findGroupBtn||watched==findPersonBtn){
            if(ui->cityPop1->isVisible()){
                ui->whereCBox->hidePopup();
            }if(ui->cityPop2->isVisible()){
                ui->hometownCBox->hidePopup();
            }
        }
        return false;
    }
    if(watched==this){
        if(event->type()==QEvent::Hide){
            if(ui->cityPop1->isVisible())
                visibleForCityPop=1;
            else if(ui->cityPop2->isVisible())
                visibleForCityPop=2;
            else visibleForCityPop=0;
            return true;
        }else if(event->type()==QEvent::Show){
            switch (visibleForCityPop) {
            case 1:
                ui->cityPop1->setVisible(true);
                break;
            case 2:
                ui->cityPop2->setVisible(true);
                break;
            default:
                break;
            }
            return true;
        }

    }
    return QMainWindow::eventFilter(watched,event);
}




void MainWindow::closeBtnClicked()
{
    close();
}

void MainWindow::minBtnClicked()
{
    showMinimized();
}

void MainWindow::sexPopup()
{
    if(ui->cityPop1->isVisible()){
        ui->whereCBox->hidePopup();
        return;
    }else if(ui->cityPop2->isVisible()){
        ui->hometownCBox->hidePopup();
        return;
    }
    if((!hasSeachCBoxShow)){
        hasSeachCBoxShow=true;
        ui->sexCBox->showPopup();
    }
}

void MainWindow::sexHidePopup()
{
    hasSeachCBoxShow=false;
}

void MainWindow::agePopup()
{
    if(ui->cityPop1->isVisible()){
        ui->whereCBox->hidePopup();
        return;
    }
    if(ui->cityPop2->isVisible()){
        ui->hometownCBox->hidePopup();
        return;
    }
    if((!hasSeachCBoxShow)){
        ui->ageCBox->showPopup();
        hasSeachCBoxShow=true;
    }
}

void MainWindow::ageHidePopup()
{
    hasSeachCBoxShow=false;
}



void MainWindow::showPersonPage()
{
    if(!findPersonBtn->isChecked()){
        findPersonBtn->setChecked(true);
        return;
    }
    findGroupBtn->setChecked(false);
    ui->mainStackedWidget->setCurrentIndex(0);
}

void MainWindow::showGroupPage()
{
    if(!findGroupBtn->isChecked()){
        findGroupBtn->setChecked(true);
        return;
    }
    findPersonBtn->setChecked(false);
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::hometownCBoxPopup()
{
    if(ui->cityPop1->isVisible()){
        ui->whereCBox->hidePopup();
        return;
    }
    if(ui->cityPop2->isVisible()){
        ui->hometownCBox->hidePopup();
        return;
    }
    if((!hasSeachCBoxShow)){
        ui->hometownCBox->showPopup();
        ui->cityPop2->show();
        ui->hometownCBox->setStyleSheet(" QComboBox {\
                                        border:1px solid rgb(0, 155, 219);\
                border-radius: 5px;\
padding: 0 0 0 5;\
background: white;\
    }\
    QComboBox::down-arrow {\
image:url(:/images/up-arrow.png)\
    }\
    QComboBox::drop-down {\
        subcontrol-origin: 15;\
        subcontrol-position: right;\
width:15px;\
color:red;\
        border-bottom-right-radius: 5px;\
    }\
    QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                               image:url(:/images/up-arrow.png);\
}");
hasSeachCBoxShow=true;
}
}

void MainWindow::hometownCBoxHidePopup()
{
    hasSeachCBoxShow=false;
    ui->cityPop2->hide();
    //因为没有定义QComboBox的弹窗视图，使用的是外部部件，所以手动修改样式
    ui->hometownCBox->setStyleSheet(" QComboBox {\
                                    border:1px solid rgb(0, 155, 219);\
            border-radius: 5px;\
padding: 0 0 0 5;\
background: white;\
}\
QComboBox::down-arrow {\
image:url(:/images/down-arrow.png)\
}\
QComboBox::drop-down {\
    subcontrol-origin: 15;\
    subcontrol-position: right;\
width:15px;\
color:red;\
    border-bottom-right-radius: 5px;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           image:url(:/images/up-arrow.png)\
                           }");
  }

  void MainWindow::whereCBoxPopup()
{
    if(ui->cityPop1->isVisible()){
        ui->whereCBox->hidePopup();
        return;
    }
    if(ui->cityPop2->isVisible()){
        ui->hometownCBox->hidePopup();
        return;
    }
    if((!hasSeachCBoxShow)){
        ui->whereCBox->showPopup();
        ui->cityPop1->show();
        //因为没有定义QComboBox的弹窗视图，使用的是外部部件，所以手动修改样式
        ui->whereCBox->setStyleSheet(" QComboBox {\
                                     border:1px solid rgb(0, 155, 219);\
                border-radius: 5px;\
padding: 0 0 0 5;\
background: white;\
    }\
    QComboBox::down-arrow {\
image:url(:/images/up-arrow.png)\
    }\
    QComboBox::drop-down {\
        subcontrol-origin: 15;\
        subcontrol-position: right;\
width:15px;\
color:red;\
        border-bottom-right-radius: 5px;\
    }\
    QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                               image:url(:/images/up-arrow.png);\
}");
hasSeachCBoxShow=true;
}
}

void MainWindow::whereCBoxHidePopup()
{
    hasSeachCBoxShow=false;
    ui->cityPop1->hide();
    //因为没有定义QComboBox的弹窗视图，使用的是外部部件，所以手动修改样式
    ui->whereCBox->setStyleSheet(" QComboBox {\
                                 border:1px solid rgb(0, 155, 219);\
            border-radius: 5px;\
padding: 0 0 0 5;\
background: white;\
}\
QComboBox::down-arrow {\
image:url(:/images/down-arrow.png)\
}\
QComboBox::drop-down {\
    subcontrol-origin: 15;\
    subcontrol-position: right;\
width:15px;\
color:red;\
    border-bottom-right-radius: 5px;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           image:url(:/images/up-arrow.png)\
                           }");
  }

  void MainWindow::connectFailed(QAbstractSocket::SocketError code)
{
    switch (code) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<("连接被拒接或超时！")<<endl;
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<("远程主机已关闭！")<<endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<("主机没有发现！")<<endl;
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<("未知错误！")<<endl;
        break;
    default:
        qDebug()<<("出现了错误！")<<endl;
    }
    QMessageBox* mes=new QMessageBox(this);
    //mes->resize(800,800);
    mes->setText(("              请重新打开                 "));
    mes->setWindowTitle(("连接失败"));
    QPushButton*btn=new QPushButton;
    btn->setText(("确认"));
    mes->addButton(btn,QMessageBox::AcceptRole);
    mes->exec();
    connect(mes,&QMessageBox::finished,btn,&QPushButton::deleteLater);
    connect(mes,&QMessageBox::finished,mes,&QMessageBox::deleteLater);
}

void MainWindow::initCityModel()
{

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","city_data");
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
    PlaceModel* countryModel1=new PlaceModel();
    PlaceModel* countryModel2=new PlaceModel();
    PlaceModel* provinceModel1=new PlaceModel();
    PlaceModel* provinceModel2=new PlaceModel();
    PlaceModel* cityModel1=new PlaceModel();
    PlaceModel* cityModel2=new PlaceModel();
    PlaceModel* countyModel1=new PlaceModel();
    PlaceModel* countyModel2=new PlaceModel();

    qDebug()<<"open:"<<db.connectionName()<<db.databaseName();
    if(!query2.exec(" begin transaction ")){
        qDebug()<<"begin transaction is of failure";
    }
    if(!query2.exec(" select*from country ")){
        qDebug()<<"warning:city.db is not found";
    }
    query2.next();
    while (query2.isValid()) {
        qint32 id=query2.value("id").toLongLong();
        QString name=query2.value("name").toString();
        if(name.isEmpty())name=("不限");
        countryModel1->append(id,name);
        countryModel2->append(id,name);
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
        if(name.isEmpty())name=("不限");
        provinceModel1->append(id,name,fid);
        provinceModel2->append(id,name,fid);
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
        if(name.isEmpty())name=("不限");
        cityModel1->append(id,name,fid);
        cityModel2->append(id,name,fid);
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
        if(name.isEmpty())name=("不限");
        countyModel1->append(id,name,fid);
        countyModel2->append(id,name,fid);
        query2.next();
    }
    if(!query2.exec(" end transaction ")){
        qDebug()<<"end transaction is of failure";
    }

    qDebug()<<"initCityModel function is viaed";
    ui->whereSub1->setModel(countryModel1);
    ui->whereSub2->setModel(provinceModel1);
    ui->whereSub3->setModel(cityModel1);
    ui->whereSub4->setModel(countyModel1);

    ui->whereSub2->setEnabled(false);
    ui->whereSub3->setEnabled(false);
    ui->whereSub4->setEnabled(false);
    ui->hometownSub1->setModel(countryModel2);
    ui->hometownSub2->setModel(provinceModel2);
    ui->hometownSub3->setModel(cityModel2);
    ui->hometownSub4->setModel(countyModel2);
    ui->hometownSub2->setEnabled(false);
    ui->hometownSub3->setEnabled(false);
    ui->hometownSub4->setEnabled(false);
    ui->whereSub2->setStyleSheet(SubComboBox::DisabledRole);
    ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
    ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
    ui->hometownSub2->setStyleSheet(SubComboBox::DisabledRole);
    ui->hometownSub3->setStyleSheet(SubComboBox::DisabledRole);
    ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
}

void MainWindow::deleteToSock()
{
    socket->close();
    socket->waitForDisconnected();
    qDebug()<<"deleteToSock function is viaed"<<(socket);
    socket->deleteLater();//在当前事件圈立即删除
    socket=nullptr;//赋0值方便后面比较删除指针
}
//完成查找槽处理
void MainWindow::getAddFriendsList()
{
    QJsonDocument& doc=socket->jsonDoc;
    QJsonArray array=  doc.array();
    if(isSeach){
    QList<FindUserData* >list;
    for (QJsonValue v : array) {
        QJsonObject obj=v.toObject();
        if(obj.isEmpty()){
            qDebug()<<"json object has a value of null";
            continue;
        }
        QFile file("./images/"+QString("%1").arg(obj.value("myqq").toVariant().toLongLong())+".png");
        if(file.open(QIODevice::ReadOnly)){
            QByteArray img;img.resize(file.size());
            file.read(img.data(),file.size());
            FindUserData*data=new FindUserData(QString("%1").arg(obj.value("myqq").toVariant().toLongLong()),obj.value("myqqName").toString(),
                                               img,obj.value("age").toString(),obj.value("sex").toString(),obj.value("location1").toString(),
                                               obj.value("location2").toString(),obj.value("location3").toString(),obj.value("location4").toString(),
                                               obj.value("signature").toString());
            list.append(data);
            file.close();
        }else
            qDebug()<<"readed img unsuccessfully";
    }
    qDebug()<<"count::"<<list.count();
    QModelIndex index=userModel->lastItemIndex();
    if(!index.isValid())index=userModel->index(0,0);
    else index=userModel->index(index.row(),index.column()+1);//最后之后
    if(list.count()==20)userView->setCanContinue(true);//可以继续获取list
    else userView->setCanContinue(false);//没有东西可以继续获得
    userModel->insert(index.row(),index.column(),list);
    list.clear();//清空list
    qDebug()<<"start setmodel"<<TcpSocket::beginId()<<userModel->sum();
    ui->userListStackWindget->setCurrentIndex(0);
    socket->disconnectFromHost();//断开连接
    socket->jsonDoc=QJsonDocument();//清空列表
    if(userModel->sum()==0){//最后判断搜索是否为空
        ui->userListStackWindget->setCurrentIndex(2);//提示什么都没有搜到
    }
    }else{
       if(array.size()>0){
          QJsonObject obj=array.at(0).toObject();
          if(!obj.isEmpty()){
              showAddFriend(obj);
          }
       }
    }
 isSeach=true;//重置标记
 userModel->index(1);
}

void MainWindow::continueAddFriendsList()
{
    if(!socket)return;//空指针返回
    QJsonObject& obj=socket->getInstruct();//返回值时个变量，可以别名引用
    obj.remove("begin-id");
    QJsonValue v=QJsonValue::fromVariant(TcpSocket::beginId());
    obj.insert("begin-id",v);
    qDebug()<<"add?";
    socket->connectToHost(host,port);
}

void MainWindow::showAddFriend(const QModelIndex index)
{
    qDebug()<<"showAddFriend";
    QString temp=  userModel->data(index,FindUserModel::MyqqRole).toString();
    qint32 length=listWidget.length();
    for (int var = 0; var < length; ++var) {
        AddFriendsWidget*tempW=listWidget.at(var);
        if(tempW->myqq==temp){
            tempW->show();
            tempW->raise();
            tempW->activateWindow();
            return;
        }
    }

    AddFriendsWidget*w=new AddFriendsWidget;//自动中间位
    listWidget.append(w);
    w->myqq=temp;
    temp=userModel->data(index,FindUserModel::NameRole).toString();
    w->name=temp;
    temp=userModel->data(index,FindUserModel::SexRole).toString();
    w->sex=temp;
    temp=userModel->data(index,FindUserModel::AgeRole).toString();
    w->age=temp;
    temp=userModel->data(index,FindUserModel::SecondLevelRegionRole).toString()+" "+userModel->data(index,FindUserModel::ThirdLevelRegionRole).toString();
    w->location=temp;
    QByteArray pix=userModel->data(index,FindUserModel::HeadImgRole).toByteArray();
    w->pixData=pix;
    temp=userModel->data(index,FindUserModel::SignatureRole).toString();
    w->signature=temp;
    w->show();
    w->inint(myqq);
    connect(w,&AddFriendsWidget::emitClosed,this,[=]()mutable{
        qDebug()<<"close a widget";
        qint32 length=listWidget.length();
        for (int var = 0; var < length; ++var) {
            AddFriendsWidget*tempW=listWidget.at(var);
            if(tempW->myqq==w->myqq){
               listWidget.removeAt(var);
                break;
            }
        }
        w->deleteLater();
        w=nullptr;
    },Qt::QueuedConnection);//队列连接 等关闭事件处理完成
    connect(w,&AddFriendsWidget::emitVerifyInfo,this,&MainWindow::addFriend);
    //connect(w,&AddFriendsWidget::listMainProcess,this,&MainWindow::addFriend);
}
//当主进程点击添加好友时获取数据打开添加界面
void MainWindow::showAddFriend(const QJsonObject&obj){

    QFile file("./images/"+QString("%1").arg(obj.value("myqq").toVariant().toLongLong())+".png");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray img;img.resize(file.size());
        file.read(img.data(),file.size());

    QString temp= QString("%1").arg(obj.value("myqq").toVariant().toLongLong());
    qint32 length=listWidget.length();
    //再次检测有没有打开添加界面
    for (int var = 0; var < length; ++var) {
        AddFriendsWidget*tempW=listWidget.at(var);
        if(tempW->myqq==temp){
            tempW->show();
            tempW->raise();
            tempW->activateWindow();
            return;
        }
    }
    AddFriendsWidget*w=new AddFriendsWidget;//自动中间位
    listWidget.append(w);
    w->myqq=temp;
    temp=obj.value("myqqName").toString();
    w->name=temp;
    temp=obj.value("sex").toString();
    w->sex=temp;
    temp=obj.value("age").toString();
    w->age=temp;
    temp=obj.value("location2").toString()+" "+obj.value("location3").toString();
    w->location=temp;
    if(img.isEmpty()){
        std::cerr<<"seached a friend's pixmap is empty";
    }
    w->pixData=img;
    temp= obj.value("signature").toString();
    w->signature=temp;
    w->show();
    w->inint(myqq);
    connect(w,&AddFriendsWidget::emitClosed,this,[=]()mutable{
        qDebug()<<"close a widget";
        qint32 length=listWidget.length();
        for (int var = 0; var < length; ++var) {
            AddFriendsWidget*tempW=listWidget.at(var);
            if(tempW->myqq==w->myqq){
               listWidget.removeAt(var);
                break;
            }
        }
        w->deleteLater();
        w=nullptr;
    },Qt::QueuedConnection);//队列连接 等关闭事件处理完成
    connect(w,&AddFriendsWidget::emitVerifyInfo,this,&MainWindow::addFriend);
    }
}
void MainWindow::openPersonalData(const QString mq)
{
    int index=userModel->rowOf(mq);
    if(index==-1)return;
    QString sig=userModel->data(userModel->index(index),FindUserModel::SignatureRole).toString();
    QString name=userModel->data(userModel->index(index),FindUserModel::NameRole).toString();
    QByteArray pixData=userModel->data(userModel->index(index),FindUserModel::HeadImgRole).toByteArray();
    if(pixData.isEmpty()){
        std::cerr<<"pixmap is empty"<<endl;
        return;
    }
    QTextStream out1(stdout);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("open fwin"));//指令 代表要更新分组
    obj.insert("number",QJsonValue(mq));
    obj.insert("signature",QJsonValue(sig));
    //由于图片太大了，不能一次接受，需要阻塞控制，所以直接传路径
    QPixmap pix;
    pix.loadFromData(pixData,"png");
    QDir td("../head_images");
    if(!td.exists())td.mkpath("./");
    pix.save(td.filePath(mq),"png");
    QFileInfo fi(td.filePath(mq));
    obj.insert("pixmap",QJsonValue(fi.absoluteFilePath()));
    obj.insert("name",QJsonValue(name));
    doc.setObject(obj);
    out1<<doc.toJson();//输出到主进程

}

void MainWindow::addFriend(QJsonDocument doc)
{

    QJsonObject obj=doc.object();
    if(obj.isEmpty()){
        cerr<<"sended verify information is lacked";
        return;
    }
    obj.insert("instruct",QJsonValue("12"));//12 添加好友验证备份
    obj.insert("content",QJsonValue("verifyInfo"));
    obj.insert("myqq",QJsonValue(myqq));//自己的
    doc=QJsonDocument();
    doc.setObject(obj);
    QByteArray data=doc.toBinaryData();
    QTcpSocket *verifySock=new QTcpSocket(this);
    QEventLoop* loop=new QEventLoop(verifySock);
    connect(verifySock,&QTcpSocket::connected,[=](){
        qDebug()<<"vrify information will be dipatched";
        verifySock->write(data);
        qDebug()<<"a loop exec";
        loop->exec();
        qDebug()<<"a loop exit";
        verifySock->close();
        verifySock->deleteLater();
    });

    connect(verifySock,&QTcpSocket::bytesWritten,loop,&QEventLoop::quit);
    connect(verifySock,QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),verifySock,[=](QAbstractSocket::SocketError code){
        switch (code) {
        case QAbstractSocket::ConnectionRefusedError:
            cerr<<"连接被拒接或超时！"<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            cerr<<"远程主机已关闭！"<<endl;
            break;
        case QAbstractSocket::HostNotFoundError:
            cerr<<"主机没有发现！"<<endl;
            break;
        case QAbstractSocket::UnknownSocketError:
            cerr<<"未知错误！"<<endl;
            break;
        default:
            cerr<<"出现了错误！"<<endl;
        }
        verifySock->deleteLater();
    });
    verifySock->connectToHost(host,port);
}

void MainWindow::readFromMainProcess()
{
    QByteArray data=localSocket->readAll();//由于数据很小，所以保守估计可以直接一套接收
    QJsonDocument json=QJsonDocument::fromJson(data);
    if(json.isObject()){
        QJsonObject obj=json.object();
        QString in=obj.value("instruct").toString();
        QString number=obj.value("number").toString();
        if(number.isEmpty()){
           cerr<<"parsing a number is not normal";
           return;
        }
        if(in=="add friend"){
            int row=userModel->rowOf(number);
           //如果列表找到则打开
            if(row!=-1){
                QModelIndex index=userModel->index(row);

                showAddFriend(index);
            }else{//远程爬取数据打开添加界面
               //先检测有没有打开添加窗口列表
                qint32 length=listWidget.length();
                for (int var = 0; var < length; ++var) {
                    AddFriendsWidget*tempW=listWidget.at(var);
                    if(tempW->myqq==number){
                        tempW->show();
                        tempW->raise();
                        tempW->activateWindow();
                        return;
                    }
                }
                //获取数据 查找号码
                isSeach=false;
                mq=number;
                this->on_findBtn_clicked();//搜索
            }
        } else if(in=="delete item"){
            int row=userModel->rowOf(number);
           //如果列表找到则打开
            if(row!=-1){
                QModelIndex index=userModel->index(row);
             emit  userModel->removeItem(index.row(),index.column());//移除信号
            }
           //检测添加界面列表，删除匹配项
            row=listWidget.size();
            for (int var = 0; var < row; ++var) {
                AddFriendsWidget*w=listWidget.at(var);
                if(w->myqq==number){
                    listWidget.removeAt(var);
                    w->deleteLater();
                    break;
                }
            }
        }

    }else{
        cerr<<"receiveing a data it's not a json object";
    }

}






void MainWindow::on_whereSub1_activated(const QString &arg1)
{
    if(arg1==("不限")){
        ui->whereSub2->setCurrentIndex(0);
        ui->whereSub2->setDisabled(true);
        ui->whereSub3->setCurrentIndex(0);
        ui->whereSub3->setDisabled(true);
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub2->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText());
        isNeedDisabled1=false;
        return;
    }
    qint64 id=ui->whereSub1->currentData(PlaceModel::IdRole).toLongLong();
    qDebug()<<id<<":id";
    ui->whereSub2->setDisabled(false);
    ui->whereSub2->filterValue(QVariant(id),PlaceModel::FidRole);
    ui->whereSub2->setStyleSheet(SubComboBox::EabledRole);
    ui->whereSub2->setCurrentIndex(0);
    isNeedDisabled1=false;
    ui->whereSub3->setCurrentIndex(0);
    ui->whereSub3->setDisabled(true);
    ui->whereSub4->setCurrentIndex(0);
    ui->whereSub4->setDisabled(true);
    ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
    ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
    ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText());
}


void MainWindow::on_whereSub2_activated(const QString &arg1)
{
    if(arg1==("不限")){
        ui->whereSub3->setCurrentIndex(0);
        ui->whereSub3->setDisabled(true);
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        isNeedDisabled1=false;
        ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText());
        return;
    }
    qint64 id=ui->whereSub2->currentData(PlaceModel::IdRole).toLongLong();
    ui->whereSub3->setDisabled(false);
    ui->whereSub3->filterValue(QVariant(id),PlaceModel::FidRole);
    if(ui->whereSub3->count()==2){
        qint64 idt=ui->whereSub3->data(1,PlaceModel::IdRole).toLongLong();
        ui->whereSub4->filterValue(QVariant(idt),PlaceModel::FidRole);
        ui->whereSub3->setFilterModel(ui->whereSub4->getFilterModel());
        isNeedDisabled1=true;
        qDebug()<<"viaed";
    }else isNeedDisabled1=false;

    ui->whereSub3->setCurrentIndex(0);
    ui->whereSub3->setStyleSheet(SubComboBox::EabledRole);
    ui->whereSub4->setCurrentIndex(0);
    ui->whereSub4->setDisabled(true);
    ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
    ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText()+","+
                           ui->whereSub2->currentText());
}

void MainWindow::on_whereSub3_activated(const QString &arg1)
{
    if(arg1==("不限")){
        ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText()+","+
                               ui->whereSub2->currentText());
        if(isNeedDisabled1)return;
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        return;
    }
    ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText()+","+
                           ui->whereSub2->currentText()+","+ui->whereSub3->currentText());
    if(isNeedDisabled1)return;
    qint64 id=ui->whereSub3->currentData(PlaceModel::IdRole).toLongLong();
    ui->whereSub4->filterValue(QVariant(id),PlaceModel::FidRole);
    ui->whereSub4->setDisabled(false);
    ui->whereSub4->setStyleSheet(SubComboBox::EabledRole);
    ui->whereSub4->setCurrentIndex(0);

}
void MainWindow::on_whereSub4_activated(const QString &arg1)
{
    ui->whereCBox->setText(("所在地：")+ui->whereSub1->currentText()+","+
                           ui->whereSub2->currentText()+","+ui->whereSub3->currentText()+
                           ","+arg1);
}

void MainWindow::on_hometownSub1_activated(const QString &arg1)
{
    if(arg1==("不限")){
        ui->hometownSub2->setCurrentIndex(0);
        ui->hometownSub2->setDisabled(true);
        ui->hometownSub3->setCurrentIndex(0);
        ui->hometownSub3->setDisabled(true);
        ui->hometownSub4->setCurrentIndex(0);
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub2->setStyleSheet(SubComboBox::DisabledRole);
        ui->hometownSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        isNeedDisabled2=false;
        ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText());
        return;
    }
    qint64 id=ui->hometownSub1->currentData(PlaceModel::IdRole).toLongLong();
    qDebug()<<id<<":id";
    ui->hometownSub2->setDisabled(false);
    ui->hometownSub2->filterValue(QVariant(id),PlaceModel::FidRole);
    ui->hometownSub2->setStyleSheet(SubComboBox::EabledRole);
    ui->hometownSub2->setCurrentIndex(0);
    isNeedDisabled2=false;
    ui->hometownSub3->setCurrentIndex(0);
    ui->hometownSub3->setDisabled(true);
    ui->hometownSub4->setCurrentIndex(0);
    ui->hometownSub4->setDisabled(true);
    ui->hometownSub3->setStyleSheet(SubComboBox::DisabledRole);
    ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
    ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText());
}

void MainWindow::on_hometownSub2_activated(const QString &arg1)
{
    if(arg1==("不限")){
        ui->hometownSub3->setCurrentIndex(0);
        ui->hometownSub3->setDisabled(true);
        ui->hometownSub4->setCurrentIndex(0);
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        isNeedDisabled2=false;
        ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText());
        return;
    }
    ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText()+","+
                              ui->hometownSub2->currentText());
    qint64 id=ui->hometownSub2->currentData(PlaceModel::IdRole).toLongLong();
    ui->hometownSub3->filterValue(QVariant(id),PlaceModel::FidRole);
    if(ui->hometownSub3->count()==2){
        qint64 idt=ui->hometownSub3->data(1,PlaceModel::IdRole).toLongLong();
        ui->hometownSub4->filterValue(QVariant(idt),PlaceModel::FidRole);
        ui->hometownSub3->setFilterModel(ui->hometownSub4->getFilterModel());
        isNeedDisabled2=true;
        qDebug()<<"viaed";
    }else isNeedDisabled2=false;

    ui->hometownSub3->setDisabled(false);
    ui->hometownSub3->setCurrentIndex(0);
    ui->hometownSub3->setStyleSheet(SubComboBox::EabledRole);
    ui->hometownSub4->setCurrentIndex(0);
    ui->hometownSub4->setDisabled(true);
    ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
}

void MainWindow::on_hometownSub3_activated(const QString &arg1)
{

    if(arg1==("不限")){
        ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText()+","+
                                  ui->hometownSub2->currentText());
        if(isNeedDisabled2)return;
        ui->hometownSub4->setCurrentIndex(0);
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        return;
    }
    ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText()+","+
                              ui->hometownSub2->currentText()+","+ui->hometownSub3->currentText());
    if(isNeedDisabled2)return;
    qint64 id=ui->hometownSub3->currentData(PlaceModel::IdRole).toLongLong();
    ui->hometownSub4->filterValue(QVariant(id),PlaceModel::FidRole);
    if(ui->hometownSub4->count()==1){
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        ui->hometownSub4->setCurrentIndex(0);
        return;
    }
    ui->hometownSub4->setDisabled(false);
    ui->hometownSub4->setStyleSheet(SubComboBox::EabledRole);
    ui->hometownSub4->setCurrentIndex(0);
}


void MainWindow::on_hometownSub4_activated(const QString &arg1)
{
    ui->hometownCBox->setText(("故乡：")+ui->hometownSub1->currentText()+","+
                              ui->hometownSub2->currentText()+","+ui->hometownSub3->currentText()+
                              ","+arg1);
}


void MainWindow::returnLabelClicked()
{
    ui->findPersonStackedWidget->setCurrentIndex(0);//返回前一页
}



//好友寻找处理
void MainWindow::on_findBtn_clicked()
{
    ui->userListStackWindget->setCurrentIndex(1);//提示加载页
    ui->findPersonStackedWidget->setCurrentIndex(1);//转到查找页
    if(socket){
        delete socket,socket=nullptr;
    }
    userModel->clear();
    QString str=("搜索：");
    QJsonObject instruct;
    QString edit=ui->findPersonEdit->text();
    if(!edit.isEmpty()){
        str.append(edit);
    }
    userView->setCanContinue(false);//在查询期间不可继续获取用户list
    TcpSocket::setBeginId("10001");//初始化最小用户ID
    instruct.insert("instruct",QJsonValue("3"));
    instruct.insert("content",QJsonValue("find-person"));
    instruct.insert("myqq",QJsonValue(this->myqq));
    instruct.insert("begin-id",QJsonValue(TcpSocket::beginId()));
   if(isSeach){
    instruct.insert("name",QJsonValue(edit));
   }else{//指定为指定号码
      instruct.insert("name",QJsonValue(mq));
   }
    instruct.insert("online",QJsonValue(ui->liveCBox->isChecked()));
    int sid=ui->sexCBox->currentIndex();
    if(sid!=0&&sid!=-1)
        str.append((" | ")+ui->sexCBox->currentText());
    instruct.insert("sex",QJsonValue(ui->sexCBox->currentText()));
    str.append((" | ")+ui->whereCBox->text());//添加保存的选择文本组合
    str.append((" | ")+ui->hometownCBox->text());
    instruct.insert("location",QJsonValue(ui->whereCBox->text().right(ui->whereCBox->text().length()-4)));
    instruct.insert("hometown",QJsonValue(ui->hometownCBox->text().right(ui->hometownCBox->text().length()-3)));
    int aid=ui->ageCBox->currentIndex();
    if(aid!=0&&aid!=-1)
        str.append((" | ")+ui->ageCBox->currentText());
    instruct.insert("age",QJsonValue(ui->ageCBox->currentIndex()));
    ui->findPersonInfLabel->setText(str);
    qDebug()<<instruct;
    socket=new TcpSocket(this);
    socket->setInstruct(instruct);
    connect(socket,QOverload<QAbstractSocket::SocketError>::of(&TcpSocket::error),[=](QAbstractSocket::SocketError code){
        switch (code) {
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<("连接被拒接或超时！")<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            qDebug()<<("远程主机已关闭！")<<endl;
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<("主机没有发现！")<<endl;
            break;
        case QAbstractSocket::UnknownSocketError:
            qDebug()<<("未知错误！")<<endl;
            break;
        default:
            qDebug()<<("出现了错误！")<<endl;
        }
        ui->userListStackWindget->setCurrentIndex(2);//提示没有搜索到
        deleteToSock();
         isSeach=true;//重置标记
    });
    connect(socket,&TcpSocket::failed,[=](){
        isSeach=true;//重置标记
        ui->userListStackWindget->setCurrentIndex(2);//提示没有搜索到
        deleteToSock();//删除指针
        TcpSocket::setBeginId("10001");//如果失败，可能是id已经大于最大 重置
    });
    connect(socket,&TcpSocket::finished,this,&MainWindow::getAddFriendsList);//连接完成查找的槽
    socket->connectToHost(host,port);//连接登录的服务器
}


//同城交友按钮：搜索所在地一致的用户
void MainWindow::on_friendBtn_clicked()
{
    ui->findPersonStackedWidget->setCurrentIndex(1);
    ui->userListStackWindget->setCurrentIndex(1);
    QTimer* timer=new QTimer(this);
    timer->setInterval(50);
    timer->setSingleShot(true);
    connect(timer,&QTimer::timeout,this,&MainWindow::handleFriendButton);
    connect(timer,&QTimer::timeout,timer,&QTimer::deleteLater);
    timer->start();
}
//同城老乡按钮：搜索故乡和所在地匹配的用户
void MainWindow::on_townsmanBtn_clicked()
{
    ui->findPersonStackedWidget->setCurrentIndex(1);
    ui->userListStackWindget->setCurrentIndex(1);
    QTimer* timer=new QTimer(this);
    timer->setInterval(50);
    timer->setSingleShot(true);
    connect(timer,&QTimer::timeout,this,&MainWindow::handleTownsmanButton);
    connect(timer,&QTimer::timeout,timer,&QTimer::deleteLater);
    timer->start();
}

void MainWindow::handleTownsmanButton()
{
    QStringList list1=where.split(",",QString::SkipEmptyParts);
    ui->whereSub1->setCurrentText(list1.at(0));
    on_whereSub1_activated(list1.at(0));
    int count1=list1.count();
    if(count1>1){
        if(ui->whereSub2->currentText()!=list1.at(1)){
            ui->whereSub2->setCurrentText(list1.at(1));
            on_whereSub2_activated(list1.at(1));
        }
    }
    if(count1>2){
        if(ui->whereSub3->currentText()!=list1.at(2)){
            ui->whereSub3->setCurrentText(list1.at(2));
            on_whereSub3_activated(list1.at(2));
        }
    }
    if(count1>3){
        if(ui->whereSub4->currentText()!=list1.at(3)){
            ui->whereSub4->setCurrentText(list1.at(3));
            on_whereSub4_activated(list1.at(3));
        }
    }
    QStringList list2=hometown.split(",",QString::SkipEmptyParts);

    ui->hometownSub1->setCurrentText(list2.at(0));
    on_hometownSub1_activated(list2.at(0));
    int count2=list2.count();
    if(count2>1){
        if(ui->hometownSub2->currentText()!=list2.at(1)){
            ui->hometownSub2->setCurrentText(list2.at(1));
        }
    }
    if(count2>2){
        if(ui->hometownSub3->currentText()!=list2.at(2)){
            ui->hometownSub3->setCurrentText(list2.at(2));
            on_hometownSub3_activated(list2.at(2));
        }
    }
    if(count2>3){
        if(ui->hometownSub4->currentText()!=list2.at(3)){
            ui->hometownSub4->setCurrentText(list2.at(3));
            on_hometownSub4_activated(list2.at(3));
        }
    }
    ui->ageCBox->setCurrentIndex(-1);
    ui->sexCBox->setCurrentIndex(-1);
    on_findBtn_clicked();
}

void MainWindow::handleFriendButton()
{
    QStringList list=where.split(",",QString::SkipEmptyParts);
    ui->whereSub1->setCurrentText(list.at(0));
    on_whereSub1_activated(list.at(0));
    int count1=list.count();
    if(count1>1){
        if(ui->whereSub2->currentText()!=list.at(1)){
            ui->whereSub2->setCurrentText(list.at(1));
            on_whereSub2_activated(list.at(1));
        }
    }else if(count1>2){
        if(ui->whereSub3->currentText()!=list.at(2)){
            ui->whereSub3->setCurrentText(list.at(2));
            on_whereSub3_activated(list.at(2));
        }
    }else if(count1>3){
        if(ui->whereSub4->currentText()!=list.at(3)){
            ui->whereSub4->setCurrentText(list.at(3));
            on_whereSub4_activated(list.at(3));
        }
    }
    QString _1=("男");
    QString _2=("女");
    if(_1==sex){
        ui->sexCBox->setCurrentIndex(2);
    } else if(_2==sex){
        ui->sexCBox->setCurrentIndex(1);
    }else{
        ui->sexCBox->setCurrentIndex(-1);
    }
    ui->ageCBox->setCurrentIndex(-1);
    ui->hometownSub1->setCurrentIndex(0);
    ui->hometownCBox->setEditText("");
    on_findBtn_clicked();
}


void MainWindow::on_ageCBox_currentIndexChanged(int index)
{
    if(index==-1){
        ui->ageLabel->show();
    }else{
        ui->ageLabel->hide();
    }
}

void MainWindow::on_sexCBox_currentIndexChanged(int index)
{
    if(index==-1){
        ui->sexLabel->show();
    }else{
        ui->sexLabel->hide();
    }
}
