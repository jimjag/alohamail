# New to 0.7.3
# This table is needed only if you would like to
# store log entries in the MySQL backend.
# See conf/conf.inc for more information on the 
# log feature.

CREATE TABLE user_log(
	logTime datetime,
	logTimeStamp int(12),
	userID int,
	account text,
	action text,
	comment text,
	ip varchar(15)
);