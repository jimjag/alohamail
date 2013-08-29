<?php
/////////////////////////////////////////////////////////
//	
//	include/session_auth.FS.inc
//
//	(C)Copyright 2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail, and released under GPL.
//	See COPYING, or http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	PURPOSE:
		1.  Make sure session (pass as $user) is valid
		2.	Initialize $loginID variable, containing IMAP login ID.
		3.  Initialize $host variable, containing IMAP server name
		4.  Initialize $password variable, containing plain text password
		5.  Initialize $my_prefs variable, which should be an associated array containing user preferecnes
		6.  Initialize $my_colors variable, which should be an associated array containing user defined colors
	PRE-CONDITIONS:
		$user - Session ID
	POST-CONDITIONS:
	COMMENTS:
		All source files should include this file for session verification and user data initialization.
		As long as the variables and functions defined above are initialized, where the data is
		(i.e. on the HD, in a SQL database, LDAP, etc) should not matter.

********************************************************/
  function auth_bail($user){
		echo "<html>";
		echo "Invalid session ID: $user<br>\n";
		echo "Please <a href=\"index.php\" ".(ereg("index.php",$_SERVER['PHP_SELF'])?"":"target=\"_parent\"").">log back in</a>.";
		echo "</html>";
		exit;
  }
	
	//cehck session ID
	if ((empty($user)) || (!file_exists(realpath($SESSION_DIR.$user.".php")))){
		echo "Invalid session ID<br>\n";
		if (!$do_not_die) exit;
	}

	include_once("../include/encryption.inc");
	include_once($SESSION_DIR.$user.".php");

	$ipkey = GetSessionEncKey($user);
	$userPath = DecodeMessage($ipkey, $userPath);

	if (!file_exists(realpath($userPath."/key.php"))){
		auth_bail($user.' (key not found)');
	}else{
		//get key
		include($userPath."/key.php");
	}
	
	//read prefs and colors
	include_once("../include/data_store.inc");
	include_once("../conf/defaults.inc");
	$DS = new DataStore_obj($userPath);
	$my_prefs = $DS->read_or_die("prefs");
	if (count($my_prefs) != count($default_prefs)){
		//if $my_prefs missing fields, copy from default
		while(list($pref_key, $def_value)=each($default_prefs)){
			if (!array_key_exists($pref_key,$my_prefs )) $my_prefs[$pref_key] = $def_value;
		}
	}
	$my_colors = $DS->read_or_die("colors");
	if (count($my_colors) != count($default_colors)){
		//if $my_colors missing fields, copy from default
		while(list($pref_key, $def_value)=each($default_colors)){
			if (!array_key_exists($pref_key,$my_colors )) $my_colors[$pref_key] = $def_value;
		}
	}

	//initialize session variables
	$my_charset=$my_prefs["charset"];
	if ($port==110) $my_prefs["list_folders"] = 0;	
	$loginID = DecodeMessage($passkey, GetUserName());
	$password = DecodeMessage($passkey, GetPassword());
	$host = DecodeMessage($passkey, GetHost());
	$check_data = DecodeMessage($ipkey, $enc_check_data);
	$test_sum = EncCheckSum($check_data);
	if ($test_sum!=$check_sum) auth_bail($user.' (key mismatch)');
	$dataID = $userPath;
	$session_dataID = $dataID;
?>