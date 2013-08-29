<?php
/////////////////////////////////////////////////////////
//	
//	source/contacts.php
//
//	(C)Copyright 2001-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE:  source/contacts.php
	PURPOSE:
		Contacts selection popup.

********************************************************/

function FormatHeaderLink($user, $label, $color, $new_sort_field, $sort_field, $sort_order){
	if (strcasecmp($new_sort_field, $sort_field)==0){
		if (strcasecmp($sort_order, "ASC")==0) $sort_order="DESC";
		else $sort_order = "ASC";
	}
	$link = "<a href=\"contacts_popup.php?user=$user&sort_field=$new_sort_field&sort_order=$sort_order\" class=\"mainHeading\">";
	$link .= "<b>".$label."</b></a>";
	return $link;
}

function ShowRow($a, $id){
	global $grp_sort;

	echo '<tr class="lt">'."\n";
#	$toString=(!empty($a["name"])?"\"".$a["name"]."\" ":"")."<".$a["email"].">";
#	$toString=htmlspecialchars($toString);
	if (empty($a["name"])) $a["name"]="--";
	echo "<td><a href=\"javascript:ac2('$id');\">".$a["name"]."</a></td>";
	echo "<td>".$a["email"]."</td>";
	if (!$grp_sort) echo "<td>".$a["grp"]."</td>";
	echo "</tr>\n";
}

function ShowRow2($a, $id, $grp){
	if (empty($a["name"])) $a["name"]="--";
	echo ($grp?'sr':'srg').'(['.$id.', "'.$a["name"].'","'.$a["email"].'","'.$a["grp"].'"]);'."\n";
}

include("../include/stopwatch.inc");
	
$timer = new stopwatch(true);
$timer->register("start");

