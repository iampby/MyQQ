#ifndef FRIENDGROUPMODEL_H
#define FRIENDGROUPMODEL_H
//qml的好友组模型
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
    bool visible() const;
    void setGroupName(const QString&) ;
    bool setOnline(const QString&) ;
    bool setCount(const QString&) ;
    void setSet(const QString&);
    void setVisible(const bool&);
private:
    QString m_groupName;
    QString m_online;
    QString m_count;
    QString m_set;
    bool m_visible;
};
class FriendGroupModel:public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRoles{
        GroupRole = Qt::DisplayRole,
        OnlineRole,
        CountRole,
        SetRole,
        Visible//代理项可视性控制 4
    };
    FriendGroupModel(QObject*parent=nullptr);
~FriendGroupModel();
    QHash<int, QByteArray>roleNames()const;//角色
   Q_INVOKABLE  QVariant data(const int&i, int role = Qt::DisplayRole)const;
    Q_INVOKABLE void update(int index);//更新好友列表
    Q_INVOKABLE void update(int index1,int index2);//更新好友列表
    Q_INVOKABLE void swap(const int&i1,const int& i2);//更新好友列表
    // Basic functionality:
    QVariant data(const QModelIndex&index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // Add data:

   void insert(const int& index,const Data &data) ;
   Q_INVOKABLE  void insert(int index, const QString &group, const QString &online, const QString &count,  const QString &set,const bool&b=false) ;
    // Remove data:
    Q_INVOKABLE void remove(const int &index);


    Q_INVOKABLE  void setData(const int&i, const QVariant &value, int role = Qt::DisplayRole);
    Q_INVOKABLE bool append(const QString &group, const QString &online, const QString &count,  const QString &set,const bool&b=false);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE QVariantList getGroups();
signals:
   void groupListChanged();
private:
    QList<Data>m_dataList;
};

#endif // FRIENDGROUPMODEL_H
