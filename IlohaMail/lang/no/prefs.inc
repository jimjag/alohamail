<?php
include("../lang/eng/prefs.inc"); // <-- do not delete
/********************************************************
	lang/no/prefs.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

	Translated by Tor Olsen <tor@monet.no>
********************************************************/

$prefHeaderStrings[0] = "Innstillinger";
$prefHeaderStrings[1] = "Generelle";
$prefHeaderStrings[2] = "Farger";

											
$prefsStrings["0.0"] = "Identitet";
$prefsStrings["0.1"] = "Navn: ";
$prefsStrings["0.2"] = "E-post: ";
$prefsStrings["1.0"] = "Internasjonal";
$prefsStrings["1.1"] = "Grensesnitt: ";
$prefsStrings["1.2"] = "Tegnsetting: ";
$prefsStrings["1.3"] = "Tidssone: ";
$prefsStrings["2.0"] = "Innstilling for oversikt";
$prefsStrings["2.1"] = "Vis opp til ";
$prefsStrings["2.2"] = " poster";
$prefsStrings["2.3"] = "Vis størrelse";
$prefsStrings["2.4"] = "Sorter etter: ";
$prefsStrings["2.5"] = "";
$prefsStrings["2.6"] = "Sorteringsorden: ";
$prefsStrings["2.7"] = "";
$prefsStrings["2.8"] = "Vis mappeliste ";
$prefsStrings["3.0"] = "Auto-Lagre valg";
$prefsStrings["3.1"] = "Lagre sendte poster.";
$prefsStrings["3.2"] = "Lagre sendte beskjeder i mappen: ";
$prefsStrings["3.3"] = "";
$prefsStrings["3.4"] = "Slett søppel automatisk";
$prefsStrings["3.5"] = "Flytt slettede poster til: ";
$prefsStrings["3.6"] = "";
$prefsStrings["3.7"] = "Rotmappen: ";
$prefsStrings["3.8"] = "Annen";
$prefsStrings["4.0"] = "Egenskaper for meldingsvisning";
$prefsStrings["4.1"] = "Se i ramme";
$prefsStrings["4.2"] = "Fargemerk anførselstegn";
$prefsStrings["4.3"] = "Dette valget gjør systemet tregere.";
$prefsStrings["4.4"] = "Konverter URLer til lenker";
$prefsStrings["4.5"] = "Vis HTML beskjeder";
$prefsStrings["4.6"] = "Vis bilder på rekke";
$prefsStrings["5.0"] = "Signatur";
$prefsStrings["5.1"] = "Vis signatur som standard";
$prefsStrings["5.2"] = "";
$prefsStrings["6.0"] = "Skriv";
$prefsStrings["6.1"] = "Vis kontakter";
$prefsStrings["6.2"] = "Vis CC/BCC felt";
$prefsStrings["6.3"] = "Lukk etter send";
$prefsStrings["6.4"] = "Skriv innenfor rammen";
$prefsStrings["6.5"] = "Tekstboksstørrelse: ";
$prefsStrings['6.6'] = 'Send kopi til deg selv som standard';
$prefsStrings["7.0.0"] = "Instillningar for disk og mappe";
$prefsStrings["7.0.1"] = "Instillningar för diskkvote";
$prefsStrings["7.1"] = "Vis antallet uleste";
$prefsStrings["7.2"] = "Uppdatere hvert %n sekund";
$prefsStrings['8.0'] = 'Grensesnitt';
$prefsStrings["8.1"] = 'Tema';
$prefsStrings["8.2"] = "JavaScript-bruk";
$prefsStrings["8.3"] = "Lav";
$prefsStrings["8.4"] = "Høy";
$prefsStrings['8.5'] = 'Automatisk';

$pref_col_title = "Omarranger kolonner";
$pref_colstr["order"] = "Sorter";
$pref_colstr["field"] = "Felt";
$pref_col_label['c'] = "Avmerkingsboks";
$pref_col_label['a'] = "Vedlegg";
$pref_col_label['m'] = "flagg/Status";




$sort_fields=array("DATE"=>"Dato", "SUBJECT"=>"Emne", "SIZE"=>"Størrelse");
$sort_orders=array("ASC"=>"Stigende", "DESC"=>"Fallende");

$prefsButtonStrings = array( 0=> "Oppdater", 1=>"Bruk", 2=> "Avbryt", 3=> "Standard");

$prefs_new_user = "Det ser ut som du logger på for første gang...velkommen!<br>";
$prefs_new_user .= "Vennligst gå igjennom følgende innstillinger og klikk, \"Bruk\"";
?>