#ifndef UpdateTimer_H
#define UpdateTimer_H

#include <QObject>
#include<qmap.h>
#include<qvariant.h>
//用于定时更新好友资料
class QTcpSocket;
class  QTimer;
class QThread;
class QEventLoop;
class UpdateTimer:public QObject
{
    Q_OBJECT
public:
    enum ReceiveType{
      NoType,
       SignatureAndName,//个性和签名更新
        HeadImage,//头像更新
        Status,//状态更新
        FGrade//指示好友等级需要更新了
    };
    UpdateTimer(QObject *parent = nullptr);
    ~UpdateTimer();
    void setMyqq(const QString &arg);
    void stop();
    void start();
    void setTimerInterval(const int&);
    void startTcpScoket();
    void setIp(const QString&);
    void setPort(const quint16&);
private:
    void splitSignatureAndName(QByteArray&data);//更新签名和昵称
    void makePixmap(const QByteArray&);//保存图片数据
    void handleStatus(const QByteArray&);//状态更新
    void handleFGrade(const QByteArray&);//对好友等级数据处理
signals:
    void stopTimer();
    void startTimer();
    void emitResult(const bool &ok);
    void update();//
    void updateMyGradeInfo(int grade,QString ads);
    void updateFGrade(QVariantMap obj);//更新好友等级
public slots:
    void immediateGet();//立即获取数据一次
private slots:
    void writeInstruct();
    void readD();
public:
    QMap<QString,QPixmap>historyMap;//记录好友头像
    QMap<QString,QString>sigMap;//记录好友个性签名
    QMap<QString,QString>nameMap;//记录好友昵称
    QVariantMap statusMap;//记录状态
    QVariantMap fgrade;//记录好友等级
    int grade;//记录我的等级
    QString ads;//记录我的活跃度
private:
    QTimer*timer;
    QThread*t;
    QTcpSocket*tcpsocket;
    QString myqq;
    QString ip;
    quint16 port;
    QEventLoop* loop;
    bool isReceiving;//是否正在处理，用来防止阻塞

    qint64 size;
    QString number;//好友MyQQ
    ReceiveType r_type;//接受的数据标识
};

#endif // UpdateTimer_H
