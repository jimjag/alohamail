<?php
/********************************************************
	lang/eng/main.inc
	(C)2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

function formatStatusString($num, $folder, $type){
	$str="";
	if ($type=="delete"){
		$str = "����� $num ������".($num>1?"s":"");
	}else if ($type == "move"){
		$str = "������ $num ������".($num>1?"s":"")." � $folder";
	}
	
	return $str;
}

$mainStrings=array(
	0=>"��� %p � %n",
	1=>" ����� �����",
	2=>"�����",
	3=>" ������",
	4=>"���",
	5=>" ������",
	6=>"����",
	7=>"��",
	8=>"���",
	9=>"�����",
	10=>"���",
	11=>"���� �� ����",
	12=>"����",
	13=>"�����",
	14=>"����",
	15=>"�� �����",
	16=>"����",
	17=>"���� ���",
	18=>"��� ����",
	19=>"���"
);

$mainErrors=array(
	0=>"�� ����� ������ ������ ���",
	1=>"��� ������ ������ ���",
	2=>"��� ������ ������ ������ ��� ����",
	3=>"��� ������ ���� ����� ��� ������",
	4=>"��� ������ ������ �����",
    5=>".�� ����� �� ����",
	6=>"��� ������ ����� �� ����",
	7=>" ��� ��� ���� ������� ����� �� �� ������� ������ ��? "
);


?>
