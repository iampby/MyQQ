#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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
    sliderMaxValue=0;//默认为0
    canContinue=1;
    verticalSpacing=10;
    horizotalSpacing=10;
    model=nullptr;//初始化为0，用于析构函数
    this->resize(826,376);
    this->setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setContentsMargins(0,0,0,0);
    this->setStyleSheet("QScrollArea{\
                        border:0px;\
            background-color:transparent;\
}");
QScrollBar*verScroll=this->verticalScrollBar();//直接QScrollArea设置没作用，不知道为什么，所以单独设置样式
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
lastBtn->setText(("上一页"));
lastBtn->setStyleSheet(m_pageSheet);
lastBtn->resize(62,25);
nextBtn=new QPushButton(buttonFrame);
nextBtn->setText(("下一页"));
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
    if(sum>120)//列表控制在2页之内 即120个
        return;
    if(model)model->clear();
    model=m;
    int c=model->columnCount(),r=model->rowCount();
    qDebug()<<"start circulation";
    int fullFirstPageCount=15*c;
    int count=0;
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++){
            if(count>=sum)break;//不能超过最大值
            UserWidget*w=new UserWidget(frame);
            QModelIndex index=model->index(i,j);
            QImage img=QImage::fromData(model->data(index,FindUserModel::HeadImgRole).toByteArray(),"png");
            QPixmap* pix=new QPixmap();
            *pix=QPixmap::fromImage(img);
            w->setPrimitivePixmap(pix);//w持有图片内存权
            w->setName(model->data(index,FindUserModel::NameRole).toString());
            w->setIndex(index);
            QString country=model->data(index,FindUserModel::AgeRole).toString();
            QString province= model->data(index,FindUserModel::FirstLevelRegionRole).toString();
            if(country.isEmpty()||province.isEmpty()){
                QPixmap temp= QPixmap(":/images/person.png","png");
                w->setAgeAndCountry(temp);
            }
            else  w->setAgeAndCountry(country+("| ")+province);
            list.append(w);
            w->hide();
            if(count<fullFirstPageCount){
                w->move(12+i*(w->width()+horizotalSpacing),10+j*(w->height()+verticalSpacing));
                w->show();//最多显示一页数据 15行
            }
            QString myqq=model->data(index,FindUserModel::MyqqRole).toString();
            count++;
            connect(w,&UserWidget::imgClicked,this,[=](){
                qDebug()<<"imgClicked:\nmyqq="<<myqq;
                emit  imgClicked(myqq);
            });
            connect(w,&UserWidget::nameClicked,this,[=](){
                qDebug()<<"nameClicked:\nmyqq="<<myqq;
                emit nameClicked(myqq);
            });
            connect(w,&UserWidget::addButtonClicked,this,[=](){
                qDebug()<<"addButtonClicked:\nmyqq="<<myqq;
                emit addButtonClicked(index);
            });
        }
    if(count>=fullFirstPageCount)
        adjustFrameSize(fullFirstPageCount,true);
    else  adjustFrameSize(count);
    connect(model,QOverload<const int &, const int &, const int &>::of(&FindUserModel::insert),this,&UserView::inserted);//连接插入信号
    connect(model,&QAbstractItemModel::modelReset,this,&UserView::reset);//连接重设信号到视图
    connect(model,&FindUserModel::dataChanged,this,&UserView::dataChanged);//连接数据变化信号到视图
    connect(model,&FindUserModel::removeItem,this,&UserView::removeItem);//连接数据变化信号到视图
}



void UserView::setCanContinue(const bool &b)
{
    if(b)
        canContinue=-1;//待定 填充数据后后再做决定
    else {
        canContinue=(int)b;
    }
}

