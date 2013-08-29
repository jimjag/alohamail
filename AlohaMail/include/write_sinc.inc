<?php
/////////////////////////////////////////////////////////
//	
//	include/write_sinc.inc
//
//	(C)Copyright 2000-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	PURPOSE:
		Shell for write_sinc.backend.inc.
		Gets config from ../conf/conf.php

********************************************************/        

include('../conf/conf.php');
if ($backend!="FS") $backend = "DB";

include('../conf/defaults.inc');
include_once('../include/fs_path.inc');
include_once('../include/cache.inc');
include_once('../include/array2php.inc');
include_once('../include/encryption.inc');
include_once("../include/data_store.inc");

//load backend-agnostic functions
include_once('../include/write_sinc.common.inc');

//load backend-specific functions
include("../include/write_sinc.".$backend.".inc");

?>