#include "images.h"
#include<qdebug.h>
#include<qapplication.h>
#include<qdir.h>
Images::Images(QObject *parent ):QObject(parent)
{
    provider1=new QmlImageProvider;
    provider2=new QmlImageProvider;
    provider3=new QmlImageProvider;
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
    if(x.isNull())qDebug()<<"image is null: "<<id;
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
    QHash<QString, QPixmap>::const_iterator i = imgs.cbegin();
    QHash<QString, QPixmap>::const_iterator end = imgs.cend();
    newImas.insert(myqq+"101",pix);
    qint64 max=0;
    while (i !=end) {
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
    emit historyImageAdded(url,newImas.size()-1);
}

void Images::setPixmap2(const QString &id, const QString &pix,const QString &status)
{
    qDebug()<<"setPixmap2 id="<<id;
    QPixmap pixmap;
    pixmap.load(pix,"png");
    if(pixmap.isNull()){
        qDebug()<<"pixmap is null";
        return;
    }
    provider2->images.insert(id,pixmap);
    provider2->control.insert(id,status);
}

bool Images::setPixmap3(const QString& id, const QString&filename)
{
    QPixmap pix;
    pix.load(filename);
    if(pix.isNull()){
        qDebug()<<"added a  pixmap of photo-wall is of failure in: "<<filename;
        return false;
    }
    provider3->images.insert(id,pix);
    return true;
}

bool Images::removePixmap3(int pos)
{
  int c=provider3->images.size();
    for(int i=pos+1;i<c;++i){
        provider3->images.insert(QString("%1").arg(i-1),provider3->valueOf(QString("%1").arg(i-1)));
    }
  c= provider3->images.remove(QString("%1").arg(c-1));
 if(c==0)return false;
 else
     return true;
}
//添加count个照片到照片墙提供器
qint32 Images::insertPixmap3(const int &count, QVector<QString> files)
{
    QHash<QString,QPixmap>temp;
    QVector<QPixmap>add;
    for(int i=count-1;i>=0;--i){
        QPixmap pix;
        if(pix.load(files.at(i))){
            qDebug()<<"loaded image in  photo wall successfully,named "<<files.at(i);
            add.append(pix);
        }else{
            qDebug()<<"loaded image in  photo wall unsuccessfully,named "<<files.at(i);
        }
    }
    qint32 length=add.size();
    for( qint32 i=0;i<length;++i){
        temp.insert(QString("%1").arg(i),add[i]);
    }
    QHash<QString, QPixmap>::const_iterator i =provider3->images.cbegin();
    QHash<QString, QPixmap>::const_iterator end = provider3->images.cend();

    while (i !=end) {
        qint8 t= i.key().toInt();
        t+=length;
        temp.insert(QString("%1").arg(t),i.value());
        ++i;
    }
    provider3->images=temp;//替换数据
    return length;
}

