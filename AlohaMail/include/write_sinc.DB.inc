<?php
/////////////////////////////////////////////////////////
//	
//	include/write_sinc.DB.inc
//
//	(C)Copyright 2002-2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	PURPOSE:
		1.  Generate session ID
		2.  Initialize session
	PRE-CONDITIONS:
		$user_name - User name
		$host - IMAP server
	POST-CONDITIONS:
		$session - Session ID
        $new_user - true if new user, else false
		$error - error messages
		session info is stored in database
        prefs and colors stored in backend (DB or FS)
	NOTE:
        In conf/db_conf.php
        ...assumes the following tables are present:
            $DB_USERS_TABLE
            $DB_SESSIONS_TABLE
        ...following are optional:
            $DB_PREFS_TABLE
            $DB_COLORS_TABLE
        If $DB_PREFS_TABLE or $DB_COLORS_TABLE is empty, a file based backend is used.

********************************************************/
//--------------
//	INCLUDES
//--------------

	include_once('../include/array2sql.inc');
	include_once('../conf/db_conf.php');

//------------
//	FUNCTIONS
//------------
	function wsinc_addUser(&$db, $user_name, $host, $userLevel, &$error){
		global $DB_USERS_TABLE;
		
		$now = time();
		$sql = 'insert into '.$DB_USERS_TABLE.' (login, host, dateCreated, lastLogin, userLevel) ';
		$sql.= "values ('$user_name', '$host', '$now', '$now', 0)";
		if (!$db->query($sql)){
			$error.="DB error: Couldn't add user to users table<br>\n";
			echo "\n<!--\nSQL:$sql\nERROR:".$db->error()."\n//-->\n";
			return false;
		}else{
			return $db->insert_id();
		}
	}
	
	function wsinc_initSessionData($session, $path, $a, &$error){
		global $db, $DB_USERS_TABLE, $DB_TYPE, $DB_SESSIONS_TABLE;
		
		// generate random session key
		$key=GenerateMessage(strlen($password)+5);
		$ipkey = InitSessionEncKey($session);        
		$unenc_check_data = GenerateRandomString(strlen($ipkey), "123456789");

		// encrypt login ID, host, and passwords
		$encpass = EncryptMessage($ipkey, $a['pass']);
		$encHost = EncryptMessage($ipkey, $a['host']);
		$encUser = EncryptMessage($ipkey, $a['user']);
		$check_sum = EncCheckSum($unenc_check_data);
		$check_data = EncryptMessage($ipkey, $unenc_check_data);
		$encPath = EncryptMessage($ipkey, $path);
		$js_mode = wsinc_getJSMode();
		$userLevel = 0;
		$port = $a['port'];
	
		//connect to database
		include_once("../include/idba.$DB_TYPE.inc");
		$db = new idba_obj;
		if (!$db->connect()){
			$error.='DB error: Connect failed ('.$db->error().")<br>\n";
			return false;
		}
		
		// check users table, create entry if necessary
		$sql = "select id,userLevel from $DB_USERS_TABLE where (login='".$a['user']."') and (host='".$a['host']."')";
		$r = $db->query($sql);
		if (!$r){
			$error.='DB error: Couldn\'t access users table ('.$db->error().")<br>\n";
			return false;
		}
		
		if ($db->num_rows($r)<1){
			// if user not in db, insert
			$dataID = wsinc_addUser($db, $a['user'], $a['host'], $userLevel, $error);
			if (!$dataID) return false;
			$new_user = true;
		}else{
			// existing user, read info
			$dataID = $db->result($r, 0, "id");
			$userLevel = $db->result($r, 0, "userLevel");
		}
			
		// Initialize session
		if (empty($port)) $port = 143;
		$sql = 'insert into '.$DB_SESSIONS_TABLE;
		$sql.= '(sid, login, password, host, path, dataID, port, userLevel, inTime, check_data, check_sum, js_mode)';
		$sql.= " values ('$session', '$encUser', '$encpass', '$encHost', '$encPath', '$dataID', '$port', '$userLevel', ".time().",'$check_data','$check_sum', '$js_mode')";
		if (!$db->query($sql)){
			$error .= "DB Insert failed: ".$db->error()." <br>\n";
			return false;
		}
		
		// Update lastLogin
		$sql = "update $DB_USERS_TABLE set lastLogin='".time()."' where id='$dataID'";
		if (!$db->query($sql)){
			$error .= "DB Update failed: ".$db->error()." <br>\n";
			return false;
		}
		
		return array($dataID, $new_user);
	}


	function wsinc(&$user_name, &$host, $password, &$port, &$new_user, &$error){
		global $init;
		
		// check port, default to IMAP
		if (empty($port)) $port = 143;
		
		// normalize case
		$user_name = strtolower($user_name);
		$host = strtolower($host);

		// initialize some vars
		$new_user = false;

		//initialize FS backend location, just in case
		$path = fs_get_path("user", $user_name, $host);

		// create session ID
		$session=CreateSessionID();
    
		// package auth data    
		$auth_a = array('pass'=>$password,'host'=>$host,'user'=>$user_name,'port'=>$port);

		// initialize session files
		$result = wsinc_initSessionData($session, $path, $auth_a, $error);		
		if ($result) list($dataID, $new_user) = $result;
		else return false;

		//initialize prefs and colors
		$ds = new DataStore_obj($dataID);
		if (!$ds->init("prefs", $init["my_prefs"])) $error = $ds->error;
		if (!$ds->init("colors", $init["my_colors"])) $error = $ds->error;
		if ($error) return false;
		
		//backend agnostic code
		wsinc_common($user_name, $host, $error);
		if ($error) return false;

		return $session;
	}
	
?>