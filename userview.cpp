#include "userview.h"
#include<qpushbutton.h>
#include<qlayout.h>
#include<qdebug.h>
#include <qpainter.h>
#include<qscrollbar.h>
#include <QMouseEvent>
#include<qevent.h>
QString UserView::m_pageSheet="QPushButton{\
        color:black;\
background-color: rgb(255, 255, 255);\
border:1px solid lightgray ;\
border-radius:3px\
    }\
    QPushButton:hover{\
        background-color:rgb(62, 156, 234);\
color: rgb(255, 255, 255);\
}";
QString UserView::m_pageNumberSheet="QPushButton:hover{\
        background-color:rgb(62, 156, 234);\
color: rgb(255, 255, 255);\
}\
QPushButton:checked{\
    background-color:rgb(62, 156, 234);\
color: rgb(255, 255, 255);\
}";
UserView::UserView(QWidget *parent)
    :QScrollArea(parent)
{//14 w
    sliderMaxValue=0;//Ĭ��Ϊ0
    canContinue=1;
    verticalSpacing=10;
    horizotalSpacing=10;
    model=nullptr;//��ʼ��Ϊ0��������������
    this->resize(826,376);
    this->setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setContentsMargins(0,0,0,0);
    this->setStyleSheet("QScrollArea{\
                        border:0px;\
            background-color:transparent;\
}");
QScrollBar*verScroll=this->verticalScrollBar();//ֱ��QScrollArea����û���ã���֪��Ϊʲô�����Ե���������ʽ
verScroll->setStyleSheet("QScrollBar:vertical {\
                         background:transparent;\
width: 8px;\
border-radius:3;\
}\
QScrollBar::handle:vertical {\
                       background:#bec2c8;\
border-radius:3;\
}\
QScrollBar::handle:vertical:hover {\
                       background:#77797c;\
}\
QScrollBar::handle:vertical:pressed {\
                       background:#d6dae0;\
}\
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
                         background: none;\
}\
QScrollBar::add-line:vertical {\
                         background: transparent;\
width: 0px;\
height:0;\
subcontrol-position: top;\
subcontrol-origin: margin;\
}\
QScrollBar::sub-line:vertical {\
                         background: transparent;\
width: 0px;\
height:0;\
subcontrol-position: bottom;\
subcontrol-origin: margin;\
}");
page=0;
buttonFrame=new QFrame;
buttonFrame->resize(this->width(),50);
buttonFrame->hide();
lastBtn=new QPushButton(buttonFrame);
lastBtn->setText(QStringLiteral("��һҳ"));
lastBtn->setStyleSheet(m_pageSheet);
lastBtn->resize(62,25);
nextBtn=new QPushButton(buttonFrame);
nextBtn->setText(QStringLiteral("��һҳ"));
nextBtn->setStyleSheet(m_pageSheet);
nextBtn->resize(62,25);
oneBtn=new QPushButton(buttonFrame);
oneBtn->resize(25,25);
oneBtn->setText("1");
oneBtn->setCheckable(true);
QString str="QPushButton{\
        color:black;background-color: rgb(255, 255, 255);border:1px solid lightgray;\
border-top-left-radius:3px;border-bottom-left-radius:3px;}";
oneBtn->setStyleSheet(str+m_pageNumberSheet);
twoBtn=new QPushButton(buttonFrame);
twoBtn->resize(25,25);
twoBtn->setText("2");
twoBtn->setCheckable(true);
twoBtn->setStyleSheet("QPushButton{\
                      color:black;background-color: rgb(255, 255, 255);border:1px solid lightgray;\
border-top-right-radius:3px;border-bottom-right-radius:3px;}"+m_pageNumberSheet);

int posx=buttonFrame->width()/2-15-oneBtn->width()-lastBtn->width();
lastBtn->move(posx,14);
lastBtn->hide();
posx+=lastBtn->width()+15;
oneBtn->move(posx,14);
posx+=oneBtn->width();
twoBtn->move(posx,14);
nextBtn->move(posx+twoBtn->width()+15,14);
oneBtn->setChecked(true);
twoBtn->setChecked(false);
lastBtn->hide();
nextBtn->show();
frame=new QFrame(this);
frame->setMinimumSize(826,376);
this->setWidget(frame);
frame->setContentsMargins(0,0,0,0);
buttonFrame->setParent(frame);
connect(lastBtn,&QPushButton::clicked,this,&UserView::lastButtonClicked);
connect(nextBtn,&QPushButton::clicked,this,&UserView::nextButtonClicked);
connect(oneBtn,&QPushButton::clicked,this,&UserView::oneButtonClicked);
connect(twoBtn,&QPushButton::clicked,this,&UserView::twoButtonClicked);
}

