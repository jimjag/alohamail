<?php
/********************************************************
	lang/eng/prefs.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/prefs.inc");

$prefHeaderStrings[0] = "Pirmumo nustatymai";
$prefHeaderStrings[1] = "Bendri";
$prefHeaderStrings[3] = "Profiliai";
$prefHeaderStrings[2] = "I�vaizda";
$prefHeaderStrings[4] = "Filtrai";

											
$prefsStrings["0.0"] = "Pagrindinis profilis nutylint";
$prefsStrings["0.1"] = "Vardas: ";
$prefsStrings["0.2"] = "El. pa�tas: ";
$prefsStrings["0.3"] = "Pakeitimai gali b�ti padaryti \"%s\" puslapyje.";
$prefsStrings["0.4"] = "Nenurodyta";
$prefsStrings["1.0"] = "Tarptautinis";
$prefsStrings["1.1"] = "Aplinka: ";
$prefsStrings["1.2"] = "Simboli� rinkinys: ";
$prefsStrings["1.3"] = "Laiko zona: ";
$prefsStrings["1.4"] = "Laiko nustatymas: ";
$prefsStrings["1.5"] = array(12=>"12-os valand� laikrodis", 24=>"24-i� valand� laikrodis");
$prefsStrings["2.0"] = "Parodyti per�i�r�jimo nustatymus";
$prefsStrings["2.1"] = "Rodyti iki ";
$prefsStrings["2.2"] = " element�";
$prefsStrings["2.3"] = "Rodyti dyd�";
$prefsStrings["2.4"] = "R��iuoti pagal: ";
$prefsStrings["2.5"] = "";
$prefsStrings["2.6"] = "R��iavimo tvarka: ";
$prefsStrings["2.7"] = "";
$prefsStrings["2.8"] = "Rodyti aplank� s�ra��";
$prefsStrings["2.9"] = "Aplank� s�ra�o dyd�iai";
$prefsStrings["2.10"] = "Tikrinti naujai gautus lai�kus kas %n sekund�i�";
$prefsStrings["2.11"] = "Rodyti �ranki� rinkin� %m";
$prefsStrings["2.12"] = array("b"=>"viduryje", "t"=>"vir�uje", "bt"=>"vir�uje ir viduryje");
$prefsStrings["2.13"] = "Pertvarkyti stulpelius: ";
$prefsStrings["2.14"] = "spausti �ia";
$prefsStrings["2.15"] = "Rodyti filtr�/greitos paie�kos �ranki� rinkin�";
$prefsStrings["3.0"] = "Automatinio i�sisaugojimo nustatymai";
$prefsStrings["3.1"] = "I�saugoti i�si�stus lai�kus.";
$prefsStrings["3.2"] = "I�saugoti i�si�stus lai�kus aplanke: ";
$prefsStrings["3.3"] = "";
$prefsStrings["3.4"] = "Automati�kai i�valyti �i�k�liad���";
$prefsStrings["3.5"] = "Perkelti i�trintus elementus � aplank�: ";
$prefsStrings["3.6"] = "";
$prefsStrings["3.7"] = "Pagrindin� direktorija: ";
$prefsStrings["3.8"] = "Kita";
$prefsStrings["3.9"] = "Pasl�pti neprisira�ytus aplankus";
$prefsStrings["3.10"] = "Rodyti kvot� %m";
$prefsStrings["3.11"] = array(""=>"nerodyti", "m"=>"lai�k� s�ra�e", "f"=>"aplank� s�ra�e");
$prefsStrings["4.0"] = "Lai�ko rodymo nustatymai";
$prefsStrings["4.1"] = "Rodyti r�muose";
$prefsStrings["4.2"] = "Nuspalvinti kvotas";
$prefsStrings["4.3"] = "�jungus �� nustatym� gali sul�t�ti j�s� sistemos darbas.";
$prefsStrings["4.4"] = "Konvertuoti adresus � nuorodas";
$prefsStrings["4.5"] = "Rodyti HTML lai�kus";
$prefsStrings["4.6"] = "Rodyti paveiksl�lius linijoje";
$prefsStrings["4.7"] = "Rodyti navigacijos nuorodas";
$prefsStrings["4.8"] = "Ne�ym�ti lai�k� kai po juos vaik��iojama";
$prefsStrings["5.0"] = "Para�as";
$prefsStrings["5.1"] = "Rodyti para�� nutylint";
$prefsStrings["5.2"] = "";
$prefsStrings["6.0"] = "Ra�yti";
$prefsStrings["6.1"] = "Rodyti kontaktus";
$prefsStrings["6.2"] = "Rodyti Kam kopija/Kam nematoma kopija laukus";
$prefsStrings["6.3"] = "U�daryti po i�siuntimo";
$prefsStrings["6.4"] = "Ra�yti r�mo viduje";
$prefsStrings["6.5"] = "Teksto d���s dydis: ";
$prefsStrings["7.0.0"] = "Disko &amp; Aplanko nustatymai";
$prefsStrings["7.0.1"] = "Kvot� nustatymai";
$prefsStrings["7.1"] = "Rodyti neperskaityt� kiek�";
$prefsStrings["7.2"] = "Atnaujinti aplank� s�ra�� kas %n sekundes";

$pref_col_title = "Pertvarkyti stulpelius";
$pref_colstr["order"] = "Tvarka";
$pref_colstr["field"] = "Laukas";
$pref_col_label['c'] = "Pa�ym�jimas";
$pref_col_label['a'] = "Prisegtos bylos";
$pref_col_label['m'] = "V�liav�l�s/statusas";

$sort_fields=array('DATE'=>'I�siuntimo laikas', 'SUBJECT'=>'Tema', 'SIZE'=>'Dydis');
$sort_fields['INTERNALDATE'] = 'Gavimo laikas';
$sort_orders=array("ASC"=>"Did�jan�ia tvarka", "DESC"=>"Ma��jan�ia tvarka");

$prefsButtonStrings = array( 0=> "Atnaujinti", 1=>"Taikyti", 2=> "At�aukti", 3=> "Pagal nutyl�jim�");

$prefs_new_user = "J�s prisijung�te pirm� kart�...sveikiname!<br>";
$prefs_new_user .= "Pra�ome per�velgti �iuos nustatymus, tada spauskite \"Taikyti\"";
?>