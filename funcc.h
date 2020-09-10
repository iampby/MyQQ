#ifndef FUNCC_H
#define FUNCC_H


#include"weatherhandle.h"
#include"friendgroupmodel.h"
#include"images.h"
#include"headimgwidget.h"
#include"netmonitor.h"
#include"UpdateTimer.h"
#include"nativeserver.h"
#include"sendsocket.h"
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


    Q_INVOKABLE int taskDirection()const;//����0 1 2 3;�ֱ����������λ��λ���¡��ϡ��ҡ���
    Q_INVOKABLE int taskBarWidth()const;// ��ȡ���������

    void setPort(const quint16&registerPort);//�����׽���ʱ����
    void setIp(const QString&address);



    //ע�⣬��Qml�������͵����ã���ָ���⣬�����ı���Ϊ���������ã�����Ϊ�������ã��²���ת��ͨ������ʵ�֣������ܶ�ʵ��ָ��ĵ�ַ���������޸ģ����Գ������á�ָ��ͳ�������ͨ��

    Q_INVOKABLE void  initImageSet(Images*arg);//��ֵimages
    Q_INVOKABLE void registerMyQQ(const QString&MyName,const QString&passwd);// ע��MyQQ,����MyQQ��passwd
    Q_INVOKABLE bool saveStringToTxt(const QString &str,const QString& title,const QString&dir);

    Q_INVOKABLE void  login(const QString&myqq, const QString&passwd);//��¼���
    void newServer();//����һ��������Զ��ͨ�ŵķ�����
    Q_INVOKABLE void realseServer();//�ͷ�server
    Q_INVOKABLE bool  writeFile(const QByteArray&content,const QString&filepath);//�������ݵ��ļ�
    bool  writeImg(const QByteArray&content,const QString&filepath, const char *format = nullptr);//����ͼƬ���ļ�
    //Q_INVOKABLE QString openFile(const QString&title,const QString&dir,const QString&filter)const;//���ļ��Ի��� ���ڵ��þ�̬��Ա���ļ��Ի���ʽ������qml���ã���ʹ��qml���ضԻ���

    Q_INVOKABLE void startClock();// ��ʼ��ʱ������ʵʱ������磩
    Q_INVOKABLE void stopClock();// ֹͣ��ʱ������ʵʱ������磩

    Q_INVOKABLE void update();// ˢ�� ��ϵ��

    Q_INVOKABLE void setMyCursor(const int &direct, QWindow*w)const;//����������� ���ڴ�������
    Q_INVOKABLE void openTempMesWin()const;//��һ��������ʾ��
    Q_INVOKABLE void addHeadWidget(QWindow *w, const int&x, const int&y, QPixmap pixmap, const QString&myqq, const bool isgot);//����ͷ����ʱ���һ��QWidget�ؼ���qml�ؼ�w
    Q_INVOKABLE void openFile(QString filename);//�򿪸���ͷ�����
    Q_INVOKABLE void closeWidget();//�����ź�ɾ������ͷ����ͼ��widget�ؼ�
    Q_INVOKABLE void getHistoryHeadImg(const QString&myqq) const;//��ȡ��ʷͷ�� this��������
    Q_INVOKABLE void updateSignature(QString signature, const QString&in);//���¸���ǩ��
    Q_INVOKABLE void updateCover(QString qmlFilePath);//���� �û����Ϸ���
    Q_INVOKABLE void deleteNetAndUpdateTimer();//ɾ����������
    Q_INVOKABLE void addCoverWidget(QWindow *w, const int&x, const int&y,QString filePath)const;//���ķ�������ʱ���һ��QWidget�ؼ���qml�ؼ�w
    Q_INVOKABLE void closeCoverWidget();//�����ź�ɾ�����ķ�������widget�ؼ�
    Q_INVOKABLE void getIndividualData(QString content, QString number, QQuickWindow *qmlWin);//��ȡԶ�̸������� arg1:ָ������ arg2:���� arg3:��Ӧ��qml���ϴ���
    Q_INVOKABLE void updatePhotoWall(quint8 length);//�����û�Զ����Ƭǽ
    Q_INVOKABLE void removePhotoFromRemoteWall(int pos);//ɾ��Զ�� ��Ƭǽ��һ����Ƭ
    Q_INVOKABLE void inintCityData(QQuickWindow*w);//��ʼ���������� ����Դsqlite
    Q_INVOKABLE void updateUserInformation(QVariantMap info);//����Զ�̵��û���Ϣ
    Q_INVOKABLE void exitMyQQ(QQuickWindow *w=nullptr);//�˳�����
    Q_INVOKABLE void getVerifyArray(const QString&myqq, QQuickWindow *qmlWin);//��ȡ��֤��Ϣ��
    Q_INVOKABLE void openAddFGroupWidget(QQuickWindow *w, QQuickWindow *qqMainWin);//��widget �������h���ѷ���
    Q_INVOKABLE void handleFVerify(QVariantMap obj);//���������֤��Ϣ
    Q_INVOKABLE void addRemoteFriendGroup(QJsonDocument &doc);//Զ����ӷ���
    Q_INVOKABLE void addRemoteFriendGroup(QVariantMap obj);//Զ����ӷ���
    Q_INVOKABLE void updateFGroup(QVariantMap obj);//����Զ�̺��ѷ��� ˳��任 ɾ�� rename
    Q_INVOKABLE void moveFriend(const int& before, const int&after, const QString &number);//Զ�̺��ѱ���� ����number��before  �ƶ��� after
    Q_INVOKABLE void deleteFriend(const int&index,const QString &number);//Զ�̺���ɾ��
    Q_INVOKABLE void alterFTag(QVariantMap map);//�޸ĺ��ѱ�ע
    Q_INVOKABLE void dmrFriend(QJsonObject&obj);//����ɾ���ƶ��޸ı�ע

    Q_INVOKABLE void screenShot();//����
    Q_INVOKABLE void removeDir(QString path);//ɾ��ĳ��Ŀ¼
    Q_INVOKABLE void getFIP(QString number);//����Ϊ���Ѻ���+1 ��ȡ ip port
    Q_INVOKABLE void sendFMessage(QString ip, QString port, QString number, QString html, QQuickWindow*win);//����html ������Ϣ ����Ϊ�Է���Ϣ
    QByteArray parseFHtml(QString&html, QString&ptext);//�ѷ��͸����ѵ�html���� ����һ��xml��ʽ����Ϣ���� �ͻ��һ�����ı�����Ϣ�б���
    Q_INVOKABLE void loadFChatLog(QQuickWindow*qmlWin,QString number);//���������¼ ����1��Ϊ���ú������� ����2 ��Ϊ���Ҽ�¼�ļ� Ϊ���Ѻ���
    Q_INVOKABLE void saveSentFLog(QString number,QString bytes);//���淢�͵ĺ��������¼
    Q_INVOKABLE QString saveSentFLog(QString bytes);//���淢�͵ĺ��������¼ ����������Լ� ������Ϣ�б���ʾ��

    Q_INVOKABLE void startAddFriendsProcess(QQuickWindow*arg, QMap<QString, QVariant> obj, QList<QVariant> arr);//��Ӻ��ѽ���
    unsigned short addFriendsProcessCount()const;//���ص�ǰ���������ĺ��ѽ�����
    Q_INVOKABLE void updateAuxiliaryProcessFGoup(QList<QVariant> arr);//�����鷢�ͱ仯 ���¸������̵�������

    Q_INVOKABLE void crawWeatherUrl(const QString&url);//��ȡ�������ݲ����д���
    Q_INVOKABLE QString _3daysdata(const int& r,const int& c);//����day3Weatherָ��������������
    Q_INVOKABLE void initWh();
    Q_INVOKABLE void mkDir(const QString&dirString);//����Ŀ¼
    //��ʼ��qml��¼����
    Q_INVOKABLE void initLoginInfo();//��¼�ǳ�ʼ�������Ϣ
    Q_INVOKABLE void readWeatherFile(const QString&fileName);
    Q_INVOKABLE void writeWeatherFile(const QString&fileName);
    Q_INVOKABLE void connectGetFile(const QString&instructDescription);//(2 headImg myqq)��ȡ���к���ͷ��δ������Ϣ

    Q_INVOKABLE QString getCityData(const int &r,const int &c)const;//��ȡcityNameAboutWeather[3][2]��Ԫ��
    Q_INVOKABLE void setCityData(const QString& v, const int &r,const int &c);//����cityNameAboutWeather[3][2]��Ԫ��
    Q_INVOKABLE QString getWeatherUrl(const QString&query)const;//��ȡ���ж�Ӧ����ַ
    Q_INVOKABLE void findCityEvent(const QString&text);
    Q_INVOKABLE QString indexForCityList(int r,int c)const;
    Q_INVOKABLE void clearForCityList();
    Q_INVOKABLE int getCityCount()const;
    template<class T>//��ȡ�����鳤�ȵ�ģ�庯��
    int getArrayLenth(T&array){
        return sizeof (array)/sizeof (array[0]);
    }
