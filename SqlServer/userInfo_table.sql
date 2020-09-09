use myqq
drop table userInfo;
go
create table userInfo(
myqq  bigint primary key not null,--�˺�
myqqName varchar(24) not null default '',--�ǳ�
passwd varchar(16)not null constraint PC_passwd check(datalength(passwd)>7),--����
headImgPath varchar(80) not null default '',--ͷ��·��
personalizedSignature varchar(80) not null default '',--����ǩ��
sex varchar(2) not null constraint SC_sex check (sex in('��','Ů','')) constraint SD_sex default '' ,--�Ա�
birthday date not null constraint BD_birthday default dbo.getDefaultBirthday(),--����
registerDateTime datetime not null default getdate(),--ע��ʱ��
bloodGroupId tinyint not null references bloodGroup_T(id) constraint BD_bloodGroupId default 0, --Ѫ��id
homeTownCountry int not null default 0 references country(id),--���� 4
homeTownProvince bigint not null default  0 references province(id),--ʡ
homeTownCity bigint not null default  0 references city(id),--����
homeTown bigint not null default  0 references county(id),--��
locateInCountry int not null default 0 references country(id),--���ڵ� 4
locateInProvince  bigint  not null default 0 references province(id), 
locateInCity  bigint not null default  0 references city(id),
locateInTown  bigint not null default 0 references county(id),
professId tinyint not null references profession_T(id) constraint PD_professId default 0,--ְҵ
corporation varchar(80) not null default '',--��˾
personalStatement varchar(162) not null default '',--����˵��
photoWallPath varchar(80) not null default '',--��Ƭǽ·��
phone numeric(11,0) not null default 0,--�ֻ����� 11λ����
education varchar(250) not null default '',--���7���������� ��ʽ:ѧУ Ժϵ ��ѧʱ�� ѧ��;...
liveTime varchar(20) not null default  '0',--����ʱ��
activeDays bigint not null default 0,--��Ծ��
myqqGrade tinyint not null default 0,--�ȼ�
userState varchar(8)not null default '',--״̬
signLog varchar(max) not null default '',--��¼��¼
--ip varchar(128) not null default '',--ip��ַ
isConnected bit not null default 0,
)
