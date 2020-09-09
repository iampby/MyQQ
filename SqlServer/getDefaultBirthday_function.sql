USE [MyQQ]
GO

/****** Object:  UserDefinedFunction [dbo].[getDefaultBirthday]    Script Date: 2020/9/9 20:28:56 ******/
/*
函数：
获取默认的出生日期 当前年份-30
*/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE function [dbo].[getDefaultBirthday]()
returns date
begin
declare @curYear int,@bir varchar(10),@r date;
set @curYear=year(getdate())-30;
set @bir=cast(@curYear as varchar(4))+'-01-01';
set @r=cast(@bir as date);
return @r;
end
GO

