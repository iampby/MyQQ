#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "widget.h"
#include "ui_widget.h"
#include"global.h"
#include"myqqloginserver.h"
#include"myqqregisterserver.h"
#include"writeserver.h"
#include<qsqldatabase.h>
#include <qsqlquery.h>
#include<qmessagebox.h>
#include <qtcpsocket.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    regServer=nullptr;logServer=nullptr;
    registerPort=5566;loginPort=5567;writePort=5568;
}

Widget::~Widget()
{
    delete ui;
    for (QFile*v :historyImgFiles) {
        delete v,v=nullptr;
    }
    historyImgFiles.clear();

    for (QFile*v :sigFiles) {
        delete v,v=nullptr;
    }
    sigFiles.clear();

}


void Widget::on_pushButton1_clicked()
{
    if(ui->pushButton1->text()==("开启")){
        regServer=new MyQQRegisterServer(this);
        if(!regServer->listen(QHostAddress::Any,registerPort)){
            QMessageBox::critical(this,("失败"),("开启注册服务器失败！"));
            delete  regServer;
            regServer=nullptr;
            return;
        }
        ui->label1->setText(("注册服务器已开启！"));
        ui->pushButton1->setText(("关闭"));
    }else{
        regServer->close();
        if(regServer){
            delete regServer;
            regServer=nullptr;
        }
        ui->label1->setText(("注册服务器已关闭！"));
        ui->pushButton1->setText(("开启"));
    }
}

void Widget::on_pushButton2_clicked()
{
    if(ui->pushButton2->text()==("开启")){
        logServer=new MyQQLoginServer(this);
        if(!logServer->listen(QHostAddress::Any,loginPort)){
            QMessageBox::critical(this,("失败"),("开启登录服务器失败！"));
            delete  logServer;
            logServer=nullptr;
            return;
        }
        ui->label2->setText(("登录服务器已开启！"));
        ui->pushButton2->setText(("关闭"));
    }else{
        logServer->close();
        if(logServer){
            delete logServer;
            logServer=nullptr;
        }
        ui->label2->setText(("登录服务器已关闭！"));
        ui->pushButton2->setText(("开启"));
    }
}

void Widget::on_pushButton3_clicked()
{
    if(ui->pushButton3->text()==("开启")){
        writeServer=new WriteServer(this);
        if(!writeServer->listen(QHostAddress::Any,writePort)){
            QMessageBox::critical(this,("失败"),("开启更新信息服务器失败！"));
            delete  writeServer;
            writeServer=nullptr;
            return;
        }
        ui->label3->setText(("更新信息服务器已开启！"));
        ui->pushButton3->setText(("关闭"));
    }else{
        writeServer->close();
        if(writeServer){
            delete writeServer;
            writeServer=nullptr;
        }
        ui->label3->setText(("更新信息服务器已关闭！"));
        ui->pushButton3->setText(("开启"));
    }
}
