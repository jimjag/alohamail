<?php
/////////////////////////////////////////////////////////
//	
//	include/main.inc
//
//	(C)Copyright 2000-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

function FormFieldHeader($p_sort_field, $p_string){

	global $user, $folder, $start, $sort_field, $sort_order, $textc;

	$p_user = $user;
	$p_folder = $folder;
	$p_start = $start;
	$p_cur_field = $sort_field;
	$p_cur_order = $sort_order;
	$p_color = $textc;
	
	$result="main.php?";
	$result.="user=$p_user";
	//$result.="&folder=".urlencode($p_folder);
	//$result.="&start=$p_start";
	if (strcasecmp($p_sort_field, $p_cur_field)==0){
		if ($p_cur_order=="ASC") $p_sort_order="DESC";
		else if ($p_cur_order=="DESC") $p_sort_order="ASC";
	}else{
		$result.="&sort_field=$p_sort_field";
		if (strcasecmp($p_sort_field, "date")==0) $p_sort_order="DESC";
		else $p_sort_order="ASC";
	}
	$result.="&sort_order=$p_sort_order";
	//$result ="<a href=\"".$result."\"><b><span class=\"tblheader\">$p_string</span></b></a>";
	$result ="<a href=\"".$result."\" class=\"tblheader\"><b>$p_string</b></a>";
	
	return $result;
}

function ShowFieldControls($field, $base_url, $num, $total){
	$total--;
	
	$result = "<td align=\"center\">";
	if ($num!=0){
		$result.= "<a href=\"".$base_url."&move_col=$field&move_direction=left\"><span class=\"tblheader\">&lt;&lt;</span></a>";
		$result.= "&nbsp;&nbsp;";
	}
	if ($num!=$total){
		$result.= "<a href=\"".$base_url."&move_col=$field&move_direction=right\"><span class=\"tblheader\">&gt;&gt;</span></a>";
		$result.= "</td>\n";
	}
	
	return $result;
}

function main_UpdateCache($folder, $cache_end, $need_end, $sort_field, &$read_cache){
	global $loginID, $host, $conn, $main_firstmod_id;


	//read what we have...
	$data = cache_read($loginID, $host, $folder.".".$sort_field);
	if (!is_array($data)) return false;
	
	//remove bad data
	for($i=$cache_end+1;$i<$need_end;$i++) unset($data[$i]);
	
	//read in new stuff
	if ($need_end>$cache_end){
		//fetch difference
		$set = $cache_end.':'.$need_end;
		$new_a = iil_C_FetchHeaderIndex($conn, $folder, $set, $sort_field);
		if (!is_array($new_a)) return false;
		
		//echo "<!-- new stuff: ".implode(",",$new_a)." //-->\n";
	
		//echo '<!--'."\n";
		//echo 'New '.$cache_end.': '.$new_a[$cache_end]."\n";
		//echo 'Old '.$cache_end.': '.$data[$cache_end]."\n";
		//echo '//-->'."\n";
		
		//check consistency
		if ($new_a[$cache_end]!=$data[$cache_end]) return false;
			
		//echo "<!-- merging new stuff //-->\n";
	
		//remove overlap
		unset($new_a[$cache_end]);
	
		//merge
		//echo "<!--";
		foreach($new_a as $id=>$val){
			$data[$id] = $val;
			//echo 'Update: '.$id."\n";
		}
		//echo "//-->\n";
	}
	
	//unset unneccessary items off the top
	//echo "<!--\n";
	while(isset($data[++$need_end])){
		unset($data[$need_end]);
		//echo 'Unset: '.$need_end."\n";
	}
	//echo "\n//-->\n";

	//echo "<!--\n";
	//print_r($new_a);
	//echo $data[$cache_end+1]."\n";
	//echo "//-->\n";
	
	return $data;
}

function  main_firstmod($a){
	global $main_firstmod_id;

	if (!is_array($a)) return;
	
	//echo "<!-- Main first mod running \n";
	//if ($main_firstmod_id===false) echo "It's false\n";
	foreach($a as $cand){
		if (empty($cand)) continue;
		if (($cand<$main_firstmod_id) || ($main_firstmod_id===false)){
			$main_firstmod_id=$cand;
			//echo "set to $cand \n";
		}
		//echo $cand.',';
	}
	//echo "\n//-->\n";
	if ($main_firstmod_id!==false) $main_firstmod_id--;
}

