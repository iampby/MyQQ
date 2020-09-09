#include "mainwindow.h"
#include <QApplication>
#include<qdebug.h>
#include<qmessagebox.h>
#include<QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc>1){
      // setvbuf(stdout,nullptr,_IONBF,0);
        QJsonDocument info=QJsonDocument::fromJson(QString::fromUtf8(argv[1]).toUtf8());
        if(info.isObject()){
            MainWindow w;
            w.setFixedSize(846,614);
            w.setMyQQInfo(info);
            w.show();
            return a.exec();
        }
    }
        QMessageBox::information(nullptr,"test",QDir::currentPath());
        MainWindow w;
        w.setFixedSize(846,614);
        w.show();
        return a.exec();

}
