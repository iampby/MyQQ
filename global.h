#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
extern QMap<QString,QFile*>historyImgFiles;//保存历史头像文件用于多线程上锁
extern QMap<QString,QFile*>sigFiles;//保存个性签名文件用于多线程上锁
extern QMutex historyImgMuter;//互斥锁
extern QMutex sigMuter;//互斥锁

#endif // GLOBAL_H
