<?php
/////////////////////////////////////////////////////////
//	
//	source/compose2.php
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
	FILE:  source/compose.php
	PURPOSE:
		1.  Provide interface for creating messages
		2.  Provide interface for uploading attachments
		3.  Form MIME format (RFC822) compliant messages
		4.  Send message
		5.  Save to "sent items" folder if so specified
	PRE-CONDITIONS:
		$user - Session ID for session validation and user preference retreaval
	POST-CONDITIONS:
		Displays standard message composition interface by default
		If "upload" button pressed, displays all inputted text and attachment info
		If "send" button pressed, sends, files, and displays status
	COMMENTS:
	
********************************************************/


include("../include/super2global.inc");
include("../include/header_main.inc");
include("../lang/".$my_prefs["lang"]."compose.inc");
include("../lang/".$my_prefs["lang"]."dates.inc");
include("../include/icl.inc");
include("../include/version.inc");
include("../include/mod_base64.inc");
include("../conf/defaults.inc");
include("../include/stopwatch.inc");
include_once("../include/fs_path.inc");
include_once('../include/cache.inc');
if ($GPG_ENABLE){
	include_once("../include/gpg.inc");
}

$timer = new stopwatch(true);
$timer->register("start");

function RemoveDoubleAddresses($to) {
	$to_adr = iil_ExplodeQuotedString(",", $to);
	$adresses = array();
	$contacts = array();
	foreach($to_adr as $addr) {
		$addr = trim($addr);
		if (preg_match("/(.*<)?.*?([^\s\"\']+@[^\s>\"\']+)/", $addr, $email)) {
			$email = strtolower($email[2]);
			if (!in_array($email, $adresses)) {						//New adres
				array_push($adresses, $email);
				$contacts[$email] = $addr;
			} elseif (strlen($contacts[$email])<strlen($addr)) {				//Adres already in list and name is longer
				$contacts[$email] = trim($addr);
			}
		}
	}
	return implode(", ",$contacts);
}

function ResolveContactsGroup($str){
	global $contacts;
	
	$tokens = explode(" ", $str);
	if (!is_array($tokens)) return $str;
	
	while ( list($k,$token)=each($tokens) ){
		if (ereg("@contacts.group", $token)){
			if (ereg("^<", $token)) $token = substr($token, 1);
			list($group, $junk) = explode("@contacts.", $token);
			$group = base64_decode($group);
			$newstr = "";
			reset($contacts);
			while ( list($blah, $contact)=each($contacts) ){
				if ($contact["grp"]==$group && !empty($contact["email"])){
					$newstr.= (!empty($newstr)?", ":"");
					$newstr.= "\"".$contact["name"]."\" <".$contact["email"].">";
				}
			}
			if (ereg(",$", $token)) $newstr.= ",";
			$tokens[$k] = $newstr;
			if (ereg(str_replace(" ", "_", $group), $tokens[$k-1])) $tokens[$k-1] = "";
		}
	}
	
	return implode(" ", $tokens);
}


if (ini_get('file_uploads')!=1){
	echo "Error:  Make sure the 'file_uploads' directive is enabled (set to 'On' or '1') in your php.ini file";
}



/******* Init values *******/
if (!isset($attachments)) $attachments=0;
if (isset($change_contacts)) $show_contacts = $new_show_contacts;
if (isset($change_show_cc)) $show_cc = $new_show_cc;

//read alternate identities
include_once("../include/data_manager.inc");
$ident_dm = new DataManager_obj;
if ($ident_dm->initialize($loginID, $host, $DB_IDENTITIES_TABLE, $DB_TYPE)){
	$alt_identities = $ident_dm->read();
}

//Handle ddresses submitted from contacts list 
//(in contacts window)
if (is_array($contact_to)) $to .= (empty($to)?"":", ").urldecode(implode(", ", $contact_to));
if (is_array($contact_cc)) $cc .= (empty($cc)?"":", ").urldecode(implode(", ", $contact_cc));
if (is_array($contact_bcc)) $bcc .= (empty($bcc)?"":", ").urldecode(implode(", ", $contact_bcc));
//(in compose window)
if ((isset($to_a)) && (is_array($to_a))){
    reset($to_a);
    while ( list($key, $val) = each($to_a)) $$to_a_field .= ($$to_a_field!=""?", ":"").stripslashes($val);
}

