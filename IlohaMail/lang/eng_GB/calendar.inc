<?php
/********************************************************
	lang/gb/calendar.inc
	(C)2003 Ryo Chijiiwa <Ryo@IlohaMail.org>
	(C)2003 Lovell Fuller <iloha@lovell.info>
	(C)2005 Andrew Alderwick <andrew@alderwick.co.uk>

	This file is part of IlohaMail, and released under GPL.
	See COPYING, or http://www.fsf.org/copyleft/gpl.html

********************************************************/

//TRANSLATORS:  Uncomment next line for all non-English languages!!
include("../lang/eng/calendar.inc");

$calStr["add_schedule"] = "Add Appointment";
$calStr["edit_schedule"] = "Edit Appointment";
$calStr["weekof"] = "Week beginning %d";
$calStr["colors"] = array("#990000"=>"Dark Red", "#FF0000"=>"Red", "#000099"=>"Deep Blue", "#0000FF"=>"Blue", 
					"#006600"=>"Dark Green", "#00FF00"=>"Green", "#9900FF"=>"Purple", "#00FFFF"=>"Cyan",
					"#FF6600"=>"Orange", "#FFFF00"=>"Yellow", "#FF00FF"=>"Magenta", ""=>"No colour");
$calStr['no schedules'] = '<p>No appointments have been entered for this week.  <br>Click on the dates above to enter new appointments.';
?>