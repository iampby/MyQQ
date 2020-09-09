use myqq
drop table globalVar_T
go
create table globalVar_T(
id tinyint primary key,--标识变量
dataType varchar(20) not null,--数据类型
value varbinary(max) not null,--数据值
explain varchar(max) not null,--变量描述
)
go
insert into globalVar_T
values
(0,'bigint',10000,'全局变量id为0, 数据类型为bigint,初始值为10000，用于记录注册MyQQ号码最大值'),
(1,'bigint',1000000,'全局变量id为1, 数据类型为bigint,初始值为1000000，用于记录注册MyQQ群号码最大值')
go
delete profession_T;
delete bloodGroup_T;
delete infoType;
insert into profession_T
values
(0,''),
(1,'计算机/互联网/通信'),
(2,'生产/工艺/制造'),
(3,'医疗/治理/制药'),
(4,'金融/银行/投资/保险'),
(5,'商业/服务业/个体经营'),
(6,'文化/广告/传媒'),
(7,'娱乐/艺术/表演'),
(8,'律师/法务'),
(9,'教育/培训'),
(10,'公务员/行政/事业单位'),
(11,'模特'),
(12,'空姐'),
(13,'学生'),
(14,'其他');
go
insert into bloodGroup_T
values
(0,''),
(1,'A型'),
(2,'B型'),
(3,'O型'),
(4,'AB型'),
(5,'其它血型');

go

