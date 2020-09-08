#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QFile>
#include<qmutex.h>
#include<qsharedpointer.h>

extern QMap<QString,QFile*>historyImgFiles;//������ʷͷ���ļ����ڶ��߳�����
extern QMap<QString,QFile*>sigFiles;//�������ǩ���ļ����ڶ��߳�����
//ע�� qmuterû��=������
extern QMap<QString,QMutex*> historyImgMuter;//ÿ���ļ��Ļ�����
extern QMap<QString,QMutex*>sigMuter;//ÿ���ļ��Ļ�����
extern QMap<QString, QSharedPointer<QMutex> >gradeMuter;//�ȼ��� �����ü�������ָ���������Դ ��д�ȼ��ļ�ʱ���� qstring�Ǻ���

#endif // GLOBAL_H
