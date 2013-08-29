<?php
/********************************************************
	include/calendar.inc
	
	(C)Copyright 2003 Ryo Chijiiwa <Ryo@IlohaMail.org>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html
	
	PURPOSE:
		Some common code for calendar feature

********************************************************/

class scheduleItem{
	var $id;
	var $title;
	var $place;
	var $description;
	var $participants;
	var $beginTime;
	var $endTime;
	var $color;
	var $shared;
}

function formatCalDate($month, $day, $year){
	$error = 0;
	$result = false;

	if (checkdate($month, $day, $year)){
		if (($day < 10) && (strlen($day)==1)) $day = "0".$day;
		if (($month < 10) && (strlen($month)==1)) $month = "0".$month;
		$result = $year.$month.$day;
	}
	
	return $result;
}

function cal_getbasetime($schedule){
	$earliest = 2400;
	$allday_exists = false;
	foreach($schedule as $day_sched)
		foreach($day_sched as $time=>$scheds){
			//some basic checks
			if (!is_numeric($time)|| !is_array($scheds)) continue;

			//go through scheduled items
			foreach($scheds as $item){
				if ($item->allday){	
					$allday_exists=true;	
					continue;
				}
				if ((int)$item->beginTime<$earliest) $earliest = (int)$item->beginTime;
			}
		}
	
	if ($allday_exists && $earliest>200) $earliest-=200;
	return $earliest;
}

function cal_padt4(&$t){
	while(strlen($t)<4) $t='0'.$t;
}


function cal_t4diff($t1,$t2){
	//takes two strings designating time as HHMM 
	//and calculates difference in minutes
	
	cal_padt4($t1);
	cal_padt4($t2);
	
	$h1 = (int)substr($t1, 0, 2);
	$h2 = (int)substr($t2, 0, 2);
	
	$m1 = (int)substr($t1, 2, 2);
	$m2 = (int)substr($t2, 2, 2);
	
	return (($h2 - $h1) * 60) + ($m2 - $m1);
}

function cal_showItem(&$item){
	global $user, $date, $IS_PUBLIC;
	global $my_prefs, $lang_datetime, $calStr, $cal_bg_colors;

	echo "<p>";
						
	if (!empty($item->color)) $style="style=\"color: ".$item->color."\"";
	else $style="";
						
	if (($item->beginTime+$item->endTime)!=0){
		echo LangFormatIntTime($item->beginTime, $my_prefs["clock_system"], $lang_datetime["ampm"], $lang_datetime["time_format"]);
		echo "-";
		echo LangFormatIntTime($item->endTime, $my_prefs["clock_system"], $lang_datetime["ampm"], $lang_datetime["time_format"]);
		echo ":<br>\n";
	}
	if (!$IS_PUBLIC) echo "<a href=\"edit_calendar.php?user=$user&edit=".$item->id."\" $style>";
	else echo '<span '.$style.'>';
	echo $item->title;
	echo (!empty($item->place)?" @":"").$item->place;
	if (!$IS_PUBLIC) echo "</a>\n";
	else echo '</span>';
}

function cal_showItemBox(&$item, $k2, $base_time, &$prev_end, &$cur_y){
	global $user, $date, $IS_PUBLIC;
	global $my_prefs, $lang_datetime, $calStr, $cal_bg_colors;

	if ($item->endTime<$item->beginTime) $item->endTime+=2400;
	if ($item->allday){
		$item->beginTime = $base_time;
		$item->endTime = 2400;
	}
		
	//calculate dimensions of box
	$duration = cal_t4diff($item->beginTime, $item->endTime);
	$height = round((float)$duration * 0.5);		//-2 to compensate for 1px border
	$since_last = cal_t4diff($prev_end, $item->beginTime);
	$since_base = cal_t4diff($base_time, $item->beginTime);
	$theoretical_y = (float)$since_base * 0.5;
	$pad_top = round($theoretical_y - $cur_y);
	$pad_top += 3 * $k2;
	$pad_left = 10 * $k2;
	
	if ($height<5) $height = 5;	
	$cur_y = max(($cur_y + $pad_top + $height),$cur_y);

	$height -= 2; //compensate for top+bottom 1px border
	
	if (!$IS_PUBLIC) $edit_url = 'edit_calendar.php?user='.$user.'&date='.$date.'&edit='.$item->id;
	
	//create div box
	$style = 'background-color: '.$cal_bg_colors[$item->color].';';
	$style.= 'margin: '.$pad_top.'px 0px 0px '.$pad_left.'px;';
	$style.= 'border: 1px solid '.$item->color.';';
	$style.= 'color: '.$item->color.';';
	$style.= 'height: '.$height.'px;';
	$style.= 'overflow:hidden;';
	if (!$IS_PUBLIC) $onclick = 'onClick="document.location=\''.$edit_url.'\';"';
	echo '<div style="'.$style.'" '.$onclick.'>';
	
	if (!empty($item->color)) $style='style="color: '.$item->color.'"';
	else $style="";
				
	//show time
	$time = '';
	if ($item->allday){
		$time = $calStr['all day'].':<br>';
	}else if ($duration!=0){
		$time = LangFormatIntTime($item->beginTime, $my_prefs['clock_system'], $lang_datetime['ampm'], $lang_datetime['time_format']);
		if ($my_prefs['clock_system']==24){
			$time.= "-";
			$time.= LangFormatIntTime($item->endTime, $my_prefs['clock_system'], $lang_datetime['ampm'], $lang_datetime['time_format']);
		}
		$time.= ":";
		if ($height>10) $time.="<br>\n";
	}
		
	//show title and place
	if (!$IS_PUBLIC) echo '<a href="'.$edit_url.'" '.$style.'>';
	echo $time;
	echo ($item->title?$item->title:'???');
	echo (!empty($item->place)?" @":"").$item->place;
	if (!$IS_PUBLIC) echo "</a>\n";
	echo '</div>';
}

$cal_colors = array("#990000"=>"Dark Red", "#CC0000"=>"Red", "#000099"=>"Deep Blue", "#0000CC"=>"Blue", 
					"#006600"=>"Dark Green", "#00CC00"=>"Green", "#9900FF"=>"Purple", "#00CCCC"=>"Cyan",
					"#CC6600"=>"Orange", "#CCCC00"=>"Yellow", "#CC00CC"=>"Magenta", "black"=>"No Color");
$cal_bg_colors = array("#990000"=>"#BB9999", "#CC0000"=>"#FFBBBB", "#000099"=>"#9999CC", "#0000CC"=>"#BBBBFF", 
					"#006600"=>"#99EE99", "#00CC00"=>"#CCFFCC", "#9900FF"=>"#BB99FF", "#00CCCC"=>"#CCFFFF",
					"#CC6600"=>"#FFCCAA", "#CCCC00"=>"#FFFFBB", "#CC00CC"=>"#FFBBFF", ""=>"white");

?>
