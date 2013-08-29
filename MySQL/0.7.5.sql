alter table sessions add column lastSend int(12) default 0;
alter table sessions add column numSent int(5) default 0;