public Q_SLOTS://ʹ�õ�����Դ�����ʱ�൱���� ����������qml�����źűȽϺ�
    void setSourceIco(const QString &arg);
    void setSourceIco(QQuickWindow*win, QString key);//�������촰�ڵ�ͼ��
    void setWin(QQuickWindow *arg);
    void setLocalCity(const QString&arg);
    void setLocalUrl(const QString&arg);
    void setMyQQ(const QString&);
    void setPasswd(const QString&);
Q_SIGNALS://ʹ�õ�����Դ�����ʱ�൱���� ����������qml�����źűȽϺ�
    void winChanged();
    void sourceIcoChanged();
    void myQQChanged();
    void passwdChanged();

    void registerResult(const bool&result,const QString&myQQ);
    void loginResult(int result);//0�ɹ� 1�˺Ų����� 2������� 3û����

    void getFriendGroup(QString name,QString set,int pos);
    void getFileFinished(int code);//��ȡͷ����ļ�����ź�
    void getReceiveFMessage(QVariantMap iobj,QVariantMap vobj);//��ת�����б�δ������Ϣ����֤��Ϣ��qml

    void localCityChanged();
    void localUrlChanged();
    void finished();//��ɽ������ص�ַ����ȡurl
    void netChanged(const bool& flags);//�Ͽ�����������
    void crawWeatherUrlFinished();
    void updateFriendsModel(const QString&value,const qint32& role,const QString&number)const;//ˢ�º���ģ������
    void updateMyGrade(int grade,QString ads)const;//�����ҵĵȼ�����Ծ��
    void updateTotalFModel();//ˢ����������ģ���ź�
    void getScreenPximap(QPixmap pix);//��ͼƬ���������
    void getAddress(QString ip=QString(),QString port=QString(),QString status=QString("0"));//����ַ���������
    void friendMessage(QString number,QString html,QString time);// һ��������Ϣ���߷��� ����Ϊ��Ϣ�б�(ʱ��;;��Ϣ�б���ʾ) ���� ��html��Ϣ����
  void writeStandartInputToProcess(QString number);//д���׼������֪ͨ�������̵����� ��� ���ѽ���
