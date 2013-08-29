<?php
include("../lang/eng/prefs.inc"); // <-- do not delete
/********************************************************
	lang/cat/prefs.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

$prefHeaderStrings[0] = "Prefer&egrave;ncies";
$prefHeaderStrings[1] = "General";
$prefHeaderStrings[2] = "Colors";

											
$prefsStrings["0.0"] = "Identitat";
$prefsStrings["0.1"] = "Nom: ";
$prefsStrings["0.2"] = "Email: ";
$prefsStrings["1.0"] = "Internacional";
$prefsStrings["1.1"] = "Interf&iacute;cie: ";
$prefsStrings["1.2"] = "Car&agrave;cters: ";
$prefsStrings["1.3"] = "Horari: ";
$prefsStrings["2.0"] = "Opcions de vista de llista";
$prefsStrings["2.1"] = "Mostrar fins a ";
$prefsStrings["2.2"] = " elements";
$prefsStrings["2.3"] = "Mostrar tamany";
$prefsStrings["2.4"] = "Ordenar per: ";
$prefsStrings["2.5"] = "";
$prefsStrings["2.6"] = "Ordre: ";
$prefsStrings["2.7"] = "";
$prefsStrings["2.8"] = "Mostrar llista de carpetes";
$prefsStrings["3.0"] = "Opcions d'Auto-Guardat";
$prefsStrings["3.1"] = "Desar elements enviats.";
$prefsStrings["3.2"] = "Desar elements enviats a la carpeta: ";
$prefsStrings["3.3"] = "";
$prefsStrings["3.4"] = "Eliminar escombraries autom&agrave;ticament";
$prefsStrings["3.5"] = "Moure elements eliminats a la carpeta: ";
$prefsStrings["3.6"] = "";
$prefsStrings["3.7"] = "Directori Arrel: ";
$prefsStrings["3.8"] = "Un altre";
$prefsStrings["4.0"] = "Opcions per la visualitzaci&oacute; d'un missatge";
$prefsStrings["4.1"] = "Veure en frame";
$prefsStrings["4.2"] = "Posar color a les cometes";
$prefsStrings["4.3"] = "Habilitar aquesta opci&oacute; pot fer m&eacute;s lent el sistema.";
$prefsStrings["4.4"] = "Convertir URLs a adreces";
$prefsStrings["4.5"] = "Mostrar missatges en HTML";
$prefsStrings["4.6"] = "Mostrar imatges en el context";
$prefsStrings["5.0"] = "Signatura";
$prefsStrings["5.1"] = "Mostrar la signatura predeterminada";
$prefsStrings["5.2"] = "";


$sort_fields=array("DATE"=>"Data", "SUBJECT"=>"Assumpte", "SIZE"=>"Tamany");
$sort_orders=array("ASC"=>"Ascendent", "DESC"=>"Descendent");

$prefsButtonStrings = array( 0=> "Actualitzar", 1=>"Aplicar", 2=> "Revertir", 3=> "Per defecte");

$prefs_new_user = "Sembla ser que &eacute;s la primera vegada que inicies sessi&oacute;. ¡Benvingut/da!<br>";
$prefs_new_user .= "Per favor, configura el teu compte i posteriorment pressiona \"Aplicar\"";
?>
