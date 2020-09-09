#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
//qml的好友模型
#include <QObject>
#include<QAbstractListModel>

class FriendData:public QObject
{
    Q_OBJECT
public:
    FriendData(QObject *parent = nullptr);
    ~FriendData();
    Q_INVOKABLE  QString myqq()const;//qml使用
    Q_INVOKABLE  QString name()const;
    Q_INVOKABLE  QString signature()const;
    Q_INVOKABLE  QString imgPath()const;
    Q_INVOKABLE  QString tag()const;
    Q_INVOKABLE  QString grade()const;
    Q_INVOKABLE   QString status()const;
    Q_INVOKABLE  QString infoSet()const;
    Q_INVOKABLE  QString statusSet()const;
    void setMyqq(const QString&);
    void setName(const QString&);
    void setSignature(const QString&);
    void setImgPath(const QString&);
    void setTag(const QString&);
    void setGrade(const QString&);
    void setStatus(const QString&);
    void setInfoSet(const QString&);
    void setStatusSet(const QString&);
private:
    QString m_myqq;//MyQQ
    QString m_name;//昵称
    QString m_signature;//个性签名
    QString m_imgPath;//头像路径
    QString m_tag;//备注
    QString m_grade;//myqq等级
    QString m_status;//状态
    QString m_infoSet;//信息设置
    QString m_statusSet;//状态设置
};

class FriendModel:public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRoles{
        MyQQRole=Qt::DisplayRole,//0
        NameRole,
        SignatureRole,
        ImgPathRole,
        TagRole,
        GradeRole,
        StatusRole,//6
        InfoSetRole,
        StatusSetRole,//8
        Visible//代理项 可视化控制
    };
    FriendModel(QObject *parent = nullptr);
    ~FriendModel();
    //基础函数
    Q_INVOKABLE  int rowCount(const QModelIndex &parent=QModelIndex()) const;
    Q_INVOKABLE void update(int index);
    Q_INVOKABLE void update(int index1, int index2);
    QHash<int,QByteArray>roleNames()const;
    Q_INVOKABLE QString data(const int &, int role=0 ) const;
    QVariant data(const QModelIndex &index, int role) const;//纯虚函数必须实现
    Q_INVOKABLE int rowOf(const QVariant&var , int role=MyQQRole) const;//返回角色名的值所在行
    //修改数据
    Q_INVOKABLE void sort();//排序
    Q_INVOKABLE  void deletion();//谨慎使用，删除内存并永不使用对象
    void insert(int row, FriendData *value);
//注意不要从列表删除并取出对象赋给另一个模型，这会导致qml代理可视变化时自动析构（有时）,这里是为了取出对象 获取属性添加到另一个模型
    Q_INVOKABLE FriendData* getItem(const int index);//返回删除项 不释放内存
    Q_INVOKABLE  void remove(const int& row,const int&count=1);//删除一个对象
    Q_INVOKABLE  void setData(const int &row,const QString& value, int role=MyQQRole);
    Q_INVOKABLE  void append(FriendData*item);//末尾添加
    Q_INVOKABLE void append(const QString &myqq, const QString&name, const QString&signature, const QString&imgPath,
                            const QString&tag,const QString&grade, const QString&status, const QString&infoSet, const QString&statusSet);
private:
    QList<FriendData*>m_dataList;
};

#endif // FRIENDMODEL_H
