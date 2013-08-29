<?php
include("../lang/eng/folders.inc"); // <-- do not delete
/********************************************************
	lang/pl/folders.inc
	(C)2002 Marcin Piliszczuk <marti@net26.pl>
	(C)2004,2005 Cezary Jackiewicz <cjackiewicz@poczta.onet.pl>
	
	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

	$fl_str["folders"] = "Foldery";
	$fl_str["manage"] = "Zarz±dzanie folderami";
	
	$sentbox=$my_prefs["sent_box_name"];
	$trashname=$my_prefs["trash_name"];
	$draftbox=$my_prefs["draft_box_name"];
	$defaults["INBOX"] = "Skrzynka odbiorcza";
	$defaults[$sentbox] = "Wys³ane";
	$defaults[$trashname] = "Kosz";
	$defaults[$draftbox] = "Kopie robocze";
	$fstr["reload"] = "reload";
	$fstr["expunge"] = "expunge";

?>
