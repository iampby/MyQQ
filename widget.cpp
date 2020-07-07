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
    for (QFile*v :files) {
        delete v,v=nullptr;
    }
    files.clear();
}


void Widget::on_pushButton1_clicked()
{
    if(ui->pushButton1->text()==QStringLiteral("开启")){
        regServer=new MyQQRegisterServer(this);
        if(!regServer->listen(QHostAddress::Any,registerPort)){
            QMessageBox::critical(this,QStringLiteral("失败"),QStringLiteral("开启注册服务器失败！"));
            delete  regServer;
            regServer=nullptr;
            return;
        }
        ui->label1->setText(QStringLiteral("注册服务器已开启！"));
        ui->pushButton1->setText(QStringLiteral("关闭"));
    }else{
        regServer->close();
        if(regServer){
            delete regServer;
            regServer=nullptr;
        }
        ui->label1->setText(QStringLiteral("注册服务器已关闭！"));
        ui->pushButton1->setText(QStringLiteral("开启"));
    }
}

void Widget::on_pushButton2_clicked()
{
    if(ui->pushButton2->text()==QStringLiteral("开启")){
        logServer=new MyQQLoginServer(this);
        if(!logServer->listen(QHostAddress::Any,loginPort)){
            QMessageBox::critical(this,QStringLiteral("失败"),QStringLiteral("开启登录服务器失败！"));
            delete  logServer;
            logServer=nullptr;
            return;
        }
        ui->label2->setText(QStringLiteral("登录服务器已开启！"));
        ui->pushButton2->setText(QStringLiteral("关闭"));
    }else{
        logServer->close();
        if(logServer){
            delete logServer;
            logServer=nullptr;
        }
        ui->label2->setText(QStringLiteral("登录服务器已关闭！"));
        ui->pushButton2->setText(QStringLiteral("开启"));
    }
}

void Widget::on_pushButton3_clicked()
{
    if(ui->pushButton3->text()==QStringLiteral("开启")){
        writeServer=new WriteServer(this);
        if(!writeServer->listen(QHostAddress::Any,writePort)){
            QMessageBox::critical(this,QStringLiteral("失败"),QStringLiteral("开启写入文件服务器失败！"));
            delete  writeServer;
            writeServer=nullptr;
            return;
        }
        ui->label3->setText(QStringLiteral("写入文件服务器已开启！"));
        ui->pushButton3->setText(QStringLiteral("关闭"));
    }else{
        writeServer->close();
        if(writeServer){
            delete writeServer;
            writeServer=nullptr;
        }
        ui->label3->setText(QStringLiteral("写入文件服务器已关闭！"));
        ui->pushButton3->setText(QStringLiteral("开启"));
    }
}
