#include"global.h"

QMap<QString,QFile*>historyImgFiles;
QMap<QString,QFile*>sigFiles;//保存个性签名文件用于多线程上锁

 QMap<QString,QMutex*> historyImgMuter;//互斥锁
 QMap<QString,QMutex*>sigMuter;//互斥锁
