USE [MyQQ]
GO

/****** Object:  StoredProcedure [dbo].[updateUserInformation]    Script Date: 2020/9/9 20:33:23 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE proc  [dbo].[updateUserInformation]
@name varchar(24),@sex varchar(2),@birthday date,@blooGroup varchar(8),@home1 varchar(20),@home2 varchar(20),@home3 varchar(20),
@home4 varchar(20), @where1 varchar(20),@where2 varchar(20),@where3 varchar(20),@where4 varchar(20),@profession varchar(24),
@corporation varchar(80),@personalStatement varchar(162),@phone numeric(11,0),@education varchar(250),@myqq bigint
as begin
begin transaction  tran_temp
declare @count int
set @count=(select count(id) from city where name=@where2)
if @count>=1
--如果是自治市区，则把第三个地点转为第四个，第二个传给第三个，如此匹配数据
begin
set @where4=@where3
set @where3=@where2
end
set @count=(select count(id) from city where name=@home2)
if @count>=1
--如果是自治市区，则把第三个地点转为第四个，第二个传给第三个，如此匹配数据
begin
set @home4=@home3
set @home3=@home2
end
declare @id1 bigint, @id2 bigint, @id3 bigint, @id4 bigint, @id11 bigint, @id22 bigint, @id33 bigint, @id44 bigint
set @id1=(select top 1 id from country  where @where1=name)
set @id2=(select  top 1 id from province  where @where2=name)
set @id3=(select top 1  id from city  where @where3=name)
set @id4=(select top 1  id from county  where @where4=name)
set @id11=(select top 1  id from country  where @home1=name)
set @id22=(select top 1  id from province where @home2=name)
set @id33=(select top 1  id from city where @home3=name)
set @id44=(select top 1  id from county where @home4=name)
--获取职业id与血型id
declare @bid tinyint,@pid tinyint
set @bid=(select id from bloodGroup_T where name=@blooGroup)
set @pid=(select id from profession_T where name=@profession)
select @bid,@pid
--更新表userinfo 行级锁直到事务结束 期间只读 不允许脏读和其他事务更新
 update userInfo with(rowlock,updlock,holdlock)
 set myqqName=@name,sex=@sex,birthday=@birthday,bloodGroupId=@bid,locateInCountry=@id1,locateInProvince=@id2,locateInCity=@id3,
 locateInTown=@id4,homeTownCountry=@id11,homeTownProvince=@id22,homeTownCity=@id33,homeTown=@id44,professId=@pid,corporation=@corporation,
 personalStatement=@personalStatement,phone=@phone,education=@education
 --条件匹配
 where @myqq=myqq
select 'updateUserInfromation proc exec is successful'
 --提交事务
 commit tran  tran_temp
end

GO

