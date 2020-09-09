
drop table county;
drop table city;
drop table province;
drop table country;
create table country(
id int primary key,
name varchar(20) not null default '',
)
create table province(
id bigint primary key,
name varchar(20) not null default '',
lookUrlPath varchar(100) not null  default '',
belongCountryId int not null default 0 references country(id) ON DELETE CASCADE ON UPDATE CASCADE,
)
create table city(
id bigint primary key,
name varchar(20) not null default '',
lookUrlPath varchar(100) not null  default '',
belongCityId bigint not null default 0 references province(id) ON DELETE CASCADE ON UPDATE CASCADE,
)
create table county(
id bigint primary key,
name varchar(20) not null default '',
lookUrlPath varchar(100) not null  default '',
belongCountyId bigint not null default 0 references city(id) ON DELETE CASCADE ON UPDATE CASCADE,
)
go
--初始化城市默认数据
insert into country
values(0,''),
(1,'中国')
insert into province(id)
values(0)
insert into city(id)
values(0)
insert into county(id)
values(0)