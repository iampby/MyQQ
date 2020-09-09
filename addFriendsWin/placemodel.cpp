#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "placemodel.h"

//数据类
PlaceData::PlaceData():m_name(QString()),m_id(qint64()),m_fid(qint64())
{

}

qint64 PlaceData::id() const
{
    return m_id;
}

qint64 PlaceData::fid() const
{
    return m_fid;
}

QString PlaceData::name() const
{
    return m_name;
}

void PlaceData::setId(const qint64 &arg)
{
    m_id=arg;
}

void PlaceData::setFid(const qint64 &arg)
{
    m_fid=arg;
}

void PlaceData::setName(const QString &arg)
{
    m_name=arg;
}

//模型类，保存地点数据
PlaceModel::PlaceModel(QObject *parent):QAbstractListModel(parent)
{

}

PlaceModel::PlaceModel(PlaceModel &other)
{
    for (PlaceData i :other.datalist) {
        this->append(i.id(),i.name(),i.fid());
    }
}

PlaceModel &PlaceModel::operator =(const PlaceModel &arg)
{
    if(this==&arg)
        return *this;
    this->remove(0,this->rowCount());
    for (PlaceData i: arg.datalist) {
        this->append(i.id(),i.name(),i.fid());
    }
    return *this;
}




void PlaceModel::append(const qint64 &id, const QString &name, const qint64 &fid)
{
    PlaceData data;
    data.setId(id);
    data.setFid(fid);
    data.setName(name);
    datalist.append(data);
}

int PlaceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return datalist.count();
}

QVariant PlaceModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    if(row<0||row>=datalist.size())
        return QVariant();
    switch (role) {
    case NameRole:
        return datalist.at(row).name();
    case IdRole:
        return datalist.at(row).id();
    case FidRole:
        return datalist.at(row).fid();
    default:
        return QVariant();
    }
}

void PlaceModel::insert(int row, const PlaceData &value)
{
    if(row<0)
        row=0;
    else if(row>=this->rowCount(QModelIndex()))
        row=this->rowCount(QModelIndex());
    beginInsertRows(this->createIndex(row,0),row,row);
    datalist.insert(row,value);
    endInsertRows();
}

void PlaceModel::remove(const int &row, const int &count)
{
    if(row<0||row>=datalist.size()||row<(row+count-1))
        return;
    beginRemoveRows(this->createIndex(row,0),row,row+count-1);
    for(int i=row;i<row+count;i++)
        datalist.removeAt(i);
    endRemoveRows();
}

void PlaceModel::setData(const int &row, QVariant value, DataRole role)
{
    if(row<0||row>=datalist.size())
        return;
    PlaceData newValue;
    PlaceData oldValue=datalist.at(row);
    switch (role) {
    case NameRole:
        newValue.setFid(oldValue.fid());
        newValue.setId(oldValue.id());
        newValue.setName(value.toString());
        break;
    case IdRole:
        newValue.setFid(oldValue.fid());
        newValue.setId(value.toLongLong());
        newValue.setName(oldValue.name());
        break;
    case FidRole:
        newValue.setFid(value.toLongLong());
        newValue.setId(oldValue.id());
        newValue.setName(oldValue.name());
        break;
    }
}

PlaceData PlaceModel::data(const int &row) const
{
    return datalist.at(row);
}











