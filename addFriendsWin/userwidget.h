#ifndef USERWIDGET_H
#define USERWIDGET_H
//�Զ�����ͼ��ʾ�ؼ�
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
void setPixmap(const QPixmap &img);//����ͷ��
void setPrimitivePixmap(QPixmap *img);//ԭ����ͷ����
void setName(const QString&name);
void setAgeAndCountry(const QString &ageAndCountry);//�������һ��
void setAgeAndCountry(QPixmap &img);//�������һ��ͼƬ
const  QModelIndex index()const;
void setIndex(const QModelIndex &);
QPixmap *pixmapToRound(QPixmap *img)const;//ͼƬתԲ�� ����Բ������
QPixmap& drawRoundOnPixmap(QPixmap&img)const;//ͼƬ�ϻ���ɫԲ
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
