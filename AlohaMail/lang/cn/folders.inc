<?php
include("../lang/eng/folders.inc"); // <-- do not delete
/********************************************************
	lang/tw/folders.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
	Simplified Chinese (GB2312), for use in China, 
	Singapore & Malaysia Chinese communities
	Translator: Leon Wang <leonwang@g.yi.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

	$fl_str["folders"] = "文件夹";
	$fl_str["manage"] = "管理";

	$sentbox=$my_prefs["sent_box_name"];
	$trashname=$my_prefs["trash_name"];
	$defaults["INBOX"] = "收件箱";
	$defaults[$sentbox] = "发件箱";
	$defaults[$trashname] = "回收站";

?>
