#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
extern QMap<QString,QFile*>files;//�����ļ����ڶ��߳�����
extern QMutex muter;//������

#endif // GLOBAL_H
