<?php
/********************************************************
	lang/de/defaultFolders.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/defaultFolders.inc");

	$sentbox = $my_prefs['sent_box_name'];
	$trashname = $my_prefs['trash_name'];
	$draftbox = $my_prefs['draft_box_name'];
	$defaults["INBOX"] = "Posteingang";
	$defaults[$sentbox] = "Gesendet";
	$defaults[$trashname] = "Papierkorb";
	$defaults[$draftbox] = "Entw&uuml;rfe";

?>