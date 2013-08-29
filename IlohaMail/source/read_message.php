<?php
/////////////////////////////////////////////////////////
//	
//	source/read_message.php
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
	FILE: source/read_message.php
	PURPOSE:
		1.  Display important message headers
		2.  Display message structure (i.e. attachments, multi-parts)
		3.  Display message body (text, images, etc)
		4.  Provide interfaces to delete/undelete or move messages
		5.  Provide interface to view/download message parts (i.e. attachments)
		6.  Provide interface to forward/reply to message
	PRE-CONDITIONS:
		$user - Session ID
		$folder - Folder in which message to open is in
		$id - Message ID (not UID)
		[$part] - IMAP (or MIME?) part code to view.
	COMMENTS:
		This message should interpret and display mime messages correctly.
		Since it is my goal to make this file as RFC822 compliant as possible, please
		notify me for any violations or errors.
		
********************************************************/

include("../include/super2global.inc");
include("../include/header_main.inc");
include("../include/icl.inc");
include("../include/mime.inc");
include("../include/cache.inc");
include("../include/stopwatch.inc");
include('../include/read_message_lib.inc');
include('themes/'.$my_prefs['theme'].'/info.inc');
if ($DISPLAY_LIB['read_message']){
	include('themes/'.$my_prefs['theme'].'/'.$DISPLAY_LIB['read_message']);
}else{
	include('themes/default/disp_lib_rm.php');
}