function main_ReadCache($folder, $messages_str, $sort_field, &$read_cache){
	global $loginID, $host, $conn, $main_firstmod_id;
	global $clock;
	$read_cache = false;
	
	$msgset = cache_read($loginID, $host, $folder.".".$sort_field.".MSGS");
	if (!$msgset) return false;
	else if ($msgset == $messages_str){
		//if same message set, likely hit, so read
		$data = cache_read($loginID, $host, $folder.".".$sort_field);
		if ($data){
			//check number of elements 
			list($start_mid,$end_mid)=explode(':',$messages_str);
			if (is_numeric($start_mid) && is_numeric($end_mid))
				if (count($data)!=($end_mid-$start_mid+1)){
					$clock->register('mrc: '.count($data).' '.$messages_str.' '.$end_mid.' '.$start_mid);
					return false;
				}
			//otherwise, assume we're good
			$read_cache = true;
			return $data;
		}
	}else{
		//cache is in inconsistent state
		if (!ereg("[0-9]:[0-9]",$msgset)) return false;
		if (!ereg("[0-9]:[0-9]",$messages_str)) return false;
		
		list($cache_start,$cache_end)=explode(':',$msgset);
		list($need_start,$need_end)=explode(':',$messages_str);
		
		//echo "<!-- cache_end: $cache_end need_end: $need_end //-->\n";
		
		if ($cache_end<$need_end){
			//more items than are in cache
			return main_UpdateCache($folder, $cache_end, $need_end,  $sort_field, $read_cache);
		}else if ($cache_end>$need_end){
			//less items than are in cache
			if (is_numeric($main_firstmod_id) && $main_firstmod_id!==false){
				//we know where cache likely got dirtied
				return main_UpdateCache($folder, $main_firstmod_id, $need_end, $sort_field, $read_cache);
			}
		}
		
	}
	
	return false;
}

function main_WriteCache($folder, $sort_field, $index_array, $messages_str){
	global $loginID, $host;
	$read_cache = false;
	$key = $folder.".".$sort_field;

	if (cache_write($loginID, $host, $key.".MSGS", $messages_str)){
		return cache_write($loginID, $host, $key, $index_array);
	}
	
	return false;
}

function main_fetch_sort(&$conn, $folder, $sort_field, $sort_order){
	// The SORT command doesn't do a very good job of decoding
	// encoded strings and addresses, which potentially leads to
	// poor results.  So, for the following fields, we'll use 
	// IlohaMail's index-based sorting, which is slower but more
	// accurate.
	if (strcasecmp($sort_field,'subject')==0) return false;
	else if (strcasecmp($sort_field,'to')==0) return false;
	else if (strcasecmp($sort_field,'from')==0) return false;
	
	//try sort command
	$a = iil_C_Sort($conn, $folder, $sort_field);
	if (!is_array($a)){
		echo '<!-- sort failed: '.$conn->error.' //-->'."\n";
		return false;
	}else{
		echo '<!-- sort worked //-->'."\n";
	}
	
	//we need to convert the results to a format compatible with $index_a
	//that is, key=mid, value=sort data
	$out = array();
	if ($sort_order=='ASC'){
		reset($a);
		while(list($k,$v)=each($a)) $out[$v]=$k;
	}else{
		end($a);
		while(current($a)){
			$out[current($a)] = key($a);
			prev($a);
		}
	}
	
	return $out;
}

