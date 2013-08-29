<?php
/********************************************************
	lang/<lang>/calendar.inc
	(C)2003 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/calendar.inc");

$calStr["add_schedule"] = "Agregar Apunte";
$calStr["edit_schedule"] = "Editar Apunte";
$calStr["weekof"] = "Semana del: %d";
$calStr["title"] = "Título:";
$calStr["color"] = "Color:";

//for $calStr['color_names'], translate strings
//on the right hand side of equal sign
$calStr["color_names"]['Dark Red'] = 'Rojo Oscuro';
$calStr["color_names"]['Red'] = 'Rojo';
$calStr["color_names"]['Deep Blue'] = 'Azul Profundo';
$calStr["color_names"]['Blue'] = 'Azul';
$calStr["color_names"]['Dark Green'] = 'Verde Oscuro';
$calStr["color_names"]['Green'] = 'Verde';
$calStr["color_names"]['Purple'] = 'Purpura';
$calStr["color_names"]['Cyan'] = 'Cyan';
$calStr["color_names"]['Orange'] = 'Naranja';
$calStr["color_names"]['Yellow'] = 'Amarillo';
$calStr["color_names"]['Magenta'] = 'Magenta';
$calStr["color_names"]['No Color'] = 'Sin Color';

$calStr["starts"] = "Comienza a:";
$calStr["ends"] = "Finaliza a:";
$calStr["from"] = "desde:";
$calStr["until"] = "hasta:";
$calStr["repeat_on"] = "Repetir en";
$calStr["of"] = "de cada";
$calStr["weeks"] = array(1=>"primer semana", 2=>"segunda semana", 3=>"tercer semana", 4=>"cuarta semana"); 
$calStr["week_blurb"] = "...de cada mes (hasta la fecha de finalizacion).<br>Se repetirá todas las semanas si no indica nada.";
$calStr["monthly"] = "Repetir mensualmente";
$calStr["yearly"] = "Repetir anualmente";
$calStr["place"] = "Lugar:";
$calStr["description"] = "Descripción:";
$calStr["add"] = "Agregar";
$calStr["update"] = "Actualizar";
$calStr["Delete"] = "Borrar";
$calStr["go"] = "&nbsp;Ir&nbsp;";
$calStr["disabled"] = "El calendario está deshabilitado.";
$calStr["public"] = "Hacer público este apunte"; 
$calStr["publish"] = "Hacer público este apunte";
$calStr['name'] = 'Apunte de %name';
$calStr['public url'] = 'URL: %url';
$calStr['no schedules'] = '<p>No hay apuntes esta semana. <br>Click en las fechas para ingresar un nuevo apunte.';
$calStr['no schedules public'] = '<p>No hay apuntes ingresados esta semana.';
$calStr['all day'] = "Todo el día";
$calStr['is all day'] = 'Este es un apunte de todo el día';
$calStr['format'] = 'Formato:';
$calStr['boxed'] = 'Cajas';
$calStr['list'] = 'Lista';
$calStr['scale'] = 'Escala:';
$calStr['bigger'] = 'Aumentar';
$calStr['smaller'] = 'Disminuir';
?>
