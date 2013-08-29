alter table users add column dateCreated int(12);
alter table users add column lastLogin int(12);
alter table users add column userLevel int(3) default 0;
alter table sessions add column userLevel int(3) default 0;