include("../include/super2global.inc");
include("../include/contacts_commons.inc");
include_once("../include/data_manager.inc");
if (isset($user)){
	include("../include/header_main.inc");
	include("../lang/".$my_prefs["lang"]."/contacts.inc");
	include("../lang/".$my_prefs["lang"]."/compose.inc");

	/*
	//authenticate
	include_once("../include/icl.inc");
	$conn=iil_Connect($host, $loginID, $password, $AUTH_MODE);
	if ($conn){
		iil_Close($conn);
	}else{
		echo "Authentication failed.";
		echo "</html>\n";
		exit;
	}
	*/
	
	//initialize source name
	$source_name = $DB_CONTACTS_TABLE;
	if (empty($source_name)) $source_name = "contacts";
	
	//open data manager connection
	$dm = new DataManager_obj;
	if ($dm->initialize($loginID, $host, $source_name, $backend)){
	}else{
		echo "Data Manager initialization failed:<br>\n";
		$dm->showError();
	}
		
	//initialize sort fields and order
	if (empty($sort_field)) $sort_field = "grp,name";
	if (empty($sort_order)) $sort_order = "ASC";
	if (ereg("^grp", $sort_field))  $grp_sort = true;
	else $grp_sort = false;
	
	//fetch and sort
	if (!$DISABLE_CONTACTS_SHARING) $dm->is_sharable = true;
	$contacts = $dm->sort($sort_field, $sort_order);
	$numContacts = count($contacts);
	$groups = explode(",", base64_decode(GetGroups($contacts)));

	//show error, if any
	if (!empty($error)) echo "<p>".$error."<br>\n";
	
	
	//show title heading
	echo "\n".'<table width="100%" cellpadding=2 cellspacing=0><tr class="dk">'."\n";
	echo '<td align=left valign=bottom>'."\n";
	echo '<span class="bigTitle">'.$cStrings[0].'</span>'."\n";
	echo '&nbsp;&nbsp;&nbsp;';
	echo '<span class="mainHeadingSmall">';
	echo '[<a href="javascript:close();" onClick="window.close();" class="mainHeadingSmall">'.$cStrings["close"].'</a>]';
	echo '</span>';
	echo '</td></tr></table>'."\n";


	//show instructions
	echo '<span class=mainLight>'.$cStrings["instructions"].'</span>'."\n";
	
	//show controls
	echo '<p><form method="POST" name="contactsopts" action="contacts_popup.php">'."\n";
	echo '<input type="hidden" name="user" value="'.$user.'">'."\n";
	echo '<input type="hidden" name="cc" value="'.$cc.'">'."\n";
	echo '<input type="hidden" name="bcc" value="'.$bcc.'">'."\n";
	echo '<input type="hidden" name="sort_order" value="'.$sort_order.'">'."\n";
	echo '<input type="hidden" name="sort_field" value="'.$sort_field.'">'."\n";
	echo '<table width="100%"><tr>'."\n";
	echo '<td valign="top"><span class=mainLight>'."\n";
		$select_str = '<select name="to_a_field">'."\n";
		$select_str.= '<option value="to">'.$composeHStrings[2].":\n";
		if ($cc) $select_str.= '<option value="cc">'.$composeHStrings[3].":\n";
		if ($bcc) $select_str.= '<option value="bcc">'.$composeHStrings[4].":\n";
		$select_str.= "</select>\n";
		echo str_replace("%s", $select_str, $cStrings["addto"]);
	echo "</span></td>\n";
	echo '<td valign="top"><span class=mainLight>'."\n";
		$select_str = '<select name="show_grp" onChange="contactsopts.submit()">'."\n";
		$select_str.= '<option value="" '.(empty($show_grp)?'SELECTED':'').'>'.$cStrings["all"]."\n";
		while ( list($k,$val)=each($groups) ) $select_str.= '<option value="'.$val.'" '.($show_grp==$val?'SELECTED':'').">$val\n";
		$select_str.= "</select>\n";
		echo str_replace("%s", $select_str, $cStrings["showgrp"]);
	echo "</span></td>\n";
	echo "</tr></table>\n";
	echo "</form>\n";
	flush();

	//show contacts
	if ( is_array($contacts) && count($contacts) > 0){

		reset($contacts);
		$num_c=0;
		echo '<table width="100%" border="0" cellspacing="1" cellpadding="1" class="md">'."\n";
		echo '<tr class="dk">';
		echo "<td>".FormatHeaderLink($user, $cStrings[3], $textc, "name", $sort_field, $sort_order)."</td>\n";
		echo "<td>".FormatHeaderLink($user, $cStrings[4], $textc, "email", $sort_field, $sort_order)."</td>\n";
		if (!$grp_sort) echo "<td>".FormatHeaderLink($user, $cStrings[6], $textc, "grp,name", $sort_field, $sort_order)."</td>\n";
		echo "</tr>\n";
		if (!$grp_sort) echo "\n".'<script>'."\n";
		$prev_grp = false;
		$num_c = 0;
		while( list($k1, $foobar) = each($contacts) ){
			$a=$contacts[$k1];
			if (empty($show_grp) || $show_grp==$a["grp"]){
				if ($grp_sort && $a["grp"]!==$prev_grp){
					//$grp = str_replace(" ", "_", $a["grp"]);
					if ($prev_grp) echo 'fb();'."\n".'</script>'."\n";
					if ( !empty($a["grp"]) ){
					   $toString = htmlspecialchars($a["grp"]);
					   echo '<tr class="lt"><td colspan=2 align=center><br><b>';
					   echo "<a href=\"javascript:addgroup('$toString');\">".$a["grp"]."</a>";
					   echo '</b></td></tr>';
					}
					echo "\n".'<script>'."\n";
					$prev_grp = $a["grp"];
					if (!$prev_grp) $prev_grp = true;
				}
				if ($a["email"]){
					ShowRow2($a, $num_c,$grp_sort); $num_c++;
				}
				if ($a["email2"]){
					$a["email"] = $a["email2"];
					ShowRow2($a, $num_c,$grp_sort); $num_c++;
				}
			}
		}
		echo 'fb();'."\n".'</script>'."\n";
		echo "</table>\n";
	}else{
		echo "<p>".$cErrors[0];
	}
}
?>
</BODY></HTML>
<!--
<?php
$timer->register("done");
$timer->dump();
?>
//-->
