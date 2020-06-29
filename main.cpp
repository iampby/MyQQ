#include <QApplication>
#include <QQmlApplicationEngine>
#include<QTranslator>
#include<QIcon>
#include<QDebug>
#include <qsqldatabase.h>
#include<qprocess.h>
#include <qsqlquery.h>
#include"funcc.h"
#include"friendgroupmodel.h"
#include"friendmodel.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);//有效防止拖拽闪屏
//qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));//调用虚拟键盘,声明app之前
    qmlRegisterType<FuncC>("FuncC",1,0,"FuncC");
    qmlRegisterType<FriendGroupModel>("Model",1,0,"FriendGroupModel");
    qmlRegisterType<FriendModel>("Model",1,0,"FriendModel");
   // qmlRegisterType<HeadImgWidget>("HeadWidget",1,0,"HeadWidget");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
QTranslator tran;
tran.load("./qt_zh_CN.qm");
app.installTranslator(&tran);

   engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
 //  engine.load(QUrl(QStringLiteral("qrc:/main/AboutMyQQ")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
