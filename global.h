#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
extern QMap<QString,QFile*>historyImgFiles;//������ʷͷ���ļ����ڶ��߳�����
extern QMap<QString,QFile*>sigFiles;//�������ǩ���ļ����ڶ��߳�����
extern QMutex historyImgMuter;//������
extern QMutex sigMuter;//������

#endif // GLOBAL_H
