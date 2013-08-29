<?php
/////////////////////////////////////////////////////////
//	
//	include/main_tools.inc
//
//	(C)Copyright 2000-2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//	This file is part of IlohaMail. IlohaMail is free software released 
//	under the GPL license.  See enclosed file COPYING for details, or 
//	see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////
/*
	This file contains the main message list tools.
	(i.e. delete, mark read/unread, move controls)
*/

			reset($main_tool_fields); //$main_tool_fields is declared in main.php
			while ( list($k,$field)=each($main_tool_fields) ){
				$field_var_name = $field."_name";
				$$field_var_name = $field.($main_tool_shown?"_2":"");
			}
			
			echo "<table width=\"100%\"><tr>";
			echo "<td>";
			if (strcmp($folder,$my_prefs["trash_name"])==0){
				echo "<input type=\"hidden\" name=\"$expunge_name\" value=\"1\">\n";
				echo "<input type=\"submit\" name=\"$empty_trash_name\" value=\"".$mainStrings[11]."\">\n";
			}else{
				echo "<input type=\"submit\" name=\"$delete_selected_name\" value=\"".$mainStrings[10]."\">\n";
			}
			echo "</td><td><span class=mainLight>\n";
			if ($ICL_CAPABILITY["flags"]){			
				$flag_buttons = '<select name="'.$mark_as_name.'">'."\n";
				$flag_buttons.= '<option value="">--'."\n";
				$flag_buttons.= '<option value="Read">'.$mainStrings[21]."\n";
				$flag_buttons.= '<option value="Unread">'.$mainStrings[22]."\n";
				$flag_buttons.= '<option value="Flagged">'.$mainStrings[26]."\n";
				$flag_buttons.= '<option value="Unflagged">'.$mainStrings[32]."\n";
				$flag_buttons.= "</select>\n";
				$flag_buttons.= "<input type=\"submit\" name=\"$mark_name\" value=\"".$mainStrings[25]."\">\n";
				$flag_group = str_replace("%b", $flag_buttons, $mainStrings[20]);
				echo $flag_group."\n";
			}
			echo "</span></td><td align=\"right\">";

			if (main_fetch_folderlist($folderlist)){
				echo "<select name=\"$moveto_name\">\n";
				echo "<option value=\"\"></option>\n";
				$options = GetRootedFolderOptions($folderlist, $defaults, $my_prefs["rootdir"]);
				main_options($options);
				echo "</select>";
				echo "<input type=submit name=\"$move_selected_name\" value=\"".$mainStrings[12]."\">";
			}
			echo "</td></tr></table>\n";
			
			$main_tool_shown = true;

?>