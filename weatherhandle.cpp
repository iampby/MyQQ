#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "weatherhandle.h"
#include<qdebug.h>
#include <qhostinfo.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qregularexpression.h>

#include <QtSql/qsqlquery.h>

WeatherHandle::WeatherHandle(QObject *parent) : QObject(parent)
{
    totalGeoAddr="";
}

WeatherHandle::~WeatherHandle()
{
    qDebug()<<"~WeatherHandle()";
}


void WeatherHandle::getIPGeoAdress()
{
    QNetworkAccessManager*manager=new QNetworkAccessManager();
    QUrl url;

    url.setUrl("http://www.ip138.com/");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");


    QNetworkReply*reply=manager->get(request);

    connect(manager,&QNetworkAccessManager::finished,this,[=](){
        QByteArray data;
        data.resize(1024);
        data=reply->readLine(1024);
        qDebug()<<QString::fromUtf8(data.data());
        while (!data.isEmpty()) {
            data=reply->readLine(1024);
            qDebug()<<QString::fromUtf8(data.data());
            QRegularExpression IPRegex("(?:<iframe src=\")(.+)(?:\" rel=\".+\" width=\".+\" height="
                                       "\".+\" frameborder=\".+\" scrolling=\".+\">)(</iframe>)");
            QRegularExpressionMatch IPM = IPRegex.match(QString::fromUtf8(data.data()));
            if(IPM.hasMatch()){
                QString cityUrl;
                cityUrl="http:"+IPM.captured(1);
                qDebug()<<cityUrl;
                QNetworkAccessManager*getCityManager=new QNetworkAccessManager;
                QNetworkRequest cityRequest((QUrl(cityUrl)));
                cityRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                                      "application/x-www-form-urlencoded");
                QNetworkReply*cityReply=getCityManager->get(cityRequest);
                connect(getCityManager,&QNetworkAccessManager::finished,this,[=](){
                    QString str="";
                    str=cityReply->readAll();
                    QRegularExpression r("(?:.+iP.+ )(?:[\u4E00-\u9FA5]{3})(.+)(?: .+)");
                    QRegularExpressionMatch m = r.match(str);
                    if(m.isValid()){
                        totalGeoAddr=m.captured(1);
                        qDebug()<<"adress:"<<m.captured(1);
                        analysisIPGeoAddr(totalGeoAddr);
                    }
                });
                break;
            }
        }
        //测试 因为网址好像不行，所以加了这句
        analysisIPGeoAddr(totalGeoAddr);
    });
}