void UserView::scrollContentsBy(int dx, int dy)
{
    frame->scroll(dx,dy);
    if(verticalScrollBar()->value()==sliderMaxValue&&canContinue==1){
        canContinue=0;
        emit continueGetList();//发送信号继续获得用户列表
    }
    qDebug()<<(verticalScrollBar()->value()==sliderMaxValue)<<canContinue;
}
//在所有支持滚动phase的平台上，pixelDelta()在某些阶段可能返回null，所以综合pixelDelta()和angleDelta()实施
void UserView::wheelEvent(QWheelEvent *wheel)
{
    QPoint numPixels = wheel->pixelDelta();//滚动向前的像素值为正值
    QPoint numDegrees = wheel->angleDelta() / 8;//滚动向前的距离值为正值 一般鼠标15度为一单位
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
        verticalScrollBar()->setValue(verticalScrollBar()->value()-numSteps.ry());//设置滚动栏值调用scrollContentsBy(int dx, int dy)函数
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





//清空视图控件
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
        list.at(i)->hide();//隐藏控件
    }
    for(int i=0;i<count;i++){
        int cr,cc;
        cc=column+i;
        cr=row+cc/c;
        cc=cc%c;
        UserWidget*w=new UserWidget(frame);
        w->hide();//先隐藏
        QModelIndex index=model->index(cr,cc);
        w->setIndex(index);
        QImage img=QImage::fromData(model->data(index,FindUserModel::HeadImgRole).toByteArray(),"png");
        QPixmap* pix=new QPixmap();
        *pix=QPixmap::fromImage(img);
        w->setPrimitivePixmap(pix);//w持有图片内存权
        w->setName(model->data(index,FindUserModel::NameRole).toString());
        QString country=model->data(index,FindUserModel::AgeRole).toString();
        QString province= model->data(index,FindUserModel::FirstLevelRegionRole).toString();
        if(country.isEmpty()||province.isEmpty()){
            QPixmap temp= QPixmap(":/images/person.png","png");
            w->setAgeAndCountry(temp);
        }
        else  w->setAgeAndCountry(country+("| ")+province);
        list.insert(realIndex+i,w);
        QString myqq=model->data(index,FindUserModel::MyqqRole).toString();
        connect(w,&UserWidget::imgClicked,this,[=](){
            qDebug()<<"imgClicked:\nmyqq="<<myqq;
            emit  imgClicked(myqq);
        });
        connect(w,&UserWidget::nameClicked,this,[=](){
            qDebug()<<"nameClicked:\nmyqq="<<myqq;
            emit nameClicked(myqq);
        });
        connect(w,&UserWidget::addButtonClicked,this,[=](){
            qDebug()<<"addButtonClicked:\nmyqq="<<myqq<<index;
            emit addButtonClicked(index);
        });
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
                    w->setAgeAndCountry(model->data(index,FindUserModel::AgeRole).toString()+("| ")+
                                        model->data(index,FindUserModel::FirstLevelRegionRole).toString());
                    break;
                case FindUserModel::FirstLevelRegionRole:
                    w->setAgeAndCountry(model->data(index,FindUserModel::AgeRole).toString()+("| ")+
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


//参数表示页按钮是否显示  用于调整fame大小和滚动栏的值
qint32 UserView::adjustFrameSize(const qint32&row,const bool &b)
{
    qint32 last=sliderMaxValue;//滚动栏最大值
    if(list.count()==0||row<=0)//设置frame大小默认值
        frame->resize(this->width(),this->height());
    else{
        UserWidget*w=list.at(0);
        if(b){
            frame->resize(this->width(),20+row*w->height()+verticalSpacing*(row-1)+buttonFrame->height());
            //注意当frame重分配大小时，frame会向上移动即滚动栏的反方向，会导致坐标系统偏差sliderMaxValue大小
            //即使frame重新移动到起点位置或者重新设置this->setWidget(frame);,视距口内的坐标也比实际位置偏差sliderMaxValue大小，所以位置修改都需要修正，即位置-sliderMaxValue
            buttonFrame->move(0,frame->height()-buttonFrame->height()-last);//位置-sliderMaxValue,抵消重分配对坐标系统的影响
            buttonFrame->show();
        }else{
            buttonFrame->hide();
            frame->resize(this->width(),20+row*w->height()+verticalSpacing*(row-1));
        }
    }
    sliderMaxValue=frame->height()-this->height();//默认的滚动栏滑块最大值为当前控件大小-滚动区域大小
    frame->move(0,0);//移动到0，抵消非自发移动对frame的影响影响
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
    if(list.count()<r*c){//出现逻辑错误返回
        oneBtn->setChecked(false);
        return;
    }
    twoBtn->setChecked(false);
    oneBtn->setChecked(true);
    nextBtn->setVisible(true);
    lastBtn->setVisible(false);
    clearLayoutElements();
    page=0;
    adjustFrameSize(r,true);//调整大小和显示按钮
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
    if(count>15*c&&count<=30*c){//2页数据 直接换页
        twoBtn->setChecked(true);
        oneBtn->setChecked(false);
        nextBtn->setVisible(false);
        lastBtn->setVisible(true);
        clearLayoutElements();
        page=1;
        adjustFrameSize(-1,true);//显示按钮,设置|滚动区大小为默认值
        verticalScrollBar()->setValue(0);//回到顶部
        int row=count/c-15,column=count%c;
        if(column!=0)row+=1;
        for (int i=0;i<row-1;i++)
            for(int j=0;j<c;j++){
                list.at(index+i*c+j)->show();//显示已经分配好的控件
            }
        for(int i=0;i<column;i++){
            list.at(index+row*c+i)->show();//显示已经分配好的控件
        }
    }else if(count==index){//1页数据 要获取数据并处理
        //预设下一次可见时状态
        twoBtn->setChecked(true);
        oneBtn->setChecked(false);
        nextBtn->setVisible(false);
        lastBtn->setVisible(true);
        clearLayoutElements();//隐藏内容
        page=1;
        adjustFrameSize(-1);//调整大小和滚动最大值并隐藏按钮
        verticalScrollBar()->setValue(0);
        continueGetList();//继续获得数据
    }else{//出现逻辑错误
        twoBtn->setChecked(false);//按钮恢复原状态
    }
}

void UserView::oneButtonClicked()
{
    bool b=oneBtn->isChecked();
    if(b){//不是当前页
        lastButtonClicked();
    }else{//当前页
        oneBtn->setChecked(!b);
        verticalScrollBar()->setValue(0);
    }
}

void UserView::twoButtonClicked()
{
    bool b=twoBtn->isChecked();
    if(b){//不是当前页
        nextButtonClicked();
    }else{//当前页
        twoBtn->setChecked(!b);
        verticalScrollBar()->setValue(0);
    }
}

void UserView::removeItem(const int &r, const int &c)
{
    int count=model->columnCount();
    UserWidget*rw= list.takeAt(r*count+c);//删除指定项
    rw->hide();
    rw->deleteLater();
    int pos=r*count+c;

    if(pos>=60){//第二页数据
        if(lastBtn->isVisible()){//第二页
            count=list.length();
            for (int var = pos; var <count; ++var) {
                UserWidget*w=list.at(var);
                QModelIndex ti=model->index(var-60);
                int i=ti.row(),j=ti.column();
                w->move(12+i*(w->width()+horizotalSpacing),10+j*(w->height()+verticalSpacing));
                w->show();
            }
            //调整
            QModelIndex index=model->lastItemIndex();
            int r=index.row();
            qint32 sliderValue=0;
            sliderValue= adjustFrameSize(r-14,true);//调整时按钮显示

            return;
        }else{
            return;
        }
    }else{
        if(list.size()<60||nextBtn->isVisible()){
            count=list.length();
            if(count>=60)count=60;//限制为第一页
            for (int var = pos; var <count; ++var) {
                UserWidget*w=list.at(var);
                QModelIndex ti=model->index(var);
                int i=ti.row(),j=ti.column();
                w->move(12+i*(w->width()+horizotalSpacing),10+j*(w->height()+verticalSpacing));
                w->show();
            }
            QModelIndex index=model->lastItemIndex();
            int r=index.row();
            if(r>=15)r=14;//一页最大显示15行
            qint32 sliderValue=0;
                if(list.count()>=15*c){
                    sliderValue= adjustFrameSize(r+1,true);
                }else
                    sliderValue=adjustFrameSize(r+1);
        }
    }
}

