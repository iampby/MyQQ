#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "addfriendswidget.h"
#include<qdebug.h>
#include <qgraphicseffect.h>
#include<QVBoxLayout>
#include<qtextedit.h>
#include<QTextStream>
#include <qjsonobject.h>
#include<qsharedmemory.h>
#include <qjsonarray.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qdir.h>
//456 355 c8c8c8
AddFriendsWidget::AddFriendsWidget(QWidget *parent, Qt::WindowFlags f)
    :MovedWidget(parent,f)
{
    setAttribute(Qt::WA_QuitOnClose,false);//�رղ��˳�
    setWindowIcon(QIcon(":./MyQQ.ico"));
    childWidget=nullptr;//0�����ж�
    hasClicked=false;
    isLinkedHover=false;
    last=0;
    myqq=QString();
    name=QString();
    sex=QString();
    age=QString();
    location=QString();
    setFixedSize(466,365);
    contentWidget=new QWidget(this);
    contentWidget->resize(456,355);
    contentWidget->move(5,5);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);//����͸������
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //������Ӱ����
    shadow->setOffset(0, 0);
    //������Ӱ��ɫ
    shadow->setColor(QColor(200,200,200));
    //������ӰԲ��
    shadow->setBlurRadius(5);
    contentWidget->setGraphicsEffect(shadow);
    //opaque
    contentWidget->setStyleSheet("QWidget{\
                                 background-color: rgb(255, 255, 255);\
}\
");
setWindowTitle(("�� - ��Ӻ���"));
//����
header=new QFrame(contentWidget);
header->resize(456,30);
header->move(0,0);
header->setStyleSheet("QFrame{\
                      image: url(:/images/addFriendTitle.png);\
} \
");
minBtn=new QPushButton(header);
closeBtn=new QPushButton(header);
minBtn->resize(30,30);
closeBtn->resize(28,30);
minBtn->move(header->width()-58,0);
closeBtn->move(header->width()-28,0);
minBtn->setStyleSheet("QPushButton{\
                      border:0;\
background-color: rgba(255, 255, 255, 0);\
}\
QPushButton:pressed {\
    border-image: url(:/images/addFriendMinPressed.png);\
}\
QPushButton:hover:!pressed{\
                       border-image: url(:/images/addFriendMinHovered.png);\
}");
closeBtn->setStyleSheet("QPushButton{\
                        border:0;\
background-color: rgba(255, 255, 255, 0);\
}\
QPushButton:pressed {\
    border-image: url(:/images/addFriendClosePressed.png);\
}\
QPushButton:hover:!pressed {\
                       border-image: url(:/images/addFriendsCloseHovered.png);\
}");
//�ײ��ؼ�
footer=new QFrame(contentWidget);
footer->resize(456,35);
footer->move(0,contentWidget->height()-35);
footer->setStyleSheet("QFrame{\
                      border:0px\
                      background-color: #d1e5f2;\
} \
");
bottomBtn1=new QPushButton(footer);
bottomBtn2=new QPushButton(footer);
bottomBtn1->resize(69,24);
bottomBtn2->resize(69,24);
bottomBtn1->setStyleSheet("QPushButton{\
                          border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
bottomBtn2->setStyleSheet("QPushButton{\
                          border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
bottomBtn1->move(footer->width()-154,footer->height()-28);
bottomBtn2->move(footer->width()-77,footer->height()-28);
bottomBtn1->setText(("��һ��"));
bottomBtn2->setText(("�ر�"));

//��߱�ǩ����

tagWidget=new QWidget(contentWidget);
tagWidget->move(0,37);
tagWidget->resize(128,283);//236 244 249
tagWidget->setStyleSheet("QWidget{\
                         background-color:rgb(236,244,249);\
}");
//ͷ������
pixLabel=new Label(tagWidget);
pixLabel->move(10,37);
pixLabel->resize(108,108);
pixLabel->setStyleSheet(QString("QLabel{\
                                border:0px;\
                        border-radius:%1px;\
}\
").arg(pixLabel->width()/2));
nameBtn=new QPushButton(tagWidget);
nameBtn->resize(108,20);
nameBtn->setStyleSheet("QPushButton{ \
                       border:0;\
font: 10pt \"����\";\
    text-align:left;\
}\
QPushButton:hover{ \
    text-decoration: underline;\
}\
");
nameBtn->setCursor(Qt::PointingHandCursor);
nameBtn->move(8,145);
nameBtn->setFlat(true);
numberLabel=new QLabel(tagWidget);
numberLabel->resize(108,20);
numberLabel->move(8,165);

sexAndAgeLabel=new QLabel(tagWidget);
sexAndAgeLabel->resize(108,40);
sexAndAgeLabel->move(8,185);
sexAndAgeLabel->setTextFormat(Qt::RichText);
sexAndAgeLabel->setStyleSheet(("QLabel{\
                               color:gray;\
                              font: 9pt \"����\";\
                              }"));
locationLabel=new QLabel(tagWidget);
locationLabel->resize(108,20);
locationLabel->move(8,225);
locationLabel->setStyleSheet(("QLabel{\
                              color:gray;\
                             font: 9pt \"����\";\
                             }"));
//fillwidget
fillWidget=new QStackedWidget(contentWidget);
fillWidget->resize(contentWidget->width()-tagWidget->width(),tagWidget->height());
fillWidget->move(128,37);
page1=new QWidget();
page2=new QWidget();
page3=new QWidget();
fillWidget->addWidget(page1);
fillWidget->addWidget(page2);
fillWidget->addWidget(page3);
page1->resize(fillWidget->width(),fillWidget->height());
page2->resize(fillWidget->width(),fillWidget->height());
page3->resize(fillWidget->width(),fillWidget->height());
//�ش���֤��Ϣ
verifyLbael=new QLabel(page1);
verifyLbael->resize(104,20);
verifyLbael->move(20,10);
QFont font(("����"),10);
verifyLbael->setFont(font);
verifyLbael->setText(("��������֤��Ϣ��"));
//��֤��
verifyEdit=new QTextEdit(page1);
verifyEdit->resize(282,70);
verifyEdit->move(20,35);
verifyEdit->setFont(font);
verifyEdit->setPlaceholderText(("����..."));
verifyEdit->setStyleSheet("QTextEdit{border:1px solid lightgray;}");

//����ѡ��
QVBoxLayout*vlay=new QVBoxLayout(page2);
vlay->setGeometry(QRect(20,10,0,0));
vlay->setSpacing(5);
vlay->setSizeConstraint(QLayout::SetFixedSize);
QHBoxLayout*hlay1=new QHBoxLayout();
hlay1->setSizeConstraint(QLayout::SetFixedSize);
pixTipLabel=new QLabel(page2);
pixTipLabel->resize(40,40);
pixTipLabel->setMargin(10);
pixTipLabel->setPixmap(QPixmap::fromImage(QImage(":/images/exMark.png","png")).scaled(30,30));
tipLabel=new QLabel(page2);
tipLabel->setFixedSize(250,40);
tipLabel->setFont(font);
tipLabel->setWordWrap(true);
hlay1->addWidget(pixTipLabel,0,Qt::AlignLeft);
hlay1->addWidget(tipLabel,0,Qt::AlignLeft);

QLabel*alterLabel=new QLabel(this);//�޸�tag
alterLabel->resize(165,20);
alterLabel->setText(("������޸ı�ע�����ͷ��飺"));
alterLabel->setFont(font);
//��ע
QGridLayout*glay1=new QGridLayout();
glay1->setSizeConstraint(QLayout::SetFixedSize);
glay1->setSpacing(5);
QLabel* tagLabel=new QLabel(page2);
tagLabel->setFixedSize(60,20);
tagLabel->setText(("��ע������"));
tagLabel->setFont(font);
tagLEdit=new QLineEdit(page2);
tagLEdit->setFixedSize(170,22);
tagLEdit->setFont(font);
tagLEdit->setPlaceholderText(("ѡ��"));
tagLEdit->setStyleSheet("QLineEdit{\
                        border:1px solid lightgray;\
border-radius:3px;\
}\
QLineEdit:hover{\
border:2px solid rgb(0, 155, 219);\
    border-radius:3px;\
}");
glay1->addWidget(tagLabel,0,0,Qt::AlignLeft);
glay1->addWidget(tagLEdit,0,1,Qt::AlignLeft);
//����

QLabel* groupLabel=new QLabel(page2);
groupLabel->setFixedSize(60,20);
groupLabel->setFont(font);
groupLabel->setText(("��    �飺"));
groupCBox=new QComboBox(page2);
groupCBox->setFixedSize(170,20);
groupCBox->setStyleSheet(" QComboBox {\
                         border:1px solid lightgray;\
border-radius: 3px;\
padding: 0 0 0 5;\
background: white;\
}\
QComboBox:hover{\
border:1px solid rgb(0, 155, 219);\
    border-radius: 3px;\
padding: 0 0 0 5;\
background: white;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           top: 1px;\
left: 1px;\
}\
QComboBox::down-arrow {\
image:url(:/images/down-arrow.png)\
}\
QComboBox::drop-down {\
    subcontrol-origin: 15;\
    subcontrol-position: right;\
width:15px;\
color:red;\
    border-bottom-right-radius: 3px;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           image:url(:/images/up-arrow.png)\
                           }");

  QPushButton*newGroupBtn=new QPushButton(page2);
newGroupBtn->setFixedSize(60,20);
newGroupBtn->setFlat(true);
newGroupBtn->setStyleSheet(("QPushButton{\
                            border:0;\
                           color:rgb(18,183,245);\
font: 9pt \"����\";\
    }\
    QPushButton:pressed{\
        border:0;\
color:black;\
font: 9pt \"����\";\
}\
"));
newGroupBtn->setText(("�½�����"));
glay1->addWidget(groupLabel,1,0,Qt::AlignLeft);
glay1->addWidget(groupCBox,1,1,Qt::AlignLeft);
glay1->addWidget(newGroupBtn,1,2,Qt::AlignLeft);
vlay->addLayout(hlay1);
vlay->addWidget(alterLabel,0,Qt::AlignLeft);
vlay->addLayout(glay1);

//�����ʾ
//304 181
QLabel*finishLabel=new QLabel(page3);
finishLabel->resize(304,181);
finishLabel->setPixmap(QPixmap::fromImage(QImage(":/images/finishTip.png")));
finishLabel->move(20,10);

connect(newGroupBtn,&QPushButton::clicked,this,&AddFriendsWidget::newGroup);
connect(tipLabel,&QLabel::linkActivated,this,&AddFriendsWidget::tipLinked);
connect(tipLabel,&QLabel::linkHovered,this,&AddFriendsWidget::tipHover);
connect(bottomBtn1,&QPushButton::clicked,this,&AddFriendsWidget::next);
connect(bottomBtn2,&QPushButton::clicked,this,&AddFriendsWidget::cancel);
connect(verifyEdit,&QTextEdit::textChanged,this,&AddFriendsWidget::districtVerifyTextLength);
connect(minBtn,&QPushButton::clicked,this,[=](){
    this->showMinimized();
});
connect(closeBtn,&QPushButton::clicked,this,[=](){
    this->close();
});
//ͷ����ǳƵ���������Ͽ��¼�
connect(pixLabel,&Label::clicked,this,&AddFriendsWidget::openPersonalData);
connect(nameBtn, QOverload<bool>::of(&QPushButton::clicked),this,&AddFriendsWidget::openPersonalData);
}

AddFriendsWidget::~AddFriendsWidget()
{
    qDebug()<<"~AddFriendsWidget()";
}

void AddFriendsWidget::closeEvent(QCloseEvent *event)
{
    emitClosed();//�����ź� �ȸ����б�
    QWidget::closeEvent(event);
}




bool AddFriendsWidget::operator ==(AddFriendsWidget *w)
{
    qDebug()<<":operator ==(AddFriendsWidget *w) result:"<<(this->myqq==w->myqq);
    if(this->myqq==w->myqq)return true;
    else return false;
}

void AddFriendsWidget::inint(QString& myNumber)
{
    qDebug()<<"init()";
    QPixmap pix;
    pix.loadFromData(pixData);
    pix=pix.scaled(pixLabel->width(),pixLabel->height());
    pixmapToRound(&pix);//תԲ��
    pixLabel->setPixmap(pix);
    QString temp;
    temp=getElideText(name,140,nameBtn->fontMetrics());
    nameBtn->setText( temp);
    temp=getElideText(myqq,108,numberLabel->fontMetrics());
    numberLabel->setText( temp);
    temp=QString("<p style='margin:0 0 5 0;font: 10pt \"����\";text-align:left;'>%1</p>").arg(("�Ա�")+sex)+
            QString("<p style='margin:0 0 5 0;font: 10pt \"����\";text-align:left;'>%1").arg(("���䣺")+age+("��"));
    sexAndAgeLabel->setText(temp);
    location=("���ڵأ�")+location;
    temp=getElideText(location,108,locationLabel->fontMetrics());
    locationLabel->setText( temp);
    temp=QString("%1").arg(name+"("+myqq+")");
    temp=("<a style='margin:0;font: 10pt \"����\";text-align:left;text-decoration: none;';")+QString("a href='%1'>%2</a>")
            .arg(myqq).arg(temp);
    tipLabel->setText(temp);
    QTextStream serr(stderr);
    QByteArray sdata;
    // sdata.resize(1024);//�������ô�С�����json�Ľṹ
    QSharedMemory sharmm;
    sharmm.setKey("addFriendgroups"+myNumber);//����Ψһ��ʶ�ļ�
    if(!sharmm.attach(QSharedMemory::ReadOnly)){
        serr<<"warning:the shared memory is not attach with  key named groups"<<endl;
        this->close();
        return;
    }
    sharmm.lock();
    sdata.resize(strlen((char*)sharmm.data()));//ע����뱣�����ݸ�ʽһ�²�����������
    memccpy(sdata.data(),sharmm.constData(),'\0',1024);
    sharmm.unlock();//����
    if(sdata.isEmpty()){
        serr<<"warning:the data is empty from shared memory with  key named groups";
        this->close();
        return;
    }
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(sdata,&err);
    QJsonArray arr=doc.array();//��
    if(arr.isEmpty()||arr.size()<=1){
        serr<<"warning:the data is not corrected from shared memory with  key named groups"<<myNumber<<endl;
        serr<<"json errstring:"<<err.errorString()<<endl;
        this->close();
        return;
    }
    qint32 length=arr.size();
    //������� ȥ���ҵ��豸
    for(qint32 i=1;i<length;++i){
        temp =arr.at(i).toString();
        if(temp.isNull()){
            serr<<"warning:a entry is null in array from shared memory with  key named groups";
            if(i<2){
                this->close();
                return;
            }else continue;
        }
        groupCBox->addItem(temp);
    }
}

QString  AddFriendsWidget::getElideText(QString &name, const qint32 &width, const QFontMetrics &f)
{
    QString str=name;
    if(f.boundingRect(name).width()>=width){
        str=f.elidedText(name,Qt::ElideRight,width-f.boundingRect("...").width());
        str.replace(str.length()-3,3,"...");
    }
    return str;
}

void AddFriendsWidget::districtVerifyTextLength()
{
    QString str=verifyEdit->toPlainText();
    qint32 size=str.toUtf8().size();
    if(size>78){
        QTextCursor cur=verifyEdit->textCursor();
        str=str.left(last);
        verifyEdit->setPlainText(str);
        cur.setPosition(str.length());
        verifyEdit->setTextCursor(cur);
    }
    last=str.length();
}

void AddFriendsWidget::next()
{
    if(fillWidget->currentIndex()==0){
        fillWidget->setCurrentIndex(1);
        bottomBtn2->setText(("ȡ��"));
    }else  if(fillWidget->currentIndex()==1){
        fillWidget->setCurrentIndex(2);
        bottomBtn1->hide();
        bottomBtn2->setText(("���"));
        //���ͺ�����Ϣ
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("verify",QJsonValue(verifyEdit->toPlainText()));//��֤��Ϣ
        obj.insert("number",QJsonValue(myqq));//myqq
        obj.insert("tag",QJsonValue(tagLEdit->text()));//��ע
        obj.insert("groupName",QJsonValue(groupCBox->currentText()));//����
        doc.setObject(obj);
        emit   emitVerifyInfo(doc);
    }
}

void AddFriendsWidget::cancel()
{
    this->close();
}

void AddFriendsWidget::tipLinked(const QString &link)
{
    qDebug()<<"tipLinked()"<<link;
}

void AddFriendsWidget::tipHover(const QString &link)
{
    isLinkedHover=!isLinkedHover;
    QString  temp=QString("%1").arg(name+"("+myqq+")");
    if(isLinkedHover){
        temp=("<a style='margin:0;font: 10pt \"����\";text-align:left;text-decoration:underline;';")+QString("a href='%1'>%2</a>")
                .arg(myqq).arg(temp);
    }else{
        temp=("<a style='margin:0;font: 10pt \"����\";text-align:left;text-decoration:none;';")+QString("a href='%1'>%2</a>")
                .arg(myqq).arg(temp);
    }
    tipLabel->setText(temp);
}

void AddFriendsWidget::newGroup()
{
    qDebug()<<"newGroup()";
    QTextStream out(stdout);
    if(groupCBox->count()>15){
        out<<"the number of friend groups is not more than 15";
        return;
    }
    if(childWidget){
        childWidget->show();
        childWidget->raise();
        childWidget->activateWindow();
        return;
    }
    //326 151
    childWidget=new MovedWidget(this);
    childWidget->setAttribute(Qt::WA_QuitOnClose,false);//�رղ��˳�
    childWidget->setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    childWidget->resize(336,161);
    childWidget->setAttribute(Qt::WA_TranslucentBackground);//����͸������
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(childWidget);
    //������Ӱ����
    shadow->setOffset(0, 0);
    //������Ӱ��ɫ
    shadow->setColor(QColor(200,200,200));
    //������ӰԲ��
    shadow->setBlurRadius(5);
    childWidget ->setGraphicsEffect(shadow);
    QWidget*contWidget=new QWidget(childWidget);
    contWidget->resize(326,151);
    contWidget->move(5,5);
    contWidget->setStyleSheet("QWidget{background-color:white;}");
    //��ͷ
    QFrame* frameHeader=new QFrame(contWidget);
    frameHeader->resize(326,30);
    frameHeader->move(0,0);
    frameHeader->setStyleSheet("QFrame{\
                               image: url(:/images/newGroupTitle.png);\
} \
");
QPushButton* closeChildBtn=new QPushButton(frameHeader);
closeChildBtn->resize(28,30);
closeChildBtn->move(frameHeader->width()-28,0);
closeChildBtn->setStyleSheet("QPushButton{\
                             border:0;\
background-color: rgba(255, 255, 255, 0);\
}\
QPushButton:pressed {\
    border-image: url(:/images/addFriendClosePressed.png);\
}\
QPushButton:hover:!pressed {\
                       border-image: url(:/images/addFriendsCloseHovered.png);\
}");
QLabel*label=new QLabel(childWidget);
label->resize(60,20);
label->move(30,50);
label->setText(("��ӷ��飺"));
QLineEdit*addLEdit=new QLineEdit(childWidget);
addLEdit->resize(278,23);
addLEdit->move(30,70);
addLEdit->setProperty("lastLength",QVariant(0));
addLEdit->setStyleSheet("QLineEdit{\
                        border:1px solid lightgray;\
border-radius:5px;\
}\
QLineEdit:hover{\
border:1px solid rgb(0, 155, 219);\
    border-radius:5px;\
}");
//�ײ��ؼ�
QFrame* frameFooter=new QFrame(contWidget);
frameFooter->resize(326,35);
frameFooter->move(0,contWidget->height()-35);
frameFooter->setStyleSheet("QFrame{\
                           border:0px\
                           background-color:rab(229,239,247);\
} \
");
QPushButton* childBottomBtn1=new QPushButton(frameFooter);
QPushButton* childBottomBtn2=new QPushButton(frameFooter);
childBottomBtn1->resize(69,24);
childBottomBtn2->resize(69,24);
childBottomBtn1->setStyleSheet("QPushButton{\
                               border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
childBottomBtn2->setStyleSheet("QPushButton{\
                               border:1px solid rgb(79, 173, 216);\
border-radius:2;\
background-color:rgb(244,244,244);\
}\
QPushButton:hover:!pressed{\
                       background-color: rgb(190, 231, 253);\
};\
QPushButton:pressed{\
    background-color: rgb(235, 236, 237);\
};\
");
childBottomBtn1->move(frameFooter->width()-154,frameFooter->height()-28);
childBottomBtn2->move(frameFooter->width()-77,frameFooter->height()-28);
childBottomBtn1->setText(("ȷ��"));
childBottomBtn2->setText(("ȡ��"));
childBottomBtn1->setEnabled(false);
//�رմ���
connect(closeChildBtn,&QPushButton::clicked,contWidget,[=](){
    delete childWidget,childWidget=nullptr;
});
connect(childBottomBtn2,&QPushButton::clicked,contWidget,[=](){
    closeChildBtn->click();
});
//����·���
connect(childBottomBtn1,&QPushButton::clicked,contWidget,[=]()mutable{
    groupCBox->insertItem(groupCBox->count()-1,addLEdit->text());
    groupCBox->setCurrentIndex(groupCBox->count()-2);
    QTextStream out1(stdout);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("add group"));//ָ�� ����Ҫ���·���
    obj.insert("index",QJsonValue(-1));//��ӵ����һ��֮ǰ
    obj.insert("groupName",QJsonValue(addLEdit->text().toUtf8().data()));
    doc.setObject(obj);
    out1<<doc.toJson();//�����������
    closeChildBtn->click();
});
connect(addLEdit,&QLineEdit::textChanged,childWidget,[=](){
    QString str=addLEdit->text();
    qint32 lastLength=addLEdit->property("lastLength").toInt();
    qint32 size=str.toUtf8().size();
    if(size>24){
        str=str.left(lastLength);
        addLEdit->setText(str);
    }
    lastLength=str.length();
    addLEdit->setProperty("lastLength",QVariant(lastLength));
    childBottomBtn1->setEnabled(true);
});
childWidget->show();
childWidget->raise();
childWidget->activateWindow();
}

void AddFriendsWidget::openPersonalData()
{
    if(hasClicked)return;
    hasClicked=true;//��ֹ��ε��
    QTextStream out1(stdout);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("instruct",QJsonValue("open fwin"));//ָ�� ����Ҫ���·���
    obj.insert("number",QJsonValue(myqq));
    obj.insert("signature",QJsonValue(signature));
    //����ͼƬ̫���ˣ�����һ�ν��ܣ���Ҫ�������ƣ�����ֱ�Ӵ�·��
    QPixmap pix;
    pix.loadFromData(pixData,"png");
    QDir td("../head_images");
    if(!td.exists())td.mkpath("./");
    pix.save(td.filePath(myqq),"png");
    QFileInfo fi(td.filePath(myqq));
    obj.insert("pixmap",QJsonValue(fi.absoluteFilePath()));
    obj.insert("name",QJsonValue(name));
    doc.setObject(obj);
    out1<<doc.toJson();//�����������
}

void AddFriendsWidget::pixmapToRound(QPixmap*img) const
{
    QBitmap mask(QSize(img->width(),img->height()));
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(QRect(0, 0, img->width(), img->height()), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(QRect(0, 0, img->width(), img->height()), 100, 100,Qt::RelativeSize);
    img->setMask(mask);
}
