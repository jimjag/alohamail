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
		List basic information of all contacts.
		Offer links to
			-view/edit contact
			-send email to contact
			-add new contact
		Process posted data to edit/add/remove contacts information
	PRE-CONDITIONS:
		Required:
			$user-Session ID for session validation and user prefernce retreaval
		Optional:
			POST'd data for add/remove/edit entries.  See source/edit_contact.php
	POST-CONDITIONS:
	COMMENTS:

********************************************************/

function FormatHeaderLink($user, $label, $color, $new_sort_field, $sort_field, $sort_order){
	if (strcasecmp($new_sort_field, $sort_field)==0){
		if (strcasecmp($sort_order, "ASC")==0) $sort_order="DESC";
		else $sort_order = "ASC";
	}
	$link = "<a href=\"contacts.php?user=$user&sort_field=$new_sort_field&sort_order=$sort_order\" class=\"mainHeading\">";
	$link .= "<b>".$label."</b></a>";
	return $link;
}

include('../include/stopwatch.inc');
$timer = new stopwatch(true);
$timer->register("start");
include('../include/super2global.inc');
include('../include/contacts_commons.inc');
include_once('../include/data_manager.inc');
if (isset($user)){
	include('../include/header_main.inc');
	include('../lang/'.$my_prefs["lang"].'/contacts.inc');
	include('../lang/'.$my_prefs["lang"].'/compose.inc');

	//authenticate
	/*
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

	$timer->register("authenticated");

	echo "\n".'<table width="100%" cellpadding=2 cellspacing=0>';
	echo '<tr class="dk">'."\n";
	echo '<td align=left valign=bottom>'."\n";
	echo '<span class="bigTitle">'.$cStrings[0].'</span>'."\n";
	echo '&nbsp;&nbsp;';
	echo '<span class="mainHeadingSmall">';
	echo '[<a href="contacts_export.php?user='.$user.'" class="mainHeadingSmall">'.$cStrings["export"].'</a>]';
	echo '[<a href="contacts_import.php?user='.$user.'" class="mainHeadingSmall">'.$cStrings["import"].'</a>]';
	echo '</span>';
	echo '</td></tr></table>'."\n";

	//initialize source name
	$source_name = $DB_CONTACTS_TABLE;
	if (empty($source_name)) $source_name = "contacts";

	//open data manager connection
	$dm = new DataManager_obj;
	if ($dm->initialize($loginID, $host, $source_name, $backend)){
	}else{
		echo 'Data Manager initialization failed:<br>'."\n";
		$dm->showError();
	}

	//init datastore
	$DS->init("cntct", array());
	$grp_stat = $DS->read("cntct");

	//do add
	if (isset($add)){
		//set group if "other"
		if (strcmp($group,"_otr_")==0) $group=$other_group;

		//create data array
    	$new_contact_array = array(
				"owner" => $session_dataID,
				"name" => $name,
				"email" => $email,
				"email2" => $email2,
				"grp" => $group,
				"aim" => $aim,
				"icq" => $icq,
				"yahoo" => $yahoo,
				"msn" => $msn,
				"jabber" => $jabber,
				"phone" => $phone,
				"work" => $work,
				"cell" => $cell,
				"address" => $address,
				"url" => $url,
				'rss_url'=>$rss_url,
				'cal_url'=>$cal_url,
				"comments" => $comments,
				"firstname" => $firstname,
				"lastname" => $lastname,
				"street" => $street,
				"extended" => $extended,
				"city" => $city,
				"region" => $region,
				"postalcode" => $postalcode,
				"country" => $country,
				"shared" => $shared
    	);

		if ($edit<=0){	//if not edit (i.e. new), do an insert
			if (!$dm->insert($new_contact_array)){
				echo 'Insert failed<br>';
				$dm->showError();
			}
		}else{			//is edit, do an update
			if (!$dm->update($edit, $new_contact_array)){
				echo 'update failed<br>';
				$dm->showError();
			}
		}
	}else if (isset($delete)){	//delete entry
		$dm->delete($delete_item);
	}else if (isset($remove)){	//confirm removal of entry
		include('../lang/'.$my_prefs['lang'].'/edit_contact.inc');
		echo '<span class="error">'.$errors[6].$name.$errors[7].'</span>'."\n";
		echo '[<a href="contacts.php?user='.$sid.'&delete=1&delete_item='.$delete_item.'" class="mainLight">'.$ecStrings[13].'</a>]'."\n";
		echo '[<a href="contacts.php?user='.$sid.'" class="mainLight">'.$ecStrings[22].'</a>]'."\n";
	}

	if (isset($flip_grp)){
		$grp_stat[$flip_grp] = !$grp_stat[$flip_grp];
		$DS->write("cntct", $grp_stat);
	}

	//initialize sort fields and order
	if (empty($sort_field)) $sort_field = "grp,name";
	if (empty($sort_order)) $sort_order = "ASC";

	if (ereg("^grp", $sort_field)) $grp_sort = true;
	else $grp_sort = false;

	//fetch and sort
	if (!$DISABLE_CONTACTS_SHARING) $dm->is_sharable = true;
	$contacts = $dm->sort($sort_field, $sort_order);
	$numContacts = count($contacts);

	//show error, if any
	if (!empty($error)) echo "<p>".$error."<br>\n";
	$timer->register("fetched");

	$groups = GetGroups($contacts);
	echo '<p><a href="edit_contact.php?user='.$sid.'&edit=-1" class="mainLight">'.$cStrings[1].'</a><br>';
	echo "\n";

	//show contacts
	if ( is_array($contacts) && count($contacts) > 0){
		reset($contacts);
		$target = ($my_prefs['compose_inside']?'list2':'_blank');
        echo '<form method="POST" action="compose2.php" target="'.$target.'">'."\n";
        echo '<input type="hidden" name="user" value="'.$user.'">'."\n";
		echo '<table width="100%" border="0" cellspacing="1" cellpadding="1" class="md">'."\n";
		echo '<tr class="dk">';
		echo '<td>'.FormatHeaderLink($user, $cStrings[3], $textc, 'name', $sort_field, $sort_order).'</td>';
		echo '<td>'.FormatHeaderLink($user, $cStrings[4], $textc, 'email', $sort_field, $sort_order).'</td>';
		if (!$grp_sort) echo '<td>'.FormatHeaderLink($user, $cStrings[6], $textc, 'grp,name', $sort_field, $sort_order).'</td>';
		echo '</tr>';

		$prev_grp = "";
		while( list($k1, $foobar) = each($contacts) ){
			$a=$contacts[$k1];

			if ($grp_sort && $a["grp"]!=$prev_grp){
				//if sorting by group, show group name at top
				echo '<tr class="lt"><td colspan=2 align=center><br>';
				echo '<b>'.$a['grp'].'</b>';
				echo '[<a href="contacts.php?user='.$user.'&flip_grp='.$a["grp"].'">';
				echo ($grp_stat[$a["grp"]]?"+":"-");
				echo '</a>]';
				echo '</td></tr>';
				$prev_grp = $a['grp'];
			}

			if (!$grp_sort || !$grp_stat[$prev_grp]){
				//group collapsing thinggy
				echo '<tr class="lt">'."\n";
				$id=$a["id"];
				$toString=(!empty($a['name'])?'"'.$a['name'].'" ':'').'<'.$a['email'].'>';
				$toString=urlencode($toString);
				if (empty($a["name"])) $a["name"]="--";
				echo '<td><a href="edit_contact.php?user='.$sid.'&k='.$k1.'&edit='.$id.'">'.$a['name'].'</a></td>';
				echo '<td>';
				echo '<a href="compose2.php?user='.$sid.'&to='.$toString.'" target='.$target.'>'.$a['email'].'</a>';
				if ($a['url']) echo '&nbsp;|&nbsp;<a href="'.$a['url'].'" class="splink">Web</a>';
				if ($a['rss_url']) echo '&nbsp;|&nbsp;<a href="news.php?user='.$user.'&url='.urlencode($a['rss_url']).'" class="splink">RSS</a>';
				if ($a['cal_url']) echo '&nbsp;|&nbsp;<a href="'.$a['cal_url'].'" class="splink">Calendar</a>';
				echo '<td>';
				if (!$grp_sort) echo "<td>".$a["grp"]."</td>";
				echo '</tr>'."\n";
			}
		}
		echo '</table>'."\n";
        echo '<input type="submit" name="contacts_submit" value="'.$cStrings[10].'">'."\n";
	}else{
		echo "<p>".$cErrors[0];
	}
}
?>
</BODY></HTML>
<!--
<?php
$timer->register("stop");
$timer->dump();
?>
//-->
