#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "findusermodel.h"
#include<qdebug.h>
#include<iostream>
//FindUserData class
FindUserData::FindUserData(const QString &myqq, const QString& name, const QByteArray &headimg, const QString &age,
                           const QString &sex, const QString &location1, const QString &location2,
                           const QString &location3, const QString &location4, const QString &signature)
{
    m_myqq=myqq,m_name=name,m_headimg=headimg,m_age=age,m_sex=sex,m_location1=location1;
    m_location2=location2,m_location3=location3,m_location4=location4;m_signature=signature;
}

QString FindUserData::myqq() const
{
    return m_myqq;
}

QString FindUserData::name() const
{
    return m_name;
}

QByteArray FindUserData::headimg() const
{
    return m_headimg;
}

QString FindUserData::age() const
{
    return m_age;
}

QString FindUserData::sex() const
{
    return m_sex;
}

QString FindUserData::location1() const
{
    return m_location1;
}

QString FindUserData::location2() const
{
    return m_location2;
}

QString FindUserData::location3() const
{
    return m_location3;
}

QString FindUserData::location4() const
{
    return m_location4;
}

QString FindUserData::signature() const
{
    return m_signature;
}

void FindUserData::setMyqq(const QString &arg)
{
    m_myqq=arg;
}

void FindUserData::setName(const QString &arg)
{
    m_name=arg;
}

void FindUserData::setHeadimg(const QByteArray &arg)
{
    m_headimg=arg;
}

void FindUserData::setAge(const QString &arg)
{
    m_age=arg;
}

void FindUserData::setSex(const QString &arg)
{
    m_sex=arg;
}

void FindUserData::setLocation1(const QString &arg)
{
    m_location1=arg;
}

void FindUserData::setLocation2(const QString &arg)
{
    m_location2=arg;
}

void FindUserData::setLocation3(const QString &arg)
{
    m_location3=arg;
}

void FindUserData::setLocation4(const QString &arg)
{
    m_location4=arg;
}

void FindUserData::setSignature(const QString &arg)
{
    m_signature=arg;
}



//FindUserModel class count of column default to four
FindUserModel::FindUserModel(QObject *parent):
    QAbstractItemModel(parent),m_column(qint32(4))
{

}

FindUserModel::~FindUserModel()
{
    for (FindUserData*v : datalist) {
        if(v){
            delete v;
            v=nullptr;
        }
    }
}


QModelIndex FindUserModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->createIndex(row,column,(void*)this);
}

QModelIndex FindUserModel::index(const int& row)
{
    if(row<0||row>=datalist.size())return QModelIndex();
    int r=0,c=0;
    r=row/columnCount();
    if(row%columnCount()==0)c=0;
    else c=row%columnCount();
    QModelIndex index=this->createIndex(r,c);
    return index;
}

int FindUserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);//忽略警告
    qint32 row=0;
    row=datalist.count()/m_column;
    if(datalist.count()%m_column!=0)
        row+=1;
    return row;
}

int FindUserModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_column;
}

const QModelIndex FindUserModel::lastItemIndex() const
{
    int sum=datalist.count();
    if(sum==0)return QModelIndex();
    int c=columnCount()-(rowCount()*columnCount()-sum)-1;
    qDebug()<<"lastindex(row,column):"<<rowCount()-1<<c;
    return createIndex(rowCount()-1,c);
}

QVariant FindUserModel::data(const QModelIndex &index, int role) const
{
    qint32 realIndex=index.row()*m_column+index.column();
    if(realIndex<0||realIndex>=datalist.count())
        return QVariant();
    switch (role) {
    case MyqqRole:
        return QVariant(datalist.at(realIndex)->myqq());
    case NameRole:
        return QVariant(datalist.at(realIndex)->name());
    case HeadImgRole:
        return QVariant(datalist.at(realIndex)->headimg());
    case AgeRole:
        return QVariant(datalist.at(realIndex)->age());
    case SexRole:
        return QVariant(datalist.at(realIndex)->sex());
    case FirstLevelRegionRole:
        return QVariant(datalist.at(realIndex)->location1());
    case SecondLevelRegionRole:
        return QVariant(datalist.at(realIndex)->location2());
    case ThirdLevelRegionRole:
        return QVariant(datalist.at(realIndex)->location3());
    case FourthLevelRegionRole:
        return QVariant(datalist.at(realIndex)->location4());
    case SignatureRole:
        return QVariant(datalist.at(realIndex)->signature());
    default:
        qDebug()<<"model return QVariant()";
        return QVariant();
    }
}