Q_SIGNALS:
    void emitHandleVerifyAndInfoList();//�����ȡ��δ���� ��Ϣ �б����֤��Ϣ
    void emitCloseMyProcess();//��qqmainwinͬ��
    void emitHeadImgOpenFile(const QString& filename);//����ͷ������һ���ļ�
    void emitCloseHead();//�ͷŸ���ͷ�����
    void emitCloseCover();//�ͷŸ��ķ������
    void emitCloseAddFGroup();//�ͷ���ӽ���
    void emitHeadImgOKClicked(Images*images );//����ͷ�� ok �ź�
    void emitCoverOKClicked(const QString&myqq);//����ͷ�� ok �ź�
    void emitReadHistory(int code)const;//����ͷ�������
    void emitSelectedImg( QPixmap pixmap);//���qmlѡ��ͷ��widget
    void emitFVeify(QVariantMap obj);//��������֤��lqml
    void emitFriend(QVariantMap obj);//�����ѵ�lqml
    void emitOffline(QVariantMap obj);//�����źŸ� qqmainwin �������߿򵯳�
    void emitDeletionExternalItemOf(QString number);//����Ƿ��ڸ�����������number���������򿪵���Ӻ����� �������
signals:
    void tempMidSig();
private slots:
    void analysisWh(QString totalGeoAddr);//��������IP��ȡ����λ�ü�Url

    void handleProcessStarted();
    // void GetInternetConnectState();
    void registerFinished();
    void registerConnectFailed();
    void  updateHandle(const bool &ok);//��ʱ��ȡ���ݺ���������
    //server
    void getFVerify(QByteArray data);
    void getFriend(QByteArray data,QPixmap pix);//�������
    void offline(QString ip, QString host, QString datetime);//���ߴ���
    void GetFMessage(QString html, QString number, QString time);//���ѵ�html������Ϣ <(.*)</p>
