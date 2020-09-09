#ifndef PLACEMODEL_H
#define PLACEMODEL_H

#include <QObject>
#include<QAbstractListModel>
class PlaceData{
public:
    PlaceData();
    qint64 id()const;
    qint64 fid()const;
    QString name()const;
    void setId(const qint64&);
    void setFid(const qint64&);
    void setName(const QString&);
private:
    qint64 m_id;
    QString m_name;
    qint64 m_fid;

};

class PlaceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum  DataRole{
        NameRole=Qt::DisplayRole,
        IdRole,
        FidRole
    };
    PlaceModel(QObject *parent = nullptr);
    PlaceModel(PlaceModel&);
  PlaceModel&operator =(const PlaceModel&);

    void append(const qint64&id=0,const QString&name="",const qint64&fid=-1);//-1´ú±íÎÞ
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void insert(int row, const PlaceData &value);
    void remove(const int& row,const int&count=1);
    void setData(const int &row, QVariant value, DataRole role);
    PlaceData data(const int &row) const;
private:
    QList<PlaceData>datalist;
};

#endif // PLACEMODEL_H
