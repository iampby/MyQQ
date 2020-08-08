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


FriendGroupModel::FriendGroupModel(QObject *parent):QAbstractListModel(parent)
{
    //this->setObjectName("ff");
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
    if(index>=rowCount())index=rowCount()-1;
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
    }
}

void FriendGroupModel::setData(const int &i, const QString &value, int role)
{
    if(m_dataList.size()<=i||i<0)
        return;
    Data newData;
    Data oldData=m_dataList.at(i);
    switch (role) {
    case GroupRole:
        newData.setCount(oldData.count());
        newData.setOnline(oldData.online());
        newData.setSet(oldData.set());
        newData.setGroupName(value);
        break;
    case OnlineRole:
        newData.setCount(oldData.count());
        newData.setGroupName(oldData.groupName());
        newData.setSet(oldData.set());
        newData.setOnline(value);
        break;
    case CountRole:
        newData.setGroupName(oldData.groupName());
        newData.setOnline(oldData.online());
        newData.setSet(oldData.set());
        newData.setCount(value);
        break;
    case SetRole:
        newData.setCount(oldData.count());
        newData.setOnline(oldData.online());
        newData.setGroupName(oldData.groupName());
        newData.setSet(value);
        break;
    default:
        return;
    }
    m_dataList.replace(i,newData);
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
        break;
    case OnlineRole:
        return data.online();
        break;
    case CountRole:
        return data.count();
        break;
    case SetRole:
        return data.set();
        break;
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
}

void FriendGroupModel::insert( int index, const QString &group, const QString &online, const QString &count, const QString &set)
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
    beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, d);
    endInsertRows();
}

void FriendGroupModel::remove(const int& index)
{
    if(index < 0 || index >= m_dataList.count()) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_dataList.removeAt(index);
    endRemoveRows();
}





bool FriendGroupModel::append(const QString &group,const QString &online, const QString &count,const QString &set)
{
    Data d;
    d.setGroupName(group);
    bool ok=true;
    ok=ok&&d.setOnline(online);
    ok=ok&&d.setCount(count);
    d.setSet(set);
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


