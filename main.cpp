#include "mainwindow.h"
#include <QApplication>
#include<qdebug.h>

int main(int argc, char *argv[])
{
    if(argc>1){
        QApplication a(argc, argv);
        QJsonDocument info=QJsonDocument::fromJson(QString::fromUtf8(argv[1]).toUtf8());
        if(info.isObject()){
            MainWindow w;
            w.setFixedSize(846,614);
            w.setMyQQInfo(info);
            w.show();
            return a.exec();
        }
    }else{
        return -1;
    }

}