function main_fetch_index(&$conn, $folder, &$messages_str, &$params){
	global $clock;
	
	if (empty($messages_str)) return array();
	
	$sort_field = $params['sf'];
	$sort_order = $params['so'];	
	$thread_view = $params['tv'];
	$start = $params['st'];
	$next_start = $params['nxs'];
	$total_num = $params['tn'];
	$num_show = $params['ns'];
	
	//attempt ot read from cache
	$read_cache = false;
	$index_a = main_ReadCache($folder, $messages_str, $sort_field, $read_cache);
		
	//if there are "recent" messages, ignore cache
	if (iil_C_CheckForRecent($conn, $folder)>0) $read_cache = false;
	
	//if not read from cache, go to server
	if (!$read_cache && $index_a===false){
		//try to use SORT command
		$index_a=main_fetch_sort($conn, $folder, $sort_field, $sort_order);
		
		if ($index_a){
			//sort command works
			$sorted = true;
		}else{
			// Sort command failed, decide if we need to fetch index. 
			// Indexing is inefficient and potentially very slow for large
			// mailboxes, and sorting by MIDs is a decent alternative.
			// But we need to index in the following cases:
			//	 -there aren't many messages (might as well index)
			//	 -not sorting by internaldate (MID isn't equivalent)
			//	 -threading is enabled (we need the index)
			//	 -not viewing INBOX (in other folders, INTERNALDATE!=date received)
			$fetch_index = false;
			if ($total_num<500) $fetch_index = true; 
			else if ($sort_field!='INTERNALDATE') $fetch_index = true; 
			else if ($thread_view) $fetch_index = true; 
			else if (strcasecmp($folder,'inbox')!=0) $fetch_index = true; 
			
			if ($fetch_index){
				$index_a=iil_C_FetchHeaderIndex($conn, $folder, $messages_str, $sort_field);
			}else{
				//If sorting by INTERNALDATE, using MIDs should be more or
				//less equivalent to actually using INTERNALDATE.
				//So, if there are many messages, we'll forget indexing and just
				//go by MIDs for better performance.
				echo '<!-- screw index, we\'ll just go with MIDs //-->'."\n";
				$index_a=false;
			}
		}
			
		if ($index_a) main_WriteCache($folder, $sort_field, $index_a, $messages_str);
		if ($clock&&$index_a) $clock->register("post index: no cache ".($sorted?'SORT':'FHI'));
	}else{
		if ($clock) $clock->register("post index: cache");
	}
		
	if ($index_a===false){
		//index failed... if sorting by date, just use MIDs
		if (strcasecmp($sort_field,"date")==0||strcasecmp($sort_field,'INTERNALDATE')==0){
			if (strcasecmp($sort_order, "ASC")==0){
				$messages_str = $start.":".($start + $num_show);
			}else{
				$messages_str = ($total_num - $start - $num_show);
				$messages_str.= ":".($total_num - $start);
			}
		}
	}else{
		//got index, sort
		if (!$sorted){
			if (strcasecmp($sort_order, "ASC")==0) asort($index_a);
			else if (strcasecmp($sort_order, "DESC")==0) arsort($index_a);
		}
		
		//if not threading, re-adjust list of messages to get
		if (!$thread_view){
			$i=0;
			foreach($index_a as $key=>$val){
				if (($i >= $start) && ($i < $next_start)) $id_a[$i]=$key;
				$i++;
			}
			if (is_array($id_a)) $messages_str=implode(",", $id_a);
		}else{
		//if thread view, we're not using $messages_str, so do nothing
		}
	}
	return $index_a;
}

function main_QScmp($qs_str, $haystack){
	if (eregi("^regx:", $qs_str)){
		return eregi(substr($qs_str, 5), $haystack);
	}else{
		return stristr($haystack, $qs_str);
	}
}

function main_doQuickSearch(&$conn, $folder, $showto, $query){
	global $my_prefs;
	
	$hits = array();

	//fetch total number of messages in folder
	$total_num=iil_C_CountMessages($conn, $folder);
	
	//fetch all subjects
	$subjects = iil_C_FetchHeaderIndex($conn, $folder, '1:'.$total_num, 'SUBJECT');
	
	//search for entries matching query
	if (is_array($subjects)){
		foreach($subjects as $mid=>$subject){
			$subject = LangDecodeSubject($subject, $my_prefs["charset"]);
			if (main_QScmp($query, $subject)) $hits[$mid] = 1;
		}
	}
	
	//fetch appropriate address field
	if ($showto) $addrs = iil_C_FetchHeaderIndex($conn, $folder, '1:'.$total_num, 'TO', false);
	else $addrs = iil_C_FetchHeaderIndex($conn, $folder, '1:'.$total_num, 'FROM', false);
	
	//look for hits
	if (is_array($addrs)){
		foreach($addrs as $mid=>$addr_str){
			$str = main_nested_implode(',',LangParseAddressList($addr_str, $my_prefs["charset"]));
			if (main_QScmp($query, $str)) $hits[$mid] = 1;
		}
	}

	//extract matching mids (we don't stick mids into an array to start with to avoid dups)
	$mids = array();
	foreach($hits as $mid=>$in){
		if ($in) $mids[] = $mid;
	}
	
	//update total num, implode mids into csv
	$total_num = count($mids);
	$messages_str = implode(',',$mids);
	
	
	return array($total_num,$messages_str,'');
}

function main_ThreadTraverse(&$t_tree, $message_id){
	//echo "got $message_id\n";
	$message_set = $t_tree[$message_id]["id"];
	
	//go through my kids
	if (is_array($t_tree[$message_id]["kids"]) && count($t_tree[$message_id]["kids"])>=1){
		//add kids to result set
		//echo "$message_id has ".count($t_tree[$message_id]["kids"])." kids\n";
		reset($t_tree[$message_id]["kids"]);
		while(list($n,$kid)=each($t_tree[$message_id]["kids"])){
			$message_set.=($message_set?",":"").main_ThreadTraverse($t_tree, $kid);
		}
	}
	
	return $message_set;
}

