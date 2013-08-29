<?php
/********************************************************
	lang/eng/prefs.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
//include("../lang/eng/prefs.inc");

$prefHeaderStrings[0] = "Preferences";
$prefHeaderStrings[1] = "General";
$prefHeaderStrings[3] = "Identities";
$prefHeaderStrings[2] = "Appearance";
$prefHeaderStrings[4] = "Filters";

											
$prefsStrings["0.0"] = "Default Identity";
$prefsStrings["0.1"] = "Name: ";
$prefsStrings["0.2"] = "Email: ";
$prefsStrings["0.3"] = "Changes can be made in the \"%s\" page.";
$prefsStrings["0.4"] = "Unspecified";
$prefsStrings["1.0"] = "International";
$prefsStrings["1.1"] = "Interface: ";
$prefsStrings["1.2"] = "Character Set: ";
$prefsStrings["1.3"] = "Time Zone: ";
$prefsStrings["1.4"] = "Time Option: ";
$prefsStrings["1.5"] = array(12=>"12 hour clock", 24=>"24 hour clock");
$prefsStrings["1.6"] = "Week start: ";
$prefsStrings["1.7"] = array(0=>"Sunday", 1=>"Monday");
$prefsStrings['1.8'] = 'Daylight Savings Time';
$prefsStrings["2.0"] = "List View Options";
$prefsStrings["2.1"] = "Show up to ";
$prefsStrings["2.2"] = " items";
$prefsStrings["2.3"] = "Show size";
$prefsStrings["2.4"] = "Sort by: ";
$prefsStrings["2.5"] = "";
$prefsStrings["2.6"] = "Sort order: ";
$prefsStrings["2.7"] = "";
$prefsStrings["2.8"] = "Show folder list";
$prefsStrings["2.9"] = "Folder list width";
$prefsStrings["2.10"] = "Check for new messages every %n seconds";
$prefsStrings["2.11"] = "Show tool bar at %m";
$prefsStrings["2.12"] = array("b"=>"bottom", "t"=>"top", "bt"=>"top and bottom");
$prefsStrings["2.13"] = "Rearrange columns: ";
$prefsStrings["2.14"] = "click here";
$prefsStrings["2.15"] = "Show filter/quick search toolbar";
$prefsStrings['2.16'] = 'Thread conversations together';
$prefsStrings["3.0"] = "Auto-Save Options";
$prefsStrings["3.1"] = "Save sent items.";
$prefsStrings["3.2"] = "Save sent messages in folder: ";
$prefsStrings["3.3"] = "";
$prefsStrings["3.4"] = "Automatically delete trash";
$prefsStrings["3.5"] = "Move deleted items to folder: ";
$prefsStrings["3.6"] = "";
$prefsStrings["3.7"] = "Root Directory: ";
$prefsStrings["3.8"] = "Other";
$prefsStrings["3.9"] = "Hide unsubscribed folders";
$prefsStrings["3.10"] = "Show quota %m";
$prefsStrings["3.11"] = array(""=>"don't show", "m"=>"in message listing", "f"=>"in folder list");
$prefsStrings['3.12'] = 'Drafts folder ';
$prefsStrings["4.0"] = "Message View Options";
$prefsStrings["4.1"] = "View in frame";
$prefsStrings["4.2"] = "Colorize quotes";
$prefsStrings["4.3"] = "Turning on this option may slow down the system.";
$prefsStrings["4.4"] = "Convert URLs to links";
$prefsStrings["4.5"] = "Show HTML messages";
$prefsStrings["4.6"] = "Show images inline";
$prefsStrings["4.7"] = "Show navigation links";
$prefsStrings["4.8"] = "Don't mark messages when traversing";
$prefsStrings['4.9'] = 'After deleting/moving, show: %menu';
$prefsStrings['4.10']= array('main'=>"Folder",'next'=>'Next','prev'=>'Previous');
$prefsStrings["5.0"] = "Signature";
$prefsStrings["5.1"] = "Show signature by default";
$prefsStrings["5.2"] = "";
$prefsStrings["6.0"] = "Compose";
$prefsStrings["6.1"] = "Show contacts";
$prefsStrings["6.2"] = "Show CC/BCC fields";
$prefsStrings["6.3"] = "Close after sending";
$prefsStrings["6.4"] = "Compose inside frame";
$prefsStrings["6.5"] = "Textbox Size: ";
$prefsStrings['6.6'] = 'BCC to self by default';
$prefsStrings["7.0.0"] = "Disk &amp; Folder Options";
$prefsStrings["7.0.1"] = "Quota Options";
$prefsStrings["7.1"] = "Show num unread";
$prefsStrings["7.2"] = "Refresh folder list every %n seconds";
$prefsStrings['8.0'] = 'User Interface';
$prefsStrings["8.1"] = 'Theme';
$prefsStrings["8.2"] = "JavaScript usage";
$prefsStrings["8.3"] = "Low";
$prefsStrings["8.4"] = "High";
$prefsStrings['8.5'] = 'Automatic';

$pref_col_title = "Rearrange Columns";
$pref_colstr["order"] = "Order";
$pref_colstr["field"] = "Field";
$pref_col_label['c'] = "Check box";
$pref_col_label['a'] = "Attachments";
$pref_col_label['m'] = "Flags/status";

$sort_fields=array('DATE'=>'Time Sent', 'SUBJECT'=>'Subject', 'SIZE'=>'Size');
$sort_fields['INTERNALDATE'] = 'Time Received';
$sort_orders=array("ASC"=>"Ascending", "DESC"=>"Descending");

$prefsButtonStrings = array( 0=> "Update", 1=>"Apply", 2=> "Cancel", 3=> "Defaults");

$prefs_new_user = "You seem to be logging in for the first time...welcome!<br>";
$prefs_new_user .= "Please go through the following settings, then press \"Apply\"";
?>
