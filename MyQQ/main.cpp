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
��Ӧ��ͨ��Qml�������棬qwidgetΪ�� ������һ��ѧϰ�����Ʒ
��Ҫ�漰qqtcuick��qwidget������ui�����ݣ�,qml��������ʹ�ã�qobject������ʹ��
��qmlģ����ͼ��jsԭ�ͷ����ȣ�qobject�����繲���ڴ棬���̣߳��׽��֣�qtcpsocket��qlocalsocket����xml,json,���ݿ����ʹ�ã�ģ����ͼ��ͼԪ��ͼ��
*/

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);//��Ч��ֹ��ק����
    QApplication app(argc, argv);
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));//�����������,����app֮ǰ
    qmlRegisterType<FuncC>("FuncC",1,0,"FuncC");
    //ģ�͵���
    qmlRegisterType<FriendGroupModel>("Model",1,0,"FriendGroupModel");
    qmlRegisterType<FriendModel>("Model",1,0,"FriendModel");
    qmlRegisterType<FriendData>("Model",1,0,"FriendData");
    //  ���ı��༭�����ർ��
    qmlRegisterType<ImageHelper>("ImageHelper",1,0,"ImageHelper");


    QQmlApplicationEngine engine;
    Images*images=new Images(&engine);//�˳�������
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