$clock = new stopwatch(true);
$clock->register("start");

	//read main context
	$MAIN_CONTEXT = cache_read($loginID, $host, "main.ctx");
	$folder = $MAIN_CONTEXT["f"];
	$sort_field = $MAIN_CONTEXT["s"];
	$sort_order = $MAIN_CONTEXT["o"];
	$num_msgs = $MAIN_CONTEXT["n"];
	$start = $MAIN_CONTEXT["t"];
	$thread_view = ($my_prefs['thread_view'] && $ICL_CAPABILITY['threads'] && strcasecmp($sort_field,"date")!==false && !$search && empty($MAIN_CONTEXT['q']));
	echo "<!--\n";
	echo "folder: $folder \nsort_field: $sort_field \nsort_order: $sort_order \nnum_msgs: $num_msgs \nstart: $start \n";
	echo "ids: ".$MAIN_CONTEXT["i"]."\n";
	print_r($MAIN_CONTEXT);
	echo "//-->\n"; flush();
	if (!$MAIN_CONTEXT) echo $CACHE_ERROR;
	
	//make sure folder is specified
	if (empty($folder)){
		echo "Folder not specified or invalid<br></body></html>";
		exit;
	}else{
		$folder_ulr = urlencode($folder);
	}
	
	//make sure message id is specified
	if (empty($id)){
		echo "Invalid or unspecified message id<br>\n";
		echo "</body></html>";
		exit;
	}

	//connect to mail server
	$conn=iil_Connect($host, $loginID, $password, $AUTH_MODE);
	if (!$conn){
		echo "<p>Failed to connect to mail server: $iil_error<br></body</html>";
		exit;
	}
	
	//actions (flagging, deleting, moving, etc)
	if ($my_prefs['action_target']=='next'||$my_prefs['action_target']=='prev'){
		$clock->register('begin action');
		include_once("../include/main.inc");

		$submit = main_get_submit();
	
		//handle actions
		if ($submit){
			include('../include/main_actions.inc');
		}
		
		//update $num_msgs
		$num_msgs = iil_C_CountMessages($conn, $folder);
		$clock->register('end action');
	}
	
	//let's have some totally useless code
	$this_folder=$folder;
	$folder=$this_folder;
	if ($undelete){
		iil_C_Undelete($conn, $folder, $id);
	}
	
	//include lang modules
	include("../lang/".$my_prefs["lang"]."defaultFolders.inc");
	include("../lang/".$my_prefs["lang"]."read_message.inc");
	include("../lang/".$my_prefs["lang"]."main.inc");
	
	
	//get message info
	$header = iil_C_FetchHeader($conn, $folder, $id);
	//check UID for consistency
	if (is_numeric($uid) && $header->uid!=$uid){
		$id = iil_C_UIDToMID($conn, $folder, $uid);
		echo "\n<!-- mid uid mismatch.  new id: $id, uid: $uid //-->\n"; flush();
		$header = iil_C_FetchHeader($conn, $folder, $id);
	}

	//load up header into
	$structure_str=iil_C_FetchStructureString($conn, $folder, $id); 
	echo "\n<!-- ".$structure_str."-->\n"; flush();
	$structure=iml_GetRawStructureArray($structure_str);
	$num_parts=iml_GetNumParts($structure, $part);
	$parent_type=iml_GetPartTypeCode($structure, $part);
	$uid = $header->uid;
	$tid = false;
	
	//get some structure info
	if (($parent_type==1) && ($num_parts==1)){
		$part = 1;
		$num_parts=iml_GetNumParts($structure, $part);
		$parent_type=iml_GetPartTypeCode($structure, $part);
	}
	
	//flag as seen, if not traversing (i.e. using prev/next links)
	if ((!$trav) || (!$my_prefs["nav_no_flag"])){
		//flag as read
		iil_C_Flag($conn, $folder, $id, "SEEN");
		//reload folder list to refresh num unread
		if (($ICL_CAPABILITY["folders"]) && ($my_prefs["list_folders"]) && ($my_prefs["showNumUnread"])){
			echo "\n<script language=\"JavaScript\">\n";
			echo "parent.list1.location=\"folders.php?user=".$user."\";\n";
			echo "</script>\n";
		}
	}
	
	//if thread mode, fetch thread data
	if ($thread_view){
		$clock->register('load thread');
		include_once("../include/main.inc");
		echo '<!--';
		$thread_data = main_fetchThreads($folder, $index_a, $num_msgs, $sort_field, $sort_order);
		$t_tree = $thread_data['t'];
		$t_index = $thread_data['i'];
		$t_uids = $thread_data['u'];
		$tid=$t_index[$id];
		if (count($t_tree[$tid])<=1) $tid=false;
		$num_msgs = count($t_tree);
		//print_r($t_tree);
		//print_r($t_index);
		//print_r($thread_data);
		echo 'id: '.$id.' tid: '.$tid."\n";
		echo '//-->';
		$clock->register('thread loaded');
	}else if (function_exists('iil_C_FetchUIDs')){
		$t_uids = iil_C_FetchUIDs($conn, $folder);
	}
	
	//generate next/previous links
	$next_link = "";
	$prev_link = "";
	$prev_img = "<b>&lt;-</b>";
	$next_img = "<b>-&gt;</b>";
	
	if ($my_prefs['showNav'] && $thread_view){
		$t_id = $t_index[$id];
		echo '<!-- t_id: '.$t_id.' //-->'."\n";
		if ($t_id>0){
			list($k,$prev_id)=each($t_tree[$t_id-1]);
		}
		if ($t_tree[$t_id+1]){
			list($k,$next_id)=each($t_tree[$t_id+1]);
		}
		$current_position_id = $t_id + 1;
	}else if (($my_prefs["showNav"]) && (isset($sort_field)) && (isset($sort_order))){
		//fetch index
		if ($MAIN_CONTEXT["c"] && $MAIN_CONTEXT["i"]){
			//stored in main context, index_a is mid=>sort_value
			$temp_a = explode(",",$MAIN_CONTEXT["i"]);
			foreach($temp_a as $key=>$temp_id) $index_a[$temp_id] = $key;
			$num_msgs = count($temp_a);
		}else if (!is_array($index_a)){	
			//attempt to read from cache
			include_once("../include/main.inc");
			$read_cache = false;
			$index_a = main_ReadCache($folder, "1:".$num_msgs, $sort_field, $read_cache);
			
			//if read form cache fails, go to server					
			if (!$read_cache) $index_a=iil_C_FetchHeaderIndex($conn, $folder, "1:".$num_msgs, $sort_field);
		}

		
		if ($index_a !== false){
			//sort index
			if (strcasecmp($sort_order, "ASC")==0) asort($index_a);
			else if (strcasecmp($sort_order, "DESC")==0) arsort($index_a);
			
			//generate array where key is continuous and data contains message indices
			$count = 0;
			while ( list($index_id, $blah) = each($index_a) ){
				$table[$count] = $index_id;
				$count++;
			}
			
			//look for current message
			$current_key = array_search($id, $table);
			$prev_id = $table[$current_key-1];
			$next_id = $table[$current_key+1];
			$current_position_id = $current_key+1;
		}else if ($sort_field=="DATE"){
			//if indexing failed, and ordered by date, just use id
			if ($sort_order=="DESC"){
				$prev_id = $id + 1;
				$next_id = $id - 1;
				if ($prev_id > $num_msgs) $prev_id = -1;
				$current_position_id = $num_msgs - $id;
			}else if ($sort_order=="ASC"){
				$prev_id = $id - 1;
				$next_id = $id + 1;
				if ($next_id > $num_msgs) $next_id = -1;
				$current_position_id = $id;
			}
		}		
	}
	if (is_array($t_uids)){
		if ($next_id) $next_uid = $t_uids[$next_id];
		if ($prev_id) $prev_uid = $t_uids[$prev_id];
	}
	if ($prev_id > 0)
		$prev_link = '<a href="'.rm_format_nav_url($prev_id, $prev_uid).'" class=mainHeading>'.$prev_img.'</a>';
		
	if ($next_id > 0)
		$next_link = '<a href="'.rm_format_nav_url($next_id, $next_uid).'" class=mainHeading>'.$next_img.'</a>';
		
	
	$params["next_url"] = $next_url;
	$params["prev_url"] = $prev_url;
	$params["back_url"] = ($my_prefs["view_inside"]?"main.php?user=$user":"");
	if ($my_prefs["js_usage"]=="h"){
		echo "<script>\n";
		echo "a = ".js_print_array($params,true).";\n";
		echo "D([0,a]);\n";
		echo "</script>\n";
	}
	
	//determine if there are multiple recipients (or recipients other than self)
	//this, in turn, determines whether or not to show the "reply all" link
	if ((!empty($header->cc)) || (substr_count($header->to, "@") > 1)){
		$multiple_recipients = true;
	}else if (empty($header->replyto) && substr_count($header->to, "@")==1){
		$multiple_recipients = true;
		
		$to_a = LangParseAddressList($header->to);
		$to_address = $to_a[0]["address"];
		
		if (!empty($my_prefs["email"]) && strcasecmp($to_address, $my_prefs["email"])==0){
			//one recipient, main address
			$multiple_recipients = false;
		}else{
			//one recipient.  check if known address for user
			include_once("../include/data_manager.inc");
			$dm = new DataManager_obj;
			if ($dm->initialize($loginID, $host, $DB_IDENTITIES_TABLE, $DB_TYPE)){
				$identities_a = $dm->read();
				if (is_array($identities_a)){
					reset($identities_a);
					while ( list($k, $v) = each($identities_a) ){
						$v = $identities_a[$k];
						if (strcasecmp($v["email"], $to_address)==0
							|| strcasecmp($v["replyto"], $to_address)==0){
								$multiple_recipients = false;
						}
					}
				}
			}
		}
	}else $multiple_recipients = false;

				//some stuff needed for toolbar
				$folder_url = urlencode($folder);
				if ($my_prefs["compose_inside"]) $target="list2";
				else $target="scr".$user.$folder_url.$id;

				//figure out who's supposed to handle the following actions
				$action_url = 'main.php?user='.$user.'&folder='.$folder_url;
				if ($my_prefs['action_target']=='next' && $next_link) $action_url = rm_format_nav_url($next_id,$next_uid);
				if ($my_prefs['action_target']=='prev' && $prev_link) $action_url = rm_format_nav_url($prev_id,$prev_uid);

	//show folder bar
	$folder_name = $defaults[$folder];
	if (empty($folder_name)){
		$delim = iil_C_GetHierarchyDelimiter($conn);
		$pos = strrpos($folder, $delim);
		if ($pos!==false) $pos++;
		$folder_name = substr($folder, $pos);
	}
	echo '';
	echo "\n".'<table width="100%" cellpadding=2 cellspacing=0>';
	echo '<tr class="dk">'."\n";
	echo '<td align=left valign="middle" class=mainLight>'."\n";
		//show folder
		$folder_name = rm_get_foldername($folder);
		echo theme_form_link(THEME_RM_FOLDER, $folder_name, ($my_prefs['view_inside']?$params['back_url']:''));						
		
		//show current position
		echo '&nbsp;(#'.$current_position_id.' of '.$num_msgs.')&nbsp;';
		
		//show nav links
		$nav_text = rm_get_navtext($sort_field,$sort_order,$rmStrings);
		if ($prev_link){
			echo theme_form_link(THEME_RM_PREV_LINK, $nav_text['previous'], rm_format_nav_url($prev_id,$prev_uid), ''); //'<a href="'.rm_format_nav_url($prev_id,$prev_uid).'" class="mainLight">&lt; Newer</a>';
			echo theme_form_link(THEME_RM_DIVIDER, '');
		}
		if ($my_prefs['view_inside']) echo theme_form_link(THEME_RM_BACK_LINK, $nav_text['back'], $params['back_url']);
		if ($next_link){
			echo theme_form_link(THEME_RM_DIVIDER, '');
			echo theme_form_link(THEME_RM_NEXT_LINK, $nav_text['next'], rm_format_nav_url($next_id,$next_uid), ''); //'<a href="'.rm_format_nav_url($next_id,$next_uid).'" class="mainLight">Older &gt;</a>';
		}
	echo '</td>';
	echo '<td align="right" valign="top" class=mainLight>';
		//show move form
		$move_form_params = array('user'=>$user, 'folder'=>$folder, 'action_url'=>$action_url, 'id'=>$id, 
								'uid'=>$uid,'num_msgs'=>$num_msgs,'start'=>$start,'tid'=>$tid,
								'move_str'=>$rmStrings[5], 'move_thread_str'=>$rmStrings[24]);
		if ($tid!==false){
			$t_uid = '';
			$t_tree_kids = $t_tree[$tid];
			foreach($t_tree_kids as $t_child_id) $t_uid.=($t_uid?',':'').$t_uids[$t_child_id];
			$move_form_params['t_uid'] = $t_uid;
		}
		echo rm_show_moveform($move_form_params);
	echo '</td></tr></table>'."\n";


	//show toolbar
	//include("../include/read_message_tools.inc");
	echo "<table width=\"100%\" class=\"md\">\n";
	flush();				
	
	//show subject
	echo "\n<!-- SUBJECT //-->\n";
	echo "<table width=\"100%\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" class=\"dkbg\">\n";
	echo "<tr class=\"dkbg\">";
	echo "<td valign=\"top\">\n";
		echo "\n<span class=\"bigTitle\"><b>".encodeUTFSafeHTML(LangDecodeSubject($header->subject, $my_prefs["charset"]))."</b>";
	echo "</td><td>";
	/***** THREAD NAVIGATOR *****/
	if ($thread_view){		
		$t_pos = $t_index[$id];
		$t_peers = $t_tree[$t_pos];
		
		if ($t_peers){
			$t_pindex = array();
			$t_i = 0;
			foreach($t_peers as $t_peer){
				$t_pindex[$t_i] = $t_peer;
				if ($t_peer==$id) $t_pos = $t_i;
				$t_i++;
			}
		}
		
		$t_pc=count($t_peers);
		if ($t_pc>1){
			echo '<span style="float:right;text-align:center;">';
			echo '<span class=mainLight>'.str_replace("%message", ($t_pos+1).'/'.count($t_peers), $rmStrings[23]).'<br>';
			echo '<nobr>';
			if ($t_pos>0){
				echo theme_form_link(THEME_RM_TH_FIRST_LINK, '', rm_format_nav_url($t_pindex[0]));
				echo theme_form_link(THEME_RM_TH_PREV_LINK, '', rm_format_nav_url($t_pindex[$t_pos-1]));
			}else{
				echo theme_form_link(THEME_RM_TH_FIRST_LINK,'');
				echo theme_form_link(THEME_RM_TH_PREV_LINK,'');
			}
			echo '&nbsp;|&nbsp;';
			if ($t_pindex[$t_pos+1]){
				echo theme_form_link(THEME_RM_TH_NEXT_LINK, '', rm_format_nav_url($t_pindex[$t_pos+1]));
				echo theme_form_link(THEME_RM_TH_LAST_LINK, '', rm_format_nav_url($t_pindex[$t_pc-1]));
			}else{
				echo theme_form_link(THEME_RM_TH_NEXT_LINK,'');
				echo theme_form_link(THEME_RM_TH_LAST_LINK,'');
			}
			echo '</nobr>';
			echo '</span>';
		}
	}
	echo "</td>";
	echo "</tr>\n";
	echo "</table>\n\n";
	//https://ssl.ilohamail.org/devmail/read_message.php?user=1104799993-83853&trav=1&id=642&checkboxes[]=641&uids[]=1611&submit=Delete&start=0
	//show header
	echo "\n<!-- HEADER //-->\n";
	echo "<table width=\"100%\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" class=\"dk\">\n";
	echo "<tr class=\"header\"><td valign=\"top\">\n";
		echo "<b>".$mainStrings[9].":  </b>".encodeUTFSafeHTML($header->date)."<br>\n"; 
	echo "</td></tr>\n";
	echo "<tr class=\"header\"><td valign=\"top\">\n";
		echo "<b>".$mainStrings[8].":  </b>".LangDecodeAddressList($header->from,  $my_prefs["charset"], $user)."\n";
		if (!empty($header->replyto) && (strcmp($header->from,$header->replyto)!==0))
			echo "&nbsp;&nbsp;&nbsp;<b>Reply-To:  </b>".LangDecodeAddressList($header->replyto,  $my_prefs["charset"], $user)."<br>\n";
	echo "</td></tr>\n";
	echo "<tr class=\"header\"><td valign=\"top\">\n";
		echo "<b>".$mainStrings[7].": </b>".LangDecodeAddressList($header->to,  $my_prefs["charset"], $user)."<br>\n";
	echo "</td></tr>\n";
	if (!empty($header->cc)){
		echo "<tr class=\"header\"><td valign=\"top\">\n";
		echo "<b>CC: </b>".LangDecodeAddressList($header->cc,  $my_prefs["charset"], $user)."<br>\n";
		echo "</td></tr>\n";
	}
		
	//show attachments/parts
	if ($num_parts > 0){
		echo "<tr class=\"header\"><td valign=\"top\">\n";
		echo "<b>".$rmStrings[6].": </b>\n";
		echo "<table size=100%><tr valign=top><tr>\n";
		//echo "<td valign=\"top\"><b>".$rmStrings[6].": </b>\n";
		echo "<td></td>\n";
		echo "<td valign=\"top\"><b>&nbsp;&nbsp;&nbsp;&nbsp;</b></td>\n";
		$icons_a = array("text.gif", "multi.gif", "multi.gif", "application.gif", "music.gif", "image.gif", "movie.gif", "unknown.gif");

		for ($i=1;$i<=$num_parts;$i++){
			//get attachment info
			if ($parent_type == 1)
				$code=$part.(empty($part)?"":".").$i;
			else if ($parent_type == 2){
				$code=$part.(empty($part)?"":".").$i;
				//echo implode(" ", iml_GetPartArray($structure, $code));
			}
				
			$type=iml_GetPartTypeCode($structure, $code);
			$name=iml_GetPartName($structure, $code);
			$typestring=iml_GetPartTypeString($structure,$code);
			list($dummy, $subtype) = explode("/", $typestring);
			$bytes=iml_GetPartSize($structure,$code);
			$encoding=iml_GetPartEncodingCode($structure, $code);
			$disposition = iml_GetPartDisposition($structure, $code);
		
			//format href
			if (($type == 1) || ($type==2) || (($type==3)&&(strcasecmp($subtype, "ms-tnef")==0))) $href = "read_message.php?user=$user&folder=$folder_url&id=$id&part=".$code;
			else $href = "view.php?user=$user&folder=$folder_url&id=$id&part=".$code;
			
			//show icon, file name, size
			echo "<td align=\"center\">";
			echo "<a href=\"".$href."\" ".(($type==1)||($type==2)||(($type==3)&&(strcasecmp($subtype, "ms-tnef")==0))?"":"target=_blank").">";
			echo "<img src=\"themes/".$my_prefs["theme"]."/images/".$icons_a[$type]."\" border=0><br>";
			echo "<span class=\"small\">";
			if (is_string($name)) echo htmlspecialchars(LangDecodeSubject($name, $my_charset));
			if ($bytes>0) echo "<br>[".ShowBytes($bytes)."]";
			if (is_string($typestring)) echo "<br>".htmlspecialchars($typestring);
			echo "</span>";
			echo "</a>";
			echo "</td>\n";
			if (($i % 4) == 0) echo "</tr><tr><td></td><td></td>";
		}
		echo "</tr>\n</table>\n";
		echo "</td></tr>\n";
	}
	
	//more header stuff (source/header links)
	echo "<tr class=\"toolbar\"><td valign=\"top\">\n";
	echo '<b>'.$rmStrings[22].': </b>';
				$compose_url = "compose2.php?user=$user&folder=$folder_url&id=$id&uid=$uid&part=$part";
				$compose_url.= ($override_cs?"&override_cs=".$override_cs:"");

				//show reply link
				echo theme_form_link(THEME_RM_REPLY_LINK, $rmStrings[1], $compose_url.'&replyto=1', $target);
				echo theme_form_link(THEME_RM_DIVIDER,'');
				
				//show reply to all link
				if ($multiple_recipients){
					echo theme_form_link(THEME_RM_REPLYALL_LINK, $rmStrings[14], $compose_url.'&replyto=1&replyto_all=1', $target);
					echo theme_form_link(THEME_RM_DIVIDER,'');
				}

				//show forward link
				echo theme_form_link(THEME_RM_FORWARD_LINK, $rmStrings[2], $compose_url.'&forward=1', $target);
				echo theme_form_link(THEME_RM_DIVIDER, '');
								
				//show delete/undelete link
				if ($header->deleted){
					$undelete_url = "read_message.php?user=$user&folder=$folder_url&id=$id&undelete=1&start=$start";
					echo theme_form_link(THEME_RM_UNDELETE_LINK, $rmStrings[3], $undelete_url);
 				}else {					
					echo "\n<script type=text/javascript language=JavaScript1.2>\n ";
					echo "if (window.name!='list2') window.name=\"$target\";\n";
						$delete_url = $action_url.'&checkboxes[]='.$id.'&uids[]='.$uid.'&submit=Delete&start='.$start;
 				    echo "document.write(\"".addslashes(theme_form_link(THEME_RM_DELETE_LINK, $rmStrings[4], $delete_url))."\");\n";
					 echo "</script>\n";
 				    echo "<noscript>\n";
						 echo theme_form_link(THEME_RM_DELETE_LINK, $rmStrings[4], $delete_url);
					echo "</noscript>\n";
 				}
				 
				echo theme_form_link(THEME_RM_DIVIDER,'');

				//show read/unread link
				$new_flag = 'Unread';
				$new_flag_str_id = 11;
				if (($trav) && ($my_prefs["nav_no_flag"]) && !$header->seen){
					$new_flag = 'Read';
					$new_flag_str_id = 20;
				}
				echo theme_form_link(($new_flag=='Unread'?THEME_RM_UNREAD_LINK:THEME_RM_READ_LINK), $rmStrings[$new_flag_str_id], 
								"$action_url&checkboxes[]=$id&uids[]=$uid&submit=$new_flag&start=$start");
				

	echo '&nbsp;&nbsp;&nbsp;<b>'.$rmStrings[21].': </b>';
	echo theme_form_link(THEME_RM_SOURCE_LINK, $rmStrings[9], "view.php?user=$user&folder=$folder_url&id=$id&source=1", '_blank');
	echo theme_form_link(THEME_RM_DIVIDER,'');
	echo theme_form_link(THEME_RM_HEADER_LINK, $rmStrings[12], "view.php?user=$user&folder=$folder_url&id=$id&show_header=1", '_blank');
	echo theme_form_link(THEME_RM_DIVIDER,'');
	echo theme_form_link(THEME_RM_PRINTABLE_LINK, $rmStrings[16], "view.php?user=$user&folder=$folder_url&id=$id&printer_friendly=1", '_blank');
	if ($report_spam_to){
		echo "&nbsp;|&nbsp;<a href=\"compose2.php?user=$user&folder=$folder_url&forward=1&id=$id&show_header=1&to=".urlencode($report_spam_to);
		echo "\" target=\"_blank\">".$rmStrings[13]."</a>\n";
	}
	if ($header->answered){
		if ($my_prefs["sent_box_name"]){
			$search_reply_url = "main.php?user=$user&folder=".urlencode($my_prefs["sent_box_name"]);
			$search_reply_url.= "&start=$start&sort_field=$sort_field&sort_order=$sort_order";
			$search_reply_url.= "&search_criteria=".urlencode("ALL HEADER IN-REPLY-TO ".$header->messageID);
			$replied_str = "<a href=\"$search_reply_url\">".$rmStrings[15]."</a>";
		}else{
			$replied_str = $rmStrings[15];
		}
		echo "&nbsp;|&nbsp;$replied_str\n";
	}
	echo "</td></tr>\n";


	echo '<tr class="lt"><td>'."\n";
		
	echo "<table width=\"90%\" align=\"left\" border=\"0\" cellpadding=\"5\"><tr><td>\n";
	echo "\n<!-- BEGIN MESSAGE CELL //-->\n";
	
	
	/***** BEGIN READ MESSAGE HANDLER ****/
	
	//now include the handler that determines what to display and how	
	include("../include/read_message_handler.inc");	
	
	/***** END READ MESSAGE HANDLER *****/
	
	echo "\n<!-- END MESSAGE CELL //-->\n";
	echo "</td></tr></table>\n";
	
	echo "</td></tr></table>\n";
	echo "</td></tr></table>\n";

	//show toolbar
	include("../include/read_message_tools.inc");


	iil_Close($conn);

?>
</BODY></HTML>
<!--
<?php
$clock->register("stop");
$clock->dump();
?>
//-->