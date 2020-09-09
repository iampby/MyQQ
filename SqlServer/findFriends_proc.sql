USE [MyQQ]
GO

/****** Object:  StoredProcedure [dbo].[findFriends_pro]    Script Date: 2020/9/9 20:31:06 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE proc [dbo].[findFriends_pro]
--参数 @begin_id：号码的最小值 ,@containName：昵称包含的文本 @sex：性别  @begin_date和@end_date：生日范围   所在地及故乡
@begin_id bigint,@containName varchar(24),@sex varchar(2)='',@begin_date date= null,@end_date date=null,@isConnected bit=0,
@location1 varchar(20)='',@location2 varchar(20)='',@location3 varchar(20)='',@location4 varchar(20)='',@hometown1 varchar(20)='',
@hometown2 varchar(20)='',@hometown3 varchar(20)='',@hometown4 varchar(20)='',@isNumber bit =1 
as begin
begin tran proc_temp
if @begin_date=null
set @begin_date=cast('1901-01-01' as date)
if @end_date=null
set @end_date=GETDATE()
--如果为自治市区,4位置值为3位置
declare @count int
set @count=(select count(id) from city where name=@location2)
if @count>=1
begin
set @location4=@location3
set @location3=@location2
end
set @count=(select count(id) from city where name=@hometown2)
if @count>=1
begin
set @hometown4=@location3
set @location3=@location2
end
if @isNumber=0
begin
 --获取最多20个记录
-- set nocount off --不能设置选择开启与否，会导致Qt prepare不正常
select top 20 myqqName,myqq ,sex,birthday,headImgPath,personalizedSignature, --昵称 号码 性别 头像 所在地 个性签名
table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,table1_4.name as location4--所在地
--连接各表 userinfo加行锁直到事务结束,期间只读不可更新
from userInfo with(rowlock,holdlock) inner join country as table1_1 with(rowlock,holdlock)on table1_1.id=locateInCountry--连接国家表 table1_1代表所在地国家
 inner join province as table1_2 with(holdlock) on table1_2.id=locateInProvince --连接省表 table1_2代表所在地省
 inner join city as table1_3  with(holdlock) on table1_3.id=locateInCity--连接城市表 table1_3代表所在地市
 inner join county as table1_4 with(holdlock) on table1_4.id=locateInTown--连接县区表 table1_4代表所在地县区
 inner join country as table2_1 with(holdlock)on table2_1.id=homeTownCountry--连接国家表 table2_1代表故乡国家
 inner join province as table2_2 with(holdlock) on table2_2.id=homeTownProvince --连接省表 table2_2代表故乡省
 inner join city as table2_3  with(holdlock) on table2_3.id=homeTownCity--连接城市表 table2_3代表故乡市
 inner join county as table2_4 with(holdlock) on table2_4.id=homeTown--连接县区表 table2_4代表故乡县区
 --条件匹配
where myqqName like('%'+@containName+'%') AND sex like('%'+@sex+'%') AND birthday>=@begin_date AND birthday<@end_date--昵称 性别 年龄筛选
 AND userInfo.myqq >=@begin_id AND isConnected=@isConnected --起始号码开始 上线筛选
--故乡及所在地筛选
AND  table1_1.name like('%'+@location1+'%') AND table1_2.name like('%'+@location2+'%') AND table1_3.name like('%'+@location3+'%') AND
 table1_4.name like('%'+@location4+'%') AND table2_1.name like('%'+@hometown1+'%') AND table2_2.name like('%'+@hometown2+'%')
 AND table2_3.name like('%'+@hometown3+'%') AND table2_4.name like('%'+@hometown4+'%') 
 commit tran proc_temp
 end
 --直接查询号码
 else
 begin
 select myqqName,myqq ,sex,birthday,headImgPath,personalizedSignature, --昵称 号码 性别 头像 所在地 个性签名
table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,table1_4.name as location4--所在地
--连接各表 userinfo加行锁直到事务结束,期间只读不可更新
from userInfo with(rowlock,holdlock) inner join country as table1_1 with(rowlock,holdlock)on table1_1.id=locateInCountry--连接国家表 table1_1代表所在地国家
 inner join province as table1_2 with(holdlock) on table1_2.id=locateInProvince --连接省表 table1_2代表所在地省
 inner join city as table1_3  with(holdlock) on table1_3.id=locateInCity--连接城市表 table1_3代表所在地市
 inner join county as table1_4 with(holdlock) on table1_4.id=locateInTown--连接县区表 table1_4代表所在地县区
 inner join country as table2_1 with(holdlock)on table2_1.id=homeTownCountry--连接国家表 table2_1代表故乡国家
 inner join province as table2_2 with(holdlock) on table2_2.id=homeTownProvince --连接省表 table2_2代表故乡省
 inner join city as table2_3  with(holdlock) on table2_3.id=homeTownCity--连接城市表 table2_3代表故乡市
 inner join county as table2_4 with(holdlock) on table2_4.id=homeTown--连接县区表 table2_4代表故乡县区
 --条件匹配
where myqq=CAST(@containName as bigint)
  commit tran proc_temp
 end
end 


GO

