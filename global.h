#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>

extern QMap<QString,QFile*>historyImgFiles;//������ʷͷ���ļ����ڶ��߳�����
extern QMap<QString,QFile*>sigFiles;//�������ǩ���ļ����ڶ��߳�����
//ע�� qmuterû��=������
extern QMap<QString,QMutex*> historyImgMuter;//ÿ���ļ��Ļ�����
extern QMap<QString,QMutex*>sigMuter;//ÿ���ļ��Ļ�����

#endif // GLOBAL_H
