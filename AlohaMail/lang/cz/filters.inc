<?php
/********************************************************
	lang/eng/filters.inc
	(C)2001-2004 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/filters.inc");

# prefs.inc:$prefHeaderStrings[4] = $filter_title["filters"] = "Filters";
	

/*
$condition[] fields: "operand", "field", "bool", "condition", "subject"
*/

$fltr["filter"] = "Filtry";
$fltr["conditions"] = "Podm�nky";
$fltr["actions"] = "Akce";
$fltr["auto"] = "Automaticky aplikovat na doru�enou po�tu";
$fltr["deletemsg"] = "Smazat zpr�vu";
$fltr["&"] = "a";
$fltr["|"] = "nebo";
$fltr["^"] = "za��n�";
$fltr["<"] = "obsahuje";
$fltr["$"] = "kon��";
$fltr["="] = "";
$fltr["!"] = "ne";
$fltr["field"] = "Pole";
$fltr["condition"] = "Podm�nka";
$fltr["action"] = "Akce";
$fltr["name"] = "Jm�no";
$fltr["sort_order"] = "Priorita";
$fltr["edit"] = "upravit";
$fltr["add"] = "P�idat filtr";
$fltr["edit_filter"] = "Upravit filtr";
$fltr["delete"] = "Smazat";
$fltr["filters"] = "Filtry";
$fltr["new"] = "Nov� filtr";
$fltr["status"] = "Stav";
$fltr["stat_en"] = "povolen�";
$fltr["stat_dis"] = "zak�zan�";
$fltr["is_active"] = "filter je povolen�";
$fltr["other"] = "Ostatn�";
$fltr["move"] = "P�esunout do";
$fltr["copy"] = "Zkop�rovat do";
$fltr["do_nothing"] = "Ned�lat nic";
$fltr["add_cond"] = "P�idat podm�nku";
$fltr["remove_cond"] = "Smazat podm�nku";
$fltr["condition_str"] = "%b %c %t";

$f_hlp["priority"] = "(Nejd��ve se aplikuje vy��� priorita.)";

$f_err["no_name"] = "Zadejte pros�m jm�o pro toto filtrovac� pravidlo\n";
$f_err["no_act"] = "Vyberte pros�m jedno nebo v�ce pravidel pro tento filtr\n";
$f_err["no_cond"] = "Vyberte pros�m jednu nebo v�ce podm�nek pro tento filtr\n";
$f_err["inv_param"] = "Neplatn� parametry pro vybranou akci\n";
$f_err["inv_sort"] = "Priorita �azen� mus� b�t ��slo\n";

?>
