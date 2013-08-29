<?php
/********************************************************
	conf/conf.php
	
	PURPOSE:
		General settings and configuration.

********************************************************/


/*******  FOR OPTIMAL PERFORMANCE *******
	As you can see this file contains a lot of comments.
	For optimal performance, remove all comments and only
	leave directives you need and use.
*/


/***************** BACKEND ****************
	IlohaMail supports multiple backends, namely, file
	and/or database based.  If a database backend is used,
	a file-based backend can still be used to certain
	features.
	
	Deprecated.  Use conf/db_conf.php
*/
$backend = "FS";

//	$UPLOAD_DIR path to uploads dir
//	MUST HAVE TRAILING '/'
//	REQUIRED:  Always
$UPLOAD_DIR = "../data/uploads/";

//	$CACHE_DIR path to cache dir
//	MUST HAVE TRAILING '/'
//  REQUIRED:  Always (folder it self does not need to exist)
$CACHE_DIR = "../data/cache/";

//	$USER_DIR path to users dir
//	MUST HAVE TRAILING'/'
//	REQUIRED:  For FS-backend
$USER_DIR = "../data/users/";

//	$SESSION_DIR path to sessions dir
//	MUST HAVE TRAILING '/'
//	REQUIRED:  For FS-backend
$SESSION_DIR = "../data/sessions/";

//  FS_LARGE_SCALE: Set to true to use user dir naming scheme 
//					for large installations
//  If false:
//		*_DIR/user.host
//	if true
//  	*_DIR/u/s/user.host
$FS_LARGE_SCALE = false;

//	FS_MIGRATE: If true, and FS_LARGE_SCALE is true, existing
//				user dirs will be moved, preserving data.
//				Set to false if there is no existing data.
$FS_MIGRATE = true;


/********************* LOG *****************
	The log feature in IlohaMail logs all log in/out
	attempts, and works with both file-base and DB-based
	backends.  The log feature is deactivated by default.
*/

//	Set the following value to true to active logging
$log_active = false;

//	Log backend
//	The log backend defaults to $backend, but falls
//	back to "FS" if $log_file (below) is set.
//	In other words, the options are:
//		"": default
//		"syslog" : log to syslog
$log_backend = "";

//	Log file path (file-based backend only)
//	Use relative path to source file, or absolute path
//	PHP must have write privileges to the file and/or
// 	parent directory.
$log_file = "";

//	Template entry (file-based backend only)
//	The log feature will take this line, and replace the
//	keywords with appropriate information to enter in the
//	log.  Use any combination of the following keywords:
//		"date" : date and time
//		"ip" : client IP address
//		"acct" : if available, email account and server
//		"comment" : Error messages or comments
$log_template = "[date] ip:acct - action (comment)";


/**************** Spam Prevention ************
	This feature is not for blocking incoming spam, but 
	for preventing possible use of IlohaMail for spamming
	purposes.
	
	This will involve a three-level approach.
	Level 1: Restrict number of recepients per email
	Level 2: Restrict number of recepients per session
	Level 3: Set minimum interval between sending
	
	In a restrictive setting, a spammer may be forced
	to send to 10 people at a time, once every minute
	and log out and log back in after sending 5 messages.  
	This should effectively discourage manual and automated 
	spamming.
*/

//	Maximum number of recepients per message.
//	Will count To,CC,BCC fields.  More specifically
//	it will count the number of '@' symbols.
$max_rcpt_message = 50;

//	Maximum number of recepients per session.
//	Note: This is not the number of messages,
//	but the total number of recepients.
$max_rcpt_session = 100;

//	Minimum interval between send operations,
//	in seconds.
$min_send_interval = 15;


/**************** spam reporting *************
	Specify an email address, if one is available,
	where users can report spam.  This will add a
	link in the read message window allowing users
	to report spam in two simple clicks.
	
	Leave the string empty to disable this feature.
*/

$report_spam_to="";


/***************** Auth Mode ***************
	Specify default IMAP authentication method.
	Choices are:
		"plain" : 	Always use plain text
					Greatest compatibility
					
		"auth"	:	Try encrypted authenctication
					(CRAM-MD5), first, then plain
					
		"check" :	Check the server's capabilities
					for CRAM-MD5, and use appropriate
					auth method.
		"apop"	:	For POP3 only
		"none"	:   For SMTP only.  Use if SMTP server
					does not require authentication.
*/

$AUTH_MODE["imap"] = "plain";
$AUTH_MODE["pop3"] = "plain";
$AUTH_MODE["smtp"] = "";


/***************** Time Limit ***************
	Set maximum execution time.  Opening mailboxes
	with even thousands of messages shouldn't have
	problems, but a large POP account may time out.
	
	Set value to 0 for unlimited timeout.
*/

$MAX_EXEC_TIME = 60;


