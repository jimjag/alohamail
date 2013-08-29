<?php
/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE: include/langs.inc
	PURPOSE:
		Provide centralized location for keeping track of 
		available languages and character sets

********************************************************/

$languages=array(
	"eng/"=>"English (US)",
	"eng_GB/"=>"English (GB)",
	"ar/"=>"Arabic",
	"bg/"=>"Bulgarian",
	"cat/"=>"Catalan",
	"tw/"=>"Chinese (BIG5)",
	"cn/"=>"Chinese(GB2312)",
	"cz/"=>"Czech",
	"dk/"=>"Dansk",
 	"de/"=>"Deutsch",
 	"es/"=>"Espa&ntilde;ol",
	"fr/"=>"Fran&ccedil;ais", 
	"ga/"=>"Galician",
	'el/'=>"Greek",
	"hu/"=>"Hungarian",
	"it/"=>"Italiano",
	"jp/"=>"Japanese", 
	"kr/"=>"Korean",
	"lt/"=>"Lithuanian",
	"nl/"=>"Nederlands",
	"no/"=>"Norsk (bokm&aring;l)",
	"nn_NO/"=>"Norsk (nynorsk)",
	"fa/"=>"Persian",
	"pl/"=>"Polski",
	"pt/"=>"Portuguese (Standard)",
	"br/"=>"Portuguese (Brazilian)",
	"ro/"=>"Romanian",
	"ru/"=>"Russian",
	"fi/"=>"Suomi",
	"se/"=>"Svenska",
	"si/"=>"Slovenian",
	"tr/"=>"Turkish",
	"vn/"=>"Vietnamese"
);

$charsets=array(
	"US-ASCII"=>"ASCII (English)", 
	"X-EUC-JP"=>"EUC-JP (Japanese)", 
	"EUC-KR"=>"EUC-KR (Korean)",
	"ISO-8859-1"=>"ISO-8859-1 (Latin-1)",
	"ISO-8859-2"=>"ISO-8895-2",
	'ISO-8859-7'=>'ISO-8859-7',
	"ISO-8859-9"=>"ISO-8895-9",
	'ISO-8859-15'=>'ISO-8859-15',
	"BIG5"=>"BIG5 (Chinese)",
	"GB2312"=>"GB2312 (Chinese)",
	"UTF-8" => "UTF-8",
	"Windows-1251" => "Windows-1251 (Cyrillic)",
	"Windows-1252" => "Windows-1252 (Western)",
	"Windows-1255" => "Windows-1255 (Hebrew)",
	"Windows-1256" => "Windows-1256 (Arabic)",
	"Windows-1257" => "Windows-1257 (Baltic)"
);

$lang_rtl=array('ar/'=>1,'fa/'=>1);
?>