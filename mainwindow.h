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
    bool eventFilter(QObject *watched, QEvent *event);//�����¼�ͨ��mainwindow����
private:
    void initCityModel();//�ڳ�ʼ��4��λ��ģ��

signals:

private slots:
    void handleTownsmanButton();//���Ӽ�ʱ��timeout�źţ����ü�ʱ��������ʾ�ؼ�����Ϊ�߳�æµʱ��Щ�ؼ��������������¼�
    void handleFriendButton();//���Ӽ�ʱ��timeout�źţ����ü�ʱ��������ʾ�ؼ�����Ϊ�߳�æµʱ��Щ�ؼ��������������¼�
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
    void showAddFriend(const QModelIndex);//show����ӽ���
    void showAddFriend(const QJsonObject&obj);//show����ӽ���
    void openPersonalData(const QString mq);
    void addFriend(QJsonDocument doc);//���ͺ�����֤��Ϣ

    void readFromMainProcess();//��ȡ���������̵�����

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
    //�ı��� �����׼���
    QTextStream tout;
    //���̽����ⲿ����
    QString myqq;
    QString hometown;//����
    QString where;//���ڵ�
    QString sex;//�Ա�

    QList<AddFriendsWidget*>listWidget;//��Ӻ��ѵ����б�

    QString host;
    quint16 port;
    titleBar*titlebar;
    TcpSocket*socket;
    FindBtn*findPersonBtn;
    FindBtn*findGroupBtn;
    UserView*userView;//����İ�����б�
    bool isSeach;//�Ƿ��ǵ����ťִ�е����� �Ǿ���������б� ���������������Ӻ���
    QString mq;//isseach=trueʱʹ��������ָ������

    FindUserModel*userModel;
    bool hasSeachCBoxShow;
    bool isNeedDisabled1;//����Ҫui->citypop1��ĳ����Ͽ�Ĺ���ģ��������һ����Ͽ�Ĺ���ģ�͵�ʱ����һ��Ӧ�ñ���ֹ�����Ϊtrue
    bool isNeedDisabled2;//����Ҫui->citypop2��ĳ����Ͽ�Ĺ���ģ��������һ����Ͽ�Ĺ���ģ�͵�ʱ����һ��Ӧ�ñ���ֹ�����Ϊtrue
    qint8 visibleForCityPop;//0 ������ 1 citypop1���� 2 citypop2���� ���ڴ��ڹر�ʱ��������Ӧ״̬

    QLocalSocket*localSocket;//�����ܵ����ں�������ͨ��
};

#endif // MAINWINDOW_H
