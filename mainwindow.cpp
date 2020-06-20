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
using namespace std ;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<"addFriendsWin.exe would is execuated";
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);//����͸������
    TcpSocket::setBeginId("0");
    where=QStringLiteral("����");
    hometown=QStringLiteral("����");
    sex=QStringLiteral("");
    host="127.0.0.1",port=5567;
    ui->userListStackWindget->setCurrentIndex(0);
    userView=new UserView(ui->userListStackWindget->currentWidget());
    userModel=new FindUserModel(this);
    userView->setModel(userModel);
    connect(userView,&UserView::continueGetList,this,&MainWindow::continueAddFriendsList);
    myqq="10010";
    hasSeachCBoxShow=false;
    isNeedDisabled1=false;
    isNeedDisabled2=false;
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //������Ӱ����
    shadow->setOffset(0, 0);
    //������Ӱ��ɫ
    shadow->setColor(QColor("gray"));
    //������ӰԲ��
    shadow->setBlurRadius(5);
    ui->frame->setGraphicsEffect(shadow);

    setWindowTitle(QStringLiteral("����"));
    setWindowIcon(QIcon(":./MyQQ.ico"));
    titlebar=new titleBar(ui->bodyWidget);//������
    connect(titlebar,SIGNAL(minBtnClicked()),this,SLOT(minBtnClicked()));
    connect(titlebar,SIGNAL(closeBtnClicked()),this,SLOT(closeBtnClicked()));

    findPersonBtn=new FindBtn(ui->frame1);
    findPersonBtn->setChecked(true);
    findPersonBtn->setText(QStringLiteral("����"));
    findPersonBtn->move(320,0);
    findGroupBtn=new FindBtn(ui->frame1);
    findGroupBtn->setChecked(false);
    findGroupBtn->setText(QStringLiteral("��Ⱥ"));
    findGroupBtn->move(434,0);
    ui->cityPop1->setVisible(false);//���ڵس��пؼ������ɼ�
    ui->cityPop2->setVisible(false);//������пؼ������ɼ�
    ui->whereCBox->setText(QStringLiteral("���ڵأ�����"));
    ui->hometownCBox->setText(QStringLiteral("���磺����"));
    ui->whereCBox->lineEdit()->setPlaceholderText(QStringLiteral("���ڵ�"));
    ui->hometownCBox->lineEdit()->setPlaceholderText(QStringLiteral("����"));
    QApplication::instance()->installEventFilter(this);//���������¼�
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
    //tcpsocket
    socket=new TcpSocket(this);
    TcpSocket::setMyqq(this->myqq);
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("3"));
    obj.insert("content",QJsonValue("city-data"));
    socket->setInstruct(obj);
    socket->connectToHost(host,port);//���ӵ�¼�ķ�����
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectFailed(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(failed()),this,SLOT(deleteToSock()));
    connect(socket,SIGNAL(getCityModel()),this,SLOT(initCityModel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMyQQInfo(const QJsonDocument &json)
{

    QJsonObject obj=json.object();
    if(obj.isEmpty()||!json.isObject()){
        qDebug()<<" parameter  not passed about user's information. ";
        return;
    }
    myqq=obj.value("myqq").toString();
    sex=obj.value("sex").toString();
    where=obj.value(QStringLiteral("���ڵ�")).toString();
    hometown=obj.value(QStringLiteral("����")).toString();
    if(where.isEmpty())where=QStringLiteral("����");
    if(hometown.isEmpty())hometown=QStringLiteral("����");
    ui->textBrowser->setText(myqq+"\n"+sex+"\n"+where+"\n"+hometown);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonRelease){
        //�������Щ�ؼ������û�з�Ӧ
        if(watched==ui->cityPop1||watched==ui->cityPop2||watched==ui->whereSub1||watched==ui->whereSub2||
                watched==ui->whereSub3||watched==ui->whereSub4||watched==ui->hometownSub1||watched==ui->hometownSub2||
                watched==ui->hometownSub3||watched==ui->hometownSub4){
            return true;
            //����������ڵؿؼ��ڵ�����͵���ź�
        }else if(watched==ui->whereCBox||watched==ui->whereCBox->lineEdit()){
            emit ui->whereCBox->clicked();
            return true;
            //������ڹ���ؼ��ڵ�����͵���ź�
        }else if(watched==ui->hometownCBox||watched==ui->hometownCBox->lineEdit()){
            emit ui->hometownCBox->clicked();
            return true;
            //�������λ�ñ༭�ؼ�������ر�citypop1 citypop2
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
    //��Ϊû�ж���QComboBox�ĵ�����ͼ��ʹ�õ����ⲿ�����������ֶ��޸���ʽ
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
        //��Ϊû�ж���QComboBox�ĵ�����ͼ��ʹ�õ����ⲿ�����������ֶ��޸���ʽ
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
    //��Ϊû�ж���QComboBox�ĵ�����ͼ��ʹ�õ����ⲿ�����������ֶ��޸���ʽ
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
        qDebug()<<QStringLiteral("���ӱ��ܽӻ�ʱ��")<<endl;
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<QStringLiteral("Զ�������ѹرգ�")<<endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<QStringLiteral("����û�з��֣�")<<endl;
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<QStringLiteral("δ֪����")<<endl;
        break;
    default:
        qDebug()<<QStringLiteral("�����˴���")<<endl;
    }
    QMessageBox* mes=new QMessageBox(this);
    //mes->resize(800,800);
    mes->setText(QStringLiteral("              �����´�                 "));
    mes->setWindowTitle(QStringLiteral("����ʧ��"));
    QPushButton*btn=new QPushButton;
    btn->setText(QStringLiteral("ȷ��"));
    mes->addButton(btn,QMessageBox::AcceptRole);
    mes->exec();
    connect(mes,&QMessageBox::finished,btn,&QPushButton::deleteLater);
    connect(mes,&QMessageBox::finished,mes,&QMessageBox::deleteLater);
}

void MainWindow::initCityModel()
{
    PlaceModel* countryModel1=new PlaceModel();
    PlaceModel* countryModel2=new PlaceModel();
    PlaceModel* provinceModel1=new PlaceModel();
    PlaceModel* provinceModel2=new PlaceModel();
    PlaceModel* cityModel1=new PlaceModel();
    PlaceModel* cityModel2=new PlaceModel();
    PlaceModel* countyModel1=new PlaceModel();
    PlaceModel* countyModel2=new PlaceModel();
    QStringList list=socket->cityData.split(";;",QString::SkipEmptyParts);
    QStringList list1=list.at(0).split(";",QString::SkipEmptyParts);
    for(int i=0;i<list1.length();i++){
        QStringList listSub=list1.at(i).split(" ",QString::SkipEmptyParts);
        if(listSub.length()==1){
            listSub<<QStringLiteral("����");
        };
        countryModel1->append(listSub.at(0).toLongLong(),listSub.at(1));
        countryModel2->append(listSub.at(0).toLongLong(),listSub.at(1));
    }
    QStringList list2=list.at(1).split(";",QString::SkipEmptyParts);
    for(int i=0;i<list2.length();i++){
        QStringList listSub=list2.at(i).split(" ",QString::SkipEmptyParts);
        if(listSub.length()==2){
            QStringList temp;
            temp<<"0"<<QStringLiteral("����")<<"0";
            listSub=temp;
        }
        provinceModel1->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
        provinceModel2->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
    }
    QStringList list3=list.at(2).split(";",QString::SkipEmptyParts);
    for(int i=0;i<list3.length();i++){
        QStringList listSub=list3.at(i).split(" ",QString::SkipEmptyParts);
        if(listSub.length()==2){
            QStringList temp;
            temp<<"0"<<QStringLiteral("����")<<"0";
            listSub=temp;
        }
        cityModel1->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
        cityModel2->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
    }
    QStringList list4=list.at(3).split(";",QString::SkipEmptyParts);
    for(int i=0;i<list4.length();i++){
        QStringList listSub=list4.at(i).split(" ",QString::SkipEmptyParts);
        if(listSub.length()==2){
            QStringList temp;
            temp<<"0"<<QStringLiteral("����")<<"0";
            listSub=temp;
        }
        countyModel1->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
        countyModel2->append(listSub.at(0).toLongLong(),listSub.at(1),listSub.at(2).toLongLong());
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
    deleteToSock();
}

void MainWindow::deleteToSock()
{
    socket->close();
    socket->waitForDisconnected();
    qDebug()<<"deleteToSock function is viaed"<<(socket);
    socket->deleteLater();//�ڵ�ǰ�¼�Ȧ����ɾ��
    socket=nullptr;//��0ֵ�������Ƚ�ɾ��ָ��
}
//��ɲ��Ҳ۴���
void MainWindow::getAddFriendsList()
{
    QJsonDocument& doc=socket->jsonDoc;
    QJsonArray array=  doc.array();
    QList<FindUserData*>list;
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
                                               obj.value("location2").toString(),obj.value("location3").toString(),obj.value("location4").toString());
            list.append(data);
            file.close();
        }else
            qDebug()<<"readed img unsuccessfully";
    }
    qDebug()<<"count::"<<list.count();
    QModelIndex index=userModel->lastItemIndex();
    if(!index.isValid())index=userModel->index(0,0);
    else index=userModel->index(index.row(),index.column()+1);//���֮��
    if(list.count()==20)userView->setCanContinue(true);//���Լ�����ȡlist
    else userView->setCanContinue(false);//û�ж������Լ������
    userModel->insert(index.row(),index.column(),list);
    qDebug()<<"start setmodel"<<TcpSocket::beginId()<<userModel->sum();
    ui->userListStackWindget->setCurrentIndex(0);
    socket->disconnectFromHost();//�Ͽ�����
    socket->jsonDoc=QJsonDocument();//����б�
    if(userModel->sum()==0){//����ж������Ƿ�Ϊ��
        ui->userListStackWindget->setCurrentIndex(2);//��ʾʲô��û���ѵ�
    }
}

