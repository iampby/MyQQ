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
    if(ui->pushButton1->text()==QStringLiteral("����")){
        regServer=new MyQQRegisterServer(this);
        if(!regServer->listen(QHostAddress::Any,registerPort)){
            QMessageBox::critical(this,QStringLiteral("ʧ��"),QStringLiteral("����ע�������ʧ�ܣ�"));
            delete  regServer;
            regServer=nullptr;
            return;
        }
        ui->label1->setText(QStringLiteral("ע��������ѿ�����"));
        ui->pushButton1->setText(QStringLiteral("�ر�"));
    }else{
        regServer->close();
        if(regServer){
            delete regServer;
            regServer=nullptr;
        }
        ui->label1->setText(QStringLiteral("ע��������ѹرգ�"));
        ui->pushButton1->setText(QStringLiteral("����"));
    }
}

void Widget::on_pushButton2_clicked()
{
    if(ui->pushButton2->text()==QStringLiteral("����")){
        logServer=new MyQQLoginServer(this);
        if(!logServer->listen(QHostAddress::Any,loginPort)){
            QMessageBox::critical(this,QStringLiteral("ʧ��"),QStringLiteral("������¼������ʧ�ܣ�"));
            delete  logServer;
            logServer=nullptr;
            return;
        }
        ui->label2->setText(QStringLiteral("��¼�������ѿ�����"));
        ui->pushButton2->setText(QStringLiteral("�ر�"));
    }else{
        logServer->close();
        if(logServer){
            delete logServer;
            logServer=nullptr;
        }
        ui->label2->setText(QStringLiteral("��¼�������ѹرգ�"));
        ui->pushButton2->setText(QStringLiteral("����"));
    }
}

void Widget::on_pushButton3_clicked()
{
    if(ui->pushButton3->text()==QStringLiteral("����")){
        writeServer=new WriteServer(this);
        if(!writeServer->listen(QHostAddress::Any,writePort)){
            QMessageBox::critical(this,QStringLiteral("ʧ��"),QStringLiteral("����д���ļ�������ʧ�ܣ�"));
            delete  writeServer;
            writeServer=nullptr;
            return;
        }
        ui->label3->setText(QStringLiteral("д���ļ��������ѿ�����"));
        ui->pushButton3->setText(QStringLiteral("�ر�"));
    }else{
        writeServer->close();
        if(writeServer){
            delete writeServer;
            writeServer=nullptr;
        }
        ui->label3->setText(QStringLiteral("д���ļ��������ѹرգ�"));
        ui->pushButton3->setText(QStringLiteral("����"));
    }
}
