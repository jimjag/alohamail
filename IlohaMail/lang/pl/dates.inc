<?php
include("../lang/eng/dates.inc"); // <-- do not delete
/********************************************************
	lang/pl/dates.inc
	(C) 2002 Marcin Piliszczuk <marti@net26.pl>
	(C)2004 Cezary Jackiewicz <cjackiewicz@poczta.onet.pl>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

$lang_months[1] = "Styczeñ";
$lang_months[2] = "Luty";
$lang_months[3] = "Marzec";
$lang_months[4] = "Kwiecieñ";
$lang_months[5] = "Maj";
$lang_months[6] = "Czerwiec";
$lang_months[7] = "Lipiec";
$lang_months[8] = "Sierpieñ";
$lang_months[9] = "Wrzesieñ";
$lang_months[10] = "Pa¼dziernik";
$lang_months[11] = "Listopad";
$lang_months[12] = "Grudzieñ";

$lang_datetime["short_mon"][1] = "Sty";
$lang_datetime["short_mon"][2] = "Lut";
$lang_datetime["short_mon"][3] = "Mar";
$lang_datetime["short_mon"][4] = "Kwi";
$lang_datetime["short_mon"][5] = "Maj";
$lang_datetime["short_mon"][6] = "Cze";
$lang_datetime["short_mon"][7] = "Lip";
$lang_datetime["short_mon"][8] = "Sie";
$lang_datetime["short_mon"][9] = "Wrz";
$lang_datetime["short_mon"][10] = "Pa¼";
$lang_datetime["short_mon"][11] = "Lis";
$lang_datetime["short_mon"][12] = "Gru";

$lang_datetime["dsow"][0] = "Nie";
$lang_datetime["dsow"][1] = "Pon";
$lang_datetime["dsow"][2] = "Wto";
$lang_datetime["dsow"][3] = "¦ro";
$lang_datetime["dsow"][4] = "Czw";
$lang_datetime["dsow"][5] = "Pi±";
$lang_datetime["dsow"][6] = "Sob";

$lang_datetime["dsowl"][0] = "Niedziela";
$lang_datetime["dsowl"][1] = "Poniedzia³ek";
$lang_datetime["dsowl"][2] = "Wtorek";
$lang_datetime["dsowl"][3] = "¦roda";
$lang_datetime["dsowl"][4] = "Czwartek";
$lang_datetime["dsowl"][5] = "Pi±tek";
$lang_datetime["dsowl"][6] = "Sobota";
			
$lang_datetime["dsow_short"] = array("N", "P", "W", "¦", "C", "Pi", "S");
$lang_datetime["today"] = "Dzisiaj %t";		//e.g. "Today 12:02"
$lang_datetime["lastweek"] = "%w %t";		//e.g. "Wed 12:00"
$lang_datetime["thisyear"] = "%m/%d";		//e.g. "2/13"
$lang_datetime["prevyears"] = "%m/%d/%y"; 	//e.g. "1/1/2100"
$lang_datetime["monthyear"] = "%m %y";		//e.g. "January 2002"
$lang_datetime["verbal"] = "%m %d, %y";  	//e.g. "Jan 1, 1900"
$lang_datetime["verbal_short"] = "%m %d";  	//e.g. "Jan 1"
$lang_datetime["hour_system"] = 24;			// 12 or 24
$lang_datetime["ampm"]["am"] = "am";
$lang_datetime["ampm"]["pm"] = "pm";
$lang_datetime["time_format"] = "%h:%m%a";	// %h=hour, %m=minutes, %a="am:pm"
$lang_datetime["hour_format"] = "%h%a";
?>