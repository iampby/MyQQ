#ifndef USERVIEW_H
#define USERVIEW_H

#include <QObject>
#include"findusermodel.h"
#include"userwidget.h"
#include<qscrollarea.h>
#include<qstackedwidget.h>
class QGridLayout;
class QVBoxLayout;
class UserView:public QScrollArea
{
    Q_OBJECT
public:
    UserView(QWidget *parent = nullptr);
    ~UserView();
    void setModel(FindUserModel*m);
    void setCanContinue(const bool &);
    qint32 adjustFrameSize(const qint32&,const bool&b=false);//���������ݸı�ʱ�����ؼ������ؾɵ�������ֵ,row�����ӿؼ�������false��ʾ����ʾ�ײ���ť�ؼ���true��֮
    void setVerticalSpacing(const qint32&);
    void setHorizontalSpacing(const qint32&);
private:
    void scrollContentsBy(int dx, int dy);
    void wheelEvent(QWheelEvent *);
    void clearLayoutElements();
public slots:
    void reset();
    void inserted(const int&row, const int&column, const int &count);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles =QVector<int>());
    void lastButtonClicked();
    void nextButtonClicked();
    void oneButtonClicked();
    void twoButtonClicked();

signals:
    void imgClicked(const QString&);
    void nameClicked(const QString&);
    void addButtonClicked(const QModelIndex&);
    void continueGetList();
private:
    QList<UserWidget*>list;
    //  QGridLayout*glay;//���ﳢ����2������ʱ�ᷢ����Ӱ����֪��ʲô���¾Ͳ����ˣ��������޶��ؼ���С�Ĺ�ϵ��non-spontaneous��ԭ��  ��һ�����ּӶ���Ŀؼ�ʱ����non-spontaneous
    // QVBoxLayout*vlay;

    QFrame*frame;
    QFrame *buttonFrame;
    FindUserModel*model;
    QPushButton*lastBtn;
    QPushButton*nextBtn;
    QPushButton*oneBtn;
    QPushButton*twoBtn;
    static QString m_pageSheet;
    static QString m_pageNumberSheet;
    qint32 sliderMaxValue;
    qint32 canContinue;//0�ȴ� 1���� -1���� ��������
    qint32 page;
    qint32 verticalSpacing;
    qint32 horizotalSpacing;
};

#endif // USERVIEW_H
