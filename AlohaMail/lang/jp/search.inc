<?php
include("../lang/eng/search.inc"); // <-- do not delete
/********************************************************
	lang/jp/search.inc
	(C) 1999-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

	PURPOSE:
		Provide interface for search function.

********************************************************/

	$search_fields=array("����..."=>"-", "������"=>"FROM", "����"=>"TO", "��̾"=>"SUBJECT","��ʸ"=>"BODY");
	
	$search_dates=array("̤����"=>"ignore", "��"=>"SENTON", "�ʹߤ�"=>"SENTSINCE", "������"=>"SENTBEFORE");

	$search_str["title"] = "����";
	$search_str["str"] = "%folders �����<br>\n";
	$search_str["str"] .= "%fields �� %value ��ޤ�<br>\n";
	$search_str["str"] .= "���դ��� %m/%d/%y %dateops �ι��� <br>\n";
	$search_str["submit"] = "��������";
?>