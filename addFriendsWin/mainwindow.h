#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStringListModel>
#include"titlebar.h"
#include"findbtn.h"
#include"tcpsocket.h"
#include"addfriendswidget.h"
#include<qvariant.h>
#include<qthread.h>
#include<QModelIndex>
#include<qlocalsocket.h>
class UserView;
class FindUserModel;
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMyQQInfo(const QJsonDocument&json);
    bool eventFilter(QObject *watched, QEvent *event);//所有事件通过mainwindow处理
private:
    void initCityModel();//在初始化4个位置模型

signals:

private slots:
    void handleTownsmanButton();//连接计时器timeout信号，利用计时器加速显示控件，因为线程忙碌时有些控件不会立即处理事件
    void handleFriendButton();//连接计时器timeout信号，利用计时器加速显示控件，因为线程忙碌时有些控件不会立即处理事件
private slots:
    void closeBtnClicked();
    void minBtnClicked();
    void sexPopup();
    void sexHidePopup();
    void agePopup();
    void ageHidePopup();
    void showPersonPage();
    void showGroupPage();
    void hometownCBoxPopup();
    void hometownCBoxHidePopup();
    void whereCBoxPopup();
    void whereCBoxHidePopup();
    void connectFailed(QAbstractSocket::SocketError socketError);
    void deleteToSock();
    void getAddFriendsList();
    void continueAddFriendsList();
    void showAddFriend(const QModelIndex);//show出添加界面
    void showAddFriend(const QJsonObject&obj);//show出添加界面
    void openPersonalData(const QString mq);
    void addFriend(QJsonDocument doc);//发送好友验证信息

    void readFromMainProcess();//读取来自主进程的数据

    void on_whereSub1_activated(const QString &arg1);

    void on_whereSub2_activated(const QString &arg1);

    void on_whereSub3_activated(const QString &arg1);

    void on_whereSub4_activated(const QString &arg1);

    void on_hometownSub1_activated(const QString &arg1);

    void on_hometownSub2_activated(const QString &arg1);

    void on_hometownSub3_activated(const QString &arg1);

    void on_hometownSub4_activated(const QString &arg1);

    void on_friendBtn_clicked();

    void on_townsmanBtn_clicked();

    void on_findBtn_clicked();

    void returnLabelClicked();

    void on_ageCBox_currentIndexChanged(int index);

    void on_sexCBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    //文本流 定向标准输出
    QTextStream tout;
    //进程接受外部参数
    QString myqq;
    QString hometown;//故乡
    QString where;//所在地
    QString sex;//性别

    QList<AddFriendsWidget*>listWidget;//添加好友弹窗列表

    QString host;
    quint16 port;
    titleBar*titlebar;
    TcpSocket*socket;
    FindBtn*findPersonBtn;
    FindBtn*findGroupBtn;
    UserView*userView;//搜索陌生人列表
    bool isSeach;//是否是点击按钮执行的搜索 是就正常添加列表 不是则仅仅用来添加好友
    QString mq;//isseach=true时使用于搜索指定号码

    FindUserModel*userModel;
    bool hasSeachCBoxShow;
    bool isNeedDisabled1;//当需要ui->citypop1的某个组合框的过滤模型来自另一个组合框的过滤模型的时候，另一个应该被禁止，这个为true
    bool isNeedDisabled2;//当需要ui->citypop2的某个组合框的过滤模型来自另一个组合框的过滤模型的时候，另一个应该被禁止，这个为true
    qint8 visibleForCityPop;//0 不可视 1 citypop1可视 2 citypop2可视 用于窗口关闭时保存其相应状态

    QLocalSocket*localSocket;//命名管道用于和主进程通信
};

#endif // MAINWINDOW_H
