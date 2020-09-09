#ifndef UpdateTimer_H
#define UpdateTimer_H

#include <QObject>
#include<qmap.h>
#include<qvariant.h>
//���ڶ�ʱ���º�������
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
       SignatureAndName,//���Ժ�ǩ������
        HeadImage,//ͷ�����
        Status,//״̬����
        FGrade//ָʾ���ѵȼ���Ҫ������
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
    void splitSignatureAndName(QByteArray&data);//����ǩ�����ǳ�
    void makePixmap(const QByteArray&);//����ͼƬ����
    void handleStatus(const QByteArray&);//״̬����
    void handleFGrade(const QByteArray&);//�Ժ��ѵȼ����ݴ���
signals:
    void stopTimer();
    void startTimer();
    void emitResult(const bool &ok);
    void update();//
    void updateMyGradeInfo(int grade,QString ads);
    void updateFGrade(QVariantMap obj);//���º��ѵȼ�
public slots:
    void immediateGet();//������ȡ����һ��
private slots:
    void writeInstruct();
    void readD();
public:
    QMap<QString,QPixmap>historyMap;//��¼����ͷ��
    QMap<QString,QString>sigMap;//��¼���Ѹ���ǩ��
    QMap<QString,QString>nameMap;//��¼�����ǳ�
    QVariantMap statusMap;//��¼״̬
    QVariantMap fgrade;//��¼���ѵȼ�
    int grade;//��¼�ҵĵȼ�
    QString ads;//��¼�ҵĻ�Ծ��
private:
    QTimer*timer;
    QThread*t;
    QTcpSocket*tcpsocket;
    QString myqq;
    QString ip;
    quint16 port;
    QEventLoop* loop;
    bool isReceiving;//�Ƿ����ڴ���������ֹ����

    qint64 size;
    QString number;//����MyQQ
    ReceiveType r_type;//���ܵ����ݱ�ʶ
};

#endif // UpdateTimer_H
