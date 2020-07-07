#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
extern QMap<QString,QFile*>files;//保存文件用于多线程上锁
extern QMutex muter;//互斥锁

#endif // GLOBAL_H
