#ifndef BIGFILESOCKET_H
#define BIGFILESOCKET_H

#include <QObject>
#include<QTcpSocket>
#include<QJsonDocument>

class BigFileSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum WriteInstruct{
        NoFile,
        HeadImg,
        Information
    };
    BigFileSocket(QObject *parent = nullptr);
    void setInstruct(const QString&);
    void setIp(const QString&arg="127.0.0.1");
    void setPort(const quint16&arg=5567);
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//保存图片到文件

signals:
    void result(int code);
    void start();
    void finished(int code);
public slots:
    void readD();
    void writeD();
    void err(QAbstractSocket::SocketError);
    void post();//连接主机
    void resultSlot(int code);
public:

private:
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
