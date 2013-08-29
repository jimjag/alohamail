<?php
/////////////////////////////////////////////////////////
//	
//	source/index.php
//
//	(C)Copyright 2000-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE: source/index.php
	PURPOSE:
		1. Provides interface for logging in.
		2. Authenticates user/password for given IMAP server
		3. Initiates session
	PRE-CONDITIONS:
		$user - IMAP account name
		$password - IMAP account password
		$host - IMAP server
	COMMENTS:
		Modify form tags for "host" as required.
		For an example, if you only want the program to be used to log into specific 
		servers, you can use "select" lists (if multiple), or "hidden" (if single) tags.
		Alternatively, simply show a text box to have the user specify the server.
********************************************************/
include("../include/super2global.inc");
include("../include/nocache.inc");

include_once("../include/encryption.inc");
include_once("../include/version.inc");
include_once("../include/langs.inc");
include_once("../conf/conf.php");
include_once("../conf/login.inc");
include_once("../include/stopwatch.inc");

$sw = new stopwatch;
$sw->register("start");

//set content type header
if (!empty($int_lang)){
	include_once("../lang/".$int_lang."init.inc");
}else{
	include_once("../lang/".$default_lang."init.inc");
}
header("Content-type: text/html; charset=".$lang_charset);

$authenticated = false;

// session not started yet
if (!isset($session) || (empty($session))){	
    //figure out lang
    if (strlen($int_lang)>2){
        //lang selected from menu (probably)
        $lang = $int_lang;
    }else{
        //default, or non-selection
        $lang = (isset($default_lang)?$default_lang:"eng/");
    }
    include_once("../conf/defaults.inc");

	//attempt to initialize session
	if (isset($host) && !empty($user) && !empty($password)){
		include("../include/do_login.inc");
	}
}


// valid session, show framset
$login_success = false;
if ((isset($session)) && ($session != "")){
	include("../include/do_post_login.inc");
}