function main_Thread(&$t_tree, $message_id){
	$parent_id = $message_id;
	while($t_tree[$parent_id]["parent"]){
		echo $parent_id." -> ".$t_tree[$parent_id]["parent"]."\n"; flush();
		$parent_id = $t_tree[$parent_id]["parent"];
	}
	echo "Found root: ".$parent_id."\n";flush();
	
	if ($parent_id!=$message_id)
		return main_ThreadTraverse(&$t_tree, $parent_id);
	else
		return "";
}

function main_DisplayRow($row, &$params){
	$num_cols=$params['num_cols'];
	$main_cols = $params['main_cols'];
	
	$class=($row['sn']?'unseen':'seen');
	if ($row['fl']) $class.=' important';

	echo "\n<tr class=\"$class\">\n";
	for ($i=0;$i<$num_cols;$i++){
		if ($main_cols[$i]=='s' && $row["is_reply"])
			echo '<td style="padding-left:30px">';
		else
			echo '<td '.($row["is_reply"]?'class="subthread"':'').'>';
			
		//echo $row[$main_cols[$i]];
		$e = $row[$main_cols[$i]];
		switch ($main_cols[$i]){
			case "s":
				$url = $params['action'].'?'.$params['args'].'&id='.$e['id'].'&uid='.$e['uid'];
				echo '<a href="'.$url.'" '.($params['open_tgt']?'target="'.$params['open_tgt'].'"':'').' '.($e['c']?'class='.$e['c']:"").">";
				echo ($e['r']?'<b>':'').$e['sbj'].($e['r']?'</b>':'');
				echo "</a>";
				break;
			case "f":
				echo LangDisplayAddressList($e, $params["charset"], $params["user"], $row["is_reply"]);
				break;
			case "d":
				echo '<nobr>'.$e.'&nbsp;</nobr>';
				break;
			case "z":
				echo '<nobr>'.$e.'</nobr>';
				break;
			case "a":
				if ($e) echo '<img src="themes/'.$params['theme'].'/images/att.gif">';
				break;
			case "c":
				echo '<input type="checkbox" name="checkboxes[]" value='.$e["id"];
				echo ($e['checked']?' CHECKE':'').'>';
				break;
			case "m":
				echo ($e["d"]?"D":"");
				echo ($e["a"]?'<img src="themes/'.$params['theme'].'/images/reply.gif">':'&nbsp;');
				echo ($e["f"]?'<span class="tcnt"><b>!</b></span>':'');
				break;
		}
		echo "</td>\n";
	}
	echo "</tr>\n";
	flush();
}

function main_DisplayJSRow($row){
	$out = 'D([1,[';
	$out.= js_print_array($row['s']).',';
	$out.= main_jsenc_ads($row['f']).',';
	$out.= '"'.addslashes($row['d']).'",';
	$out.= '"'.$row['z'].'",';
	$out.= $row['a'].',';
	$out.= js_print_array($row['c']).',';
	$out.= js_print_array($row['m']).',';
	$out.= $row['is_reply'].',';
	$out.= (empty($row['nc'])?'0':$row['nc']).',';
	$out.= $row['sn'].','.$row['fl'];
	$out.= ']]);'."\n";
	return $out;
}

function main_packageHeader($folder, &$header, &$t_num_kids, $showto, &$selected_boxes){
	global $my_prefs, $user, $lang_datetime, $mainStrings;
	
	$id = $header->id;
	$seen = ($header->seen?"Y":"N");
	$deleted = ($header->deleted&&strcmp($folder,$my_prefs['trash_name'])!=0?"D":"");
	if ($id==0 || $deleted) return false;
	
	$qs_match = false;
			
	//form subject
	$subject=trim(chop($header->subject));
	if (empty($subject)) $subject=$mainStrings[15];
	else $subject=encodeUTFSafeHTML(LangDecodeSubject($subject, $my_prefs["charset"]));

	$row["s"]["id"] = $id;
	$row["s"]["uid"] = $header->uid;
	$row["s"]["t"] = ($my_prefs['view_inside']!=1?"\"scr".$user.urlencode($folder).$id."\"":"");
	$row["s"]["c"] = $header->is_reply?'mainlt':'';
	$row["s"]['sbj']= $subject;
	$row['s']['r'] = ($seen=='Y'?0:1);
			
	//form sender||recipient
	if ($showto) $addr_str = $header->to;
	else $addr_str = $header->from;
	$row["f"] = LangParseAddressList($addr_str, $my_prefs["charset"]);

	//form date/time
	$timestamp = $header->timestamp;
	$timestamp = $timestamp + ((float)$my_prefs["timezone"] * (float)3600);
	$row["d"] = ShowShortDate($timestamp, $lang_datetime);

	//form size
	$row["z"] = ShowBytes($header->size);

	//attachments?
	$row["a"] = preg_match("/multipart\/m/i", $header->ctype)?1:0;

	//form checkbox
	$row["c"]['id'] = $id;
	$row["c"]['checked'] = (isset($check_all)?1:0);
	$row["c"]['checked'] = (is_array($selected_boxes) && in_array($id, $selected_boxes)?1:0);
	$row['c']['uid'] = $header->uid;

	//form flags
	$row["m"]["d"] = ($header->deleted?1:0);
	$row["m"]["a"] = ($header->answered?1:0);
	$row["m"]["f"] = (stristr($header->flags, "Flagged")?1:0);
			
	$row['is_reply'] = ($header->is_reply?1:0);
	$row['nc'] = $t_num_kids[$id];
	$row['sn'] = $row['s']['r'];
	$row['fl'] = $row['m']['f'];
	
	return $row;
}


