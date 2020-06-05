#ifndef FINDUSERMODEL_H
#define FINDUSERMODEL_H

#include <QObject>
#include<qbytearray.h>
#include<qabstractitemmodel.h>
class FindUserData{
public:
    FindUserData(const QString &myqq=QString(), const QString& name=QString(), const QByteArray &headimg=QByteArray(),
                 const QString &age=QString(),const QString &sex=QString(), const QString &location1=QString(), const QString &location2=QString(),
                 const QString &location3=QString(), const QString &location4=QString());
    QString myqq()const;
    QString name()const;
    QByteArray headimg()const;
    QString age()const;
    QString sex()const;
    QString location1()const;
    QString location2()const;
    QString location3()const;
    QString location4()const;
    void setMyqq(const QString&arg);
    void setName(const QString&arg);
    void setHeadimg(const QByteArray&arg);
    void setAge(const QString&arg);
    void setSex(const QString&arg);
    void setLocation1(const QString&arg);
    void setLocation2(const QString&arg);
    void setLocation3(const QString&arg);
    void setLocation4(const QString&arg);
private:
    QString m_myqq;
    QString m_name;
    QByteArray m_headimg;
    QString m_age;
    QString m_sex;
    QString m_location1;
    QString m_location2;
    QString m_location3;
    QString m_location4;
};

class FindUserModel:public QAbstractItemModel
{
    Q_OBJECT
public:
    enum DataRole{
        MyqqRole=Qt::UserRole,
        NameRole,
        HeadImgRole,
        AgeRole,
        SexRole,
        FirstLevelRegionRole,
        SecondLevelRegionRole,
        ThirdLevelRegionRole,
        FourthLevelRegionRole,
    };
    FindUserModel(QObject *parent = nullptr);
    ~FindUserModel();
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    const QModelIndex lastItemIndex()const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    void insert(int row, int column, QList<FindUserData *> &value);
    void insert(int row, int column, FindUserData*);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void clear();
    const qint32 setColumn(const qint32& column);//最小列必须为1，否则返回-1
   int sum()const;
signals:
    void insert(const int &row, const int &column, const int &count);
private:
    QList<FindUserData*>datalist;
    qint32 m_column;//必须指定列数
};

#endif // FINDUSERMODEL_H