//generate authenticated email address
if (empty($init_from_address)){
	$sender_addr = $loginID.( strpos($loginID, "@")>0 ? "":"@".$host );
}else{
	$sender_addr = str_replace("%u", $loginID, str_replace("%h", $host, $init_from_address));
}

//generate user's name
$from_name = $my_prefs["user_name"];
$from_name = LangEncodeSubject($from_name, $my_charset);
if ((!empty($from_name)) && (count(explode(" ", $from_name)) > 1)) $from_name = "\"".$from_name."\"";

if ($TRUST_USER_ADDRESS){
    //Honor User Address
    //If email address is specified in prefs, use that in the "From"
    //field, and set the Sender field to an authenticated address
    $from_addr = (empty($my_prefs["email_address"]) ? $sender_addr : $my_prefs["email_address"] );
    $from = $from_name." <".$from_addr.">";
    $reply_to = "";
}else{
    //Default
    //Set "From" to authenticated user address
    //Set "Reply-To" to user specified address (if any)
	$from_addr = $sender_addr;
    $from = $from_name." <".$sender_addr.">";
    if (!empty($my_prefs["email_address"])) $reply_to = $from_name." <".$my_prefs["email_address"].">";
    else $reply_to = "";
}
$original_from = $from;

echo "\n<!-- FROM: $original_from //-->\n";


//resolve groups added from contacts selector
$to_has_group = $cc_has_group = $bcc_has_group = false;
if (!empty($to)) $to_has_group = ereg("@contacts.group", $to);
if (!empty($cc)) $cc_has_group = ereg("@contacts.group", $cc);
if (!empty($bcc)) $bcc_has_group = ereg("@contacts.group", $bcc);
if ($to_has_group || $cc_has_group || $bcc_has_group){
	$dm = new DataManager_obj;
	if ($dm->initialize($loginID, $host, $DB_CONTACTS_TABLE, $DB_TYPE)){
		if (empty($sort_field)) $sort_field = "grp,name";
		if (empty($sort_order)) $sort_order = "ASC";
		$contacts = $dm->sort($sort_field, $sort_order);
		
		if ($to_has_group) $to = ResolveContactsGroup($to);
		if ($cc_has_group) $cc = ResolveContactsGroup($cc);
		if ($bcc_has_group) $bcc = ResolveContactsGroup($bcc);
	}
}



/***
	CHECK UPLOADS DIR
***/
$uploadDir = fs_get_path("upload", $loginID, $host);
if (!$uploadDir || !file_exists(realpath($uploadDir))) $error .= "Uploads dir not found: \"$uploadDir\"<br>";


/****
	SEND
****/
function cmp_send(){}
if (isset($save)) $send1=true;
if (isset($send1) || isset($send2)){
	include("../include/compose_send_main.inc");
}


/****
	HANDLE UPLOADED FILE
****/
function upload(){}
if (isset($upload)){
	if (($userfile)&&($userfile!="none")){
		$i=$attachments;
		$newfile = $user.".".mod_base64_encode($userfile_name).".".mod_base64_encode($userfile_type).".".mod_base64_encode($userfile_size);
		$newpath=$uploadDir."/".$newfile;
		if (@move_uploaded_file($userfile, $newpath)){
			$attach[$newfile] = 1;
		}else{
			echo $userfile_name." : ".$composeErrors[3];
		}
	}else{
		echo $composeErrors[4];
	}
}


/****
	FETCH LIST OF UPLOADED FILES
****/
function fetchUploads(){}
if (file_exists(realpath($uploadDir))){
	//open directory
	if ($handle = opendir($uploadDir)) {
		//loop through files
		while (false !== ($file = readdir($handle))) {
			if ($file != "." && $file != "..") {
				//split up file name
				$file_parts = explode(".", $file);
				
				//make sure first part is session ID, and add to list
				if ((strcmp($file_parts[0], $user)==0)||(strpos($file_parts[0], "fwd-")!==false))
					$uploaded_files[] = $file;
			} 
		}
		closedir($handle); 
	}
}
if (is_array($fwd_att_list)){
	reset($fwd_att_list);
	while ( list($file, $v) = each($fwd_att_list) ){
		$uploaded_files[] = $file;
	}
}


/****
	REPLYING OR FORWARDING
****/
if ((isset($replyto)) || (isset($forward)) || (isset($draft))){
	include("../include/compose_fetch_ref.inc");
}else{
	$message = stripslashes($message);
}



?>

