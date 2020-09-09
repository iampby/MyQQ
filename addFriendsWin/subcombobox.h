#ifndef SUBCOMBOBOX_H
#define SUBCOMBOBOX_H
//自定义城市复合框
#include <QObject>
#include<qcombobox.h>
#include<qlistview.h>
#include"placemodel.h"

class SubComboBox : public QComboBox
{
    Q_OBJECT
public:
    enum SheetRole{
        EabledRole,
        DisabledRole
    };
    SubComboBox(QWidget *parent = nullptr);
    ~SubComboBox();
    void hidePopup();
    void showPopup();
    void setModel(PlaceModel *arg);
    void setView(QListView *arg);
    QVariant data(const int& row,const int &role);
    void filterValue(const QVariant& value, const PlaceModel::DataRole &role);
    void setStyleSheet(const SheetRole &role);
    void setStyleSheet(const QString&sheet);
    void appendItem(const qint64&, const QString&,const qint64&fid=-1);
    int count()const;
    PlaceModel*getFilterModel() const;
    void setFilterModel(PlaceModel*arg);
private:
    PlaceModel* model;
    PlaceModel*filterModel;
    QListView* view;

};

#endif // SUBCOMBOBOX_H
