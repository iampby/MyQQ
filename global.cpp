#include"global.h"

QMap<QString,QFile*>historyImgFiles;
QMap<QString,QFile*>sigFiles;//�������ǩ���ļ����ڶ��߳�����
QMutex historyImgMuter;
QMutex sigMuter;
