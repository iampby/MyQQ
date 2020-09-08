#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "friendgroupmodel.h"

Data::Data():
    m_groupName(QString()),m_online(QString()), m_count(QString()),m_set(QString())
{

}

QString Data::groupName() const
{
    return m_groupName;
}

QString Data::online() const
{
    return m_online;
}

QString Data::count() const
{
    return m_count;
}

bool Data::visible() const
{
    return m_visible;
}
QString Data::set() const
{
    return m_set;
}

void Data::setGroupName(const QString &arg)
{
    m_groupName=arg;
}

bool Data::setOnline(const QString &arg)
{
    if(arg.toInt()>m_count.toInt()){
        return false;
    }
    m_online=arg;
    return true;
}

bool Data::setCount(const QString &arg)
{
    if(m_count.toInt()<m_online){
        return false;
    }
    m_count=arg;
    return true;
}

void Data::setSet(const QString&arg)
{
    m_set=arg;
}

void Data::setVisible(const bool &b)
{
    m_visible=b;
}


FriendGroupModel::FriendGroupModel(QObject *parent):QAbstractListModel(parent)
{
    //this->setObjectName("ff");
}

FriendGroupModel::~FriendGroupModel()
{
    qDebug()<<"~FriendGroupModel()";
}

QVariant FriendGroupModel::data(const int &i, int role) const
{
    QModelIndex x= this->createIndex(i,0);
    return this->data(x,role);

}

void FriendGroupModel::update(int index)
{
    if(rowCount()==0)
        return;
    if(index<0)index=0;
    if(index>rowCount())index=rowCount();
    dataChanged(createIndex(index,0),createIndex(index,0));
}

void FriendGroupModel::update(int index1, int index2)
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

void FriendGroupModel::swap(const int &i1, const int &i2)
{
    if(i1>i2){
        m_dataList.move(i2,i1);
        this->update(i2,i1);
    }else{
        m_dataList.move(i1,i2);
        this->update(i1,i2);
         emit groupListChanged();
    }
}

void FriendGroupModel::setData(const int &i, const QVariant &value, int role)
{
    if(m_dataList.size()<=i||i<0)
        return;
    Data& newData=m_dataList[i];
    switch (role) {
    case GroupRole:
        newData.setGroupName(value.toString());
        emit groupListChanged();
        break;
    case OnlineRole:
        newData.setOnline(value.toString());
        break;
    case CountRole:
        newData.setCount(value.toString());
        break;
    case SetRole:
        newData.setSet(value.toString());
        break;
    case Visible:
        newData.setVisible(value.toBool());
        qDebug()<<"a friend group had changed to "<<value.toBool()<<" on visible ";
        break;
    default:
        return;
    }
   // m_dataList.replace(i,newData);//有时会导致视图自动释放指针内存 不知道为啥
    dataChanged(createIndex(i,0),createIndex(i,0));
}

int FriendGroupModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.count();
}

QVariant FriendGroupModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_dataList.count()) {
        return QVariant();
    }

    const Data &data = m_dataList[row];
    switch (role) {
    case GroupRole:
        return data.groupName();
    case OnlineRole:
        return data.online();
    case CountRole:
        return data.count();
    case SetRole:
        return data.set();
    case Visible:
        return data.visible();
    }
    return QVariant();
}


QHash<int, QByteArray> FriendGroupModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(GroupRole,QByteArray("r_group"));
    roles.insert(OnlineRole,QByteArray("r_online"));
    roles.insert(SetRole,QByteArray("r_set"));
    roles.insert(CountRole,QByteArray("r_count"));
    roles.insert(Visible,QByteArray("r_visible"));
    return roles;
}

void FriendGroupModel::insert(const int& index, const Data &data)
{
    if(index < 0 || index > m_dataList.count()) {
        return;
    }

    beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, data);
    endInsertRows();
     emit groupListChanged();
}

void FriendGroupModel::insert( int index, const QString &group, const QString &online, const QString &count, const QString &set,const bool&b)
{
    if(index < 0 ) {
      index=0;
    }
    else if(index > m_dataList.count()){
        index=m_dataList.count();
    }
Data d;
d.setGroupName(group);
d.setOnline(online);
d.setCount(count);
d.setSet(set);
d.setVisible(b);
    beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, d);
    endInsertRows();
     emit groupListChanged();
}

void FriendGroupModel::remove(const int& index)
{
    if(index < 0 || index >= m_dataList.count()) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_dataList.removeAt(index);
    endRemoveRows();
    emit groupListChanged();
}





bool FriendGroupModel::append(const QString &group,const QString &online, const QString &count,const QString &set,const bool&b)
{
    Data d;
    d.setGroupName(group);
    bool ok=true;
    ok=ok&&d.setOnline(online);
    ok=ok&&d.setCount(count);
    d.setSet(set);
    d.setVisible(b);
    if(ok)
        insert(this->count(), d);
    return ok;
}


int FriendGroupModel::count() const
{
    return rowCount(QModelIndex());
}

QVariantList FriendGroupModel::getGroups()
{
    QVariantList temp;
    qint32 length=m_dataList.length();
    for(qint32 i=0;i<length;++i){
        temp.append(QVariant( m_dataList.at(i).groupName()));
    }
    return temp;
}


