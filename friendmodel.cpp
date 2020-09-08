#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "friendmodel.h"

#include <qdebug.h>


//FriendData class
FriendData::FriendData(QObject *parent):QObject(parent),
    m_myqq(QString()),m_name(QString()),m_signature(QString()),m_imgPath(QString()),m_tag(QString()),
    m_grade(QString()),m_status(QString()),m_infoSet(QString()),m_statusSet(QString())
{

}

FriendData::~FriendData()
{
    qDebug()<<"~FriendData()";
}

QString FriendData::myqq() const
{
    return m_myqq;
}
QString FriendData::name() const
{
    return m_name;
}

QString FriendData::signature() const
{
    return m_signature;
}

QString FriendData::imgPath() const
{
    return m_imgPath;
}

QString FriendData::tag() const
{
    return m_tag;
}

QString FriendData::grade() const
{
    return m_grade;
}

QString FriendData::status() const
{
    return m_status;
}

QString FriendData::infoSet() const
{
    return m_infoSet;
}

QString FriendData::statusSet() const
{
    return m_statusSet;
}

void FriendData::setMyqq(const QString &arg)
{
    m_myqq=arg;
}

void FriendData::setName(const QString &arg)
{
    m_name=arg;
}

void FriendData::setSignature(const QString &arg)
{
    m_signature=arg;
}

void FriendData::setImgPath(const QString &arg)
{
    m_imgPath=arg;
}

void FriendData::setTag(const QString &arg)
{
    m_tag=arg;
}

void FriendData::setGrade(const QString &arg)
{
    m_grade=arg;
}

void FriendData::setStatus(const QString &arg)
{
    m_status=arg;
}

void FriendData::setInfoSet(const QString &arg)
{
    m_infoSet=arg;
}

void FriendData::setStatusSet(const QString &arg)
{
    m_statusSet=arg;
}



//FriendModel class
FriendModel::FriendModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

FriendModel::~FriendModel()
{
    for (FriendData* i : m_dataList) {
        i->deleteLater();
    }
    m_dataList.clear();
    qDebug()<<"~FriendModel()";
}

void FriendModel::sort()
{
    int total=m_dataList.size();
    //Ã°ÅÝÅÅÐò status:ÔÚÏß>ÀëÏß name:a>b
    for (int var = 0; var < total-1; ++var) {
        for(int j=var;j<total-1;++j){
            QString status=m_dataList.at(j)->status();
            //ÀëÏß×´Ì¬
            QString as=m_dataList.at(j+1)->status();
            if(status=="0"){
                if(status<as){
                    m_dataList.swap(j,j+1);
                }else {
                    if(m_dataList.at(j)->name()>m_dataList.at(j+1)->name())
                        m_dataList.swap(j,j+1);
                }
            }else{
                if(as>"0")
                if(m_dataList.at(j)->name()>m_dataList.at(j+1)->name())
                    m_dataList.swap(j,j+1);
            }
        }
    }
    this->update(0,total);
}

void FriendModel::deletion()
{
    this->deleteLater();
}
int FriendModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);//ÒÆ³ý¾¯¸æ
    return m_dataList.count();
}

void FriendModel::update( int index)
{
    if(rowCount()==0)
        return;
    if(index<0)index=0;
    if(index>rowCount())index=rowCount();
    dataChanged(createIndex(index,0),createIndex(index,0));
}

void FriendModel::update(int index1,  int index2)
{
    if(index1>index2){
        qint32 temp=index2;
        index2=index1;
        index1=temp;
    }
    if(index1<0){
        index1=0;
    }
    if(index2>rowCount())
        index2=rowCount();

    dataChanged(createIndex(index1,0),createIndex(index2,0));
}

QHash<int, QByteArray> FriendModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole,QByteArray("r_name"));
    roles.insert(SignatureRole,QByteArray("r_signature"));
    roles.insert(ImgPathRole,QByteArray("r_imgPath"));
    roles.insert(TagRole,QByteArray("r_tag"));
    roles.insert(GradeRole,QByteArray("r_grade"));
    roles.insert(StatusRole,QByteArray("r_status"));
    roles.insert(InfoSetRole,QByteArray("r_infoSet"));
    roles.insert(StatusSetRole,QByteArray("r_statusSet"));
    roles.insert(MyQQRole,QByteArray("r_myqq"));
    return roles;
}

QString FriendModel::data(const int &row, int role) const
{
    if(row<0||row>=this->rowCount())
        return QVariant().toString();
    QModelIndex index=this->createIndex(row,0);
    return  this->data(index,role).toString();
}