function main_jsenc_ads($ads){
	$out = "";
	foreach($ads as $ad){
		$ad['name'] = trim(chop(str_replace("\n", "", $ad['name'])));
		$ad['address'] = trim(chop(str_replace("\n", "", $ad['address'])));
		$out.=($out?',':'').'["'.addslashes($ad['name']).'","'.addslashes($ad['address']).'"]';
	}
	return '['.$out.']';
}


function main_nested_implode($d,$a){
	$out="";
	while(list($k,$val)=each($a)){
		$out.=($out?$d:"");
		if (is_array($a[$k])) $out.=main_nested_implode($d, $a[$k]);
		else $out.=$val;
	}
	return $out;
}

function main_showPageDateOptions($total, $num, $start, &$index){
	global $sort_order;
	
	if (strcasecmp($sort_order,'asc')==0) $asc = true;
	else $asc = false;
	
	$format = 'm/d/y';

	$p = 1;
	for ($from=0;$from<$total;$from+=$num){
		$to = $from + $num - 1;
		if ($to>$total) $to = $total;
		echo '<option value="'.$from.'" '.($start==$from?'SELECTED':'').'>';
		if ($asc) echo ($p++).': '.date($format,$index[$from]).'&nbsp;-&gt;&nbsp;'.date($format,$index[$to])."\n";
		else echo ($p++).': '.date($format,$index[$to]).'&nbsp;-&gt;&nbsp;'.date($format,$index[$from])."\n";
	}
}

function main_showPageSubjectOptions($total, $num, $start, &$index){
	$p = 1;
	for ($from=0;$from<$total;$from+=$num){
		$to = $from + $num - 1;
		if ($to>$total) $to = $total;
		echo '<option value="'.$from.'" '.($start==$from?'SELECTED':'').'>';
		echo ($p++).': '.substr($index[$from],0,15)."\n";
	}
}

function main_showPageOptions($total, $num, $start){
	global $index_a;
	global $sort_field,$sort_order;
	global $thread_view;
	
	//return plain old numerical page menu
	return main_showPageOptionsDefault($total, $num, $start);
	
	/*
		Fancy page menus have been disabled for now, because
		indexing only occurs in certain limited cases.
		See main_fetch_index() for more why indexing no longer
		occurs.
	*/
	
	if (!is_array($index_a))
		return main_showPageOptionsDefault($total, $num, $start);	
	
	if ($thread_view)
		return main_showPageOptionsDefault($total, $num, $start);
	
	$n = 0;
	foreach($index_a as $mid=>$val){
		$index[$n++] = $val;
	}
	
	if (stristr($sort_field,"date")!==false)
		main_showPageDateOptions($total, $num, $start, $index);
	else if (strcasecmp($sort_field,'subject')==0)
		main_showPageSubjectOptions($total, $num, $start, $index);
	else
		main_showPageOptionsDefault($total, $num, $start);
}

function main_showPageOptionsDefault($total, $num, $start){
	global $my_prefs;
	
	if ($my_prefs["js_usage"]=="h"){
		echo "<script>\n";
		echo "D([3,".js_print_array(array((int)$total,(int)$num,(int)$start))."]);\n";
		echo "</script>\n";
	}else{
		$c=0;
		while ($c < $total){
			$c2=($c + $num);
			if ($c2 > $total) $c2=$total;
			echo "<option value=".$c.($c==$start?" SELECTED":"").">".($c+1)."-".$c2."\n";
			$c = $c + $num;
		}
	}
}

