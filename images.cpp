#include "images.h"
#include<qdebug.h>
#include<qapplication.h>
#include<qdir.h>
Images::Images(QObject *parent ):QObject(parent)
{
    provider=new QmlImageProvider;

}

Images::~Images()
{
qDebug()<<"images delete";
}

void Images::setMyQQ(QString  arg)
{
    myqq=arg;
}



QPixmap Images::findPixmap(const QString &id)
{

    return provider->valueOf(id);
}
void Images::setPixmap(const QString &id, const QString &pix)
{
    QPixmap pixmap;
    pixmap.load(pix,"png");
    if(pixmap.isNull()){
        qDebug()<<"pixmap is null";
        return;
    }
    qDebug()<<id<<endl;
    provider->images.insert(id,pixmap);
}

void Images::readHistory()
{
    QDir dir("../user/"+myqq+"/historyHeadImg/");
    QStringList list;
    QString fileList;
    list<<"*.png";
    list= dir.entryList(list,QDir::Files,QDir::Name);
    for (QString&v : list) {
        if(v=="01.png"){
            qDebug()<<"added not "<<v<<list.length()<<endl<<endl<<endl;
            continue;
        }
        QPixmap pixmap;
        pixmap.load(dir.absoluteFilePath(v),"png");
        QString temp=myqq+"1"+v.left(v.length()-4);
        fileList+="image://qc/"+temp+".";
        provider->images.insert(temp,pixmap);
    }
    emit readHistoryHeadImg(fileList);
}

void Images::removeHistory()
{
    QDir dir("../user/"+myqq+"/historyHeadImg/");
    if(dir.exists()){
            qDebug()<<"dir.removeRecursively()";
            dir.removeRecursively();
    }
}
