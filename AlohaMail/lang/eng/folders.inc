<?php
/********************************************************
	lang/eng/folders.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

	//TRANSLATORS:  Uncomment next line for all non-English languages!!
	//include("../lang/eng/folders.inc");

	$fl_str['folders'] = 'Folders';
	$fl_str['manage'] = 'Manage Folders';
	
	$sentbox=$my_prefs['sent_box_name'];
	$trashname=$my_prefs['trash_name'];
	$draftbox=$my_prefs['draft_box_name'];
	$defaults['INBOX'] = 'Inbox';
	$defaults[$sentbox] = 'Sent';
	$defaults[$trashname] = 'Trash';
	$defaults[$draftbox] = 'Drafts';
	$fstr['reload'] = 'reload';
	$fstr['expunge'] = 'expunge';

?>