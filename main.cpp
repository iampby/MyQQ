#include <QApplication>
#include <QQmlApplicationEngine>
#include<QTranslator>
#include<QIcon>
#include<QDebug>
#include <qsqldatabase.h>
#include<qprocess.h>
#include <qsqlquery.h>
#include<qqmlcontext.h>
#include"funcc.h"
#include"friendgroupmodel.h"
#include"friendmodel.h"
#include"images.h"
#include <QTextCodec>
#include<qmessagebox.h>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);//有效防止拖拽闪屏
    QApplication app(argc, argv);
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));//调用虚拟键盘,声明app之前
    qmlRegisterType<FuncC>("FuncC",1,0,"FuncC");
    qmlRegisterType<FriendGroupModel>("Model",1,0,"FriendGroupModel");
    qmlRegisterType<FriendModel>("Model",1,0,"FriendModel");
    // qmlRegisterType<HeadImgWidget>("HeadWidget",1,0,"HeadWidget");




    QQmlApplicationEngine engine;
    Images*images=new Images(&engine);//退出即毁灭
    engine.rootContext()->setContextProperty("images",images);
    engine.addImageProvider("qc",images->provider);
    QTranslator tran;
    tran.load("./qt_zh_CN.qm");
    app.installTranslator(&tran);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //  engine.load(QUrl(QStringLiteral("qrc:/main/AboutMyQQ")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
    qDebug()<<"end";
}
