<?php
include("../lang/eng/defaultFolders.inc"); // <-- do not delete

/********************************************************
	lang/jp/defaultFolders.inc
	(C) 1999-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

	PURPOSE:
		Initialize default folder names.

********************************************************/

	$sentbox=$my_prefs["sent_box_name"];
	$trashbox=$my_prefs["trash_name"];
	$draftbox=$my_prefs['draft_box_name'];
	$defaults=array(
		"INBOX"=>"¼õ¿®È¢",
		$sentbox=>"Á÷¿®ºÑ¤ß",
		$draftbox=>"²¼½ñ¤­",
		$trashbox=>"¥´¥ßÈ¢"
 	);
?>