QVariant FriendModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    if(row<0||row>=m_dataList.count())
        return QVariant();
    qDebug()<<m_dataList.at(row)->myqq();
    switch (role) {
    case MyQQRole:
        return m_dataList.at(row)->myqq();
    case NameRole:
        return m_dataList.at(row)->name();
    case SignatureRole:
        return m_dataList.at(row)->signature();
    case ImgPathRole:
        return m_dataList.at(row)->imgPath();
    case TagRole:
        return m_dataList.at(row)->tag();
    case GradeRole:
        return m_dataList.at(row)->grade();
    case StatusRole:
        return m_dataList.at(row)->status();
    case InfoSetRole:
        return m_dataList.at(row)->infoSet();
    case StatusSetRole:
        return m_dataList.at(row)->statusSet();
    default:
        return QVariant();
    }

}

int FriendModel::rowOf(const QVariant &var, int role) const
{
    int i=-1;
    switch (role) {
    case MyQQRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->myqq()==var.toString())
                return i;
        }
        break;
    case NameRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->name()==var.toString())
                return i;
        }
        break;
    case SignatureRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->signature()==var.toString())
                return i;
        }
        break;
    case ImgPathRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->imgPath()==var.toString())
                return i;
        }
        break;
    case TagRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->tag()==var.toString())
                return i;
        }
        break;
    case GradeRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->grade()==var.toString())
                return i;
        }
        break;
    case StatusRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->status()==var.toString())
                return i;
        }
        break;
    case InfoSetRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->infoSet()==var.toString())
                return i;
        }
        break;
    case StatusSetRole:
        for ( i = 0; i <m_dataList.length(); ++i) {
            if(m_dataList.at(i)->statusSet()==var.toString())
                return i;
        }
        break;
    default:
        break;
    }
    return -1;
}


//row<0  value is inserted and before first item or row >=size() value is inserted and after last item
void FriendModel::insert(int row, FriendData*value)
{
    if(row<0)
        row=0;
    else if(row>this->rowCount())
        row=this->rowCount();
    beginInsertRows(this->createIndex(row,0),row,row);
    m_dataList.insert(row,value);
    endInsertRows();
}

FriendData* FriendModel::getItem(const int index)
{
    try{
        if(index<0||index>=rowCount())
            throw -1;
    }catch(int&code){
        qDebug()<<"index is more than range of array";
    }
    FriendData* &item=m_dataList[index];

    return item;
}



void FriendModel::remove(const int& row,const  int& count)
{
    if(row<0||row>=m_dataList.size()||row<(row+count-1))
        return;
    beginRemoveRows(this->createIndex(row,0),row,row+count-1);
    for(int i=row;i<row+count;i++){
        FriendData*data=m_dataList.at(i);
        m_dataList.removeAt(i);
        delete data;
        data=nullptr;
    }
    endRemoveRows();
}

void FriendModel::setData(const int &row,const QString& value, int role)
{
    if(row<0||row>=m_dataList.size())
        return;
    FriendData*data=m_dataList.at(row);
    switch (role) {
    case MyQQRole:
        data->setMyqq(value);
        break;
    case NameRole:
        data->setName(value);
        break;
    case SignatureRole:
        data->setSignature(value);
        break;
    case ImgPathRole:
        data->setImgPath(value);
        qDebug()<<data->imgPath();
        break;
    case TagRole:
        data->setTag(value);
        break;
    case GradeRole:
        data->setGrade(value);
        break;
    case StatusRole:
        data->setStatus(value);
        this->sort();
        break;
    case InfoSetRole:
        data->setInfoSet(value);
        break;
    case StatusSetRole:
        data->setStatusSet(value);
        break;
    default:
        break;
    };
    dataChanged(createIndex(row,0),createIndex(row,0));
}

void FriendModel::append(FriendData *item)
{
    this->insert(this->rowCount(),item);
}



void FriendModel::append(const QString &myqq,const QString &name, const QString &signature,
                         const QString &imgPath, const QString &tag, const QString &grade,
                         const QString &status, const QString &infoSet, const QString &statusSet)
{
    FriendData*data=new FriendData();
    data->setMyqq(myqq);
    data->setName(name);
    data->setSignature(signature);
    data->setImgPath(imgPath);
    data->setTag(tag);
    data->setGrade(grade);
    data->setStatus(status);
    data->setInfoSet(infoSet);
    data->setStatusSet(statusSet);
    this->insert(this->rowCount(),data);
}


