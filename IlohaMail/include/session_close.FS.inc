<?php
/////////////////////////////////////////////////////////
//	
//	include/session_close.FS.inc
//
//	(C)Copyright 2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail, and released under GPL.
//	See COPYING, or http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////
/********************************************************

	PURPOSE:
	PRE-CONDITIONS:
		This page is include'd in "source/login.php" which is loaded upon logout.
		Session should be closed, and any cached data (i.e. password) purged.
	COMMENTS:
		This file is for the default file-based back-end.  For other backends, 
		modify as necessary.

********************************************************/


include_once('../conf/conf.php');
include_once('../include/encryption.inc');

function sess_close($user){
	global $SESSION_DIR;
	
	unlink($SESSION_DIR.$user.".php");
	RemoveSessionEncKey($user);
}

?>