//couldn't log in, show login form
if (!$login_success){
	//check for cookie...
	if ($_COOKIE["ILOHAMAIL_SESSION"]){
		$user = "";
		setcookie("ILOHAMAIL_SESSION", "");
	}

	//put together lang options
	$langOptions="<option value=\"--\">--";
	while (list($key, $val)=each($languages)) 
		$langOptions.="<option value=\"$key\" ".(strcmp($key,$int_lang)==0?"SELECTED":"").">$val\n";

	//colors...
	$bgcolor = $default_colors["folder_bg"];
	$textColorOut = $default_colors["folder_link"];
	$bgcolorIn = $default_colors["tool_bg"];
	$textColorIn = $default_colors["tool_link"];
	
	//load lang file
	if (!empty($int_lang)){
		include("../lang/".$int_lang."login.inc");
	}else{
		include("../lang/".$default_lang."login.inc");
	}
	
	//set a test cookie
	if ($USE_COOKIES){
		setcookie ("IMAIL_TEST_COOKIE", "test", time()+$MAX_SESSION_TIME, "/", $_SERVER[SERVER_NAME]);
	}
	
	//print document head
	include("../conf/HTML_head.inc");
	
	echo "\n<!-- \nSESS_KEY: $IMAIL_SESS_KEY $MAX_SESSION_TIME ".$_SERVER[SERVER_NAME]."\nOLD: $OLD_SESS_KEY\n //-->\n";
	include("../conf/HTML_login_top.inc");

	//start table of form fields
	?>
	<form method="POST" action="index.php">
	<script>
		document.write("<input type=\"hidden\" name=\"js_enabled\" value=\"1\">\n");
	</script>
	    <input type="hidden" name="logout" value=0>
        	<table width="280" border="0" cellspacing="0" cellpadding="0" bgcolor="<?php echo $bgcolorIn?>">
	<?php
        if (!empty($error)) echo "<font color=\"#FFAAAA\">".$error."</font><br>";
	?>
	<tr><td align=right><?php echo $loginStrings[0] ?>:</td><td><input type="text" name="user" value="<?php echo $user; ?>" size=15></td></tr>
	<tr><td align=right><?php echo $loginStrings[1] ?>: </td><td><input type="password" name="password" value="" size=15 AUTOCOMPLETE="off"></td></tr>
	<?php
		$HTTP_HOST = strtolower($_SERVER["HTTP_HOST"]);
		if (is_array($VDOMAIN_DETECT) && empty($host)){
			$host = $VDOMAIN_DETECT[$HTTP_HOST];
		}
		//empty default host
			//show text box
		//default host is array
			//hide
			//show list (select $host)
		//default host is string
			//show host
			//don't show host
		if (empty($default_host)){
			echo "<tr><td align=right>".$loginStrings[2].": </td><td><input type=text name=\"host\" value=\"$host\">&nbsp;&nbsp;</td></tr>";			
		}else if (is_array($default_host)){
			if ($hide_host && !empty($default_host[$host])){
				echo "<input type=hidden name=\"host\" value=\"$default_host\">";
			}else{
				echo  "<tr><td align=right>".$loginStrings[2].":</td><td><select name=\"host\">\n";
				reset($default_host);
				while ( list($server, $name) = each($default_host) ){
					echo "<option value=\"$server\" ".($server==$host?"SELECTED":"").">$name\n";
				}
				echo "</select></td></tr>";		
			}			
		}else{
			echo "<input type=hidden name=\"host\" value=\"$default_host\">";
			if (!$hide_host){
				echo  "<tr><td align=right>".$loginStrings[2].": </td><td><b>$host</b>&nbsp;&nbsp;";
				echo "</td></tr>";
			}
		}
			
		//initialize default rootdir and port 
		if ((!isset($rootdir))||(empty($rootdir))) $rootdir = $default_rootdir;
		if ((!isset($port))||(empty($port))) $port = $default_port;
		
		//show (or hide) protocol selection
		if (!$hide_protocol){
			echo "<tr>";
			echo "<td align=\"right\">".$loginStrings[3].": </td>\n<td>";
            echo "<select name=\"port\">\n";
            echo "<option value=\"143\" ".($port==143?"SELECTED":"").">IMAP\n";
            if ($SSL_ENABLED) echo "<option value=\"993\" ".($port==993?"SELECTED":"").">IMAP-SSL\n";
            echo "<option value=\"110\" ".($port==110?"SELECTED":"").">POP3\n";
            if ($SSL_ENABLED) echo "<option value=\"995\" ".($port==995?"SELECTED":"").">POP-SSL\n";
            echo "</select>\n";
			//echo "<td><input type=\"text\" name=\"port\" value=\"$port\" size=\"4\"></td>";
			echo "</td></tr>\n";
		}else{
			echo "<input type=\"hidden\" name=\"port\" value=\"$default_port\">\n";
		}
		
		//show (or hide) root dir box
		if (!$hide_rootdir){
			echo "<tr>";
			echo "<td align=\"right\">".$loginStrings[4].":</td>";
			echo "<td><input type=\"text\" name=\"rootdir\" value=\"$rootdir\" size=\"12\"></td>";
			echo "</tr>\n";
		}else{
			echo "<input type=\"hidden\" name=\"rootdir\" value=\"$default_rootdir\">\n";
		}
		
		if (!$hide_lang){
			echo "<tr><td align=right>".$loginStrings[5].": </td><td>\n";
   		 	echo "<select name=\"int_lang\">\n";
			echo $langOptions;
			echo "</select></td></tr>\n";
		}
	?>
		<tr><td align="right" colspan="2">
			<input type=submit value="<?php echo $loginStrings[6] ?>">&nbsp;&nbsp;<p> 
		</td></tr>
	</table>
	</form>
	<?php
	include("../conf/HTML_login_bottom.inc");
	
}
$sw->register("stop");
echo "<!--\n";
$sw->dump();
echo "//-->\n";
	?>
</HTML>
