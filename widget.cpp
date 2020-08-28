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
    if(ui->pushButton1->text()==("����")){
        regServer=new MyQQRegisterServer(this);
        if(!regServer->listen(QHostAddress::Any,registerPort)){
            QMessageBox::critical(this,("ʧ��"),("����ע�������ʧ�ܣ�"));
            delete  regServer;
            regServer=nullptr;
            return;
        }
        ui->label1->setText(("ע��������ѿ�����"));
        ui->pushButton1->setText(("�ر�"));
    }else{
        regServer->close();
        if(regServer){
            delete regServer;
            regServer=nullptr;
        }
        ui->label1->setText(("ע��������ѹرգ�"));
        ui->pushButton1->setText(("����"));
    }
}

void Widget::on_pushButton2_clicked()
{
    if(ui->pushButton2->text()==("����")){
        logServer=new MyQQLoginServer(this);
        if(!logServer->listen(QHostAddress::Any,loginPort)){
            QMessageBox::critical(this,("ʧ��"),("������¼������ʧ�ܣ�"));
            delete  logServer;
            logServer=nullptr;
            return;
        }
        ui->label2->setText(("��¼�������ѿ�����"));
        ui->pushButton2->setText(("�ر�"));
    }else{
        logServer->close();
        if(logServer){
            delete logServer;
            logServer=nullptr;
        }
        ui->label2->setText(("��¼�������ѹرգ�"));
        ui->pushButton2->setText(("����"));
    }
}

void Widget::on_pushButton3_clicked()
{
    if(ui->pushButton3->text()==("����")){
        writeServer=new WriteServer(this);
        if(!writeServer->listen(QHostAddress::Any,writePort)){
            QMessageBox::critical(this,("ʧ��"),("����������Ϣ������ʧ�ܣ�"));
            delete  writeServer;
            writeServer=nullptr;
            return;
        }
        ui->label3->setText(("������Ϣ�������ѿ�����"));
        ui->pushButton3->setText(("�ر�"));
    }else{
        writeServer->close();
        if(writeServer){
            delete writeServer;
            writeServer=nullptr;
        }
        ui->label3->setText(("������Ϣ�������ѹرգ�"));
        ui->pushButton3->setText(("����"));
    }
}
