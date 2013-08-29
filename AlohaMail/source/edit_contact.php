<?php
/////////////////////////////////////////////////////////
//
//	source/edit_contact.php
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
	FILE: source/edit_contact.php
	PURPOSE:
		Provide an interface for viewing/adding/updating contact info.
	PRE-CONDITIONS:
		$user - Session ID
		[$edit] - $id of item to modify or update (-1 means "new")
	POST-CONDITIONS:
		POST's data to contacts.php, which makes the requested changes.
	COMMENTS:

********************************************************/
include('../include/stopwatch.inc');
$timer = new stopwatch(true);
$timer->register("start");

include("../include/super2global.inc");
include("../include/header_main.inc");
include("../lang/".$my_prefs["lang"]."/contacts.inc");
include("../lang/".$my_prefs["lang"]."/edit_contact.inc");
include("../include/contacts_commons.inc");
include("../include/data_manager.inc");

//get data source name
$source_name = $DB_CONTACTS_TABLE;
if (empty($source_name)) $source_name = "contacts";

//open data manager connection
$dm = new DataManager_obj;
if ($dm->initialize($loginID, $host, $source_name, $backend)){
}else{
	echo "Data Manager initialization failed:<br>\n";
	$dm->showError();
}
if (!$DISABLE_CONTACTS_SHARING) $dm->is_sharable = true;

//get groups
if (!isset($groups)){
	$groups_a = $dm->getDistinct("grp", "ASC");
}

//if edit mode, fill in default values
if ($edit>0){
	$contact = $dm->fetch_id($edit);
	if (is_array($contact)){
		extract($contact);
		$group=$contact["grp"];
	}else{
		$error.=$cErrors[1];
		$edit=-1;
	}
}else{
	$edit=-1;
}

$timer->register("fetched");
?>

<FORM ACTION="contacts.php" METHOD=POST>
	<input type="hidden" name="user" value="<?php echo $user; ?>">
	<input type="hidden" name="delete_item" value="<?php echo $edit; ?>">
	<input type="hidden" name="edit" value="<?php echo $edit; ?>">

	<table width="100%" cellpadding=2 cellspacing=0>
           <tr class="dk">
	      <td align=left valign=bottom>
	         <span class="bigTitle"><?php echo ($edit>0?$cStrings["edit_contact"]:$cStrings["add_contact"]); ?></span>
	      </td>
           </tr>
        </table>

	<?php
		if ($error) echo '<span class="error">'.$error.'</span>';
	?>

<table cellpadding="10" cellspacing="0" border="0" bordercolor="white">
<style>
.contact {
	width: 200px;
}
</style>
	<tr>
		<td valign="top">
			<!-- row 1 left column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[3]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="name" value="<?php echo $name?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[15]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="firstname" value="<?php echo $firstname?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[16]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="lastname" value="<?php echo $lastname?>"/></td>
			</tr>
			</table>
		</td>
		<td valign="top">
			<!-- row 1 right column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[4]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="email" value="<?php echo $email?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[12]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="email2" value="<?php echo $email2?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[5]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="url" value="<?php echo $url?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[24]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="rss_url" value="<?php echo $rss_url?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[23]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="cal_url" value="<?php echo $cal_url?>"/></td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td valign="top">
			<!-- row 2 left column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[8]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="phone" value="<?php echo $phone?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[9]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="work" value="<?php echo $work?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[10]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="cell" value="<?php echo $cell?>"/></td>
			</tr>
			</table>
		</td>
		<td valign="top">
			<!-- row 2 right column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight">AIM:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="aim" value="<?php echo $aim?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight">ICQ:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="icq" value="<?php echo $icq?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight">MSN:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="msn" value="<?php echo $msn?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight">Yahoo:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="yahoo" value="<?php echo $yahoo?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight">Jabber:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="jabber" value="<?php echo $jabber?>"/></td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td valign="top">
			<!-- row 3 left column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[17]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="street" value="<?php echo $street?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight">&nbsp;</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="extended" value="<?php echo $extended?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[18]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="city" value="<?php echo $city?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[19]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="region" value="<?php echo $region?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[20]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="postalcode" value="<?php echo $postalcode?>"/></td>
			</tr>
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[21]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="country" value="<?php echo $country?>"/></td>
			</tr>
			</table>
		</td>
		<td valign="top">
			<!-- row 3 right column -->
			<table width="100%">
			<tr>
				<td align="left" class="mainLight"><?php echo $ecStrings[6]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right">
					<select class="contact" name="group">
						<option value="_otr_"><?php echo $ecStrings[14]?></option>
						<?php
							//$groups=base64_decode($groups);
							//$groups_a=explode(",", $groups);
							if (is_array($groups_a)){
								while (list($key,$val)=each($groups_a)){
									if (!empty($val)) {
										echo "<option";
										echo (strcmp($val,$group)==0 ? " SELECTED" : "");
										echo ">$val</option>\n";
									}
								}
							}
						?>
					</select>
				</td>
			</tr>
			<tr>
				<td align="left">&nbsp;</td>
				<td width="10">&nbsp;</td>
				<td align="right"><input type="text" class="contact" name="other_group" /><?php echo $other_group?></td>
			</tr>
			<tr>
				<td valign="top" align="left" class="mainLight"><?php echo $ecStrings[7]?>:</td>
				<td width="10">&nbsp;</td>
				<td align="right"><textarea class="contact" rows="5" name="comments"><?php echo $comments.(!$comments?$address:'')?></textarea></td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<?php if (!$DISABLE_CONTACTS_SHARING) { ?>
		<td align="left" class="mainLight">
			<!-- row 5 left column -->
			<input type="checkbox" name="shared" value="1" <?php echo $shared?"CHECKED":""?> > <?php echo $cStrings["share"]?>
		</td>
		<?php } else { ?>
		<td align="left" class="mainLight">
			<!-- row 5 left column -->
			&nbsp;
		</td>
		<?php } ?>
		<td align="right" class="mainLight">
		<?php 
		if (!$owner || $owner==$session_dataID){
		?>
			<!-- row 5 right column -->
			<input type="submit" name="add" value="<?php echo ($edit>0?$cStrings['edit']:$cStrings['add']); ?>" />
			<input type="submit" name="remove" value="<?php echo $ecStrings[13]?>" />
		<?php
		}
		?>
		</td>
	</tr>
</table>

     </FORM>
   </body>
</html>
<!--
<?php
$timer->register("done");
$timer->dump();
?>
//-->