<?php
/////////////////////////////////////////////////////////
//	
//	source/calendar.php
//
//	(C)Copyright 2002-2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE:  source/compose.php
	PURPOSE:
		Display calendar...
	
********************************************************/
include("../include/stopwatch.inc");
	
$timer = new stopwatch(true);
$timer->register("start");

$override_cs = 'UTF-8';
include("../include/super2global.inc");
include("../include/header_main.inc");
include("../lang/".$my_prefs["lang"]."dates.inc");
include("../lang/".$my_prefs["lang"]."calendar.inc");
include("../include/icl.inc");
include("../include/version.inc");
include("../conf/defaults.inc");
include("../include/calendar.inc");

//make sure feature is not disabled
if ($DISABLE_CALENDAR){
	echo $calStr["disabled"];
	echo "</body></html>\n";
	exit;
}

//include for add/edit
if (!$IS_PUBLIC && ($edit_cal || $delete_cal)){
	include("../include/edit_calendar.inc");
}

echo "<center>\n";

if (($go_month > 0) && ($go_year > 0)) $date = formatCalDate($go_month, 1, $go_year);

if (empty($date)) $date = date("Ymd", time());
if (strlen($date)==8){
	$current_year = substr($date, 0, 4);
	$current_month = substr($date, 4, 2);
	$current_day = substr($date, 6, 2);
}
if (!isset($current_ts)) $current_ts = mktime(0, 0, 0, $current_month, $current_day, $current_year);

//what's today's day of week?
$dow = date("w", $current_ts);

$disp_mode = $my_prefs['disp_mode'];
$week_start = $my_prefs['week_start'];
if (!isset($disp_mode)) $disp_mode = 0;
if (!isset($week_start)) $week_start = 0; // Default: Sunday
if ($week_start %7 == 1) $disp_mode=1; 
//0: begin on sunday
//1: begin on monday
//2: begin 3 days ago
//3: begin today
if ($disp_mode==0) $offset = $dow;
else if ($disp_mode==1) $offset = ($dow?$dow:7)-1;
else if ($disp_mode==2) $offset = 3;
else if ($disp_mode==3) $offset = 0;

//time stamp for last sunday (beginning of week)
$start_ts = $current_ts - ($offset * (60 * 60 * 24));

//time stamp for next saturday (end of week)
$end_ts = $start_ts + (60 * 60 * 24 * 6);

//which week of the month?
$end_day = date("d", $end_ts);
$wom = (int)($end_day / 7);
if (($end_day % 7) != 0) $wom++;

//starting date
$start_date = date("Ymd", $start_ts);
$end_date = date("Ymd", $end_ts);

//create hash tables for day-of-week <-> date
for ($i=0; $i<7; $i++){
	$temp_ts = $start_ts + ($i * (60 * 60 * 24));
	$temp_date = date("Ymd", $temp_ts);
	$temp_dow = date("w", $temp_ts);
	$date_dow[$temp_date] = "d".$temp_dow;		//date to day-of-week lookup
	$dow_date["d".$temp_dow] = $temp_date;		//day-of-week to date lookup
	$schedule2[$temp_date]["dow"] = $temp_dow;
}

$owner_sql = "userID='$session_dataID'";
if ($IS_PUBLIC) $owner_sql.= ' and publish=\'1\'';
else if (!$DISABLE_CALENDAR_SHARING) $owner_sql.= ' or shared=1';
$sql = 'SELECT * FROM '.$DB_CALENDAR_TABLE.' WHERE (('.$owner_sql.')';
$sql.= "and ((beginDate<='$end_date' and endDate>='$start_date') ";
$sql.= "or (endDate<='$end_date' and endDate>='$start_date')))";
echo '<!-- '.$sql.' //-->'."\n";

$backend_result = false;

