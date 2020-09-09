USE [MyQQ]
GO

/****** Object:  StoredProcedure [dbo].[myselfInformation]    Script Date: 2020/9/9 20:33:03 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE proc [dbo].[myselfInformation]
@number bigint 
as begin
select myqqName,personalizedSignature,sex,myqqGrade,birthday,registerDateTime,table3.name as bloodGroup,education,table4.name as profession,
corporation,table1_1.name as location1,table1_2.name as location2,table1_3.name as location3,
table1_4.name as location4 ,table2_1.name as home1,table2_2.name as home2,
table2_3.name as home3,table2_4.name as home4,personalStatement,phone ,photoWallPath
from userInfo  inner join country as table1_1 on table1_1.id=locateInCountry--连接国家表 table1_1代表所在地国家
 inner join province as table1_2  on table1_2.id=locateInProvince --连接省表 table1_2代表所在地省
 inner join city as table1_3   on table1_3.id=locateInCity--连接城市表 table1_3代表所在地市
 inner join county as table1_4  on table1_4.id=locateInTown--连接县区表 table1_4代表所在地县区
 inner join country as table2_1 on table2_1.id=homeTownCountry--连接国家表 table2_1代表故乡国家
 inner join province as table2_2  on table2_2.id=homeTownProvince --连接省表 table2_2代表故乡省
 inner join city as table2_3   on table2_3.id=homeTownCity--连接城市表 table2_3代表故乡市
 inner join county as table2_4  on table2_4.id=homeTown--连接县区表 table2_4代表故乡县区
 inner join bloodGroup_T as table3 on table3.id=bloodGroupId--连接血型表
 inner join profession_T as table4 on table4.id=professId--连接职业表
 where @number=myqq --指定用户
 end

GO

