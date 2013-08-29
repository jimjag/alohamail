<?php
/////////////////////////////////////////////////////////
//	
//	source/bookmarks.php
//
//	(C)Copyright 2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>
	FILE: source/bookmarks.php
	PURPOSE:
		Create/edit/delete bookmarks
	PRE-CONDITIONS:
		$user - Session ID
		
********************************************************/

	include("../include/stopwatch.inc");
	
	$timer = new stopwatch(true);
	$timer->register("start");
	
	$override_cs = 'UTF-8';
	include("../include/super2global.inc");
	include("../include/header_main.inc");
	include("../include/langs.inc");
	include("../include/icl.inc");	
	include("../lang/".$my_prefs["lang"]."bookmarks.inc");
	include("../include/data_manager.inc");   
	include('../include/bookmarks_manager.inc');

	$timer->register("included");
	
	//make sure feature is not disabled
	if ($DISABLE_BOOKMARKS){
		echo $bmError[2];
		echo "</body></html>\n";
		exit;
	}

	//open DM connection
	$bm_url = 'bookmarks.php?user='.$user;
	$news_url = 'news.php?user='.$user;
	$xml_text = '<img src="themes/'.$my_prefs['theme'].'/images/xml.gif" border="0">';
	
	$dm = new BookmarksManager_obj($bm_url, $news_url, $xml_text);
	if ($dm->initialize($loginID, $host, $DB_BOOKMARKS_TABLE, $DB_TYPE)){
	}else{
		echo "Data Manager initialization failed:<br>\n";
		$dm->showError();
	}

	//do actions
	if (!$IS_PUBLIC && ($add || $edit || $delete)){
		//do add
		if (isset($add)){
			if ($dm->addBookmark()){
				$new_name = $new_url = $new_rss = $new_grp = $new_comments = $new_private = $new_grp_other = "";
			}else{
				$error = $dm->bmerr;
			}
		}
	
		//do edit
		if (isset($edit) && ($edit_id > 0)){
			if ($dm->editBookmark($edit_id)) $edit_id = 0;
			else $error = $dm->bmerr;
		}
	
		//do delete
		if (isset($delete) && ($edit_id > 0)){
			if ($dm->delete($edit_id)) $edit_id = 0;
			else $error .= "Deletion failed<br>\n";
		}
	}
	
	//get sorted list of bookmarks
	$urls_a = $dm->getBookmarks();
	
	//get groups and form <option> list
	$groups = $dm->getGroups();
	
	$error .= $dm->error;
	
	if ($format=='js_blogroll'){
		header('Content-type: text/javascript');
		$data = $dm->createBlogroll($urls_a);
		$timer->purge();
		echo $data;
		exit;
	}
	
	//show title and error
	$new_entry = 'bookmarks.php?user='.$user.'#frm';
	if ($IS_PUBLIC) $title = str_replace('%name', $my_prefs['user_name'], $bmStrings['pubtitle']);
	else $title = $bmStrings['bookmarks'];
	?>
	<table border="0" cellspacing="2" cellpadding="0" width="100%">
		<tr class="dk"><td>
			<span class="bigTitle"><?php echo $title?></span>
			&nbsp;
			<span class="mainHeadingSmall">
			<?php if (!$IS_PUBLIC){ ?>
				[<a href="<?php echo $new_entry ?>" class="mainHeadingSmall"><?php echo $bmStrings["new"] ?></a>]
			<?php } ?>		
				[<a href="news.php?user=<?php echo $user ?>" class="mainHeadingSmall">News Reader</a>]
			</span>
		</td></tr>
	</table>

	<font color="red"><?php echo $error?></font>
	<p>
	
	<?php
	echo "<center>\n";
	
	$timer->register("2");

	//list bookmark entries
	if (is_array($urls_a) && count($urls_a)>0){
		echo '<table border="0" cellspacing="1" cellpadding="2" class="md" width="95%">';
		echo $dm->showBookmarks($urls_a);
		echo "</table>";
		if (!$IS_PUBLIC && !$DISABLE_BOOKMARKS_SHARING){
			$public_url = getPublicURL($dataID);
			$public_url = '<a href="bookmarks.php?user=_'.$dataID.'" class="rmnav">'.$public_url.'</a>';
			echo '<span class="mainLight">'.str_replace('%url', $public_url, $bmStrings['public url']).'</span>';
		}
	}


	//show edit/add form
	echo '<p><a name="frm">';

	if ($edit_id>0 && !$IS_PUBLIC){
		$v = $dm->fetch_id($edit_id);
		include('../include/bookmark_editform.inc');
	}else if (!$IS_PUBLIC){
		include('../include/bookmark_addform.inc');
	}
	
?>
</BODY></HTML>
<!--
<?php
$timer->register("stop");
$timer->dump();
?>
//-->