void MainWindow::continueAddFriendsList()
{
    if(!socket)return;//��ָ�뷵��
    QJsonObject& obj=socket->getInstruct();//����ֵʱ�����������Ա�������
    obj.remove("begin-id");
    QJsonValue v=QJsonValue::fromVariant(TcpSocket::beginId());
    obj.insert("begin-id",v);
    qDebug()<<"add?";
    socket->connectToHost(host,port);
}



void MainWindow::on_whereSub1_activated(const QString &arg1)
{
    if(arg1==QStringLiteral("����")){
        ui->whereSub2->setCurrentIndex(0);
        ui->whereSub2->setDisabled(true);
        ui->whereSub3->setCurrentIndex(0);
        ui->whereSub3->setDisabled(true);
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub2->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText());
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
    ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText());
}


void MainWindow::on_whereSub2_activated(const QString &arg1)
{
    if(arg1==QStringLiteral("����")){
        ui->whereSub3->setCurrentIndex(0);
        ui->whereSub3->setDisabled(true);
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        isNeedDisabled1=false;
        ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText());
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
    ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText()+","+
                           ui->whereSub2->currentText());
}

void MainWindow::on_whereSub3_activated(const QString &arg1)
{
    if(arg1==QStringLiteral("����")){
        ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText()+","+
                               ui->whereSub2->currentText());
        if(isNeedDisabled1)return;
        ui->whereSub4->setCurrentIndex(0);
        ui->whereSub4->setDisabled(true);
        ui->whereSub4->setStyleSheet(SubComboBox::DisabledRole);
        return;
    }
    ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText()+","+
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
    ui->whereCBox->setText(QStringLiteral("���ڵأ�")+ui->whereSub1->currentText()+","+
                           ui->whereSub2->currentText()+","+ui->whereSub3->currentText()+
                           ","+arg1);
}

