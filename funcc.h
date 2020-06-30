#ifndef FUNCC_H
#define FUNCC_H
// Flags for InternetGetConnectedState
#define INTERNET_CONNECTION_MODEM           1
#define INTERNET_CONNECTION_LAN             2
#define INTERNET_CONNECTION_PROXY           4
#define INTERNET_CONNECTION_MODEM_BUSY      8

#include"weatherhandle.h"
#include"friendgroupmodel.h"
#include <QObject>
#include<QQuickWindow>
#include<qapplication.h>
#include<qdesktopwidget.h>
#include<qtimer.h>
#include<QNetworkReply>
#include<QNetworkConfigurationManager>
#include <QHostAddress>
#include<QTcpSocket>
#include<qvector.h>
class QXmlStreamReader;
class LoginSocket;
class QProcess;
class RegisterSocket;
//class BigFileSocket;
#include"headimgwidget.h"
class FuncC:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickWindow* win READ win WRITE setWin NOTIFY winChanged)
    Q_PROPERTY(QString sourceIco READ sourceIco WRITE setSourceIco NOTIFY sourceIcoChanged)
    Q_PROPERTY(QString localCity READ localCity WRITE setLocalCity NOTIFY localCityChanged)
    Q_PROPERTY(QString localUrl READ localUrl WRITE setLocalUrl NOTIFY localUrlChanged)
    Q_PROPERTY(QString myQQ READ myQQ WRITE setMyQQ NOTIFY myQQChanged)
    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY passwdChanged)

public:
    FuncC(QObject *parent = nullptr);
    QQuickWindow *win()const;
    QString sourceIco() const;
    QString localCity() const;
    QString localUrl() const;
    QString myQQ()const;
    QString passwd()const;


    Q_INVOKABLE int taskDirection()const;//返回0 1 2 3;分别代表任务栏位置位于下、上、右、左
    Q_INVOKABLE int taskBarWidth()const;// 获取任务栏宽度

    void setPort(const quint16&registerPort);//连接套接字时调用
    void setIp(const QString&address);



    //注意，对Qml数据类型的引用，除指针外，其他的必须为常量级引用，不能为变量引用，猜测是转换通过复制实现，即不能对实际指向的地址数据做出修改，所以常量引用、指针和常数才能通过


    Q_INVOKABLE void registerMyQQ(const QString&MyName,const QString&passwd);// 注册MyQQ,参数MyQQ、passwd
    Q_INVOKABLE bool saveStringToTxt(const QString &str,const QString& title,const QString&dir);

    Q_INVOKABLE void  login(const QString&myqq,const QString&passwd);
    Q_INVOKABLE bool  writeFile(const QByteArray&content,const QString&filepath);//保存数据到文件
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//保存图片到文件
    //Q_INVOKABLE QString openFile(const QString&title,const QString&dir,const QString&filter)const;//打开文件对话框

    Q_INVOKABLE void startClock();// 开始计时（用于实时监测网络）
    Q_INVOKABLE void stopClock();// 停止计时（用于实时监测网络）

    Q_INVOKABLE void setMyCursor(const int &direct, QWindow*w)const;//设置鼠标类型 用于窗口拉伸
    Q_INVOKABLE void openTempMesWin()const;//打开一个升级提示框
    Q_INVOKABLE void addHeadWidget(QWindow *w, const int&x, const int&y, QPixmap pixmap)const;//更改头像框打开时添加一个QWidget控件到qml控件w
    Q_INVOKABLE void openFile(QString filename);//打开更改头像界面
    Q_INVOKABLE void closeWidget();//发送信号删除widget控件
    Q_INVOKABLE void okClicked();//发送更改头像 ok 信号

    Q_INVOKABLE void startAddFriendsProcess(QQuickWindow*arg, QMap<QString, QVariant> obj);
    unsigned short addFriendsProcessCount()const;
    Q_INVOKABLE void crawWeatherUrl(const QString&url);//获取天气数据并进行处理
    Q_INVOKABLE QString _3daysdata(const int& r,const int& c);//返回day3Weather指定索引处的数据
    Q_INVOKABLE void initWh();
    Q_INVOKABLE void mkDir(const QString&dirString);//制造目录
    //初始化qml登录数据
    Q_INVOKABLE void initLoginInfo();//登录是初始化相关信息
    Q_INVOKABLE void readWeatherFile(const QString&fileName);
    Q_INVOKABLE void writeWeatherFile(const QString&fileName);
    Q_INVOKABLE void connectGetFile(const QString&instructDescription);//(2 headImg myqq)获取所有好友头像

    Q_INVOKABLE QString getCityData(const int &r,const int &c)const;//读取cityNameAboutWeather[3][2]的元素
    Q_INVOKABLE void setCityData(const QString& v, const int &r,const int &c);//设置cityNameAboutWeather[3][2]的元素
    Q_INVOKABLE QString getWeatherUrl(const QString&query)const;//获取城市对应的网址
    Q_INVOKABLE void findCityEvent(const QString&text);
    Q_INVOKABLE QString indexForCityList(int r,int c)const;
    Q_INVOKABLE void clearForCityList();
    Q_INVOKABLE int getCityCount()const;
    Q_INVOKABLE template<class T>//获取简单数组长度的模板函数
    int getArrayLenth(T&array){
        return sizeof (array)/sizeof (array[0]);
    }
