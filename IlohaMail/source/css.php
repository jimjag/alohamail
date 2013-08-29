<?php
/////////////////////////////////////////////////////////
//	
//	source/main.php
//
//	(C)Copyright 2000-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

include("../include/super2global.inc");
include_once("../conf/conf.php");
include_once("../conf/db_conf.php");
include("../include/session_auth.inc");

header("Content-Type: text/css");
//header("Expires: " . gmdate("D, d M Y H:i:s", time() + 3600) . " GMT");
//header("Cache-Control: max-age=3600, public");
list($in_time,$rand)=explode("-",$user);
header("Last-Modified: ".gmdate('D, d M Y H:i:s', $in_time - 3600 ).' GMT');

include_once('themes/'.$my_prefs['theme'].'/info.inc');

$css_file = $CSS_INCLUDES[$page];
if (empty($css_file)) $css_file = $CSS_INCLUDES['default'];
include('themes/'.$my_prefs['theme'].'/'.$css_file);
?>