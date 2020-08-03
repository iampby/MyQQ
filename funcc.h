#ifndef FUNCC_H
#define FUNCC_H


#include"weatherhandle.h"
#include"friendgroupmodel.h"
#include"images.h"
#include"headimgwidget.h"
#include"netmonitor.h"
#include"UpdateTimer.h"
#include"nativeserver.h"
#include <QObject>
#include<QQuickWindow>
#include<qapplication.h>
#include<qdesktopwidget.h>
#include<qtimer.h>
#include<QNetworkReply>
#include <QHostAddress>
#include<QTcpSocket>
#include<qvector.h>
#include<qdatetime.h>
#include<qsharedmemory.h>
#include<qpixmap.h>
class AddFriendGroupWidget;
class QXmlStreamReader;
class LoginSocket;
class QProcess;
class RegisterSocket;
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
    ~ FuncC();
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

    Q_INVOKABLE void  initImageSet(Images*arg);//赋值images
    Q_INVOKABLE void registerMyQQ(const QString&MyName,const QString&passwd);// 注册MyQQ,参数MyQQ、passwd
    Q_INVOKABLE bool saveStringToTxt(const QString &str,const QString& title,const QString&dir);

    Q_INVOKABLE void  login(const QString&myqq, const QString&passwd);
    void newServer();//建立一个本地与远程通信的服务器
    Q_INVOKABLE void realseServer();//释放server
    Q_INVOKABLE bool  writeFile(const QByteArray&content,const QString&filepath);//保存数据到文件
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//保存图片到文件
    //Q_INVOKABLE QString openFile(const QString&title,const QString&dir,const QString&filter)const;//打开文件对话框 这个函数不能在qml调用 指针问题难以解决固不用

    Q_INVOKABLE void startClock();// 开始计时（用于实时监测网络）
    Q_INVOKABLE void stopClock();// 停止计时（用于实时监测网络）

    Q_INVOKABLE void setMyCursor(const int &direct, QWindow*w)const;//设置鼠标类型 用于窗口拉伸
    Q_INVOKABLE void openTempMesWin()const;//打开一个升级提示框
    Q_INVOKABLE void addHeadWidget(QWindow *w, const int&x, const int&y, QPixmap pixmap, const QString&myqq, const bool isgot)const;//更改头像框打开时添加一个QWidget控件到qml控件w
    Q_INVOKABLE void openFile(QString filename);//打开更改头像界面
    Q_INVOKABLE void closeWidget();//发送信号删除更改头像视图的widget控件
    Q_INVOKABLE void getHistoryHeadImg(const QString&myqq) const;//获取历史头像 this常量调用
    Q_INVOKABLE void updateSignature(QString signature, const QString&in);//更新个性签名
    Q_INVOKABLE void updateCover(QString qmlFilePath);//更新 用户资料封面
    Q_INVOKABLE void deleteNetAndUpdateTimer();//删除网络监测器
    Q_INVOKABLE void addCoverWidget(QWindow *w, const int&x, const int&y,QString filePath)const;//更改封面界面打开时添加一个QWidget控件到qml控件w
    Q_INVOKABLE void closeCoverWidget();//发送信号删除更改封面界面的widget控件
    Q_INVOKABLE void getIndividualData();//获取远程个人资料
    Q_INVOKABLE void updatePhotoWall(quint8 length);//更新用户远程照片墙
    Q_INVOKABLE void inintCityData(QQuickWindow*w);//初始化城市数据 数据源sqlite
    Q_INVOKABLE void updateUserInformation(QVariantMap info);//更新远程的用户信息
    Q_INVOKABLE void addRemoteFriendGroup(QJsonDocument &doc);//远程添加分组
    Q_INVOKABLE void exitMyQQ(QQuickWindow *w=nullptr);//退出处理
    Q_INVOKABLE void getVerifyArray(const QString&myqq, QQuickWindow *qmlWin);//获取验证消息组
    Q_INVOKABLE void openAddFGroupWidget(QQuickWindow *w, QQuickWindow *qqMainWin);//打开widget 用于添加hao'you分组
    Q_INVOKABLE void handleFVerify(QVariantMap obj);//处理好友验证消息



    Q_INVOKABLE void startAddFriendsProcess(QQuickWindow*arg, QMap<QString, QVariant> obj, QList<QVariant> arr);//添加好友进程
    unsigned short addFriendsProcessCount()const;//返回当前进程启动的好友进程数
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
    template<class T>//获取简单数组长度的模板函数
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
    void updateFriendsModel(const QString&value,const qint32& role,const QString&number)const;//刷新好友模型数据
Q_SIGNALS:
    void emitCloseMyProcess();//随qqmainwin同死
    void emitHeadImgOpenFile(const QString& filename);//更改头像界面打开一个文件
    void emitCloseHead();//释放更改头像界面
    void emitCloseCover();//释放更改封面界面
    void emitCloseAddFGroup();//释放添加界面
    void emitHeadImgOKClicked(Images*images );//更改头像 ok 信号
    void emitCoverOKClicked(const QString&myqq);//更改头像 ok 信号
    void emitReadHistory(int code)const;//更改头像界面点击
    void emitSelectedImg( QPixmap pixmap);//添加qml选中头像到widget
    void emitPersonalJsonInfo(QVariantMap obj);//添加d大部分信息到qml
    void emitPersonalCoverAndPhoto(QVector<QString> walls,QString cover);//添加封面和照片到qml
    void emitFVeify(QVariantMap obj);//传好友验证到lqml
    void emitFriend(QVariantMap obj);//传好友到lqml
private slots:
    void analysisWh(QString totalGeoAddr);//解析本地IP获取地理位置及Url
    void handleProcessStarted();
    // void GetInternetConnectState();
    void registerFinished();
    void registerConnectFailed();
    void  updateHandle(const bool &ok);//定时获取数据后处理界面更新
    //server
    void getFVerify(QByteArray data);
    void getFriend(QByteArray data,QPixmap pix);//处理好友
private:
    QQuickWindow *m_win;//便于调用 不要删除它
    QString m_sourceIco;

    Images*images;//值等于一个qml注册对象,提供qpixmap与qml image的交互,便于调用 不要删除它

    QString ip;
    RegisterSocket*registerSock;
    quint16 registerPort;//注册账号

    NativeServer*server;
    LoginSocket*loginSock;
    quint16 loginPort;//一些界面数据获取及更新
    quint16 updatePort;//更新远程数据 如签名、备注、历史头像、资料封面
    //记载MyQQ信息
    //用户的 name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    QMap<QString, QVariant>userInfo;

    QString m_myQQ;
    QString m_passwd;
    QVariantMap setInfo;//记录设置信息，登录用
    // QMap<QString,QVector<QVector<QMap<QString,QString>>>>groupChatInfo;

    NetMonitor* timer;//网络监测定时 windows用 子线程运行
    UpdateTimer*updateTimer;//定时更新用户界面 子线程运行
    bool online;//网络状态记录
    QString m_localCity;
    QString m_localUrl;
    QString _3dayWeaAndTem[3][2];
    unsigned short processCount;//添加好友进程个数 非0即1
    QSharedMemory addSMm;//添加界面的共享内存，传好友组名数据过去

    AddFriendGroupWidget*addFGWidget;

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
