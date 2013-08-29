<?php
include("../lang/eng/search.inc"); // <-- do not delete
/********************************************************
	lang/eng/search.inc
	(C)2004 Cezary Jackiewicz <cjackiewicz@poczta.onet.pl>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

	//TRANSLATORS:
	//in the follow arrays, translate key, not value
	//i.e.  translate "From" not "FROM".
	$search_fields=array("select"=>"-", "Od"=>"FROM", "Do"=>"TO", "Temat"=>"SUBJECT","Tre¶æ"=>"BODY");
	$search_dates=array("nie ma znaczenia"=>"ignore", "is on"=>"ON", "jest po"=>"SINCE", "jest przed"=>"BEFORE");

	$search_str["title"] = "Szukaj";
	$search_str["str"] = "Szukaj w folderze %folders <br>\n";
	$search_str["str"] .= "gdy pole %fields zawiera %value <br>\n";
	$search_str["str"] .= "i data %dateops %m/%d/%y <br>\n";
	$search_str["submit"] = "szukaj";
?>