void MainWindow::on_hometownSub1_activated(const QString &arg1)
{
    if(arg1==QStringLiteral("����")){
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
        ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText());
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
    ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText());
}

void MainWindow::on_hometownSub2_activated(const QString &arg1)
{
    if(arg1==QStringLiteral("����")){
        ui->hometownSub3->setCurrentIndex(0);
        ui->hometownSub3->setDisabled(true);
        ui->hometownSub4->setCurrentIndex(0);
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub3->setStyleSheet(SubComboBox::DisabledRole);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        isNeedDisabled2=false;
        ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText());
        return;
    }
    ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText()+","+
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

    if(arg1==QStringLiteral("����")){
        ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText()+","+
                                  ui->hometownSub2->currentText());
        if(isNeedDisabled2)return;
        ui->hometownSub4->setCurrentIndex(0);
        ui->hometownSub4->setDisabled(true);
        ui->hometownSub4->setStyleSheet(SubComboBox::DisabledRole);
        return;
    }
    ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText()+","+
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
    ui->hometownCBox->setText(QStringLiteral("���磺")+ui->hometownSub1->currentText()+","+
                              ui->hometownSub2->currentText()+","+ui->hometownSub3->currentText()+
                              ","+arg1);
}


void MainWindow::returnLabelClicked()
{
    ui->findPersonStackedWidget->setCurrentIndex(0);//����ǰһҳ
}



