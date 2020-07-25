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
        SingleJson
    };
    BigFileSocket(QObject *parent = nullptr);
   ~ BigFileSocket();
    void setInstruct(const QString&);
    void setIp(const QString&arg="127.0.0.1");
    void setPort(const quint16&arg=5567);
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//保存图片到文件
void setTimeout(const qint64&t);
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
    void post();//连接主机
    void resultSlot(int code, const QString& folder, const QString& type);
public:
    QEventLoop loop;//控制写入是否继续
    QJsonDocument instruct;
    QByteArray carrier;//数据载体
    QMap<QString,QByteArray>img;//图片集合

private:
    QString ip;
    quint16 port;
    enum WriteInstruct m_write;
    qint64 size;
    QString imgName;
    QString myqq;
    qint64 timeout;
};

#endif // BIGFILESOCKET_H
