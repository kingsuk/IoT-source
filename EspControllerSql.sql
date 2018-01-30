drop table dbo.UserData

CREATE TABLE [dbo].[UserData](
	[t_id] [int] IDENTITY(1,1),
	[user_id] [nvarchar](128) NOT NULL,
	[device_id] [int] NOT NULL,
	[name] [varchar](20) NULL,
	[jsondata] [varchar](100) NULL,
	[jsondataOpposit] [varchar](100) NULL,
	primary key(t_id),
	foreign key(user_id) references dbo.AspNetUsers(Id))

	select * from dbo.AspNetUsers

create procedure proc_user_userData
@user_id nvarchar(128),
@device_id int,
@name varchar(20),
@jsondata varchar(100),
@jsondataOpposit varchar(100)
AS
begin
INSERT INTO [dbo].[UserData]
           ([user_id]
		   ,[device_id]
           ,[name]
           ,[jsondata]
           ,[jsondataOpposit])
     VALUES
           (@user_id,@device_id,@name,@jsondata,@jsondataOpposit)
end





create proc proc_user_userData_showAll
as
begin
	select * from [dbo].[UserData]
end

exec proc_user_userData_showAll

create proc proc_user_userData_showAll_by_id
@user_id nvarchar(128)
as
begin
	select * from [dbo].[UserData] where user_id = @user_id
end

exec proc_user_userData_showAll_by_id 'f13c5fbf-2056-4da2-b180-6f6ce029e6ed'

create proc proc_user_get_id_by_email @email nvarchar(256)
as
begin
	SELECT Id FROM [dbo].[AspNetUsers] where Email=@email
end

