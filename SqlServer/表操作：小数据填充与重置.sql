use myqq
drop table globalVar_T
go
create table globalVar_T(
id tinyint primary key,--��ʶ����
dataType varchar(20) not null,--��������
value varbinary(max) not null,--����ֵ
explain varchar(max) not null,--��������
)
go
insert into globalVar_T
values
(0,'bigint',10000,'ȫ�ֱ���idΪ0, ��������Ϊbigint,��ʼֵΪ10000�����ڼ�¼ע��MyQQ�������ֵ'),
(1,'bigint',1000000,'ȫ�ֱ���idΪ1, ��������Ϊbigint,��ʼֵΪ1000000�����ڼ�¼ע��MyQQȺ�������ֵ')
go
delete profession_T;
delete bloodGroup_T;
delete infoType;
insert into profession_T
values
(0,''),
(1,'�����/������/ͨ��'),
(2,'����/����/����'),
(3,'ҽ��/����/��ҩ'),
(4,'����/����/Ͷ��/����'),
(5,'��ҵ/����ҵ/���徭Ӫ'),
(6,'�Ļ�/���/��ý'),
(7,'����/����/����'),
(8,'��ʦ/����'),
(9,'����/��ѵ'),
(10,'����Ա/����/��ҵ��λ'),
(11,'ģ��'),
(12,'�ս�'),
(13,'ѧ��'),
(14,'����');
go
insert into bloodGroup_T
values
(0,''),
(1,'A��'),
(2,'B��'),
(3,'O��'),
(4,'AB��'),
(5,'����Ѫ��');

go