/***************** Trust User Address ********
    This directive specifies whether to use the
    user specified email address in the From header.
    
    When on (set to true or 1):
        User specified address is used in From header
        Authenticated email address used for Sender header
    When off (set to false or 0):
        Authenticated email address used for From header
        User specified address used in Reply-To header
        
    Note:
        The 'init_from_address' option needs to be configured
        in conf/defaults.generic.inc (or defaults.host.inc)
        if server host name is not same as email domain name
        e.g. 
            if server is imap.domain.com but email address
            is user@domain.com
         
*/

$TRUST_USER_ADDRESS = 1;


/***************** SMTP Server  **************
	SMTP server to use.  Default is "localhost",
	however any SMTP server that'll allow relaying
	from your webmail server can be used.
	Authenticated SMTP is currently not supported.
	
	Leave string empty to use PHP's mail() function.
	(Might work more reliably.)
*/

$SMTP_SERVER = "";


/***************** SMTP Type  **************
	The SMTP server type.  This is used to 
	accomodate slight differences between SMTP
	servers.
	
	This directive only takes effect if $SMTP_SERVER
	is specified.
	
	Supported values:  "sendmail", "courier"
	
	NOTE: if you're not sure, just use "sendmail"
*/

$SMTP_TYPE = "sendmail";


/***************** SMTP User/Pass  **************
	If you have a password protected SMTP server,
	and would like all webmail users to send even
	if they can't directly authenticate against
	your SMTP server themselves, use the directives
	below to specify which user to authenticate as.
	If empty, users' login and passwords will be used
	instead.
	
	ONLY FOR AUTH SMTP
*/
$SMTP_USER = "";
$SMTP_PASSWORD = "";


/***************** Tag-Lines  *****************
	TAG-LINES are blurbs that can be attached at the
	end of messages sent through IlohaMail.
	If you do not want tag-lines attached, leave the
	string empty.  If you want taglines on some users
	but not on others, set the $TAG_LEVEL to a number
	greater than 0, and change the user's userLevel
	in the backend (only works with database backend).
	
	Example:
	$TAG_LINE = "---------------\n";
	$TAG_LINE .= "This message was sent using IlohaMail";
*/

$TAG_LINE = "";

$TAG_LEVEL = 0;


/***************** Maximum Session  *****************
	MAX_SESSION_TIME specifies the maximum length of
	time users can be logged in without having to log
	out.  After the MAX_SESSION_TIME is over, users
	will be forced to log out and log back in.
	
	VALUE MUST BE SECONDS.  
	
	e.g. 
	$MAX_SESSION_TIME = (60 * 60 * 24); // 24 hours
	$MAX_SESSION_TIME = 3600; // 1 hour
	
*/

$MAX_SESSION_TIME = (60 * 60 * 24);


/***************** Stay Logged In  *****************
	STAY_LOGGED_IN overrides the previous setting 
	and allows users to be logged in indefinitely.
	
	If enabled (set to 1), $MAX_SESSION_TIME becomes
	the length of time a user can be inactive before
	getting logged out.
*/

$STAY_LOGGED_IN = 1;


/***************** POP3 QUOTA  *****************
	POP3 disk quotas must be hard codded, since
	POP3 does not provide the means to retreive
	a user's quota.
	
	VALUES ARE IN KILOBYTES
	
	Quotas are set for each host, even if there's 
	only one.
	
	e.g.
	
	$POP_QUOTA["losers.domain.com"] = 5000;
	$POP_QUOTA["winners.domain.com"] = 20000;
*/

//$POP_QUOTA["host"] = 0;


/***************** REFRESH INTERVALS  ***********
	Users can configure the radar (indicator in top 
	left frame) and folder list to reload periodically.
	
	Since a large installation with dozens or hundreds
	of users whose refresh is set to mere seconds 
	could cause excessive traffic, you can set the
	minimum intervals here.
	
	As reference, if there are 100 users logged in
	and they all have both refresh values set to 5
	seconds, they will generate 24 requests per minute
	each.  Collectively, they will generate 2400 page
	views per minute, on top of the usual more resource
	intensive calls.  Do the math and set the values
	as appropriate for your environment.
*/

$MIN_FOLDERLIST_REFRESH = 10;
$MIN_RADAR_REFRESH = 10;


/*****************  DISABLE ******************
	The following directives can be used to disable
	some of the "extra" features.  
*/

$DISABLE_CALENDAR = 0;
$DISABLE_BOOKMARKS = 0;


/***************** SHARING ********************
	Disallows users to create contacts,schedules that are 
	accessible by all (but can only be modified/deleted by 
	original user).
	
	These settings currently only disable checkboxes in the UI,
	which means it's technically possible to selectively enter
	shared entries (for example, if you want your support email
	address accessible, but don't want everybody to enter
	shared addresses).
*/

$DISABLE_CONTACTS_SHARING = 0;
$DISABLE_CALENDAR_SHARING = 0;
$DISABLE_BOOKMARKS_SHARING = 0;


/***************** CREATE_DEF_DIRS ************
	If this is directive is enabled AND default
	folder paths are specified in conf/defaults.*.inc
	any default folders that are missing will be created
	automatically upon login.

	Disabled by default because default paths aren't
	set.
*/
$CREATE_DEF_DIRS = 0;