function main_options($options, $default=FALSE, $default_val=""){
	global $my_prefs;
	
	if (!is_array($options)) echo "main_options got a non-array!\n";
	
	if ($my_prefs["js_usage"]=="h"){
		$default = ($default?1:0);
		if (is_string($default_val)) $default_val="\"".$default_val."\"";
		echo "<script>\n";
		echo "var a=".js_print_array($options, true).";\n";
		echo 'D([4, a]);'."\n";
		echo 'D([5]);'."\n";
		//$a = array($def, $default_val, "a");
		//echo "D([2,[$default,$default_val,a]]);\n";
		echo "parent.flush_buffer();\n";
		echo "</script>\n";
	}else{
		reset($options);
		while(list($key,$val)=each($options)){
			echo "<option value=\"$key\" ";
			if ($default && $key==$default_val) echo "SELECTED";
			echo ">$val\n";
		}
	}
}


function main_getThreadCache($folder, $total_num, $sort_field, $sort_order){
	global $loginID, $host;

	//read what we have...
	$data = cache_read($loginID, $host, $folder.'.THREADS');
	if (!is_array($data)) return false;
	
	if (!isset($data['meta'])||!is_array($data['meta'])) return false;
	
	if ($data['meta']['tn']!=$total_num) return false;
	if ($data['meta']['sf']!=$sort_field) return false;
	if ($data['meta']['so']!=$sort_order) return false;
	if (!is_array($data['t'])) return false;
	if (!is_array($data['i'])) return false;
	
	return $data;
}

function main_fetchThreads($folder, &$index_a, $total_num, $sort_field, $sort_order){
	global $loginID, $host, $conn, $clock;
	global $read_cache;
	
	if ($total_num==0) return false;
	
	/* try cache */
	$clock->register('pre tree');
	$data = main_getThreadCache($folder, $total_num, $sort_field, $sort_order);
	if (is_array($data)){
		$clock->register('read cached tree');
		return $data;
	}
	
	/* check index */
	$clock->register('pre index check');
	if (!is_array($index_a) || count($index_a)!=$total_num){
		//try cached index
		$index_a = main_ReadCache($folder, '1:'.$total_num, $sort_field, $read_cache);
		$clock->register('read indexed cache');
		
		//if that fails, try fetching directly
		if (!is_array($index_a)){
			$index_a = iil_C_FetchHeaderIndex($conn, $folder, "1:".$total_num, $sort_field);
			$clock->register('re-fetched index');
		}
		
		//if we got it, sort it
		if ($index_a){
			if (strcasecmp($sort_order, "ASC")==0) asort($index_a);
			else if (strcasecmp($sort_order, "DESC")==0) arsort($index_a);
			$clock->register('sorted index');
		}else{
			echo 'main_fetchThreads: Index failed (cache and direct).  This is bad.';
			return false;
		}
	}
	$clock->register('post index check');
	
	/* initialize data */
	$data = array('meta'=>array('tn'=>$total_num,'sf'=>$sort_field,'so'=>$sort_order));

	/* refresh data */
	$t_tree = iil_C_BuildThreads2($conn, $folder, "1:".$total_num, $clock);
	$clock->register('built tree ('.count($t_tree).')');
	$t_tree = iil_SortThreads($t_tree, $index_a, $sort_order);
	$t_index = iil_IndexThreads($t_tree);
	$clock->register('sorted tree ('.count($t_index).')');
	
	$data['t'] = $t_tree;
	$data['i'] = $t_index;
	$data['u'] = iil_C_FetchUIDs($conn, $folder);
	
	/* write to cache */
	cache_write($loginID, $host, $folder.'.THREADS', $data);
	
	return $data;
}


function main_SortThreadHeaders(&$headers, &$ids){
	//sometimes iil_C_FetchHeaders returns headers out of order
	//since we know the order we want (i.e. in $thread below)
	//use that to sort
	
	$result = array();
	foreach($ids as $id) $result[$id] = $headers[$id];
	return $result;
}


function main_fetchThreadHeaders(&$conn, $folder, $t_tree, $start, $next_start){
	global $clock;
	
	$debug=false;
	if (!is_array($t_tree)) return false;
	
	echo $start.' to '.$next_start."\n";
	
	$i = 0;
	$headers = array();
	$t_num_kids = array();
	foreach($t_tree as $n=>$thread){
		if ($n==='') continue;
		if (($i < $start) || ($i >= $next_start)){
			$i++;
			continue;
		}
		
		ksort($thread);
		$thread_headers = iil_C_FetchHeaders($conn, $folder, implode(',',$thread));
		$thread_headers = main_SortThreadHeaders($thread_headers, $thread);
		if ($debug){
			echo "Fetching: ".implode(',',$thread)."\n";
			print_r($thread_headers);
		}
		if (is_array($thread_headers)){
			$c = count($thread_headers);
			list($root,$blah)=each($thread_headers);
			$t_num_kids[$root] = $c-1;
			while(list($j,$blah)=each($thread_headers)){
				$thread_headers[$j]->is_reply = 1;
				if (!$thread_headers[$j]->seen) $thread_headers[$root]->seen = false;
			}
			$headers = array_merge($headers, $thread_headers);
		}

		$i++;
	}
	$clock->register('mFTH: fetched '.count($headers).' headers');
	return array('h'=>$headers,'k'=>$t_num_kids);
}

