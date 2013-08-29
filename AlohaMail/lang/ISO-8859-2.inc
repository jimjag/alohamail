<?php
/////////////////////////////
//	ISO-8859-2.inc
//	(C)2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//		
//		Description:
//				Encoding library for the ISO-8859-2 charset.
//		Author: Ch3mik
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
////////////////////////////

include_once("../include/qp_enc.inc");

function utf82iso88592($string) {

	$utf=array("\xC3\x81","\xC4\x8C","\xC4\x8E","\xC3\x89","\xC4\x9A","\xC3\x8D","\xC4\xBD",
	           "\xC5\x87","\xC3\x93","\xC5\x98","\xC5\xA0","\xC5\xA4","\xC3\x9A","\xC5\xAE",
    		   "\xC3\x9D","\xC5\xBD","\xC3\xA1","\xC4\x8D","\xC4\x8F","\xC3\xA9","\xC4\x9B",
	           "\xC3\xAD","\xC4\xBE","\xC5\x88","\xC3\xB3","\xC5\x99","\xC5\xA1","\xC5\xA5",
    		   "\xC3\xBA","\xC5\xAF","\xC3\xBD","\xC5\xBE","\xC3\x84","\xC3\x8B","\xC3\x96",
	           "\xC3\x9C","\xC3\xA4","\xC3\xAB","\xC3\xB6","\xC3\xBC","\xC3\xB4","\xC3\x94",
    		   "\xC4\xB9","\xC4\xBA","\xC4\xBD","\xC4\xBE","\xC5\x94","\xC5\x95","\xC2\xB1");

	$iso=array(chr(193),chr(200),chr(207),chr(201),chr(204),chr(205),chr(165),
	           chr(210),chr(211),chr(216),chr(169),chr(171),chr(218),chr(217),
		   chr(221),chr(174),chr(225),chr(232),chr(239),chr(233),chr(236),
		   chr(237),chr(181),chr(242),chr(243),chr(248),chr(185),chr(187),
		   chr(250),chr(249),chr(253),chr(190),chr(196),chr(203),chr(214),
		   chr(220),chr(228),chr(235),chr(246),chr(252),chr(244),chr(212),
		   chr(197),chr(229),chr(165),chr(181),chr(192),chr(224),"&plusmn;");

        return str_replace($utf, $iso, $string);

}

function win12502iso88592($string) {

	return strtr($string,"\x8A\x8D\x8E\x9A\x9D\x9E\xBC\xBE","\xA9\xAB\xAE\xB9\xBB\xBE\xA5\xB5");

}

function win12512iso88592($string) {
	
	$win1251=array("\xCA","\xEE","\xEC","\xEF","\xE0","\xED","\xE8","\xFF",
	               "\xCF","\xF0","\xE5","\xE4","\xEB","\xE3","\xF2","\xE2",
	               "\xC8","\xF4","\xF6","\xFB","\xC0","\xF5","\xCD","\xF8",
	               "\xCE","\xE6","\xE1","\xE7","\xAB","\xF1","\xEA","\xBB",
	               "\xC1","\xD2","\xD0","\xD4","\xC4","\xC7","\xE9","\xFC",
		       "\xD1","\xF3","\xF9","\xC3","\xC2","\xDD","\xF7","\xFE",
		       "\xCC","\xC5","\xD8","\xCB","\xDB","\xD3","\xDC","\x96",
		       "\xFD","\xD6");
	
	$iso=array("&#1050;","&#1086;","&#1084;","&#1087;","&#1072;","&#1085;","&#1080;","&#1103;",
	           "&#1055;","&#1088;","&#1077;","&#1076;","&#1083;","&#1075;","&#1090;","&#1074;",
	           "&#1048;","&#1092;","&#1094;","&#1099;","&#1040;","&#1093;","&#1053;","&#1096;",
	           "&#1054;","&#1078;","&#1073;","&#1079;","&#171;","&#1089;","&#1082;","&#187;",
		   "&#1041;","&#1058;","&#1056;","&#1060;","&#1044;","&#1047;","&#1081;","&#1100;",
		   "&#1057;","&#1091;","&#1096;","&#1043;","&#1042;","&#1047;","&#1095;","&#1102;",
		   "&#1052;","&#1045;","&#1064;","&#1051;","&#1067;","&#1059;","&#1068;","-",
		   "&#1101;","&#1062;");
	
	return str_replace($win1251, $iso, $string);

}

function LangIs8Bit($string){
	$len = strlen($string);
	for ($i=0; $i < $len; $i++)
		if (ord($string[$i])>=128) return true;
	
	return false;
}

function LangConvert($string, $charset, $charset2){
	if ( strtoupper($charset) != strtoupper($charset2) ) {
		if ( strtoupper($charset2) == "UTF-8" ) {
		        return utf82iso88592($string);
		} elseif ( strtoupper($charset2) == "WINDOWS-1250" ) {
		        return win12502iso88592($string);
		} elseif ( strtoupper($charset2) == "WINDOWS-1251" ) {
		        return win12512iso88592($string);
		} else {
			return $string;
		}
	}
	else return $string;
}

function LangEncodeSubject($input, $charset){
	$words = explode(" ", $input);
	if ( count($words) > 0){
		while ( list($k, $word) = each($words) ){
			if (LangIs8Bit($word)) $words[$k] = "=?".$charset."?Q?".qp_enc($word, 76)."?=";
		}
		$input = implode(" ", $words);
	}
	return $input;
}

function LangEncodeMessage($input, $charset){
	$message=$input;
	
	$result["type"]="Content-Type: text/plain; charset=".$charset."\r\n";
	$result["encoding"]="Content-Transfer-Encoding: quoted-printable\r\n";
	$result["data"] = qp_enc($message, 78);
		
	return $result;
}

include_once("../lang/common.inc");

function LangWrap($str){
	return wordwrap($str);
}
?>