if ($backend!="FS"){
	include_once("../conf/db_conf.php");
    include_once("../include/idba.$DB_TYPE.inc");
	
	if (!isset($db)) $db = new idba_obj;
	if ($db->connect()){
		$backend_result = $db->query($sql);
	}else{
		$backend_result = false;
	}
}
if ($backend_result){
	if ($db->num_rows($backend_result) > 0){
		//initialize schedule array
		//$schedule = array();
		
		//echo $backend_num_rows($backend_result)." rows";
		
		//loop through records
		while( $a = $db->fetch_row($backend_result) ){
			//create & initialize new scheduleItem object
			$item = new scheduleItem;
			$item->id = $a["id"];
			$item->title = $a["title"];
			$item->place = $a["place"];
			$item->description = $a["description"];
			$item->participants = $a["participants"];
			$item->beginTime = $a["beginTime"];
			$item->endTime = $a["endTime"];
			$item->color = $a["color"];
			$item->shared = $a["shared"];
			$item->allday = $a['allday'];
						
			$pattern = $a["pattern"];
			$beginDate = $a["beginDate"];
			$endDate = $a["endDate"];
			
			//non-repeating event
			//if (strpos($pattern, "d:")===false){
			$pattern = chop($pattern);
			if (empty($pattern)){
				
				if ($beginDate == $endDate){
				//single day event
					$schedule2[$beginDate][$item->beginTime][] = $item;
				}else{
				//multi-day event
					if (($beginDate>=$start_date) && ($beginDate <= $end_date)){
						//insert event for first day (event ends at midnight)
						$temp_item = $item;
						$temp_item->endTime = 2400;
						$schedule2[$beginDate][$item->beginTime][]=$temp_item;					}
					if (($endDate<=$end_date) && ($endDate >= $start_date)){
						//insert event for last day (event beings at midnight)
						$temp_item = $item;
						$temp_item->beginTime = 0;
						$schedule2[$endDate][0][]=$temp_item;
					}
					
					if (($endDate-$beginDate)>1){
						//if event spans more than two days, insert events for all days inbetween
						
						if (!empty($date_dow[$beginDate])) $fromD = $date_dow[$beginDate][1]+1;
						else $fromD = 0;
						if (!empty($date_dow[$endDate])) $untilD = $date_dow[$endDate][1];
						else $untilD = 7;
						
						reset($date_dow);
						while ( list($k,$v) = each($date_dow) ){
							if (($k > $beginDate) && ($k < $endDate)){
								$temp_item = $item;
								$temp_item->beginTime = 0;
								$temp_item->endTime = 2400;
								$schedule2[$k][0][]=$temp_item;
							}
						}
						
					}
				}
			}else{
			//repeating event
				//check if this week is included in repetition
				if ((strpos($pattern,"w:all")!==false) || (strpos($pattern, "w".$wom)!==false)){
					$words = explode(" ", $pattern);
					while( list($k,$w)=each($words) ) if ($w[0]=="d") $dowpat=$w;
					$dowpat = substr($dowpat, 2);
					$dows = explode(",", $dowpat);
					while( list($k, $d)=each($dows) ){
						if (($dow_date["d".$d]>=$start_date) && ($dow_date["d".$d]<=$end_date)){
							$schedule2[$dow_date["d".$d]][$item->beginTime][]=$item;
						}
					}
				}else if (false !== ($pos=strpos($pattern, "m:"))){
					$repeat_day = substr($pattern, $pos+2, 2);
					$repeat_date = $current_year.$current_month.$repeat_day;
					if (($repeat_date >= $start_date) && ($repeat_date <= $end_date))
						$schedule2[$repeat_date][$item->beginTime][]=$item;
				}else if (false !== ($pos=strpos($pattern, "y:"))){
					$repeat_date = substr($pattern, $pos+2, 4);
					$repeat_date = $current_year.$repeat_date;
					if (($repeat_date >= $start_date) && ($repeat_date <= $end_date))
						$schedule2[$repeat_date][$item->beginTime][]=$item;					
				}
			}
		}

	}else{
		echo $error;
	}
	
	echo '<!--'."\n";
	//print_r($schedule2);
	echo '//-->'."\n";

	if (is_array($schedule2)){
		$start_date_a = getdate($start_ts);
		$end_date_a = getdate($end_ts);
		
		$last_ts = $current_ts - (60 * 60 * 24 * 7);
		$next_ts = $current_ts + (60 * 60 * 24 * 7);
		
		$base_time = cal_getbasetime(&$schedule2);
		cal_padt4($base_time);
		echo '<!-- base time: '.$base_time.' //-->';
		
		echo '<table width="60%" cellspacing=0 cellpadding=0>'."\n";
		echo '<tr class="dk">'."\n";
		echo '<td width="15%" align="left">';
			echo '<a class=mainHeading href="calendar.php?user='.$user.'&date='.date("Ymd",$last_ts).'&mode='.$mode.'">&lt;&lt;</a>';
		echo "</td>\n";
		echo '<td width="70%" align="center">';
			echo '<span class="bigTitle">';
			if ($IS_PUBLIC) echo str_replace('%name', $my_prefs['user_name'], $calStr['name']).'<br>';
			$wdate_a = array("m"=>$lang_datetime["short_mon"][$start_date_a["mon"]], "d"=>$start_date_a["mday"], "y"=>$start_date_a["year"]);
			$wdate_str = LangInsertStringsFromAK($lang_datetime["verbal"], $wdate_a);
			echo str_replace("%d", $wdate_str, $calStr["weekof"]);
			echo "</span>\n";
		echo "</td>\n";
		echo '<td width="15%" align="right">';
				echo '<a class=mainHeading href="calendar.php?user='.$user.'&date='.date("Ymd",$next_ts).'&mode='.$mode.'">&gt;&gt;</a>';
		echo "</td>\n";
		echo "</table>\n";
		
		$dsow = array("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat");
		
		$boxurl = 'calendar.php?user='.$user.'&date='.$date.'&scale='.$scale.'&format=boxed';
		$listurl = 'calendar.php?user='.$user.'&date='.$date.'&scale='.$scale.'&format=list';
		echo '<span class="mainLight">';
		echo $calStr['format'];
		echo '&nbsp;<a href="'.$boxurl.'" class="rmnav">'.($format=='boxed'?'<b>':'').$calStr['boxed'].($format=='boxed'?'</b>':'').'</a>';
		echo '&nbsp;|&nbsp;';
		echo '<a href="'.$listurl.'" class="rmnav">'.($format=='list'?'<b>':'').$calStr['list'].($format=='list'?'</b>':'').'</a>';
		echo '</span>';
		
		//display weekly schedule
		echo "\n<p>\n";
		echo '<table width="98%" border="0" cellspacing="0" cellpadding="1" class="dk">';
		
		//show tabl header -days of week and dates
		echo "\n<tr>\n";
		reset($date_dow);
		while ( list($k,$d)=each($date_dow) ){
			$tbh_month = (int)substr($k, 4, 2);
			$tbh_day = (int)substr($k, 6, 2);
			$tbh_a = array("m"=>$lang_datetime["short_mon"][$tbh_month], "d"=>$tbh_day);
			$tbh_str = LangInsertStringsFromAK($lang_datetime["verbal_short"], $tbh_a);
			echo '<td align="center" class="dk" width="14%">';
			if (!$IS_PUBLIC) echo '<a href="edit_calendar.php?user='.$user.'&date='.$k.'&edit=" class=mainLight>'."\n";
			else echo '<span class="mainLight">';
			echo ($k==$date?"<b>":"").$lang_datetime["dsow"][$d[1]]."<br>".$tbh_str.($k==$date?"</b>":"");
			echo '</a>';
			echo "</td>\n";
		}
		echo "</tr>\n";
		
		//show schedule
		echo "<tr>\n";
		$num_displayed = 0;
		reset($schedule2);
		while (list($k,$schedules)=each($schedule2)){
			if (empty($k)) continue;
			
			echo '<td align="left" valign="top" class="lt" width="14%"><span class="small">';
			
			$schedules = $schedule2[$k];
			
			$prev_end = $base_time;
			$cur_y = 0;
				
			ksort($schedules);
			reset($schedules);
			while(list($start, $blah) = each($schedules)){
				if (strcmp($start,"date")==0) continue;
				if (!is_array($schedules[$start])) continue;

				while ( list($k2, $item)=each($schedules[$start]) ){
					if ($format=='list') cal_showItem($item);
					else cal_showItemBox($item, $k2, $base_time, $prev_end, $cur_y);

					$num_displayed++;
					$prev_end = $item->endTime;
				}
			}
			echo "</span></td>\n";
		}//while
	}
	echo "</tr>\n";
	if ($num_displayed==0){
		echo '<tr>';
		echo '<td colspan="7" class="lt" align="center">';
		if ($IS_PUBLIC) echo $calStr['no schedules public'];
		else echo $calStr['no schedules'];
		echo '</td>';
		echo '</tr>';
	}
	echo "</table>\n";
	if (!$IS_PUBLIC && !$DISABLE_CALENDAR_SHARING){
		$public_url = getPublicURL($dataID).'&date='.$date;
		$public_url = '<a href="calendar.php?user=_'.$dataID.'&date='.$date.'" class="rmnav">'.$public_url.'</a>';
		echo '<span class="mainLight">'.str_replace('%url', $public_url, $calStr['public url']).'</span>';
	}
}//if ($backend_result)


