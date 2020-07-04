#ifndef FRIENDGROUPMODEL_H
#define FRIENDGROUPMODEL_H

#include <QObject>
#include<QQuickWindow>
#include<QAbstractListModel>

class Data
{
public:
    Data();

    QString groupName() const;
    QString online() const;
    QString count() const;
    QString set() const;
    void setGroupName(const QString&) ;
    bool setOnline(const QString&) ;
    bool setCount(const QString&) ;
    void setSet(const QString&);
private:
    QString m_groupName;
    QString m_online;
    QString m_count;
    QString m_set;
};
class FriendGroupModel:public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRoles{
        GroupRole = Qt::DisplayRole,
        OnlineRole,
        CountRole,
        SetRole
    };
    FriendGroupModel(QObject*parent=nullptr);

    QHash<int, QByteArray>roleNames()const;
   Q_INVOKABLE  QVariant data(const int&i, int role = Qt::DisplayRole)const;
    // Basic functionality:
    QVariant data(const QModelIndex&index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // Add data:

    Q_INVOKABLE void insert(const int& index,const Data &data) ;
    // Remove data:
    Q_INVOKABLE void remove(const int &index);


    Q_INVOKABLE  void setData(const int&i,const QString&value, int role = Qt::DisplayRole);
    Q_INVOKABLE bool append(const QString &group, const QString &online, const QString &count,  const QString &set);
    Q_INVOKABLE int count() const;
private:
    QList<Data>m_dataList;
};

#endif // FRIENDGROUPMODEL_H