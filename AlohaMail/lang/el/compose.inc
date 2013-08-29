<?php
/********************************************************
	lang/eng/compose.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/compose.inc");

$composeHStrings[0] = "Θέμα";
$composeHStrings[1] = "Από";
$composeHStrings[2] = "Πρός";
$composeHStrings[3] = "Κοινοποίηση";
$composeHStrings[4] = "Κρυφή Κοινοποίηση";
$composeHStrings[5] = "Ημερομηνία";


$composeStrings[0] = "Σύνθεση Επιστολής";
$composeStrings[1] = "Αποστολή Επιστολής";
$composeStrings[2] = "Ανέβασμα";
$composeStrings[3] = "Προσθήκη υπογραφής";
$composeStrings[4] = "Επισύναψη";
$composeStrings[5] = "Επαφές";
$composeStrings[6] = "Απόκρυψη Επαφών";
$composeStrings[7] = "Κείμενο Επιστολής";
$composeStrings[8] = "Προσθήκη";
$composeStrings[9] = "Στις %d, ο/η %s έγραψε:\n";
$composeStrings[10] = "--- Αρχική Επιστολή ---\n";
$composeStrings[11] = "Κλείσιμο Παραθύρου";

$composeStrings[12] = "Ορθογραφικός Έλεγχος";
$composeStrings[13] = "Σωστή Ορθογραφία";
$composeStrings[14] = "Χωρίς Αλλαγές";
$composeStrings[15] = "Πιθανά Ορθογραφικά Λάθη";
$composeStrings[16] = "Ορθογραφικές Αλλαγές";
$composeStrings[17] = "παράλειψη";
$composeStrings[18] = "διαγραφή";
$composeStrings[19] = 'Κρυφή Κοινοποίηση σε αυτή τη διεύθυνση';

$composeStrings["title"] = "Σύνθεση Επιστολής";

$composeErrors[0] = "Το πεδίο του θέματος είναι άδειο. Παρακαλώ εισάγετε ένα θέμα ή πατήστε το \"".$composeStrings[1]."\" ξανά για να στείλετε.";
$composeErrors[1] = "Το πεδίο \"Από\" είναι κενό. Παρακαλώ καθορίστε την ηλεκτρονική σας διεύθυνση.";
$composeErrors[2] = "Το πεδίο \"Προς\" είναι κενό. Παρακαλώ καθορίστε τη διεύθυνση προορισμού.";
$composeErrors[3] = "Το ανέβασμα απέτυχε.";
$composeErrors[4] = "Κανένα αρχείο δεν παραλήφθηκε.";
$composeErrors[5] = "Για λόγους παρεμπόδισης της χρήσης αυτού του συστήματος για την "
	. "αυτοματοποιημένη αποστολή παρενοχλητικών ή διαφημιστικών επιστολών, μπορείτε "
	. "να στείλετε μονάχα σε %1 ανθρώπους (%2 συνολικά σε κάθε συνεδρία) κάθε "
	. "%3 δευτερόλεπτα.";

$composeErrors[6] = "Δε βρέθηκαν ορθογραφικά λάθη.";
$composeErrors[7] = "Ο ορθογραφικός έλεγχος δεν είναι διαθέσιμος για τη καθορισμένη γλώσσα..";
$composeErrors[8] = "(%s λεξικό σε χρήση)";
?>
