#include"global.h"

QMap<QString,QFile*>historyImgFiles;
QMap<QString,QFile*>sigFiles;//保存个性签名文件用于多线程上锁

 QMap<QString,QMutex*> historyImgMuter;//互斥锁
 QMap<QString,QMutex*>sigMuter;//互斥锁
QMap<QString, QSharedPointer<QMutex> >gradeMuter;//等级锁 用引用计数智能指针控制锁资源 读写等级文件时上锁 qstring是号码
