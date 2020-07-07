#include "images.h"
#include<qdebug.h>
#include<qapplication.h>
#include<qdir.h>
Images::Images(QObject *parent ):QObject(parent)
{
    provider1=new QmlImageProvider;
    provider2=new QmlImageProvider;
}

Images::~Images()
{
qDebug()<<"~Images()";
}

void Images::setMyQQ(QString  arg)
{
    myqq=arg;
}



QPixmap Images::findPixmap(const QString &id)
{
QPixmap x=provider1->valueOf(id);
if(x.isNull())qDebug()<<"image is null "<<id;
    return x;
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
    provider1->images.insert(id,pixmap);
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
        fileList+="image://history/"+temp+".";
        provider1->images.insert(temp,pixmap);
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



void Images::insert(QPixmap& pix)
{
    QHash<QString,QPixmap>&imgs= provider1->images;
  QHash<QString,QPixmap>newImas;
    QHash<QString, QPixmap>::iterator i = imgs.begin();
    newImas.insert(myqq+"101",pix);
    qint64 max=0;
    while (i !=imgs.constEnd()) {
        qint64 name;
       try{
            bool b;
        name=i.key().toLongLong(&b);
        if(!b)throw false;
        }catch(bool&exp){
            qDebug()<<" name is not incorrect for history image of head ";
        }
        if(name>max)max=name;
        newImas.insert(QString("%1").arg(++name),i.value());
      ++i;
    }
    QString url;
    if(newImas.size()>18){
     newImas.remove(myqq+"119");
     url="";
    }else{
        url=QString("%1").arg(++max);
    }
 provider1->images=newImas;
 qDebug()<<"ok clicked is finished";
 emit historyImageAdded(url);
}

void Images::setPixmap2(const QString &id, const QString &pix)
{
    qDebug()<<"setPixmap2 id="<<id;
    QPixmap pixmap;
    pixmap.load(pix,"png");
    if(pixmap.isNull()){
        qDebug()<<"pixmap is null";
        return;
    }
    provider2->images.insert(id,pixmap);
}
