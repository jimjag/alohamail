alter table prefs add column showContacts char default '0';
alter table prefs add column showCC char default '1';
alter table prefs add column closeAfterSend char default '1';
alter table colors add column font_family varchar(255);
alter table colors add column font_size int(2) default '12';
alter table colors add column small_font_size int(2) default '10';
alter table colors add column menu_font_size int(2) default '12';
alter table colors add column main_head_bg varchar(15) default '#333377';
alter table colors add column main_head_txt varchar(15) default '#FFFFFF';
alter table contacts add column msn text;
alter table contacts add column yahoo text;
alter table contacts add column jabber text;
alter table prefs add column showNav char default '1';
alter table prefs add column folderlistWidth int(3) default '150';
alter table colors add column main_darkbg varchar(15) default '#5B5B79';
alter table colors add column main_light_txt varchar(15) default '#FFFFFF';
alter table prefs add column hideUnsubscribed char default '';
alter table prefs add column compose_inside char default '';
alter table prefs add column show_quota char default '';
alter table prefs add column showNumUnread char default '';
alter table prefs add column refresh_folderlist char default '';
alter table prefs add column folderlist_interval int(3) default '150';
alter table prefs add column radar_interval int(3) default '150';
alter table colors add column folderlist_font_size int(2) default '12';
alter table prefs add column theme varchar(128) default 'default';
alter table prefs add column main_toolbar varchar(3) default 'b';
alter table prefs add column alt_identities text;
CREATE TABLE identities(
	id int NOT NULL auto_increment,
    owner mediumint(9) NOT NULL,
	name varchar(128),
	email varchar(128),
	replyto varchar(128),
	sig text,
	PRIMARY KEY (id),
	KEY id (id),
	KEY owner (owner)
);
