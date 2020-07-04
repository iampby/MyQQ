#ifndef BIGFILESOCKET_H
#define BIGFILESOCKET_H

#include <QObject>
#include<QTcpSocket>
#include<QJsonDocument>
#include<QEventLoop>
class BigFileSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum WriteInstruct{
        NoFile,
        Img,
        Information
    };
    BigFileSocket(QObject *parent = nullptr);
    void setInstruct(const QString&);
    void setIp(const QString&arg="127.0.0.1");
    void setPort(const quint16&arg=5567);
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//����ͼƬ���ļ�

signals:
    void result(int code,const QString& folder=QString(), const QString& type=QString());
    void start();
    void finished(int code);
    void loopStop();
    void writtenInstruction();
public slots:
    void readD();
    void writeD();
    void err(QAbstractSocket::SocketError);
    void post();//��������
    void resultSlot(int code, const QString& folder, const QString& type);
public:

private:
    QEventLoop loop;//����д���Ƿ����

    QMap<QString,QByteArray>img;
    QByteArray temp;
    QString ip;
    quint16 port;
    QJsonDocument instruct;
    enum WriteInstruct m_write;
    qint64 size;
    QString imgName;
    QString myqq;
};

#endif // BIGFILESOCKET_H
