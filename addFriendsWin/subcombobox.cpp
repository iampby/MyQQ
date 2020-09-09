#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "subcombobox.h"
#include<QDebug>

static QString comboBoxSheet0;
static QString comboBoxSheet1;
SubComboBox::SubComboBox(QWidget *parent):QComboBox(parent)
{
    view=new QListView(this);
    model=new PlaceModel(this);
    filterModel=nullptr;
    setMaxVisibleItems(10);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->setResizeMode(QListView::Adjust);
    view->setStyleSheet("QListView {\
                        background:rgb(243,249,252);\
            border:1px solid rgb(0, 155, 219);\
width:150;\
height:200;\
}\
QListView {\
    show-decoration-selected: 1; /* make the selection span the entire width of the view */\
}\
QListView::item:hover {\
                    background:rgb(65,153,215);\
color:black;\
} \
QListView::!active {\
  background:rgb(243,249,252);\
 }\
QListView::item:!hover {\
                     background:rgb(243,249,252);\
color:black;\
}\
QScrollBar:vertical {\
background:transparent;\
width: 10px;\
 border-radius:3;\
}\
QScrollBar::handle:vertical {\
background:#bec2c8;\
min-height: 20px;\
max-height:200px;\
 border-radius:3;\
}\
QScrollBar::handle:vertical:hover {\
background:#77797c;\
}\
QScrollBar::handle:vertical:pressed {\
 background:#d6dae0;\
}\
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
      background: none;\
  }\
QScrollBar::add-line:vertical {\
    background: transparent;\
    width: 0px;\
height:0;\
    subcontrol-position: top;\
    subcontrol-origin: margin;\
}\
QScrollBar::sub-line:vertical {\
                         background: transparent;\
                         width: 0px;\
                     height:0;\
                         subcontrol-position: bottom;\
subcontrol-origin: margin;\
                     }\
");
QComboBox*t=dynamic_cast<QComboBox*>(this);
t->setView(view);
t->setModel(model);
//用于设置QComboBox 的编辑框颜色(位于ui->citypop上)
comboBoxSheet0="QComboBox {\
        color:rgb(0,0,0);\
border:1px solid rgb(0, 155, 219);\
border-radius: 5px;\
padding: 0 0 0 5;\
background-color: rgb(255, 255, 255);\
}\
QComboBox::down-arrow {\
image:url(:/images/down-arrow.png)\
}\
QComboBox::drop-down {\
    subcontrol-origin: 15;\
    subcontrol-position: right;\
width:15px;\
color:red;\
    border-bottom-right-radius: 5px;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           image:url(:/images/up-arrow.png)\
                           }";
  comboBoxSheet1="QComboBox {\
  color:rgb(150,150,150);\
border:1px solid rgb(0, 155, 219);\
border-radius: 5px;\
padding: 0 0 0 5;\
background-color: rgb(238, 238, 238);\
}\
QComboBox::down-arrow {\
image:url(:/images/down-arrow.png)\
}\
QComboBox::drop-down {\
    subcontrol-origin: 15;\
    subcontrol-position: right;\
width:15px;\
color:red;\
    border-bottom-right-radius: 5px;\
}\
QComboBox::down-arrow:on { /* shift the arrow when popup is open */\
                           image:url(:/images/up-arrow.png)\
                           }";
  }

  SubComboBox::~SubComboBox()
{
    if(view){
        delete view;
        view=nullptr;
    }
    if(model){
        delete model;
        model=nullptr;
    }
    if(filterModel){
        delete filterModel;
        filterModel=nullptr;
    }
}

void SubComboBox::hidePopup()
{
    QComboBox::hidePopup();
    view->hide();
}

void SubComboBox::showPopup()
{
    QComboBox::showPopup();
    view->show();
    qDebug()<<"showed view of subcombobox ";
}

void SubComboBox::setModel(PlaceModel *arg)
{
    if(model==arg){
        qDebug()<<"model==arg";
        return;
    }
    delete model;
    model=nullptr;
    model=arg;
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    t->setModel(model);
}

void SubComboBox::setView(QListView* arg)
{
    if(view==arg)return;
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    delete view;
    view=nullptr;
    view=arg;
    t->setView(view);
    t->setModel(model);

}

QVariant SubComboBox::data(const int &row, const int &role)
{
    if(filterModel->rowCount()>0){
        return filterModel->data(filterModel->index(row,0),role);
    }else{
         return model->data(model->index(row,0),role);
    }
}


void SubComboBox::filterValue(const QVariant &value, const PlaceModel::DataRole &role)
{
    //注意setRowHidden函数并不能更改视图大小，会导致不符合逻辑现象，所以只能用个模型来刷新视图
    if(filterModel){
        delete filterModel;
        filterModel=nullptr;
    };
    filterModel=new PlaceModel(this);
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    switch (role) {
    case PlaceModel::IdRole:
        filterModel->insert(0,model->data(0));
        for(int i=1;i<model->rowCount();i++)
            if(value.toLongLong()==model->data(model->index(i,0),role).toLongLong()){
               filterModel->insert(i,model->data(i));
            }
t->setModel(filterModel);
view->updateGeometry();
        return;
    case PlaceModel::NameRole:
        filterModel->insert(0,model->data(0));
        for(int i=1;i<model->rowCount();i++)
            if(value.toString()==model->data(model->index(i,0),role).toString()){
               filterModel->insert(i,model->data(i));
            }
t->setModel(filterModel);
view->updateGeometry();
        return;
    case PlaceModel::FidRole:
        filterModel->insert(0,model->data(0));
        for(int i=1;i<model->rowCount();i++)
            if(value.toLongLong()==model->data(model->index(i,0),role).toLongLong()){
               filterModel->insert(i,model->data(i));
            }
t->setModel(filterModel);
view->updateGeometry();
        return;
    default:
        return;
    }
}



void SubComboBox::setStyleSheet(const SubComboBox::SheetRole &role)
{
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    switch (role) {
    case SheetRole::EabledRole:
        t->setStyleSheet(comboBoxSheet0);
        break;
    case SheetRole::DisabledRole:
        t->setStyleSheet(comboBoxSheet1);
        break;
    default:
        break;
    }
}

void SubComboBox::setStyleSheet(const QString &sheet)
{
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    t->setStyleSheet(sheet);
}

void SubComboBox::appendItem(const qint64&id, const QString&text,const qint64&fid)
{
    model->append(id,text,fid);
}

int SubComboBox::count() const
{
    if(filterModel->rowCount()>0)
        return filterModel->rowCount();
    else
       return  model->rowCount();
}

PlaceModel *SubComboBox::getFilterModel()const
{
    PlaceModel*temp=new PlaceModel();
    *temp=*filterModel;
    return temp;
}

void SubComboBox::setFilterModel(PlaceModel*arg)
{
    filterModel=arg;
    QComboBox*t=dynamic_cast<QComboBox*>(this);
    t->setModel(filterModel);
}


