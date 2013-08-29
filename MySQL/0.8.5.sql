alter table prefs add column main_cols varchar(10) default 'csfdzam';
CREATE TABLE bookmarks(
	id int NOT NULL auto_increment,
	owner int NOT NULL,
	name text,
	url text,
	grp text,
	is_private char,
	comments text,
	PRIMARY KEY (id),
	KEY id (id),
	KEY owner (owner)
);
alter table prefs add column clock_system tinyint(4) default 12;
CREATE TABLE cache(
	id int NOT NULL auto_increment,
	owner int NOT NULL,
	cache_key varchar(64),
	cache_data text,
	PRIMARY KEY(id),
	KEY id (id),
	KEY owner (owner),
	KEY cache_key (cache_key)
);
alter table prefs add column nav_no_flag char;