<?php
/////////////////////////////////////////////////////////
//	
//	include/javascript.inc
//
//	(C)Copyright 2002 Ryo Chijiiwa <Ryo@IlohaMail.org>
//
//		This file is part of IlohaMail.
//		IlohaMail is free software released under the GPL 
//		license.  See enclosed file COPYING for details,
//		or see http://www.fsf.org/copyleft/gpl.html
//
/////////////////////////////////////////////////////////

/********************************************************

	AUTHOR: Ryo Chijiiwa <ryo@ilohamail.org>, Jeroen van der Vegt <jeroen@ilohamail.org>
	FILE:  include/javascript.inc
	PURPOSE:
		Display JavaScript (included from header_main.inc)
	COMMENTS:
		Assume that no global vars are available at this
		stage.  Anything that requires embedded PHP vars
		should be passed as JS parameters.
	
********************************************************/

echo "\n<!-- $current_page //-->\n";


if (strpos($current_page, "main.php")!==false){
?>
		<SCRIPT type="text/javascript" language="JavaScript1.2">
		function SelectAllMessages(val) {
		    for (var i = 0; i < document.messages.elements.length; i++) {
				if(document.messages.elements[i].type == 'checkbox') {
					document.messages.elements[i].checked = !(document.messages.elements[i].checked);
					document.messages.elements[i].checked = val;
				}
		    }
		}
		
		if (parent.init){
			try{
				//var p_start = top.getMS();
				parent.initEvents(window, "main");
				D=function(d){parent.P(window,d)};
				T=function(n,m){parent.thread_toggler(window,n,m)};
				C=function(n,m){parent.thread_check(window,n,m)};
				CR=function(e,n){parent.clickedRow(window, e, n)};
				DCR=function(e,n){Parent.dblclickedRow(window,e,n)};
			}catch(e){
				alert("Exception calling parent.InitEvents: "+e.name+" - "+e.message);
			}
		}else{
			D=function(){};
		}
		</SCRIPT>

<?php
}else if (strpos($current_page, "compose2.php")!==false){
?>
		<script type="text/javascript" language="JavaScript1.2">
		var contacts_popup_visible=false;
		var contacts_popup;
		function CopyAdresses() {
			switch (document.forms[0].to_a_field.selectedIndex) {
			case 1:
				var target = document.forms[0].cc;
				break;
			case 2:
				var target = document.forms[0].bcc;
				break;
			default:
				var target = document.forms[0].to;
			}
			var selbox=document.forms[0].elements['to_a[]'];
			for (var i=0; selbox.length>i; i++) {
				if ((selbox.options[i].selected == true) &&
		 		 (target.value.indexOf(selbox.options[i].text, 0)==-1)) { //A check to prevent adresses from getting listed twice.
					if (target.value != '') 
						target.value += ', ';
					target.value += selbox.options[i].text;
				}
			}
		}
		
		function DeselectAdresses() {
			var selbox = document.forms[0].elements['to_a[]'];
			if (selbox) {
				for (var i=0; selbox.length>i; i++)
					selbox.options[i].selected = false;
			}
		}
		
		function DoCloseWindow(redirect_url){
			if(parent.frames.length!=0){
				parent.list2.location=redirect_url;
			}else{
				window.close();
			}
		}		
		
		function fixtitle(title_str) {
			if (document.forms[0].subject.value=='')
				document.title=title;
			else
				document.title=title_str+": "+document.forms[0].subject.value;
		}
		
		function open_popup(comp_uri) {
			if (comp_uri) {
				if (contacts_popup_visible==false) {
					if (document.forms[0].cc) comp_uri += "&cc=1";
					if (document.forms[0].bcc) comp_uri += "&bcc=1";
					contacts_popup = window.open(comp_uri, "_blank","width=500,height=500,scrollbars=yes,resizable=yes");
					if (contacts_popup.opener == null)
					contacts_popup.opener = window;
				}
				contacts_popup.focus();
			}
			return;
		}
		
  		function close_popup(){
			if (contacts_popup_visible)
  				contacts_popup.close();
  		}

		</SCRIPT>
<?php
}else if (strpos($current_page, "contacts_popup.php")!==false){
?>
		<script type="text/javascript" language="JavaScript1.2">
		var contacts = [];
		var buffer = "";
		
		function gettarget() {
			switch (document.contactsopts.to_a_field.selectedIndex) {
			case 1:
				var target = opener.document.forms[0].cc;
				break;
			case 2:
				var target = opener.document.forms[0].bcc;
				break;
			default:
				var target = opener.document.forms[0].to;
			}
			return target;
		}

		function addcontact(address) {
			var target = gettarget();
			if (target.value.indexOf(address, 0)==-1) { //A check to prevent adresses from getting listed twice.
				if (target.value != '') target.value += ', ';
				target.value += address;
			}
		}
		
		function ac2(id) {
			addcontact("\""+contacts[id][1]+"\" <"+contacts[id][2]+">");
		}
		
		function sr(data){
			var id = data[0];
			buffer+="<tr class=\"lt\"><td><a href=\"javascript:ac2('"+id+"');\">";
			buffer+=data[1]+"</a></td><td>"+data[2]+"</td></tr>";
			contacts[id] = data;
		}
		
		function srg(data){
			var id = data[0];
			buffer+="<tr class=\"lt\"><td><a href=\"javascript:ac2('"+id+"');\">";
			buffer+=data[1]+"</a></td><td>"+data[2]+"</td>";
			buffer+="<td>"+data[3]+"</tr>";
			contacts[id] = data;
		}
		
		function fb(){
			document.write(buffer);
			buffer="";
		}
		
		function addgroup(group) {
			for (var i=0; i<contacts.length; i++) {
				if (group==contacts[i][3])
					addcontact("\""+contacts[i][1]+"\" <"+contacts[i][2]+">");
			}
		}

		function acknowledge_popup() {
			opener.contacts_popup_visible=true;
		}
		
		function alert_close() {
			opener.contacts_popup_visible=false;
		}
		</script>
<?php
}else if (strpos($current_page, "prefs.php")!==false){
?>
		<script type="text/javascript" language="JavaScript1.2">
		var colprefs_popup_visible=false;
		var colprefs_popup;

		
		function open_popup(comp_uri) {
			if (comp_uri) {
				if (colprefs_popup_visible==false) {
					colprefs_popup = window.open(comp_uri, "_blank","width=350,height=350,scrollbars=yes,resizable=yes");
					if (colprefs_popup.opener == null)
					colprefs_popup.opener = window;
				}
				colprefs_popup.focus();
			}
			return;
		}
		
  		function close_popup(){
			if (colprefs_popup_visible)
  				colprefs_popup.close();
  		}

		</SCRIPT>
<?php
}else if (strpos($current_page, "read_message.php")!==false && $my_prefs["js_usage"]=="h"){
?>
<script>
try{
	if (parent && parent.init){
		parent.initEvents(window, "read");
		D=function(d){parent.P(window,d)};
	}else{
		D=function(){};
	}
	window.focus();
}catch(e){
	alert("JS Init Exception: "+e.name+"-"+e.message);
}
function deletemsg(url) {
	if (opener){
		opener.parent.list2.location=url;
		window.close();
	}else location=url;
}
</script>
<?php
}

function js_print_val($val, $with_keys=false){
	if (is_numeric($val)) return $val;
	else if ($val=="") return "\"\"";
	else if (is_array($val)) return js_print_array($val, $with_keys);
	else return "\"".addslashes(str_replace("\n", "", $val))."\"";
}

function js_print_array($a, $with_keys=false){
	$first = TRUE;
	if (!is_array($a) || count($a)==0) return "[]";
	$out = ($with_keys?"{":"[");
	
	reset($a);
	while(list($key,$val)=each($a)){
		if ($first) $first=FALSE;
		else $out.=",";
		$out.= ($with_keys?js_print_val($key).":":"");
		$out.=js_print_val($a[$key], $with_keys);
	}
	$out.=($with_keys?"}":"]");
	
	return $out;
}

?>