QModelIndex FindUserModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

void FindUserModel::insert(int row,  int column,  QList<FindUserData*> &value)
{
    int r,c;
    QModelIndex index=lastItemIndex();
    if(index.isValid()){
        r=index.row(),c=index.column()+1;
        if(c%columnCount()==0){
            r+=1,c=0;//得到最后之后索引
        }
    }else {
        r=0,c=0;
    }
    if(column<0)column=0;
    else if(column>=columnCount()){row+=1,column=0;}
    if(row<0)row=0;
    else if((row*columnCount()+column)>=(r*columnCount()+c)){
        row=r,column=c;
    }
    int realIndex=row*m_column+column;
    for(int i=value.count()-1;i>=0;i--){
        datalist.insert(realIndex,value[i]);
    }
   emit  insert(row,column,value.count());//发射一个自定义信号提示已插入信息
}

void FindUserModel::insert(int row, int column, FindUserData *value)
{
    int r,c;
    QModelIndex index=lastItemIndex();
    if(index.isValid()){
        r=index.row(),c=index.column()+1;
        if(c%columnCount()==0){
            r+=1,c=0;//得到最后之后索引
        }
    }else {
        r=0,c=0;
    }
    if(column<0)column=0;
    else if(column>=columnCount()){row+=1,column=0;}
    if(row<0)row=0;
    else if((row*columnCount()+column)>=(r*columnCount()+c)){
        row=r,column=c;
    }
    int realIndex=row*m_column+column;
    datalist.insert(realIndex,value);
    insert(row,column,1);//发射一个自定义信号提示已插入信息
}

void FindUserModel::removeItemOf(const int&row)
{
  QModelIndex tm=index(row);
  datalist.removeAt(row);
  emit removeItem(tm.row(),tm.column());
}

bool FindUserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qint32 row=index.row(),column=index.column();
    if(row<0||row>=rowCount(QModelIndex())||column<0||column>=m_column)
        return false;
    qint32 realIndex=row*m_column+column;
    QVector<int>roles(1,role);
    FindUserData*item=datalist.at(realIndex);
    switch (role) {
    case MyqqRole:
        item->setMyqq(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case NameRole:
        item->setName(value.toString());
        return true;
    case HeadImgRole:
        item->setHeadimg(value.toByteArray());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case AgeRole:
        item->setAge(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case SexRole:
        item->setSex(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case FirstLevelRegionRole:
        item->setLocation1(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case SecondLevelRegionRole:
        item->setLocation2(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case ThirdLevelRegionRole:
        item->setLocation3(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case FourthLevelRegionRole:
        item->setLocation4(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    case SignatureRole:
        item->setSignature(value.toString());
        emit dataChanged(index,index,roles);//发射信号提示改变数据，定位索引index，角色role
        return true;
    default:
        return false;
    }
}

int FindUserModel::rowOf(const QString &myqq) const
{
    int length=datalist.length();
    for (int var = 0; var < length; ++var) {
        FindUserData*item=datalist.at(var);
        if(item->myqq()==myqq){
            return var;
        }
    }
    return -1;
}

void FindUserModel::clear()
{
    beginResetModel();
    for (FindUserData*v : datalist) {
        delete v;
        v=nullptr;
    }
    datalist.clear();
    endResetModel();
}



const qint32 FindUserModel::setColumn(const qint32& column)
{
    if(column<1||datalist.count()>0)
        return -1;
    m_column=column;
    return m_column;
}

int  FindUserModel::sum() const
{
    return datalist.count();
}

void FindUserModel::showAll() const
{
    for (FindUserData*v : datalist) {
        qDebug()<<"myqq="<<v->myqq();
        qDebug()<<"name="<<v->name();
        qDebug()<<"sex="<<v->sex();
        qDebug()<<"age="<<v->age();
        qDebug()<<"two="<<v->location2();
        qDebug()<<"three="<<v->location3();
    }
}
