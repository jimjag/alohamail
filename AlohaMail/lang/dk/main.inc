<?php
include("../lang/eng/main.inc"); // <-- do not delete
/********************************************************
        lang/dk/main.inc
        (C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

        This file is part of IlohaMail, and released under GPL.
        See COPYING, or http://www.fsf.org/copyleft/gpl.html

        Translated by Torben Rybner, Ziberex (try@ziberex.dk)
	          and Niels Baggesen <nba@users.sourceforge.net>

********************************************************/

function formatStatusString($num, $folder, $type){
        $str="";
        if ($type=="delete"){
                $str = "Slet $num meddelelse".($num>1?"r":"");
        }else if ($type == "move"){
                $str = "Flyttet $num meddelelse".($num>1?"r":"")." til $folder";
        }
        
        return $str;
}

$mainMessages["delete"] = "Slet %n meddelelse(r)";
$mainMessages["move"] = "Flyttede %n meddelelse(r) til %f";

$mainStrings[0] = "Meddelelse %p af %n";
$mainStrings[1] = " meddelelser fundet";
$mainStrings[2] = "Foreg&aring;ende";
$mainStrings[3] = " medd.";
$mainStrings[4] = "N&aelig;ste";
$mainStrings[5] = " medd.";
$mainStrings[6] = "Emne";
$mainStrings[7] = "Til";
$mainStrings[8] = "Fra";
$mainStrings[9] = "Dato";
$mainStrings[10] = "Slet";
$mainStrings[11] = "T&oslash;m papirkurv";
$mainStrings[12] = "Flyt markerede til";
$mainStrings[13] = "S&oslash;g";
$mainStrings[14] = "St&oslash;rrelse";
$mainStrings[15] = "Ikke navngivet";
$mainStrings[16] = "Vis";
$mainStrings[17] = "Opdater";
$mainStrings[18] = "Slet alle";
$mainStrings[19] = "Afbryd";
$mainStrings[20] = "Marker som %b";
$mainStrings[21] = "L&aelig;st";
$mainStrings[22] = "Ul&aelig;st";
$mainStrings[23] = "V&aelig;lg...";
$mainStrings[24] = "Brug filter";
$mainStrings[25] = "Marker";
$mainStrings[26] = "Vigtig";
$mainStrings[27] = "Filtre:";
$mainStrings[28] = "Alle filtre";
$mainStrings[29] = "Anvend";
$mainStrings[30] = "Hurtig s&oslash;gning:";
$mainStrings[31] = "V&aelig;lg";
$mainStrings[32] = 'Normal';

$mainErrors[0] = "Ingen meddelelser fundet i denne folder";
$mainErrors[1] = "Ingen meddelelser i denne folder";
$mainErrors[2] = "Kunne ikke flytte meddelse til papirkurven.";
$mainErrors[3] = "Kunne ikke m&aelig;rke meddelelsen som slettet.";
$mainErrors[4] = "Kunne ikke flytte meddelelser.";
$mainErrors[5] = "Papirkurv ikke angivet.";
$mainErrors[6] = "Kunne ikke &aring;bne papirkurven";
$mainErrors[7] = "Er du sikker p&aring; at du vil slette ALLE meddelelser i %f?";
$mainErrors[8] = "Klik p&aring; pilene nedenfor kolonneoverskriften for at flytte dem.<br>N&aring;r du er f&aelig;rdig lukker du bare vinduet.";

?>