function main_fetchHeaders(&$conn, $folder, $messages_str, $p){
	global $clock;
	
	$sort_field = $p['sf'];
	$sort_order = $p['so'];
	$start = $p['st'];
	$next_start = $p['nxs'];
	$index_failed = $p['if'];
	$total_num = $p['tn'];
	
	$headers=iil_C_FetchHeaders($conn, $folder, $messages_str);
	$headers=iil_SortHeaders($headers, $sort_field, $sort_order);  //if not from index array	
	
	if ($clock) $clock->register("post headers");
		
	/* if indexing failed, we need to get messages within range */
	if ($index_failed && count($headers)>$total_num){
		$i = 0;
		$new_header_a=array();
		reset($headers);
		while ( list($k, $h) = each($headers) ){
			if (($i >= $start) && ($i < $next_start)){
				$new_header_a[$k] = $headers[$k];
			}
			$i++;
		}
		$headers = $new_header_a;
	}
	return $headers;
}

function main_get_submit(){
	global $js_action, $move_selected, $delete_selected;
	global $empty_trash, $mark, $mark_as, $mark_read, $mark_unread;
	global $submit;
	
	if ($js_action) return $js_action;
	else if ($move_selected) return "File";
	else if ($delete_selected) return "Delete";
	else if ($empty_trash) return "Expunge";
	else if ($mark) return $mark_as;
	else if ($mark_read) return "Read";
	else if ($mark_unread) return "Unread";
	else return $submit;
}

function main_fetch_folderlist(&$folderlist){
	global $ICL_CAPABILITY;
	global $loginID, $host;
	global $my_prefs;
	global $conn;

	if (!$ICL_CAPABILITY["folders"]) return false;
	if (is_array($folderlist)) return true;
	
	$cached_folders = cache_read($loginID, $host, "folders");
	if (is_array($cached_folders)){
		$folderlist = $cached_folders;
		return true;
	}else{
		if ($my_prefs["hideUnsubscribed"]) $folderlist = iil_C_ListSubscribed($conn, $my_prefs["rootdir"], "*");
		else $folderlist = iil_C_ListMailboxes($conn, $my_prefs["rootdir"], "*");
		$cache_result = cache_write($loginID, $host, "folders", $folderlist);
	}
	return is_array($folderlist);
}

function main_check_criteria(&$params){
	global $searchErrors;
	
	$field = $params['field'];
	$string = $params['string'];
	$date_operand = $params['date_operand'];
	$params['date'] = $params['month'].'/'.$params['day'].'/'.$params['year'];
	$date = $params['date'];
	
	// check criteria
	if ($date_operand=="ignore"){
		if ($field=="-") $error=$searchErrors["field"];
		if (empty($string)) $error=$searchErrors["empty"];
	}else if ((empty($date))||($date=="mm/dd/yyyy")){
		$error=$searchErrors["date"];
	}
	if (!empty($date)){
		$date_a=explode("/", $date);
		$params['date']=iil_FormatSearchDate($date_a[0], $date_a[1], $date_a[2]);
	}
	
	echo '<! main_check_criteria error: '.$error.' //-->';

	return $error;
}

function main_form_searchstr(&$search_criteria, $params){
	$field = $params['field'];
	$string = $params['string'];
	$date_operand = $params['date_operand'];
	$date = $params['date'];
	
	// format search string
	if (empty($search_criteria)){
		$criteria="ALL";
		if ($field!="-") $criteria.=" $field \"$string\"";
		if ($date_operand!="ignore") $criteria.=" $date_operand $date";
		$search_criteria = $criteria;
	}else{
		$search_criteria = stripslashes($search_criteria);
		$criteria = $search_criteria;
	}
	
	return $criteria;
}

function main_do_search(&$conn, $folder, $criteria, &$total_num){
	// search
	$messages_a=iil_C_Search($conn, $folder, $criteria);
	if ($messages_a!==false){
		$total_num=count($messages_a);
		$messages_str=implode(",", $messages_a);

		$original_search_results = $messages_str;
	}else{
		$total_num = 0;
		$messages_str = "";
		echo "Error: ".$conn->error."<br>\n"; flush();
	}
	return $messages_str;
}

