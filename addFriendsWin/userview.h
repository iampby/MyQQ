#ifndef USERVIEW_H
#define USERVIEW_H
//自定义查找好友显示视图 非标准视图类
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
    qint32 adjustFrameSize(const qint32&,const bool&b=false);//当布局内容改变时调整控件并返回旧的最大滚动值,row代表子控件行数，false表示不显示底部按钮控件，true反之
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
void removeItem(const int&r,const int&c);//移除一个项
signals:
    void imgClicked(const QString);
    void nameClicked(const QString);
    void addButtonClicked(const QModelIndex);
    void continueGetList();
private:
    QList<UserWidget*>list;
    //  QGridLayout*glay;//这里尝试用2个布局时会发生重影，不知道什么回事就不用了，可能是限定控件大小的关系和non-spontaneous的原因  用一个布局加额外的控件时发生non-spontaneous
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
    qint32 canContinue;//0等待 1继续 -1待定 插入后继续
    qint32 page;
    qint32 verticalSpacing;
    qint32 horizotalSpacing;
};

#endif // USERVIEW_H
