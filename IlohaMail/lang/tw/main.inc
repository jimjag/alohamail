<?php
include("../lang/eng/main.inc"); // <-- do not delete
/********************************************************
	lang/tw/main.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
	Traditional Chinese (BIG5), for use in Taiwan, 
	Hong Kong, and Overseas Chinese communities
	Translator: Danny Lin <danny@groopie.com>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

function formatStatusString($num, $folder, $type){
	$str="";
	if ($type=="delete"){
		$str = "�R�� $num �ʫH��".($num>1?"":"");
	}else if ($type == "move"){
		$str = "�h�� $num �ʫH��".($num>1?"":"")." �� $folder";
	}
	
	return $str;
}

$mainStrings[0] = "�\Ū�G�� %p ��/�@ %n ��";
$mainStrings[1] = " �ʫH��";
$mainStrings[2] = "�W�@��";
$mainStrings[3] = " ��";
$mainStrings[4] = "�U�@��";
$mainStrings[5] = " ��";
$mainStrings[6] = "�H����D";
$mainStrings[7] = "����H";
$mainStrings[8] = "�H��H";
$mainStrings[9] = "���";
$mainStrings[10] = "�R��";
$mainStrings[11] = "�M�Ÿ귽�^����";
$mainStrings[12] = "����";
$mainStrings[13] = "�j�M";
$mainStrings[14] = "�j�p";
$mainStrings[15] = "���R�W";
$mainStrings[16] = "���";
$mainStrings[17] = "�ˬd�s�H";
$mainStrings[18] = "�M��";
$mainStrings[19] = "����";
$mainStrings[20] = "�Хܦ� %b";
$mainStrings[21] = "�w�\Ū";
$mainStrings[22] = "���\Ū";
$mainStrings[23] = "���...";


$mainErrors[0] = "���H��X���S������H��C";
$mainErrors[1] = "���H��X���S������H��C";
$mainErrors[2] = "�L�k�h���H��ܸ귽�^����";
$mainErrors[3] = "�L�k�аO�H�󬰤w�R���C";
$mainErrors[4] = "�L�k�h���H��C";
$mainErrors[5] = "�|�����w�귽�^����";
$mainErrors[6] = "�L�k�M�Ÿ귽�^����";
$mainErrors[7] = "�A�T�w�n�M�ũҦ��b %f �����H��H";
$mainErrors[8] = "�b�U����줤���b�ιϮפW,������بӲ��ʥ���<br>����������������";



?>