UserView::~UserView()
{
    if(lastBtn){
        delete lastBtn,lastBtn=nullptr;
    }
    if(nextBtn){
        delete nextBtn,nextBtn=nullptr;
    }
    if(oneBtn){
        delete oneBtn,oneBtn=nullptr;
    }
    if(twoBtn){
        delete twoBtn,twoBtn=nullptr;
    }
    if(buttonFrame){
        delete buttonFrame,buttonFrame=nullptr;
    }
    if(frame){
        delete frame;
        frame=nullptr;
    }
    if(model){
        delete model,model=nullptr;
    }
}

void UserView::setModel(FindUserModel *m)
{
    const int sum=m->sum();
    if(sum>120)//�б������2ҳ֮�� ��120��
        return;
  if(model)model->clear();
    model=m;
    int c=model->columnCount(),r=model->rowCount();
    qDebug()<<"start circulation";
    int fullFirstPageCount=15*c;
    int count=0;
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++){
            if(count>=sum)break;//���ܳ������ֵ
            UserWidget*w=new UserWidget(frame);
            QModelIndex index=model->index(i,j);
            QImage img=QImage::fromData(model->data(index,FindUserModel::HeadImgRole).toByteArray(),"png");
            QPixmap* pix=new QPixmap();
            *pix=QPixmap::fromImage(img);
            w->setPrimitivePixmap(pix);
            w->setName(model->data(index,FindUserModel::NameRole).toString());
            w->setIndex(index);
            QString country=model->data(index,FindUserModel::AgeRole).toString();
            QString province= model->data(index,FindUserModel::FirstLevelRegionRole).toString();
            if(country.isEmpty()||province.isEmpty()){
                QPixmap temp= QPixmap(":/images/person.png","png");
                w->setAgeAndCountry(temp);
            }
            else  w->setAgeAndCountry(country+QStringLiteral("| ")+province);
            list.append(w);
            w->hide();
            if(count<fullFirstPageCount){
                w->move(12+i*(w->width()+horizotalSpacing),10+j*(w->height()+verticalSpacing));
                w->show();//�����ʾһҳ���� 15��
            }
            QString myqq=model->data(index,FindUserModel::MyqqRole).toString();
            count++;
            connect(w,&UserWidget::imgClicked,this,[=](){

            });
            connect(w,&UserWidget::nameClicked,this,[=](){
                emit nameClicked(myqq);
            });
            connect(w,&UserWidget::addButtonClicked,this,[=](){
                emit addButtonClicked(index);
            });
        }
    if(count>=fullFirstPageCount)
        adjustFrameSize(fullFirstPageCount,true);
    else  adjustFrameSize(count);
    connect(model,QOverload<const int &, const int &, const int &>::of(&FindUserModel::insert),this,&UserView::inserted);//���Ӳ����ź�
    connect(model,&QAbstractItemModel::modelReset,this,&UserView::reset);//���������źŵ���ͼ
    connect(model,&FindUserModel::dataChanged,this,&UserView::dataChanged);//�������ݱ仯�źŵ���ͼ
}



void UserView::setCanContinue(const bool &b)
{
    if(b)
        canContinue=-1;//���� ������ݺ����������
    else {
        canContinue=(int)b;
    }
}

