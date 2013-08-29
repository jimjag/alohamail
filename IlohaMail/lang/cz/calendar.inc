<?php
/********************************************************
	lang/<lang>/calendar.inc
	(C)2003 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/calendar.inc");

$calStr["add_schedule"] = "Pøidat plán";
$calStr["edit_schedule"] = "Upravit plán";
$calStr["weekof"] = "Týden: %d";
$calStr["title"] = "Název:";
$calStr["color"] = "Barva:";

//for $calStr['color_names'], translate strings
//on the right hand side of equal sign
$calStr["color_names"]['Dark Red'] = 'Tmavì èervená';
$calStr["color_names"]['Red'] = 'Èervená';
$calStr["color_names"]['Deep Blue'] = 'Tmavì modrá';
$calStr["color_names"]['Blue'] = 'Modrá';
$calStr["color_names"]['Dark Green'] = 'Tmavì zelená';
$calStr["color_names"]['Green'] = 'Zelená';
$calStr["color_names"]['Purple'] = 'Fialová';
$calStr["color_names"]['Cyan'] = 'Azurová';
$calStr["color_names"]['Orange'] = 'Oran¾ová';
$calStr["color_names"]['Yellow'] = '®lutá';
$calStr["color_names"]['Magenta'] = 'Purpurová';
$calStr["color_names"]['No Color'] = 'Bez barvy';

$calStr["starts"] = "Zaèíná:";
$calStr["ends"] = "Konèí:";
$calStr["from"] = "od:";
$calStr["until"] = "do:";
$calStr["repeat_on"] = "Opakovat";
$calStr["of"] = "ka¾dý";
$calStr["weeks"] = array(1=>"první týden", 2=>"druhý týden", 3=>"tøetí týden", 4=>"ètvrtý týden"); 
$calStr["week_blurb"] = "...ka¾dý mìsíc (do data ukonèení).<br>Mù¾e se opakovat ka¾dý týden pokud není ¾ádný vybrán.";
$calStr["monthly"] = "Opakovat ka¾dý mìsíc";
$calStr["yearly"] = "Opakovat ka¾dý rok";
$calStr["place"] = "Místo:";
$calStr["description"] = "Popis:";
$calStr["add"] = "Pøidat";
$calStr["update"] = "Aktualizovat";
$calStr["Delete"] = "Smazat";
$calStr["go"] = "Zobraz";
$calStr["disabled"] = "Funkce kalendáøe byla zakázána.";
$calStr["public"] = "Oznaèit záznam jako veøejný";
$calStr["publish"] = "Zveøejnit záznam";
$calStr['name'] = '%name\'s plán';
$calStr['public url'] = 'Veøejná URL: %url';
$calStr['no schedules'] = '<p>V tomto týdnu nebyly zadány ¾ádné plány.  <br>Pro zadání plánu kliknìte na data vý¹e.';
$calStr['no schedules public'] = '<p>V tomto týdnu nebyly zadány ¾ádné plány.';
$calStr['all day'] = "Celý den";
$calStr['is all day'] = 'Toto je celodenní akce';
?>
