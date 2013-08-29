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

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE: source/main.php
	PURPOSE:
		1.  List specified number of messages in specified order from given folder.
		2.  Provide interface to read messages (link subjects to source/read_message.php)
		3.  Provide interface to send messasge to senders (link "From" field to source/compose.php)
		4.  Provide interface to move or delete messages
		5.  Provide interface to view messages not currently listed.
		6.  Provide functionality to move, delete messages and expunge folders.
	PRE-CONDITIONS:
		$user - Session ID
		$folder - Folder name
		[$sort_field] - Field to sort by {"subject", "from", "to", "size", "date"}
		[$sort_order] - Order, "ASC" or "DESC"
		[$start] - Show specified number of messages starting with this index

********************************************************/

$exec_start_time = microtime();
//include('../include/optimizer.inc');
include('../include/stopwatch.inc');
$clock = new stopwatch(true);

$clock->register('start');

include('../include/super2global.inc');
$clock->register('pre-header');
include('../include/header_main.inc');
$clock->register('post-header');
include('../include/ryosdates.inc');
include('../include/icl.inc');
include('../include/main.inc');
include('../include/cache.inc');

include('../lang/'.$my_prefs['lang'].'defaultFolders.inc');
include('../lang/'.$my_prefs['lang'].'main.inc');
include('../lang/'.$my_prefs['lang'].'dates.inc');

$disp_lib_path = 'themes/default/disp_lib_main.php';
if ($DISPLAY_LIB['main']) $disp_lib_path = 'themes/'.$my_prefs['theme'].'/'.$DISPLAY_LIB['main'];
include($disp_lib_path);

$clock->register('includes done');

/////////////////
//	LOAD CORE
//		The main_core file processes actions, 
//		creates message indices, and fetches headers
/////////////////

include('../include/main_core.inc');


