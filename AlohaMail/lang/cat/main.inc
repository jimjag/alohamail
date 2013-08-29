<?php
include("../lang/eng/main.inc"); // <-- do not delete
/********************************************************
	lang/cat/main.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

function formatStatusString($num, $folder, $type){
	$str="";
	if ($type=="delete"){
		$str = "Eliminar missatge $num ".($num>1?"s":"");
	}else if ($type == "move"){
		$str = "S'ha mogut el missatge $num ".($num>1?"s":"")." a $folder";
	}
	
	return $str;
}

$mainStrings[0] = "%n missatges en total";
$mainStrings[1] = " missatges trobats";
$mainStrings[2] = "Previ";
$mainStrings[3] = " missatges";
$mainStrings[4] = "Seg&uuml;ent";
$mainStrings[5] = " missatges";
$mainStrings[6] = "Assumpte";
$mainStrings[7] = "Per a";
$mainStrings[8] = "De";
$mainStrings[9] = "Data";
$mainStrings[10] = "Eliminar";
$mainStrings[11] = "Buidar Paperera";
$mainStrings[12] = "Arxiu";
$mainStrings[13] = "Cerca";
$mainStrings[14] = "Tamany";
$mainStrings[15] = "Sense t&iacute;tol";
$mainStrings[16] = "Mostrar";
$mainStrings[17] = "Comprovar nous";
$mainStrings[18] = "Esborrar tots";
$mainStrings[19] = "Cancel&middot;lar";


$mainErrors[0] = "No s'han trobat missatges en aquesta carpeta";
$mainErrors[1] = "No hi han missatges en aquesta carpeta";
$mainErrors[2] = "No s'han pogut moure els missatges a la paperera.";
$mainErrors[3] = "No s'han pogut marcar els missatges com esborrats.";
$mainErrors[4] = "No s'han pogut moure els missatges'.";
$mainErrors[5] = "Carpeta Paperera no especificada.";
$mainErrors[6] = "No s'ha pogut llen&ccedil;ar a la paperera";
$mainErrors[7] = "Est&agrave;s segur/a que vols eliminar TOTS els missatges de %f?";



?>
