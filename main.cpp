#include"funcc.h"
#include"friendgroupmodel.h"
#include"friendmodel.h"
#include"images.h"
#include"imagehelper.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include<QTranslator>
#include<QIcon>
#include<QDebug>
#include <qsqldatabase.h>
#include<qprocess.h>
#include <qsqlquery.h>
#include<qqmlcontext.h>
#include <QTextCodec>
#include<qmessagebox.h>
/*
本应用通过Qml开发界面，qwidget为辅 仅仅是一个学习借鉴作品
主要涉及qqtcuick与qwidget交互（ui及数据）,qml基础部件使用，qobject基础类使用
如qml模型视图，js原型方法等，qobject方面如共享内存，多线程，套接字（qtcpsocket、qlocalsocket），xml,json,数据库类的使用，模型视图，图元视图等
*/

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);//有效防止拖拽闪屏
    QApplication app(argc, argv);
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));//调用虚拟键盘,声明app之前
    qmlRegisterType<FuncC>("FuncC",1,0,"FuncC");
    //模型导入
    qmlRegisterType<FriendGroupModel>("Model",1,0,"FriendGroupModel");
    qmlRegisterType<FriendModel>("Model",1,0,"FriendModel");
    qmlRegisterType<FriendData>("Model",1,0,"FriendData");
    //  富文本编辑辅助类导入
    qmlRegisterType<ImageHelper>("ImageHelper",1,0,"ImageHelper");


    QQmlApplicationEngine engine;
    Images*images=new Images(&engine);//退出即毁灭
    engine.rootContext()->setContextProperty("images",images);
    engine.addImageProvider("history",images->provider1);
    engine.addImageProvider("friends",images->provider2);
    engine.addImageProvider("wall",images->provider3);
    QTranslator tran;
    tran.load("./qt_zh_CN.qm");
    app.installTranslator(&tran);
    engine.load(QUrl(("qrc:/main.qml")));
    //  engine.load(QUrl(("qrc:/main/AboutMyQQ")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
