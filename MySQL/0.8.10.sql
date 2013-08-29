drop table cache;
CREATE TABLE cache(
	id int NOT NULL auto_increment,
	owner int NOT NULL,
	cache_key varchar(64),
	cache_data text,
	cache_ts int(11),
	volatile bool,
	PRIMARY KEY(id),
	KEY id (id),
	KEY owner (owner),
	KEY cache_key (cache_key),
	KEY cache_ts (cache_ts),
	KEY volatile (volatile)
);