void UserView::scrollContentsBy(int dx, int dy)
{
    frame->scroll(dx,dy);
    if(verticalScrollBar()->value()==sliderMaxValue&&canContinue==1){
        canContinue=0;
        emit continueGetList();//�����źż�������û��б�
    }
    qDebug()<<(verticalScrollBar()->value()==sliderMaxValue)<<canContinue;
}
//������֧�ֹ���phase��ƽ̨�ϣ�pixelDelta()��ĳЩ�׶ο��ܷ���null�������ۺ�pixelDelta()��angleDelta()ʵʩ
void UserView::wheelEvent(QWheelEvent *wheel)
{
    QPoint numPixels = wheel->pixelDelta();//������ǰ������ֵΪ��ֵ
    QPoint numDegrees = wheel->angleDelta() / 8;//������ǰ�ľ���ֵΪ��ֵ һ�����15��Ϊһ��λ
    if (!numPixels.isNull()) {
        qDebug()<<"pixel wheel";
        int y=verticalScrollBar()->value()-numPixels.ry();
        if(y<0)numPixels.setY(verticalScrollBar()->value());
        else if(y>sliderMaxValue)numPixels.setY(verticalScrollBar()->value()-sliderMaxValue);
        verticalScrollBar()->setValue(verticalScrollBar()->value()-numPixels.ry());
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees;
        int y=verticalScrollBar()->value()-numSteps.ry();
        if(y<0)numSteps.setY(verticalScrollBar()->value());
        else if(y>sliderMaxValue)numSteps.setY(verticalScrollBar()->value()-sliderMaxValue);
        verticalScrollBar()->setValue(verticalScrollBar()->value()-numSteps.ry());//���ù�����ֵ����scrollContentsBy(int dx, int dy)����
    }
    wheel->accept();
}



void UserView::clearLayoutElements()
{
    int c=model->columnCount();
if(page==0){
    for(int i=0;i<c*15;i++){
        list.at(i)->setVisible(false);
    }
}else{
    int count=list.count();
    for(int i=15*c;i<count;i++){
        list.at(i)->setVisible(false);
    }
}
}





//�����ͼ�ؼ�
void UserView::reset()
{
    qDebug()<<"reset";
    for (UserWidget*v : list) {
        delete v,v=nullptr;
    }
    list.clear();
    frame->resize(this->width(),this->height());
    sliderMaxValue=0;
    frame->move(0,0);
    oneBtn->setChecked(true);
    twoBtn->setChecked(false);
    lastBtn->hide();
    nextBtn->show();
    buttonFrame->hide();
}

