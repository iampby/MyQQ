#ifndef USERWIDGET_H
#define USERWIDGET_H

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
void setPixmap(const QPixmap &img);
void setPrimitivePixmap(QPixmap *img);
void setName(const QString&name);
void setAgeAndCountry(const QString &ageAndCountry);
void setAgeAndCountry(QPixmap &img);
const  QModelIndex index()const;
void setIndex(const QModelIndex &);
QPixmap *pixmapToRound(QPixmap *img)const;
QPixmap& drawRoundOnPixmap(QPixmap&img)const;
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
