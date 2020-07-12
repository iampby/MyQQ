#include "myqqloginserver.h"
#include"global.h"
#include"loginthread.h"

#include <qthread.h>
#include <qtimer.h>
MyQQLoginServer::MyQQLoginServer(QObject *parent)
    :QTcpServer(parent)
{
    count=100;
}

MyQQLoginServer::~MyQQLoginServer()
{
    qDebug()<<"~MyQQLoginServer()";
}

void MyQQLoginServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"viaed incomingC"<<endl;
    QThread*thread=new QThread();
    //����һ���ڿ��Դ򿪵����ݿ�������
    if(count>=200)
        count=100;
    LoginThread*loginHandle=new LoginThread(socketDescriptor,count++);
    loginHandle->moveToThread(thread);//�ƶ������̣߳�ͨ���źŲۻ��ư��̴߳ӵ㵽��
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::finished,loginHandle,&LoginThread::deleteLater);
    connect(loginHandle,&LoginThread::finished,loginHandle,[=](){
        thread->exit(0);
        thread->quit();
        qDebug()<<"login thread exit";
    });
     //ɾ��historyImgMuter�������е�һ����
    connect(loginHandle,&LoginThread::delayedHeadImgDeletion,this,[=](const QString number){
        //�ӳ�5sɾ��
        QTimer::singleShot(5000,this,[=](){
            if(!historyImgFiles.contains(number)){
                QMutex*muter=historyImgMuter.value(number);
                historyImgMuter.remove(number);//���û�и��±���ļ�������ɾ����
                delete muter,muter=nullptr;
            }
        });
    });
    //ɾ���������е�һ����
    connect(loginHandle,&LoginThread::delayedSigAndNameDeletion,this,[=](const QString number){
        //�ӳ�5sɾ��
        QTimer::singleShot(5000,this,[=](){
            if(!sigFiles.contains(number)){
                 QMutex*muter=sigMuter.value(number);
                sigMuter.remove(number);//���û�и��±���ļ�������ɾ����
                   delete muter,muter=nullptr;
            }
        });
    });
    thread->start();
    emit loginHandle->startTimer();
}