/***************** MAX_UPLOAD_SIZE ***********
	Maximum attachment upload size.  If set to 0, it 
	matches the "upload_max_filesize" directive in php.ini.
	
	Value must be equal to or less than 
	"upload_max_filesize" and "post_max_size" directives 
	set in php.ini.
*/
$MAX_UPLOAD_SIZE = 0;


/***************** IMAP OPTIONS *************
	IMAP_USE_HEADER_DATE
	Set to true if you want the IMAP library to parse
	date headers found in messages.  The alternative is
	to use IMAP's INTERNALDATE which is faster, but may
	not be the same as dates specified in headers.
	
	IMAP_NO_CACHE
	Set to true if you don't want the IMAP library to
	cache results.  Turning the cache off will make
	things slower, but guarantees that there are no
	synchronization issues between the actual account
	state and IlohaMail's cache-state.
*/
$IMAP_USE_HEADER_DATE = false;
$IMAP_NO_CACHE = false;


/***************** ATTACHMENT FILTERS ********
	This directive allows you to allow/deny/warn
	users who try to download attachments with
	specific file name patterns.  For example, 
	you can prevent users from downloading files
	with extensions commonly used by viruses.
	
	The possible values are:
		"allow" : allow users to download
		"warn"  : show warning
		"deny"  : don't allow users to download
*/

//catch-all, by default, allow
$ATTFLTR_DEFAULT = 'allow';

//messages
$ATTFLTR_MESSAGES['deny'] = 'For security reasons, access to attachments of this type have been blocked.';
$ATTFLTR_MESSAGES['warn'] = 'Files of this type have been know to contain malicious content.';
$ATTFLTR_MESSAGES['warn'].= '  Do not download unless it was sent from a reliable source.';

//rules, array key contains regex
$ATTFLTR_RULES = array();

//Here are some examples.  Modify as you see fit.
//$ATTFLTR_RULES['\.exe$'] = 'deny';
//$ATTFLTR_RULES['\.cmd$'] = 'deny';
//$ATTFLTR_RULES['\.bat$'] = 'deny';
//$ATTFLTR_RULES['\.ws$']  = 'deny';
//$ATTFLTR_RULES['\.wsf$']  = 'deny';
//$ATTFLTR_RULES['\.wsc$']  = 'deny';
//$ATTFLTR_RULES['\.vb$']  = 'deny';
//$ATTFLTR_RULES['\.vbe$']  = 'deny';
//$ATTFLTR_RULES['\.vbs$']  = 'deny';
//$ATTFLTR_RULES['\.js$']  = 'deny';
//$ATTFLTR_RULES['\.scr$'] = 'deny';
//$ATTFLTR_RULES['\.pif$'] = 'deny';
//$ATTFLTR_RULES['\.msi$'] = 'deny';
//$ATTFLTR_RULES['\.hta$'] = 'deny';
//$ATTFLTR_RULES['\.cpl$'] = 'deny';
//$ATTFLTR_RULES['\.ini$'] = 'deny';
//$ATTFLTR_RULES['\.inf$'] = 'deny';
//$ATTFLTR_RULES['\.dll$'] = 'deny';

//$ATTFLTR_RULES['\.zip$'] = 'warn';

/*****************  SPELL CHECK **************
	IlohaMail uses aspell (http://aspell.net/) but
	does NOT use PHP's aspell/pspell module.  All you
	need is athe aspell executable, and appropriate
	dictionaries.
	
	UNCOMMENT TO ENABLE:
	
	$ASPELL_PATH = "path to aspell binary";
	
	specify supported languages with:
	$DICTIONARIES[<lang>] = <name>
*/

//path to aspell binary (Linux/Unix):
//$ASPELL_PATH = "/usr/bin/aspell";

//on Windows:
//$ASPELL_PATH = "D:\\Aspell\\bin\\aspell.exe";

//supported languages
//$DICTIONARIES["en"] = "English";


/*****************  IGNORE FOLDERS **********
	Specify regular expression rules of folders
	to ignore.  For example:
	
	$IGNORE_FOLDERS["mail.domain.com"] = "^Public Folders/";
	
	ignores all folders inside "Public Folders" including the
	folder itself for the host "mail.domain.com".
*/
//$IGNORE_FOLDERS[<host>] = <regexp>;



/*****************  GPG *********************
	---EXPERIMENTAL---
	A complete web interface for GPG.  Supports:
		-Key generation
		-Import public keys
		-Encrypt on send
		-Decrypt on receive
	This feature works regardless of whether the user
	has a shell account on the webmail server or not.
	---EXPERIMENTAL---
*/

$GPG_ENABLE=0;
$GPG_PATH = "/usr/bin/gpg";
$GPG_HOME_STR = "../data/gpg/%u.%h";
//$GPG_HOME_STR = "/home/%u";


/****   WARNING ***
	Make sure there are NO BLANK LINES after
	the '?>' below!!  Blank lines will cause
	all kinds of problems!!
*******************/
?>