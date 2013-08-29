<?php
/////////////////////////////////////////////////////////
//	
//	source/prefs.php
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
	FILE: source/prefs.php
	PURPOSE:
		Provide interface for setting general options.
		Form posts to index.php (the frame page), changes are saved to back end,
		and all frames are reloaded (so that changes apply to all frames).
	PRE-CONDITIONS:
		$user - Session ID
		
********************************************************/
include("../include/stopwatch.inc");
	
$timer = new stopwatch(true);
$timer->register("start");

	include("../include/super2global.inc");
	include("../include/header_main.inc");

	if (isset($int_lang)){
		$lang = $int_lang;
	}else{
		$lang = $my_prefs["lang"];
	}

	include("../include/langs.inc");
	include("../include/icl.inc");
		
	include("../conf/defaults.inc");
	include("../lang/".$lang."prefs.inc");

	include("../include/pref_header.inc");
    
    if ($new_user){
        echo '<br><table class="md"><tr class="lt"><td>';
        echo $prefs_new_user;
        echo '</td></tr></table>';
    }
    	
	//authenticate
	
	$conn=iil_Connect($host, $loginID, $password, $AUTH_MODE);
	if ($conn){
		if ($ICL_CAPABILITY["folders"]){
			if ($my_prefs["hideUnsubscribed"]){
				$mailboxes = iil_C_ListSubscribed($conn, $my_prefs["rootdir"], "*");
			}else{
				$mailboxes = iil_C_ListMailboxes($conn, $my_prefs["rootdir"], "*");
			}
			sort($mailboxes);
		}
		iil_Close($conn);
	}else{
		echo "Authentication failed.";
		echo "</body></html>\n";
		exit;
	}

	$lang=$my_prefs["lang"];
	$langOptions="";
	while (list($key, $val)=each($languages)) 
		$langOptions.="<option value=\"$key\"".($my_prefs["lang"]==$key?" SELECTED":"").">$val\n";
		
	if (strcasecmp($my_prefs["charset"], "euc-jp")==0) $my_prefs["charset"]="X-EUC-JP";	
	$charsetOptions="";
	while (list($key, $val)=each($charsets))
		$charsetOptions.="<option value=\"$key\"".($my_prefs["charset"]==$key?" SELECTED":"").">$val\n";
		
	//in session_auth.inc we added dst, so now remove it to get selected timezone
	$my_prefs['timezone'] = (float)$my_prefs['timezone'] - (float)$my_prefs['dst'];
	
	//timezone options
	$timezones = array();
	$timezones['-12'] = '(GMT -12:00) Eniwetok, Kwajalein';
	$timezones['-11'] = '(GMT -11:00) Midway Island, Samoa';
	$timezones['-10'] = '(GMT -10:00) Hawaii';
	$timezones['-9.5'] = '(GMT -9:30) Marquesas Islands';
	$timezones['-9'] = '(GMT -9:00) Alaska';
	$timezones['-8'] = '(GMT -8:00) Pacific Time (US/Canada)';
	$timezones['-7'] = '(GMT -7:00) Mountain Time (US/Canada)';
	$timezones['-6'] = '(GMT -6:00) Central Time (US/Canada), Mexico City';
	$timezones['-5'] = '(GMT -5:00) Eastern Time (US/Canada), Bogota, Lima';
	$timezones['-4'] = '(GMT -4:00) Atlantic Time (Canada), Caracas, La Paz';
	$timezones['-3.5'] = '(GMT -3:30) Newfoundland Standard Time';
	$timezones['-3'] = '(GMT -3:00) Brazil, Buenos Aires, Georgetown';
	$timezones['-2'] = '(GMT -2:00) Mid-Atlantic';
	$timezones['-1'] = '(GMT -1:00) Azores, Cape Verde Islands';
	$timezones['0'] = '(GMT) Western Europe, London, Lisbon, Casablanca';
	$timezones['1'] = '(GMT +1:00) Central European Time';
	$timezones['2'] = '(GMT +2:00) EET: Kaliningrad, South Africa';
	$timezones['3'] = '(GMT +3:00) Baghdad, Kuwait, Riyadh, Moscow, Nairobi';
	$timezones['3.5'] = '(GMT +3:30) Tehran';
	$timezones['4'] = '(GMT +4:00) Abu Dhabi, Muscat, Baku, Tbilisi';
	$timezones['4.5'] = '(GMT +4:30) Kabul';
	$timezones['5'] = '(GMT +5:00) Ekaterinburg, Islamabad, Karachi';
	$timezones['5.5'] = '(GMT +5:30) Dehli';
	$timezones['5.75'] = '(GMT +5:45) Nepal';
	$timezones['6'] = '(GMT +6:00) Almaty, Dhaka, Colombo';
	$timezones['6.5'] = '(GMT +6:30) Cocos Islands, Myanmar';
	$timezones['7'] = '(GMT +7:00) Bangkok, Hanoi, Jakarta';
	$timezones['8'] = '(GMT +8:00) Beijing, Perth, Singapore, Taipei';
	$timezones['9'] = '(GMT +9:00) Tokyo, Seoul, Yakutsk';
	$timezones['9.5'] = '(GMT +9:30) Australian Central Standard Time';
	$timezones['10'] = '(GMT +10:00) EAST/AEST: Guam, Vladivostok';
	$timezones['10.5'] = '(GMT +10:30) Lord Howe Island';
	$timezones['11'] = '(GMT +11:00) Magadan, Solomon Islands';
	$timezones['11.5'] = '(GMT +11:30) Norfolk Island';
	$timezones['12'] = '(GMT +12:00) Auckland, Wellington, Kamchatka';
	$timezones['13'] = '(GMT +13:00) Tonga, Pheonix Islands';
	$timezones['14'] = '(GMT +14:00) Kiribati';
	$tzOptions='';
	foreach($timezones as $offset=>$label)
		$tzOptions.='<option value="'.$offset.'" '.((float)$offset==(float)$my_prefs['timezone']?'selected':'').'>'.$label."\n";
	?>
		<font color="red"><?php echo $error?></font>
		<form method="post" action="index.php" target="_parent" onSubmit='close_popup(); return true;'>
		<input type="hidden" name="user" value="<?php echo $user?>">
		<input type="hidden" name="session" value="<?php echo $user?>">
		<table width="100%">
			<tr valign=top>
			<td width="50%">
                <table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["0.0"]?></b></span></td></tr>
                <tr class="lt"><td>
				<?php echo $prefsStrings["0.1"]?>
					<?php echo ($my_prefs["user_name"]?$my_prefs["user_name"]:$prefsStrings["0.4"])?>
					<input type=hidden name="user_name" value="<?php echo $my_prefs["user_name"]; ?>">
				<br><?php echo $prefsStrings["0.2"]?>
					<?php echo ($my_prefs["email_address"]?$my_prefs["email_address"]:$prefsStrings["0.4"])?>
					<input type=hidden name="email_address" value="<?php echo $my_prefs["email_address"]; ?>">
					<input type="hidden" name="identity_id" vlaue="<?php echo $my_prefs['ident_id'] ?>">
				<p><font size="-1">
				<?php
				$ident_link = "<a href=\"pref_identities.php?user=$user\">".$prefHeaderStrings[3]."</a>";
				echo str_replace("%s", $ident_link, $prefsStrings["0.3"]);
				?>
				</font>
				</td></tr></table>
			</td>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["1.0"]?></b></span></td></tr>
                <tr class="lt"><td>
                    <?php echo $prefsStrings["1.1"]?><select name="int_lang"><?php echo $langOptions; ?></select>
                    <br><?php echo $prefsStrings["1.2"]?><select name="charset"><?php echo $charsetOptions; ?></select>
                    <br><?php echo $prefsStrings["1.3"]?>
					<br><select name="timezone" class=small><?php echo $tzOptions; ?></select>
					<br><input type="checkbox" name="dst" value="1" <?php echo ($my_prefs['dst']?'checked':'') ?>>
						<?php echo $prefsStrings['1.8'] ?>
					<br><?php echo $prefsStrings["1.4"]?>
					<select name="clock_system">
					<option value="12" <?php echo ($my_prefs["clock_system"]==12?"SELECTED":"")?>><?php echo $prefsStrings["1.5"][12]?>
					<option value="24" <?php echo ($my_prefs["clock_system"]==24?"SELECTED":"")?>><?php echo $prefsStrings["1.5"][24]?>
					</select>
					<br><?php echo $prefsStrings["1.6"]?>
					<select name="week_start">
					<option value="0" <?php echo ($my_prefs["week_start"]==0?"SELECTED":"")?>><?php echo $prefsStrings["1.7"][0]?>
					<option value="1" <?php echo ($my_prefs["week_start"]==1?"SELECTED":"")?>><?php echo $prefsStrings["1.7"][1]?>
					</select>
				</td></tr></table>
			</td>
			</tr>
			<tr valign=top>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["2.0"]?></b></span></td></tr>
                <tr class="lt"><td>

				<?php echo $prefsStrings["2.1"]?><input type=text name="view_max" value="<?php echo $my_prefs["view_max"]; ?>" size=3>
					<?php echo $prefsStrings["2.2"]?>
				<!--
				<p><input type=checkbox name="show_size" value=1 <?php echo ($my_prefs["show_size"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["2.3"]?>
				//-->
				<p><?php echo $prefsStrings["2.13"]?>
					<?php
						$popup_url = "pref_columns.php?user=$user";
						$link_html = "<a href=\"javascript:open_popup('$popup_url')\">".$prefsStrings["2.14"]."</a>";
						$link_html = addslashes($link_html);
						echo "<script type=\"text/javascript\" language=\"JavaScript1.2\">\n";
						echo "document.write('$link_html');\n";
						echo "</script>\n";
						echo "<noscript>\n<a href=\"$popup_url\" target=_blank>".$prefsStrings["2.14"]."\n</noscript>\n";

					?>
					<!--
					<a href="main.php?user=<?php echo $user?>&folder=INBOX&MOVE_FIELDS=1" target=_blank>
					<?php echo $prefsStrings["2.14"]?></a>
					//-->
				<p><?php echo $prefsStrings["2.4"]?><select name="sort_field">
					<?php
					DefaultOptions($sort_fields, $my_prefs["sort_field"]);
					?>
					</select><?php echo $prefsStrings["2.5"]?>
				<br><?php echo $prefsStrings["2.6"]?><select name="sort_order">
					<?php
					DefaultOptions($sort_orders, $my_prefs["sort_order"]);
					?>
					</select><?php echo $prefsStrings["2.7"]?>

				<?php
				if ($ICL_CAPABILITY['threads']){
					echo '<br><label>';
					echo '<input type=checkbox name="thread_view" value=1 '.($my_prefs['thread_view']==1?'CHECKED':'').'>';
					echo $prefsStrings['2.16'].'</label>';
				}
				
				$main_tool_options = "\n<select name=\"main_toolbar\">\n";
				while ( list($k,$v)=each($prefsStrings["2.12"]) ) 
					$main_tool_options .= "<option value=\"$k\" ".($k==$my_prefs["main_toolbar"]?"SELECTED":"").">$v\n";
				$main_tool_options.= "</select>\n";
				$main_tool_str = str_replace("%m", $main_tool_options, $prefsStrings["2.11"]);
				echo "<p>".$main_tool_str."\n";
				?>
				<br><label><input type=checkbox name="show_qsbar" value=1 <?php echo ($my_prefs["show_qsbar"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["2.15"]?></label>

				<?php
				$refresh_int_str = "<input type=\"text\" name=\"radar_interval\" value=\"".$my_prefs["radar_interval"]."\" size=4>";
                $prefsStrings["2.10"] = str_replace("%n", $refresh_int_str, $prefsStrings["2.10"]);
				echo "<p>".$prefsStrings["2.10"]."\n";
				?>
				<input type="hidden" name="main_cols" value="<?php echo $my_prefs["main_cols"]?>">
				</td></tr></table>

			</td>
			<td width="50%">
                <?php
                if ($ICL_CAPABILITY["folders"]){
                ?>
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["3.0"]?></b></span></td></tr>
                <tr class="lt"><td>

				<label><input type=checkbox name="save_sent" value=1 <?php echo ($my_prefs['save_sent']==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["3.1"]?></label>
				<br><?php echo $prefsStrings["3.2"]?>
					<select name="sent_box_name">
					<option value="">
					<?php
						FolderOptions2($mailboxes, $my_prefs['sent_box_name']);
					?>
					</select><?php echo $prefsStrings["3.3"]?>
				<p><?php echo $prefsStrings["3.5"]?>
					<select name="trash_name">
					<option value="">
					<?php
						FolderOptions2($mailboxes, $my_prefs['trash_name']);
					?>
					</select><?php echo $prefsStrings["3.6"]?>
				<br>&nbsp;&nbsp;&nbsp;&nbsp;
					<label><input type=checkbox name="delete_trash" value=1 <?php echo ($my_prefs["delete_trash"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["3.4"]?></label>
				<p><?php echo $prefsStrings["3.12"]?>:
					<select name="draft_box_name">
					<option value="">
					<?php
						FolderOptions2($mailboxes, $my_prefs['draft_box_name']);
					?>
					</select>
				<p><?php echo $prefsStrings["3.7"]?>
					<select name="rootdir">
					<option value="">
					<option value="-"><?php echo $prefsStrings["3.8"]?>
					<?php
						FolderOptions2($mailboxes, $my_prefs["rootdir"]);
					?>
					</select>
					<br><?php echo $prefsStrings["3.8"]?>:<input type="text" name="rootdir_other" value="<?php echo $rootdir_other?>">

				</td></tr></table>
                <?php
                }
                ?>
			</td>
			</tr>
			<tr valign=top>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["4.0"]?></b></span></td></tr>
                <tr class="lt"><td>
				

				<label><input type=checkbox name="view_inside" value=1 <?php echo ($my_prefs["view_inside"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.1"]?></label>
				<br><label><input type=checkbox name="showNav" value=1 <?php echo ($my_prefs["showNav"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.7"]?></label>
				<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<label><input type=checkbox name="nav_no_flag" value=1 <?php echo ($my_prefs["nav_no_flag"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.8"]?></label>
				<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<?php 
					echo str_replace(
									'%menu', 
									LangDefaultMenuAssoc(
										$prefsStrings['4.10'], 
										$my_prefs['action_target'], 
										'action_target'
										), 
									$prefsStrings['4.9']
									); 
				?>

				<p><label><input type=checkbox name="html_in_frame" value=1 <?php echo ($my_prefs["html_in_frame"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.5"]?></label>
				<br><label><input type=checkbox name="show_images_inline" value=1 <?php echo ($my_prefs["show_images_inline"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.6"]?></label>

				<p><label><input type=checkbox name="colorize_quotes" value=1 <?php echo ($my_prefs["colorize_quotes"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.2"]?></label>
				<br><label><input type=checkbox name="detect_links" value=1 <?php echo ($my_prefs["detect_links"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["4.4"]?></label>
					<!-- <br><span class="small"><?php echo $prefsStrings["4.3"]?></span> //-->
				</td></tr></table>
			</td>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
				<?php
				$key = ($ICL_CAPABILITY["folders"]?"7.0.0":"7.0.1");
				?>
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings[$key]?></b></span></td></tr>
                <tr class="lt"><td>


				<p>
				<?php
				$quota_options = "\n<select name=\"show_quota\">\n";
				while ( list($k,$v)=each($prefsStrings["3.11"]) ) 
					$quota_options .= "<option value=\"$k\" ".($k==$my_prefs["show_quota"]?"SELECTED":"").">$v\n";
				$quota_options.= "</select>\n";
				$quota_str = str_replace("%m", $quota_options, $prefsStrings["3.10"]);
				echo "<br>".$quota_str."\n";
				?>

                <?php
                if ($ICL_CAPABILITY["folders"]){
					$refresh_int_str = "<input type=\"text\" name=\"folderlist_interval\" value=\"".$my_prefs["folderlist_interval"]."\" size=4>";
                	$prefsStrings["7.2"] = str_replace("%n", $refresh_int_str, $prefsStrings["7.2"]);
				?>
				<p><label><input type=checkbox name="hideUnsubscribed" value=1 <?php echo ($my_prefs["hideUnsubscribed"]==1?"CHECKED":""); ?>><?php echo $prefsStrings["3.9"]?></label>
				<p><label><input type=checkbox name="list_folders" value=1 <?php echo ($my_prefs["list_folders"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["2.8"]?></label>
				<br>&nbsp;&nbsp;&nbsp;<?php echo $prefsStrings["2.9"]?><input type="text"name="folderlistWidth" value=<?php echo $my_prefs["folderlistWidth"]; ?> size=4>
				<br>&nbsp;&nbsp;&nbsp;<label><input type=checkbox name="showNumUnread" value=1 <?php echo ($my_prefs["showNumUnread"]==1?"CHECKED":""); ?>><?php echo $prefsStrings["7.1"]?></label>
				<br>&nbsp;&nbsp;&nbsp;<label><input type=checkbox name="refresh_folderlist" value=1 <?php echo ($my_prefs["refresh_folderlist"]==1?"CHECKED":""); ?>><?php echo $prefsStrings["7.2"]?></label>
                <?php
                }
                ?>

				</td></tr></table>
			</td>
			</tr>
			<tr valign=top>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings["6.0"]?></b></span></td></tr>
                <tr class="lt"><td>
				
				<label><input type=checkbox name="compose_inside" value=1 <?php echo ($my_prefs["compose_inside"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["6.4"]?></label>
				<!--
				<p><input type=checkbox name="showContacts" value=1 <?php echo ($my_prefs["showContacts"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["6.1"]?>
				//-->
				<p><label><input type=checkbox name="showCC" value=1 <?php echo ($my_prefs["showCC"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["6.2"]?></label>
				<br><label><input type=checkbox name="closeAfterSend" value=1 <?php echo ($my_prefs["closeAfterSend"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["6.3"]?></label>
				<br><label><input type=checkbox name="show_sig1" value=1 <?php echo ($my_prefs["show_sig1"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["5.1"]?></label>
				<br><label><input type=checkbox name="bcc_self" value=1 <?php echo ($my_prefs["bcc_self"]==1?"CHECKED":""); ?>>
					<?php echo $prefsStrings["6.6"]?></label>

				<p><?php echo $prefsStrings["6.5"] ?> 
				<input type="text" name="textarea_width" value="<?php echo $my_prefs["textarea_width"]?>" size=3> x 
				<input type="text" name="textarea_height" value="<?php echo $my_prefs["textarea_height"]?>" size=3> 
				</td></tr></table>
			</td>
			<td width="50%">
				<table border="0" cellspacing="1" cellpadding="1" class="md" width="100%">
                <tr class="dk"><td><span class="tblheader"><b><?php echo $prefsStrings['8.0']; ?></b></span></td></tr>
                <tr class="lt"><td>
				<?php echo $prefsStrings["8.1"]; ?>: <select name="theme">
				<?php
				if ($handle = opendir("./themes")) {
					while (false !== ($file = readdir($handle))) { 
						if ($file != "." && $file != "..") {
							echo "<option value=\"$file\" ".($file==$my_prefs["theme"]?"SELECTED":"").">$file\n";
						} 
					}
					closedir($handle); 
				}
				?>
				</select>
				
				<br><?php echo $prefsStrings["8.2"]; ?>: <select name="js_usage">
				<option value=''><?php echo $prefsStrings['8.5'] ?>
				<option value="l" <?php echo ($js_mode=="l"?"SELECTED":"") ?>><?php echo $prefsStrings["8.3"]; ?>
				<option value="h" <?php echo ($js_mode=="h"?"SELECTED":"") ?>><?php echo $prefsStrings["8.4"]; ?>
				</select>
				</td></tr></table>
			</td>
			</tr>
		</table>
            <p>
			<input type="hidden" name="do_prefs" value="1">
            <!--
			<input type="submit" name="update" value="<?php echo $prefsButtonStrings[0]?>">
            -->
			<input type="submit" name="apply" value="<?php echo $prefsButtonStrings[1]?>">
			<input type="submit" name="cancel" value="<?php echo $prefsButtonStrings[2]?>">
			<input type="submit" name="revert" value="<?php echo $prefsButtonStrings[3]?>">
		</form>
</BODY></HTML>
<!--
<?php
$timer->register("done");
$timer->dump();
?>
//-->