private:
    QQuickWindow *m_win;//���ڵ��� ��Ҫɾ����
    QString m_sourceIco;

    Images*images;//ֵ����һ��qmlע�����,�ṩqpixmap��qml image�Ľ���,���ڵ��� ��Ҫɾ����

    QString ip;
    RegisterSocket*registerSock;
    quint16 registerPort;//ע���˺�

    NativeServer*server;
    LoginSocket*loginSock;
    quint16 loginPort;//һЩ�������ݻ�ȡ������
    quint16 updatePort;//����Զ������ ��ǩ������ע����ʷͷ�����Ϸ���
    //����MyQQ��Ϣ
    //�û��� name���ǳƣ� sex���Ա� signature������ǩ���� days����Ծ������ grade���ȼ�) status(״̬�� ���ڵ� ����
    QMap<QString, QVariant>userInfo;

    QString m_myQQ;
    QString m_passwd;
    QVariantMap setInfo;//��¼������Ϣ����¼��
    // QMap<QString,QVector<QVector<QMap<QString,QString>>>>groupChatInfo;

    NetMonitor* timer;//�����ⶨʱ windows�� ���߳�����
    UpdateTimer*updateTimer;//��ʱ�����û����� ���߳�����
    bool online;//����״̬��¼
    QString m_localCity;
    QString m_localUrl;
    QString _3dayWeaAndTem[3][2];
    unsigned short processCount;//��Ӻ��ѽ��̸��� ��0��1
    QSharedMemory addSMm;//��ӽ���Ĺ����ڴ棬�������������ݹ�ȥ

    AddFriendGroupWidget*addFGWidget;

    WeatherHandle*wh;//��ȡ��������
    QString cityNameAboutWeather[3][2];
    QString cityList[50][2];
    int cityCount;//������
    QMap<QString,QString>allCitys;//�����б�


    void initSetInfo(QXmlStreamReader&);//��ʼ��������Ϣ û��ʵ�ֹ���
    void initFriendInfo(QXmlStreamReader&);//��ʼ��������Ϣ
    void initGroupChatInfo(QXmlStreamReader&);//��ʼ������Ϣ û��ʵ�ֹ���
    void parseFriendInfo(QXmlStreamReader &reader, QString&, int &pos);//����ȡ������Ϣ
    void initAllCitys();//�������г�����Ϣ��������ѯ����
};


#endif // FUNCC_H
