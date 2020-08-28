#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QObject>
#include<qtcpsocket.h>
#include<QEventLoop>
#include<qsqlquery.h>
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
        DMRFGroup,//ɾ�� �ƶ� ���������ѷ���
        DMRFiend,//�Ժ��� ɾ�� �ƶ� �޸ı�ע
        FMessageXml//������Ϣ
    };
    WriteThread(qintptr socketDescriptor,qint64 count,QObject *parent = nullptr);
    ~WriteThread();
    QSqlQuery openDB(bool &ok)const;
    bool userExit();//�û��˳��ı�״̬
    bool adjustHistoryImg(QByteArray&bytes,const QString&filePath,const QString&fileType);//������ʷͷ��
    bool updateSignature(QByteArray&bytes);//���¸���ǩ��
    bool updateCover(QByteArray&bytes);//�����û����Ϸ���
    bool updateWall(QByteArray&bytes);//�����û���Ƭǽ
    bool updateWall(int& pos);//�����û���Ƭǽ
    bool updateUserInfo(QByteArray&bytes);//�����û���Ϣ
    bool addFGroup(QByteArray&bytes);//����û����ѷ���
    bool dmrFGroup(QByteArray&bytes);//�����û����ѷ���
    bool dmrFriend(QByteArray&bytes);//�Ժ��� ɾ�� �ƶ� �޸ı�ע ����
    bool fMessageSave(QByteArray&bytes);
private:
    bool updateSignatureHandle(QString &sig);//ѭ��д֪ͨǩ���仯�ļ�
    void  updateNameHandle(QString &name);//ѭ��д֪ͨname�仯�ļ�
    void updateXMLSignature(QString&number, QString&me, QString &sig);//��дme�� xml�ļ���ָ��myqq=numberǩ��
    void updateXMLName(QString&number, QString&me, QString &name);//��дme�� xml�ļ���ָ��myqq=number�ǳ�
    bool exitStatusHandle(QString&status);//�˳��ı�xml��ֵ
    void setXmlStatus(QString &opposite, QString &me, QString &status);//�޸�xml״̬
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
};

#endif // WRITETHREAD_H
