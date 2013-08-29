CREATE TABLE datastore(
	id int NOT NULL auto_increment,
	owner int NOT NULL,
	ds_key varchar(16) NOT NULL,
	ds_data text,
	PRIMARY KEY id (id),
	KEY id (id),
	KEY owner (owner),
	KEY ds_key (ds_key)
);

CREATE TABLE filters(
	id int NOT NULL auto_increment,
	owner int NOT NULL,
	name text,
	rule text,
	flags text,
	sort_order int,
	PRIMARY KEY id (id),
	KEY id (id),
	KEY sort_order (sort_order)
);

#added 3/22/04
ALTER TABLE contacts ADD COLUMN shared char;
ALTER TABLE contacts ADD KEY shared(shared);

#added 4/8/04
ALTER TABLE calendar ADD COLUMN shared char;
ALTER TABLE calendar ADD KEY shared(shared);

#added 08/13/04
alter table sessions add column check_data varchar(64);
alter table sessions add column check_sum int(4);

#added 8/16/04
ALTER TABLE contacts ADD COLUMN firstname text;
ALTER TABLE contacts ADD COLUMN lastname text;
ALTER TABLE contacts ADD COLUMN street text;
ALTER TABLE contacts ADD COLUMN extended text;
ALTER TABLE contacts ADD COLUMN city text;
ALTER TABLE contacts ADD COLUMN region text;
ALTER TABLE contacts ADD COLUMN postalcode text;
ALTER TABLE contacts ADD COLUMN country text;

#added 9/5/04
ALTER TABLE bookmarks ADD COLUMN rss text;

#added 12/27/04
ALTER TABLE cache ADD COLUMN cache_ts int(11);
ALTER TABLE cache ADD COLUMN volatile bool;
ALTER TABLE cache ADD INDEX cache_ts (cache_ts);
ALTER TABLE cache ADD INDEX volatile (volatile);

#added 1/3/05
ALTER TABLE calendar ADD COLUMN allday char;

#added 3/12/05
alter table datastore add column format char;

#added 3/21/05
ALTER TABLE sessions ADD COLUMN js_mode char(1);

#added 3/22/05
ALTER TABLE calendar ADD COLUMN publish char;
ALTER TABLE calendar ADD KEY publish (publish);

#added 3/31/05
ALTER TABLE contacts ADD COLUMN cal_url text;
ALTER TABLE contacts ADD COLUMN rss_url text;