//default values for month/year
$month = (int)$current_month;
$year = (int)$current_year;
$prev_month = ($month>1?$month-1:12);
$prev_year = ($month==1?$year-1:$year);
$next_month = ($month<12?$month+1:1);
$next_year = ($month==12?$year+1:$year);

echo "<p>\n";
echo '<table cellspacing=0 cellpadding=0><tr class="dk">'."\n";
echo '<td valign="middle">';
echo "<a class=mainHeading href=\"calendar.php?user=$user&date=".formatCalDate($current_month, $current_day, $current_year-1)."\">&nbsp;&lt;&lt;&nbsp;</a>\n";
echo "<a class=mainHeading href=\"calendar.php?user=$user&date=".formatCalDate($prev_month, $current_day, $prev_year)."\">&nbsp;&lt;&nbsp;</a>\n";
echo "</td>";
?>
<td class=mainHeading valign=bottom>
<form method="POST" action="calendar.php">
	<input type="hidden" name="user" value="<?php echo $user ?>">
	<select name="go_month">
	<?php
	for ($i=1;$i<=12;$i++) echo '<option value="'.$i.'" '.($i==$current_month?'SELECTED':'').'>'.$lang_months[$i]."\n";
	?>
	</select>
	<select name="go_year">
	<?php
	for ($i=-5;$i<=10;$i++){
		$go_year = $current_year + $i;
		echo '<option value="'.$go_year.'" '.($go_year==$current_year?'SELECTED':'').'>'.$go_year."\n";
	}
	?>
	</select>
	<input type="submit" name="go" value="<?php echo $calStr["go"]?>">
