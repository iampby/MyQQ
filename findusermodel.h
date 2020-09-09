#ifndef FINDUSERMODEL_H
#define FINDUSERMODEL_H
//自定义查找好友列表模型
#include <QObject>
#include<qbytearray.h>
#include<qabstractitemmodel.h>
#include<qdebug.h>
class FindUserData{
public:
    FindUserData(const QString &myqq=QString(), const QString& name=QString(), const QByteArray &headimg=QByteArray(),
                 const QString &age=QString(),const QString &sex=QString(), const QString &location1=QString(), const QString &location2=QString(),
                 const QString &location3=QString(), const QString &location4=QString(),const QString&signature=QString());
    QString myqq()const;
    QString name()const;
    QByteArray headimg()const;
    QString age()const;
    QString sex()const;
    QString location1()const;
    QString location2()const;
    QString location3()const;
    QString location4()const;
    QString signature()const;
    void setMyqq(const QString&arg);
    void setName(const QString&arg);
    void setHeadimg(const QByteArray&arg);
    void setAge(const QString&arg);
    void setSex(const QString&arg);
    void setLocation1(const QString&arg);
    void setLocation2(const QString&arg);
    void setLocation3(const QString&arg);
    void setLocation4(const QString&arg);
    void setSignature(const QString&arg);
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
    //打开资料界面用数据
    QString m_signature;
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
        SignatureRole
    };

    FindUserModel(QObject *parent = nullptr);
    ~FindUserModel();
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex index(const int &row);//获取一个（row，column）索引值
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    const QModelIndex lastItemIndex()const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    void insert(int row, int column, QList<FindUserData *> &value);
    void insert(int row, int column, FindUserData*);
    void removeItemOf(const int&row);//移除一个项
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowOf(const QString &myqq)const;//获取myqqrole第一次出现的索引
    void clear();
    const qint32 setColumn(const qint32& column);//最小列必须为1，否则返回-1
   int sum()const;
   void showAll()const;
signals:
    void insert(const int &row, const int &column, const int &count);
    void removeItem(const int& r,const int &c);//移除信号
private:
    QList<FindUserData*>datalist;
    qint32 m_column;//必须指定列数
};

#endif // FINDUSERMODEL_H