<FORM NAME="messageform" ENCTYPE="multipart/form-data" ACTION="compose2.php" METHOD="POST" onSubmit='DeselectAdresses(); close_popup(); return true;'>
	<input type="hidden" name="user" value="<?php echo $user?>">
	<input type="hidden" name="show_contacts" value="<?php echo $show_contacts?>">
	<input type="hidden" name="show_cc" value="<?php echo $show_cc?>">
	<input type="hidden" name="override_cs" value="<?php echo $override_cs ?>">
	<?php
        if ($no_subject) echo '<input type="hidden" name="confirm_no_subject" value="1">';
    
		if (($replyto) || ($in_reply_to) || ($draft)){
			if (empty($in_reply_to)) $in_reply_to = $folder.":".$uid;
			echo "<input type=\"hidden\" name=\"in_reply_to\" value=\"$in_reply_to\">\n";
			echo "<input type=\"hidden\" name=\"replyto_messageID\" value=\"$replyto_messageID\">\n";
		}else if (($forward) || ($forward_of)){
			if (empty($forward_of)) $forward_of = $folder.":".$uid;
			echo "<input type=\"hidden\" name=\"forward_of\" value=\"$forward_of\">\n";
		}
		if ($draft){
			echo '<input type="hidden" name="is_draft" value="1">'."\n";
		}
		
		if (is_array($fwd_att_list)){
			reset($fwd_att_list);
			while ( list($file,$v) = each($fwd_att_list)){
				echo "<input type=\"hidden\" name=\"fwd_att_list[".$file."]\" value=\"1\">\n";
			}
		}
		if (!empty($folder)){
			echo '<input type="hidden" name="folder" value="'.$folder.'">';
		}
	?>
	
	<table border="0" width="100%" class="dk">
	<tr>
		<td valign="bottom" align="left">
			<span class="bigTitle"><?php echo $composeStrings[0]; ?></span>
			&nbsp;&nbsp;&nbsp;
			<span class="mainHeadingSmall">
			<?php
			if (!$my_prefs["compose_inside"]){
				$jsclose="[<a href=\"javascript:window.close();\" class=\"mainHeadingSmall\">".$composeStrings[11]."</a>]";
				echo "<SCRIPT type=\"text/javascript\" language=\"JavaScript1.2\">\n";
				echo "document.write('$jsclose');\n</SCRIPT>";
			}
			?>
			</span>
		</td>
		<td valign="bottom" align="right">
						
		</td>
	</tr>
	</table>
    <?php
    	if (!empty($error)) echo '<br><span class="error">'.$error.'</span>';
		$to = encodeUTFSafeHTML($to);
		$cc = encodeUTFSafeHTML($cc);
		$bcc = encodeUTFSafeHTML($bcc);
		
		// format sender's email address (i.e. "from" string)
        	$email_address = htmlspecialchars($original_from);

		// External table
		echo "<table border=0><tr valign=\"top\"><td>\n";
		
		echo "<table border=0>";
		echo "<tr>";
		echo "<td align=right class=\"mainLight\">".$composeHStrings[0].":</td><td class=\"mainLight\">";
		echo '<input type=text name="subject" value="'.encodeUTFSafeHTML(stripslashes($subject)).'" size="60" onKeyUp="fixtitle(\''.$composeStrings["title"].'\');"></td>';
		echo "</td></tr>\n";
		
		//display from field
		echo "<tr>";
		echo "<td align=right class=\"mainLight\">".$composeHStrings[1].":</td><td class=\"mainLight\">";
		if (($alt_identities) && (count($alt_identities)>1)){
			echo '<!-- original_recipients: '.$original_recipients.' //-->';
			echo "<select name=\"sender_identity_id\">\n";
			foreach($alt_identities as $key=>$ident_a){
				$is_def_id=false;
				if ($key==$sender_identity_id) $is_def_id=true;
				else if (!$sender_identity_id){
					//if replying, try to find identity the original message was sent to
					if ($replyto || $forward){
						if (stristr($original_recipients, $ident_a['email'])!==false){
							$sender_identity_id = $ident_a['id'];
							$is_def_id = true;
						}
					}
					//if still not it, check for default
					if (!$is_def_id){
						$is_def_id = ($ident_a['name']==$my_prefs['user_name'] && $ident_a['email']==$my_prefs['email_address']);
						$is_def_id = ($is_def_id || $ident_a['id']==$my_prefs['ident_id']);
					}
						
				}
				echo '<option value="'.$key.'" '.($is_def_id?'SELECTED':'').'>';
				echo '"'.$ident_a['name'].'"&nbsp;&nbsp;&lt;'.$ident_a['email'].'&gt;'."\n";
			}
			echo "</select>\n";
		}else{
			echo LangDecodeSubject($email_address, $my_prefs["charset"]);
		}

		//show "bcc self" checkbox
		if (($send1||$send2||$upload)&&empty($bcc_self)) $bcc_self = false;		//if submitted and disabled, leave it so
		if (!isset($bcc_self)&&$my_prefs['bcc_self']) $bcc_self = true;	//not set, set to default
		echo '<input type="checkbox" value="1" name="bcc_self" '.($bcc_self?'CHECKED':'').' id=bcc_self>';
		echo '<label for=bcc_self>'.$composeStrings[19].'</label>';
		echo "</td></tr>\n";
  
		//if (($show_contacts) || ($my_prefs["showContacts"])){
		if ($show_contacts){
			echo "<tr>\n<td align=right valign=top>";
			echo "<select name=\"to_a_field\">\n";
			echo "<option value=\"to\">".$composeHStrings[2].":\n";
			echo "<option value=\"cc\">".$composeHStrings[3].":\n";
			echo "<option value=\"bcc\">".$composeHStrings[4].":\n";
			echo "</select>\n";
			echo"</td><td>";
		
			// display "select" box with contacts
			include_once("../include/data_manager.inc");
			$source_name = $DB_CONTACTS_TABLE;
			if (empty($source_name)) $source_name = "contacts";
			$dm = new DataManager_obj;
			if ($dm->initialize($loginID, $host, $source_name, $DB_TYPE)){
				if (empty($sort_field)) $sort_field = "grp,name";
				if (empty($sort_order)) $sort_order = "ASC";
				$contacts = $dm->sort($sort_field, $sort_order);
			}else{
				echo "Data Manager initialization failed:<br>\n";
				$dm->showError();
			}

			if ((is_array($contacts)) && (count($contacts) > 0)){
				echo "<select name=\"to_a[]\" MULTIPLE SIZE=7 onDblClick='CopyAdresses(); return true;'>\n";
				while ( list($key, $foobar) = each($contacts) ){
					$contact = $contacts[$key];
					if (!empty($contact["email"])){
						$line = "\"".$contact["name"]."\" <".$contact["email"].">";
						echo "<option>".htmlspecialchars($line)."\n";
					}
					if (!empty($contact["email2"])){
						$line = "\"".$contact["name"]."\" <".$contact["email2"].">";
						echo "<option>".htmlspecialchars($line)."\n";
					}
				}
				echo "</select>"; 
				
				echo "<script type=\"text/javascript\" language=\"JavaScript1.2\">";
				echo "document.write('<input type=\"button\" name=\"add_contacts\" value=\"".$composeStrings[8]."\" onClick=\"CopyAdresses()\">');\n";
				echo "</script>\n";
				echo "<noscript><input type=\"submit\" name=\"add_contacts\" value=\"".$composeStrings[8]."\"><br></noscript>\n";
				if ($my_prefs["showContacts"]!=1){
					echo "<input type=\"hidden\" name=\"new_show_contacts\" value=0>\n";
					echo "<input type=\"submit\" name=\"change_contacts\" value=\"".$composeStrings[6]."\">\n";
				}
			}
			echo "</td></tr>\n";
			$contacts_shown = true;
		}else{
			$contacts_shown = false;
		}
		
		// display to field
		echo "<tr>\n<td align=right class=\"mainLight\">".$composeHStrings[2].":</td><td>";
		if (strlen($to) < 60)
            echo "<input type=text name=\"to\" value=\"".stripslashes($to)."\" size=60>";
        else
            echo "<textarea name=\"to\" cols=\"60\" rows=\"3\">".stripslashes($to)."</textarea>";
		if (!$contacts_shown){
			//"show contacts" button
			echo "<input type=\"hidden\" name=\"new_show_contacts\" value=1>\n";
			$popup_url = "contacts_popup.php?user=$user";
			$showcon_link = "<a href=\"javascript:open_popup('$popup_url')\" class=\"mainLight\">";
			$showcon_link.= "<img src=\"themes/".$my_prefs["theme"]."/images/addc.gif\">".$composeStrings[5]."</a>";
			$showcon_link = addslashes($showcon_link);
			echo "<script type=\"text/javascript\" language=\"JavaScript1.2\">\n";
			echo "document.write('$showcon_link');\n";
			echo "</script>\n";
			echo "<noscript>\n<input type=\"submit\" name=\"change_contacts\" value=\"".$composeStrings[5]."\">\n</noscript>\n";
			//echo "<input type=\"submit\" name=\"change_contacts\" value=\"".$composeStrings[5]."\">\n";
		}
		echo "</td></tr>\n";
		
		if ((!empty($cc)) || ($my_prefs["showCC"]==1) || ($show_cc)){
			// display cc box
			echo "<tr>\n<td align=right class=\"mainLight\">".$composeHStrings[3].":</td><td>";
        	if (strlen($cc) < 60)
            	echo "<input type=text name=\"cc\" value=\"".stripslashes($cc)."\" size=60>";
        	else
            	echo "<textarea name=\"cc\" cols=\"60\" rows=\"3\">".stripslashes($cc)."</textarea>";
			echo "</td></tr>\n";
			
			$cc_field_shown = true;
		}else{
			$cc_field_shown = false;
		}
		
		if ((!empty($bcc)) || ($my_prefs["showCC"]==1) || ($show_cc)){
			// display bcc box
			echo "<tr>\n<td align=right class=\"mainLight\">".$composeHStrings[4].":</td><td>";
        	if (strlen($bcc) < 60)
            	echo "<input type=text name=\"bcc\" value=\"".stripslashes($bcc)."\" size=60>";
			else
            	echo "<textarea name=\"bcc\" cols=\"60\" rows=\"3\">".stripslashes($bcc)."</textarea>\n";
			echo "</td></tr>\n";
			$bcc_field_shown = true;
		}else{
			$bcc_field_shown = false;
		}

		//show attachments
		echo "<tr>";
		echo "<td align=\"right\" valign=\"top\" class=\"mainLight\">".$composeStrings[4].":</td>";
		echo "<td valign=\"top\">";
		if ((is_array($uploaded_files)) && (count($uploaded_files)>0)){
			//echo "<table>";
			echo '<table border="0" cellspacing="1" cellpadding="1" class="dk">'."\n";
			reset($uploaded_files);
			while ( list($k,$file) = each($uploaded_files) ){
				$file_parts = explode(".", $file);
				echo '<tr class="lt">';
				echo "<td valign=\"bottom\"><input type=\"checkbox\" name=\"attach[$file]\" value=\"1\" ".($attach[$file]==1?"CHECKED":"")."></td>";
				echo "<td valign=\"bottom\">".mod_base64_decode($file_parts[1])."&nbsp;</td>";
				echo "<td valign=\"bottom\" class=\"small\">".mod_base64_decode($file_parts[3])."bytes&nbsp;</td>";
				echo "<td valign=\"bottom\" class=\"small\">(".mod_base64_decode($file_parts[2]).")</td>";
				echo "</td></tr>\n";
			}
			echo "</table>";
		}
		
		if ($MAX_UPLOAD_SIZE) $max_file_size = $MAX_UPLOAD_SIZE;
		else $max_file_size = ini_get('upload_max_filesize');
		if (eregi("M$", $max_file_size)) $max_file_size = (int)$max_file_size * 1000000;
		else if (eregi("K$", $max_file_size)) $max_file_size = (int)$max_file_size * 1000;
		echo '<input type="hidden" name="MAX_FILE_SIZE" value="'.$max_file_size.'">';
		echo "<INPUT NAME=\"userfile\" TYPE=\"file\">";
		echo '<INPUT TYPE="submit" NAME="upload" VALUE="'.$composeStrings[2].'">';
		echo "</td></tr>\n";
		
		echo "<tr>\n<td></td><td>";
		
		if ((!$cc_field_shown) || (!$bcc_field_shown)){
			//"show cc/bcc field" button
			include("../lang/".$my_prefs["lang"]."prefs.inc");
			echo '<input type="hidden" name="new_show_cc" value="1">';
			echo '<input type="submit" name="change_show_cc" value="'.$prefsStrings["6.2"].'">';
		}else{
			echo '<input type="hidden" name="new_show_cc" value="'.$show_cc.'">';
		}
		echo "</td></tr>\n";

        echo "</table>";

	// External table
	echo "</td><td>\n";
	echo '<input type="submit" name="send1" value="'.$composeStrings[1].'">';
	if ($my_prefs['draft_box_name']) echo '<input type="submit" name="save" value="'.$composeStrings['save'].'">';
	echo "</td></tr><td colspan=2>\n";
	
	/***
		SPELL CHECK
	****/
	if ($check_spelling){
		include("../include/spellcheck.inc");

		//run spell check
		$result = splchk_check($message, $spell_dict_lang);
		
		//handle results
		if ($result){
			echo '<table><tr class="lt"><td>'."\n";
			$words = $result["words"];
			$positions = $result["pos"];
			if (count($positions)>0){
				//show errors and possible corrections
				echo "<b>".$composeStrings[15]."</b><br>\n";
				echo str_replace("%s", $DICTIONARIES[$spell_dict_lang], $composeErrors[8]);

				$splstr["ignore"] = $composeStrings[17];
				$splstr["delete"] = $composeStrings[18];
				$splstr["correct"] = $composeStrings[13];
				$splstr["nochange"] = $composeStrings[14];
				$splstr["formname"] = "messageform";
			
				splchk_showform($positions, $words, $splstr);
			}else{
				//show "no changes needed"
				echo $composeErrors[6].str_replace("%s", $DICTIONARIES[$spell_dict_lang], $composeErrors[8]);
			}
			echo "</td></tr></table>\n";
			
		}else{
			echo $composeErrors[7];
		}
	}else if ($correct_spelling){
		//correct spelling
		include("../include/spellcheck.inc");

		//do some shifting here...
		while (list($num,$word)=each($words)){
			$correct_var = "correct".$num;
			$correct[$num] = $$correct_var;
		}
		
		echo '<table><tr class="lt"><td>'."\n";
		echo "<b>".$composeStrings[16]."</b><br>\n";

		//do the actual corrections
		$message = splchk_correct($message, $words, $offsets, $suggestions, $correct);

		echo "</td></tr></table>\n";
	}
	
	/***
		SHOW TEXT BOX
	***/
	?>
	<br><span class="mainLight"><?php echo $composeStrings[7]?></span><br>
	<TEXTAREA NAME=message <?php echo "ROWS=".$my_prefs["textarea_height"]." COLS=".$my_prefs["textarea_width"] ?> WRAP=virtual><?php echo "\n".encodeUTFSafeHTML($message); ?></TEXTAREA>
	
	<?php
		//spell check controls
		if (is_array($DICTIONARIES) && count($DICTIONARIES)>0){
			echo "<br><select name='spell_dict_lang'>\n";
			reset($DICTIONARIES);
			while ( list($l,$n)=each($DICTIONARIES) ) echo "<option value='$l'>$n\n";
			echo "</select>\n";
			echo '<input type="submit" name="check_spelling" value="'.$composeStrings[12].'">';
		}
	?>

	<!-- External talbe -->
	</td><tr><td>
	
	
	<table border=0 width="100%">
	<?php
	//GPG stuff
	if ($GPG_ENABLE){
	?>
		<tr>
			<span class="mainLight">Whose public key to use? (this feature is still experimental)<br>
			<?php
				$keys = gpg_list_keys();
				$options = "";
				if (is_array($keys) && count($keys)>0){
					while (list($k,$str)=each($keys)){
						$options.= "<option value=\"$k\">$str\n";
					}
				}
			?>
			<select name="keytouse">
			<option value = "noencode">None</option>
			<?php echo $options ?>
			</select>
			</span>
		</tr>
	<?php
	} //end if $GPG_ENABLE
	?>
	<tr>
		<td>
		<input type=checkbox name="attach_sig" value=1 <?php  echo ($my_prefs["show_sig1"]==1?"CHECKED":""); ?> > 
		<span class="mainLight"><?php  echo $composeStrings[3]; ?></span>
		</td>
	</tr>
	</table>

	</td><td>
		<input type="submit" name="send2" value="<?php  echo  $composeStrings[1]; ?>">
		<?php
		if ($my_prefs['draft_box_name']){
			echo '<input type="submit" name="save" value="'.$composeStrings['save'].'">';
		}
		?>
	</td></tr></table><!-- End External table -->

</form>
	<script type=text/javascript>
		var _p = this.parent;
		if (_p==this){
			_p.document.title = "<?php echo $composeStrings["title"] ?>";
		}
	</script>
</BODY></HTML>
<!--
<?php
$timer->register("stop");
$timer->dump();
?>
//-->