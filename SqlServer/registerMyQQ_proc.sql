USE [MyQQ]
GO

/****** Object:  StoredProcedure [dbo].[registerMyQQ_pro]    Script Date: 2020/9/9 20:33:12 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE proc [dbo].[registerMyQQ_pro]
@myqqName varchar(24),@passwd varchar(16),@myqq bigint output,@sucessful bit output
as begin
declare @err int=0,@value varbinary(max)=0
begin transaction mani_data
set @value=(select value from globalVar_T with(tablockx,holdlock) where id=0 )+1--加上表锁直到事务结束
set @err +=@@ERROR
set @myqq=cast(@value as bigint)
set @err +=@@ERROR
update  globalVar_T 
set value= @value where id=0
set @err +=@@ERROR
insert into  userInfo(myqq,myqqName,passwd)
values(@myqq,@myqqName,@passwd)
set @err +=@@ERROR
if(@err=0)
begin
set @sucessful=1
commit tran 
end
else
begin
set @sucessful=0
rollback tran 
end
end

GO

