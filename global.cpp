#include"global.h"

QMap<QString,QFile*>historyImgFiles;
QMap<QString,QFile*>sigFiles;//�������ǩ���ļ����ڶ��߳�����

 QMap<QString,QMutex*> historyImgMuter;//������
 QMap<QString,QMutex*>sigMuter;//������
QMap<QString, QSharedPointer<QMutex> >gradeMuter;//�ȼ��� �����ü�������ָ���������Դ ��д�ȼ��ļ�ʱ���� qstring�Ǻ���
