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
        Png
    };
    WriteThread(qintptr socketDescriptor,QObject *parent = nullptr);
    ~WriteThread();
    bool adjustHistoryImg(QByteArray&bytes,const QString&filePath,const QString&fileType);
signals:
    void error(QAbstractSocket::SocketError);
    void finished();
    void loopStop();
public slots:

private slots:
      void readD();
      void disconnected();
private:
    qintptr socketDescriptor;
    QTcpSocket*tcpsocket;
    QEventLoop loop;
    qint64 size;
    FileType FT;
    QByteArray png;
    QString fileName;
    QString myqq;
};

#endif // WRITETHREAD_H