function main_handle_search(&$conn, $folder, &$search_criteria, $srchp){
	global $my_prefs;
	global $MAIN_CONTEXT;
	global $searchErrors;
	global $original_search_results;
	
	include("../lang/".$my_prefs["lang"]."search_errors.inc");
	$criteria="";
	$error="";

	if ($search_criteria && $MAIN_CONTEXT["i"]){
		//results were cached
		$total_num = $MAIN_CONTEXT['n'];
		$messages_str = $MAIN_CONTEXT["i"];
		return array($total_num, $messages_str, '');
	}
	
	if (empty($search_criteria)){
		//if not using stored search_criteria, check submitted search criteria
		$error = main_check_criteria($srchp);
		if ($error) return array(0,'',$error);
	}
			
	// format search string
	$criteria = main_form_searchstr($search_criteria, $srchp);
	echo '<!-- Searching "'.$criteria.'" in '.$folder." //-->\n";
		
	// actually do search 
	$messages_str = $original_search_results = main_do_search($conn, $folder, $criteria, $total_num);
	
	
	return array($total_num, $messages_str, '');
}

function main_contextInit(&$OLD_CONTEXT){
	global $loginID, $host, $folder, $my_prefs, $cmc;
	global $sort_field, $sort_order, $start, $thread;
	global $do_quick_search, $quick_search_str;
	global $search_criteria;

	//initialize main context
	$MAIN_CONTEXT = cache_read($loginID, $host, "main.ctx");
	if (!is_array($MAIN_CONTEXT) || count($MAIN_CONTEXT)<3 || $cmc==1 || ($folder && $MAIN_CONTEXT["f"]!=$folder)){
		echo "<!-- Main Context reset //-->\n";
		$MAIN_CONTEXT["f"] = $folder;
		$MAIN_CONTEXT["s"] = $my_prefs["sort_field"];
		$MAIN_CONTEXT["o"] = $my_prefs["sort_order"];
		$MAIN_CONTEXT["n"] = 0;							//total number
		$MAIN_CONTEXT["c"] = false;						//search criteria
		$MAIN_CONTEXT["q"] = "";						//query cache
		$MAIN_CONTEXT["i"] = "";						//id cache
		$MAIN_CONTEXT["t"] = 0;							//start id
		$MAIN_CONTEXT['h'] = 0;							//thread view
	}else{
		$OLD_CONTEXT = $MAIN_CONTEXT;
	}

	$folder = $MAIN_CONTEXT["f"];
	$total_num = $MAIN_CONTEXT["n"];
	
	$var_map = array('s'=>'sort_field','o'=>'sort_order','t'=>'start','h'=>'thread');
	foreach($var_map as $key=>$var){
		if (isset($$var)) $MAIN_CONTEXT[$key] = $$var;
		else $$var = $MAIN_CONTEXT[$key];
	}

	if ($do_quick_search && $quick_search_str!=$MAIN_CONTEXT["q"]){
		$MAIN_CONTEXT["i"] = "";
		$MAIN_CONTEXT["c"] = false;
		$MANI_CONTEXT["q"] = "";
		if (empty($quick_search_str)) $do_quick_search = false;
	}else if ($MAIN_CONTEXT["c"]=="q"){
		$do_quick_search = 1;
		$quick_search_str = $MAIN_CONTEXT["q"];
	}
	if ($MAIN_CONTEXT["c"]=="s") $search_criteria = $MAIN_CONTEXT["q"];
	
	return $MAIN_CONTEXT;
}

function main_contextSave(&$MAIN_CONTEXT, &$OLD_CONTEXT){
	global $total_num_msgs, $quick_search_str, $do_quick_search;
	global $displayed_mids, $search_criteria, $original_search_results;
	global $messages_str, $total_num;
	global $loginID, $host;
	
	$MAIN_CONTEXT["n"] = $total_num_msgs;
	if ($do_quick_search && $quick_search_str){
		$MAIN_CONTEXT["c"] = "q";
		$MAIN_CONTEXT["q"] = $quick_search_str;
		if (is_array($displayed_mids)){
			$MAIN_CONTEXT['n'] = count($displayed_mids);
			sort($displayed_mids);
			$MAIN_CONTEXT["i"] = (is_array($displayed_mids)?implode(",",$displayed_mids):"");
		}else{
			$MAIN_CONTEXT['n'] = $total_num;
			$MAIN_CONTEXT["i"] = $messages_str;
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
	echo '<!--';
	print_r($MAIN_CONTEXT);
	echo '//-->';
	if ($mc_dirty) cache_write($loginID, $host, "main.ctx", $MAIN_CONTEXT);
}

?>