////////////////////
//	OUTPUT 
////////////////////
	echo "<!-- Messages: $messages_str got ".count($headers)." headers ".count($index_a)." indexed //-->\n";


	/*  start form */
	echo "\n".'<form name="messages" method="POST" action="main.php">'."\n";
	echo '<input type="hidden" name="post_total_num" value="'.$total_num.'">'."\n";
	echo '<input type="hidden" name="js_action" value="">'."\n";

	/* Show folder name, num messages, page selection pop-up */
	
	if ($headers==false) $headers=array();
	echo '<table width="100%" cellpadding=2 cellspacing=0><tr class="dk">'."\n";
	echo "<td align=left valign=bottom>\n";
		$disp_folderName = $defaults[$folder];
		if (empty($disp_folderName)){
			$disp_folderName = $folder;
			if ($my_prefs["rootdir"] && iil_StartsWith($disp_folderName, $my_prefs["rootdir"])){
				$rtdir_len = strlen($my_prefs["rootdir"]);
				if ($my_prefs["rootdir"][$len-1]!='.' || $my_prefs["rootdir"][$len-1]!="/") $rtdir_len++;
				$disp_folderName = substr($disp_folderName, $rtdir_len);
			}
		}
		if (empty($search)){
			echo theme_form_link(THEME_MAIN_FOLDER, iil_utf7_decode($disp_folderName));
		}
		echo '<span class="mainHeadingSmall">'."\n";
		if (strcasecmp("INBOX", $folder)==0){
			echo theme_form_link(THEME_MAIN_NEW, $mainStrings[17], "main.php?user=$user&folder=$folder&cmc=1");
			echo theme_form_link(THEME_MAIN_DIVIDER, '');
		}
        if (strcmp($folder,$my_prefs["trash_name"])!=0){
			echo theme_form_link(THEME_MAIN_DELETE, $mainStrings[18], "main.php?user=$user&folder=$folder&delete_all=1");
			echo theme_form_link(THEME_MAIN_DIVIDER, '');
		}
		if (count($filters_a)>0){
			echo theme_form_link(THEME_MAIN_APPLY, $mainStrings[24], "main.php?user=$user&folder=$folder&apply_filter=all");
		}
		echo "</span>\n";
	echo "</td>\n";
	echo "<td align=\"right\" valign=\"bottom\" class=\"mainHeadingSmall\">";
		//show quota
		if ($my_prefs["show_quota"]=="m"){
			$quota = iil_C_GetQuota($conn);
			include("../lang/".$my_prefs["lang"]."quota.inc");
			if ($quota) echo $quotaStr["label"].LangInsertStringsFromAK($quotaStr["full"], $quota);
			else echo $quotaStr["label"].$quotaStr["unknown"];
		}
	echo "</td>\n";
	echo "</tr></table>";
	
	/* Confirm "delete all" request */
	if ($delete_all==1){
		echo "<p>".str_replace("%f", $folder, $mainErrors[7]);
		echo "<span class=\"small\">[<a href=\"main.php?user=$user&folder=$folder&delete_all=2&delete_all_num=$total_num&submit=Delete\">";
			echo $mainStrings[18]."</a>]</span>";
		echo "<span class=\"small\">[<a href=\"main.php?user=$user&folder=$folder\">".$mainStrings[19]."</a>]</span>";
	}
	
	
	/* Show error messages, and reports */
	if (!empty($error)) echo "<p><center><span style=\"color: red\">$error</span></center>";
	//if ((empty($error)) && (empty($report))) echo "<p>";


	$c_date["day"]=GetCurrentDay();
	$c_date["month"]=GetCurrentMonth();
	$c_date["year"]=GetCurrentYear();

	if (count($headers)>0) {
		if (!isset($start)) $start=0;
		$i=0;

		if (sizeof($headers)>0){			
			/*  show num msgs and any notices */
			echo '<table width="100%"><tr>';
			echo '<td valign=bottom align="left"><span class="mainLightSmall">';

			echo str_replace("%p", ($num_show>$total_num?$total_num:$num_show), str_replace("%n", $total_num, $mainStrings[0]))."&nbsp;";
			
			echo '</span</td>';
			echo '<td align=center><span class="mainLightSmall">';
			if (!empty($report)) echo $report;
			echo '</span></td>'."\n";
			echo '<td valign=bottom align=right class="mainLightSmall">';
			//page controls
			$num_items=$total_num;
			if ($num_items > $num_show){
				if ($prev_start < $start){
					$prev_args = "&start=".$prev_start;
					echo theme_form_link(THEME_MAIN_PREVIOUS, $mainStrings[2]." $num_show".$mainStrings[3], "main.php?user=$sid&folder=".urlencode($folder).$prev_args);
				}
				if (($prev_start < $start) && ($next_start<$num_items))
					echo theme_form_link(THEME_MAIN_DIVIDER,'');
				if ($next_start<$num_items){
					$num_next_str = $num_show;
					if (($num_items - $next_start) < $num_show) $num_next_str = $num_items - $next_start;
					$next_args = "&start=".$next_start;
					echo theme_form_link(THEME_MAIN_NEXT, $mainStrings[4]." $num_next_str".$mainStrings[5], "main.php?user=$sid&folder=".urlencode($folder).$next_args);
				}

				echo '<select name=start class="small">'."\n";
					main_showPageOptions($total_num, $num_show, $start);
				echo '</select>';
				echo '<input type=submit value="'.$mainStrings[16].'">';
				
			}
			echo "</td>\n";
			echo "</tr></table>\n";

			$clock->register("pre list");

			/***
			Show tool bar
			***/
			if (strpos($my_prefs["main_toolbar"], "t")!==false){
				include("../include/main_tools.inc");
			}

			
			/* main list */
			$num_cols = strlen($my_prefs["main_cols"]);
			echo "\n<!-- MAIN LIST //-->\n";
			echo '<table width="100%" border="0" cellspacing="1" cellpadding="1" class="md">'."\n";
			
			/* Show filters/search tool bar */
			if ($my_prefs["show_qsbar"]){
			echo '<tr class="dk"><td colspan="'.strlen($my_prefs["main_cols"]).'">'."\n";
				echo '<table width="100%" border="0" cellspacing="0" cellpadding="0">'."\n";
				echo "<tr>\n";
				echo '<td width="50%" class=mainLightSmall>'."\n";
				if (is_array($filters_a) && count($filters_a)>0){
					//echo '<a href="pref_filters.php?user='.$user.'" class=mainLightSmall>'.$mainStrings[27].' </a>';
					echo theme_form_link(THEME_MAIN_FILTERS, $mainStrings[27], 'pref_filters.php?user='.$user);
					echo "\n".'<select name="apply_filter">'."\n";
					echo '<option value="">--'."\n";
					echo '<option value="all">'.$mainStrings[28]."\n";
					reset($filters_a);
					while ( list($filter_id, $v) = each($filters_a) ){
						if (!ereg("[d]", $v["flags"]))
							echo "<option value=\"".$filter_id."\">".$v["name"]."\n";	//add one to filter_id so 0 (i.e. false) becomes 1
					}
					echo "</select>\n";
					echo '<input type="submit" name="do_apply_filter" value="'.$mainStrings[29].'">';
				}
				echo "</td>\n";
				echo "<td align=\"right\" width=\"50%\" class=mainLightSmall>\n";
				echo '<span class=mainLightSmall>'.$mainStrings[30].'</span>';
				if ($quick_search_str) $quick_search_str = stripslashes($quick_search_str);
				echo '<input type="text" name="quick_search_str" value="'.$quick_search_str.'" size=15 class="small">';
				echo '<input type="submit" name="do_quick_search" value="'.$mainStrings[31].'" >';
				echo '</td>'."\n";
				echo '<tr>'."\n";
				echo '</table>'."\n";
			echo '</td></tr>'."\n";
			}

			/* message list table headers */
			echo '<tr class="dk">'."\n";
				$check_link="<SCRIPT type=\"text/javascript\" language=JavaScript1.2><!-- Make old browsers think this is a comment.\n";
				$check_link.="document.write(\"<a href=javascript:SelectAllMessages(true) class='tblheader'><b>+</b></a><span class=tblheader>|</span><a href=javascript:SelectAllMessages(false) class=tblheader><b>-</b></a>\")";
				$check_link.="\n--></SCRIPT><NOSCRIPT>";
				$check_link.='<a href="main.php?folder='.urlencode($folder).'&check_all=1"><b>+</b></a>|';
 				$check_link.='<a href="main.php?folder='.urlencode($folder).'&uncheck_all=1"><b>-</b></a>';
				$check_link.='</NOSCRIPT>';
				$tbl_header["c"] = "\n<td>$check_link</td>";
				$tbl_header["s"] = "\n<td>".FormFieldHeader('subject', $mainStrings[6]).'</td>';
				if ($showto)
					$tbl_header["f"] = "\n<td>".FormFieldHeader("to", $fromheading).'</td>';
				else
					$tbl_header["f"] = "\n<td>".FormFieldHeader("from", $fromheading).'</td>';
				$tbl_header["d"] = "\n<td>".FormFieldHeader("date", $mainStrings[9]).'</td>';
				$tbl_header["z"] = "\n<td>".FormFieldHeader("size", $mainStrings[14]).'</td>';
				$tbl_header["a"] = '<td><img src="themes/'.$my_prefs["theme"].'/images/att.gif"></td>';
				$tbl_header["m"] = '<td><img src="themes/'.$my_prefs["theme"].'/images/reply.gif"></td>';
				for ($i=0;$i<$num_cols;$i++) echo $tbl_header[$my_prefs["main_cols"][$i]];
			echo "\n</tr>\n";
			
			/* message listing preparations... */
			$display_i=0;
			$prev_id = "";
			
			if ($is_draft_box){
				$row_params['args'] = 'user='.$user.'&draft=1&folder='.urlencode($folder); 
				$row_params['action'] = 'compose2.php'; 
				$row_params['open_tgt'] = ($my_prefs['compose_inside']?'list2':'_blank');
			}else{
				$row_params['args'] = "user=$user"; 
				$row_params['action'] = 'read_message.php'; 
				$row_params['open_tgt'] = ($my_prefs['view_inside']!=1?"scr".$user.urlencode($folder).$id:"list2");
			}
			$row_params['next_args'] = $next_args;
			$row_params['prev_args'] = $prev_args;
			$row_params['main_cols'] = $my_prefs['main_cols'];
			$row_params['num_cols'] = strlen($my_prefs['main_cols']);
			$row_params['theme'] = $my_prefs['theme'];
			$row_params['charset'] = $my_prefs['charset'];
			$row_params['user'] = $user;
			$row_params['com_tgt'] = ($my_prefs['compose_inside']?'list2':'_blank');
			$row_params['hilite_color'] = '#ddddff';

			if ($my_prefs['js_usage']=="h"){
				echo "<script>\n";
				echo "D([0,".js_print_array($row_params, true)."]);\n";
			}
			
			/*	show message listing */
			while (list ($key,$val) = each ($headers)) {
				$header = $headers[$key];				
				$row = main_packageHeader($folder, $header, $t_num_kids, $showto, $selected_boxes);
				if ($row===false) continue;
				$display_i++;
				if ($my_prefs['js_usage']=="h"){
					echo main_DisplayJSRow($row);
					if (($display_i%15==0)) echo 'top.flush_buffer();</script>'."\n".'<script>'."\n"; 
				}else{
					main_DisplayRow($row, $row_params);
				}
				$displayed_mids[] = $header->id;
				flush();
				
				$i++;
			}
			
			if ($my_prefs["js_usage"]=="h"){
				echo "parent.flush_buffer();\n";
				echo "var fin=parent.getMS();\n";
				echo "</script>";
			}
			echo "</table>";
			
			flush();
			
			$clock->register("post list: $i");
			
			if (is_array($displayed_mids))
				echo "<input type=\"hidden\" name=\"displayed_set\" value=\"".implode(",",$displayed_mids)."\">\n";
			echo "<input type=\"hidden\" name=\"user\" value=\"$user\">\n";

			if (isset($search)) echo "<input type=hidden name=search_done value=1>\n";
			echo "<input type=\"hidden\" name=\"max_messages\" value=\"".$display_i."\">\n";
			
			/***
			Show tool bar
			***/
			if (strpos($my_prefs["main_toolbar"], "b")!==false){
				$clock->register("pre tools include");
				include("../include/main_tools.inc");
				$clock->register("post tools include");
			}
			
			echo "</form>\n";
			
			if (($folder=="INBOX")&&($ICL_CAPABILITY["radar"])){
				/*** THIS JavaScript code does NOT run reliably!! ***/
				echo "\n<script language=\"JavaScript\">\n";
				echo "if (parent.radar)";
				echo "  parent.radar.location=\"radar.php?user=".$user."\";\n";
				echo "</script>\n";
			}
			if (($ICL_CAPABILITY["folders"]) && ($my_prefs["list_folders"]) && ($my_prefs["showNumUnread"]) && ($reload_folders)){
				echo "\n<script language=\"JavaScript\">\n";
				echo "parent.list1.location=\"folders.php?user=".$user."\";\n";
				echo "</script>\n";
			}
		}else{
			if (!empty($search)) echo "<p><center>".$mainErrors[0]."</center>";
			else echo "<p><center><span class=mainLight>".$mainErrors[1]."<br>".$conn->error."</span></center>";
		}
	}else{
		if (!empty($search)) echo "<p><center><span class=mainLight>".$mainErrors[0]."</span></center>";
		else echo "<p><center><span class=mainLight>".$mainErrors[1]."<br>".$conn->error."</span></center>";
	}
	
	
	
	iil_Close($conn);
	
	$clock->register("pre write ctx");
	main_contextSave($MAIN_CONTEXT, $OLD_CONTEXT);

	/*
	$MAIN_CONTEXT["n"] = $total_num_msgs;
	if ($do_quick_search && $quick_search_str){
		$MAIN_CONTEXT["c"] = "q";
		$MAIN_CONTEXT["q"] = $quick_search_str;
		if (is_array($displayed_mids)){
			$MAIN_CONTEXT['n'] = count($displayed_mids);
			sort($displayed_mids);
			$MAIN_CONTEXT["i"] = (is_array($displayed_mids)?implode(",",$displayed_mids):"");
		}else{
			$MAIN_CONTEXT['n'] = 0;
			$MAIN_CONTEXT["i"] = '';
		}
	}
	if ($search_criteria && $original_search_results){
		$MAIN_CONTEXT["c"] = "s";
		$MAIN_CONTEXT["q"] = $search_criteria;
		$MAIN_CONTEXT["i"] = $original_search_results;
	}
	if (is_array($OLD_CONTEXT)){
		$mc_dirty = false;
		reset($MAIN_CONTEXT);
		while(list($mc_key,$new_data)=each($MAIN_CONTEXT)){
			if ($MAIN_CONTEXT[$mc_key]!=$OLD_CONTEXT[$mc_key]){
				$mc_dirty = true;
				break;
			}
		}
	}else $mc_dirty = true;
	
	if ($mc_dirty) cache_write($loginID, $host, "main.ctx", $MAIN_CONTEXT);
	*/
	$clock->register("post write ctx: ".$mc_dirty);
	
flush();
$clock->register("done");
$exec_finish_time = microtime();
echo "\n<!--\n";
$clock->dump();
echo "\n//-->\n";
?>
<div id="jsdebug"></div>
<script>
//var p_fin = top.getMS();
//document.write("Page took: "+(p_fin-p_start)+" ms.");
</script>
</BODY></HTML>