//����Ѱ�Ҵ���
void MainWindow::on_findBtn_clicked()
{
    ui->userListStackWindget->setCurrentIndex(1);//��ʾ����ҳ
    ui->findPersonStackedWidget->setCurrentIndex(1);//ת������ҳ
    if(socket){
        delete socket,socket=nullptr;
    }
    userModel->clear();
    QString str=QStringLiteral("������");
    QJsonObject instruct;
    QString edit=ui->findPersonEdit->text();
    if(!edit.isEmpty()){
        str.append(edit);
    }
    userView->setCanContinue(false);//�ڲ�ѯ�ڼ䲻�ɼ�����ȡ�û�list
    TcpSocket::setBeginId("10001");//��ʼ����С�û�ID
    instruct.insert("instruct",QJsonValue("3"));
    instruct.insert("content",QJsonValue("find-person"));
    instruct.insert("myqq",QJsonValue(this->myqq));
    instruct.insert("begin-id",QJsonValue(TcpSocket::beginId()));
    instruct.insert("name",QJsonValue(edit));
    instruct.insert("online",QJsonValue(ui->liveCBox->isChecked()));
    int sid=ui->sexCBox->currentIndex();
    if(sid!=0&&sid!=-1)
        str.append(QStringLiteral(" | ")+ui->sexCBox->currentText());
    instruct.insert("sex",QJsonValue(ui->sexCBox->currentText()));
    str.append(QStringLiteral(" | ")+ui->whereCBox->text());//��ӱ����ѡ���ı����
    str.append(QStringLiteral(" | ")+ui->hometownCBox->text());
    instruct.insert("location",QJsonValue(ui->whereCBox->text().right(ui->whereCBox->text().length()-4)));
    instruct.insert("hometown",QJsonValue(ui->hometownCBox->text().right(ui->hometownCBox->text().length()-3)));
    int aid=ui->ageCBox->currentIndex();
    if(aid!=0&&aid!=-1)
        str.append(QStringLiteral(" | ")+ui->ageCBox->currentText());
    instruct.insert("age",QJsonValue(ui->ageCBox->currentIndex()));
    ui->findPersonInfLabel->setText(str);
    qDebug()<<instruct;
    socket=new TcpSocket(this);
    socket->setInstruct(instruct);
    connect(socket,QOverload<QAbstractSocket::SocketError>::of(&TcpSocket::error),[=](QAbstractSocket::SocketError code){
        switch (code) {
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<QStringLiteral("���ӱ��ܽӻ�ʱ��")<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            qDebug()<<QStringLiteral("Զ�������ѹرգ�")<<endl;
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<QStringLiteral("����û�з��֣�")<<endl;
            break;
        case QAbstractSocket::UnknownSocketError:
            qDebug()<<QStringLiteral("δ֪����")<<endl;
            break;
        default:
            qDebug()<<QStringLiteral("�����˴���")<<endl;
        }
        ui->userListStackWindget->setCurrentIndex(2);//��ʾû��������
        deleteToSock();
    });
    connect(socket,&TcpSocket::failed,[=](){
        ui->userListStackWindget->setCurrentIndex(2);//��ʾû��������
        deleteToSock();//ɾ��ָ��
    });
    connect(socket,&TcpSocket::finished,this,&MainWindow::getAddFriendsList);//������ɲ��ҵĲ�
    socket->connectToHost(host,port);//���ӵ�¼�ķ�����
}

void MainWindow::on_comboBox_editTextChanged(const QString &arg1)
{
    TcpSocket::setBeginId(arg1);
}
//ͬ�ǽ��Ѱ�ť���������ڵ�һ�µ��û�
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
//ͬ�����簴ť��������������ڵ�ƥ����û�
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
    QString _1=QStringLiteral("��");
    QString _2=QStringLiteral("Ů");
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
