<?php
/////////////////////////////////////////////////////////
//	
//	include/header_main.inc
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
		Provide header and initialization functionality common to many pages.
		Including:
			1.  Display HTML header	
					(including BODY tag with colors set according to user prefs)
			2.  Read user preferences into $my_prefs and $my_colors arrays
	PRE-CONDITIONS:
		$user - Session ID
	COMMENTS:
		Reading preferences into array is actually done in "include/session_auth.inc"

********************************************************/
include_once("../include/nocache.inc");
include_once("../conf/conf.php");
include_once("../conf/db_conf.php");
include_once("../include/ryosimap.inc");

//disable time limit
if(!ini_get("safe_mode")){
	@set_time_limit($MAX_EXEC_TIME);
}

//get current page
$current_page = getCurrentPage();

//setup code for onLoad and onUnload
$onUnLoad = "";
if (strpos($current_page, "compose2.php")!==false) {
	$onUnLoad = " onUnload=\"close_popup();\"";
}
$onLoad = "";
if (strpos($current_page, "contacts_popup.php")!==false) {
	$onLoad = " onLoad=\"acknowledge_popup();\"";
	$onUnLoad = " onUnLoad=\"alert_close();\"";
}

//continue only if valid session ID
if (isset($user)|| ($login&&$host)){
	$sid=$user;

	include("../include/session_auth.inc");
	include("../lang/".$my_prefs["charset"].".inc");

	echo '<HTML '.($lang_rtl[$my_prefs['lang']]?'dir="rtl"':'').">\n";
	echo '<HEAD>'."\n";
	echo "<!--\nIlohaMail by Ryo Chijiiwa\nhttp://ilohamail.org\n//-->\n";
	
	//override charset if necessary 
	if ($override_charset) $my_prefs["charset"] = $override_charset;

	//determine URL for CSS	
	include_once('themes/'.$my_prefs['theme'].'/info.inc');
	$css_url = getCssURL($user, $current_page);

	echo '<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset='.$my_prefs['charset'].'">'."\n";
	echo '<link rel="stylesheet" href="'.$css_url.'" type="text/css">'."\n";

	//include javascript
	include("../include/javascript.inc");

	echo '</HEAD>'."\n";
 	echo '<BODY '.$onLoad.$onUnLoad.'>';
}else{
	echo '<html><BODY>';
	echo "User unspecified:".$user;
	echo '<!-- '."\n";
	print_r($_REQUEST);
	echo '//-->'."\n";
	echo '</HTML></BODY>';
	exit;
}
flush();
?>