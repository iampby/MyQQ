#include"global.h"

QMap<QString,QFile*>historyImgFiles;
QMap<QString,QFile*>sigFiles;//保存个性签名文件用于多线程上锁
QMutex historyImgMuter;
QMutex sigMuter;
