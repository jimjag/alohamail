<?php
include("../lang/eng/search.inc"); // <-- do not delete
/********************************************************
	lang/eng/search.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
	Simplified Chinese (GB2312), for use in China, 
	Singapore & Malaysia Chinese communities
	Translator: Danny Lin <danny@groopie.com>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

	//TRANSLATORS:
	//in the follow arrays, translate key, not value
	//i.e.  translate "From" not "FROM".
	$search_fields=array("select"=>"-", "�ļ���"=>"FROM", "�ռ���"=>"TO", "�ʼ�����"=>"SUBJECT","�ʼ�����"=>"BODY");
	$search_dates=array("����"=>"ignore", "��������"=>"ON", "�ڴ����ں�"=>"SINCE", "�ڴ�����ǰ"=>"BEFORE");

	$search_str["title"] = "����";
	$search_str["str"] = "��<b>%folders</b>�в���<br>\n";
	$search_str["str"] .= "��<b>%fields</b>�����а���<b>%value</b>�ַ���<br>\n";
	$search_str["str"] .= "���ҷ�������Ϊ<b>%dateops %m/%d/%y</b> ���ʼ�<br>\n";
	$search_str["submit"] = "��������";
?>