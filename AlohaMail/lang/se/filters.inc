<?php
/********************************************************
	lang/eng/filters.inc
	(C)2001-2004 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

	Translated by Stefan Pettersson <stefan@pezzen.org>

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/filters.inc");

 # prefs.inc:$prefHeaderStrings[4] = $filter_title["filters"] = "Filter";

/*
$condition[] fields: "operand", "field", "bool", "condition", "subject"
*/
$fltr["filter"] = "Filter";
$fltr["conditions"] = "Villkor";
$fltr["actions"] = "�tg�rder";
$fltr["auto"] = "K�r automatiskt i inkorgen";
$fltr["deletemsg"] = "Radera Meddelande";
$fltr["&"] = "och";
$fltr["|"] = "eller";
$fltr["^"] = "b�rjar med";
$fltr["<"] = "inneh�ller";
$fltr["$"] = "slutar med";
$fltr["="] = "Meddelanden som";
$fltr["!"] = "Meddelanden som inte";
$fltr["field"] = "F�lt";
$fltr["condition"] = "Villkor";
$fltr["action"] = "�tg�rd";
$fltr["name"] = "Namn";
$fltr["sort_order"] = "Prioritet";
$fltr["edit"] = "Redigera";
$fltr["add"] = "L�gg till filter";
$fltr["edit_filter"] = "Redigera filter";
$fltr["delete"] = "Ta bort";
$fltr["filters"] = "Filter";
$fltr["new"] = "Nytt Filter";
$fltr["status"] = "Status";
$fltr["stat_en"] = "Aktiv";
$fltr["stat_dis"] = "Deaktiverad";
$fltr["is_active"] = "filtret �r aktivt";
$fltr["other"] = "Annat";
$fltr["move"] = "Flytta till";
$fltr["copy"] = "Kopiera till";
$fltr["do_nothing"] = "G�r ingenting";
$fltr["add_cond"] = "L�gg till villkor";
$fltr["remove_cond"] = "Ta bort villkor";
$fltr["condition_str"] = "%b %c %t";

$f_hlp["priority"] = "(H�gre prioriteringar k�rs f�rst.)";

$f_err["no_name"] = "Namnge det h�r filtret\n";
$f_err["no_act"] = "V�lj en eller flera �tg�rder f�r det h�r filtret\n";
$f_err["no_cond"] = "V�lj en eller flera villkor f�r det h�r filtret\n";
$f_err["inv_param"] = "Ogiltiga parametrar f�r filtret\n";$f_err["inv_sort"] = "Prioriterinsordningen m�ste vara numerisk\n";

?>