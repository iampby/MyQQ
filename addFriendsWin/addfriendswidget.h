#ifndef ADDFRIENDSWIDGET_H
#define ADDFRIENDSWIDGET_H

#include"movedwidget.h"
#include"label.h"
#include<qframe.h>
#include<qpushbutton.h>
#include<qstackedwidget.h>
#include<qlabel.h>
#include<qlineedit.h>
#include<qcombobox.h>
#include <qjsondocument.h>
class QTextEdit;
class AddFriendsWidget:public MovedWidget
{
    Q_OBJECT
public:

    AddFriendsWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AddFriendsWidget();
    void closeEvent(QCloseEvent *event);
    bool operator ==(AddFriendsWidget*w);
    void  inint(QString &myNumber);
    void pixmapToRound(QPixmap*img)const;
private:
    QString getElideText(QString &name,const qint32& width,const QFontMetrics&f);
signals:
    void emitClosed();
    void emitVerifyInfo(QJsonDocument doc);//发送添加验证json
    void listMainProcess();//发送信号给窗口去监听主线程的 写入
private slots:
    void districtVerifyTextLength();
    void next();//下一个按钮点击
    void cancel();//取消按钮点击
    void tipLinked(const QString &link);//提示是好友链接触发
    void tipHover(const QString &link);
    void newGroup();
    void openPersonalData();//发送信号给主进程 打开一个资料界面
public:
    QByteArray pixData;
    QString myqq;
    QString name;
    QString sex;
    QString age;
    QString location;
    QString signature;
private:
    bool hasClicked;//指示是否打开过资料界面
    qint32 last;//verifyedit 上一个文本长度
    bool isLinkedHover;//判断是否悬浮在page2超文本上面
    QWidget* contentWidget;//内容
    QFrame*header;//标题
    QFrame*footer;//底部
    QStackedWidget*fillWidget;//填充块
    QWidget*page1;//页一
    QWidget*page2;
    QWidget*page3;//完成页
    QWidget*tagWidget;//信息片
    QPushButton*minBtn;//标题按钮 30 30
    QPushButton*closeBtn;//28 30
    QPushButton*bottomBtn1;//下面按钮1
    QPushButton*bottomBtn2;//
    Label*pixLabel;//头像
    QPushButton*nameBtn;//昵称
    QLabel*numberLabel;
    QLabel*sexAndAgeLabel;
    QLabel*locationLabel;//所在地
    //fillwidget 附属小部件
    QLabel*verifyLbael;
    QTextEdit*verifyEdit;
    QLabel*tipLabel;//提示是好友标签
    QLabel*pixTipLabel;//提示是好友图标
    QLineEdit*tagLEdit;//备注编辑框
    QComboBox*groupCBox;//分组复合框
    //新建分组框
   MovedWidget*childWidget;//可移动子控件
};

#endif // ADDFRIENDSWIDGET_H
