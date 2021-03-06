<?php
/********************************************************
	lang/<lang>/calendar.inc
	(C)2003 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/calendar.inc");

$calStr["add_schedule"] = "P�idat pl�n";
$calStr["edit_schedule"] = "Upravit pl�n";
$calStr["weekof"] = "T�den: %d";
$calStr["title"] = "N�zev:";
$calStr["color"] = "Barva:";

//for $calStr['color_names'], translate strings
//on the right hand side of equal sign
$calStr["color_names"]['Dark Red'] = 'Tmav� �erven�';
$calStr["color_names"]['Red'] = '�erven�';
$calStr["color_names"]['Deep Blue'] = 'Tmav� modr�';
$calStr["color_names"]['Blue'] = 'Modr�';
$calStr["color_names"]['Dark Green'] = 'Tmav� zelen�';
$calStr["color_names"]['Green'] = 'Zelen�';
$calStr["color_names"]['Purple'] = 'Fialov�';
$calStr["color_names"]['Cyan'] = 'Azurov�';
$calStr["color_names"]['Orange'] = 'Oran�ov�';
$calStr["color_names"]['Yellow'] = '�lut�';
$calStr["color_names"]['Magenta'] = 'Purpurov�';
$calStr["color_names"]['No Color'] = 'Bez barvy';

$calStr["starts"] = "Za��n�:";
$calStr["ends"] = "Kon��:";
$calStr["from"] = "od:";
$calStr["until"] = "do:";
$calStr["repeat_on"] = "Opakovat";
$calStr["of"] = "ka�d�";
$calStr["weeks"] = array(1=>"prvn� t�den", 2=>"druh� t�den", 3=>"t�et� t�den", 4=>"�tvrt� t�den"); 
$calStr["week_blurb"] = "...ka�d� m�s�c (do data ukon�en�).<br>M��e se opakovat ka�d� t�den pokud nen� ��dn� vybr�n.";
$calStr["monthly"] = "Opakovat ka�d� m�s�c";
$calStr["yearly"] = "Opakovat ka�d� rok";
$calStr["place"] = "M�sto:";
$calStr["description"] = "Popis:";
$calStr["add"] = "P�idat";
$calStr["update"] = "Aktualizovat";
$calStr["Delete"] = "Smazat";
$calStr["go"] = "Zobraz";
$calStr["disabled"] = "Funkce kalend��e byla zak�z�na.";
$calStr["public"] = "Ozna�it z�znam jako ve�ejn�";
$calStr["publish"] = "Zve�ejnit z�znam";
$calStr['name'] = '%name\'s pl�n';
$calStr['public url'] = 'Ve�ejn� URL: %url';
$calStr['no schedules'] = '<p>V tomto t�dnu nebyly zad�ny ��dn� pl�ny.  <br>Pro zad�n� pl�nu klikn�te na data v��e.';
$calStr['no schedules public'] = '<p>V tomto t�dnu nebyly zad�ny ��dn� pl�ny.';
$calStr['all day'] = "Cel� den";
$calStr['is all day'] = 'Toto je celodenn� akce';
?>