public Q_SLOTS://使用第三方源码解析时相当有用 这里用来给qml传递信号比较好
    void setSourceIco(const QString &arg);
    void setWin(QQuickWindow *arg);
    void setLocalCity(const QString&arg);
    void setLocalUrl(const QString&arg);
    void setMyQQ(const QString&);
    void setPasswd(const QString&);
Q_SIGNALS://使用第三方源码解析时相当有用 这里用来给qml传递信号比较好
    void winChanged();
    void sourceIcoChanged();
    void myQQChanged();
    void passwdChanged();

    void registerResult(const bool&result,const QString&myQQ);
    void loginResult(int result);//0成功 1账号不存在 2密码错误 3没连上

    void getFriendGroup(QString name,QString set,int pos);
    void getFileFinished(int code);//获取头像等文件完成信号

    void localCityChanged();
    void localUrlChanged();
    void finished();//完成解析当地地址并获取url
    void netChanged(const bool& flags);//断开或连接网络
    void crawWeatherUrlFinished();
signals:
    void emitOpenFile(const QString& filename);//更改头像界面打开一个文件
    void emitCloseHead();//释放更改头像界面
    void emitOKClicked();//更改头像界面点击
private slots:
    void analysisWh(QString totalGeoAddr);//解析本地IP获取地理位置及Url
    void handleProcessStarted();
    void GetInternetConnectState();
    void registerFinished();
    void registerConnectFailed();
private:
    QQuickWindow *m_win;

    QString m_sourceIco;

    QString ip;
    RegisterSocket*registerSock;
    quint16 registerPort;

    LoginSocket*loginSock;
    quint16 loginPort;

    //记载MyQQ信息
    //用户的 name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    QMap<QString, QVariant>userInfo;
    QString m_myQQ;
    QString m_passwd;
    QVariantMap setInfo;//记录设置信息，登录用



    QMap<QString,QVector<QVector<QMap<QString,QString>>>>groupChatInfo;



    QTimer* timer;
    bool online;
    QString m_localCity;
    QString m_localUrl;
    QString _3dayWeaAndTem[3][2];
    unsigned short processCount;//添加好友进程个数 非0即1
    QNetworkConfigurationManager*mgr;
    WeatherHandle*wh;
    QString cityNameAboutWeather[3][2];
    QString cityList[50][2];
    int cityCount;
    QMap<QString,QString>allCitys;


    void initSetInfo(QXmlStreamReader&);
    void initFriendInfo(QXmlStreamReader&);
    void initGroupChatInfo(QXmlStreamReader&);
    void parseFriendInfo(QXmlStreamReader &reader, QString&, int &pos);
    void initAllCitys();//保存所有城市信息以用来查询天气
};


#endif // FUNCC_H
