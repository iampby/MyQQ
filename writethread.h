#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QObject>
#include<qtcpsocket.h>
#include<QEventLoop>
#include<qsqlquery.h>
#include<qsqldatabase.h>
#include<qqueue.h>
class WriteThread:public QObject
{
    Q_OBJECT
public:
    enum FileType{
        NoFile,
        HistoryHeadImage,
        Signature,
        CoverImage,
        PhotoWall,
        UserInformation,
        AddFGroup,
        ExitChangeStatus,
        DMRFGroup,//删除 移动 重命名好友分组
        DMRFiend,//对好友 删除 移动 修改备注
        FMessageXml//好友消息
    };
    WriteThread(qintptr socketDescriptor,qint64 count,QObject *parent = nullptr);
    ~WriteThread();
    QSqlQuery openDB(bool &ok);
    bool userExit();//用户退出改变状态
    bool adjustHistoryImg(QByteArray&bytes,const QString&filePath,const QString&fileType);//更新历史头像
    bool updateSignature(QByteArray&bytes);//更新个性签名
    bool updateCover(QByteArray&bytes);//更新用户资料封面
    bool updateWall(QByteArray&bytes);//更新用户照片墙
    bool updateWall(int& pos);//更新用户照片墙
    bool updateUserInfo(QByteArray&bytes);//更新用户信息
    bool addFGroup(QByteArray&bytes);//添加用户好友分组
    bool dmrFGroup(QByteArray&bytes);//更新用户好友分组
    bool dmrFriend(QByteArray&bytes);//对好友 删除 移动 修改备注 操作
    bool fMessageSave(QByteArray&bytes);
private:
    bool updateSignatureHandle(QString &sig);//循环写通知签名变化文件
    void  updateNameHandle(QString &name);//循环写通知name变化文件
    void updateXMLSignature(QString&number, QString&me, QString &sig);//改写me的 xml文件的指定myqq=number签名
    void updateXMLName(QString&number, QString&me, QString &name);//改写me的 xml文件的指定myqq=number昵称
    bool exitStatusHandle(QString&status);//退出改变xml的值
    void setXmlStatus(QString &opposite, QString &me, QString &status);//修改xml状态
signals:
    void error(QAbstractSocket::SocketError);
    void finished();
    void startTimer();
public slots:
    void timer();
private slots:
    void readD();
    void disconnected();
private:
    qintptr socketDescriptor;
    qint64 count;
    QTcpSocket*tcpsocket;
    qint64 size;
    FileType FT;
    QByteArray bytes;
    QString fileName;
    QString myqq;

        QQueue<QString>dbPool;//数据库池
};

#endif // WRITETHREAD_H