void UserView::inserted(const int &row, const int &column, const int&count)
{
    const int realIndex=row*model->columnCount()+column;
    int sum=list.count();
    int c=model->columnCount();
    for(int i=realIndex;i<sum;i++){
        list.at(i)->hide();//���ؿؼ�
    }
    qDebug()<<"inserted "<<row<<column;
    for(int i=0;i<count;i++){
        int cr,cc;
        cc=column+i;
        cr=row+cc/c;
        cc=cc%c;
        UserWidget*w=new UserWidget(frame);
        w->hide();//������
        QModelIndex index=model->index(cr,cc);
        w->setIndex(index);
        QImage img=QImage::fromData(model->data(index,FindUserModel::HeadImgRole).toByteArray(),"png");
        QPixmap* pix=new QPixmap();
        *pix=QPixmap::fromImage(img);
        w->setPrimitivePixmap(pix);
        w->setName(model->data(index,FindUserModel::NameRole).toString());
        QString country=model->data(index,FindUserModel::AgeRole).toString();
        QString province= model->data(index,FindUserModel::FirstLevelRegionRole).toString();
        if(country.isEmpty()||province.isEmpty()){
            QPixmap temp= QPixmap(":/images/person.png","png");
            w->setAgeAndCountry(temp);
        }
        else  w->setAgeAndCountry(country+QStringLiteral("| ")+province);
        list.insert(realIndex+i,w);
    }
    QModelIndex index=model->lastItemIndex();
    int r=index.row();
    qint32 sliderValue=0;
    if(row<15){
        if(list.count()==15*c){
           sliderValue= adjustFrameSize(r+1,true);
           canContinue=0;
        }else
            sliderValue=adjustFrameSize(r+1);
        qDebug()<<sliderValue<<sliderMaxValue;
        for(int i=row;i<r;i++)
            for(int j=0;j<c;j++){
                UserWidget*w=list.at(i*c+j);
                w->move(12+j*(w->width()+horizotalSpacing),10+i*(w->height()+verticalSpacing)-sliderValue);
                w->show();
            }
        for(int i=0;i<=index.column();i++){
            UserWidget*w=list.at(r*c+i);
            w->move(12+i*(w->width()+horizotalSpacing),10+r*(w->height()+verticalSpacing)-sliderValue);
            w->show();
        }
    }else{
        if(list.count()==30*c){
            canContinue=0;
           sliderValue= adjustFrameSize(r-14,true);
        }else {
            if(canContinue==0)
               sliderValue= adjustFrameSize(r-14,true);
            else
               sliderValue= adjustFrameSize(r-14);
        }
        for(int i=row;i<r;i++)
            for(int j=0;j<c;j++){
                UserWidget*w=list.at(i*c+j);
                w->move(12+j*(w->width()+horizotalSpacing),10+(i-15)*(w->height()+verticalSpacing)-sliderValue);
                qDebug()<<"add(row,column):"<<i-15<<j<<10+(i-15)*(w->height()+verticalSpacing)-sliderValue;
                w->show();
            }
        for(int i=0;i<=index.column();i++){
            UserWidget*w=list.at(r*c+i);
            w->move(12+i*(w->width()+horizotalSpacing),10+(r-15)*(w->height()+verticalSpacing)-sliderValue);
            w->show();
        }
    }
    if(canContinue==-1)
        canContinue=1;
    qDebug()<<"can?="<<canContinue;
}

void UserView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    int r1=topLeft.row(),r2=bottomRight.row(),c1=topLeft.column(),c2=bottomRight.column();
    for(int i=r1;i<=r2;i++){
        for(int j=c1;j<=c2;j++){
            UserWidget*w=list.at(i*model->columnCount()+j);
            QModelIndex index=model->index(i,j);
            QImage img;QPixmap pix;
            for(int v:roles) {
                switch (v) {
                case FindUserModel::HeadImgRole:
                    img=QImage::fromData(model->data(index,FindUserModel::HeadImgRole).toByteArray(),"png");
                    pix=QPixmap::fromImage(img);
                    w->setPixmap(pix);
                    break;
                case FindUserModel::AgeRole:
                    w->setAgeAndCountry(model->data(index,FindUserModel::AgeRole).toString()+QStringLiteral("| ")+
                                        model->data(index,FindUserModel::FirstLevelRegionRole).toString());
                    break;
                case FindUserModel::FirstLevelRegionRole:
                    w->setAgeAndCountry(model->data(index,FindUserModel::AgeRole).toString()+QStringLiteral("| ")+
                                        model->data(index,FindUserModel::FirstLevelRegionRole).toString());
                    break;
                case FindUserModel::NameRole:
                    w->setName(model->data(index,FindUserModel::NameRole).toString());
                    break;
                default:
                    break;
                }
            }
        }
    }
}


