<?php
/////////////////////////////////////////////////////////
//	
//	include/write_sinc.inc
//
//	(C)Copyright 2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	PURPOSE:
		1.  Generate session ID
		2.	Read user preference settings from data source, and write into session file.
		3. Initialize session
	PRE-CONDITIONS:
		$user_name - User name
		$password - Account password
		$host - Mail server
		[$port] - Port (e.g. 110,143)
	POST-CONDITIONS:
		$session - Session ID
		$error - Error messages
        $new_user - true if new user, else false
		Session file (in the form of a PHP include) is written into sessions folder,
		with file name $user."inc".

********************************************************/

//------------
//	FUNCTIONS
//------------

	function wsinc_writeKey($path, $key, &$error){
		$fp=fopen($path."/key.php", 'w');
	    if (!$fp){
			$error.='Failed to open '.$path.'/key.php  for writing.<br>';
			return false;
		}
		fputs($fp, '<?php $passkey="'.$key.'"; ?>');
		fclose($fp);
	
		return true;
	}


	function wsinc_writeSession($path, $session, $key, $ipkey, $a, &$error){
		global $SESSION_DIR;

		$unenc_check_data = GenerateRandomString(strlen($ipkey), "123456789");

		// encrypt login ID, host, and passwords
		$encpass = EncryptMessage($key, $a['pass']);
		$encHost = EncryptMessage($key, $a['host']);
		$encUser = EncryptMessage($key, $a['user']);    
		$encPath = EncryptMessage($ipkey, $path);	
		$check_sum = EncCheckSum($unenc_check_data);
		$check_data = EncryptMessage($ipkey, $unenc_check_data);
		$js_mode = wsinc_getJSMode();

		// dynamically generate code to put in session include file.
		$string="<?php\n";
		$string.='function GetPassword(){ return "'.$encpass."\";}\n";
		$string.='function GetHost(){ return "'.$encHost."\"; }\n";
		$string.='function GetUserName(){ return "'.$encUser."\";}\n";
		$string.='$userPath="'.$encPath."\";\n";
		$string.='$port='.ereg_replace('[^0-9]','',$a['port']).";\n";
		$string.='$js_mode="'.$js_mode.'";'."\n";
		$string.='$enc_check_data="'.$check_data.'";'."\n";
		$string.='$check_sum="'.$check_sum.'";'."\n";
		$string.="\n?>";
    
		// write code to session include file (in sessions folder)
		$session_file_path = $SESSION_DIR.$session.".php";
		fs_mkdir($SESSION_DIR);
		$fp=fopen($session_file_path, 'w');
		if ($fp){
			if (!fputs($fp,$string))
				$error.= "Failed to write to \"$session_file_path\"\n";
			fclose($fp);
		}else{
			$error .= "Failed to open \"$session_file_path\"\n";
			echo "filesystem error";
		}
	}


	function wsinc_initSessionFiles($session, $path, $a, &$error){
		// generate random session key
		$key=GenerateMessage(strlen($password)+5);
		$ipkey = InitSessionEncKey($session);        
    
		// save session key in $userPath/key.php
		wsinc_writeKey($path, $key, $error);
	
		// write session file
		wsinc_writeSession($path, $session, $key, $ipkey, $a, $error);
	}
	
	
	function wsinc_getPath($user_name, $host, &$created){
    	// get user dir path
		$path = fs_get_path("user", $user_name, $host, false);
	
		// if path doesn't exist, create it
		if ($path && !fs_exists($path)){
			$path = fs_get_path("user", $user_name, $host, true);
			$created = true;
		}else $created = false;
		
		return $path;
	}
	
	
	function wsinc(&$user_name, &$host, $password, &$port, &$new_user, &$error){
		global $init;
		
		// check port, default to IMAP
		if (empty($port)) $port = 143;
		
		// normalize case
		$user_name = strtolower($user_name);
		$host = strtolower($host);
		
		// get path to user's main folder
		$path = wsinc_getPath($user_name, $host, $new_user);
    	if (!$path){
	        $error .= "Couldn't create user dir: $path<br>\n";
			return false;
		}
		
        // create session ID
        $session=CreateSessionID();

		// package auth data    
		$auth_a = array('pass'=>$password,'host'=>$host,'user'=>$user_name,'port'=>$port);

		// initialize session files
		wsinc_initSessionFiles($session, $path, $auth_a, $error);		
		if ($error) return false;

		//initialize prefs and colors
		$ds = new DataStore_obj($path);
		if (!$ds->init("prefs", $init["my_prefs"])) $error.= $ds->error;
		if (!$ds->init("colors", $init["my_colors"])) $error.= $ds->error;
		if ($error) return false;
		
		//backend agnostic code
		wsinc_common($user_name, $host, $error);
		if ($error) return false;
		
		return $session;
	}
?>
