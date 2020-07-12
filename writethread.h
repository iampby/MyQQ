#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QObject>
#include<qtcpsocket.h>
#include<QEventLoop>

class WriteThread:public QObject
{
    Q_OBJECT
public:
    enum FileType{
        NoFile,
        HistoryHeadImage,
        Signature
    };
    WriteThread(qintptr socketDescriptor,qint64 count,QObject *parent = nullptr);
    ~WriteThread();
    bool adjustHistoryImg(QByteArray&bytes,const QString&filePath,const QString&fileType);
    bool updateSignature(QByteArray&bytes);
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