//������ʾҳ��ť�Ƿ���ʾ  ���ڵ���fame��С�͹�������ֵ
qint32 UserView::adjustFrameSize(const qint32&row,const bool &b)
{
    qint32 last=sliderMaxValue;//���������ֵ
    if(list.count()==0||row<=0)//����frame��СĬ��ֵ
        frame->resize(this->width(),this->height());
    else{
        UserWidget*w=list.at(0);
        if(b){
            frame->resize(this->width(),20+row*w->height()+verticalSpacing*(row-1)+buttonFrame->height());
            //ע�⵱frame�ط����Сʱ��frame�������ƶ����������ķ����򣬻ᵼ������ϵͳƫ��sliderMaxValue��С
            //��ʹframe�����ƶ������λ�û�����������this->setWidget(frame);,�Ӿ���ڵ�����Ҳ��ʵ��λ��ƫ��sliderMaxValue��С������λ���޸Ķ���Ҫ��������λ��-sliderMaxValue
            buttonFrame->move(0,frame->height()-buttonFrame->height()-last);//λ��-sliderMaxValue,�����ط��������ϵͳ��Ӱ��
            buttonFrame->show();
        }else{
            buttonFrame->hide();
            frame->resize(this->width(),20+row*w->height()+verticalSpacing*(row-1));
        }
    }
    sliderMaxValue=frame->height()-this->height();//Ĭ�ϵĹ������������ֵΪ��ǰ�ؼ���С-���������С
   frame->move(0,0);//�ƶ���0���������Է��ƶ���frame��Ӱ��Ӱ��
   return last;
}

void UserView::setVerticalSpacing(const qint32 &spacing)
{
    verticalSpacing =spacing;
}

void UserView::setHorizontalSpacing(const qint32 &spacing)
{
    horizotalSpacing=spacing;
}




void UserView::lastButtonClicked()
{
    int c=model->columnCount(),r=15;
    if(list.count()<r*c){//�����߼����󷵻�
        oneBtn->setChecked(false);
        return;
    }
    twoBtn->setChecked(false);
    oneBtn->setChecked(true);
    nextBtn->setVisible(true);
    lastBtn->setVisible(false);
    clearLayoutElements();
    page=0;
    adjustFrameSize(r,true);//������С����ʾ��ť
    for (int i=0;i<r;i++)
        for(int j=0;j<c;j++){
          list.at(i*c+j)->show();
        }
    verticalScrollBar()->setValue(0);
}

//
void UserView::nextButtonClicked()
{
    int c=model->columnCount(),count=list.count();
    int index=15*c;
    if(count>15*c&&count<=30*c){//2ҳ���� ֱ�ӻ�ҳ
        twoBtn->setChecked(true);
        oneBtn->setChecked(false);
        nextBtn->setVisible(false);
        lastBtn->setVisible(true);
        clearLayoutElements();
        page=1;
        adjustFrameSize(-1,true);//��ʾ��ť,����|��������СΪĬ��ֵ
        verticalScrollBar()->setValue(0);//�ص�����
        int row=count/c-15,column=count%c;
        if(column!=0)row+=1;
        for (int i=0;i<row-1;i++)
            for(int j=0;j<c;j++){
                list.at(index+i*c+j)->show();//��ʾ�Ѿ�����õĿؼ�
            }
        for(int i=0;i<column;i++){
            list.at(index+row*c+i)->show();//��ʾ�Ѿ�����õĿؼ�
        }
    }else if(count==index){//1ҳ���� Ҫ��ȡ���ݲ�����
        //Ԥ����һ�οɼ�ʱ״̬
        twoBtn->setChecked(true);
        oneBtn->setChecked(false);
        nextBtn->setVisible(false);
        lastBtn->setVisible(true);
        clearLayoutElements();//��������
        page=1;
        adjustFrameSize(-1);//������С�͹������ֵ�����ذ�ť
        verticalScrollBar()->setValue(0);
        continueGetList();//�����������
    }else{//�����߼�����
        twoBtn->setChecked(false);//��ť�ָ�ԭ״̬
    }
}

void UserView::oneButtonClicked()
{
    bool b=oneBtn->isChecked();
    if(b){//���ǵ�ǰҳ
        lastButtonClicked();
    }else{//��ǰҳ
        oneBtn->setChecked(!b);
        verticalScrollBar()->setValue(0);
    }
}

void UserView::twoButtonClicked()
{
    bool b=twoBtn->isChecked();
    if(b){//���ǵ�ǰҳ
        nextButtonClicked();
    }else{//��ǰҳ
        twoBtn->setChecked(!b);
        verticalScrollBar()->setValue(0);
    }
}

