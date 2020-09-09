use myqq
drop table userInfo;
go
create table userInfo(
myqq  bigint primary key not null,--账号
myqqName varchar(24) not null default '',--昵称
passwd varchar(16)not null constraint PC_passwd check(datalength(passwd)>7),--密码
headImgPath varchar(80) not null default '',--头像路径
personalizedSignature varchar(80) not null default '',--个性签名
sex varchar(2) not null constraint SC_sex check (sex in('男','女','')) constraint SD_sex default '' ,--性别
birthday date not null constraint BD_birthday default dbo.getDefaultBirthday(),--生日
registerDateTime datetime not null default getdate(),--注册时间
bloodGroupId tinyint not null references bloodGroup_T(id) constraint BD_bloodGroupId default 0, --血型id
homeTownCountry int not null default 0 references country(id),--故乡 4
homeTownProvince bigint not null default  0 references province(id),--省
homeTownCity bigint not null default  0 references city(id),--城市
homeTown bigint not null default  0 references county(id),--镇
locateInCountry int not null default 0 references country(id),--所在地 4
locateInProvince  bigint  not null default 0 references province(id), 
locateInCity  bigint not null default  0 references city(id),
locateInTown  bigint not null default 0 references county(id),
professId tinyint not null references profession_T(id) constraint PD_professId default 0,--职业
corporation varchar(80) not null default '',--公司
personalStatement varchar(162) not null default '',--个人说明
photoWallPath varchar(80) not null default '',--照片墙路径
phone numeric(11,0) not null default 0,--手机号码 11位数字
education varchar(250) not null default '',--最多7条教育经历 格式:学校 院系 入学时间 学历;...
liveTime varchar(20) not null default  '0',--在线时间
activeDays bigint not null default 0,--活跃度
myqqGrade tinyint not null default 0,--等级
userState varchar(8)not null default '',--状态
signLog varchar(max) not null default '',--登录记录
--ip varchar(128) not null default '',--ip地址
isConnected bit not null default 0,
)
