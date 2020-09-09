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
    void emitVerifyInfo(QJsonDocument doc);//���������֤json
    void listMainProcess();//�����źŸ�����ȥ�������̵߳� д��
private slots:
    void districtVerifyTextLength();
    void next();//��һ����ť���
    void cancel();//ȡ����ť���
    void tipLinked(const QString &link);//��ʾ�Ǻ������Ӵ���
    void tipHover(const QString &link);
    void newGroup();
    void openPersonalData();//�����źŸ������� ��һ�����Ͻ���
public:
    QByteArray pixData;
    QString myqq;
    QString name;
    QString sex;
    QString age;
    QString location;
    QString signature;
private:
    bool hasClicked;//ָʾ�Ƿ�򿪹����Ͻ���
    qint32 last;//verifyedit ��һ���ı�����
    bool isLinkedHover;//�ж��Ƿ�������page2���ı�����
    QWidget* contentWidget;//����
    QFrame*header;//����
    QFrame*footer;//�ײ�
    QStackedWidget*fillWidget;//����
    QWidget*page1;//ҳһ
    QWidget*page2;
    QWidget*page3;//���ҳ
    QWidget*tagWidget;//��ϢƬ
    QPushButton*minBtn;//���ⰴť 30 30
    QPushButton*closeBtn;//28 30
    QPushButton*bottomBtn1;//���水ť1
    QPushButton*bottomBtn2;//
    Label*pixLabel;//ͷ��
    QPushButton*nameBtn;//�ǳ�
    QLabel*numberLabel;
    QLabel*sexAndAgeLabel;
    QLabel*locationLabel;//���ڵ�
    //fillwidget ����С����
    QLabel*verifyLbael;
    QTextEdit*verifyEdit;
    QLabel*tipLabel;//��ʾ�Ǻ��ѱ�ǩ
    QLabel*pixTipLabel;//��ʾ�Ǻ���ͼ��
    QLineEdit*tagLEdit;//��ע�༭��
    QComboBox*groupCBox;//���鸴�Ͽ�
    //�½������
   MovedWidget*childWidget;//���ƶ��ӿؼ�
};

#endif // ADDFRIENDSWIDGET_H