</form>
</td>
<?php
echo '<td valign="middle">';
echo "<a class=mainHeading href=\"calendar.php?user=$user&date=".formatCalDate($next_month, $current_day, $next_year)."\">&nbsp;&gt;&nbsp;</a>\n";
echo "<a class=mainHeading href=\"calendar.php?user=$user&date=".formatCalDate($current_month, $current_day, $current_year+1)."\">&nbsp;&gt;&gt;&nbsp;</a>\n";
echo '</td>';
echo '</tr></table>'."\n";
echo '<p><table wdith="95%" cellspacing="10"><tr>'."\n";
echo '<td valign="top">';
$month = $prev_month;
$year = $prev_year;
include('../include/display_monthly_calendar.inc');
echo '</td><td valign="top">';
$month = (int)$current_month;
$year = $current_year;
include('../include/display_monthly_calendar.inc');
echo '</td><td valign="top">';
$month = $next_month;
$year = $next_year;
include('../include/display_monthly_calendar.inc');
echo '</td>';
echo '</tr></table>';

if ($IS_PUBLIC){
	echo '<p><span class="mainLight">Software by <a href="http://ilohamail.org" class="rmnav">IlohaMail.org</a></span>';
}

echo '</center>';

?>
</body></html>
<!--
<?php
$timer->register("done");
$timer->dump();
?>
//-->
