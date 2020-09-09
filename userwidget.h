#ifndef USERWIDGET_H
#define USERWIDGET_H
//自定义视图显示控件
#include <QObject>
#include<qwidget.h>
#include<QModelIndex>
class QPushButton;
class QLabel;
class Label;
class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = nullptr);
    ~UserWidget();
void setPixmap(const QPixmap &img);//设置头像
void setPrimitivePixmap(QPixmap *img);//原生的头像处理
void setName(const QString&name);
void setAgeAndCountry(const QString &ageAndCountry);//设置年纪一栏
void setAgeAndCountry(QPixmap &img);//设置年纪一栏图片
const  QModelIndex index()const;
void setIndex(const QModelIndex &);
QPixmap *pixmapToRound(QPixmap *img)const;//图片转圆形 即加圆形遮罩
QPixmap& drawRoundOnPixmap(QPixmap&img)const;//图片上画蓝色圆
bool eventFilter(QObject *watched, QEvent *event);
signals:
    void imgClicked();
    void nameClicked();
    void addButtonClicked();
private:
    QPixmap*primitivePixmap;
    Label*pixLabel;
    QLabel*ageAndCountryLabel;
    QPushButton*nameBtn;
    QPushButton*addBtn;
    QModelIndex m_index;
};

#endif // USERWIDGET_H
