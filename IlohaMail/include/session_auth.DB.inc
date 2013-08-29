<?php
/////////////////////////////////////////////////////////
//	
//	include/session_auth.DB.inc
//
//	(C)Copyright 2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
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
		This file uses a DB backend for session management.

********************************************************/

	include_once('../include/encryption.inc');
	include_once('../include/ryosimap.inc');
  

	function auth_bail($user){
		echo "<html>";
		echo "Invalid session ID: $user<br>\n";
		echo "Please <a href=\"index.php\" ".(ereg("index.php",$_SERVER['PHP_SELF'])?"":"target=\"_parent\"").">log back in</a>.";
		echo "</html>";
		exit;
  	}

    $my_prefs = false;
    $my_colors = false;
    
    $dataID = 0;

	//connect to database
	include_once("../conf/db_conf.php");
	include_once("../include/idba.$DB_TYPE.inc");
	$db = new idba_obj;
	if (!$db->connect()){
		echo 'DB connection failed<br>'."\n".$db->error();
		exit;
	}
	
	//load user ID from user name and host
	if (!$user && $login && $host){
		$sql = "select id from $DB_USERS_TABLE WHERE login like '$login' and host like '$host'";
		$result = $db->query($sql);
		if ($result && $db->num_rows($result)==1){
			$a = $db->fetch_row($result);
			$user = '_'.$a['id'];
		}
	}
	
	//load user & session info from session ID
	if ($user[0]=='_'){
		//we've got actual user ID, make sure this page is accessible by user ID
		if (!$current_page) $current_page = getCurrentPage();
		$permitted = array('css.php'=>1);
		$permitted['calendar.php'] = !$DISABLE_CALENDAR_SHARING;
		$permitted['bookmarks.php'] = !$DISABLE_BOOKMARKS_SHARING; 
		$permitted['news.php'] = $permitted['bookmarks.php'];
		if (!$permitted[$current_page]) auth_bail($user.' (no public access to '.$current_page.')');
		
		//extract id
		$uid = substr($user, 1);
		
		//get user data directly
		$sql = "select * from $DB_USERS_TABLE where id='".$uid."'";
		$result = $db->query($sql);
		if ((!$result) || ($db->num_rows($result)!=1)) auth_bail($uid.' (unknown user ID)');
		
		$a = $db->fetch_row($result);
		$session_dataID = $dataID = $uid;
		$loginID = $a['login'];
		$host = $a['host'];
		$userLevel = $a['userLevel'];
		
		//get JavaScript usage
		if (!isset($_REQUEST['js_enabled'])) $_REQUEST['js_enabled'] = true; //pretend like JavaScript is on
		include_once('../include/write_sinc.common.inc');
		$sess_js_mode = $js_mode = wsinc_getJSMode();
		
		$IS_PUBLIC = true;
	}else{
		//we should have a session ID
		$result = $db->query("select * from $DB_SESSIONS_TABLE where sid = '$user'");
		if ((!$result) || ($db->num_rows($result)!=1)) auth_bail($user.'(unknown sessionID)');
	
		//read in session data
		$a = $db->fetch_row($result);
		$encLogin = $a["login"];
		$encPass = $a["password"];
		$encHost = $a["host"];
		$userPath = $a["path"];
		$dataID = $a["dataID"];
		$port = $a["port"];
		$lastSend = $a["lastSend"];
		$numSent = $a["numSent"];
		$userLevel = $a["userLevel"];
		$inTime = $a["inTime"];
		$session_dataID = $dataID;
		$enc_check_data = $a["check_data"];
		$check_sum = $a["check_sum"];
		$sess_js_mode = $js_mode = $a['js_mode'];
				
		$ttl = time() - $inTime;
		if ($STAY_LOGGED_IN && ($MAX_SESSION_TIME/10)<$ttl){
			// if session time remaining is 10% of max session lifespan, update so we stay logged in
			$db->query("UPDATE $DB_SESSIONS_TABLE SET inTime=".time()." WHERE sid='$user'");
		}
            
    
    	//--------- END DB Specific stuff -----------

		$ipkey = GetSessionEncKey($user);
	
		$loginID = DecodeMessage($ipkey, $encLogin);
		$password = DecodeMessage($ipkey, $encPass);
		$host = DecodeMessage($ipkey, $encHost);
		$check_data = DecodeMessage($ipkey, $enc_check_data);
		$test_sum = EncCheckSum($check_data);
	
		if ($test_sum!=$check_sum){
			auth_bail($user);
		}
	}
	
	//read prefs and colors
	include_once("../include/data_store.inc");
	$DS = new DataStore_obj($dataID);
	include_once("../conf/defaults.inc");
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
    
  $my_charset=$my_prefs["charset"];
  
  if (($dataID==0)&&(!$do_not_die)){
  	auth_bail($user);
  }
?>