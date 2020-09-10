
#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
#include<qsharedpointer.h>

extern QMap<QString,QFile*>historyImgFiles;//保存历史头像文件用于多线程上锁
extern QMap<QString,QFile*>sigFiles;//保存个性签名文件用于多线程上锁
//注意 qmuter没有=操作符
extern QMap<QString,QMutex*> historyImgMuter;//每个文件的互斥锁
extern QMap<QString,QMutex*>sigMuter;//每个文件的互斥锁
extern QMap<QString, QSharedPointer<QMutex> >gradeMuter;//等级锁 用引用计数智能指针控制锁资源 读写等级文件时上锁 qstring是号码

#endif // GLOBAL_H

